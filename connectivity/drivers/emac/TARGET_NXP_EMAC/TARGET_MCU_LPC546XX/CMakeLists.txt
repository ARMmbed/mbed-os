# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("FF_LPC546XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_FF_LPC546XX)
elseif("LPCXpresso" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPCXpresso)
endif()

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        lpc546xx_emac.cpp
)
