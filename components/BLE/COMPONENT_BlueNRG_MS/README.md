# Cordio BLE-X-NUCLEO-IDB0XA1

BLE_API wrapper Library for X-NUCLEO-IDB05A1 BlueNRG (Bluetooth Low Energy) Expansion Board. It uses ARM Cordio stack instead of the ST BlueNRG stack. 

## Introduction

This firmware package implements the port of BLE_API to STMicroelectronics' [X-NUCLEO-IDB05A1](https://developer.mbed.org/components/X-NUCLEO-IDB05A1-Bluetooth-Low-Energy/) Bluetooth Low Energy Nucleo Expansion Board.

### Arduino Connector Compatibility Warning

X-NUCLEO-IDB05A1 is Arduino compatible with an exception: instead of using pin **D13** for the SPI clock, pin **D3** is used.
The default configuration for this library is having the SPI clock on pin **D3**.

To be fully Arduino compatible, X-NUCLEO-IDB05A1 needs a small HW patch.

For X-NUCLEO-IDB05A1 this patch consists in removing zero resistor **R4** and instead soldering zero resistor **R6**.

In case you patch your board, then you also have to configure this library to use pin **D13** to drive the SPI clock. To this aim you need to compile this driver with macro `BLUENRG_PIN_SPI_SCK=D13` defined.

If you use pin **D13** for the SPI clock, please be aware that on STM32 Nucleo boards you may **not** drive the LED, otherwise you will get a conflict: the LED on STM32 Nucleo boards is connected to pin **D13**.

Referring to the current list of tested platforms (see [X-NUCLEO-IDB05A1](https://developer.mbed.org/components/X-NUCLEO-IDB05A1-Bluetooth-Low-Energy/) page), the patch is required by [ST-Nucleo-F103RB](https://developer.mbed.org/platforms/ST-Nucleo-F103RB/); [ST-Nucleo-F302R8](https://developer.mbed.org/platforms/ST-Nucleo-F302R8/); [ST-Nucleo-F411RE](https://developer.mbed.org/platforms/ST-Nucleo-F411RE/); [ST-Nucleo-F446RE](https://developer.mbed.org/platforms/ST-Nucleo-F446RE/); and [FRDM-K64F](https://developer.mbed.org/platforms/FRDM-K64F/).


### Driver configuration

In order to use the BlueNRG-MS module together with other targets, you need to set the macros defined in file [bluenrg_targets.h](https://github.com/ARMmbed/ble-x-nucleo-idb0xa1/blob/master/bluenrg/bluenrg_targets.h). Please, update the [mbed_lib.json](https://github.com/ARMmbed/ble-x-nucleo-idb0xa1/blob/master/mbed_lib.json) to include the list of extra macros that configure the driver for your target.

## Target Configuration

To use that library, the target requires some extra configuration in the application `mbed_app.json`. In the `target_overides` section:   

* BLE feature has to be enabled for the target using the BlueNRG module

```json
"target.features_add": ["BLE"]
```

* Extra labels have to be defined to include the cordio stack and this library: 

```json
"target.extra_labels_add": ["CORDIO", "CORDIO_BLUENRG"]
```

As an example, the target overide section for the `NUCLEO_F401RE` would be: 

```json
        "NUCLEO_F401RE": {
            "target.features_add": ["BLE"],
            "target.extra_labels_add": ["CORDIO", "CORDIO_BLUENRG"]
        }
```
