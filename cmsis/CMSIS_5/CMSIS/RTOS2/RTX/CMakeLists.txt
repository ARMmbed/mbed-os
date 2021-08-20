# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${CMAKE_CROSSCOMPILING})
    function(_mbed_get_cortex_m_exception_handlers toolchain_dir)
        foreach(key ${MBED_TARGET_LABELS})
            if(${key} STREQUAL CORTEX_A)
                set(STARTUP_RTX_FILE TARGET_CORTEX_A/irq_ca.S)
            elseif(${key} STREQUAL M0)
                set(STARTUP_RTX_FILE TARGET_M0/irq_cm0.S)
            elseif(${key} STREQUAL M0P)
                set(STARTUP_RTX_FILE TARGET_M0P/irq_cm0.S)
            elseif(${key} STREQUAL M23)
                set(STARTUP_RTX_FILE TARGET_M23/irq_armv8mbl.S)
            elseif(${key} STREQUAL M3)
                set(STARTUP_RTX_FILE TARGET_M3/irq_cm3.S)
            elseif(${key} STREQUAL M33)
                set(STARTUP_RTX_FILE TARGET_M33/irq_armv8mml.S)
            elseif(${key} STREQUAL M55)
                set(STARTUP_RTX_FILE TARGET_M33/irq_armv8mml.S)
            elseif(${key} STREQUAL RTOS_M4_M7)
                set(STARTUP_RTX_FILE TARGET_RTOS_M4_M7/irq_cm4f.S)
            endif()

            target_sources(mbed-rtos
                INTERFACE
                    Source/${toolchain_dir}/${STARTUP_RTX_FILE}
            )
        endforeach()
    endfunction()
    
    function(_mbed_get_cortex_a_exception_handlers)
        foreach(key ${MBED_TARGET_LABELS})
            if(${key} STREQUAL CORTEX_A)
                target_sources(mbed-rtos INTERFACE Config/TARGET_CORTEX_A/handlers.c)
            endif()
        endforeach()
    endfunction()
    
    _mbed_get_cortex_a_exception_handlers()
    
    _mbed_get_cortex_m_exception_handlers(TOOLCHAIN_GCC)
endif()

target_include_directories(mbed-rtos
    INTERFACE
        Config
        Include
        Include1
        Source
)

target_sources(mbed-rtos
    INTERFACE
        Config/RTX_Config.c

        Library/cmsis_os1.c

        Source/rtx_delay.c
        Source/rtx_evflags.c
        Source/rtx_evr.c
        Source/rtx_kernel.c
        Source/rtx_lib.c
        Source/rtx_memory.c
        Source/rtx_mempool.c
        Source/rtx_msgqueue.c
        Source/rtx_mutex.c
        Source/rtx_semaphore.c
        Source/rtx_system.c
        Source/rtx_thread.c
        Source/rtx_timer.c
)
