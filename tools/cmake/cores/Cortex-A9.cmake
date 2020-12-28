# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb-interwork"
        "-marm"
        "-march=armv7-a"
        "-mfpu=vfpv3"
        "-mfloat-abi=hard"
        "-mno-unaligned-access"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND c_cxx_compile_options
        "-mcpu=cortex-a9"
    )
    list(APPEND asm_compile_options
        "-mcpu=Cortex-A9"
    )
    list(APPEND link_options
        "--cpu=Cortex-A9"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_A9
            ARM_MATH_CA9
            __FPU_PRESENT
            __CMSIS_RTOS
            __EVAL
            __MBED_CMSIS_RTOS_CA9
    )
endfunction()
