# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-wio-h725ae INTERFACE)

target_sources(mbed-wio-h725ae
    INTERFACE
        PeripheralPins.c
        system_clock_override.c
)

target_include_directories(mbed-wio-h725ae
    INTERFACE
        .
)

target_link_libraries(mbed-wio-h725ae INTERFACE mbed-stm32h725xe)
