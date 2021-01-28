# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(UHURU_RAVEN INTERFACE)

target_sources(UHURU_RAVEN
    INTERFACE
        PeripheralPins.c
        system_clock.c
        uhuru_raven_init.c
)

target_include_directories(UHURU_RAVEN
    INTERFACE
        .
)

target_link_libraries(UHURU_RAVEN INTERFACE STM32F767xI)
