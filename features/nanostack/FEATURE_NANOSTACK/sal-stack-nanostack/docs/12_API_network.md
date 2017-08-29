The Network Layer Control APIs
=========================

This chapter describes the functions of the network control layer. It contains the following sections:

- [_Network control API_](#network-control-api)
- [_6LoWPAN MAC layer configuration API_](#6lowpan-mac-layer-configuration-api)
- [_Network start and stop control API_](#network-start-and-stop-control-api)
- [_Interface address mode API_](#interface-address-mode-api)
- [_Border router interface API_](#border-router-interface-api)
- [_Network sleep control API_](#network-sleep-control-api)
- [_RPL structures and definitions API_](#rpl-structures-and-definitions-api)
- [_RPL root configuration API_](#rpl-root-configuration-api)
- [_NET address retrieval API_](#net-address-retrieval-api)
- [_IPv6 API_](#ipv6-api)
- [_MLE router and host lifetime configuration API_](#mle-router-and-host-lifetime-configuration-api)
- [_MLE neighbor limits configuration API_](#mle-neighbor-limits-configuration-api)
- [_MLE token bucket configuration API_](#mle-token-bucket-configuration-api)
- [_6LoWPAN ND configuration API_](#6lowpan-nd-configuration-api)
- [_PANA configuration API_](#pana-configuration-api)
- [_Network Information retrieval API_](#network-information-retrieval-api)
- [_Multicast API_](#multicast-api)

## API Headers

To use the Network Layer Control APIs, include the following headers:

```
#include net_interface.h
#include net_ipv6_api.h
#include net_pana_parameters_api.h
#include net_6lowpan_parameter_api.h
#include net_mle.h
#include net_nwk_scan.h
#include net_rpl.h
#include net_sleep.h
#include net_polling.h
#include multicast_api.h
#include mac_api.h
```
## Network control API

This section describes the functions of the network control layer where each function is presented with its valid parameters.

The 6LoWPAN stack contains the features and related functions shown in _Table 3-13_.

**Table 3-13 The network control API features and related functions**

Feature|API function
-------|------------
Network interface creation|`arm_nwk_interface_lowpan_init()`
Network interface configuration|`arm_nwk_interface_configure()`
Network interface start, operation mode setting, and interface-specific bootstrap start|`arm_nwk_interface_up()`
Network interface stop|`arm_nwk_interface_down()`

### Interface enable

To set up the configured network interface and enable the interface-specific bootstrap:

```
int8_t arm_nwk_interface_up
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The interface is set up OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 Already in up state.</dd>
<dd>-3 No configured IP stack at the interface (Ethernet and WiFi can return).</dd>
</dl>

### Interface disable

To stop and set the interface to idle:

```
int8_t arm_nwk_interface_down
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The process is OK.</dt>
<dd>-1 An unknown network ID.</dt>
<dd>-2 Not active.</dt>
</dl>


## 6LoWPAN MAC layer configuration API

Use the 6LoWPAN MAC layer API to configure:

- The network security mode for an interface:
	* No security.
	* PANA network authentication.
	* PSK network authentication.
- The channel page, channel mask and scan time for an active scan.
- The channel hopping mode:
	* Single channel
	* FHSS
- FHSS tuning parameters.
- Node interface filters:
	* Beacon protocol ID.
	* PAN ID.
	* Network ID.
- Beacon join priority.
- Node interface address mode:
	* Use only GP64.
	* Use only GP16.
	* Support GP16 and GP64.
- Host poll control (requires MLE support):
	* Deep sleep enable.
	* Set the poll mode.
	* Read mode.

You must configure these before calling the `arm_nwk_interface_up()` function.

Starting the network with security enabled, but without having set the security level and key, will result in an error. The stack will store the 6LoWPAN MAC configuration after the first call and will use the existing configuration until it is explicitly changed by the application, or if the device is power-cycled.

### Scan parameters

The configuration API for setting scan time uses the `arm_nwk_6lowpan_link_scan_parameter_set` function. Parameter `scan_time` is a scan duration exponent, which defines the Beacon wait duration after Beacon request in active scanning. Scan durations in table below are directional.

`scan_time` | scan duration(ms) | `scan_time` | scan duration(ms)
-------|------------|-------|------------
0|16|8|4000
1|32|9|8000
2|64|10|16000
3|125|11|32000
4|250|12|64000
5|500|13|128000
6|1000|14|256000
7|2000



```
int8_t arm_nwk_6lowpan_link_scan_parameter_set
(
	int8_t 		interface_id,
	uint8_t 	scan_time
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`scan_time`|Scan duration exponent.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd>-1 Unknown interface ID.</dd>
<dd>-3 Too long scan time.</dd>
<dd>-4 Interface is not active.</dd>
</dl>

### Setting channel list

This function is used for setting the channel page and channel in the border router and for setting the scanned channel page and channel mask in nodes.

```
int8_t arm_nwk_set_channel_list
(
	int8_t interface_id,
	const channel_list_s *nwk_channel_list
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`nwk_channel_list`|A pointer to channel list structure.

```
typedef struct channel_list_s
{
	channel_page_e channel_page;
	uint32_t channel_mask[8];
} channel_list_s;
```

Member|Description
------|-----------
`channel_page`|The channel page.
`channel_mask`|The used channel mask.


```
typedef enum
{
	CHANNEL_PAGE_0 = 0,
	CHANNEL_PAGE_1 = 1,
	CHANNEL_PAGE_2 = 2,
	CHANNEL_PAGE_3 = 3,
	CHANNEL_PAGE_4 = 4,
	CHANNEL_PAGE_5 = 5,
	CHANNEL_PAGE_6 = 6,
	CHANNEL_PAGE_9 = 9,
	CHANNEL_PAGE_10 = 10
} channel_page_e;
```

<dl>
<dt>Return value</dt>
<dd>>=0 Channel configuration OK.</dd>
<dd>-1 Unknown network interface ID.</dd>
<dd>-2 Empty channel list, no channel enabled.</dd>
<dd>-4 If network interface is already active and cannot be re-configured.</dd>
</dl>

### Channel hopping mode

There are two hopping modes:

- Single channel (non-hopping).
	* After the network scan, the node changes to the channel with the best available network.
- FHSS.
	* Frequency hopping mode.
	* After the network scan, the node synchronizes to the best available network and starts continuous channel hopping using a predefined channel list.

By default, the 6LoWPAN stack uses the single channel mode. 

To enable the FHSS mode, you need to create, configure and register a FHSS instance to the software MAC instance.

To create a FHSS instance and set the basic configuration, use:

```
fhss_api_t *ns_fhss_create
(
	const fhss_configuration_t *fhss_configuration,
	const fhss_timer_t *fhss_timer,
	fhss_statistics_t *fhss_statistics
)
```

An application developer must implement the FHSS platform timer functions as described below. The FHSS timer uses 1us resolution if not reduced by the resolution divider.

Member|Description
------|-----------
`fhss_timer_start` | FHSS timer start platform function.
`fhss_timer_stop` | FHSS timer stop platform function.
`fhss_get_remaining_slots` | FHSS timer get remaining slots platform function.
`fhss_get_timestamp` | FHSS timer timestamp read.
`fhss_resolution_divider` | FHSS timer resolution divider.

The  basic configuration for FHSS:

Member|Description
------|-----------
`fhss_tuning_parameters` | Tuning parameters to enhance synchronization accuracy.
`fhss_max_synch_interval` | Maximum interval for requesting synchronization info from a FHSS parent device.
`fhss_number_of_channel_retries` | Number of channel retries.
`channel_mask` | Channel mask.

You can provide platform-specific tuning parameters:

Parameter|Description
---------|-----------
`tx_processing_delay` | Delay between data pushed to PHY TX function and TX started (contains CSMA-CA maximum random period).
`rx_processing_delay` | Delay between TX done (by transmitter) and received data pushed to MAC (by receiver).
`ack_processing_delay` | Delay between TX done (by transmitter) and ACK transmission started (by receiver).

The FHSS synchronization configuration is always given from the border router using the `ns_fhss_configuration_set` function. The endpoint learns the configuration from the received synchronization message.

```
int ns_fhss_configuration_set
(
	fhss_api_t *fhss_api,
	const fhss_synch_configuration_t *fhss_synch_configuration
)
```
Synchronization configuration:

Member|Description
------|-----------
`fhss_number_of_bc_channels` | Number of broadcast channels.
`fhss_number_of_tx_slots` | Number of TX slots per channel.
`fhss_superframe_length` | Superframe dwell time (us).
`fhss_number_of_superframes` | Number of superframes per channel.

To register the created FHSS instance to software MAC instance, use the `ns_sw_mac_fhss_register` function:
```
int ns_sw_mac_fhss_register
(
	struct mac_api_s *mac_api,
	struct fhss_api *fhss_api
)
```

FHSS implements the following API used by the higher layer (software MAC):

Member|Description
------|-----------
`fhss_is_broadcast_channel` | Checks if the current channel is a broadcast channel.
`fhss_use_broadcast_queue` | Checks if a broadcast queue must be used instead of a unicast queue.
`fhss_tx_handle` | Set the destination channel and write synchronization info.
`fhss_check_tx_conditions` | Check TX permission.
`fhss_receive_frame` | Notification of received FHSS synch or synch request frame.
`fhss_data_tx_done` | Data TX done callback.
`fhss_data_tx_fail` | Data TX or CCA failed callback.
`fhss_synch_state_set` | Change the synchronization state.
`fhss_read_timestamp` | Read the timestamp.
`fhss_get_retry_period` | Get the retransmission period.
`fhss_init_callbacks` | Initialize MAC functions.


The software MAC implements the following callbacks to serve requests from FHSS:

Member|Description
------|-----------
`mac_read_tx_queue_size` | Read MAC TX queue size.
`mac_read_mac_address` | Read MAC address.
`mac_read_datarate` | Read PHY datarate.
`mac_change_channel` | Change channel.
`mac_send_fhss_frame` | Send FHSS frame.
`mac_synch_lost_notification` | Send a notification when FHSS synchronization is lost.
`mac_tx_poll` | Poll TX queue.
`mac_broadcast_notify` | Broadcast channel notification from FHSS.
`mac_read_coordinator_mac_address` | Read the coordinator MAC address.

To disable FHSS, you can delete an instance using the `ns_fhss_delete` function.
```
int ns_fhss_delete
(
	fhss_api_t *fhss_api
)
```

### Beacon protocol ID filter

During an active 802.15.4 MAC scan, the 6LoWPAN stack transmits MAC beacon requests to the pre-defined channels. If it receives a beacon, it will filter the beacon by using the protocol ID (this filter is disabled by default).

To set the protocol ID filter:

```
int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan
(
	int8_t 	interface_id,
	uint8_t 	protocol_ID
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`protocol_ID`|The value `0-0xFE` enables the filter for a specific ID.<br>
The value `0xFF` (default) disables the filter.

<dl>
<dt>Return value</dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
</dl>

The stack will respond to the application with `ARM_NWK_SCAN_FAIL`, if no valid networks are found.

### PAN ID filter

To set a PAN ID filter of a configured network interface for a network scan:

```
int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan
(
	int8_t 	interface_id,
	uin16_t pan_id_filter
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`pan_id_filter`|Values `0x0000-0xFFFE` enable the filter for a specific ID.<br>
The value `0xFFFF` (default) disables the filter.

<dl>
<dt>Return value</dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
</dl>

### Network ID filter

To set a network ID filter of a configured network interface for a network scan:

```
int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan
(
	int8_t interface_id,
	uin8_t *nwk_id_filter
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`nwk_id_filter`|A pointer to a 16-byte array used to filter network. The value NULL (default) disables the filter.

<dl>
<dt>Return value</dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
</dl>

###  Beacon join priority

This section defines the beacon join priority interface. 

#### Beacon join priority transmit callback

This callback defines the join priority that is transmitted in the beacon. Join priority is an 8-bit field in the beacon that can be set for example based on RPL protocol rank data.

```
typedef uint8_t beacon_join_priority_tx_cb
(
int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>Join priority to be transmitted in beacon.</dd>
</dl>

#### Compare received beacon callback

This callback defines the preference rate of the node that has sent beacon, when connecting to the network.

```
typedef uint8_t beacon_compare_rx_cb
(
int8_t interface_id, 
uint8_t join_priority,
uint8_t link_quality
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`join_priority`|The join priority that has been received in beacon. 0 to 255.
`link_quality`|Link quality. 0 to 255. 255 is the best quality.

<dl>
<dt>Return value</dt>
<dd>Connect to preference. 0 to 255. 255 is highest connect to preference.</dd>
</dl>

#### Set callback for beacon join priority transmit

Sets the callback that defines the join priority that is transmitted in beacon. If the callback is not set, the default functionality is used. By default, the join priority is combined from the RPL DAGRank and RPL DODAG preference.

```
int8_t arm_nwk_6lowpan_beacon_join_priority_tx_callback_set
(
int8_t interface_id, 
beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`beacon_join_priority_tx_cb_ptr`|A function pointer.

<dl>
<dt>Return value</dt>
<dd>=0 Success.</dd>
<dd>-1 Unknown network ID.</dd>
<dd>-2 Other error.</dd>
</dl>

#### Set callback for comparing received beacon

Sets the callback that defines the preference rate of the node that has sent the beacon, when connecting to the network. If the callback is not set, the default functionality is used. By default, the connecting priority is defined based on the link quality and the join priority received in the beacon.

```
int8_t arm_nwk_6lowpan_beacon_compare_rx_callback_set
(
int8_t interface_id, 
beacon_compare_rx_cb *beacon_compare_rx_cb_ptr
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`beacon_compare_rx_cb_ptr`|A function pointer.

<dl>
<dt>Return value</dt>
<dd>=0 Success.</dd>
<dd>-1 Unknown network ID.</dd>
<dd>-2 Other error.</dd>
</dl>

### Security API

This section describes the functions of the security API that you can use to:

- Cipher PANA and set PSK (for a server and a client).
- Set a TLS PSK key and certificate.
- Initialize a certificate chain:
	* Network.
	* Transport layer.
- Update the PANA key.
- Set a timer period for the key trigger after delivery.
- Activate a new key before timer triggering.


#### Set TLS PSK key

To set a PSK key to the TLS module:

```
int8_t arm_tls_add_psk_key
(
	uint8_t *key_ptr,
	uint16_t key_id
)
```

Parameter|Description
---------|-----------
`key_ptr`|A pointer to a 16-byte long PSK key.
`key_id`|ID for the PSK key.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

#### Remove TLS PSK key

To remove the PSK key from the TLS module:

```
int8_t arm_tls_remove_psk_key
(
	uint16_t key_id
)
```

Parameter|Description
---------|-----------
`key_id`|ID for the PSK key.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

#### Set certificate chain for network

To set a certificate chain for network authentication:

```
int8_t arm_network_certificate_chain_set
(
	arm_certificate_chain_entry_t *chain_info
)
```

Parameter|Description
---------|-----------
`chain_info`|A pointer to a certificate chain.

<dl>
<dt>Return value</dt>
<dd>>=0 The certificate chain register is OK.</dd>
<dd>-1 TLS is not supported in this stack.</dd>
<dd>-2 A NULL pointer parameter.</dd>
</dl>

## Network start and stop control API

This section specifies how to start and stop the network state of the stack.

After configuring the network interface, start the network using the `arm_nwk_interface_up()` function that starts the 6LoWPAN bootstrap process:

- An Edge router in use is required.
- Mesh network in use.
- Full RPL mesh routing is available.
- Node derives GP64 or GP16.
- Router or host mode is available.
- Link layer security is supported.

### 6LoWPAN bootstrap configure

To configure the 6LoWPAN bootstrap:

```
int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set
(
	int8_t				interface_id,
	net_6lowpan_mode_e	bootstrap_mode,
	uint8_t				enable_mle_protocol
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`bootstrap_mode`|Values to start the bootstrap mode:<br>
`NET_6LOWPAN_BORDER_ROUTER` initializes a basic setup for the border router.<br>
`NET_6LOWPAN_ROUTER` enables normal 6LoWPAN ND and RPL to bootstrap.<br>
`NET_6LOWPAN_HOST` enables normal 6LoWPAN ND only to bootstrap.<br>
`NET_6LOWPAN_SLEEPY_HOST` enables normal 6LoWPAN ND only to bootstrap.
`enable_mle_protocol`|The value `0x01` enables the MLE protocol.<br>
The value `0x00` disables the MLE for bootstrap.

<dl>
<dt>Return value</dt>
<dd>>=0 The bootstrap mode is set OK.</dd>
<dd>-1 An unknown network ID.</dd>
<dd>-2 An unsupported bootstrap type in this stack.</dd>
<dd>-3 No memory for a 6LoWPAN stack.</dd>
<dd>-4 A <code>NULL</code> pointer parameter.</dd>
</dl>

The `arm_nwk_interface_up()` function call starts the stack bootstrap process. The bootstrap process performs 802.15.4 MAC beacon active scanning on the selected channel list, 6LoWPAN ND, and RPL router discovery.

If the application wants to save power and enter sleep, the network mode should use `NET_6LOWPAN_HOST` or `NET_6LOWPAN_SLEEPY_HOST`; sleepy host mode uses indirect transmission. When the device awakens, it uses polling data from its parent. A host device does not route packets so it can continue to sleep. Router devices can, likewise, enter sleep; however, the network topology will become unstable for the duration while the device remains in sleep.

All configuration calls (such as channel selection) must be performed before calling `arm_nwk_interface_up()`.

<span class="notes">**Note**: After successfully calling the `arm_nwk_interface_up()` function, the application must wait for the `ARM_LIB_NWK_INTERFACE_EVENT` status event from the stack before proceeding to perform another communications stack related function.</span>

### IPv6 bootstrap configure

#### IPv6 bootstrap configuration

To configure the IPv6 bootstrap:

```
int8_t arm_nwk_interface_configure_ipv6_bootstrap_set
(
	int8_t 			interface_id,
	net_ipv6_mode_e bootstrap_mode,
	uint8_t 		*ipv6_prefix_pointer
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`bootstrap_mode`|Values to start the bootstrap mode.
`NET_IPV6_BOOTSTRAP_STATIC`|The application defines the IPv6 prefix.
`ipv6_prefix_pointer`|A pointer to a 64-bit IPv6 prefix.

<dl>
<dt>Return value</dt>
<dd>>=0 The bootstrap mode is set OK.</dd>
<dd>-1 An unknown network ID.</dd>
</dl>

The `arm_nwk_interface_up()` function call starts the stack bootstrap process and performs neighbour discovery.


#### Accept Router Advertisements 

You can change the setting when an interface is created. If you change the setting you must do it before the bootstrap is started.

```
int8_t arm_nwk_interface_accept_ipv6_ra
(
	int8_t 			interface_id,
	net_ipv6_accept_ra_e 	accept_ra
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`accept_ra`|Router Advertisements handling mode:<br>`NET_IPV6_RA_ACCEPT_IF_AUTONOMOUS` Accept Router Advertisements when using autonomous IPv6 address allocation. Ignore when using a static address. This is the default value for the setting.<br>`NET_IPV6_RA_ACCEPT_ALWAYS` Accept Router Advertisements always, even when using static IPv6 address allocation.

<dl>
<dt>Return value</dt>
<dd>>=0 Setting done.</dd>
<dd><0 Failed (for example an invalid interface ID).</dd>
</dl>


### Security mode for RF link layer

To set the security mode for the link layer of a configured network interface:

```
int8_t arm_nwk_link_layer_security_mode
(
	int8_t 					interface_id,
	net_6lowpan_link_layer_sec_mode_e  mode,
	uint8_t 				sec_level,
	net_link_layer_psk_security_info_s *psk_key_info
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`mode`|Defines the security mode for the link layer:<br>`NET_SEC_MODE_NO_SECURITY`, security is disabled.<br>`NET_SEC_MODE_PSK_LINK_SECURITY`, security is enabled with selected mode and PSK key info.<br>`NET_SEC_MODE_PANA_LINK_SECURITY`, PANA authentication server defines the link layer keys.
`sec_level`|Supported values are 1 to 7. This parameter is only checked when the mode is `NET_SEC_MODE_PSK_LINK_SECURITY`.
`psk_key_info`|A pointer to PSK key material: key and key ID. This parameter is only checked when the mode is `NET_SEC_MODE_PSK_LINK_SECURITY`.

<dl>
<dt>Return value</dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 Unsupported PANA authentication in this stack.</dd>
<dd>-3 Unsupported security level.</dd>
<dd>-4 An unsupported key ID from the PSK info.</dd>
<dd>-5 A <code>NULL</code> pointer psk_key_info parameter.</dd>
</dl>

## Interface address mode API

To set the global address mode for a configured network interface:

```
int8_t arm_nwk_6lowpan_gp_address_mode
(
	int8_t 					interface_id,
	net_6lowpan_gp_address_mode_e 	mode,
	uint16_t 				short_address_base,
	uint8_t 				define_new_short_address_at_DAD
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`mode`|Defines the register mode of the 6LoWPAN global address:<br>`NET_6LOWPAN_GP64_ADDRESS`, the interface only registers GP64.<br>`NET_6LOWPAN_GP16_ADDRESS`, the interface only registers GP16.<br>`NET_6LOWPAN_MULTI_GP_ADDRESS`, the interface registers both GP16 (primary) and GP64 (secondary) addresses.
`short_address_base`|Short address base. If an application defines the value `0-0xFFD`, 6LoWPAN tries to register the GP16 address using that address. Values `0xFFFE` and `0xFFFF` generate a random 16-bit short address. If the border router wants to use a short address, it defines it here.
`define_new_short-address_at_DAD`|This parameter is only checked when the mode is not `NET_6LOWPAN_GP64_ADDRESS` and the `short_address_base` is `0-0xFFD`. The recommended value 1 enables an automatic new address definition at Duplicate Address Detection (DAD). The value 0 generates a DAD error for an interface bootstrap, which the border router does not check.

<dl>
<dt>Return value</dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 Bootstrap mode is not selected.</dd>
<dd>-3 An unsupported short address at this platform.</dd>
</dl>

## Border router interface API

The border router is configured in three phases:

1. MAC and 6LoWPAN ND:
	1. MAC:
		* PAN ID.
		* NWK ID.
		* Beacon protocol ID.
		* Channel.
	2. Interface address mode:
		* Use only GP64.
		* Use only GP16.
		* Support for both GP16 and GP64.
	3. ND setup:
		* RA liftime.
		* Contexts.
		* ABRO version number.
		* Global Prefix address prefix/64.
2. Optional PANA server configuration.
3. Optional RPL DODAG configuration:
	1. Generate a DODAG root base.
	2. Prefixes.
	3. Routes.

#### Define border router setup

To define the border router MAC and 6LoWPAN ND setup for a selected interface:

```
int8_t arm_nwk_6lowpan_border_router_init
(
	int8_t 					interface_id,
	border_router_setup_t	*border_router_setup_ptr
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`border_router_setup_ptr`|A pointer to the MAC and 6LoWPAN ND setup.

<dl>
<dt>Return value</dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 A NULL pointer parameter.</dd>
<dd>-3 Already in up state.</dd>
<dd>-4 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code>.</dd>
<dd>-5 Not a proper interface type.</dd>
</dl>

#### Update border router context list

To update the context list of the interface border router at the configure proxy, use the following function. The update will go to the proxy setup where it must be pushed by `arm_nwk_6lowpan_border_router_configure_push()` if the interface is up.

```
int8_t arm_nwk_6lowpan_border_router_context_update
(
int8_t interface_id,
	uint8_t c_id_flags,
	uint8_t context_len,
	uint16_t ttl,
	uint8_t *context_ptr
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`c_id_flags`|Bit [5] indicates compression support and bit [0:3] a context ID.
`context_len`|The context length in bits must be [64:128].
`ttl`|The time to live value of the context in minutes.
`context_ptr`|A pointer to a full 128-bit memory area.

<dl>
<dt>Return value</dt>
<dd>>=0 Context is added to the proxy OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 No memory for a new context.</dd>
<dd>-3 Failure of a given parameter (<code>c_id_flags</code> > <code>0x1F</code> or <code>contex_len</code> < 64).</dd>
<dd>-4 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-5 Not a proper interface type.</dd>
</dl>


#### Context parameter update
To change the compression state or time-to-live value of the context, use the following function. The update will go to the proxy setup where it must be pushed by `arm_nwk_6lowpan_border_router_configure_push()` if the interface is up.

```
int8_t arm_nwk_6lowpan_border_router_context_parameter_update
(
	int8_t interface_id,
	uint8_t c_id,
	uint8_t compress_mode,
	uint16_t ttl
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`c_id`|A context ID for detection of a proper context.
`compress_mode`|1 Enables 6LoWPAN compression for TX and RX. 0 Disables context compression for TX.
`ttl`|The time-to-live value of the context in minutes.

<dl>
<dt>Return value</dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 An unknown context ID.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
</dl>


#### Delete context

To delete the selected context, use the following function. The update will go to the proxy setup where it must be pushed by `arm_nwk_6lowpan_border_router_configure_push()` if the interface is up.

```
int8_t arm_nwk_6lowpan_border_router_context_remove_by_id
(
	int8_t interface_id,
	uint8_t c_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`c_id`|A context ID for detection of a proper context.

<dl>
<dt>Return value</dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 An unknown context ID.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
</dl>

#### Reload 6LoWPAN contexts

To reload all 6LoWPAN contexts from the proxy and update the ABRO version number, use the following function. The RA advertisement of the border router starts advertising a new 6LoWPAN context setup.

```
int8_t arm_nwk_6lowpan_border_router_configure_push
(
	int8_t interface_id
	uint8_t c_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`c_id`|A context ID for detection of a proper context.

<dl>
<dt>Return value</dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 An unknown context ID.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
</dl>

## Network sleep control API

If the application wants to save power, the 6LoWPAN stack can enter sleep state by calling the `arm_net_enter_sleep()` function. 
Sleep functionality is supported when the 6LoWPAN bootstrap mode is `NET_6LOWPAN_HOST` or `NET_6LOWPAN_SLEEPY_HOST`.

The following reference implementation for a "bare-metal" (no OS) port shows an implementation of idle that provides full sleep 
functionality. It requires the platform to provide the `eventOS_scheduler_sleep()` and `eventOS_scheduler_wait()` calls. The 
implementation will automatically call sleep when appropriate, based on feedback from the scheduler and the network 
stack (with some input from application APIs to the stack).

```
void eventOS_scheduler_idle(void)
{
	uint32_t sleep_possible = arm_net_check_enter_deep_sleep_possibility();
	if(sleep_possible)
	{
		uint32_t system_timer_next_tick_time = eventOS_event_timer_shortest_active_timer();
		if(system_timer_next_tick_time)
		{
			if(system_timer_next_tick_time < sleep_possible)
				sleep_possible = system_timer_next_tick_time; //Select shorter next event
		}
		if(arm_net_enter_sleep() == 0)
		{
			if(eventOS_scheduler_timer_stop() != 0)
			{
				debug("EventOS timer sleep fail\n");
			}

			sleep_possible = eventOS_scheduler_sleep(sleep_possible);
			//Enable Data Polling after sleep & Synch Times
			arm_net_wakeup_and_timer_synch(sleep_possible);

			//Update Runtime ticks and event timers
			if(eventOS_scheduler_timer_synch_after_sleep(sleep_possible)!= 0)
			{
				debug("Timer wakeUP Fail\n");
			}
		}
	}
	else
	{
		eventOS_scheduler_wait();
	}
}
```

Where `eventOS_scheduler_sleep( )` and `eventOS_scheduler_wait( )` are functions in the platform port. They put the processor in sleep or in an idle (=waiting signal) state.

### Checking if sleep mode possible

Use the function `arm_net_check_enter_deep_sleep_possibility()` to check whether there is a possibility of putting the stack in deep sleep or not.

```
uint32_t time_to_sleep=0;
time_to_sleep = arm_net_check_enter_deep_sleep_possibility()
```

<dl>
<dt>Return value</dt>
<dd>0, not possible to sleep.</dd>
<dd>Time in milliseconds, possible to sleep for that amount of time.</dd>
</dl>

### Enter sleep mode

To put the stack to sleep:

```
int arm_net_enter_sleep(void);
```

<dl>
<dt>Return value</dt>
<dd>0, stack stopped.</dd>
<dd>-1, action not supported at the moment.</dd>
</dl>

### Restart stack and synch timers

To restart the stack and synchronize timers:

```
int arm_net_wakeup_and_timer_synch(uint32_t time_to_sleep);
```

<dl>
<dt>Return value</dt>
<dd>0, stack restarted.</dd>
<dd>1, stack will restart after sleeping for a time defined as time_to_sleep.</dd>
<dd>-1, stack already active.</dd>
</dl>

## RPL structures and definitions API

The 6LoWPAN Border Router defines an RPL MOP and DODAG preference. The ZigBeeIP must use `(BR_DODAG_MOP_NON_STORING | BR_DODAG_PREF_7)` only one RPL DODAG root. The RPL module defines a DODAG ID for an interface DAG from the default GP address of the interface. Therefore, before the interface can use the RPL module, the interface bootstrap and address must first be configured.

The DODAG instance ID is dynamically allocated. It generates the first free number starting from `0x01`, and ends to `0xFE`.

The DODAG configuration structure has the following members:

```
typedef struct dodag_config_s
{
	uint8_t DAG_SEC_PCS;
	uint8_t DAG_DIO_INT_DOUB;
	uint8_t DAG_DIO_INT_MIN;
	uint8_t DAG_DIO_REDU;
	uint16_t DAG_MAX_RANK_INC;
	uint16_t DAG_MIN_HOP_RANK_INC;
	uint16_t DAG_OCP;
	uint8_t LIFE_IN_SECONDS;
	uint16_t LIFETIME_UNIT;
} dodag_config_s;
```

Member|Description
------|-----------
`DAG_SEC_PCS`|Defines the possible parents for node. The recommended and maximum value 1 means two parents.
`DAG_DIO_INT_DOUB`|RPL Trickle DIOIntervalDoublings. Should use 12.
`DAG_DIO_INT_MIN`|RPL Trickle DIOIntervalMin. Should use 9.
`DAG_DIO_REDU`|RPL Trickle DIORedundancyConstant. Should use 3.
`DAG_MAX_RANK_INC`|RPL MaxRankIncrease. Should use 2048.
`DAG_MIN_HOP_RANK_INC`|RPL MinHopRankIncrease. Should use 128.
`DAG_OCP`|Objective Code Point must use 1.
`LIFE_IN_SECONDS`|Lifetime that is used as default for all routes (expressed in units of lifetime unit). Default lifetime in seconds is `LIFE_IN_SECONDS * LIFETIME_UNIT`.
`LIFETIME_UNIT`|Defines the unit in seconds that is used to express route lifetimes. 60 means 1 minute.

### DODAG root setup

The DODAG root setup is as follows:

**_DODAGPreference_ (Prf)**

A 3-bit unsigned integer that defines how preferable the root of this DODAG is compared to other DODAG roots within the instance. The DAGPreference ranges from `0x00` (least preferred) to `0x07` (most preferred). The default is `0`, least preferred.

```
#define RPL_DODAG_PREF_MASK 0x07
#define RPL_DODAG_PREF(n) ((n) & RPL_DODAG_PREF_MASK)
```

**_Mode of Operation_ (MOP)**

The MOP field identifies the mode of operation of the RPL instance as administratively provisioned at and distributed by the DODAG root. All nodes that join the DODAG must be able to honor the MOP to fully participate as a router, or else they must only join as a leaf.

```
/**Non-Storing Mode of Operation  */
#define RPL_MODE_NON_STORING 0x08
/** Storing Mode of Operation with no multicast support */
#define RPL_MODE_STORING 0x10
```

**_Grounded_ (G)**

The Grounded `G` flag indicates that the DODAG advertised can satisfy the application-defined goal:

- If the flag is set, the DODAG is grounded.
- If the flag is cleared, the DODAG is floating.

```
#define RPL_GROUNDED 0x80
/** RPL Prefix update Flags for A-flag AUTONOMOUS address generation*/
#define RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG 0x40
/** RPL Prefix update Flags for R-Flag */
#define RPL_PREFIX_ROUTER_ADDRESS_FLAG 0x20
```

## RPL root configuration API

This section introduces the functionality of the RPL root API.

### Define RPL DODAG root proxy

To define the RPL DODAG root proxy for a selected interface:

```
int8_t arm_nwk_6lowpan_rpl_dodag_init
(
	int8_t interface_id,
	dodag_config_s *config,
	uint8_t flags
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`config`|A pointer to the DODAG configure structure.
`flags`|Defines the RPL MOP,  DODAG Pref and Grounded. The ZigBeeIP must use the non storing mode of operation and the most preferred preference `(BR_DODAG_MOP_NON_STORING | BR_DODAG_PREF_7)`

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG proxy allocation is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has already defined the DODAG root.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-4 No memory for DODAG root base.</dd>
</dl>

### Update RPL prefix

To update the RPL prefix to the DODAG proxy:

```
int8_t arm_nwk_6lowpan_rpl_dodag_prefix_update
(
	int8_t 		interface_id,
	uint8_t 	*prefix_ptr,
	uint8_t 	prefix_len,
	uint8_t 	flags,
	uint32_t 	lifetime
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID that defines the root proxy where the prefix update will come.
`prefix_ptr`|A pointer to the IPv6 prefix (16 bytes).
`prefix_len`|The prefix length must be 64.
`flags`|Defines:<br>R-flag `(RPL_PREFIX_ROUTER_ADDRESS_FLAG)`.<br>A-flag `(RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG)`.
`lifetime`|The prefix lifetime.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG proxy allocation is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 No root defined for a specific interface.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-4 No memory for the prefix.</dd>
</dl>

### Update RPL route information

To update the RPL route information to the DODAG proxy:

```
int8_t arm_nwk_6lowpan_rpl_dodag_route_update
(
	int8_t 		interface_id,
	uint8_t 	*prefix_ptr,
	uint8_t 	prefix_len,
	uint8_t 	flags,
	uint32_t 	lifetime
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID that defines the root proxy where the prefix update will come.
`prefix_ptr`|A pointer to the IPv6 prefix (16 bytes).
`prefix_len`|The prefix length must be 64.
`flags`|Defines the R-flag `(RPL_PREFIX_ROUTER_ADDRESS_FLAG)`
`lifetime`|The route lifetime.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG proxy allocation is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 No root defined for a specific interface.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-4 No memory for the route.</dd>
</dl>

### Activate RPL DODAG

To activate the defined and configured RPL DODAG, if the configuration was done when the interface was in _up_ state:

```
int8_t arm_nwk_6lowpan_rpl_dodag_start
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG is started OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### DODAG DAO trig

To trigger an RPL DODAG DAO by DTSN increment, use the following function. DODAG nodes send DAO routing information to the root.

```
int8_t arm_nwk_6lowpan_rpl_dodag_dao_trig
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG DAO trig is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### DODAG version increment

Network devices reset the current RPL instance. To do a unicast DIS/DIO and DAO/DAO ACK handshake:

```
int8_t arm_nwk_6lowpan_rpl_dodag_version_increment
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG DAO trig is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### RPL DODAG preference set

To set the RPL DODAG preference:

```
int8_t arm_nwk_6lowpan_rpl_dodag_pref_set
(
int8_t interface_id,
uint8_t preference
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`preference`|DODAG preference. 0 to 7. 0 is least preferred.

<dl>
<dt>Return value</dt>
<dd>>=0 Set OK</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### Run down RPL DODAG

To run down the RPL DODAG interface by flooding the poison rank, use the following function. Wait for an event from the stack when the flooding is completed. After this call, DODAG removal is recommended.

```
int8_t arm_nwk_6lowpan_rpl_dodag_poison
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG flooding started OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### Remove DODAG root setup

To remove the DODAG root setup from the selected interface:

```
int8_t arm_nwk_6lowpan_rpl_dodag_remove
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value</dt>
<dd>>=0 The RPL DODAG removal is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

## NET address retrieval API

An application can read the interface NET address (see _Table 3-14_) from the selected interface.

**Table 3-14 NET address**

Address|Description
-------|-----------
`ADDR_IPV6_GP`|Primary GP IPv6 address. Every IPv6 and 6LoWPAN interface supports this by default.
`ADDR_IPV6_GP_SEC`|Secondary IPv6 address. The 6LoWPAN interface supports this only when the address mode for bootstrap is `NET_6LOWPAN_MULTI_GP_ADDRESS`.

To read the network address information:

```
extern int8_t arm_net_address_get
(
	int8_t 		interface_id
	net_address_t	addr_id,
	uint8_t		*address
)
```

Parameter|Description
---------|-----------
`addr_id`|Identifies the address information type to be read.
`address`|A pointer to a structure where the address information is written to.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

## IPv6 API

The behaviour of the IPv6 layer can be controlled by the following functions.

### Maximum fragmented-datagram reception unit

To adjust the maximum IPv6 fragmented datagram size that the IPv6 stack will allocate memory for:

```
int8_t arm_nwk_ipv6_frag_mru(
	uint16_t frag_mru
)
```

Parameter|Description
---------|-----------
`frag_mru`|Fragmented Maximum Reception Unit in octets.

<dl>
<dt>Return value</dt>
<dd>0 Change OK - actual MRU is at least requested value.</dd>
<dd><0 Change invalid - unable to set the specified MRU.</dd>
</dl>

RFC 2460 requires this value to be at least 1500. It should also be at least as large as the MTU of each attached link. Implementation details such as alignment mean that the actual MRU may be larger than requested - 1504 is typical.

### Flow label generation

To modify the system default for automatic IPv6 flow label generation:

```
void arm_nwk_ipv6_auto_flow_label(
	bool auto_flow_label
)
```

Parameter|Description
---------|-----------
`auto_flow_label`|`true` to enable auto-generation.


If enabled, the stack auto-generates flow labels on outgoing packets following the guidelines in [RFC 6437](https://tools.ietf.org/html/rfc6437), either by random assignment for connected sockets, otherwise by hashing the 5-tuple
{dest addr, source addr, protocol, dest port, source port}.

When acting as a tunnel entry point, flow labels are created for tunnel packets according to the guidelines in [RFC 6438](https://tools.ietf.org/html/rfc6438).

Flow labels can also be controlled on a per-socket basis using `socket_setsockopt(SOCKET_IPV6_FLOW_LABEL)`.


## MLE router and host lifetime configuration API

**Router lifetime** is a global parameter in the 6LoWPAN network. The same value must be used within the network. When a router receives an MLE advertisement message, it refreshes all the neighbours. 

**Host lifetime** is node-specific parameter and a host changes it during the MLE handshake. The host refreshes a neighbour connection when about 30% of the lifetime is left.

In a 6LoWPAN network, the default router lifetime is 128 seconds. The MLE advertisement interval is 32 seconds with [0.9-1.1] times random. If a router misses four advertisment from a neighbour router it removes the entry silently.

If you set a longer router lifetime:

* The MLE advertisement period is longer (router lifetime in seconds/4).
* Routers react slower when the neighbour router cannot be reached.
* New neighbours are detected slower after the bootstrap.
* Large networks work better.

If you set a shorter router lifetime:

* The  MLE advertisment period is shorter.
* New neighbours and disappeared routers are detected faster.
* Good solution for a small network that needs to react fast.

### Router lifetime set

The router lifetime can be changed when an interface is created. It must be set before the bootstrap is started.

```
int8_t arm_nwk_6lowpan_mle_router_lifetime_set(
int8_t interface_id,
uint16_t lifetime
)
```

Parameter|Description
---------|-----------
`interface_id`|The interface ID for setting a new lifetime value.
`lifetime`|Supported lifetime values are between 64 and 2560 seconds.

<dl>
<dt>Return value</dt>
<dd>0, Lifetime update OK.</dd>
<dd><0 Lifetime update fail.</dd>
</dl>

### Host lifetime set

The host lifetime can be changed when an interface is created. It must be set before the bootstrap is started.

```
int8_t arm_nwk_6lowpan_mle_host_lifetime_set(
int8_t interface_id,
uint16_t lifetime
)
```

Parameter|Description
---------|-----------
`interface_id`|The interface ID for setting a new lifetime value.
`lifetime`|Supported lifetime values are between 64 and 2560 seconds.

<dl>
<dt>Return value</dt>
<dd>0, Lifetime update OK.</dd>
<dd><0 Lifetime update fail.</dd>
</dl>

## MLE neighbor limits configuration API

MLE neighbor limits settings limit the number of neighbors added to MLE neighbor list.

If the number of neighbors reaches the lower threshold, MLE starts to limit addition of new neighbors by starting to ignore multicast MLE messages from unknown neighbors (ignore probability is randomized). The value must be smaller than the upper threshold and maximum value.

If the number of neighbors reaches the upper threshold, MLE stops adding new neighbors based on multicast MLE messages. Only the nodes that select this node for a parent during the bootstrap are accepted. The value must be smaller or the same as the maximum value.

If the number of neighbors reaches the maximum value, no new neighbors are added.

If the MLE neighbor list limits are not used, all values must be set to 0.
  
### Set MLE neighbor list limits

The MLE neighbor limits can be changed when an interface is created. The limit must be set before
the bootstrap is started.

```
int8_t arm_nwk_6lowpan_mle_neighbor_limits_set(
int8_t interface_id,
uint16_t lower_threshold,
uint16_t upper_threshold,
uint16_t max
)
```

Parameter|Description
---------|-----------
`interface_id`|The interface ID for setting new MLE neighbor list limits.<
`lower_threshold`|Lower threshold. 5 to 499. 0 limits not used.
`upper_threshold`|Upper threshold. 5 to 500. 0 limits not used.
`max`|The maximum number of neighbors. 5 to 500. 0 limits not used.

<dl>
<dt>Return value</dt>
<dd>0, Limits update OK.</dd>
<dd><0 Limits update fail.</dd>
</dl>

## MLE token bucket configuration API

The MLE message token bucket limits the MLE message sending rate. The token bucket size controls the bucket size. The token bucket rate controls the rate of adding new tokens. The count defines how many tokens at a time are added to the bucket.

The minimum interval of the rate is 0.1 seconds (for example, if the rate is 3 and the count is 4, then 4 new tokens are added to bucket every 0.3 seconds).

If the token bucket is not used, all values must be set to 0.

### Set MLE message token bucket settings

```
int8_t arm_nwk_6lowpan_mle_token_bucket_settings_set(
int8_t interface_id,
uint8_t size,
uint8_t rate,
uint8_t count
)
```

Parameter|Description
---------|-----------
`interface_id`|Interface ID for setting new MLE message token bucket settings.
`size`|Bucket size. 1 to 255. 0 token bucket not used.
`rate`|Token rate. 1 to 255. 0 token bucket not used.
`count`|Token count. 1 to 255. 0 token bucket not used.

<dl>
<dt>Return value</dt>
<dd>0, Token bucket settings update OK.</dd>
<dd><0 Token bucket settings update fail.</dd>
</dl>

## 6LoWPAN ND configuration API

This section introduces the functions to configure the 6LoWPAN ND (Neighbour Discovery) parameters. _Table 3-15_ shows the configuration functions available.

**Table 3-15 Functions to configure 6LoWPAN ND parameters**

Function|Description
--------|-----------
`net_6lowpan_nd_parameter_read()`|Read a 6LoWPAN ND parameter.
`net_6lowpan_nd_parameter_set()`|Set a 6LoWPAN ND parameter.

### Parameter structure

This structure defines the 6LoWPAN ND parameters and has the following members:

```
typedef struct nd_parameters_s
{
	uint8_t 	rs_retry_max;
	uint8_t 	ns_retry_max;
	uint16_t 	timer_random_max;
	uint16_t 	rs_retry_interval_min;
	uint16_t 	ns_retry_interval_min;
	uint16_t 	ns_retry_linear_backoff;
	bool		multihop_dad;
	bool		iids_map_to_mac;
	bool		send_nud_probes;
	uint16_t 	ra_interval_min;
	uint8_t 	ra_transmits;
	uint8_t		ra_cur_hop_limit;
	uint32_t	ra_link_mtu;
	uint32_t	ra_reachable_time;
	uint32_t	ra_retrans_timer;
	uint16_t 	ns_forward_timeout;
} nd_parameters_s;
```

Member|Description
------|-----------
`rs_retry_max`|Defines the maximum retry count of the bootstrap RS.
`ns_retry_max`|Defines the maximum retry count of the bootstrap NS.
`timer_random_max`|Defines the interval random in 6LoWPAN bootstrap timer ticks for RS, NS and starting NS-NA process.
`rs_retry_interval_min`|Defines the retry interval in 6LoWPAN bootstrap timer ticks waiting for RA.
`ns_retry_interval_min`|Defines the retry interval in 6LoWPAN bootstrap timer ticks waiting for NA.
`ns_retry_linear_backoff`|Defines the linear backoff of the retry interval in bootstrap timer ticks.
`multihop_dad`|Defines whether routers perform duplicate address detection with border router or locally.
`iids_map_to_mac`|Defines whether IPv6 IIDs can be assumed to be based on MAC address (so no address resolution by routers).
`send_nud_probes`|Defines whether IPv6 NUD probes are enabled (disabling may limit fault detection).
`ra_interval_min`|Defines the initial transmission interval for Router Advertisements in standard timer ticks.
`ra_transmits`|Defines the number of RA transmissions.
`ra_cur_hop_limit`|Defines the value of current hop limit placed in Router Advertisements.
`ra_link_mtu`|Defines the value of link MTU placed in Router Advertisements.
`ra_reachable_time`|Defines the value of reachable time placed in Router Advertisements (in milliseconds).
`ra_retrans_time`|Defines the value of retrans timer placed in Router Advertisements (in milliseconds).
`ns_forward_timeout`|Defines the timeout when forwarding NS messages. If reached, our own address discovery process is restarted.

### 6LoWPAN ND parameter set

Use this API to change the 6LoWPAN ND bootstrap parameters.

<span class="notes">**Note**: This function should be called after `net_init_core()` and definitely before creating any 6LoWPAN interface.</span>

For future compatibility, to support extensions to this structure:

1. Read the current parameters using `net_6lowpan_timer_parameter_read()`.
2. Modify the fields known.
3. Set the parameters.

```
int8_t net_6lowpan_nd_parameter_set
(
	const nd_parameters_s *parameter_ptr
)
```

Parameter|Description
---------|-----------
`parameter_ptr`|A pointer for ND parameters.

<dl>
<dt>Return value</dt>
<dd>0 Change is OK.</dd>
<dd>-1 Invalid values.</dd>
<dd>-2 6LoWPAN interface is already active.</dd>
</dl>

### 6LoWPAN ND parameter read

Use this API to read the 6LoWPAN ND bootstrap parameters:

```
void net_6lowpan_nd_parameter_read
(
	nd_parameters_s *parameter_ptr
)
```

Parameter|Description
---------|-----------
`parameter_ptr`|An output pointer for ND parameters.

## PANA configuration API

This section introduces the functions to configure PANA protocol parameters. _Table 3-16_ shows the configuration functions available.

**Table 3-16 Functions to configure PANA protocol parameters**

Function|Description
--------|-----------
`net_pana_parameter_read()`|Read the currently used parameters.
`net_pana_parameter_set()`|Write new parameters.

### Parameter structure

This structure defines the PANA module dynamic parameters and has the following members:

```
typedef struct pana_lib_parameters_s
{
	uint16_t PCI_IRT;
	uint16_t PCI_MRT;
	uint8_t PCI_MRC;
	uint16_t REQ_IRT;
	uint16_t REQ_MRT;
	uint16_t REQ_MRC;
	uint16_t AUTHENTICATION_TIMEOUT;
	uint16_t KEY_UPDATE_THRESHOLD;
	uint8_t KEY_ID_MAX_VALUE;
	uint16_t EAP_FRAGMENT_SIZE;
	uint8_t  AUTH_COUNTER_MAX;
} pana_lib_parameters_s;
```

Member|Description
------|-----------
`PCI_IRT`|Initial PCI timeout in seconds. The default is 10.
`PCI_MRT`|Maximum PCI timeout value in seconds. The default is 60.
`PCI_MRC`|PCI_MRC</code> Maximum PCI retransmission attempts. The default is 5.
`REQ_IRT`|PCI_MRC</code> Initial request timeout in seconds. The default is 20.
`REQ_MRT`|Maximum request timeout value. The default is 60.
`REQ_MRC`|Maximum request retransmission attempts. The default is 4.
`AUTHENTICATION_TIMEOUT`|Maximum timeout for authentication timeout. The default is 100 seconds.
`KEY_UPDATE_THRESHOLD`|A gap in seconds when the server starts sending a new network key. The default is 10.
`KEY_ID_MAX_VALUE`|Defines the resolution for key ID <code>[1-KEY_ID_MAX_VALUE]</code>. The default is 255. The minimum accepted value is 3.
`EAP_FRAGMENT_SIZE`|Defines the EAP fragment slot size. Fragmentation is activated when the EAP payload is more than 920. The default is 296.
`AUTH_COUNTER_MAX`|Defines the limit for the PANA session re-authentication. When the maximum value is reached, the server does not respond to PANA notify request. The default is `0xFF`.

### PANA parameter set

Use this API to change the PANA module parameters.

<span class="notes">**Note**: This function should be called after `net_init_core()` and definitely before creating any 6LoWPAN interface.</span>

For future compatibility, to support extensions to this structure:

1. Read the current parameters using `net_pana_parameter_read()`.
2. Modify the fields known.
3. Set the parameters.

```
int8_t net_pana_parameter_set
(
	const pana_lib_parameters_s *parameter_ptr
)
```

Parameter|Description
---------|-----------
`parameter_ptr`|A pointer for PANA parameters.

<dl>
<dt>Return value</dt>
<dd>0 Change is OK.</dd>
<dd>-1 Invalid values.</dd>
<dd>-2 PANA is not supported.</dd>
</dl>

### PANA parameter read

Use this API to read the PANA module parameters:

```
int8_t net_pana_parameter_read
(
	pana_lib_parameters_s *parameter_ptr
)
```

Parameter|Description
---------|-----------
`parameter_ptr`|An output pointer for PANA parameters.

<dl>
<dt>Return value</dt>
<dd>0 Read OK.</dd>
<dd>-1 PANA is not supported.</dd>
</dl>

## Network Information retrieval API

This section introduces functions to read network parameters. _Table 3-17_ shows the configuration functions available.

**Table 3-17 Functions to read network parameters**

Function|Description
--------|-----------
`arm_nwk_param_read()`|Read network layer configurations.
`arm_nwk_mac_address_read()`|Read MAC PAN ID, short address and EUI-64.
`arm_nwk_nd_address_read()`|Read the 6LoWPAN ND border router address and NWK prefix.

### Network layer configurations

To read network layer configurations:

```
int8_t arm_nwk_param_read
(
	int8_t 			interface_id,
	link_layer_setups_s 	*network_params
)
```
Parameter|Description
---------|-----------
`network_params`|A pointer to the structure where the network layer configurations are written to.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd>-1 Unknown PAN coordinator.</dd>
</dl>

This structure defines a parameter list of a network coordinator and it is used to read the network parameter for a warm start:

```
typedef struct link_layer_setups_s
{
	 uint16_t 		PANId;
	 uint8_t 		LogicalChannel;
	 net_mac_address_t 	addr_mode;
	 uint8_t 		address[8];
	 uint8_t 		sf;
}link_layer_setups_s;
```

Member|Description
------|-----------
`PANId`|Network PAN ID.
`LogicalChannel`|Network logical channel.
`addr_mode`|Coordinator address mode.
`address`|Coordinator address.
`sf`|Network super frame setup.

### Link layer address read

To read the MAC PAN ID, short address and EUI-64:

```
int8_t arm_nwk_mac_address_read
(
	int8_t 			interface_id,
	link_layer_address_s 	*mac_params
)
```

Parameter|Description
---------|-----------
`mac_params`|A pointer to the structure where the MAC addresses are written to.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd><0 Interface is not known.</dd>
</dl>

This structure defines the network MAC address information and it is used to read the link layer address:

```
typedef struct link_layer_address_s
{
	 uint16_t 	PANId;
	 uint16_t 	mac_short;
	 uint8_t 	mac_long[8];
	 uint8_t 	iid_eui64[8];
}link_layer_address_s;
```

Member|Description
------|-----------
`PANId`|Network PAN ID.
`mac_short`|MAC short address. Valid, if the value is `<0xFFFE`.
`mac_long`|MAC long address (EUI-64 for IEEE 802.15.4; EUI-48 for Ethernet).
`iid_eui64`|IPv6 Interface ID based on EUI-64.


### 6LoWPAN parameter read

To read the 6LoWPAN ND border router address and NWK prefix:

```
int8_t arm_nwk_nd_address_read
(
	int8_t 			interface_id,
	network_layer_address_s *nd_addr_info
)
```

Parameter|Description
---------|-----------
`nd_addr_info`|A pointer to the structure where the 6LoWPAN ND border router address is written to.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd><0 Network interface is not known or it is not in active or ready state.</dd>
</dl>

This structure defines the parent address information of the network layer and comprises the following variables:

```
typedef struct network_layer_address_s
{
	 uint8_t border_router[16];
	 uint8_t prefix[8];
}network_layer_address_s;
```

Member|Description
------|-----------
`border_router`|ND border router address.
`prefix`|A long, 64-bit network ID.

## Multicast API

This section introduces functions for multicasting where data can be forwarded to several devices within the network and what devices are included is subject to the multicast scope. For example, a link local multicast is sent to neighbors and cannot be forwarded. However, a site local multicast can be forwarded with a trickle throughout the network and can travel through to the border router. See more on the [Trickle Algorithm](https://tools.ietf.org/html/rfc6206).

<span class="notes">**Note**: The site local multicast is the only multicast scope that can be routed through the border router.</span>

The multicast API can be used to subscribe and unsubscribe different multicast groups and can change the trickle multicast parameters. The multicast parameters are set and changed using the function `multicast_set_parameters()`where multicast groups are managed using the function calls `multicast_add_address()` and `multicast_free_address()`.

<span class="notes">**Note:**
- Only multicast addresses are accepted.
- Trickle forwarding cannot be used with link local addresses.
- The maximum number of multicast groups is 100 where including multicast groups consumes memory allocated by the 6LoWPAN stack.</span>


### Set new parameters for trickle multicast

To set new parameters for a trickle multicast:

```
void multicast_set_parameters
(
	uint8_t		i_min,
	uint8_t		i_doublings,
	uint8_t		k,
	uint8_t		timer_expirations,
	uint8_t		window_expiration
)
```

Parameter|Description
---------|-----------
`i_min`|The minimum trickle timer interval (50ms resolution), so: `Imin = i_min * 50ms`.
`i_doublings`|The maximum trickle timer interval expressed as number of doublings of minimum interval.
`k`|A redundancy constant.
`timer_expirations`|Number if trickle timer expires before terminating the trickle process.
`window_expiration`|Time window state is kept after the trickle process has ended in 50ms resolution.

<span class="notes">**Note**: If the `window_expiration` value is set too small, an infinite retransmission loop can occur when using the trickle multicast.</span>

### Add a new address to a multicast group

To add a new address to a multicast group:

```
uint8_t multicast_add_address
(
	unit8_t		*address_ptr,
	uint8_t		use_trickle
)
```

Parameter|Description
---------|-----------
`address_ptr`|A pointer to an array containing the address to be added.
`use_trickle`|Possible trickle values:<br>0 - No trickle multicast forwarding.<br>>0 - Trickle multicast forward is used with this address.

<dl>
<dt>Return value</dt>
<dd>0 General error.</dd>
<dd>1 Address updated.</dd>
<dd>2 Address added.</dd>
<dd>255 Link local not allowed when using multicast.</dd>
</dl>

### Free an address from a multicast group

To free an address from a multicast group:

```
uint8_t multicast_free_address
(
	uint8_t		*address_ptr
)
```

Parameter|Description
---------|-----------
`address_ptr`|A pointer to a 16 byte array that contains the address to be removed.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>
<dd>>0 Failure, an error occurred trying to remove the address.</dd>
</dl>

The following source code example shows you how to use the multicast API:

```
/*Set up 16 byte address*/
static const uint8_t multicast_address[16] =
{
	0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F
};

uint8_t add_multicast_address
(
	void
)
{

	uint8_t return_value = 0;


	/*Set new trickle multicast parameters:

		- 400 ms minimum interval (Imin)

		- 1.6 s maximum interval (Imax)

		- redundancy constant k = 8, if node receives same message
		 at least 8 time it will suppress retransmissions
		- 4 timer expirations before stopping trickle TX
		- 75 s window expiration */

	multicast_set_parameters( 8, 2, 8, 4, 150 );

	/*Try to subscribe to new site local address using trickle multicast*/
	return_value = multicast_add_address( multicast_address, 1 );

	if( return_value == 1 )
	{
		/*Address already added, update successful */
	}
	else if( return_value == 2 )
	{
		/*New address successfully added.*/
	}
	else
	{
		/*Error occurred*/
	}
	return return_value;
}
```
