# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os-emac-stm32xx
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-stm32xx
    INTERFACE
        stm32f7_eth_conf.c
)
