# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(stack)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/driver
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_adaptation
)

target_sources(mbed-os
    PRIVATE
        driver/CordioHCIDriver.cpp
        driver/CordioHCITransportDriver.cpp
        driver/H4TransportDriver.cpp

        source/BLEInstanceBaseImpl.cpp
        source/PalAttClientImpl.cpp
        source/PalEventQueueImpl.cpp
        source/PalGapImpl.cpp
        source/PalGattClientImpl.cpp
        source/PalGenericAccessServiceImpl.cpp
        source/PalSecurityManagerImpl.cpp
        source/PalSigningMonitorImpl.cpp

        stack_adaptation/hci_tr.c
        stack_adaptation/hci_vs.c
        stack_adaptation/pal_mbed_os_adaptation.cpp
        stack_adaptation/wsf_cs.c
        stack_adaptation/wsf_mbed_os_adaptation.c
        stack_adaptation/wsf_os.c
)
