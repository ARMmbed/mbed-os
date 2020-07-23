# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-a9)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND common_toolchain_options
        "-mthumb-interwork"
        "-marm"
        "-march=armv7-a"
        "-mfpu=vfpv3"
        "-mfloat-abi=hard"
        "-mno-unaligned-access"
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
        "-mcpu=cortex-a9"
    )

    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${compile_options}>
            $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
            $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-A9>
    )

    target_link_options(mbed-os
        PUBLIC
            "--cpu=Cortex-A9"
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        __CORTEX_A9
        ARM_MATH_CA9
        __FPU_PRESENT
        __CMSIS_RTOS
        __EVAL
        __MBED_CMSIS_RTOS_CA9
)
