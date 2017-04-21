IPv6/6LoWPAN Stack Initialization
===============================

This chapter describes the 6LoWPAN stack initialization sequence. It contains the following sections:

- [_About initalization_](#about-initalization)
- [_Stack initialize sequence_](#stack-initialize-sequence)
- [_How to configure a network interface_](#how-to-configure-a-network-interface)

## About initalization

To bring the 6LoWPAN stack into use, follow these instructions:

1. To initialize the event OS, call `eventOS_scheduler_init()`.
2. To initialize the 6LoWPAN stack and platform, call `net_init_core()`.
3. To allocate a dynamic tasklet for an application, call `eventOS_event_handler_create()`.
4. Configure an interface and network.

## Stack initialize sequence

Perform stack initialization in the `main( )` function, as follows:

1. To initialize the event OS, call `eventOS_scheduler_init()`.
2. To initialize the 6LoWPAN stack, call `net_init_core()`.
3. To start the event dispatch loop, call `eventOS_scheduler_run();`. The 6LoWPAN stack scheduler will now run in an infinite loop.

The `eventOS_scheduler_run()` function call starts the processing of the 6LoWPAN stack events. This function is an infinite loop where the processor cannot execute any commands below the `eventOS_scheduler_run()` call.

An example 6LoWPAN stack initialization sequence:

```
void main(void)
{
	// Run any platform specific
	// initialization here
	board_init();
	// Initialize 6LoWPAN stack
	eventOS_scheduler_init();
	net_init_core();
	// Register a tasklet
	tasklet_id = eventOS_event_handler_create(&tasklet_main,
	ARM_LIB_TASKLET_INIT_EVENT);
	// Enter the event loop
	eventOS_scheduler_run();
}
```

## How to configure a network interface

To effectively use the 6LoWPAN stack, you must run at least one tasklet. Some API interfaces require tasklets for handling incoming events. The following resources must be configured from a tasklet:

- Interface:
	- When you generate an interface it is tied to the registering tasklet. If the application removes the tasklet, the interface events do not work anymore.

- Sockets:
	- When the sockets are generated, they are also tied to the tasklet used in registration.

To configure a network interface, follow these instructions:

1. To create an interface, call the function:
	* 802.15.4 `arm_nwk_interface_lowpan_init()`.
	* Ethernet or other `arm_nwk_interface_ethernet_init()`.
2. To define the 6LoWPAN bootstrap, call the function `arm_nwk_interface_configure_6lowpan_bootstrap_set()`.
3. To configure the 6LoWPAN link layer security, call the function `arm_nwk_6lowpan_link_layer_security_mode()`.
4. (Optional) To configure the 6LoWPAN address mode, call the function  `arm_nwk_6lowpan_gp_address_mode()`.
5. (Optional) To configure the 6LoWPAN node NWK scan parameters, call the function  `arm_nwk_6lowpan_link_scan_parameter_set()`.
6. (Optional) To configure the 6LoWPAN node NWK scan filters, call the functions:
	* PAN ID:  `arm_nwk_6lowpan_link_panid_filter_for_nwk_scan()`.
	* Network ID: `arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan().`
	* Protocol ID: `arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan()`.
7. (Optional) To configure the PANA, if it is activated for link layer security, call the functions:
	* Server: `arm_pana_server_library_init()`.
	* Client: `arm_pana_client_library_init()`.
8. To start the interface, call the function `arm_nwk_interface_up()`.
