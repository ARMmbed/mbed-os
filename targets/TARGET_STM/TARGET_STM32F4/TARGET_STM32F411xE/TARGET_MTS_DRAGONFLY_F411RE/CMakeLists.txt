# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f411xe.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F411XE.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f411xe.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f411re.sct)
endif()

add_library(mbed-mts-dragonfly-f411re INTERFACE)

target_sources(mbed-mts-dragonfly-f411re
    INTERFACE
        system_clock.c
        ONBOARD_TELIT_HE910.cpp
        PeripheralPins.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-mts-dragonfly-f411re
    INTERFACE
        .
)

mbed_set_linker_script(mbed-mts-dragonfly-f411re ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-mts-dragonfly-f411re INTERFACE mbed-stm32f4)
