# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy_psoc6_scl INTERFACE)

target_include_directories(mbed-cy_psoc6_scl
    INTERFACE
        .
        inc
        src/include
)

target_sources(mbed-cy_psoc6_scl
    INTERFACE
        scl_buffer_api.c
        scl_wifi_api.c

        src/IPC/scl_ipc.c
)
