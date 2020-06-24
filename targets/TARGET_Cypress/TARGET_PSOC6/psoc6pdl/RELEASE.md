# PSoC 6 Peripheral Driver Library v1.6.0

Please refer to the [README.md](./README.md) and the
[PDL API Reference Manual](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/index.html)
for a complete description of the Peripheral Driver Library.

## New Features

* Added support for the PSoC 64 Secure MCU devices.
Limitations for the secure devices are described as a part of the [PRA](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__pra.html) driver.


## Updated Personalities
* Power - Added possibility to select the Normal/Minimum Current for the both LDO and BUCK Core regulators.

## Personalities with patch version updates

 Minor updates to support the PSoC 64 Secure MCU devices

* Wco
* TimerClk
* TickClk
* SysClock
* SlowClk
* PumpClk
* Pll
* Pin
* Pilo
* PeriClk
* PathMux
* LfClk
* HvIlo
* HfClk
* Fll
* FastClk
* ExtClk
* Eco
* BakClk
* AltHf_BleEco

## Added Drivers

* [PRA 1.0](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__pra.html)

## Updated Drivers

* [Startup 2.80](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__system__config.html)
* [WDT 1.30](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__wdt.html)
* [SysTick 1.20](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__arm__system__timer.html)
* [SysPm 5.10](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syspm.html)
* [SysLib 2.60](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syslib.html)
* [SysClk 2.10](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sysclk.html)
* [SCB 2.50](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__scb.html)
* [LVD 1.20](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__lvd.html)
* [Flash 3.40](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__flash.html)
* [CTB 1.20](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__ctb.html)

### Drivers with patch version updates

Minor documentation changes:

* USBFS 2.20.1
* TrigMux 1.20.2
* Tcpwm 1.10.2
* SysInt 1.30.1
* SysAnalog 1.10.1
* SMIF 1.50.1
* Smart I/O 1.0.1
* Sd_host 1.50.1
* Sar 1.20.3
* Rtc 2.30.1
* Prot 1.30.3
* Profiler 1.20.1

## Known Issues

[SysClk](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sysclk.html)
driver.

## Defect Fixes

See the Changelog section of each Driver in the [PDL API Reference](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/modules.html) for all fixes and updates.

## Supported Software and Tools

This version of PDL was validated for compatibility with the following Software and Tools:

| Software and Tools                                                            | Version      |
| :---                                                                          | :----        |
| [Cypress Core Library](https://github.com/cypresssemiconductorco/core-lib)    | 1.1.2        |
| [Cypress HAL](https://github.com/cypresssemiconductorco/psoc6hal)             | 1.2.0        |
| CMSIS-Core(M)                                                                 | 5.2.1        |
| GCC Compiler                                                                  | 9.2.1        |
| IAR Compiler                                                                  | 8.42.2       |
| ARM Compiler 6                                                                | 6.13         |
| FreeRTOS                                                                      | 10.0.1       |

## More information

* [Peripheral Driver Library README.md](./README.md)
* [Peripheral Driver Library API Reference Manual](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [ModusToolbox Device Configurator Tool Guide](https://www.cypress.com/ModusToolboxDeviceConfig)
* [AN210781 Getting Started with PSoC 6 MCU with Bluetooth Low Energy (BLE) Connectivity](http://www.cypress.com/an210781)
* [PSoC 6 Technical Reference Manuals](https://www.cypress.com/search/all/PSoC%206%20Technical%20Reference%20Manual?f%5b0%5d=meta_type%3Atechnical_documents&f%5b1%5d=resource_meta_type%3A583)
* [PSoC 6 MCU Datasheets](https://www.cypress.com/search/all?f%5b0%5d=meta_type%3Atechnical_documents&f%5b1%5d=resource_meta_type%3A575&f%5b2%5d=field_related_products%3A114026)
* [Cypress Semiconductor](http://www.cypress.com)

---
© Cypress Semiconductor Corporation, 2020.
