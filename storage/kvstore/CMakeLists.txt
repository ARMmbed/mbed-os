# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(tdbstore)
add_subdirectory(filesystemstore)
add_subdirectory(securestore)
add_subdirectory(kv_config)
add_subdirectory(direct_access_devicekey)
add_subdirectory(kvstore_global_api)

target_include_directories(mbed-storage-kvstore
    INTERFACE
        .
        include
        include/kvstore
)

target_link_libraries(mbed-storage-kvstore
    INTERFACE
        mbed-storage-kv-global-api
)
