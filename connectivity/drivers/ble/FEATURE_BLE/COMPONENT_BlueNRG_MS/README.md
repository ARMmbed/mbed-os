# BlueNRG_MS

HCI driver for BlueNRG_MS BLE component

A simple table like this could help:

|Module Name|Processor Name|Bluetooth compliance|Status|Used in shields & boards|Link|
|-------------|-----------|-----|-|-|-|
|SPBTLE-RF    |BlueNRG-MS (network processor) |v4.1 |Not recommended for new designs             |X-NUCLEO-IDB05A1, DISCO-L475VG-IOT01A, DISCO-L562QE | https://www.st.com/en/wireless-transceivers-mcus-and-modules/spbtle-rf.html |
|BlueNRG-M0   |BlueNRG-MS (network processor) |v4.2 |Active (included in ST's Longevity Program) |X-NUCLEO-IDB05A2 | https://www.st.com/en/wireless-transceivers-mcus-and-modules/bluenrg-m0.html |
|BlueNRG-M2SP |BlueNRG-2 (application processor) |v5.0 |Active (included in ST's Longevity Program) |X-NUCLEO-BNRG2A1 | https://www.st.com/en/wireless-transceivers-mcus-and-modules/bluenrg-m2.html |


It uses ARM Cordio stack instead of the ST BlueNRG stack.

## History

- deprecated ST BLE port: https://github.com/ARMmbed/ble-x-nucleo-idb0xa1
- deprecated X-NUCLEO-IDB05A1 BlueNRG : https://github.com/ARMmbed/cordio-ble-x-nucleo-idb0xa1


## Boards

### X-NUCLEO-IDB05A1

Bluetooth Low Energy Nucleo Expansion Board:

https://developer.mbed.org/components/X-NUCLEO-IDB05A1-Bluetooth-Low-Energy/

### DISCO-L475VG-IOT01A

STM32L4 Discovery kit IoT node, low-power wireless, Bluetooth V4.1 module (SPBTLE-RF)

https://os.mbed.com/platforms/ST-Discovery-L475E-IOT01A/


### DISCO-L562QE

STM32L562E-DK Discovery kit with Bluetooth V4.1 low energy module and Arm Cortex-M33 with TrustZone

https://os.mbed.com/platforms/ST-Discovery-L562QE/


## Driver configuration

In order to use the BlueNRG-MS module together with other targets,
you may need to override default settings in your local `mbed_app.json` file

### ST-DISCO boards

Default settings for `DISCO-L475VG-IOT01A` and `DISCO-L562QE` are configured

### Arduino Connector Compatibility Warning

Default Arduino connection is using:

```
        "SPI_MOSI":  "D11",
        "SPI_MISO":  "D12",
        "SPI_nCS":   "A1",
        "SPI_RESET": "D7",
        "SPI_IRQ":   "A0",
        "SPI_SCK":   "D3",
        "valid-public-bd-address": false
```

X-NUCLEO-IDB05A1 is Arduino compatible with an exception: instead of using pin **D13** for the SPI clock, pin **D3** is used.
The default configuration for this library is having the SPI clock on pin **D3**.

To be fully Arduino compatible, X-NUCLEO-IDB05A1 needs a small HW patch.

For X-NUCLEO-IDB05A1 this patch consists in removing zero resistor **R4** and instead soldering zero resistor **R6**.

In case you patch your board, then you also have to configure this library to use pin **D13** to drive the SPI clock.
To this aim you need to update your local mbed_app.json file with:

```
    "target_overrides": {
        "XXXX": {
            "bluenrg_ms.SPI_SCK": "D13"
        },
```

If you use pin **D13** for the SPI clock, please be aware that on some STM32 Nucleo boards you may **not** drive the LED,
otherwise you will get a conflict: the LED on STM32 Nucleo boards is connected to pin **D13**.

Referring to the current list of tested platforms (see [X-NUCLEO-IDB05A1](https://developer.mbed.org/components/X-NUCLEO-IDB05A1-Bluetooth-Low-Energy/) page),
the patch is required for
- [ST-Nucleo-F103RB](https://developer.mbed.org/platforms/ST-Nucleo-F103RB/)
- [ST-Nucleo-F411RE](https://developer.mbed.org/platforms/ST-Nucleo-F411RE/)
- [ST-Nucleo-F446RE](https://developer.mbed.org/platforms/ST-Nucleo-F446RE/)
- [FRDM-K64F](https://developer.mbed.org/platforms/FRDM-K64F/)

### Target Configuration

To use that library, the target requires some extra configuration in the application `mbed_app.json`. In the `target_overides` section:   

* BLE feature has to be enabled

```json
"target.features_add": ["BLE"]
```

* BlueNRG module has to be enabled

```json
"target.components_add": ["BlueNRG_MS"]
```

* Extra labels have to be defined to include the cordio stack and this library: 

```json
"target.extra_labels_add": ["CORDIO"]
```

As an example, the target overide section for the `NUCLEO_F401RE` would be: 

```json
        "NUCLEO_F401RE": {
            "target.features_add": ["BLE"],
            "target.components_add": ["BlueNRG_MS"],
            "target.extra_labels_add": ["CORDIO"]
        }
```
