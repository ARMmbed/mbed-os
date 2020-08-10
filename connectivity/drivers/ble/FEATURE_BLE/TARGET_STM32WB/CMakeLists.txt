# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW
)

target_sources(mbed-os
    PRIVATE
        HCIDriver.cpp
        hw_ipcc.c

        STM32Cube_FW/shci.c
        STM32Cube_FW/shci_tl.c
        STM32Cube_FW/stm_list.c
        STM32Cube_FW/tl_mbox.c
)
