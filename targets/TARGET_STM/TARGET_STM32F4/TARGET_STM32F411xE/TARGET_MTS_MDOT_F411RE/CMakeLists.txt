# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mts-mdot-f411re INTERFACE)

target_sources(mbed-mts-mdot-f411re
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-mts-mdot-f411re
    INTERFACE
        .
)

target_link_libraries(mbed-mts-mdot-f411re INTERFACE mbed-stm32f411xe)
