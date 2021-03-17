# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME filesystemstore-unittest)

add_executable(${TEST_NAME})

target_include_directories(${TEST_NAME}
    PRIVATE
        ${MBED_PATH}
)

target_compile_definitions(${TEST_NAME} 
    PRIVATE
        UNITTEST
        MBED_LFS_READ_SIZE=64
        MBED_LFS_PROG_SIZE=64
        MBED_LFS_BLOCK_SIZE=512
        MBED_LFS_LOOKAHEAD=512
)

target_sources(${TEST_NAME}
    PRIVATE
        ${MBED_PATH}/storage/blockdevice/source/HeapBlockDevice.cpp
        ${MBED_PATH}/storage/kvstore/filesystemstore/source/FileSystemStore.cpp
        ${MBED_PATH}/storage/filesystem/littlefs/source/LittleFileSystem.cpp
        ${MBED_PATH}/storage/filesystem/source/Dir.cpp
        ${MBED_PATH}/storage/filesystem/source/File.cpp
        ${MBED_PATH}/storage/filesystem/source/FileSystem.cpp
        ${MBED_PATH}/platform/mbed-trace/source/mbed_trace.c        
        ${MBED_PATH}/storage/filesystem/littlefs/littlefs/lfs.c
        ${MBED_PATH}/platform/source/FileBase.cpp
        ${MBED_PATH}/platform/source/FileSystemHandle.cpp
        ${MBED_PATH}/platform/source/FileHandle.cpp        
        moduletest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers
        mbed-stubs-platform
        mbed-stubs-storage
        gmock_main
)

add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})

set_tests_properties(${TEST_NAME} PROPERTIES LABELS "storage")
