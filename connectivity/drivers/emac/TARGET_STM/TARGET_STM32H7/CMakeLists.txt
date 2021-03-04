# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-stm32h7 INTERFACE)

add_subdirectory(TARGET_DISCO_H747I EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_PORTENTA_H7 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_H743ZI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_H743ZI2 EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-stm32h7
    INTERFACE
        .
        ./lan8742
)

target_sources(mbed-emac-stm32h7
    INTERFACE
        lan8742/lan8742.c
)

target_link_libraries(mbed-emac-stm32h7 INTERFACE mbed-emac-stm)
