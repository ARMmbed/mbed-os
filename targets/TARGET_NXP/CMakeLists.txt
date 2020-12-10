# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("LPC11XX_11CXX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPC11XX_11CXX)
elseif("LPC176X" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPC176X)
elseif("MCUXpresso_MCUS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCUXpresso_MCUS)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        USBHAL_LPC17.cpp
)
