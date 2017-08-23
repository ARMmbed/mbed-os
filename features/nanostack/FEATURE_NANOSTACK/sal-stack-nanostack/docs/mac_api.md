# Introduction

Nanostack has a lower level API for the IEEE 802.15.4-2006 MAC standard. This enables developers to support different MACs, be it SW or HW based solution. Nanostack offers SW MAC that you can use when your board does not have 15.4 MAC available.

## SW MAC

Nanostack includes an IEEE 802.15.4 based SW MAC class. You can use SW MAC when your board does not support MAC. To use the SW MAC service you must have a working RF driver registered to Nanostack. To create SW MAC, call the following function:

```
ns_sw_mac_create()
```

This creates a SW MAC class and sets a callback function to be used by Nanostack.

<span class="notes">**Note:** You must not call `ns_sw_mac_create()` more than once!</span>

### Initializing SW MAC

Deploy SW MAC as follows:

1. Call `arm_net_phy_register()` to register the configured [_RF driver_](driver_api.md#how-to-create-a-new-rf-driver) class to Nanostack.
2. Call `ns_sw_mac_create()` to create SW MAC with needed list sizes.
	- a sleepy device needs only 1-4 as the size of the `device_decription_table_size`.
	- the minimum and recommended `key_description_table_size` for the Thread stack is 4. (2 for 6LoWPAN)
	- the recommended value for `key_lookup_size` is 1 and for `key_usage_size` 3.
3. Call `arm_nwk_interface_lowpan_init()` to create Nanostack with the created SW MAC class. Nanostack will initialize SW MAC before using it.

### Example

See a simple code snippet for creating SW MAC with 16 as neighbour table size with three key descriptions:

```
int8_t generate_6lowpan_interface(int8_t rf_phy_device_register_id)
{
	mac_description_storage_size_t storage_sizes;
	storage_sizes.device_decription_table_size = 16;
	storage_sizes.key_description_table_size = 3;
	storage_sizes.key_lookup_size = 1;
	storage_sizes.key_usage_size = 3;
	mac_api_t *mac_api = ns_sw_mac_create(rf_phy_device_register_id, &storage_sizes);
	if (!mac_api) {
		tr_error("Mac create fail!");
		return -1;
	}
	return arm_nwk_interface_lowpan_init(mac_api, "6LoWPAN_ROUTER");
}
```

### Enabling [_FHSS_](12_API_network.md#channel-hopping-mode)

SW MAC supports FHSS. To enable it, you need to do the following:

1. Call `arm_net_phy_register()` to register the configured RF driver class to Nanostack. 
2. Call `ns_sw_mac_create()` to create SW MAC with needed list sizes.
3. Call `ns_fhss_create()` to configure and define the FHSS class.
4. Call `ns_sw_mac_fhss_register()` to register FHSS to SW MAC.
5. Call `arm_nwk_interface_lowpan_init()` to create Nanostack with the created SW MAC class.

## IEEE 802.15.4 MAC sublayer APIs

The stack uses the IEEE 802.15.4 defined MAC management service entity (MLME-SAP) and MAC data service (MCPS-SAP) interfaces. MAC API follows MCPS and MLME primitives defined by the IEEE 802.15.4-2006 standard.

The following primitives are used in MAC layer:

| Primitive | Despription |
| --------- | ----------- |
| Request | Request made by service user. |
| Confirm | MAC layer response to earlier request. |
| Indication | Indication event from MAC to service user. |
| Response | Service user's response to received indication. |

MAC API is defined in the following header files:

- `mac_api.h` Main header which defines a transparent MAC API for Nanostack to use.
- `mlme.h` Definitions for MLME-SAP primitives.
- `mac_mcps.h` Definitions for MCPS-SAP primitives.
- `mac_common_defines.h` Definitions for common MAC constants.

### MCPS-SAP interface

MCPS-SAP defines 802.15.4 data flow API with the following primitives:

| Primitive | Despription |
| --------- | ----------- |
| MCPS-DATA-REQ | Data request primitive to MAC. |
| MCPS-DATA-CONF | MAC generated confirmation for ongoing MCPS-DATA-REQ. |
| MCPS-DATA-IND | MAC generated data indication event. |
| MCPS-PURGE-REQ | Cancel ongoing MCPS-DATA-REQ from MAC. |
| MCPS-PURGE-CONF | Confirmation from MAC to MCPS-PURGE-REQ operation. |

### MLME-SAP interface

MLME-SAP defines a set of different management primitives and this chapter introduces both supported and unsupported primitives in Nanostack.

#### Supported MLME APIs

MLME-SAP primitives used by Nanostack:

| Primitive | Despription |
| --------- | ----------- |
| MLME-BEACON-NOTIFY | MAC generated event for received beacons.|
| MLME-GET-REQ | Request information about a specified PAN Information Base (PIB) attribute. |
| MLME-GET-CONF | MAC generated response to MLME-GET-REQ. |
| MLME-RESET-REQ | Request to reset MAC to idle state and clean data queues. |
| MLME-SCAN-REQ | Start MAC scan process. Orphan scan is not supported. |
| MLME-SCAN-CONF | Result of the scan made by MLME-SCAN-REQ. |
| MLME-COMM-STATUS-IND | MAC generated indication about the communications status. |
| MLME-SET-REQ | Request to write data into a specified PIB attribute. |
| MLME-SET-CONF | MAC generated response to MLME-SET-REQ. |
| MLME-START-REQ | Starts or enables MAC with specified options. Nanostack uses this also for RFD device. |
| MLME-SYNCH-LOSS-IND | Indicate syncronization loss from wireless PAN. Only used by SW MAC when [_FHSS_](12_API_network.md#channel-hopping-mode) is in use! |
| MLME-POLL-REQ | Request MAC to do data poll to parent. |

#### Non-supported MLME APIs

Unsupported MLME-SAP primitives:

| Primitive | Support planned | Despription |
| --------- | --------------- | ----------- |
| MLME-ASSOCIATE-REQ | Not yet | Start MAC association process. |
| MLME-ASSOCIATE-CONF | Not yet | MAC association process confirmation status. |
| MLME-ASSOCIATE-IND | Not yet | MAC association indication to indicate the reception of assocation request. |
| MLME-ASSOCIATE-RES | Not yet | MAC association response for indication. |
| MLME-DISASSOCIATE-REQ | Not yet | MAC disassociation request from service user. |
| MLME-DISASSOCIATE-IND | Not yet | MAC disassociation indication event to service user. |
| MLME-DISASSOCIATE-CONF | Not yet | MAC disassociation confirmation when the disassociation request is handled. |
| MLME-GTS-REQ | Not yet | MAC Guaranteed Time Slot (GTS) request. |
| MLME-GTS-IND | Not yet | MAC GTS allocate event indication. |
| MLME-GTS-CONF | Not yet | MAC GTS request confirmation. |
| MLME-ORPHAN-IND | Not yet | Service user indicated by orphaned device. |
| MLME-ORPHAN-RES | Not yet | Service user response to orphan indication event. |
| MLME-RESET-CONF | Yes | MAC reset confirmation. |
| MLME-RX-ENABLE-REQ | Yes | Enable (or disable) RX receiver for a specified time. |
| MLME-RX-ENABLE-CONF | Yes | Confirmation for MLME-RX-ENABLE-REQ. |
| MLME-START-CONF | Yes | Confirmation for MLME start request. |
| MLME-SYNCH-REQ | Not yet | Request MAC to synchronize with coordinator. |

### MAC API class introduction

This chapter introduces MAC mesh interface `mac_api_s`. It is a structure that defines the function callbacks needed by a service user.

The base class defines the functions for two-way communications between an external MAC and service user. The application creates a `mac_api_s` object by calling the MAC adapter's create function. The newly created object is then passed to Nanostack which initializes its own callback functions by calling `mac_api_initialize()` function. A service user operates MAC by calling MLME or MCPS primitive functions.

The MAC API class structure `mac_api_t` is defined as below:

```
typedef struct mac_api_s {
	//Service user defined initialization function which is called when Nanostack takes MAC into use
    mac_api_initialize 				*mac_initialize;
    //MAC adapter function callbacks for MLME & MCPS SAP
    mlme_request 					*mlme_req;
    mcps_data_request 				*mcps_data_req;
    mcps_purge_request  				*mcps_purge_req;
    //Service user defined function callbacks
    mcps_data_confirm 				*data_conf_cb;
    mcps_data_indication				*data_ind_cb;
    mcps_purge_confirm      			*purge_conf_cb;
    mlme_confirm            			*mlme_conf_cb;
    mlme_indication         			*mlme_ind_cb;
    //MAC extension API for service user
    mac_ext_mac64_address_set   		*mac64_set;
    mac_ext_mac64_address_get   		*mac64_get;
    mac_storage_decription_sizes_get *mac_storage_sizes_get;
    int8_t                      		parent_id;
    uint16_t                    		phyMTU;
};
```

Member|Description
------|-----------
`mac_initialize` | MAC initialize function called by Nanostack.
`mlme_req` | MLME request function to use MLME-SAP commands, MAC defines.
`mcps_data_req` | MCPS data request function to use, MAC defines.
`mcps_purge_req` | MCPS purge request function to use, MAC defines.
`mcps_data_confirm` | MCPS data confirm callback function, service user defines.
`data_ind_cb` | MCPS data indication callback function, service user defines.
`purge_conf_cb` | MCPS purge confirm callback function, service user defines.
`mlme_conf_cb` | MLME confirm callback function, service user defines.
`mlme_ind_cb` | MLME indication callback function, service user defines.
`mac64_set` | MAC extension function to set mac64 address.
`mac64_get` | MAC extension function to get mac64 address.
`mac_storage_sizes_get` | Getter function to query data storage sizes from MAC.
`parent_id` | Service user ID used to indentify the MAC service user. Optional.
`phyMTU` | Maximum Transmission Unit (MTU) used by MAC. Standard 802.15.4 MAC must set 127.

### MAC API standard extensions

This chapter introduces MAC API standard extensions.

#### MAC 64-bit address set and get

NanoStack uses 64-bit address set and get. There are two 64-bit addresses available: 

- NVM EUI64.
- dynamic 64-bit address used at the link layer. 

Thread generates a random MAC64 after commissioning. Therefore, MAC and the RF driver must support updating of radio's dynamic 64-bit address anytime.

Address set and get support two different 64-bit addresses:

| Address enumeration type | Despription |
| -------------------------- | ----------- |
| MAC_EXTENDED_READ_ONLY | A unique EUI64. |
| MAC_EXTENDED_DYNAMIC | Dynamic 64-bit address. Same as EUI64 after boot. |

#### MAC max storage information get

Usually, HW MAC and SW MAC have static keys and neighbour list sizes. Nanostack always asks the max size to limit its neighbour table size. The service user must define the `mac_storage_sizes_get()` function returning the following values:

- MAC Device description list size (must be > 1).
- MAC Key description list size (must be > 1).

<span class="notes">**Note:** The Key description list size must at least 4 if using Thread!</span>

#### MLME attribute extension

Nanostack uses MLME attribute extensions which have to be ported to the HW MAC adapter. To configure the extensions, use the `MLME-SET-REQ` command.

| Enumeration type | Value | Despription |
| ---------------- | ----- | ----------- |
| `macLoadBalancingBeaconTx` | `0xfd` | Trigger to MAC layer to send a beacon. Called by the [_load balancer_](17_API_load_balancer.md) module periodically. |
| `macLoadBalancingAcceptAnyBeacon` | `0xfe` | Configure MAC layer to accept beacons from other networks. Enabled by [_load balancer_](17_API_load_balancer.md), default value is `False`. Value size boolean, `true=enable`, `false=disable`. |
| `macThreadForceLongAddressForBeacon` | `0xff` | The Thread standard forces beacon source address to have an extended 64-bit address. |

#### Thread Sleepy End Device (SED) keepalive extension

Thread 1.1 stack defines that sleepy end device data poll process must enable neighbour table keepalive functionality as well. When SED finishes data polling succesfully, it updates its parents keepalive value in a neighbour table. A service user at a parent device does not have a standard mechanism to indicate the data polling event. Therefore, the MAC layer must generate an `MLME-COMM-STATUS` indication callback with status `MLME_DATA_POLL_NOTIFICATION`.

Enumeration extension for MLME communication status enumeration:

| Enumeration type | Value | Despription |
| ---------------- | ----- | ----------- |
| MLME_DATA_POLL_NOTIFICATION | `0xff` | Thread requirement for MLME-COMM-STATUS to start indicating the successful data poll events. |

## HW MAC

To use HW MAC, you need to create an adapter class that links function calls between Nanostack and HW MAC. To create the adapter class, you need to implement the functions defined in the `mac_api_s` structure. When HW MAC generates an event the adapter must handle it and do a parameter adaptation before calling the correct function from the `mac_api_s` structure. You may need the same parameter adaptation for requests from Nanostack to HW MAC.

<span class="notes">**Note:** Function calls from Nanostack to HW MAC must be non-blocking in the adapter layer!</span>

 
