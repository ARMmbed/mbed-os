# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MIMXRT1050_EVK" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MIMXRT1050_EVK)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        imx_emac.cpp
)
