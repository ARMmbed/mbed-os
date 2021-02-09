# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-g474re INTERFACE)

target_sources(mbed-nucleo-g474re
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-g474re
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-g474re INTERFACE mbed-stm32g474xe)
