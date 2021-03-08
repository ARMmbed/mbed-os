# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M453.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM/M453.sct)
endif()

add_library(mbed-m451 INTERFACE)

target_sources(mbed-m451
    INTERFACE
        analogin_api.c
        analogout_api.c
        can_api.c

        device/startup_M451Series.c
        device/system_M451Series.c
        device/StdDriver/m451_acmp.c
        device/StdDriver/m451_can.c
        device/StdDriver/m451_clk.c
        device/StdDriver/m451_crc.c
        device/StdDriver/m451_dac.c
        device/StdDriver/m451_eadc.c
        device/StdDriver/m451_ebi.c
        device/StdDriver/m451_fmc.c
        device/StdDriver/m451_gpio.c
        device/StdDriver/m451_i2c.c
        device/StdDriver/m451_pdma.c
        device/StdDriver/m451_pwm.c
        device/StdDriver/m451_rtc.c
        device/StdDriver/m451_sc.c
        device/StdDriver/m451_scuart.c
        device/StdDriver/m451_spi.c
        device/StdDriver/m451_sys.c
        device/StdDriver/m451_timer.c
        device/StdDriver/m451_tk.c
        device/StdDriver/m451_uart.c
        device/StdDriver/m451_usbd.c
        device/StdDriver/m451_wdt.c
        device/StdDriver/m451_wwdt.c

        dma_api.c
        flash_api.c
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
        watchdog_api.c
)

target_include_directories(mbed-m451
    INTERFACE
        .
        device
        device/StdDriver
)

target_link_libraries(mbed-m451 INTERFACE mbed-nuvoton)

mbed_set_linker_script(mbed-m451 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})


add_library(mbed-numaker-pfm-m453 INTERFACE)

target_link_libraries(mbed-numaker-pfm-m453 INTERFACE mbed-m451 mbed-flash-cmsis-algo)
