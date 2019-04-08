# PSA tools

This document describes the following scripts:

* \_\_init\_\_.py
* generate_partition_code.py
* mbed_spm_tfm_common.py
* generate_mbed_spm_partition_code.py
* generate_tfm_partition_code.py
* release.py

## \_\_init\_\_.py

This file holds common functions dedicated to help SiP with their post-build logic.

* find_secure_image - Scans a Resource object to find the correct binary of the secure image to merge with the non-secure build.

## Code generation scripts

Mbed OS holds two implementations of PSA:

* MBED_SPM - Implementation for dual-core v7 targets.
* TF-M - Implementation for v8 targets.

Each implementation requires a set of auto-generated files describing the secure partitions:

* `generate_partition_code.py` - Generate files for both implementations.
* `generate_mbed_spm_partition_code.py` - Generate files for MBED_SPM.
* `generate_tfm_partition_code.py` - Generate files for TF-M.
* `mbed_spm_tfm_common.py` - Holds common functions for both.

## Secure image generation

`release.py` is the script assigned with compiling the secure images:

```
usage: release.py [-h] [-m MCU] [-d] [-q] [-l] [--commit] [--skip-tests]
                  [-x ...]

optional arguments:
  -h, --help           show this help message and exit
  -m MCU, --mcu MCU    build for the given MCU
  -d, --debug          set build profile to debug
  -q, --quiet          No Build log will be printed
  -l, --list           Print supported PSA secure targets
  --commit             create a git commit for each platform
  --skip-tests         skip the test build phase
  -x ..., --extra ...  additional build parameters
```

* When `MCU ` is not specified, the script compiles all the images for all the targets.
* When `-d/--debug` is not specified, the script compiles the images using the release profile.
* When `--commit` is not specified, the script will not commit the images to git.
* A user can specify additional commands that will be passed on to the build commands (Ex. -D for compilation defines).

This script should be run in following scenarios:

* Release.
* Update to files originating in the secure side.
* Drivers update.
* PSA updates.
