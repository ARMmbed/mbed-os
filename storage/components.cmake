# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND storage-components
    dataflash
    flashiap
    i2cee
    qspif
    sd
    spif
)

set(dataflash_PATH blockdevice/COMPONENT_DATAFLASH)
set(flashiap_PATH blockdevice/COMPONENT_FLASHIAP)
set(i2cee_PATH blockdevice/COMPONENT_I2CEE)
set(qspif_PATH blockdevice/COMPONENT_QSPIF)
set(sd_PATH blockdevice/COMPONENT_SD)
set(spif_PATH blockdevice/COMPONENT_SPIF)
