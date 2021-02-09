# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l433rc-p INTERFACE)

target_sources(mbed-nucleo-l433rc-p
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l433rc-p
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l433rc-p INTERFACE mbed-stm32l433xc)
