# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-ble-stm32wb OBJECT)

target_include_directories(mbed-os-ble-stm32wb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW
)

target_sources(mbed-os-ble-stm32wb
    PRIVATE
        HCIDriver.cpp
        hw_ipcc.c

        STM32Cube_FW/shci.c
        STM32Cube_FW/shci_tl.c
        STM32Cube_FW/stm_list.c
        STM32Cube_FW/tl_mbox.c
)

mbed_link_component(mbed-os-ble-stm32wb mbed-os-ble mbed-os)
