# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-headers-kvstore INTERFACE)

target_include_directories(mbed-headers-kvstore
    INTERFACE
        ${mbed-os_SOURCE_DIR}/storage/kvstore/include
        ${mbed-os_SOURCE_DIR}/storage/kvstore/kv_config/include
        ${mbed-os_SOURCE_DIR}/storage/kvstore/kvstore_global_api/include
        ${mbed-os_SOURCE_DIR}/storage/kvstore/securestore/include
        ${mbed-os_SOURCE_DIR}/storage/kvstore/tdbstore/include
        ${mbed-os_SOURCE_DIR}/storage/kvstore/filesystemstore/include
)
