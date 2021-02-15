# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l496zg INTERFACE)

target_sources(mbed-nucleo-l496zg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l496zg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l496zg INTERFACE mbed-stm32l496xg)


add_library(mbed-nucleo-l496zg-p INTERFACE)
target_link_libraries(mbed-nucleo-l496zg-p INTERFACE mbed-nucleo-l496zg)
