# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_EV_COG_AD4050LZ EXCLUDE_FROM_ALL)

add_library(mbed-aducm4050 INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/ADuCM4050.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE TOOLCHAIN_ARM_STD/ADuCM4050.sct)
endif()

mbed_set_linker_script(mbed-aducm4050 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-aducm4050
    INTERFACE
        api
        bsp
        bsp/adc
        bsp/beep
        bsp/config
        bsp/crc
        bsp/crypto
        bsp/drivers/adc
        bsp/drivers/beep
        bsp/drivers/crc
        bsp/drivers/crypto
        bsp/drivers/dma
        bsp/drivers/flash
        bsp/drivers/general
        bsp/drivers/gpio
        bsp/drivers/i2c
        bsp/drivers/pwr
        bsp/drivers/rng
        bsp/drivers/rtc
        bsp/drivers/spi
        bsp/drivers/sport
        bsp/drivers/tmr
        bsp/drivers/uart
        bsp/drivers/wdt
        bsp/drivers/xint
        bsp/flash
        bsp/gpio
        bsp/i2c
        bsp/pwr
        bsp/rng
        bsp/rtc
        bsp/rtos_map
        bsp/spi
        bsp/sport
        bsp/sys
        bsp/uart
        bsp/xint
)

target_sources(mbed-aducm4050
    INTERFACE
        api/analogin_api.c
        api/flash_api.c
        api/gpio_api.c
        api/gpio_dev_mem.c
        api/gpio_irq_api.c
        api/i2c_api.c
        api/PeripheralPins.c
        api/pinmap.c
        api/rtc_api.c
        api/serial_api.c
        api/sleep.c
        api/spi_api.c
        api/trng_api.c
        api/us_ticker.c

        bsp/adc/adi_adc.c
        bsp/adc/adi_adc_data.c

        bsp/beep/adi_beep.c

        bsp/crc/adi_crc.c

        bsp/crypto/adi_crypto.c

        bsp/dma/adi_dma.c

        bsp/flash/adi_flash.c
        bsp/flash/adi_flash_data.c

        bsp/gpio/adi_gpio.c

        bsp/i2c/adi_i2c.c
        bsp/i2c/adi_i2c_data.c

        bsp/pwr/adi_pwr.c

        bsp/rng/adi_rng.c

        bsp/rtc/adi_rtc_data.c
        bsp/rtc/adi_rtc.c

        bsp/spi/adi_spi.c
        bsp/spi/adi_spi_data.c

        bsp/sport/adi_sport.c

        bsp/tmr/adi_tmr.c
        bsp/tmr/adi_tmr_data.c

        bsp/uart/adi_uart.c

        bsp/wdt/adi_wdt.c

        bsp/xint/adi_xint.c
)

target_link_libraries(mbed-aducm4050 INTERFACE mbed-analog-devices)
