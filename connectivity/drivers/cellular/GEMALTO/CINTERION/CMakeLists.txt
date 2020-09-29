# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular-gemalto-cinterion OBJECT)

target_include_directories(mbed-os-cellular-gemalto-cinterion
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-gemalto-cinterion
    PRIVATE
        GEMALTO_CINTERION.cpp
        GEMALTO_CINTERION_CellularContext.cpp
        GEMALTO_CINTERION_CellularInformation.cpp
        GEMALTO_CINTERION_CellularStack.cpp
)

mbed_link_component(mbed-os-cellular-gemalto-cinterion mbed-os-cellular mbed-os)
