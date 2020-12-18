# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb"
        "-mcpu=cortex-m0"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND c_cxx_compile_options
        "-mcpu=cortex-m0"
    )
    list(APPEND asm_compile_options
        "-mcpu=Cortex-M0"
    )
    list(APPEND link_options
        "--cpu=Cortex-M0"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_M0
            ARM_MATH_CM0
            __CMSIS_RTOS
            __MBED_CMSIS_RTOS_CM
    )
endfunction()
