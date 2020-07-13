# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_TOOLCHAIN strequal "GCC_ARM")

set(GCC_FLAGS
    -mthumb
    -mfpu=fpv5-d16
    -mfloat-abi=softfp
    -mcpu=cortex-m7
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
    --cpu=Cortex-M7.fp.dp
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
    __CORTEX_M7
    ARM_MATH_CM7
    __FPU_PRESENT=1
    __CMSIS_RTOS
    __MBED_CMSIS_RTOS_CM
)
