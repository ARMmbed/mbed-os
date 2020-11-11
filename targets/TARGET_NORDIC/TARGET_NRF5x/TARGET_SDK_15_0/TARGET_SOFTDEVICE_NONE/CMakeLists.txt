# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


target_include_directories(mbed-core
    INTERFACE
        ./libraries
        ./libraries/fstorage
        ./nrf_soc_nosd/
)

target_sources(mbed-core
    INTERFACE
        libraries/fstorage/nrf_fstorage_nvmc.c

        nrf_soc_nosd/nrf_nvic.c
        nrf_soc_nosd/nrf_soc.c
)
