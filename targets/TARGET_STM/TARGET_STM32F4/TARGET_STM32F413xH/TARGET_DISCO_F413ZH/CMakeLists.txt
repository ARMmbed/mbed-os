# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-f413zh INTERFACE)

target_sources(mbed-disco-f413zh
    INTERFACE
        PeripheralPins.c
        TargetInit.c
)

target_include_directories(mbed-disco-f413zh
    INTERFACE
        .
)

target_link_libraries(mbed-disco-f413zh INTERFACE mbed-stm32f413xh)
