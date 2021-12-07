# STM32WL family

[st.com STM32WL family page](https://www.st.com/en/microcontrollers-microprocessors/stm32wl-series.html)

This ST MCU family is dual-core : based on an Arm Cortex-M4 core and an Arm Cortex-M0+ core.

Only M4 core is currently used by mbed-os applications.

* [Supported boards](#supported-boards)
   * [NUCLEO_WL55JC](#nucleo_wl55jc)
   * [CUSTOM boards](#custom-boards)
* [LoRa](#lora)
   * [MBED-OS support](#mbed-os-support)
   * [baremetal support](#baremetal-support)
   * [Antenna configuration](#antenna-configuration)
   * [GPIO debug pins](#gpio-debug-pins)
   * [mbed-os test](#mbed-os-test)
   * [Application example](#application-example)


# Supported boards

## NUCLEO_WL55JC

[st.com STM32WL5x MCU page](https://www.st.com/en/microcontrollers-microprocessors/stm32wl5x.html)

[st.com NUCLEO page](https://www.st.com/en/evaluation-tools/nucleo-wl55jc.html)

[mbed.com NUCLEO page](https://os.mbed.com/platforms/ST-Nucleo-WL55JC/)

- Total FLASH is 256KB
- RAM: 64 KB

**[tip] mbed-os export feature**

`mbed export -m NUCLEO_WL55JC -i uvision6`

This needs to locally add a patch:
https://github.com/ARMmbed/mbed-os/pull/10424/files

`mbed export -m NUCLEO_WL55JC -i iar`

This needs to locally add a patch:
https://github.com/jeromecoutant/mbed/blob/IAR_EXPORT_SUPPORT/tools/export/iar/iar_definitions.json#L62-L64


## CUSTOM boards

Check https://github.com/ARMmbed/stm32customtargets#stm32wl

# LoRa

## MBED-OS support

Lora radio is enabled in default STM32WL configuration

## baremetal support

Baremetal is supported.

mbed_app.json:
```
{
    "requires": ["bare-metal", "stm32wl-lora-driver"]
}
```


## Antenna configuration

NUCLEO HW configuration is provided as the default configuration.

https://github.com/ARMmbed/mbed-os/blob/master/connectivity/drivers/lora/TARGET_STM32WL/STM32WL_radio_driver.cpp#L23
```
MBED_WEAK void set_antenna_switch(RBI_Switch_TypeDef state)
```

So you can overwrite it for your custom board.


## GPIO debug pins

2 pins can be configured to check RX and TX activity.

Example of mbed_app.json:
```
{
    "target_overrides": {
        "NUCLEO_WL55JC": {
            "stm32wl-lora-driver.debug_rx": "LED1",
            "stm32wl-lora-driver.debug_tx": "LED2"
        }
    }
}
```

## mbed-os test

| target              | platform_name | test suite                                         | result | elapsed_time (sec) | copy_method |
|---------------------|---------------|----------------------------------------------------|--------|--------------------|-------------|
| NUCLEO_WL55JC-ARMC6 | NUCLEO_WL55JC | connectivity-lorawan-tests-tests-lorawan-loraradio | OK     | 44.8               | default     |



## Application example

https://github.com/ARMmbed/mbed-os-example-lorawan/
