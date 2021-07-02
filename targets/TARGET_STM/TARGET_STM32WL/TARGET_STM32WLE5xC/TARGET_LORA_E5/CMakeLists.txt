# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lora-e5 INTERFACE)

target_sources(mbed-lora-e5
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-lora-e5
    INTERFACE
        .
)

target_link_libraries(mbed-lora-e5 INTERFACE mbed-stm32wle5xc)
