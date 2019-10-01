# Supporting PSA in Mbed OS

This document is still a work in progress. It describes the process of adding new PSA targets to Mbed OS. It also outlines the new directory structure created to support PSA on single v7-M, dual v7-M and single v8-M.

## TODO
* Adding dual v7-M targets
* Adding v8-M targets
* Building TF-M
* Running PSA compliance tests

## Adding new PSA targets
To help with the creation of new PSA targets, couple of generic PSA targets have been added to `targets/targets.json`.
* `PSA_Target` (Root level PSA target)
* `PSA_V7_M_NSPE` (Single v7-M NSPE generic target)
* `PSA_V7_M_SPE` (Single v7-M SPE generic target)
* `PSA_DUAL_V7_M_NSPE` (Dual v7-M NSPE generic target)
* `PSA_DUAL_V7_M_SPE` (Dual v7-M SPE generic target)
* `PSA_V8_M_NSPE` (v8-M NSPE generic target)
* `PSA_V8_M_SPE` (v8-M SPE generic target)

The new PSA targets **MUST** `inherit` from one of these targets depending on the architecture:

Example: 
```json
    "K64F": {
        "inherits": ["PSA_V7_M_NSPE", "Target"],
    }
```

In addition, the new PSA target **MUST** `inherit` from `Target` which defines default target config options. The only exception is, if the new PSA target is inheriting from another generic target which inherits from `Target`.

Example:
```json
    "FAMILY_STM32": {
        "inherits": ["Target"],
        "public": false,
    }
    "NUCLEO_F429ZI": {
        "inherits": ["PSA_V7_M_NSPE", "FAMILY_STM32"],
    }
```

### Adding single v7-M targets
The PSA on single v7-M is supported with the help of Mbed OS PSA services. The following is taken from `targets/targets.json` and shows a PSA enabled target, `K64F`.

```json
    "K64F": {
        "inherits": ["PSA_V7_M_NSPE"],
        "supported_form_factors": ["ARDUINO"],
        "components_add": ["SD", "FLASHIAP"],
        "core": "Cortex-M4F",
        "supported_toolchains": ["ARM", "GCC_ARM", "IAR"],
        "extra_labels_add": [
            "Freescale",
            "MCUXpresso_MCUS",
            "KSDK2_MCUS",
            "FRDM",
            "KPSDK_MCUS",
            "KPSDK_CODE",
            "MCU_K64F",
            "Freescale_EMAC"
        ],
        "is_disk_virtual": true,
        "macros": ["CPU_MK64FN1M0VMD12", "FSL_RTOS_MBED", "MBED_SPLIT_HEAP"],
        "detect_code": ["0240"],
        "device_has_add": [
            "USTICKER",
            "LPTICKER",
            "RTC",
            "CRC",
            "ANALOGIN",
            "ANALOGOUT",
            "EMAC",
            "I2C",
            "I2CSLAVE",
            "INTERRUPTIN",
            "PORTIN",
            "PORTINOUT",
            "PORTOUT",
            "PWMOUT",
            "RESET_REASON",
            "SERIAL",
            "SERIAL_FC",
            "SERIAL_ASYNCH",
            "SLEEP",
            "SPI",
            "SPI_ASYNCH",
            "SPISLAVE",
            "STDIO_MESSAGES",
            "TRNG",
            "FLASH",
            "USBDEVICE",
            "WATCHDOG"
        ],
        "release_versions": ["2", "5"],
        "device_name": "MK64FN1M0xxx12",
        "bootloader_supported": true,
        "overrides": {
            "network-default-interface-type": "ETHERNET"
        }
    }
```

Please pay attention to config options `extra_labels_add` and `device_has_add`. If needed then a PSA target definition **MUST** use [extra_labels/device_has]`_add` or [extra_labels/device_has]`_remove` (not `extra_labels` or `device_has`) to add/remove either extra_labels or target capabilities. Also, use `[feature_]`add/remove to add/remove a feature.
Check [extra_labels](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html), [device_has](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html) and [features](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html) for more information.

### Adding dual v7-M targets

### Adding v8-M targets

##  Enabling PSA at application level
Having an entropy source is crucial for Mbed TLS and Mbed Crypto. The [document](https://os.mbed.com/docs/mbed-os/v5.14/porting/entropy-sources.html) talks about entropy and how to add an entropy source. Sometimes a target might not have a True Random Number Generator (TRNG), in that case the target will be configured as a non-PSA target in `targets/targets.json`. In that scenario, if an application wants to use that target as a PAS target then it is the responsibility of application to provide an entropy source and mark that target as PSA target at application level. The config option [target_overrides](https://os.mbed.com/docs/mbed-os/v5.14/reference/configuration.html) can be used to enable PSA for a target.

example mbed_app.json:
```json
"target_overrides": {
    "K64F": {
        "inherits": ["PSA_V7_M_NSPE"]
    }
}
```


## New directory structure

```console
features/FEATURE_PSA/
├── TARGET_TFM
│   ├── TARGET_TFM_V8M
│   └── TARGET_TFM_TWINCPU
└── TARGET_MBED_PSA_SRV
    ├── services
    │   ├── storage
    │   │   ├── ps
    │   │   ├── its
    │   │   └── common
    │   ├── platform
    │   └── attestation
    │       ├── qcbor
    │       │   ├── test
    │       │   ├── src
    │       │   └── inc
    │       ├── implementation
    │       │   └── tfm_impl
    │       │       └── t_cose
    │       │           ├── src
    │       │           └── inc
    │       └── emulation
    ├── inc
    │   └── psa
    └── TESTS
        ├── compliance_its
        │   ├── test_s010
        │   ├── test_s009
        │   ├── test_s008
        │   ├── test_s007
        │   ├── test_s006
        │   ├── test_s005
        │   ├── test_s004
        │   ├── test_s003
        │   ├── test_s002
        │   └── test_s001
        ├── compliance_crypto
        │   ├── test_c043
        │   ├── test_c042
        │   ├── test_c041
        │   ├── test_c040
        │   ├── test_c039
        │   ├── test_c038
        │   ├── test_c037
        │   ├── test_c036
        │   ├── test_c035
        │   ├── test_c034
        │   ├── test_c033
        │   ├── test_c032
        │   ├── test_c031
        │   ├── test_c030
        │   ├── test_c029
        │   ├── test_c028
        │   ├── test_c027
        │   ├── test_c026
        │   ├── test_c025
        │   ├── test_c024
        │   ├── test_c023
        │   ├── test_c022
        │   ├── test_c021
        │   ├── test_c020
        │   ├── test_c019
        │   ├── test_c018
        │   ├── test_c017
        │   ├── test_c016
        │   ├── test_c015
        │   ├── test_c014
        │   ├── test_c013
        │   ├── test_c012
        │   ├── test_c011
        │   ├── test_c010
        │   ├── test_c009
        │   ├── test_c008
        │   ├── test_c007
        │   ├── test_c006
        │   ├── test_c005
        │   ├── test_c004
        │   ├── test_c003
        │   ├── test_c002
        │   └── test_c001
        └── compliance_attestation
            └── test_a001
```