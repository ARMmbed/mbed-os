# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("K64F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_K64F)
elseif("K66F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_K66F)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        kinetis_emac.cpp
)
