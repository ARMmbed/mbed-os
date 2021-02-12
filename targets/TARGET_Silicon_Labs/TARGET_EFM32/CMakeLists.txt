# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_EFM32GG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_EFM32GG11 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_EFR32MG12 EXCLUDE_FROM_ALL)

add_library(mbed-efm32 INTERFACE)

target_include_directories(mbed-efm32
    INTERFACE
        .
        common
        emlib/inc
        trng
)

target_sources(mbed-efm32
    INTERFACE
        analogin_api.c
        analogout_api.c
        burtc.c
        can_api.c
        crc_api.c
        dma_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        itm_api.c
        lp_ticker.c
        pinmap.c
        pinmap_function.c
        port_api.c
        pwmout_api.c
        qspi_api.c
        resetreason_api.c
        rtcc.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
        watchdog_api.c

        common/mbed_overrides.c

        emlib/src/em_acmp.c
        emlib/src/em_adc.c
        emlib/src/em_aes.c
        emlib/src/em_assert.c
        emlib/src/em_burtc.c
        emlib/src/em_can.c
        emlib/src/em_cmu.c
        emlib/src/em_core.c
        emlib/src/em_cryotimer.c
        emlib/src/em_crypto.c
        emlib/src/em_csen.c
        emlib/src/em_dac.c
        emlib/src/em_dbg.c
        emlib/src/em_dma.c
        emlib/src/em_ebi.c
        emlib/src/em_emu.c
        emlib/src/em_eusart.c
        emlib/src/em_gpcrc.c
        emlib/src/em_gpio.c
        emlib/src/em_i2c.c
        emlib/src/em_iadc.c
        emlib/src/em_idac.c
        emlib/src/em_int.c
        emlib/src/em_lcd.c
        emlib/src/em_ldma.c
        emlib/src/em_lesense.c
        emlib/src/em_letimer.c
        emlib/src/em_leuart.c
        emlib/src/em_mpu.c
        emlib/src/em_msc.c
        emlib/src/em_opamp.c
        emlib/src/em_pcnt.c
        emlib/src/em_pdm.c
        emlib/src/em_prs.c
        emlib/src/em_qspi.c
        emlib/src/em_rmu.c
        emlib/src/em_rtc.c
        emlib/src/em_rtcc.c
        emlib/src/em_se.c
        emlib/src/em_system.c
        emlib/src/em_timer.c
        emlib/src/em_usart.c
        emlib/src/em_vcmp.c
        emlib/src/em_vdac.c
        emlib/src/em_wdog.c

        trng/sl_trng.c
        trng/trng_api.c
)

target_link_libraries(mbed-efm32 INTERFACE mbed-silicon-labs)
