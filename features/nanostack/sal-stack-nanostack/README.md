ARM IPV6/6LoWPAN stack.
=======================

This repository contains the ARM IPv6/6LoWPAN/Thread Stack for mbed OS.

![](docs/img/thread_certified.png)

mbed OS is now a Thread Certified Component. Using IPv6 with 6LoWPAN as the foundation, Thread technology provides a low-power, self-healing mesh network designed for the home.

The documentation is hosted in [here](https://os.mbed.com/docs/v5.6/tutorials/6lowpan-mesh.html).

On mbed OS, usage is through [mbed Mesh API](https://os.mbed.com/docs/v5.6/reference/mesh.html) and [Socket API](https://os.mbed.com/docs/v5.6/reference/network-socket.html).

To see, how the 6LoWPAN Stack works, check the example application [mbed-os-example-mesh-minimal](https://github.com/ARMmbed/mbed-os-example-mesh-minimal).

## License

The software is partially provided under a Apache 2.0 license and partially BSD-3-Clause as described below.

This project also contains code from other projects as listed below. The original license text is included in those source files.

* Public header files are provided under Apache 2.0 license.
* Source code excluding Thread protocol, is provided under Apache 2.0
* Source code implementing Thread protocol is provided under BSD-3-Clause
* mDNS library is from FNET project and provided under Apache 2.0 license.
