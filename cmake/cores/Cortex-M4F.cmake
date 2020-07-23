# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND common_toolchain_options
        "-mthumb"
        "-mcpu=cortex-m4"
        "-mfpu=fpv4-sp-d16"
        "-mfloat-abi=softfp"
    )

    target_compile_options(mbed-os
        PUBLIC
            ${common_toolchain_options}
    )

    target_link_options(mbed-os
        PUBLIC
            ${common_toolchain_options}
    )
elseif(MBED_TOOLCHAIN STREQUAL "ARM")
    list(APPEND compile_options
        "-mcpu=cortex-m4"
        "-mfpu=fpv4-sp-d16"
        "-mfloat-abi=hard"
    )

    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${compile_options}>
            $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
            $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-M4>
    )

    target_link_options(mbed-os
        PUBLIC
            "--cpu=Cortex-M4"
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        __CORTEX_M4
        ARM_MATH_CM4
        __FPU_PRESENT=1
        __CMSIS_RTOS
        __MBED_CMSIS_RTOS_CM
)
