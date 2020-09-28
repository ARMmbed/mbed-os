# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular OBJECT)

add_subdirectory(source/framework)

target_include_directories(mbed-os-cellular
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

target_link_libraries(mbed-os-cellular mbed-os-nanostack-libservice mbed-os-netsockets mbed-os)
