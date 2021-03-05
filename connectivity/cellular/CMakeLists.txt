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

target_link_libraries(mbed-cellular
    PRIVATE
        mbed-events
        mbed-core
        mbed-rtos
        mbed-randlib
)

# Get common headers from netsockets, cellular requires only declarations
# TODO: if we link to netsocket, we get entire tree of dependencies that are very hard to
# get rid of. The huge refactor is required for nanostack and netsockets
target_include_directories(mbed-cellular PUBLIC $<TARGET_PROPERTY:mbed-netsocket,INCLUDE_DIRECTORIES>)
