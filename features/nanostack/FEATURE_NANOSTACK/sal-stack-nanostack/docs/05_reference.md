#ARM mbed Networking Stack API Reference Manual

## About this manual

This manual describes the APIs regarding the ARM mbed Networking stack, for example, 6LoWPAN related APIs, Thread stack related APIs, General network related APIs, Socket APIs, Event OS APIs, Platform API, and Driver API.

As Thread runs on top of the 6LoWPAN stack, most of the networking stack functionality is shared. However, when building the stack you can cherry-pick the features, configurations or functionalities. In such case, please refer to [Build time configurations](quick_start_build.md).

To read even more of the APIs, check the [Doxygen documentation](https://docs.mbed.com/docs/arm-ipv66lowpan-stack/en/latest/api/index.html).

## Intended audience

This manual has been written for experienced software developers to assist with using the ARM software product range.

## Using this manual

The reference manual contains several chapters; you can read them in sequence, but they can also be read as stand-alone. They are:

1. [**Introduction to APIs**](06_API_introduction.md): Read this to understand more about the 6LoWPAN stack from API perspective.

2. [**IPv6/6LoWPAN Stack Initialization**](07_API_initialize.md): Read this to understand how to initialize the IPv6/6LoWPAN stack.

3. [**An Event-driven Scheduling Model**](08_API_events.md): Read this to understand more about the event-driven scheduling model. If you are using ARM mbed OS, skip this.

4. [**Platform API**](platform_API.md): Brief description of Platform API. Detailed discussion in Porting section.

5. [**Device Driver API**](driver_api.md): Brief description of Device Driver API. Detailed discussion in Porting section.


6. [**Network API Definitions**](09_API_network_def.md): Read this to understand more about the network API definitions.

7. [**Library Timer API**](10_API_timer.md): Read this to understand more about the timer functionality.

8. [**Sockets Use and Available APIs**](11_API_sockets.md): Read this to understand more about socket use and the available APIs.

9. [**The Network Control Layer**](12_API_network.md): Read this to understand the functions of the network control layer.

10. [**Dynamic Memory API**](13_API_memory.md): Read this for an introduction to the dynamic memory API of the 6LoWPAN stack.

11. [**Data Structures, Types and Variables**](14_API_data.md): Read this to understand the data structure, types and variables used in the socket and RF layers.

12. [**Debug Statistics Interface API**](15_API_debug.md): Read this to understand the debug statistics interface API.

13. [**Thread Related APIs**](thread_APIs.md):Read this to familiarize yourself with the Thread protocol related APIs.

14. [**Load balancing API**](17_API_load_balancer.md):Read this to familiarize yourself with the Load balancing API

15. [**Warm restart**](18_warm_restart.md):Read this to familiarize yourself with the Warm restart feature


