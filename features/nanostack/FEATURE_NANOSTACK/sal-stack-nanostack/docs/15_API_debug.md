Network debug statistics API
==============================

This chapter describes the debug statistics interface API.

## API Header

Using the network debug statistics API, it is possible to collect statistics from the stack in real-time for debugging purposes. You can use this API by including the following header to your application:

```
#include nwk_stats_api.h
```

An overview of the function definitions and the Network statistics structure (`nwk_stats_t`) itself are presented further in this document.

##Initializing protocol stats collection

To initialize collection of network statistics in the network (NWK) statistics structure:

```
void protocol_stats_init
(
	void
)
```

## Start stats collection

To start collecting network statistics in the Network (NWK) statistics structure:

```
void protocol_stats_start
(
	nwk_stats_t	*stats_ptr
)
```

Parameter|Description
---------|-----------
`stats_ptr`|A pointer to the statistics structure on application.

## Stop stats collection

To stop collecting network statistics in NWK statistics structure:

```
void protocol_stats_stop
(
	void
)
```

## Reset stats

To reset network statistics to clean slate state, that is, all the statistics counters are set to zero:

```
void protocol_stats_reset
(
	void
)
```

## Types of collectable stats

Various types of network stats can be collected using the structure `typedef struct nwk_stats_t `. The following statistics can be collected:

### General MAC related stats

Member|Description
------|-----------
`mac_tx_buffer_overflow` | Provides a count MAC TX queue overflow.
`mac_tx_queue_size` | Current MAC TX queue size.
`mac_tx_queue_peak` | MAC TX queue peak size.
`mac_rx_count` | Number of received MAC packets.
`mac_tx_count` | Number of transmitted MAC packets.
`mac_bc_tx_count` | >Number of transmitted MAC broadcast packets.
`mac_rx_drop` | Number of dropped MAC packets.

### MAC payload flow

Member|Description
------|-----------
`mac_tx_bytes` | Number of bytes transmitted.
`mac_rx_bytes` | Number of bytes received.
`mac_tx_failed` | Number of times a transmission failed.
`mac_tx_retry` | Number of times retries were made.
`mac_tx_cca_cnt` | Number of times clear channel assessment was made.
`mac_tx_failed_cca` | Number of failed CCA attempts.
`mac_security_drop` | Number of security packets dropped.

### FHSS related statistics

Member|Description
------|-----------
`fhss_drift_compensation` | FHSS synchronization drift compensation (us/channel).
`fhss_hop_count` | FHSS hop count.
`fhss_synch_interval` | FHSS synchronization interval (s).
`fhss_prev_avg_synch_fix` | Average of 5 preceding synchronization fixes (us).
`fhss_synch_lost` | FHSS synchronization lost counter.

### 6LoWPAN related statistics

Member|Description
------|-----------
`ip_rx_count` | Number of IP packets received.
`ip_tx_count` | Number of IP packets transmitted.
`ip_rx_drop` | Number of IP packets dropped.
`ip_cksum_error` | IP checksum error count.

### IP payload flow

Member|Description
------|-----------
`ip_tx_bytes` | Number of bytes transmitted as IP packets.
`ip_rx_bytes` | Number of bytes received as IP packets.
`ip_routed_up` | Number of bytes routed as IP packets.
`ip_no_route` | Number of times an IP route was not found.

### Fragmentation statistics

Member|Description
------|-----------
`frag_rx_errors` | Fragmentation errors in reception.
`frag_tx_errors` | Fragmentation errors in transmission.

### RPL statistics

Member|Description
------|-----------
`rpl_route_routecost_better_change` | Number of times an RPL parent was changed.
`ip_routeloop_detect` | Number of times an RPL IP loop creation was detected.
`rpl_memory_overflow` | Total of allocations failed due RPL memory overflow.
`rpl_parent_tx_fail` | Number of times transmission to a DODAG parent failed.
`rpl_unknown_instance` | Number of times a packet was received with an unknown RPL Instance ID.
`rpl_local_repair` | Number of times local repair was initiated.
`rpl_global_repair` | Number of times global repair was initiated.
`rpl_malformed_message` | Number of malformed RPL messages received.
`rpl_time_no_next_hop` | Number of seconds spent with no RPL next hop.
`rpl_total_memory` | Total memory currently allocated by RPL.

### Various buffers

Member|Description
------|-----------
`buf_alloc` | A count of buffer allocation.
`buf_headroom_realloc` | A buffer headroom allocation count.
`buf_headroom_shuffle` | A buffer headroom shuffling count.
`buf_headroom_fail` | A buffer headroom failure count.

### ETX statistics

Member|Description
------|-----------
`etx_1st_parent` | Primary parent ETX.
`etx_2nd_parent` | Secondary parent ETX.
