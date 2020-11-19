# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy_psoc6csp_rtos INTERFACE)

if("RTX" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-cy_psoc6csp_rtos
        INTERFACE
            include/COMPONENT_RTX
    )
    target_sources(mbed-cy_psoc6csp_rtos
        INTERFACE
            source/COMPONENT_RTX/cyabs_rtos_rtxv5.c
    )
endif()

target_include_directories(mbed-cy_psoc6csp_rtos
    INTERFACE
        include
)

target_sources(mbed-cy_psoc6csp_rtos
    INTERFACE
        source/cy_worker_thread.c
)
