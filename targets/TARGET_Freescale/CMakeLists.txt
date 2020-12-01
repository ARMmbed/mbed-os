# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("KLXX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_KLXX)
elseif("MCUXpresso_MCUS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCUXpresso_MCUS)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        USBPhy_Kinetis.cpp
)
