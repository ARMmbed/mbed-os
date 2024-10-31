# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-efm32gg11 ${CMAKE_LIBRARY_TYPE})

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/efm32gg11.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_efm32gg11.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/efm32gg11.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_efm32gg11.S)
endif()

target_include_directories(mbed-efm32gg11
    ${CMAKE_LIBRARY_TYPE}
        device
)

target_sources(mbed-efm32gg11
    ${CMAKE_LIBRARY_TYPE}
        PeripheralPins.c

        device/system_efm32gg11b.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-efm32gg11 ${CMAKE_LIBRARY_TYPE} mbed-efm32)

mbed_set_linker_script(mbed-efm32gg11 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-efm32gg11-stk3701 ${CMAKE_LIBRARY_TYPE})

target_include_directories(mbed-efm32gg11-stk3701
    ${CMAKE_LIBRARY_TYPE}
        TARGET_EFM32GG11_STK3701
)

target_link_libraries(mbed-efm32gg11-stk3701 ${CMAKE_LIBRARY_TYPE} mbed-efm32gg11)
