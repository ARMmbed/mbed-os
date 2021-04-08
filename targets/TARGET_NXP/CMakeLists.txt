# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/scripts")

add_subdirectory(TARGET_LPC11XX_11CXX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPC176X EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCUXpresso_MCUS EXCLUDE_FROM_ALL)

add_library(mbed-nxp INTERFACE)

target_include_directories(mbed-nxp
    INTERFACE
        .
)

target_sources(mbed-nxp
    INTERFACE
        USBHAL_LPC17.cpp
)

target_link_libraries(mbed-nxp INTERFACE mbed-cmsis-cortex-m)
