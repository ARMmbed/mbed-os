# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-stmod INTERFACE)

target_include_directories(mbed-cellular-stmod
    INTERFACE
        .
)

target_sources(mbed-cellular-stmod
    INTERFACE
        STModCellular.cpp
)
