# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g070xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g070xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g070xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g070xb.sct)
endif()

add_library(mbed-stm32g070xb INTERFACE)

target_include_directories(mbed-stm32g070xb
    INTERFACE
        .
)

target_sources(mbed-stm32g070xb
    INTERFACE
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32g070xb ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32g070xb INTERFACE mbed-stm32g0)
