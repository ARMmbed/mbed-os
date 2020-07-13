# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-a9)

if(MBED_TOOLCHAIN strequal "GCC_ARM")

set(GCC_FLAGS
    -mthumb-interwork
    -marm
    -march=armv7-a
    -mfpu=vfpv3
    -mfloat-abi=hard
    -mno-unaligned-access
)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${GCC_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${GCC_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${GCC_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${GCC_FLAGS}
)

elseif(MBED_TOOLCHAIN strequal "ARMC6")

set(ARM_FLAGS
    --cpu=Cortex-A9
)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${ARM_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${ARM_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${ARM_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${ARM_FLAGS}
)

endif()

add_definitions(
    __CORTEX_A9
    ARM_MATH_CA9
    __FPU_PRESENT
    __CMSIS_RTOS
    __EVAL
    __MBED_CMSIS_RTOS_CA9
)
