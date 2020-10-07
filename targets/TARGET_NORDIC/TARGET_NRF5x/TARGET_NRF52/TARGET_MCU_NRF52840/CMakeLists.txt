# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NRF52840_DK" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NRF52840_DK)
endif()

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/config
)

target_sources(mbed-os
    PRIVATE
        PeripheralPins.c
        USBPhy_Nordic.cpp
)

add_subdirectory(device)
