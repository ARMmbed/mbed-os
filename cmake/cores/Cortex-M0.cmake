# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-m0)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND common_toolchain_options
        "-mthumb"
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
    list(APPEND options
        "-mcpu=cortex-m0"
    )

    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${compile_options}>
            $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
            $<$<COMPILE_LANGUAGE:ASM>:-cpu=Cortex-M0>
    )

    target_link_options(mbed-os
        PUBLIC
            "-cpu=Cortex-M0"
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        __CORTEX_M0
        ARM_MATH_CM0
        __CMSIS_RTOS
        __MBED_CMSIS_RTOS_CM
)
