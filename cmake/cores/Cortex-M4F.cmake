# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")

set(GCC_FLAGS " \
-mthumb \
-mcpu=cortex-m4 \
-mfpu=fpv4-sp-d16 \
-mfloat-abi=softfp \
")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    ${GCC_FLAGS} \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    ${GCC_FLAGS} \
")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
    ${GCC_FLAGS} \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
    ${GCC_FLAGS} \
")

elseif(MBED_TOOLCHAIN STREQUAL "ARMC6")

set(ARM_FLAGS " \
    --cpu=Cortex-M4.fp.sp \
")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    ${ARM_FLAGS} \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    ${ARM_FLAGS} \
")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
    ${ARM_FLAGS} \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
    ${ARM_FLAGS} \
")

endif()

add_definitions(
    -D__CORTEX_M4
    -DARM_MATH_CM4
    -D__FPU_PRESENT=1
    -D__CMSIS_RTOS
    -D__MBED_CMSIS_RTOS_CM
)
