# Nordic platform's readme

This readme file contains several hints useful for any Nordic's user.

## 1. Nordic’s development kit targets

Nordic provides several development kit targets, called *[base target]*. For example:
- NRF51_DK
- NRF52_DK
- NRF51_DONGLE

Nordic continuously supports mbed and other targets will become available with releases of new Nordic development kits.

## 2. Regular and FOTA targets types
For most of nRF5x SoC development kits, three flavors of the targets are provided:

- *[base target]* is the default target. A compiled binary will incorporate both the user application and the SoftDevice. A binary built using this target is intended to be downloaded to the DK using an on-board USB debugger/programmer. For example, NRF52_DK target.

- *[base target]*_BOOT is a target with firmware update support. A compiled binary will incorporate a user application, the SoftDevice, and the precompiled BLE bootloader code. This target should be used to provide DFU capabilities by the application (e.g BLE DFU service). A binary built using this target is intended to be downloaded to the DK using an on-board USB debugger/programmer. For example, NRF52_DK_BOOT target.

*[base target]*_OTA. A compiled binary will contain only the application. A binary build using this target is intended to be downloaded to  the DK using BLE Bootloader. For example, NRF52_DK_OTA target.

## 3. Bootloader selection

It is possible to select one of the provided precompiled bootloaders for NRF51_DK_BOOT and NRF52_DK_BOOT targets. Two versions of the bootloader are provided: single-bank bootloader and the dual-bank bootloader. For further information about the differences, see Nordic's [infocenter](http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v11.0.0%2Fbledfu_memory_banks.html). To select the bootloader, use the *target.bootloader_select_index* keyword over [mbed configuration system](../../docs/config_system.md). The expected values are:  
  - target.bootloader_select_index = 0 -> Use single-bank bootloader. It is the default value.  
  - target.bootloader_select_index = 1 -> Use dual-bank bootloader.

The following is an example of selecting the dual-bank bootloader from application settings by putting a description into mbed_app.json:
```json
{
"target_overrides": {
        "*": {
            "target.bootloader_select_index": 1
        }
    }
}
```

## 4. LF Clock configuration using mbed configuration system
The feature is provided so far for NRF5 targets, such as NRF51_DK, NRF52_DK, and TARGET_TY51822R3. For further information, see [LF_Clock_config.md](./TARGET_NRF5/LF_Clock_config.md).

## 5. UART hardware flow control configuration using mbed configuration system
The feature is provided so far for NRF5 targets, such as NRF51_DK, NRF52_DK, and TARGET_TY51822R3. 
In order to configure hardware flow control for nRF5x UART, use the target.uart_hwfc keyword. Value 0 will disable flow control. Value 1 will enable flow control. Flow control is turned on by default.
For example, on application level, put the following description into the mbed_app.json file:
```json
"target_overrides": {
    "NRF51_DK": {
        "target.uart_hwfc": 0
    }
}
```



