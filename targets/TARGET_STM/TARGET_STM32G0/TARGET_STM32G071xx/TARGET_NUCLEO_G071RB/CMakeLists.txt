# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-g071rb INTERFACE)

target_sources(mbed-nucleo-g071rb
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-g071rb
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-g071rb INTERFACE mbed-stm32g071xx)
