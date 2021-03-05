# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("ARCH_MAX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_ARCH_MAX)
elseif("NUCLEO_F429ZI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F429ZI)
elseif("NUCLEO_F439ZI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F439ZI)
endif()

add_library(mbed-emac-stm32f4 INTERFACE)

target_include_directories(mbed-emac-stm32f4
    INTERFACE
        .
)

target_link_libraries(mbed-emac-stm32f4 INTERFACE mbed-emac-stm)
