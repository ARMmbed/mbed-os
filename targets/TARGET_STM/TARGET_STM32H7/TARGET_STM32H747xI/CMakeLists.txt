# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_H747I" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_H747I)
elseif("STM32H747xI_CM4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H747xI_CM4)
elseif("STM32H7A3xIQ" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H7A3xIQ)
endif()

target_sources(mbed-core
    INTERFACE
        system_clock.c
)
