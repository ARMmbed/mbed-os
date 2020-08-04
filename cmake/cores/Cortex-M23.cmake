# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
function(mbed_set_cpu_core_options target mbed_toolchain)
    if(${mbed_toolchain} STREQUAL "GCC_ARM")
        list(APPEND common_toolchain_options
            "-mthumb"
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
            "-mcpu=cortex-m23"
        )

        target_compile_options(${target}
            PUBLIC
                $<$<COMPILE_LANGUAGE:C>:${compile_options}>
                $<$<COMPILE_LANGUAGE:CXX>:${compile_options}>
                $<$<COMPILE_LANGUAGE:ASM>:--cpu=Cortex-M23>
        )

        target_link_options(${target}
            PUBLIC
                "--cpu=Cortex-M23"
        )
    endif()

    target_compile_definitions(${target}
        PUBLIC
            __CORTEX_M23
            ARM_MATH_ARMV8MBL
            DOMAIN_NS=1
            __CMSIS_RTOS
            __MBED_CMSIS_RTOS_CM
    )
endfunction()
