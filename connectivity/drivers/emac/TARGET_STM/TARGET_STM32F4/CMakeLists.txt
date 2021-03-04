# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-stm32f4 INTERFACE)

add_subdirectory(TARGET_ARCH_MAX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F429ZI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F439ZI EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-stm32f4
    INTERFACE
        .
)

target_link_libraries(mbed-emac-stm32f4 INTERFACE mbed-emac-stm)
