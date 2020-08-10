# Cordio Link Layer port for Nordic platforms

To use this port amend your mbed_app.json file as following:
```json
        "NRF52840_DK": {
            "target.features_add": ["BLE"],
            "target.extra_labels_add": ["CORDIO", "CORDIO_LL", "SOFTDEVICE_NONE", "NORDIC_CORDIO"],
            "target.extra_labels_remove": ["SOFTDEVICE_COMMON", "SOFTDEVICE_S140_FULL", "NORDIC_SOFTDEVICE"]
        },
        "NRF52_DK": {
            "target.features_add": ["BLE"],
            "target.extra_labels_add": ["CORDIO", "CORDIO_LL", "SOFTDEVICE_NONE", "NORDIC_CORDIO"],
            "target.extra_labels_remove": ["SOFTDEVICE_COMMON", "SOFTDEVICE_S132_FULL", "NORDIC_SOFTDEVICE"]
        }
```

Resources used:
* RADIO
* TIMER0
* PPI channels 14 and 15
* HF clock is enabled