# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/gap
        ${CMAKE_CURRENT_SOURCE_DIR}/generic
        ${CMAKE_CURRENT_SOURCE_DIR}/pal
        ${CMAKE_CURRENT_SOURCE_DIR}/services
)
