# PSoC 6 Peripheral Driver Library v1.4.0

Please refer to the [README.md](./README.md) and the [PDL API Reference Manual](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/index.html) for a complete description of the Peripheral Driver Library.

### New Features
* The structure of BSP startup templates directory (devices/templates) is updated to match the BSP layout.
* The updated core-lib is reused - see [SysLib changelog](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syslib.html) for details.
* Removed redundant legacy PSoC Creator-compatibility macros.
* The startup code reuses sysclk driver API - see [Startup changelog](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__system__config.html) for details.

Updated Personalities
* CSD
* Power
* SegLCD
* WiFi
Updated the configurators launch parameters in CSD and SegLCD personalities: switched from GUI to console applications for regenerating the source code without opening the configurator itself. This improves the user experience, performance, and enables using machines without a GUI.
The Power personality code generation is corrected due to the customer's request.
The TCP Keepalive Offload feature support is added to the WiFi Low Power Assistant (LPA) personality.

Updated Drivers
* [BLE_CLK 3.30](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__ble__clk.html)
* [SCB 2.40](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__system__scb.html)
* [Startup 2.70](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__system__config.html)
* [SysClk 1.50](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sysclk.html)
* [SysLib 2.50](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syslib.html)
* [SysPm 4.50](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syspm.html)
* [WDT 1.20](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__wdt.html)

Drivers with patch version updates
* [Flash 3.30.3](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__flash.html)
* [SAR 1.20.2](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sar.html)
* [SegLCD 1.0.1](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__seglcd.html)
* [SMIF 1.40.1](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__smif.html)
* [TrigMux 1.20.1](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__trigmux.html)

### Known Issues
None

### Defect Fixes
See the Changelog section of each Driver in the [PDL API Reference](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/modules.html) for all fixes and updates.

### Supported Software and Tools
This version of PDL was validated for compatibility with the following Software and Tools:

| Software and Tools                                      | Version      |
| :---                                                    | :----        |
| CMSIS-Core(M)                                           | 5.2.1        |
| GCC Compiler                                            | 7.2.1        |
| IAR Compiler                                            | 8.32         |
| ARM Compiler 6                                          | 6.11         |
| MBED OS                                                 | 5.13.1       |
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
© Cypress Semiconductor Corporation, 2019.