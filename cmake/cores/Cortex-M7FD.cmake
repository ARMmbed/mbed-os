# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND common_toolchain_options
        "-mthumb"
        "-mfpu=fpv5-d16"
        "-mfloat-abi=softfp"
        "-mcpu=cortex-m7"
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
        "-mcpu=cortex-m7"
        "-mfpu=fpv5-d16"
        "-mfloat-abi=hard"
    )

    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${compile_options}>
            $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
            $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-M7>
    )

    target_link_options(mbed-os
        PUBLIC
            "--cpu=Cortex-M7"
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        __CORTEX_M7
        ARM_MATH_CM7
        __FPU_PRESENT=1
        __CMSIS_RTOS
        __MBED_CMSIS_RTOS_CM
)
