# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_sources(cmsis
        PRIVATE
            TOOLCHAIN_GCC_ARM/mbed_boot_gcc_arm.c
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    target_sources(cmsis
        PRIVATE
            TOOLCHAIN_ARM_STD/mbed_boot_arm_std.c
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    target_sources(cmsis
        PRIVATE
            TOOLCHAIN_IAR/mbed_boot_iar.c
    )
endif()

target_include_directories(cmsis
    PUBLIC
        include
)

target_sources(cmsis PRIVATE
    source/mbed_boot.c
    source/mbed_rtos_rtx.c
    source/mbed_rtx_handlers.c
    source/mbed_rtx_idle.cpp
)
