# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-802.15.4-kw41z OBJECT)


target_include_directories(mbed-os-802.15.4-kw41z
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-802.15.4-kw41z
    PRIVATE
        NanostackRfPhyKw41z.cpp
)

mbed_link_component(mbed-os-802.15.4-kw41z mbed-os-nanostack mbed-os)
