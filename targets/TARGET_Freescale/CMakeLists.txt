# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MCUXpresso_MCUS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCUXpresso_MCUS)
endif()

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        USBPhy_Kinetis.cpp
)
