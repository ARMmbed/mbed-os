# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND c_cxx_compile_options
        "-mcpu=cortex-m1"
    )
    list(APPEND asm_compile_options
        "-mcpu=Cortex-M1"
    )
    list(APPEND link_options
        "--cpu=Cortex-M1"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_M3
            ARM_MATH_CM1
            __CMSIS_RTOS
            __MBED_CMSIS_RTOS_CM
    )
endfunction()
