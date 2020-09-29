# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular-telit-he910 OBJECT)

target_include_directories(mbed-os-cellular-telit-he910
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-telit-he910
    PRIVATE
        TELIT_HE910.cpp
)

mbed_link_component(mbed-os-cellular-telit-he910 mbed-os-cellular mbed-os)
