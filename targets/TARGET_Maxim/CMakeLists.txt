# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MAX32620C" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MAX32620C)
elseif("MAX32625" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MAX32625)
elseif("MAX32630" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MAX32630)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)
