# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lorawan-stm32wl INTERFACE)

target_include_directories(mbed-lorawan-stm32wl
    INTERFACE
        .
)

target_sources(mbed-lorawan-stm32wl
    INTERFACE
        STM32WL_LoRaRadio.cpp
)

target_link_libraries(mbed-lorawan
    PRIVATE
        mbed-lorawan-stm32wl
)

target_include_directories(mbed-lorawan
    PUBLIC
        $<TARGET_PROPERTY:mbed-lorawan-stm32wl,INTERFACE_INCLUDE_DIRECTORIES>
)
