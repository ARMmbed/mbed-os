# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

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

target_link_libraries(mbed-storage-kvstore
    INTERFACE
        mbed-storage-kv-global-api
        ${mbed_blockdevice_libs}
)
