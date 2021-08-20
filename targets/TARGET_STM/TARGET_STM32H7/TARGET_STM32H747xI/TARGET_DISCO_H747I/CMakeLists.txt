# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-h747i INTERFACE)

target_sources(mbed-disco-h747i
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-h747i
    INTERFACE
        .
)

target_link_libraries(mbed-disco-h747i INTERFACE mbed-stm32h747xi-cm7)


add_library(mbed-disco-h747i-cm7 INTERFACE)

target_sources(mbed-disco-h747i-cm7
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-h747i-cm7
    INTERFACE
        .
)

target_link_libraries(mbed-disco-h747i-cm7 INTERFACE mbed-stm32h747xi-cm7)


add_library(mbed-disco-h747i-cm4 INTERFACE)

target_sources(mbed-disco-h747i-cm4
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-h747i-cm4
    INTERFACE
        .
)

target_link_libraries(mbed-disco-h747i-cm4 INTERFACE mbed-stm32h747xi-cm4)
