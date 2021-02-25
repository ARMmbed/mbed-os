# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-headers INTERFACE)
add_library(mbed-headers INTERFACE)
add_library(mbed-headers-base INTERFACE)
add_library(mbed-headers-platform INTERFACE)
add_library(mbed-headers-connectivity INTERFACE)
add_library(mbed-headers-storage INTERFACE)
add_library(mbed-headers-drivers INTERFACE)
add_library(mbed-headers-hal INTERFACE)
add_library(mbed-headers-events INTERFACE)
add_library(mbed-headers-rtos INTERFACE)

target_link_libraries(mbed-headers
    INTERFACE
        mbed-headers-base
        mbed-headers-platform
        mbed-headers-connectivity
        mbed-headers-storage
        mbed-headers-drivers
        mbed-headers-hal
        mbed-headers-events
        mbed-headers-rtos
)

target_include_directories(mbed-headers-platform
    INTERFACE
        ${MBED_PATH}/platform/include
        ${MBED_PATH}/platform/include/platform
        ${MBED_PATH}/platform/randlib/include/mbed-client-randlib/
        ${MBED_PATH}/platform/randlib/include/
        ${MBED_PATH}/platform/mbed-trace/include
)

target_include_directories(mbed-headers-base
    INTERFACE
        ${MBED_PATH}/UNITTESTS/target_h
        ${MBED_PATH}/UNITTESTS/target_h/platform
        ${MBED_PATH}/UNITTESTS/target_h/platform/cxxsupport
        ${MBED_PATH}/UNITTESTS/target_h/drivers
        ${MBED_PATH}/UNITTESTS/target_h/rtos/include
        ${MBED_PATH}/UNITTESTS/target_h/rtos
        ${MBED_PATH}/UNITTESTS/target_h/sys
)

target_include_directories(mbed-headers-storage
    INTERFACE        
        ${MBED_PATH}/storage/filesystem/fat/include
        ${MBED_PATH}/storage/filesystem/fat/ChaN
        ${MBED_PATH}/storage/filesystem/littlefs/include
        ${MBED_PATH}/storage/filesystem/littlefsv2/littlefs
        ${MBED_PATH}/storage/filesystem/littlefsv2/littlefs/bd        
        ${MBED_PATH}/storage/filesystem/littlefs/littlefs
        ${MBED_PATH}/storage/blockdevice/include
        ${MBED_PATH}/storage/filesystem/include
        ${MBED_PATH}/storage/kvstore/include
        ${MBED_PATH}/storage/kvstore/kv_config
        ${MBED_PATH}/storage/kvstore/kv_config/include
        ${MBED_PATH}/storage/kvstore/tdbstore/include
        ${MBED_PATH}/storage/kvstore/filesystemstore/include
        ${MBED_PATH}/storage/kvstore/kvstore_global_api/include
        ${MBED_PATH}/storage/blockdevice/include/blockdevice
)

target_include_directories(mbed-headers-connectivity
    INTERFACE
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/mbed-client-libservice
        ${MBED_PATH}/connectivity/netsocket/include
        ${MBED_PATH}/connectivity/cellular/include/cellular/framework/API
        ${MBED_PATH}/connectivity/cellular/include/cellular/framework/AT
        ${MBED_PATH}/connectivity/cellular/include/cellular/framework/device
        ${MBED_PATH}/connectivity/cellular/include/cellular/framework
        ${MBED_PATH}/connectivity/cellular/include/cellular/framework/common
        ${MBED_PATH}/connectivity
        ${MBED_PATH}/connectivity/lorawan/include/lorawan
        ${MBED_PATH}/connectivity/lorawan/lorastack
        ${MBED_PATH}/connectivity/lorawan/lorastack/mac
        ${MBED_PATH}/connectivity/lorawan/lorastack/phy
        ${MBED_PATH}/connectivity/lorawan
        ${MBED_PATH}/connectivity/mbedtls
        ${MBED_PATH}/connectivity/mbedtls/include
        ${MBED_PATH}/connectivity/FEATURE_BLE/include
        ${MBED_PATH}/connectivity/FEATURE_BLE/include/ble
)

target_include_directories(mbed-headers-drivers
    INTERFACE
        ${MBED_PATH}/drivers
        ${MBED_PATH}/drivers/include
        ${MBED_PATH}/drivers/include/drivers        
)

target_include_directories(mbed-headers-events
    INTERFACE
        ${MBED_PATH}/events/tests/UNITTESTS/target_h
        ${MBED_PATH}/events/tests/UNITTESTS/target_h/equeue
        ${MBED_PATH}/events/include
        ${MBED_PATH}/events/include/events
        ${MBED_PATH}/events/include/events/internal        
)

target_include_directories(mbed-headers-hal
    INTERFACE
        ${MBED_PATH}/hal
        ${MBED_PATH}/hal/include
)

target_include_directories(mbed-headers-rtos
    INTERFACE
        ${MBED_PATH}/rtos/include
        ${MBED_PATH}/rtos/include/rtos
)

target_include_directories(mbed-headers
    INTERFACE
        ${MBED_PATH}/features
        ${MBED_PATH}/features/frameworks
)

target_include_directories(mbed-stubs-headers
    INTERFACE
        .
)

add_subdirectory(connectivity)
add_subdirectory(drivers)
add_subdirectory(events)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(rtos)
add_subdirectory(storage)

add_library(mbed-stubs INTERFACE)

target_link_libraries(mbed-stubs
    INTERFACE
        mbed-stubs-connectivity
        mbed-stubs-drivers
        mbed-stubs-events
        mbed-stubs-hal
        mbed-stubs-platform
        mbed-stubs-rtos
        mbed-stubs-storage
)
