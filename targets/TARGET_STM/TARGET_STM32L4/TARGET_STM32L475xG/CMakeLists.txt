# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_L475VG_IOT01A" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_L475VG_IOT01A)
endif()

add_subdirectory(device)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
