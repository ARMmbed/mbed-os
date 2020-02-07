# PSoC 6 Peripheral Driver Library v1.4.1

Please refer to the [README.md](./README.md) and the [PDL API Reference Manual](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/index.html) for a complete description of the Peripheral Driver Library.

### New Features
* The updated core-lib is reused - see [SysLib changelog](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syslib.html) for details.


Updated Personalities
* CAN FD - Fix filter configuration issue.
* DMA - Fixed the Trigger Input parameter behaviour.
* WiFi - Update for LPA TCP keepalive offload.
* I2S - Fixed the IRQn generation for all supported devices.
* PDM-PCM - Fixed the IRQn generation for all supported devices.
* QSPI - Data terminals UI enhancement.
* SegLCD - Added the ability to route output signals to Smart I/O.
* Smart I/O - GUI improvement.
* SysClocks - Disable ILO in Hibernate.

Updated Drivers
* [BLE_CLK 3.40](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__ble__clk.html)
* [CAN FD 1.10](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__canfd.html)
* [RTC 2.30](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__rtc.html)
* [SMIF 1.50](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__smif.html)
* [SysClk 1.60](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sysclk.html)
* [SysPm 5.0](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syspm.html)

Drivers with patch version updates
* [eFuse 1.10.2](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__efuse.html)
* [Flash 3.30.4](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__flash.html)
* [Prot 1.30.2](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__prot.html)
* [SysLib 2.50.1](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syslib.html)

### Known Issues
None

### Defect Fixes
See the Changelog section of each Driver in the [PDL API Reference](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/modules.html) for all fixes and updates.

### Supported Software and Tools
This version of PDL was validated for compatibility with the following Software and Tools:

| Software and Tools                                      | Version      |
| :---                                                    | :----        |
| [Cypress Core Library](https://github.com/cypresssemiconductorco/core-lib)                                    | 1.1.1        |
| [Cypress HAL](https://github.com/cypresssemiconductorco/psoc6hal)                                             | 1.1.1        |
| CMSIS-Core(M)                                           | 5.2.1        |
| GCC Compiler                                            | 9.2.1        |
| IAR Compiler                                            | 8.32         |
| ARM Compiler 6                                          | 6.13         |
| FreeRTOS                                                | 10.0.1       |

### More information
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