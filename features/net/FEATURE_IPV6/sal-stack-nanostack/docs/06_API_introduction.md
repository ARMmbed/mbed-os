API Introduction
============

Thread protocol is a wrapping around the IPv6/6LoWPAN stack, and it is capable to work stand-alone in bare-metal implementations. In subsequent chapters of this manual, we walk through the base of the networking stack that is IPv6/6LoWPAN and proceed further to Thread.

**Figure 3-1 High level API interfaces over Networking stack**

![High level design Architecture](img/arch_general.png)



This chapter is an introduction to the 6LoWPAN stack, modules and API interfaces. It contains the following sections:

- [_About the 6LoWPAN stack_](#about-the-6lowpan-stack)
- [_Eventing system_](#eventing-system)


## About the 6LoWPAN stack

The 6LoWPAN stack is designed to be portable for various platforms and radio drivers. It uses a lightweight eventing system that allows to run on minimal design on the smallest microcontrollers even without operating systems. The application programming interfaces are designed to allow porting for new platforms. _Figure 3-2_ shows the high level APIs on the 6LoWPAN stack.

**Figure 3-2 High level API interfaces over 6LoWPAN stack**

![Bare metal 6LoWPAN](img/high_level_stack_API_interfaces.png)

The 6LoWPAN stack provides application designers with interfaces for sending and receiving packets, configuring the network and running the event base system in the form of tasklets. For more information on the interfaces, see chapters [_An Event-driven Scheduling Model_](08_API_events.md), [_Network API Definitions_](09_API_network_def.md) and [_Sockets Use and Available APIs_](11_API_sockets.md).

The 6LoWPAN stack relies on two interfaces that must be ported to run it on a specific platform. Platform API defines an abstraction layer of the underlying microcontrollers or operating system. For more information on the Platform API, see section _Platform API_ in chapter [_Porting 6LoWPAN Stack_](16_API_porting.md).

PHY driver API is an abstraction between the driver and the 6LoWPAN stack and allows 6LoWPAN network to be formed on any 802.15.4 compatible or similar radio chip. The PHY driver API is not limited to only radio chips, even serial communication may be used. For more information on the PHY driver API, see [_Device Driver API_](driver_api.md).

For more information on the 6LoWPAN stack architecture, and the protocol and application modules used by the 6LoWPAN stack, see [_ARM mbed 6LoWPAN Stack Overview_](01_overview.md).

## Eventing system

6LoWPAN stack eventing system is designed to be minimal and portable. Its main focus is on running in small embedded devices without any help of operating systems. The event-based approach allows 6LoWPAN stack based applications to be designed for extremely low power use cases where thread (not to be confused with Thread, the stack) based systems may consume too much battery life. The eventing system is designed to rely entirely on the Platform API so it is possible to port it to run on any operating system when the thread based approach is favored.

The eventing system specifies event handlers (tasklets) that form the application logic. The 6LoWPAN stack allows developers to define multiple tasklets to ease the task of writing complex applications. Each of these tasklets can then have full access to the network stack and its features through Socket, Eventing and NET APIs.

The following example shows the usage of a simple tasklet:

```
/* Main event handler */
void tasklet_main(arm_event_s *event)
{
	switch(event->event_type) {
	case ARM_LIB_TASKLET_INIT_EVENT:
		// Initialzation event is given when tasklet is created.
		// It will be called after a tasklet is registered to event OS.

		// Launch timer event to fire after a second
		timer_sys_event(MY_TX_TIMER, T_1000_MS);
		break;
	case ARM_LIB_SYSTEM_TIMER_EVENT:
		// Timer events are received in tasklet
		// Using a event type TIMER_EVENT
		// They receive 8bit identifier that
		// was passed to timer_sys_event().
		switch(event->event_id) {
		case MY_TX_TIMER:
			send_sensor_info();
			timer_sys_event(MY_TX_TIMER, T_1000_MS); //Relaunch
			break;
		}
		break;
	}
}

void main(void)
{
	int8_t tasklet_id;
	eventOS_scheduler_init();
	eventOS_event_handler_create(&tasklet_main, ARM_LIB_TASKLET_INIT_EVENT);
	eventOS_scheduler_run(); // Enter the event loop
}
```

In this code example, the main application registers an event handler called `tasklet_main`. This event handler is then used to send and receive timed events in one-second intervals. For a description of the functions and their parameters, see chapter [_An Event-driven Scheduling Model_](08_API_events.md).

