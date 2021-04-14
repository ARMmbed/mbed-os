# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_PSOC6 EXCLUDE_FROM_ALL)

add_library(mbed-cy-external-wifi-fw INTERFACE)
target_compile_definitions(mbed-cy-external-wifi-fw
    INTERFACE
        "CY_ENABLE_XIP_PROGRAM"
        "CY_STORAGE_WIFI_DATA=\".cy_xip\""
        "CY_STORAGE_WIFI_DATA_OUTPUT=cy_xip"
        "CY_EXT_WIFI_FW_STORAGE=QSPIF"
)

