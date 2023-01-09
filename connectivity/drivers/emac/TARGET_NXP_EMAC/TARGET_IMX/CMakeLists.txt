# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MIMXRT1050_EVK" IN_LIST MBED_TARGET_LABELS OR "MIMXRT1060_EVK" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MIMXRT10x0_EVK)
endif()

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        imx_emac.cpp
)
