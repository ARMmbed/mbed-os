# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular OBJECT)

add_subdirectory(source/framework)

target_include_directories(mbed-cellular
    PUBLIC
        .
        ./include
        ./include/cellular
        ./include/cellular/framework
        ./include/cellular/framework/API
        ./include/cellular/framework/AT
        ./include/cellular/framework/common
        ./include/cellular/framework/device
)

target_compile_definitions(mbed-cellular
    PUBLIC
        MBED_CONF_CELLULAR_PRESENT=1
)

target_link_libraries(mbed-cellular mbed-netsocket mbed-core mbed-rtos)
