# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        lp_ticker.c
        pinmap.c
        port_api.c
        qspi_api.c
        rtc_api.c
)

mbed_add_cmake_directory_if_labels("TARGET")
