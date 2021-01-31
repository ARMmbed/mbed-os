# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TMPM46B" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TMPM46B)
elseif("TMPM4G9" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TMPM4G9)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)
