# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-imx INTERFACE)

add_subdirectory(TARGET_MIMXRT1050_EVK EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-imx
    INTERFACE
        .
)

target_sources(mbed-emac-imx
    INTERFACE
        imx_emac.cpp
)
