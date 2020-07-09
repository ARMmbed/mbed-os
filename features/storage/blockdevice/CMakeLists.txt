# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        BufferedBlockDevice.cpp
        ChainingBlockDevice.cpp
        ExhaustibleBlockDevice.cpp
        FlashSimBlockDevice.cpp
        HeapBlockDevice.cpp
        MBRBlockDevice.cpp
        ObservingBlockDevice.cpp
        ProfilingBlockDevice.cpp
        ReadOnlyBlockDevice.cpp
        SlicingBlockDevice.cpp
)
