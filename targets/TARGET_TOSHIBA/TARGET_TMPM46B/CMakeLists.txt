# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/tmpm46bf10fg.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_TMPM46B.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/tmpm46bf10fg.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_TMPM46b.S)
endif()

add_library(mbed-tmpm46b INTERFACE)

target_include_directories(mbed-tmpm46b
    INTERFACE
        .
        device
        Periph_Driver/inc
)

target_sources(mbed-tmpm46b
    INTERFACE
        analogin_api.c
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
        trng_api.c
        us_ticker.c

        device/system_TMPM46B.c

        Periph_Driver/src/tmpm46b_adc.c
        Periph_Driver/src/tmpm46b_cg.c
        Periph_Driver/src/tmpm46b_esg.c
        Periph_Driver/src/tmpm46b_fc.c
        Periph_Driver/src/tmpm46b_fuart.c
        Periph_Driver/src/tmpm46b_gpio.c
        Periph_Driver/src/tmpm46b_i2c.c
        Periph_Driver/src/tmpm46b_rtc.c
        Periph_Driver/src/tmpm46b_ssp.c
        Periph_Driver/src/tmpm46b_tmrb.c
        Periph_Driver/src/tmpm46b_uart.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-tmpm46b ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-tmpm46b INTERFACE mbed-toshiba)
