# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb-interwork"
        "-marm"
        "-mfpu=vfpv3"
        "-mfloat-abi=softfp"
        "-mno-unaligned-access"
        "-mcpu=${MBED_CPU_CORE}"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND common_options
        "-mfpu=vfpv3"
        "-mfloat-abi=hard"
        "-mcpu=${MBED_CPU_CORE}"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_${MBED_CPU_CORE_CODE}
            ARM_MATH_C${MBED_CPU_CORE_CODE}
            __FPU_PRESENT
            __CMSIS_RTOS
            __EVAL
    )
endfunction()
