# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${CMAKE_CROSSCOMPILING})
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        target_sources(mbed-rtos
            INTERFACE
                TOOLCHAIN_GCC_ARM/mbed_boot_gcc_arm.c
        )
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        target_sources(mbed-rtos
            INTERFACE
                TOOLCHAIN_ARM_STD/mbed_boot_arm_std.c
        )
    endif()
endif()

target_include_directories(mbed-rtos
    INTERFACE
        include
)

target_sources(mbed-rtos
    INTERFACE
        source/mbed_boot.c
        source/mbed_rtos_rtx.c
        source/mbed_rtx_handlers.c
        source/mbed_rtx_idle.cpp
)

target_compile_definitions(mbed-rtos
    INTERFACE
        MBED_CONF_RTOS_PRESENT=1
)
