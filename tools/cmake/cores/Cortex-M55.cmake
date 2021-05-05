# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb"
        "-mfpu=fpv5-sp-d16"
        "-mfloat-abi=softfp"
        "-mcpu=cortex-m55"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND common_options
        "-mfpu=fpv5-sp-d16"
        "-mfloat-abi=hard"
        "-mcpu=cortex-m55"
    )
endif()

# We'd like to use just "-mcpu=cortex-m55" in common_options, but due to a bug
# in armclang passing options to armasm, we use the following flags as a
# workaround to select M55.
list(APPEND asm_compile_options
    -mcpu=cortex-r7
    -Wa,--cpu=cortex-m55
)

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_M55
            __FPU_PRESENT=1
            __CMSIS_RTOS
            __MBED_CMSIS_RTOS_CM
    )
endfunction()
