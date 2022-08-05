/*
 * The Shadow Simulator
 * Copyright (c) 2010-2011, Rob Jansen
 * See LICENSE for licensing information
 */

/* manages the scheduling of events and hosts to threads,
 * following one of several scheduling policies */
#include <glib.h>
#include <math.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/types.h>

#include "lib/logger/logger.h"
#include "main/bindings/c/bindings.h"
#include "main/core/scheduler/scheduler.h"
#include "main/core/scheduler/scheduler_policy.h"
#include "main/core/support/config_handlers.h"
#include "main/core/support/definitions.h"
#include "main/core/work/event.h"
#include "main/core/worker.h"
#include "main/host/host.h"
#include "main/utility/count_down_latch.h"
#include "main/utility/utility.h"

static int _parallelism;
ADD_CONFIG_HANDLER(config_getParallelism, _parallelism)

struct _Scheduler {
    WorkerPool* workerPool;

    /* global lock for all threads, hold this as little as possible */
    GMutex globalLock;

    /* the serial/parallel host/thread mapping/scheduling policy */
    SchedulerPolicy* policy;

    /* we store the hosts here */
    GHashTable* hostIDToHostMap;

    /* used to randomize host-to-thread assignment */
    Random* random;

    /* auxiliary information about current running state */
    gboolean isRunning;
    SimulationTime endTime;
    struct {
        SimulationTime endTime;
        SimulationTime minNextEventTime;
    } currentRound;

    /* for memory management */
    MAGIC_DECLARE;
};

static void _scheduler_startHostsWorkerTaskFn(void* voidScheduler) {
    Scheduler* scheduler = voidScheduler;
    MAGIC_ASSERT(scheduler);
    if(scheduler->policy->getAssignedHosts) {
        GQueue* myHosts = scheduler->policy->getAssignedHosts(scheduler->policy);
        if(myHosts) {
            guint nHosts = g_queue_get_length(myHosts);
            info("starting to boot %u hosts", nHosts);
            worker_bootHosts(myHosts);
            info("%u hosts are booted", nHosts);
        }
    }
}

static void _scheduler_runEventsWorkerTaskFn(void* voidScheduler) {
    Scheduler* scheduler = voidScheduler;

    // Reset the round end time before starting the new round.
    worker_setRoundEndTime(scheduler->currentRound.endTime);

    Event* event = NULL;
    while ((event = scheduler->policy->pop(
                scheduler->policy, scheduler->currentRound.endTime)) != NULL) {
        // get the host to run this event on
        Host* host = scheduler_getHost(scheduler, event_getHostID(event));
        worker_runEvent(event, host);
    }

    // Gets the time of the event at the head of the event queue right now.
    SimulationTime minQTime = scheduler->policy->getNextTime(scheduler->policy);

    // We'll compute the global min time across all workers.
    worker_setMinEventTimeNextRound(minQTime);
}

static void _scheduler_finishTaskFn(void* voidScheduler) {
    Scheduler* scheduler = voidScheduler;
    /* free all applications before freeing any of the hosts since freeing
     * applications may cause close() to get called on sockets which needs
     * other host information. this may cause issues if the hosts are gone.
     *
     * do the following if it turns out we need each worker to free their assigned hosts.
     * i dont think it should be a problem to swap hosts between threads given our current
     * program state context switching, but am not sure about plugins that use other linked libs.
     *
     * **update** it doesnt work. for example, each instance of the tor plugin keeps track of
     * how many hosts it created, and then when that many hosts are freed, it frees openssl
     * structs. so if we let a single thread free everything, we run into issues. */

    GQueue* myHosts = NULL;
    if(scheduler->policy->getAssignedHosts) {
        myHosts = scheduler->policy->getAssignedHosts(scheduler->policy);
    }
    worker_finish(myHosts, scheduler->endTime);
}

Scheduler* scheduler_new(const Controller* controller, const ChildPidWatcher* pidWatcher,
                         const ConfigOptions* config, guint nWorkers, guint schedulerSeed,
                         SimulationTime endTime) {
    Scheduler* scheduler = g_new0(Scheduler, 1);
    MAGIC_INIT(scheduler);

    /* global lock */
    g_mutex_init(&(scheduler->globalLock));

    scheduler->workerPool =
        workerpool_new(controller, pidWatcher, scheduler, config, /*nThreads=*/nWorkers,
                       /*nParallel=*/_parallelism);

    scheduler->endTime = endTime;
    scheduler->currentRound.endTime = scheduler->endTime;// default to one single round
    scheduler->currentRound.minNextEventTime = SIMTIME_MAX;

    scheduler->hostIDToHostMap = g_hash_table_new(g_direct_hash, g_direct_equal);

    scheduler->random = random_new(schedulerSeed);

    utility_assert(nWorkers >= 1);

    /* create the configured policy to handle queues */
    scheduler->policy = schedulerpolicyhostsingle_new();
    utility_assert(scheduler->policy);

    info("main scheduler thread will operate with %u worker threads", nWorkers);

    return scheduler;
}

void scheduler_shutdown(Scheduler* scheduler) {
    MAGIC_ASSERT(scheduler);

    info("scheduler is shutting down now");

    /* this launches delete on all the plugins and should be called before
     * the engine is marked "killed" and workers are destroyed, so that
     * each plug-in is able to destroy/free its virtual nodes properly */
    g_hash_table_destroy(scheduler->hostIDToHostMap);

    info("waiting for %d worker threads to finish", workerpool_getNWorkers(scheduler->workerPool));
    workerpool_joinAll(scheduler->workerPool);
}

void scheduler_free(Scheduler* scheduler) {
    MAGIC_ASSERT(scheduler);

    /* finish cleanup of shadow objects */
    scheduler->policy->free(scheduler->policy);
    random_free(scheduler->random);

    g_mutex_clear(&(scheduler->globalLock));

    info("%d worker threads finished", workerpool_getNWorkers(scheduler->workerPool));
    workerpool_free(scheduler->workerPool);

    MAGIC_CLEAR(scheduler);
    g_free(scheduler);
}

gboolean scheduler_push(Scheduler* scheduler, Event* event, Host* sender, Host* receiver) {
    MAGIC_ASSERT(scheduler);

    SimulationTime eventTime = event_getTime(event);
    if(eventTime >= scheduler->endTime) {
        event_unref(event);
        return FALSE;
    }

    /* parties involved. sender may be NULL, receiver may not!
     * we MAY NOT OWN the receiver, so do not write to it! */
    utility_assert(receiver);

    /* push to a queue based on the policy */
    scheduler->policy->push(scheduler->policy, event, sender, receiver, scheduler->currentRound.endTime);

    // Store the minimum time of events that we are pushing between hosts. The
    // push operation may adjust the event time, so make sure we call this after
    // the push.
    worker_setMinEventTimeNextRound(event_getTime(event));

    return TRUE;
}

EmulatedTime scheduler_nextHostEventTime(Scheduler* scheduler, Host* host) {
    MAGIC_ASSERT(scheduler);

    if (!scheduler->policy->nextHostEventTime) {
        panic("scheduler_nextHostEventTime not implemented");
    }
    return scheduler->policy->nextHostEventTime(scheduler->policy, host);
}

int scheduler_addHost(Scheduler* scheduler, Host* host) {
    MAGIC_ASSERT(scheduler);

    /* this function should only be executed during the initActions phase in
     * scheduler_awaitStart, in which we are already holding the globalLock */

    /* save the host */
    GQuark hostID = host_getID(host);
    gpointer hostIDKey = GUINT_TO_POINTER(hostID);

    if (g_hash_table_contains(scheduler->hostIDToHostMap, hostIDKey)) {
        // the host ID is derived from the hostname, so duplicate host IDs means duplicate hostnames
        error("Cannot have two hosts with the same name '%s'", host_getName(host));
        return -1;
    }

    g_hash_table_replace(scheduler->hostIDToHostMap, hostIDKey, host);
    return 0;
}

Host* scheduler_getHost(Scheduler* scheduler, GQuark hostID) {
    MAGIC_ASSERT(scheduler);
    return (Host*) g_hash_table_lookup(scheduler->hostIDToHostMap, GUINT_TO_POINTER((guint)hostID));
}

static void _scheduler_appendHostToQueue(gpointer uintKey, Host* host, GQueue* allHosts) {
    g_queue_push_tail(allHosts, host);
}

static void _scheduler_shuffleQueue(Scheduler* scheduler, GQueue* queue) {
    if(queue == NULL) {
        return;
    }

    /* convert queue to array */
    guint length = g_queue_get_length(queue);
    gpointer array[length];

    for(guint i = 0; i < length; i++) {
        array[i] = g_queue_pop_head(queue);
    }

    /* we now should have moved all elements from the queue to the array */
    utility_assert(g_queue_is_empty(queue));

    /* shuffle array - Fisher-Yates shuffle */
    for(guint i = 0; i < length-1; i++) {
        gdouble randomFraction = random_nextDouble(scheduler->random);
        gdouble maxRange = (gdouble) length-i;
        guint j = (guint)floor(randomFraction * maxRange);

        gpointer temp = array[i];
        array[i] = array[i+j];
        array[i+j] = temp;
    }

    /* reload the queue with the newly shuffled ordering */
    for(guint i = 0; i < length; i++) {
        g_queue_push_tail(queue, array[i]);
    }
}

static void _scheduler_assignHostsToThread(Scheduler* scheduler, GQueue* hosts, pthread_t thread, uint maxAssignments) {
    MAGIC_ASSERT(scheduler);
    utility_assert(hosts);
    utility_assert(thread);

    guint numAssignments = 0;
    while((maxAssignments == 0 || numAssignments < maxAssignments) && !g_queue_is_empty(hosts)) {
        Host* host = (Host*) g_queue_pop_head(hosts);
        utility_assert(host);
        scheduler->policy->addHost(scheduler->policy, host, thread);
        numAssignments++;
    }
}

static void _scheduler_assignHosts(Scheduler* scheduler) {
    MAGIC_ASSERT(scheduler);

    g_mutex_lock(&scheduler->globalLock);

    /* get queue of all hosts */
    GQueue* hosts = g_queue_new();
    g_hash_table_foreach(scheduler->hostIDToHostMap, (GHFunc)_scheduler_appendHostToQueue, hosts);

    int nWorkers = workerpool_getNWorkers(scheduler->workerPool);
    /* we need to shuffle the list of hosts to make sure they are randomly assigned */
    _scheduler_shuffleQueue(scheduler, hosts);

    /* now that our host order has been randomized, assign them evenly to worker threads */
    int workeri = 0;
    while (!g_queue_is_empty(hosts)) {
        pthread_t nextThread = workerpool_getThread(scheduler->workerPool, workeri++ % nWorkers);
        _scheduler_assignHostsToThread(scheduler, hosts, nextThread, 1);
    }

    if(hosts) {
        g_queue_free(hosts);
    }
    g_mutex_unlock(&scheduler->globalLock);
}

__attribute__((unused)) static void _scheduler_rebalanceHosts(Scheduler* scheduler) {
    MAGIC_ASSERT(scheduler);
    utility_panic("Unimplemented");

    // WARNING if this is run, then all existing eventSequenceCounters
    // need to get set to the max of all existing counters to ensure order correctness

    /* get queue of all hosts */
    GQueue* hosts = g_queue_new();
    g_hash_table_foreach(scheduler->hostIDToHostMap, (GHFunc)_scheduler_appendHostToQueue, hosts);

    _scheduler_shuffleQueue(scheduler, hosts);

    /* now that our host order has been randomized, assign them evenly to worker threads */
    while(!g_queue_is_empty(hosts)) {
        Host* host = g_queue_pop_head(hosts);
        // SchedulerThreadItem* item = g_queue_pop_head(scheduler->threadItems);
        // pthread_t newThread = item->thread;

        //        TODO this needs to get uncommented/updated when migration code
        //        is added scheduler->policy->migrateHost(scheduler->policy,
        //        host, newThread);

        // g_queue_push_tail(scheduler->threadItems, item);
    }

    if(hosts) {
        g_queue_free(hosts);
    }
}

gboolean scheduler_isRunning(Scheduler* scheduler) {
    MAGIC_ASSERT(scheduler);
    return scheduler->isRunning;
}

void scheduler_start(Scheduler* scheduler) {
    /* Called by the scheduler thread. */

    _scheduler_assignHosts(scheduler);

    g_mutex_lock(&scheduler->globalLock);
    scheduler->isRunning = TRUE;
    g_mutex_unlock(&scheduler->globalLock);

    workerpool_startTaskFn(scheduler->workerPool,
                           _scheduler_startHostsWorkerTaskFn, scheduler);
    workerpool_awaitTaskFn(scheduler->workerPool);
}

void scheduler_continueNextRound(Scheduler* scheduler, SimulationTime windowStart, SimulationTime windowEnd) {
    /* Called by the scheduler thread. */

    g_mutex_lock(&scheduler->globalLock);
    scheduler->currentRound.endTime = windowEnd;
    scheduler->currentRound.minNextEventTime = SIMTIME_MAX;
    g_mutex_unlock(&scheduler->globalLock);

    workerpool_startTaskFn(scheduler->workerPool,
                           _scheduler_runEventsWorkerTaskFn, scheduler);
}

SimulationTime scheduler_awaitNextRound(Scheduler* scheduler) {
    /* Called by the scheduler thread. */

    // Await completion of _scheduler_runEventsWorkerTaskFn
    workerpool_awaitTaskFn(scheduler->workerPool);

    // Workers are done running the round and waiting to get woken up, so we can
    // safely read memory without a lock to compute the min next event time.
    scheduler->currentRound.minNextEventTime =
        workerpool_getGlobalNextEventTime(scheduler->workerPool);

    return scheduler->currentRound.minNextEventTime;
}

void scheduler_finish(Scheduler* scheduler) {
    /* make sure when the workers wake up they know we are done */
    g_mutex_lock(&scheduler->globalLock);
    scheduler->isRunning = FALSE;
    g_mutex_unlock(&scheduler->globalLock);

    workerpool_startTaskFn(scheduler->workerPool, _scheduler_finishTaskFn,
                           scheduler);
    workerpool_awaitTaskFn(scheduler->workerPool);

    g_mutex_lock(&scheduler->globalLock);
    if(g_hash_table_size(scheduler->hostIDToHostMap) > 0) {
        g_hash_table_remove_all(scheduler->hostIDToHostMap);
    }
    g_mutex_unlock(&scheduler->globalLock);
}
