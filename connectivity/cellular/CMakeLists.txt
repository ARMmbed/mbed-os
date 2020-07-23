# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(source/framework)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/API
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/AT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/device
)
