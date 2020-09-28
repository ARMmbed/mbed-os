# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-client-cli OBJECT)

target_include_directories(mbed-os-client-cli
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mbed-client-cli
)

target_sources(mbed-os-client-cli
    PRIVATE
        source/ns_cmdline.c
)
