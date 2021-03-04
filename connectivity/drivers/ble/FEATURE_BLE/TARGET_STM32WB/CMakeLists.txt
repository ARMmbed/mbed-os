# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-stm32wb INTERFACE)

target_include_directories(mbed-ble-stm32wb
    INTERFACE
        .
        ./STM32Cube_FW
)

target_sources(mbed-ble-stm32wb
    INTERFACE
        HCIDriver.cpp
        hw_ipcc.c

        STM32Cube_FW/shci.c
        STM32Cube_FW/shci_tl.c
        STM32Cube_FW/stm_list.c
        STM32Cube_FW/tl_mbox.c
)

target_link_libraries(mbed-ble PRIVATE mbed-ble-stm32wb)
