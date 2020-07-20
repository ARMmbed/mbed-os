# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(tech)
add_subdirectory(transceiver)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ndef
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

target_sources(mbed-os
    PRIVATE
        ndef/ndef.c

        platform/nfc_scheduler.c
        platform/nfc_transport.c
)
