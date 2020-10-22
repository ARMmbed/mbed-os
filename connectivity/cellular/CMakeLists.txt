# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(source/framework)

target_include_directories(mbed-cellular
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

target_compile_definitions(mbed-cellular
    INTERFACE
        MBED_CONF_CELLULAR_PRESENT=1
)

target_link_libraries(mbed-cellular
    INTERFACE
        mbed-netsocket
        mbed-core
)
