[![Mbed OS][mbed-os-logo]][mbed-os-link]

[![Build status release][mbed-travis-release-svg]][mbed-travis-release] 
[![Build status master][mbed-travis-master-svg]][mbed-travis-master] 
[![Tools coverage status][mbed-coveralls-tools-svg]][mbed-coveralls-tools] 
[![PR progress][mbed-waffle-svg]][mbed-waffle] 

[mbed-os-logo]: logo.png
[mbed-os-link]: https://www.mbed.com/en/platform/mbed-os/
[mbed-travis-master]: https://travis-ci.org/ARMmbed/mbed-os
[mbed-travis-master-svg]: https://travis-ci.org/ARMmbed/mbed-os.svg?branch=master
[mbed-travis-release]: https://travis-ci.org/ARMmbed/mbed-os/branches
[mbed-travis-release-svg]: https://travis-ci.org/ARMmbed/mbed-os.svg?branch=latest
[mbed-coveralls-tools]: https://coveralls.io/github/ARMmbed/mbed-os?branch=master
[mbed-coveralls-tools-svg]: https://coveralls.io/repos/github/ARMmbed/mbed-os/badge.svg?branch=master
[mbed-waffle]: https://waffle.io/ARMmbed/mbed-os
[mbed-waffle-svg]: https://badge.waffle.io/ARMmbed/mbed-os.svg?columns=all

Arm Mbed OS is an open source embedded operating system designed specifically for the "things" in the Internet of Things. It includes all the features you need to develop a connected product based on an Arm Cortex-M microcontroller, including security, connectivity, an RTOS and drivers for sensors and I/O devices.

Mbed OS provides a platform that includes:

- Security foundations.
- Cloud management services.
- Drivers for sensors, I/O devices and connectivity. 

## Release notes
The [release notes](https://os.mbed.com/releases) detail the current release. You can also find information about previous versions.

## License and contributions 

The software is provided under [Apache-2.0 license](LICENSE). Contributions to this project are accepted under the same license. Please see [contributing.md](CONTRIBUTING.md) for more info. 

This project contains code from other projects. The original license text is included in those source files. They must comply with our [license guide](https://os.mbed.com/docs/latest/reference/license.html)

Folders containing files not under Apache 2.0 as listed above. The original license text is included in those source files.

- cmsis - MIT/BSD-3-Clause
- components/802.15.4_RF/mcr20a-rf-driver - BSD-3-Clause
- features/cryptocell/FEATURE_CRYPTOCELL310 - BSD-style
- features/FEATURE_BOOTLOADER - PBL
- features/FEATURE_BLE/targets - BSD-style/PBL/MIT
- features/lorawan - Revised BSD
- features/lwipstack - BSD-3-Clause/MIT
- features/nanostack/sal-stack-nanostack - BSD-3-Clause
- features/storage/FEATURE_STORAGE - BSD-style
- features/storage/blockdevice - MIT
- features/storage/filesystem/littlefs - BSD-3-Clause
- features/storage/filesystem/fat - MIT/BSD-style
- features/netsocket/emac-drivers - BSD-3-Clause
- features/frameworks/unity/unity - MIT
- rtos - MIT
- drivers - MIT
- TESTS/mbed_hal/trng/pithy - BSD-3-Clause
- tools/data/rpc - MIT
- targets - PBL/BSD-style/MIT/Zlib

## Getting started for developers
 
We have a [developer website](https://os.mbed.com) for asking questions, engaging with others, finding information on boards and components, using an online IDE and compiler, reading the documentation and learning about what's new and what's coming next in Mbed OS.

## Getting started for contributors
 
We also have a [contributing and publishing guide](https://os.mbed.com/contributing/) that covers licensing, contributor agreements and style guidelines.

## Documentation

For more information about Mbed OS, please see [our published documentation](https://os.mbed.com/docs/latest). It includes Doxygen for our APIs, step-by-step tutorials, porting information and background reference materials about our architecture and tools.

To contribute to this documentation, please see the [mbed-os-5-docs repository](https://github.com/ARMmbed/mbed-os-5-docs).

