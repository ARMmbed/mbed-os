# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/util.cmake)

add_subdirectory(TARGET_MAX32620C EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32625 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32630 EXCLUDE_FROM_ALL)

add_library(mbed-maxim INTERFACE)
add_library(mbed-maxim-obj INTERFACE)

target_include_directories(mbed-maxim
    INTERFACE
        .
)

target_sources(mbed-maxim-obj
    INTERFACE
        USBPhy_Maxim.cpp
)

target_include_directories(${MBED_TARGET_CONVERTED}-core
    INTERFACE
        .
)

target_link_libraries(mbed-maxim INTERFACE mbed-cmsis-cortex-m mbed-core)
