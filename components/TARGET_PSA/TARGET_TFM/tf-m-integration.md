# TF-M integration to Mbed-OS
This document is an initial draft for TF-M for Mbed-OS porting guide .

## Audience
This guide is intended for developers wishing to port Mbed-OS with TF-M used as a secure kernel for ARMv8-M targets.

Prior knowledge with both TF-M & Mbed-OS concepts is assumed.

## Build system concepts:

Mbed-OS build system is based on [Mbed-CLI](https://github.com/ARMmbed/mbed-cli).
Mbed-CLI build system performs lookup for source and header files within project directory and adds them all to a build. All folders will be scanned for sources except for:
- folders starting with `TARGET_*`
- folders starting with `COMPONENT_*`
- folders starting with `FEATURE_*`
- folders starting with `TESTS_*` (not true for `mbed test` builds)
- files and folders listed in `.mbedignore`

The ignored folders listed above can be explicitly added to a compilation by adding following keys to a target description in `targets.json`:
- adding `extra_labels_add`, `inherits` and `sub_target` for adding `TARGET_*`
- adding `components_add` for adding `COMPONENT_*`
- `features_add` for adding `FEATURE_*`

TF-M is built as bare-metal in a secure target, in order to build a secure target with TF-M as its' kernel need to add `--app-config <MBED-OS-ROOT>/tools/psa/tfm/mbed_app.json` to the build command of the secure target.

## Build hooks

Mbed-OS testing tools are designed to work with a single image (`.bin` or `.hex`).
When building mbed-os for TF-M targets two images are created. One for normal world(NW) and one for TrustZone(TZ).
Mbed-OS build system provides `post_binary_hook` that allows executing arbitrary Python script for merging NW and TZ images. Typically `post_binary_hook` is added to NW target and assumes TZ target images as a prerequisite.

## Porting TF-M targets

Typically firmware for TF-M targets consist of 2 or more images: normal world and TrustZone image. More images can be present in case boot loaders are used.
Two images must be built and linked separately. TrustZone image must be built first.

There may be code and/or header files sharing between the two targets.
Nested folder layout typically provides more easy code reuse between two targets:
Example:

```txt
└── tragets
    └── TARGET_<VENDOR>
        └── TARGET_<BOARD>
            ├── common_files            <-- files shared between NW and TZ images
            ├── TARGET_<BOARD>_NS
            │   └── normal_world_files  <-- files only to be included for NW build
            └── TARGET_<BOARD>_S
                └── trustzone_files     <-- files only to be included for TZ build
```

The target should be represented in a following way in `target.json` (`MUSCA_A1` is taken for example and should be substituted):
```json
...
"ARM_MUSCA_A1": {
        "public": false,
        "inherits": ["Target"],
        "supported_toolchains": ["ARMC6", "GCC_ARM"],
        "default_toolchain": "ARMC6",
        "extra_labels": ["ARM_SSG", "MUSCA_A1"],
    },
    "ARM_MUSCA_A1_NS": {
        "inherits": ["NSPE_Target", "ARM_MUSCA_A1"],
        "core": "Cortex-M33-NS",
        "device_has_add": ["INTERRUPTIN", "LPTICKER", "SERIAL", "SLEEP", "USTICKER"],
        "macros": [
            "MBED_FAULT_HANDLER_DISABLED",
            "MBEDTLS_PSA_CRYPTO_C"
        ],
        "extra_labels_add": ["MUSCA_A1_NS", "PSA", "TFM"],
        "post_binary_hook": {"function": "ArmMuscaA1Code.binary_hook"}
    },
    "ARM_MUSCA_A1_S": {
        "inherits": ["SPE_Target", "ARM_MUSCA_A1"],
        "core": "Cortex-M33",
        "device_has_add": ["FLASH"],
        "macros": [
            "MBED_MPU_CUSTOM",
            "MBEDTLS_PSA_CRYPTO_SPM",
            "MBEDTLS_PSA_CRYPTO_C",
            "MBEDTLS_ENTROPY_NV_SEED"
        ],
        "components_add": ["FLASHIAP"],
        "extra_labels_add": ["MUSCA_A1_S", "PSA", "TFM"]
    },
```

Example details:
- Secure target:
  - `"device_has_add": ["FLASH"]` and `"components_add": ["FLASHIAP"]` for enabling storage stack. Required by PSA Internal storage service.
  - `"extra_labels_add": ["PSA", "TFM"]` are required to add PSA services and TF-M SPM implementation sources to a compilation
  - all the macros from the example above are required
  - must inherit from `SPE_Target`
- Nonsecure target:
  -  must inherit from `NSPE_Target`
  - `"extra_labels_add": ["PSA", "TFM"]` are required to add PSA services and TF-M SPM implementation sources to a compilation
  - all the macros from the example above are required
  -  `post_binary_hook` is used to combine secure and non-secure images

### HAL
For porting Mbed-OS & TF-M both Mbed-OS and TF-M HAL layers should be created.

#### Mbed-OS HAL:
Follow instructions for [Mbed-OS HAL porting](https://os.mbed.com/docs/mbed-os/v5.11/porting/porting-hal-modules.html)

#### TF-M:
Mbed-OS contains customized TF-M version. TF-M services reference implementation was replaced by Mbed-OS version. Thus TF-M has different HAL layer comparing to vanilla [TF-M reference implementation](https://git.trustedfirmware.org/trusted-firmware-m.git/about/).

The porting layer consists of:
- All functions listed in: `components/TARGET_PSA/TARGET_TFM/COMPONENT_SPE/platform/include/tfm_spm_hal.h`
- Flash API `mbed-os/hal/flash_api.h` implementation is required for TZ image. It is used by PSA Internal trusted storage implementation.
