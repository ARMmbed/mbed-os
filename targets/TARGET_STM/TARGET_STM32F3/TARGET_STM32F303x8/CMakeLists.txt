# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_F303K8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F303K8)
endif()

add_subdirectory(device)

target_include_directories(mbed-core
    INTERFACE
        .
)
