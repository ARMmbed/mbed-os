# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/TARGET_LPC11XX/startup_LPC11xx.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/TARGET_LPC11XX/LPC1114.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_LPC11xx.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/TARGET_LPC11XX/LPC1114.ld)
endif()

add_subdirectory(TARGET_LPC11XX EXCLUDE_FROM_ALL)

add_library(mbed-lpc11xx-11cxx INTERFACE)

target_include_directories(mbed-lpc11xx-11cxx
    INTERFACE
        .
        device
)

target_sources(mbed-lpc11xx-11cxx
    INTERFACE
        analogin_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c

        device/cmsis_nvic.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-lpc11xx-11cxx INTERFACE mbed-nxp)

add_library(mbed-lpc1114 INTERFACE)

mbed_set_linker_script(mbed-lpc1114 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-lpc1114 INTERFACE mbed-lpc11xx)
