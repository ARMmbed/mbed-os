# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/conf
        ${CMAKE_CURRENT_SOURCE_DIR}/direct_access_devicekey
        ${CMAKE_CURRENT_SOURCE_DIR}/filesystemstore
        ${CMAKE_CURRENT_SOURCE_DIR}/global_api
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/kv_map
        ${CMAKE_CURRENT_SOURCE_DIR}/securestore
        ${CMAKE_CURRENT_SOURCE_DIR}/tdbstore
)

target_sources(mbed-os
    PRIVATE
        conf/kv_config.cpp

        direct_access_devicekey/DirectAccessDevicekey.cpp

        filesystemstore/FileSystemStore.cpp

        global_api/kvstore_global_api.cpp

        kv_map/KVMap.cpp

        securestore/SecureStore.cpp

        tdbstore/TDBStore.cpp
)
