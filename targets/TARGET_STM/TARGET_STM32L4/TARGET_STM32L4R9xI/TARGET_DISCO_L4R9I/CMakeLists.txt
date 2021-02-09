# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-l4r9i INTERFACE)

target_sources(mbed-disco-l4r9i
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-l4r9i
    INTERFACE
        .
)

target_link_libraries(mbed-disco-l4r9i INTERFACE mbed-stm32l4r9xi)
