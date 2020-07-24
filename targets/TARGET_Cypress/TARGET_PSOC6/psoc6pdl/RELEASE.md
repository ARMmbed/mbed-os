# PSoC 6 Peripheral Driver Library v1.6.1

Please refer to the [README.md](./README.md) and the
[PDL API Reference Manual](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/index.html)
for a complete description of the Peripheral Driver Library.

Some restrictions apply to the PSoC 64 devices configuration. Please refer to [PRA (Protected Register Access)](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__pra.html) driver documentation for the details.

## New Features
Added support of the PSoC 64 CYB06xx7 devices.

## Updated Personalities

## Personalities with patch version updates

* HvIlo - Changed ILO frequency from 32 kHz to 32.768 kHz, and ILO accuracy from 30% to 10% to reflect the actual values.
* Csd - Fixed the CSD resource display condition on the Peripherals tab of the Device Configurator, to display this resource for all MPNs containing the CSD HW block.
* SysClock - Renamed cy_stc_pra_system_config_t member from altHFfreq to altHFclkFreq.

## Added Drivers

## Updated Drivers

* [SysClk 2.20](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sysclk.html)
* [SysPm 5.20](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__syspm.html)
* [PRA 2.0](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__pra.html)
* [SysInt 1.40](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__sysint.html)
* [Prot 1.40](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__prot.html)
* [ble_clk 3.50](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__ble__clk.html)
* [Startup 2.90](https://cypresssemiconductorco.github.io/psoc6pdl/pdl_api_reference_manual/html/group__group__system__config.html)

### Drivers with patch version updates

Minor documentation changes:

* IPC 1.40.2
* SysLib 2.60.1
* Crypto 2.30.4

## Known Issues

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
