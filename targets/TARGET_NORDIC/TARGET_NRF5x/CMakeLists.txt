# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NRF52 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SDK_15_0 EXCLUDE_FROM_ALL)

add_library(mbed-nrf5x INTERFACE)

target_include_directories(mbed-nrf5x
    INTERFACE
        .
)

target_sources(mbed-nrf5x
    INTERFACE
        lp_ticker.c
        pinmap.c
        port_api.c
        qspi_api.c
        rtc_api.c
)

target_link_libraries(mbed-nrf5x INTERFACE mbed-nordic)
