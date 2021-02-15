# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/NANO130.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM/NANO130.sct)
endif()

add_library(mbed-nano100 INTERFACE)

target_sources(mbed-nano100
    INTERFACE
        device/cmsis_nvic.c
        device/startup_Nano100Series.c
        device/system_Nano100Series.c

        device/StdDriver/nano100_adc.c
        device/StdDriver/nano100_clk.c
        device/StdDriver/nano100_crc.c
        device/StdDriver/nano100_dac.c
        device/StdDriver/nano100_ebi.c
        device/StdDriver/nano100_fmc.c
        device/StdDriver/nano100_gpio.c
        device/StdDriver/nano100_i2c.c
        device/StdDriver/nano100_i2s.c
        device/StdDriver/nano100_lcd.c
        device/StdDriver/nano100_pdma.c
        device/StdDriver/nano100_pwm.c
        device/StdDriver/nano100_rtc.c
        device/StdDriver/nano100_sc.c
        device/StdDriver/nano100_scuart.c
        device/StdDriver/nano100_spi.c
        device/StdDriver/nano100_sys.c
        device/StdDriver/nano100_timer.c
        device/StdDriver/nano100_uart.c
        device/StdDriver/nano100_usbd.c
        device/StdDriver/nano100_wdt.c
        device/StdDriver/nano100_wwdt.c

        analogin_api.c
        analogout_api.c
        dma_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        PeripheralPins.c
        pinmap.c
        port_api.c
        pwmout_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
        wait_ns.c
        watchdog_api.c
)

target_include_directories(mbed-nano100
    INTERFACE
        .
        device
        device/StdDriver
)

target_link_libraries(mbed-nano100 INTERFACE mbed-nuvoton)

mbed_set_linker_script(mbed-nano100 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-numaker-pfm-nano130 INTERFACE)

target_link_libraries(mbed-numaker-pfm-nano130 INTERFACE mbed-nano100)
