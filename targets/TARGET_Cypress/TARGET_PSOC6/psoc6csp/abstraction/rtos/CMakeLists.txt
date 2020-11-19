# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy_psoc6csp_rtos INTERFACE)

target_include_directories(mbed-cy_psoc6csp_rtos
    INTERFACE
        include
)

target_sources(mbed-cy_psoc6csp_rtos
    INTERFACE
        source/cy_worker_thread.c
)

target_compile_definitions(mbed-core
    INTERFACE
        MBED_CONF_CY_PSOC6CSP_RTOS_PRESENT=1
)
