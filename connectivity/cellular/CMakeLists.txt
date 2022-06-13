# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-cellular STATIC EXCLUDE_FROM_ALL)

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

target_sources(mbed-cellular
    PRIVATE
        source/CellularNonIPSocket.cpp
)

target_link_libraries(mbed-cellular
    PUBLIC
        mbed-netsocket-api
        mbed-core-flags
        mbed-randlib
)
