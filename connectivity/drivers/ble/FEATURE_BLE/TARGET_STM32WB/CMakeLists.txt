# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-stm32wb INTERFACE)

target_include_directories(mbed-os-ble-stm32wb
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW
)

target_sources(mbed-os-ble-stm32wb
    INTERFACE
        HCIDriver.cpp
        hw_ipcc.c

        STM32Cube_FW/shci.c
        STM32Cube_FW/shci_tl.c
        STM32Cube_FW/stm_list.c
        STM32Cube_FW/tl_mbox.c
)

target_link_libraries(mbed-os-ble-stm32wb INTERFACE mbed-os-ble)
