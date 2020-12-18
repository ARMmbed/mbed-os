# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32H745xI_CM4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H745xI_CM4)
elseif("STM32H745xI_CM7" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H745xI_CM7)
endif()

target_sources(mbed-core
    INTERFACE
        system_clock.c
)
