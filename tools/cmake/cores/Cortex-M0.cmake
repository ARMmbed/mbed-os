# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb"
        "-mcpu=cortex-m0"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND common_options
        "-mcpu=cortex-m0"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_M0
            ARM_MATH_CM0
            __CMSIS_RTOS
    )
endfunction()
