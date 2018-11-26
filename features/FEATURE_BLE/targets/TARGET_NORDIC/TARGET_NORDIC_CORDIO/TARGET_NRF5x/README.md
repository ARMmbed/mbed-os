# Cordio Link Layer port for Nordic platforms

To use this port amend your mbed_app.json file as following:
```json
        "NRF52840_DK": {
            "target.features_add": ["BLE"]
        },
        "NRF52_DK": {
            "target.features_add": ["BLE"]
        }
```

Resources used:
* RADIO
* TIMER0
* PPI channels 14 and 15
* HF clock is enabled