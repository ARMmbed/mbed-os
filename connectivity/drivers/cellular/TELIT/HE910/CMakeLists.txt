# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-telit-he910 OBJECT)

target_include_directories(mbed-os-cellular-telit-he910
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-telit-he910
    PRIVATE
        TELIT_HE910.cpp
)
