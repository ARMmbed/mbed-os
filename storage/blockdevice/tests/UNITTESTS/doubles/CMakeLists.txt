# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-blockdevice)

target_include_directories(mbed-stubs-blockdevice
    PUBLIC
        .
)

target_sources(mbed-stubs-blockdevice
    PRIVATE
        BufferedBlockDevice_stub.cpp
        ChainingBlockDevice_stub.cpp
        ExhaustibleBlockDevice_stub.cpp
        FlashSimBlockDevice_stub.cpp
        HeapBlockDevice_stub.cpp
        MBRBlockDevice_stub.cpp
        ObservingBlockDevice_stub.cpp
        ProfilingBlockDevice_stub.cpp
        ReadOnlyBlockDevice_stub.cpp
        SlicingBlockDevice_stub.cpp
)

target_link_libraries(mbed-stubs-blockdevice
    PRIVATE
        mbed-headers-base
        mbed-headers-platform
        mbed-headers-storage
)
