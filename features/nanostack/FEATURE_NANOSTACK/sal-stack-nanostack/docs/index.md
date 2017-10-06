ARM mbed Networking Stack
===============================

Welcome to the documentation of ARM mbed Networking Stack. It is a robust, scalable and self-healing mesh networking stack for low power, less complex constrained devices. Such devices are the fundamental building blocks needed to bring a full fledge Internet of Things (IoT) to life.

ARM mbed Networking Stack is an enabler technology for IoT allowing rapid protoyping or product-to-market practices. It can be run as a bare-metal stack as well as with ARM mbed OS, an open source embedded operating system designed specifically for the _things_ in IoT.

The stack provides many state-of-the-art features and legacy support. However, as the focus here is upon spreading out and enabling IoT, IPv6 based enabler protocols are emphasized. The key features of the stack are listed below.

## Key features

Feature|Description
-----------|-----------
`IPv6/6LoWPAN stack`|IPv6 support and 6LoWPAN adaptation layer.
`Thread stack`|Runs on the top of IPv6/6LoWPAN. Specialized protocol for M2M communication in Connected Home applications.
`IPv4 Stack`|Not discussed in this document.
`Mesh link establishment (MLE) `|For safe, dynamic and reconfigurable radio link establishment.
`RPL routing protocol `|A specialized routing protocol for constrained devices.
`Security modules`|PANA, EAP, PSK, ECC, DTLS, TLS.

This document is divided into following sections:

* [6LoWPAN Quick Start Guide](quick_start_intro.md)
* [ARM mbed IPv6/6LoWPAN Stack Overview](01_overview.md)
* [ARM mbed Thread Stack Overview](thread_intro.md)
* [API Reference Manual](05_reference.md)
* [Porting ARM mbed Networking Stack](16_API_porting.md)

