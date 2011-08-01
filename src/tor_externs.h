#include "tor_includes.h"

extern uint8_t trusted_dir_servers[8];
extern uint8_t gzip_is_supported[4];
extern uint8_t pending_vote_list[8];
extern uint8_t num_cpuworkers_busy[4];
extern uint8_t rephist_total_num[4];
extern uint8_t v2_share_times_seconds[8];
extern uint8_t stats_n_padding_cells_processed[8];
extern uint8_t consensus_dl_status[32];
extern uint8_t called_loop_once[4];
extern uint8_t stats_prev_global_read_bucket[4];
extern uint8_t num_seen[4];
extern uint8_t global_circuitlist[8];
extern uint8_t hibernate_end_time[8];
extern uint8_t n_bytes_read_in_interval[8];
extern uint8_t can_reach_or_port[4];
extern uint8_t last_dummy_download[8];
extern uint8_t time_to_downrate_stability[8];
extern uint8_t last_warned1[8];
extern uint8_t last_warned2[8];
extern uint8_t torrc_fname[8];
extern uint8_t freelists[280];
extern uint8_t geoip_entries[8];
extern uint8_t next_virtual_addr[4];
extern uint8_t dns_wildcard_response_count[8];
extern uint8_t named_server_map[8];
extern uint8_t desc_clean_since[8];
extern uint8_t interval_wakeup_time[8];
extern uint8_t buf[48];
extern uint8_t stats_n_relay_cells_processed[8];
extern uint8_t last_desc_dumped[8];
extern uint8_t time_to_check_for_correct_dns[8];
extern uint8_t consider_republishing_rend_descriptors[4];
extern uint8_t predicted_ports_list[8];
extern uint8_t history_map[8];
extern uint8_t global_tls_context[8];
extern uint8_t start_of_entry_stats_interval[8];
extern uint8_t write_stats_to_extrainfo[4];
extern uint8_t last_rotated_x509_certificate[8];
extern uint8_t dns_is_completely_invalid[4];
extern uint8_t bootstrap_problems[4];
extern uint8_t stats_n_relay_cells_relayed[8];
extern uint8_t trusted_dir_certs[8];
extern uint8_t n_bogus_times[4];
extern uint8_t main_thread_id[8];
extern uint8_t rend_service_list[8];
extern uint8_t it_pool[8];
extern uint8_t time_to_new_circuit[8];
extern uint8_t n_sockets_open[4];
extern uint8_t shutdown_time[8];
extern uint8_t dirreq_map[24];
extern uint8_t total_bandwidth[8];
extern uint8_t CLIENT_CIPHER_DUMMIES[8];
extern uint8_t desc_needs_upload[4];
extern uint8_t auth_hid_servs[8];
extern uint8_t _last_circid_orconn_ent[8];
extern uint8_t last_sent_bootstrap_message[1024];
extern uint8_t last_routerdesc_download_attempted[8];
extern uint8_t stats_n_relay_cells_delivered[8];
extern uint8_t have_warned_about_new_version[4];
extern uint8_t cached_time_hires[16];
extern uint8_t key_lock[8];
extern uint8_t virtaddress_reversemap[8];
extern uint8_t share_seconds[4];
extern uint8_t dns_wildcard_notice_given[4];
extern uint8_t built_last_stability_doc_at[8];
extern uint8_t previous_vote_list[8];
extern uint8_t logfiles[8];
extern uint8_t stats_n_data_bytes_received[8];
extern uint8_t dns_wildcarded_test_address_list[8];
extern uint8_t time_to_download_next_consensus[8];
extern uint8_t cached_approx_time[8];
extern uint8_t log_mutex_initialized[4];
extern uint8_t time_to_check_listeners[8];
extern uint8_t v3_share_times_seconds[8];
extern uint8_t resolv_conf_fname[8];
extern uint8_t time_to_write_stats_files[8];
extern uint8_t cached_directory[8];
extern uint8_t time_to_retry_dns_init[8];
extern uint8_t ftime_slop[4];
extern uint8_t _crypto_global_initialized[4];
extern uint8_t authdir_baddir_policy[8];
extern uint8_t global_event_mask[4];
extern uint8_t the_microdesc_cache[8];
extern uint8_t stats_n_create_cells_processed[8];
extern uint8_t interval_end_time[8];
extern uint8_t sl_last_weighted_bw_of_me[8];
extern uint8_t pending_consensuses[32];
extern uint8_t lockfile[8];
extern uint8_t start_daemon_called[4];
extern uint8_t onionkey[8];
extern uint8_t time_of_last_signewnym[8];
extern uint8_t entry_guards_dirty[4];
extern uint8_t the_evdns_base[8];
extern uint8_t desc_extrainfo[8];
extern uint8_t closeable_connection_lst[8];
extern uint8_t dirport_reachability_count[4];
extern uint8_t tlsmap_root[24];
extern uint8_t ns_v2_responses[24];
extern uint8_t stats_n_bytes_written[8];
extern uint8_t rephist_total_alloc[8];
extern uint8_t onionkey_set_at[8];
extern uint8_t did_circs_fail_last_period[4];
extern uint8_t time_of_process_start[8];
extern uint8_t warned_nonexistent_family[8];
extern uint8_t the_v2_networkstatus_is_dirty[8];
extern uint8_t n_freelist_miss[8];
extern uint8_t global_options[8];
extern uint8_t guard_bandwidth_including_exits[4];
extern uint8_t should_init_bridge_stats[4];
extern uint8_t ns_v3_responses[24];
extern uint8_t reachable_dir_addr_policy[8];
extern uint8_t _escaped_val[8];
extern uint8_t global_relayed_read_bucket[4];
extern uint8_t reachable_or_addr_policy[8];
extern uint8_t guard_bandwidth_excluding_exits[4];
extern uint8_t last_read_bytes_noted[8];
extern uint8_t circuits_for_buffer_stats[8];
extern uint8_t use_unsafe_renegotiation_op[4];
extern uint8_t cached_consensuses[8];
extern uint8_t the_event_base[8];
extern uint8_t CLIENT_CIPHER_STACK[8];
extern uint8_t cell_pool[8];
extern uint8_t cfg_start_day[4];
extern uint8_t exit_bytes_written[8];
extern uint8_t sl_last_total_weighted_bw[8];
extern uint8_t dir_read_array[8];
extern uint8_t current_consensus[8];
extern uint8_t hibernate_state[4];
extern uint8_t bridge_list[8];
extern uint8_t orconn_identity_map[8];
extern uint8_t last_rotation_time[8];
extern uint8_t authentication_cookie_is_set[4];
extern uint8_t cfg_unit[4];
extern uint8_t dns_wildcard_list[8];
extern uint8_t total_exit_bandwidth[8];
extern uint8_t stable_mtbf[8];
extern uint8_t daemon_filedes[8];
extern uint8_t dns_wildcarded_test_address_notice_given[4];
extern uint8_t legacy_key_certificate[8];
extern uint8_t last_resolved_addr[4];
extern uint8_t ctr[1];
extern uint8_t time_to_save_stability[8];
extern uint8_t last_time_noted[8];
extern uint8_t networkstatus_v2_list_has_changed[4];
extern uint8_t _version[8];
extern uint8_t time_to_clean_caches[8];
extern uint8_t ftime_skew[4];
extern uint8_t socket_accounting_mutex[8];
extern uint8_t predicted_internal_uptime_time[8];
extern uint8_t entry_guards[8];
extern uint8_t last_networkstatus_download_attempted[8];
extern uint8_t interval_start_time[8];
extern uint8_t can_reach_dir_port[4];
extern uint8_t use_unsafe_renegotiation_flag[4];
extern uint8_t time_to_add_entropy[8];
extern uint8_t policy_root[24];
extern uint8_t authdir_invalid_policy[8];
extern uint8_t rend_cache[8];
extern uint8_t time_to_reset_descriptor_failures[8];
extern uint8_t stats_n_created_cells_processed[8];
extern uint8_t last_written_bytes_noted[8];
extern uint8_t stats_n_data_bytes_packaged[8];
extern uint8_t ol_length[4];
extern uint8_t launch_event[8];
extern uint8_t start_of_exit_stats_interval[8];
extern uint8_t legacy_signing_key[8];
extern uint8_t libevent_initialized[4];
extern uint8_t guard_tk[8];
extern uint8_t time_to_check_descriptor[8];
extern uint8_t time_to_write_bridge_status_file[8];
extern uint8_t global_read_bucket[4];
extern uint8_t signewnym_is_pending[4];
extern uint8_t time_to_write_bridge_stats[8];
extern uint8_t the_directory_is_dirty[8];
extern uint8_t addressmap[8];
extern uint8_t start_of_dirreq_stats_interval[8];
extern uint8_t n_bytes_written_in_interval[8];
extern uint8_t trusted_dir_servers_certs_changed[4];
extern uint8_t cached_resolve_pqueue[8];
extern uint8_t ol_tail[8];
extern uint8_t dh_param_p[8];
extern uint8_t tls_library_is_initialized[4];
extern uint8_t cfg_start_hour[4];
extern uint8_t appname[8];
extern uint8_t have_performed_bandwidth_test[4];
extern uint8_t stability_last_downrated[8];
extern uint8_t predicted_internal_capacity_time[8];
extern uint8_t n_connections_allocated[8];
extern uint8_t global_cmdline_options[8];
extern uint8_t the_directory[8];
extern uint8_t stats_n_versions_cells_processed[8];
extern uint8_t time_to_try_getting_descriptors[8];
extern uint8_t global_state[8];
extern uint8_t dh_param_g[8];
extern uint8_t stats_n_bytes_read[8];
extern uint8_t voting_schedule[64];
extern uint8_t bootstrap_percent[4];
extern uint8_t server_is_advertised[4];
extern uint8_t microdesc_consensus_methods[8];
extern uint8_t circuits_pending_or_conns[8];
extern uint8_t virtual_addr_netmask_bits[1];
extern uint8_t unnamed_server_map[8];
extern uint8_t should_add_entry_nodes[4];
extern uint8_t have_warned_about_old_version[4];
extern uint8_t dns_wildcard_one_notice_given[4];
extern uint8_t backup_argc[4];
extern uint8_t ol_list[8];
extern uint8_t fast_bandwidth[4];
extern uint8_t n_circuits_allocated[4];
extern uint8_t current_second[8];
extern uint8_t stats_prev_global_write_bucket[4];
extern uint8_t syslog_count[4];
extern uint8_t time_to_shrink_memory[8];
extern uint8_t warned_nicknames[8];
extern uint8_t orconn_circid_circuit_map[24];
extern uint8_t disable_log_messages[4];
extern uint8_t guard_wfu[8];
extern uint8_t backup_argv[8];
extern uint8_t second_timer[8];
extern uint8_t info[8];
extern uint8_t authdir_badexit_policy[8];
extern uint8_t rend_cache_v2_dir[8];
extern uint8_t pending_consensus_signature_list[8];
extern uint8_t start_of_bridge_stats_interval[8];
extern uint8_t uname_result_is_set[4];
extern uint8_t connection_array[8];
extern uint8_t v2_download_status_map[8];
extern uint8_t global_relayed_write_bucket[4];
extern uint8_t last_warned[8];
extern uint8_t exit_bytes_read[8];
extern uint8_t fingerprint_list[8];
extern uint8_t dir_write_array[8];
extern uint8_t predicted_ports_times[8];
extern uint8_t exit_streams[8];
extern uint8_t virtual_addr_network[4];
extern uint8_t finish_daemon_called[4];
extern uint8_t authority_signing_key[8];
extern uint8_t n_wildcard_requests[4];
extern uint8_t geoip_countries[8];
extern uint8_t bridge_stats_controller[8];
extern uint8_t dir_info_status[128];
extern uint8_t have_min_dir_info[4];
extern uint8_t outgoing_addrs[8];
extern uint8_t global_dirfrontpagecontents[8];
extern uint8_t cached_v2_networkstatus[8];
extern uint8_t the_v2_networkstatus[8];
extern uint8_t freelist_len[4];
extern uint8_t global_write_bucket[4];
extern uint8_t dir_policy[8];
extern uint8_t runningrouters_is_dirty[8];
extern uint8_t n_seconds_active_in_interval[4];
extern uint8_t last_hid_serv_requests[8];
extern uint8_t active_linked_connection_lst[8];
extern uint8_t write_buckets_empty_last_second[4];
extern uint8_t last_stability_doc[8];
extern uint8_t memory_lock_attempted[4];
extern uint8_t expected_bandwidth_usage[8];
extern uint8_t arrays_initialized[4];
extern uint8_t write_array[8];
extern uint8_t country_idxplus1_by_lc_code[8];
extern uint8_t freelist[8];
extern uint8_t need_to_update_have_min_dir_info[4];
extern uint8_t time_to_check_for_expired_networkstatus[8];
extern uint8_t the_runningrouters[112];
extern uint8_t authdir_reject_policy[8];
extern uint8_t bridge_stats_extrainfo[8];
extern uint8_t have_already_switched_id[4];
extern uint8_t cache_root[24];
extern uint8_t started_tracking_stability[8];
extern uint8_t num_cpuworkers[4];
extern uint8_t consensus_waiting_for_certs[64];
extern uint8_t socks_policy[8];
extern uint8_t has_completed_circuit[4];
extern uint8_t pending_consensus_signatures[8];
extern uint8_t last_time_determined_shares[8];
extern uint8_t stats_n_netinfo_cells_processed[8];
extern uint8_t uname_result[256];
extern uint8_t last_interface_ip[4];
extern uint8_t client_history[24];
extern uint8_t all_down[4];
extern uint8_t cached_runningrouters[112];
extern uint8_t next_id[8];
extern uint8_t predicted_internal_time[8];
extern uint8_t pk_op_counts[88];
extern uint8_t advertising[4];
extern uint8_t stats_n_data_cells_packaged[8];
extern uint8_t read_array[8];
extern uint8_t total_cells_allocated[4];
extern uint8_t routerlist[8];
extern uint8_t last_changed[8];
extern uint8_t stable_uptime[4];
extern uint8_t ewma_enabled[4];
extern uint8_t stats_n_data_cells_received[8];
extern uint8_t resolv_conf_mtime[8];
extern uint8_t last_guessed_ip[4];
extern uint8_t authentication_cookie[32];
extern uint8_t desc_routerinfo[8];
extern uint8_t nameserver_config_failed[4];
extern uint8_t networkstatus_v2_list[8];
extern uint8_t time_to_recheck_bandwidth[8];
extern uint8_t n_libevent_errors[4];
extern uint8_t circ_times[4104];
extern uint8_t identitykey_digest[20];
extern uint8_t n_circuit_failures[4];
extern uint8_t suppress_msg[8];
extern uint8_t authority_key_certificate[8];
extern uint8_t signal_events[128];
extern uint8_t cfg_start_min[4];
extern uint8_t buf1[65];
extern uint8_t lastonionkey[8];
extern uint8_t buf3[32];
extern uint8_t buf2[40];
extern uint8_t buf5[64];
extern uint8_t buf4[64];
extern uint8_t unit_tests[4];
extern uint8_t buf6[96];
extern uint8_t identitykey[8];
extern uint8_t time_to_check_v3_certificate[8];
extern uint8_t ewma_scale_factor[8];
extern uint8_t stats_n_destroy_cells_processed[8];
extern uint8_t enough_mtbf_info[4];
extern uint8_t nameservers_configured[4];
extern uint8_t start_of_buffer_stats_interval[8];
extern uint8_t time_to_check_ipaddress[8];
extern uint8_t stats_n_seconds_working[8];
