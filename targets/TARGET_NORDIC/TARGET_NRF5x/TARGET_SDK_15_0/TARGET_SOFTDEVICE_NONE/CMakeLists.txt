# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-softdevice-none INTERFACE)

target_include_directories(mbed-softdevice-none
    INTERFACE
        libraries
        libraries/fstorage
        nrf_soc_nosd
)

target_sources(mbed-softdevice-none
    INTERFACE
        libraries/fstorage/nrf_fstorage_nvmc.c

        nrf_soc_nosd/nrf_nvic.c
        nrf_soc_nosd/nrf_soc.c
)
