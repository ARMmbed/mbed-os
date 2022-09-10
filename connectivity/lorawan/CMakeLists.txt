# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-lorawan STATIC EXCLUDE_FROM_ALL)

add_subdirectory(lorastack)
add_subdirectory(system)

target_include_directories(mbed-lorawan
    PUBLIC
        ..
        .
        ./include
        ./include/lorawan
)

target_sources(mbed-lorawan
    PRIVATE
        source/LoRaWANInterface.cpp
        source/LoRaWANStack.cpp
)

target_compile_definitions(mbed-lorawan
    PUBLIC
        MBED_CONF_LORAWAN_PRESENT=1
)

target_link_libraries(mbed-lorawan
    PUBLIC
        mbed-mbedtls
)
