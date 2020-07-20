# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/fstorage
        ${CMAKE_CURRENT_SOURCE_DIR}/nrf_soc_nosd
)

target_sources(mbed-os
    PRIVATE
        libraries/fstorage/nrf_fstorage_nvmc.c
        nrf_soc_nosd/nrf_nvic.c
        nrf_soc_nosd/nrf_soc.c
)
