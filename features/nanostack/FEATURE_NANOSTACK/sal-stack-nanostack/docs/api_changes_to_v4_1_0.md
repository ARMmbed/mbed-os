# About this document

This document walks you through the API changes in the Nanostack version 4.1.x in detail and helps you to update your current applications to use the new and changed APIs.

**New APIs:**

- Load balancing
- Whiteboard API

**Changed APIs:**

- FHSS platform API
- eDTLS API
- dev_stat API


## [**Load balancing**](17_API_load_balancer.md)

Load balancing (LB) is a new feature to allow devices to switch from a loaded network into a better one. Load balancing operates with given rules and developer(s) SHOULD NOT try to aim at evenly balanced networks (might cause churn).

To enable this feature, use the build time flag `HAVE_6LOWPAN_ND`.

The new LB functions are presented briefly here:

- `typedef bool net_load_balance_network_switch_notify(int8_t interface_id);`

  Typedef of a callback function, that you can implement to control network switching. If you do not implement this, the network switch takes place automatically when applicable.

- `void net_load_balance_network_switch_cb_set(net_load_balance_network_switch_notify *network_switch_notify);`

  A setter function to set (/unset with NULL) a callback function, which is called when the system has found a better network to join.

- `int8_t net_load_balance_create(int8_t interface_id, bool enable_periodic_beacon_interval);`

  Creates and enables the load balancing feature.

- `int8_t net_load_balance_delete(int8_t interface_id);`

  Deletes and disables the load balancing feature.

- `int8_t net_load_balance_threshold_set(int8_t interface_id, uint8_t threshold_min, uint8_t threshold_max);`

  A setter for a threshold to control when a device can change the network. The difference (diff) of networks is counted. If the diff is smaller than the minimum treshold, nothing happens. If the diff is greater than or equal with the max threshold, the default or set probability value is used. Otherwise, min and max values affect the switch probability:
  
   - If the diff is close to min -> very poor chance to switch.
   - If the diff is close to max --> almost the same probability as is set. 
   
  See `net_load_balance_set_max_probability` below for more details.

- `int8_t net_load_balance_set_max_probability(int8_t interface_id , uint8_t max_p);`

  A setter for maximum probability for a device will switch the network. The proposed value is between 10 and 25 to prevent all devices from switching network at the same time (the default value is 25). Also a block period is activated if the device fails to change the network (this is automatic). Threshold minimum and maximum will add more probability on top of this and therefore, this value can be thought only as a maximum probability for switching.

- `int8_t net_load_balance_load_level_update_enable(int8_t interface_id, uint16_t expected_device_count);`

  This needs the build time flag `HAVE_RPL`.

  <span class="notes">**Note:** Border router (BR) feature only! Enables automatic updates of the DODAG preference, which is used to measure the quality of a network. The expected device count is calculated by network designer to set the number of devices this BR should connect. This is not a hard limit, it just tells when the designed device count has been reached!</span>

- `int8_t net_load_balance_load_level_update_disable(int8_t interface_id);`

  This needs the build time flag `HAVE_RPL`.

  <span class="notes">**Note:** Border router feature only! Disables automatic updates of DODAG preference. Also disables measurement of expected device count.</span>

## Whiteboard API

Whiteboard API is used to limit the number of devices allowed to join to network.

To enable this feature, use the build time flag `WHITEBOARD`.

The limit is needed in situations where one BR goes down and the devices connected to it start to find another networks. This can cause other BRs to have out-of-memory (OOM) situations, if there are lots of connected devices. When the limit is reached, the BR will not accept any new connection(s). If set to 0, there is no limit.

`void whiteboard_set_device_hard_limit(uint16_t limit);`

## Frequency-hopping Spread Spectrum (FHSS)

### FHSS platform API

FHSS platform API was simplified in this version of Nanostack. The FHSS time measurement functions were removed and replaced with the timestamp callback.

See the instructions below to fix any conflicts:

Remove the time measure callbacks from the FHSS platform function structure and replace them with the timestamp read callback:

	static fhss_platform_functions_s fhss_functions = {
	    &fhss_timer_start,
	    &fhss_timer_stop,
	    &fhss_timer_get_remaining_slots,
	+   &fhss_timestamp_read,
	-   &fhss_time_measure_start,
	-   &fhss_time_measure_read,
	-   &fhss_time_measure_stop,
	    RESOLUTION_DIVIDER
	};
	
Remove also corresponding time measure functions from the driver as they are not needed anymore.

The new timestamp callback function must be implemented:

	uint32_t fhss_timestamp_read(void)
	{
		.
		.
		.
	    return timestamp;
	}
	
The timestamp function must return a 32-bit timestamp value with 1us resolution (if resolution divider is not used). The return value is time since the FHSS driver was started and a 32-bit counter is allowed to overflow.

## eDTLS API update

###  eDTLS connection status update and sn_edtls_exec() call guidance

The eDTLS library calls the connection status callback by `EDTLS_ECC_CALCULATING` when it starts the ECC calculation. When ECC is started, the application should call the `sn_edtls_exec()` function rapidly to trigger a new event until the connection status gives `EDTLS_ECC_CALCULATION_READY`. This status means that callbacks to `sn_edtls_exec()` can be stopped.

The `sn_edtls_exec()` function should be called periodically using one second interval with the updated timer value, which triggers the re-transmission of the eDTLS library when needed. The `sn_edtls_exec()` should also be called when the eDTLS library has parsed the handshake message successfully (`sn_edtls_parse_data` returns 0). When the eDTLS callback has status `EDTLS_CONNECTION_OK`, `EDTLS_CONNECTION_CLOSED` or `EDTLS_CONNECTION_FAILED`, the periodical timer can be stopped until a new connection is started by the `sn_edtls_connect()` function.

## dev_stats_api API update

The `dev stats_api.h` file has been removed which means that the following functions are not available anymore:

- `dev_stat_internal_init()`
- `dev_stat_get_longest_heap_sector()`
- `dev_stat_get_runtime_seconds()`

The heap stats are still available when the `ns_dyn_mem_init()` call `info_pr` is defined. (`nsdynMEMLIB.h`)

