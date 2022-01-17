# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-g0b1re INTERFACE)

target_sources(mbed-nucleo-g0b1re
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-g0b1re
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-g0b1re INTERFACE mbed-stm32g0b1xe)
