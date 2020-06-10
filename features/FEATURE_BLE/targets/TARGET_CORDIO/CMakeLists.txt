# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(stack)

target_sources(mbed-os
    PRIVATE
        driver/CordioHCIDriver.cpp
        driver/CordioHCITransportDriver.cpp
        driver/H4TransportDriver.cpp

        source/CordioBLE.cpp
        source/CordioGattServer.cpp
        source/CordioImplementations.cpp
        source/CordioPalAttClient.cpp
        source/CordioPalGap.tpp
        source/CordioPalSecurityManager.tpp

        stack_adaptation/hci_drv.c
        stack_adaptation/hci_tr.c
        stack_adaptation/hci_vs.c
        stack_adaptation/pal_mbed_os_adaptation.cpp
        stack_adaptation/wsf_cs.c
        stack_adaptation/wsf_mbed_os_adaptation.c
        stack_adaptation/wsf_os.c
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/driver
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_adaptation
)
