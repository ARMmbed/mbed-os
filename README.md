[![Mbed OS][mbed-os-logo]][mbed-os-link]

[![Build status master][mbed-master-svg]][mbed-master]
[![Tools coverage status][mbed-coveralls-tools-svg]][mbed-coveralls-tools]

[mbed-os-logo]: logo.png
[mbed-os-link]: https://www.mbed.com/en/platform/mbed-os/
[mbed-master]: https://github.com/ARMmbed/mbed-os/actions/workflows/basic_checks.yml
[mbed-master-svg]: https://github.com/ARMmbed/mbed-os/actions/workflows/basic_checks.yml/badge.svg
[mbed-coveralls-tools]: https://coveralls.io/github/ARMmbed/mbed-os?branch=master
[mbed-coveralls-tools-svg]: https://coveralls.io/repos/github/ARMmbed/mbed-os/badge.svg?branch=master

Arm Mbed OS is an open source embedded operating system designed specifically for the "things" in the Internet of Things. It includes all the features you need to develop a connected product based on an Arm Cortex-M microcontroller, including security, connectivity, an RTOS and drivers for sensors and I/O devices.

Mbed OS provides a platform that includes:

- Security foundations.
- Cloud management services.
- Drivers for sensors, I/O devices and connectivity.

## Release notes
The [release notes](https://os.mbed.com/releases) detail the current release. You can also find information about previous versions.

## License and contributions

The software is provided under the [Apache-2.0 license](LICENSE-apache-2.0.txt). Contributions to this project are accepted under the same license. Please see [contributing.md](CONTRIBUTING.md) for more information.

This project contains code from other projects. The original license text is included in those source files. They must comply with our [license guide](https://os.mbed.com/docs/mbed-os/latest/contributing/license.html).

Folders containing files under different permissive license than Apache 2.0 are listed in the [LICENSE](LICENSE.md) file.

## Getting started for developers

We have a [developer website](https://os.mbed.com) for asking questions, engaging with others, finding information on boards and components, using an online IDE and compiler, reading the documentation and learning about what's new and what's coming next in Mbed OS.

## Getting started for contributors

We also have a [contributing and publishing guide](https://os.mbed.com/contributing/) that covers licensing, contributor agreements and style guidelines.

## Documentation

For more information about Mbed OS, please see [our published documentation](https://os.mbed.com/docs/latest). It includes Doxygen for our APIs, step-by-step tutorials, porting information and background reference materials about our architecture and tools.

To contribute to this documentation, please see the [mbed-os-5-docs repository](https://github.com/ARMmbed/mbed-os-5-docs).

## Security considerations for production application

Please note that if you intend to use Mbed OS in a real product then you should consider the security implications of your application. Mbed OS provides user hooks (functions prefixed with WEAK symbol) that are intended to be overridden. We recommend that you carefully consider the threat model of your application and override the default user hooks provided by Mbed OS to fit your application's security needs.

For example, Mbed OS executes [`mbed_die`](https://github.com/ARMmbed/mbed-os/blob/master/platform/source/mbed_board.c#L26) when there is an error. `mbed_die` by default halts the system. A production application should override weakly linked `mbed_die` function and provide own implementation suitable for their needs taking care of any security vulnerabilities and production considerations. 
