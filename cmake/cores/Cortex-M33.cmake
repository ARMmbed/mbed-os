# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

list(APPEND options)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND common_toolchain_options
        "-mthumb"
        "-march=armv8-m.main"
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
        "-mcpu=cortex-m33+nodsp"
        "-mfpu=none"
    )

    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${compile_options}>
            $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
            $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-M33.no_dsp.no_fp>
    )

    target_link_options(mbed-os
        PUBLIC
            "--cpu=Cortex-M33.no_dsp.no_fp"
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        __CORTEX_M33
        ARM_MATH_ARMV8MML
        DOMAIN_NS=1
        __CMSIS_RTOS
        __MBED_CMSIS_RTOS_CM
)
