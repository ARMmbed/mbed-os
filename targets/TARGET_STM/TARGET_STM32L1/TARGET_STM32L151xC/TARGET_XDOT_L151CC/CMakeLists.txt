# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-xdot-l151cc INTERFACE)

target_sources(mbed-xdot-l151cc
    INTERFACE
        PeripheralPins.c
        system_clock.c
        xdot_eeprom.c
        xdot_low_power.c
)

target_include_directories(mbed-xdot-l151cc
    INTERFACE
        .
)

target_link_libraries(mbed-xdot-l151cc INTERFACE mbed-stm32l151xc)

add_library(mbed-ff1705-l151cc INTERFACE)
target_link_libraries(mbed-ff1705-l151cc INTERFACE mbed-xdot-l151cc)
