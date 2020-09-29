# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-stm32xx OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os-emac-stm32xx
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-stm32xx
    PRIVATE
        stm32xx_emac.cpp
)

target_link_libraries(mbed-os-emac-stm32xx mbed-os-netsocket mbed-os)
