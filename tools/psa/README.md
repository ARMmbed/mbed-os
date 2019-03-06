# PSA Tools

This document will describe the following scripts:

* \_\_init\_\_.py
* generate_partition_code.py
* mbed_spm_tfm_common.py
* generate_mbed_spm_partition_code.py
* generate_tfm_partition_code.py
* release.py

## \_\_init\_\_.py

This file holds common function dedicated to help SiP with their post-build logic.

* find_secure_image - Scans a Resource object to find the correct binary of the secure image to merge with the non-secure build.


## Code generation scripts

mbed-os holds 2 implementations of PSA:
* MBED_SPM - Implementation for dual core v7 targets.
* TF-M - Implementation for v8 targets.

each implementation requires a set of auto generated files describing the secure partitions.

* `generate_partition_code.py` - Generate files for both implementations.
* `generate_mbed_spm_partition_code.py` - Generate files for BMED_SPM.
* `generate_tfm_partition_code.py` - Generate files for TF-M.
*  `mbed_spm_tfm_common.py` - Holds common functions for both.

## Secure image generation

`release.py` is the script assigned with compiling the secure images.

```python
usage: release.py [-h] [-m MCU]

optional arguments:
  -h, --help         show this help message and exit
  -m MCU, --mcu MCU  build for the given MCU
```

when `MCU ` is not specified the script will compile all the images for all the targets

This script should be ran in following scenarios:
* Release.
* Update to files originating in the secure side.
* Drivers update.
* PSA updates.
