# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/tmpm4g9f15.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_TMPM4G9.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/tmpm4g9f15fg.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_TMPM4G9.S)
endif()

add_library(mbed-tmpm4g9 INTERFACE)

target_include_directories(mbed-tmpm4g9
    INTERFACE
        .
        device
        Periph_Driver/inc
)

target_sources(mbed-tmpm4g9
    INTERFACE
        analogin_api.c
        analogout_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        reset_reason_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c

        device/system_TMPM4G9.c

        Periph_Driver/src/txz_adc.c
        Periph_Driver/src/txz_adc_ch.c
        Periph_Driver/src/txz_cg.c
        Periph_Driver/src/txz_fc.c
        Periph_Driver/src/txz_fuart.c
        Periph_Driver/src/txz_fuart_ex.c
        Periph_Driver/src/txz_gpio.c
        Periph_Driver/src/txz_hal.c
        Periph_Driver/src/txz_i2c.c
        Periph_Driver/src/txz_i2c_api.c
        Periph_Driver/src/txz_t32a.c
        Periph_Driver/src/txz_tspi.c
        Periph_Driver/src/txz_uart.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-tmpm4g9 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-tmpm4g9 INTERFACE mbed-toshiba)
