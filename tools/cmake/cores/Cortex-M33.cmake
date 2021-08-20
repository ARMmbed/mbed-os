# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets cpu core options
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    list(APPEND common_options
        "-mthumb"
        "-mcpu=cortex-m33+nodsp+nofp"
    )
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    list(APPEND common_options
        "-mcpu=cortex-m33+nodsp"
        "-mfpu=none"
    )
    list(APPEND link_options
        # Necessary as the linker does not always detect
        # the architecture from the objectfiles correctly.
        # Also, the complete flag should be "--cpu=Cortex-M33.no_dsp.no_fp"
        # but this currently conflicts with CMake's compiler test until fixed
        "--cpu=Cortex-M33.no_fp"
    )
endif()

function(mbed_set_cpu_core_definitions target)
    target_compile_definitions(${target}
        INTERFACE
            __CORTEX_M33
            ARM_MATH_ARMV8MML
            __CMSIS_RTOS
    )
endfunction()
