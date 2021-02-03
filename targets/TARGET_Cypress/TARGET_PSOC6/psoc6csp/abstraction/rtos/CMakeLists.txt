# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("CY_RTOS_AWARE" IN_LIST MBED_CONFIG_DEFINITIONS)
    if("RTX" IN_LIST MBED_TARGET_LABELS)
        target_include_directories(mbed-core
            INTERFACE
                include/COMPONENT_RTX
        )
        target_sources(mbed-core
            INTERFACE
                source/COMPONENT_RTX/cyabs_rtos_rtxv5.c
        )
    endif()

    target_include_directories(mbed-core
        INTERFACE
            include
    )

    target_sources(mbed-core
        INTERFACE
            source/cy_worker_thread.c
    )
endif()
