# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(BUILD_GREENTEA_TESTS)
        add_subdirectory(TESTS)
    endif()
endif()

target_include_directories(mbed-ble-cordio
    INTERFACE
        .
        source
        stack_adaptation
)

target_sources(mbed-ble-cordio
    INTERFACE
        driver/CordioHCIDriver.cpp
        driver/CordioHCITransportDriver.cpp
        driver/H4TransportDriver.cpp

        source/BLEInstanceBaseImpl.cpp
        source/GattServerImpl.cpp
        source/PalAttClientImpl.cpp
        source/PalEventQueueImpl.cpp
        source/PalGapImpl.cpp
        source/PalGenericAccessServiceImpl.cpp
        source/PalPrivateAddressControllerImpl.cpp
        source/PalSecurityManagerImpl.cpp

        stack_adaptation/hci_tr.c
        stack_adaptation/hci_vs.c
        stack_adaptation/pal_mbed_os_adaptation.cpp
        stack_adaptation/wsf_cs.c
        stack_adaptation/wsf_mbed_os_adaptation.c
        stack_adaptation/wsf_os.c
        stack_adaptation/mbed_wsf_trace.c
)
