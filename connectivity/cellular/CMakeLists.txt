# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular INTERFACE)

add_subdirectory(source/framework)

target_include_directories(mbed-os-cellular
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/API
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/AT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cellular/framework/device
)

target_compile_definitions(mbed-os-cellular
    INTERFACE
        MBED_CONF_CELLULAR_PRESENT=1
)

target_link_libraries(mbed-os-cellular INTERFACE mbed-os-nanostack-libservice mbed-os-netsocket mbed-os)
