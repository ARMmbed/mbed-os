# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F413ZH INTERFACE)

target_sources(NUCLEO_F413ZH
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_F413ZH
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F413ZH INTERFACE STM32F413xH)
