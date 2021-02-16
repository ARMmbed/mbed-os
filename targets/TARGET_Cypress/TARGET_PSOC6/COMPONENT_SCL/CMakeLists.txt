# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy-psoc6-scl INTERFACE)

target_include_directories(mbed-cy-psoc6-scl
    INTERFACE
        .
        inc
        src/include
)

target_sources(mbed-cy-psoc6-scl
    INTERFACE
        src/scl_buffer_api.c
        src/scl_wifi_api.c

        src/IPC/scl_ipc.c
)
