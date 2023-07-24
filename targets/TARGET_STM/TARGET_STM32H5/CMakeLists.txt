# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32h5 INTERFACE)

target_sources(mbed-stm32h5
    INTERFACE
)

target_include_directories(mbed-stm32h5
    INTERFACE
        .
)

target_link_libraries(mbed-stm32h5 INTERFACE mbed-stm mbed-stm32h5cube-fw)
