# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


add_subdirectory(TARGET_MAX32620C EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32625 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32630 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32660 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32670 EXCLUDE_FROM_ALL)

add_library(mbed-maxim INTERFACE)

target_include_directories(mbed-maxim
    INTERFACE
        .
)

target_sources(mbed-maxim
    INTERFACE
        USBPhy_Maxim.cpp
)

target_link_libraries(mbed-maxim INTERFACE mbed-cmsis-cortex-m)
