# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy-rtx INTERFACE)
target_include_directories(mbed-cy-rtx
    INTERFACE
        include/COMPONENT_RTX
)
target_sources(mbed-cy-rtx
    INTERFACE
        source/COMPONENT_RTX/cyabs_rtos_rtxv5.c
)

add_library(mbed-cy-rtos INTERFACE)
target_include_directories(mbed-cy-rtos
    INTERFACE
        include
)
target_sources(mbed-cy-rtos
    INTERFACE
        source/cy_worker_thread.c
)
