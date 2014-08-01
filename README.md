mbed SDK
========

[![Build Status](https://travis-ci.org/mbedmicro/mbed.png)](https://travis-ci.org/mbedmicro/mbed/builds)

The mbed Software Development Kit (SDK) is a C/C++ microcontroller software platform relied upon by tens of thousands of
developers to build projects fast.

The SDK is licensed under the permissive Apache 2.0 licence, so you can use it in both commercial and personal projects
with confidence.

The mbed SDK has been designed to provide enough hardware abstraction to be intuitive and concise, yet powerful enough
to build complex projects. It is built on the low-level ARM CMSIS APIs, allowing you to code down to the metal if needed.
In addition to RTOS, USB and Networking libraries, a cookbook of hundreds of reusable peripheral and module libraries
have been built on top of the SDK by the mbed Developer Community.

Documentation
-------------
* [Tools](http://mbed.org/handbook/mbed-tools): how to setup and use the build system.
* [mbed library internals](http://mbed.org/handbook/mbed-library-internals)
* [Adding a new target microcontroller](http://mbed.org/handbook/mbed-SDK-porting)

Supported Microcontrollers and Boards
-------------------------------------
View all on the [mbed Platforms](https://mbed.org/platforms/) page.

NXP:
* [mbed LPC1768](http://mbed.org/platforms/mbed-LPC1768/) (Cortex-M3)
* [u-blox C027 LPC1768](http://mbed.org/platforms/u-blox-C027/) (Cortex-M3)
* [mbed LPC11U24](http://mbed.org/platforms/mbed-LPC11U24/) (Cortex-M0)
* [EA LPC11U35](http://mbed.org/platforms/EA-LPC11U35/) (Cortex-M0)
* mbed LPC2368 (ARM7TDMI-S)
* LPC810 (Cortex-M0+)
* [LPC812](http://mbed.org/platforms/NXP-LPC800-MAX/) (Cortex-M0+)
* [EA LPC4088](http://mbed.org/platforms/EA-LPC4088/) (Cortex-M4F)
* LPC4330 (Cortex-M4F + Cortex-M0)
* [LPC1347](http://mbed.org/platforms/DipCortex-M3/) (Cortex-M3)
* [LPC1114](http://mbed.org/platforms/LPC1114FN28/) (Cortex-M0)
* LPC11C24 (Cortex-M0)
* [LPC1549](https://mbed.org/platforms/LPCXpresso1549/) (Cortex-M3)
* [LPC800-MAX](https://mbed.org/platforms/NXP-LPC800-MAX/) (Cortex-M0+)
* [DipCortex-M0](https://mbed.org/platforms/DipCortex-M0/) (Cortex-M0)
* [DipCortex-M3](https://mbed.org/platforms/DipCortex-M3/) (Cortex-M3)
* [BlueBoard-LPC11U24](https://mbed.org/platforms/BlueBoard-LPC11U24/) (Cortex-M0)
* LPCCAPPUCCINO (Cortex-M0)

Freescale:
* FRDM-K20D50M (Cortex-M4)
* [FRDM-KL05Z](https://mbed.org/platforms/FRDM-KL05Z/) (Cortex-M0+)
* [FRDM-KL25Z](http://mbed.org/platforms/KL25Z/) (Cortex-M0+)
* [FRDM-KL46Z](https://mbed.org/platforms/FRDM-KL46Z/) (Cortex-M0+)
* [FRDM-K64F](https://mbed.org/platforms/FRDM-K64F/) (Cortex-M4F)

STMicroelectronics:
* [Nucleo-F103RB](https://mbed.org/platforms/ST-Nucleo-F103RB/) (Cortex-M3)
* [Nucleo-L152RE](https://mbed.org/platforms/ST-Nucleo-L152RE/) (Cortex-M3)
* [Nucleo-F030R8](https://mbed.org/platforms/ST-Nucleo-F030R8/) (Cortex-M0)
* [Nucleo-F401RE](https://mbed.org/platforms/ST-Nucleo-F401RE/) (Cortex-M4F)
* [Nucleo-F411RE](https://mbed.org/platforms/ST-Nucleo-F411RE/) (Cortex-M4F)
* [Nucleo-F302R8](https://mbed.org/platforms/ST-Nucleo-F302R8/) (Cortex-M4F)
* STM32F4XX (Cortex-M4F)
* STM32F3XX (Cortex-M4F)
* STM32F0-Discovery (Cortex-M0)
* STM32VL-Discovery (Cortex-M3)
* STM32F3-Discovery (Cortex-M4F)
* STM32F4-Discovery (Cortex-M4F)


Nordic:
* [nRF51822-mKIT](https://mbed.org/platforms/Nordic-nRF51822/) (Cortex-M0)

Supported Toolchains and IDEs
-----------------------------
* GCC ARM: [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded/4.7/4.7-2012-q4-major)
* ARMCC (standard library and MicroLib): [uVision](http://www.keil.com/uvision/)
* IAR: [IAR Embedded Workbench](http://www.iar.com/en/Products/IAR-Embedded-Workbench/ARM/)
* GCC code_red: [Red Suite](http://www.code-red-tech.com/)
* GCC CodeSourcery: [Sourcery CodeBench](http://www.mentor.com/embedded-software/codesourcery)

API Documentation
-----------------
* [RTOS API](http://mbed.org/handbook/RTOS)
* [TCP/IP Socket API](http://mbed.org/handbook/Socket) (Transports: Ethernet, WiFi, 3G)
* [USB Device API](http://mbed.org/handbook/USBDevice)
* [USB Host API](http://mbed.org/handbook/USBHost)
* [DSP API](http://mbed.org/users/mbed_official/code/mbed-dsp/docs/tip/)
* Flash File Systems: [SD](http://mbed.org/handbook/SDFileSystem), [USB MSD](http://mbed.org/handbook/USBHostMSD), [semihosted](http://mbed.org/handbook/LocalFileSystem)
* [Peripheral Drivers API](http://mbed.org/handbook/Homepage)

Community
---------
For discussing the development of the mbed SDK itself (Addition/support of microcontrollers/toolchains, build and test system, Hardware Abstraction Layer API, etc) please join our [mbed-devel mailing list](https://groups.google.com/forum/?fromgroups#!forum/mbed-devel).

For every topic regarding the use of the mbed SDK, rather than its development, please post on the [mbed.org forum](http://mbed.org/forum/), or the [mbed.org Q&A](http://mbed.org/questions/).

For reporting issues in the mbed libraries please open a ticket on the issue tracker of the relevant [mbed official library](http://mbed.org/users/mbed_official/code/).
