# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-storage)

target_sources(mbed-stubs-storage
    PRIVATE
        BufferedBlockDevice_stub.cpp
        ChainingBlockDevice_stub.cpp
        EmulatedSD.cpp
        ExhaustibleBlockDevice_stub.cpp
        FlashSimBlockDevice_stub.cpp
        HeapBlockDevice_stub.cpp
        MBRBlockDevice_stub.cpp
        ObservingBlockDevice_stub.cpp
        ProfilingBlockDevice_stub.cpp
        ReadOnlyBlockDevice_stub.cpp
        SlicingBlockDevice_stub.cpp
        kv_config_stub.cpp
)

target_link_libraries(mbed-stubs-storage
    PRIVATE
        mbed-headers
        mbed-stubs-headers
)
