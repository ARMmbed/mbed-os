# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NANO_RP2040_CONNECT" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_NANO_RP2040_CONNECT
    )
endif()

add_subdirectory(rp2_common)
add_subdirectory(rp2040)