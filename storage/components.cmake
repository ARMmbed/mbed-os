# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Extend the cached internal components list
list(APPEND mbed-os-internal-components
    mbed-os-dataflash
    mbed-os-flashiap
    mbed-os-i2cee
    mbed-os-qspif
    mbed-os-sd
    mbed-os-spif
)

# Components paths to the main CMake file
set(mbed-os-dataflash_PATH blockdevice/COMPONENT_DATAFLASH)
set(mbed-os-flashiap_PATH blockdevice/COMPONENT_FLASHIAP)
set(mbed-os-i2cee_PATH blockdevice/COMPONENT_I2CEE)
set(mbed-os-qspif_PATH blockdevice/COMPONENT_QSPIF)
set(mbed-os-sd_PATH blockdevice/COMPONENT_SD)
set(mbed-os-spif_PATH blockdevice/COMPONENT_SPIF)
