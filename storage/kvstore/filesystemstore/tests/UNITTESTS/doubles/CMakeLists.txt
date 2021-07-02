# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-filesystemstore)

target_sources(mbed-stubs-filesystemstore
    PRIVATE
        filesystemstore_kv_config_stub.cpp
)

target_link_libraries(mbed-stubs-filesystemstore
    PRIVATE
        mbed-headers-blockdevice
        mbed-headers-kvstore
)
