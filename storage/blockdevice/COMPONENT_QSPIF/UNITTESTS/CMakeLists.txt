# Copyright (c) 2021 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

add_executable(qspif-unittest)

target_compile_definitions(qspif-unittest
    PRIVATE
        DEVICE_QSPI=1
        MBED_CONF_QSPIF_QSPI_MIN_READ_SIZE=1
        MBED_CONF_QSPIF_QSPI_MIN_PROG_SIZE=1
)

target_include_directories(qspif-unittest
    PRIVATE
        ${mbed-os_SOURCE_DIR}/storage/blockdevice/COMPONENT_QSPIF/include
)

target_sources(qspif-unittest
    PRIVATE
        ${mbed-os_SOURCE_DIR}/storage/blockdevice/COMPONENT_QSPIF/source/QSPIFBlockDevice.cpp
        test_QSPIFBlockDevice.cpp
)

target_link_libraries(qspif-unittest
    PRIVATE
        mbed-headers-blockdevice
        mbed-headers-drivers
        mbed-headers-platform
        mbed-headers-rtos
        mbed-stubs-blockdevice
        mbed-stubs-platform
        mbed-stubs-drivers
        mbed-stubs-rtos
        gmock_main
)

gtest_discover_tests(qspif-unittest PROPERTIES LABELS "storage")
