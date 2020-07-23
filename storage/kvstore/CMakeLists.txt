# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(direct_access_devicekey)
add_subdirectory(kv_config)
add_subdirectory(securestore)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/kvstore
)

target_sources(mbed-os
    PRIVATE
        source/FileSystemStore.cpp
        source/KVMap.cpp
        source/TDBStore.cpp
        source/kvstore_global_api.cpp
)
