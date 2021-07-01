# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb"
        "-mfpu=fpv5-d16"
        "-mfloat-abi=softfp"
        "-mcpu=cortex-m7"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND common_options
        "-mfpu=fpv5-d16"
        "-mfloat-abi=hard"
        "-mcpu=cortex-m7"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_M7
            ARM_MATH_CM7
            __FPU_PRESENT=1
            __CMSIS_RTOS
    )
endfunction()
