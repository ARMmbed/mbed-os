# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("EFM32" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_EFM32)
endif()

if("SL_RAIL" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_SL_RAIL)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)
