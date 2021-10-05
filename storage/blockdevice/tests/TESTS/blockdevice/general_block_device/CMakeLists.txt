# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.. CACHE INTERNAL "")
set(TEST_TARGET mbed-storage-blockdevice-general_block_device)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

if("DATAFLASH" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-dataflash)
endif()

if("FLASHIAP" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-flashiap)
endif()

if("I2CEE" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-i2cee)
endif()

if("OSPIF" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-ospif)
endif()

if("QSPIF" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-qspif)
endif()

if("SD" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-sd)
endif()

if("SPIF" IN_LIST MBED_TARGET_LABELS)
    list(APPEND mbed_blockdevice_libs mbed-storage-spif)
endif()

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage-blockdevice
        mbed-storage
        ${mbed_blockdevice_libs}
)
