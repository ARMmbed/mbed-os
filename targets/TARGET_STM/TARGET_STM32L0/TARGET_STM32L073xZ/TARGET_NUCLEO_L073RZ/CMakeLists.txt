# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l073rz INTERFACE)

target_sources(mbed-nucleo-l073rz
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l073rz
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l073rz INTERFACE mbed-stm32l073xz)
