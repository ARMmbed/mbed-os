# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-uhuru-raven INTERFACE)

target_sources(mbed-uhuru-raven
    INTERFACE
        PeripheralPins.c
        system_clock.c
        uhuru_raven_init.c
)

target_include_directories(mbed-uhuru-raven
    INTERFACE
        .
)

target_link_libraries(mbed-uhuru-raven INTERFACE mbed-stm32f767xi)
