# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("GD32F30X" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_GD32F30X)
elseif("GD32F4XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_GD32F4XX)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        gd32xx_emac.cpp
)
