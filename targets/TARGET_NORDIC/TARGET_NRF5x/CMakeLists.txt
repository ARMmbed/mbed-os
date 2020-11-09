# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NRF52" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NRF52)
endif()

if("SDK_11" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_SDK_11)
elseif("SDK_15_0" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_SDK_15_0)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        lp_ticker.c
        pinmap.c
        port_api.c
        qspi_api.c
        rtc_api.c
)
