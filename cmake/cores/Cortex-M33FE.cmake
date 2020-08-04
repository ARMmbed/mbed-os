# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
function(mbed_set_cpu_core_options target mbed_toolchain)
    if(${mbed_toolchain} STREQUAL "GCC_ARM")
        list(APPEND common_toolchain_options
            "-mthumb"
            "-mfpu=fpv5-sp-d16"
            "-mfloat-abi=softfp"
            "-march=armv8-m.main+dsp"
        )

        target_compile_options(${target}
            PUBLIC
                ${common_toolchain_options}
        )

        target_link_options(${target}
            PUBLIC
                ${common_toolchain_options}
        )
    elseif(${mbed_toolchain} STREQUAL "ARM")
        list(APPEND compile_options
            "-mcpu=cortex-m33"
        )

        target_compile_options(${target}
            PUBLIC
                $<$<COMPILE_LANGUAGE:C>:${compile_options}>
                $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
                $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-M33>
        )

        target_link_options(${target}
            PUBLIC
                "--cpu=Cortex-M33"
        )
    endif()

    target_compile_definitions(${target}
        PUBLIC
            __CORTEX_M33
            ARM_MATH_ARMV8MML
            DOMAIN_NS=1
            __FPU_PRESENT=1U
            __CMSIS_RTOS
            __MBED_CMSIS_RTOS_CM
            __DSP_PRESENT=1U
    )
endfunction()
