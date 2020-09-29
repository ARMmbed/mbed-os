# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular-stmod OBJECT)

target_include_directories(mbed-os-cellular-stmod
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-stmod
    PRIVATE
        STModCellular.cpp
)

mbed_link_component(mbed-os-cellular-stmod mbed-os-cellular mbed-os)
