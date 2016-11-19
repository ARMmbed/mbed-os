Configuring the network
=======================

The Nanostack configuration has a huge impact on the network behaviour. Due the number of different network types possible, the configuration must always planned to match the topology. Appropriate default values cannot always be provided.

The 6LoWPAN network consist of different protocols. Each of them has a different scope on the network bootsrap.
The following list provides examples of the parameters you need to consider when designing a network:

* IEEE 802.15.4 Beacons
    * Protocol ID
 	* PAN ID
 	* Network ID
* Node type: Router/Host/sleepy host.
* Network prefix
* 6LoWPAN Neighbor Discovery (6LoWPAN-ND)
    * Address mode (GP64/GP16)
 	* Neighbor discovery probes
* Mesh Link Establishment (MLE)
    * Lifetimes
* Routing Protocol for Low power and Lossy networks (RPL)
    * Lifetimes
* Security
    * Autheticating nodes
    * Link-layer security
    * Transport layer security

Configuring lifetime values for routing related protocols is important. In general, the faster the network reacts to failures, the more bandwidth it uses for keep alive messages and RPL+MLE signalling.

Most of the configuration values must be set when initializing the stack, before starting the network bootstrap. Refer to section [IPv6/6LoWPAN Stack Initialization](07_API_initialize.md) for more information.

### Beacon information

Nanostack uses 802.15.4 beacons to carry information, such as protocol ID, PAN ID and network ID, that needs to be configured by the user. This beacon payload can then be used for filtering.

The 6LoWPAN nodes use beacons to find the specific network that the device wants to join.
All configurable fields must be set on the border router. On the node side, these fields can be used to filter the beacons to find the specific network you want to join.

On the border router, the beacon payload information is part of the `border_router_setup_s` structure that is used when initializing the stack:

```
int8_t arm_nwk_6lowpan_border_router_init(int8_t interface_id, border_router_setup_s *border_router_setup_ptr);
```

#### Protocol ID

ZigbeeIP defines the protocol ID to be `2` and we recommend to leave this as default.

This is a mandatory setting on the border router, as it is a part of the `border_router_setup_s` structure.

On the 6LoWPAN nodes, you can filter beacons by protocol ID. To enable the filtering, call:

```
int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan(int8_t interface_id, uint8_t protocol_ID);
```

#### PAN ID

PAN ID is a locally unique identifier for the network. On the same range, different 6LoWPAN networks must use
different PAN IDs.

On the border router, `mac_panid` is part of the `border_router_setup_s` structure and must be set to a value between `1` and `0xfffe`.

To enable the PAN ID filtering on the 6LoWPAN nodes, call:

```
int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(int8_t interface_id, uint16_t pan_id_filter);
```

#### Network ID

Network ID is analogous to WiFi network names. It is a 16 bytes-long binary string.

On the border router, the field `uint8_t network_id[16]` is part of the `border_router_setup_s` structure.

To enable filtering on the node side, call:

```
int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(int8_t interface_id, const uint8_t *nwk_id_filter);
```

### Selecting the node type

When initializing the stack, a node must call:

```
int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension);
```

Where the `bootstrap_mode` can be one of the following:

* NET_6LOWPAN_BORDER_ROUTER
* NET_6LOWPAN_ROUTER
* NET_6LOWPAN_HOST
* NET_6LOWPAN_SLEEPY_HOST

### Configuring mesh routing

The RPL protocol is required to route unicast traffic across meshes. On a multihop mesh network,
the border router must configure RPL by calling:

```
int8_t arm_nwk_6lowpan_rpl_dodag_init(int8_t interface_id, const uint8_t *dodag_id, const dodag_config_t *config, uint8_t instace_id, uint8_t flags);
```

### Configuring for different network types

Following sections provide you with recommended configuration values for specific network types.

See the API documentation for [net_6lowpan_parameter_api.h](https://docs.mbed.com/docs/arm-ipv66lowpan-stack/en/latest/api/net__6lowpan__parameter__api_8h.html) to learn more about the Nanostack configuration parameters.

#### A sensor network for an apartment

This can be a simple network with a single border router, and the remaining nodes being hosts or sleepy hosts. The star configuration does not use RPL for multihop routing - all nodes communicate directly with the border router. MPL can still be used to provide broadcast resiliance.

* MLE:
  * Router lifetime = 128 seconds
  * Host lifetime = 400 seconds
* MPL:
  * Imin = 2 (100 ms)
  * Idoublings = 2 (so Imax = 400 ms)
  * Timer Expirations = 3 for border router, 0 for hosts
  * k = 10
  * Window Expiration = 100 (5 seconds)
* RPL:
  * n/a

#### A sensor/actuator network for a building

This type of network has a mixture of routers and nodes - one or more routers are required to span multiple hops from the border router to outlying nodes. Bandwidth is not critical, and timing is chosen to give routing responsiveness at a sub-hour scale.

* MLE:
  * Router lifetime = 128 seconds
  * Host lifetime = 400 seconds
* MPL:
  * Imin = 11 (550 ms)
  * Idoublings = 0 (so Imax = Imin)
  * Timer Expirations = 3 for routers, 0 for hosts
  * k = 255
  * Window Expiration = 100 (5 seconds)
* RPL:
  * Objective Code Point = 1 (MRHOF)
  * Min Hop Rank Increase = 128
  * DAG Max Rank Increase = 2048
  * DIO Interval Min = 9 (512 ms)
  * DIO Interval Doublings = 12 (so maximum interval = 2097 s = 35 min)
  * DIO Redundancy Constant = 3
  * Path Control Size = 5
  * Lifetime Unit = 60 (1 minute)
  * Default Lifetime = 120 (2 hours)
  
Note that MLE link maintenance advertisements are sent roughly 4 times per lifetime interval, so 3 or 4 consecutive missed adverts will lead to link loss.

RPL allows DIO interval and lifetime to be separately configured, so they need to be chosen proportionately, so that it takes at least a few consecutive missed DIO intervals (here 35 min) for the lifetime to expire (here 2 hours).

#### A sensor/actuator network for a city

This can be a wide-area network with hundreds of nodes - most or all of them being powered routers to allow the mesh to configure its topology in the most flexible way using RPL.

Minimizing routing and link maintenance overhead can be important at this scale, and real-time control is not critical. As such, the timing parameters will be set much longer, with more than an hour between refreshes. (Node refresh interval can be even longer).

* MLE:
  * Router lifetime = 320 seconds
  * Host lifetime = 600 seconds
* MPL:
  * Imin = 20 (1 s)
  * Idoublings = 0 (so Imax = Imin)
  * Timer Expirations = 2
  * k = 3
  * Window Expiration = 200 (10 seconds)
* RPL:
  * Objective Code Point = 1 (MRHOF)
  * Min Hop Rank Increase = 128
  * DAG Max Rank Increase = 4096
  * Dio Interval Min = 12 (4 s)
  * Dio Interval Doublings = 11 (so maximum interval = 8389 s = 2.3 hours)
  * Dio Redundancy Constant = 10
  * Path Control Size = 3
  * Lifetime Unit = 600 (10 minutes)
  * Default Lifetime = 72 (12 hours)


## Enabling the security

Nanostack allows you to select from the following link layer security features:

* No security.
* PSK network authentication.
* PANA network authentication.

To learn how to enable the security features, see the [The Network Layer Control APIs](12_API_network.md).
