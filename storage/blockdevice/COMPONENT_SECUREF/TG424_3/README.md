Getting started
==================

JEDEC have published the [JESD261 standard](https://www.jedec.org/standards-documents/docs/jesd261), titled Serial NOR Security
Hardware Abstraction Layer(HAL). This standard defines a software layer to provide
a uniform interface for cryptographic operations on compliant Secure Flash devices.

JESD261 HAL enables manufacturer-neutral communication with secure Flash devices.
Security features introduced in JESD261 include session-based communication, secure program
and erase operations, as well as partitioning of the Flash device into protected regions,
that are available through an application interface. JESD261 defines rules for implementing
this application interface to simplify the use and evaluation of these common security features
accross secure Flash vendors.

**************
## Design concept

JEDEC security HAL module could be compiled for use within different OSes and software platforms.
In this design, this module consits of JEDEC security HAL API layer and vendor specific implementation layer.

JEDEC security HAL API layer implements the interfaces defined in JEDEC JESD261 standard.
And calls the pre-binding specific sub-steps implemented in vendor specific implementation layer.
Besides, the JEDEC security HAL API layer calls crypto services via vendor specific crypto wrapper functions.
Currently, mbedtls library and TF-M PSA crypto services are both supported.

Vendor specific implementations layer provides specific sub-steps implementaion required by
JEDEC security HAL API layer. In addition, this layer also includes the specific implementation
of secure Flash provisioning which should be performed before deployment.

Code structure
--------------
```bash
TG424_3/
├─JEDEC_security_HAL    --------------------------------> JEDEC security HAL API layer
│  │  crypto_wrapper.h    ------------------------------> Header file for crypto wrapper functions
│  │  jedec_security_hal.c   ---------------------------> JEDEC security HAL API implementation
│  │  jedec_security_hal.h   ---------------------------> Header file for JEDEC security HAL API layer
│  │  queue.c   ----------------------------------------> Queue operations implementation
│  │  queue.h   ----------------------------------------> Queue operations definition
│  │  vendor_security_impl.h  --------------------------> Header file for vendor specific implementation of security HAL sub-steps
│  │
│  └─include
│
└─vendor_impl  -----------------------------------------> Flash vendors specific implementation
    │  vendor_provisioning_impl.h    -------------------> Header file for vendor specific secure flash provisioning operations
    │  vendor_secureflash.h   --------------------------> Vendor secure flash definitions
    │  vendor_secureflash_defs.h  ----------------------> Vendor secure flash informations
    │
    ├─macronix
    │  ├─armorflash_mx75   -----------------------------> The specific implementations for Macronix MX75 ArmorFlash
    │  │
    │  └─armorflash_mx78   -----------------------------> The specific implementations for Macronix MX78 ArmorFlash
    │      │  libmx78_armor_lib.a   --------------------> Binary library of MX78 ArmorFlash driver
    │      │  mx78_armor.c   ---------------------------> The specific implementations of JEDEC secuiry HAL sub-steps for Macronix MX78 ArmorFlash
    │      │  mx78_armor.h
    │      │  mx78_armor_lib.h
    │      │  secureflash_layout.h   -------------------> The specific parameters and secure region layout of MX78 ArmorFlash 
    │      │
    │      ├─crypto_wrapper  ---------------------------> The specific crypto wrapper functions for Macronix MX78 ArmorFlash
    │      │      crypto_wrapper_mbedtls.c   -----------> The specific crypto wrapper functions based on mbedtls library
    │      │      crypto_wrapper_psa.c    --------------> The specific crypto wrapper functions based on Trusted Firmware-M crypto service
    │      │
    │      └─provisioning   ----------------------------> The specific provisioning implementation for Macronix MX78 ArmorFlash
    │              libmx78_armor_provision_lib.a  ------> Binary library of MX78 ArmorFlash provisioning driver
    │              mx78_armor_provision.h
    │
    └─vendor_template   --------------------------------> Reserved vendor specific implementation for other secure Flash vendors reference
```
********
## Examples

This JEDEC security HAL implementation has been integrated and tested with following OSes and software platforms.

- [Arm Mbed OS](https://www.arm.com/products/development-tools/embedded-and-software/mbed-os)

For Arm Mbed OS, JEDEC security HAL is integrated in the COMPONENT_SECUREF. The source code is available on [github](https://github.com/macronix/).

```
git clone -b macronix_secureflash_TG424_3 --recursive https://github.com/macronix/mbed-os
```



- [Arm Trusted Firmware-M](https://tf-m-user-guide.trustedfirmware.org/index.html)

For Arm Trusted Firmware-M, JEDEC security HAL is integrated in the ETSS partition which provides solution for supporting secure Flash in Arm Trusted Firmware-M framework. The source code is available on [github](https://github.com/macronix/).

```
git clone -b macronix_secureflash_TG424_3 --recursive https://github.com/macronix/tf-m-extras
```


