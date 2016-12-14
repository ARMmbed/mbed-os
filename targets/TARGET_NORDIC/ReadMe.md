# Nordic platforms ReadMe

This Description contains several hints useful for any Nordic's user.

## 1. Nordic’s development kit targets


Nordic provide few development kit targets - called beyond *[base target]*, for example:
- NRF51_DK  
- NRF51_DK_LEGACY  
- NRF52_DK  
- NRF51_DONGLE  
- others, mbed-os is under continuous construction!

For most of Nordic’s development kits few version of targets are provided:  
A *[base target]* is the default target. Compiled binary will incorporate both a user application and the SoftDevice. Binary build using this target is intended to be download to DK using on-board USB debugger/programmer. For example, NRF52_DK target.  

*[base target]*_BOOT – it is target with firmware update support. Compiled binary will incorporate a user application, the SoftDevice and the precompiled BLE bootloader code. This target should be used to provide DFU capabilities by an application (e.g BLE DFU service). Binary build using this target is intended to be download to DK using on-board USB debugger/programmer. For example, NRF52_DK_BOOT target.

*[base target]*_OTA – Compiled binary will contain only an application. Binary build using this target is intended to be download to DK using BLE Bootloader. For example, NRF52_DK_OTA target.

## 2. Bootloader selection
It is posisble to select on of provided precompiled bootloader for NRF51_DK_BOOT and  NRF52_DK_BOOT targets. There are provide two version of bootloader: the single bank bootloader and the dual bank bootloader. For further information about differences look into Nordic's [infocenter](http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v11.0.0%2Fbledfu_memory_banks.html). Selection is possible using *target.bootloader_select_index* keyword over  [mbed configuration system](../../docs/config_system.md). Expected values are:  
  - target.bootloader_select_index = 0 -> use single bank bootloader. It is default value.  
  - target.bootloader_select_index = 1 -> use dual bank bootloader

For example selecting dual bank bootloader from application settings by putting description into mbed_app.json
```json
{
"target_overrides": {
        "*": {
            "target.bootloader_select_index": 1
        }
    }
}
```

## 3. LF Clock configuration using mbed configuration system
Feature is provided so far for NRF5 targets e.g.: (NRF51_DK, NRF52_DK, TARGET_TY51822R3). For further information see [LF_Clock_config.md](./TARGET_NRF5/LF_Clock_config.md).

## 4. UART hardware flow control configuration using mbed configuration system.
Feature is provided so far for NRF5 targets e.g.: (NRF51_DK, NRF52_DK, TARGET_TY51822R3). 
In order to configure hardware flow control for nRF5x UART use target.uart_hwfc keyword. Value 0 will disable flow control. Value 1 will enable flow control. Flow control is turn on by default.
For example on application level put bellow description into mbed_app.json file.
```json
"target_overrides": {
    "NRF51_DK": {
        "target.uart_hwfc": 0
    }
}
```



