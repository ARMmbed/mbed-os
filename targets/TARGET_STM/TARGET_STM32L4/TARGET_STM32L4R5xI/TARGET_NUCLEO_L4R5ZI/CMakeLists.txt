# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l4r5zi INTERFACE)

target_sources(mbed-nucleo-l4r5zi
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l4r5zi
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l4r5zi INTERFACE mbed-stm32l4r5xi)


add_library(mbed-nucleo-l4r5zi-p INTERFACE)
target_link_libraries(mbed-nucleo-l4r5zi-p INTERFACE mbed-nucleo-l4r5zi)
