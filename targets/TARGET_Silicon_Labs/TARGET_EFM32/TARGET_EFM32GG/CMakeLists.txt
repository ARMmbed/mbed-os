# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-1024k ${CMAKE_LIBRARY_TYPE})

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TARGET_1024K/TOOLCHAIN_ARM_STD/efm32gg.sct)
    set(STARTUP_FILE device/TARGET_1024K/TOOLCHAIN_ARM_STD/startup_efm32gg.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TARGET_1024K/TOOLCHAIN_GCC_ARM/efm32gg.ld)
    set(STARTUP_FILE device/TARGET_1024K/TOOLCHAIN_GCC_ARM/startup_efm32gg.S)
endif()

target_include_directories(mbed-1024k
    ${CMAKE_LIBRARY_TYPE}
        device
)

target_sources(mbed-1024k
    ${CMAKE_LIBRARY_TYPE}
        PeripheralPins.c

        device/system_efm32gg.c
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-1024k ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-efm32gg990f1024 ${CMAKE_LIBRARY_TYPE})

target_link_libraries(mbed-efm32gg990f1024 ${CMAKE_LIBRARY_TYPE} mbed-efm32 mbed-1024k)

add_library(mbed-efm32gg-stk3700 ${CMAKE_LIBRARY_TYPE})

target_include_directories(mbed-efm32gg-stk3700
    ${CMAKE_LIBRARY_TYPE}
        TARGET_EFM32GG_STK3700
)

target_link_libraries(mbed-efm32gg-stk3700 ${CMAKE_LIBRARY_TYPE} mbed-efm32gg990f1024)
