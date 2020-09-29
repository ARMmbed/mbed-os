# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-stmod INTERFACE)

target_include_directories(mbed-os-cellular-stmod
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-stmod
    INTERFACE
        STModCellular.cpp
)

target_link_libraries(mbed-os-cellular-stmod INTERFACE mbed-os-cellular)
