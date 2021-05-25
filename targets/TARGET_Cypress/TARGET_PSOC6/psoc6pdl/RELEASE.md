# MTB CAT1A Peripheral Driver Library v2.2.0

Please refer to the [README.md](./README.md) and the
[PDL API Reference Manual](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/index.html)
for a complete description of the Peripheral Driver Library.

Some restrictions apply to the PSoC 64 devices configuration. Please refer to [PRA (Protected Register Access)](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__pra.html) driver documentation for the details.

## New Features

* Added support for new silicon revision of CY8C61x4 and CY8C62x4 devices.
* Added RTOS friendly delay APIs in SYSLIB driver.
* Enhanced SYSPM driver to manage the backup domain reset in a better manner and to store/restore the WCO trim value.

## Updated Personalities

* SAR - Bug fix/Code enhancement.
* SYSCLK - Fixed compiler warning.
* SYSPM - Minor bug fix.
* TCPWM - Minor bug fix.

## Added Drivers

## Updated Drivers

* [GPIO 1.50](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__gpio.html)
* [SCB 2.80](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__scb.html)
* [SDHOST 1.80](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__sd__host.html)
* [SMIF 2.20](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__smif.html)
* [SYSCLK 3.20](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__sysclk.html)
* [SYSLIB 2.90](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__syslib.html)

### Drivers with patch version updates

Minor documentation changes:
* [MCWDT 1.50.1](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__mcwdt.html)
* [SAR 2.0.1](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/group__group__sar.html)

### Obsoleted part numbers

The ModusToolbox Device Configurator can not create the designs targeting the obsolete PSoC 6 part numbers.

Below is a list of PSoC 6 part numbers obsoleted in psoc6pdl release-v1.6.0, with the suggested next best alternative:

| Obsoleted part number | Next best alternative |
| :---                  | :----                 |
| CY8C624AAZI-D44       | CY8C624AAZI-S2D44     |
| CY8C624AFNI-D43       | CY8C624AFNI-S2D43     |
| CY8C624ABZI-D04       | CY8C624ABZI-S2D04     |
| CY8C624ABZI-D14       | CY8C624ABZI-S2D14     |
| CY8C624AAZI-D14       | CY8C624AAZI-S2D14     |
| CY8C6248AZI-D14       | CY8C6248AZI-S2D14     |
| CY8C6248BZI-D44       | CY8C6248BZI-S2D44     |
| CY8C6248AZI-D44       | CY8C6248AZI-S2D44     |
| CY8C6248FNI-D43       | CY8C6248FNI-S2D43     |
| CY8C624ALQI-D42       | N/A                   |

## Known Issues
* On soft reset, user need to reset back up domain using Cy_SysLib_ResetBackupDomain() to receive Cy_RTC_CenturyInterrupt() callback on Century roll over.
* On building with tools 2.2, user get warning related to the older version of tools used. To avoid this warning, user is advised to migrate to newer tools version or keep working with previous version of this library.  The warning generated is as follows:
  * _#warning "(Library) The referenced 'device support library' contains newer content than is supported. Either downgrade the version of the 'device support library' referenced or upgrade the version of tools being used_
  

## Defect Fixes

See the Changelog section of each Driver in the [PDL API Reference](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/modules.html) for all fixes and updates.

## Supported Software and Tools

This version of PDL was validated for compatibility with the following Software and Tools:

| Software and Tools                                                            | Version      |
| :---                                                                          | :----        |
| [Cypress Core Library](https://github.com/cypresssemiconductorco/core-lib)    | 1.1.4        |
| [Cypress HAL](https://github.com/cypresssemiconductorco/psoc6hal)             | 1.4.0        |
| CMSIS-Core(M)                                                                 | 5.4.0        |
| GCC Compiler                                                                  | 9.3.1        |
| IAR Compiler                                                                  | 8.42.2       |
| ARM Compiler 6                                                                | 6.13         |
| FreeRTOS                                                                      | 10.0.1       |

## More information

* [Peripheral Driver Library README.md](./README.md)
* [Peripheral Driver Library API Reference Manual](https://cypresssemiconductorco.github.io/mtb-pdl-cat1/pdl_api_reference_manual/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [ModusToolbox Device Configurator Tool Guide](https://www.cypress.com/ModusToolboxDeviceConfig)
* [AN210781 Getting Started with PSoC 6 MCU with Bluetooth Low Energy (BLE) Connectivity](http://www.cypress.com/an210781)
* [PSoC 6 Technical Reference Manuals](https://www.cypress.com/search/all/PSoC%206%20Technical%20Reference%20Manual?f%5b0%5d=meta_type%3Atechnical_documents&f%5b1%5d=resource_meta_type%3A583)
* [PSoC 6 MCU Datasheets](https://www.cypress.com/search/all?f%5b0%5d=meta_type%3Atechnical_documents&f%5b1%5d=resource_meta_type%3A575&f%5b2%5d=field_related_products%3A114026)
* [Cypress Semiconductor](http://www.cypress.com)

---
© Cypress Semiconductor Corporation, 2021.
