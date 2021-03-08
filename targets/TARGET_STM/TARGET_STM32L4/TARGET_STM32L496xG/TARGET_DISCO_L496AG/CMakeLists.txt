# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-l496ag INTERFACE)

target_sources(mbed-disco-l496ag
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-l496ag
    INTERFACE
        .
)

target_link_libraries(mbed-disco-l496ag INTERFACE mbed-stm32l496xg)
