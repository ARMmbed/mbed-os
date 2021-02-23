# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/util.cmake)

add_subdirectory(TARGET_MAX32620C EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32625 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32630 EXCLUDE_FROM_ALL)

add_library(mbed-maxim INTERFACE)
add_library(mbed-maxim-obj OBJECT)

target_include_directories(mbed-maxim
    INTERFACE
        .
)

target_sources(mbed-maxim-obj
    PRIVATE
        USBPhy_Maxim.cpp
)

mbed_attach_object_lib_with_interface_lib(mbed-maxim)

target_link_libraries(mbed-maxim-obj PRIVATE mbed-cmsis-cortex-m mbed-core)
