# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/drivers/include
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/drivers/src
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/drivers/src/prs
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/hal
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/mdk
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/soc
        ${CMAKE_CURRENT_SOURCE_DIR}/softdevice
        ${CMAKE_CURRENT_SOURCE_DIR}/softdevice/common
)

target_sources(mbed-os
    PRIVATE
        nrfx/drivers/src/nrfx_adc.c
        nrfx/drivers/src/nrfx_clock.c
        nrfx/drivers/src/nrfx_comp.c
        nrfx/drivers/src/nrfx_gpiote.c
        nrfx/drivers/src/nrfx_i2s.c
        nrfx/drivers/src/nrfx_lpcomp.c
        nrfx/drivers/src/nrfx_pdm.c
        nrfx/drivers/src/nrfx_power.c
        nrfx/drivers/src/nrfx_power_clock.c
        nrfx/drivers/src/nrfx_ppi.c
        nrfx/drivers/src/nrfx_pwm.c
        nrfx/drivers/src/nrfx_qdec.c
        nrfx/drivers/src/nrfx_qspi.c
        nrfx/drivers/src/nrfx_rng.c
        nrfx/drivers/src/nrfx_rtc.c
        nrfx/drivers/src/nrfx_saadc.c
        nrfx/drivers/src/nrfx_spi.c
        nrfx/drivers/src/nrfx_spim.c
        nrfx/drivers/src/nrfx_spis.c
        nrfx/drivers/src/nrfx_swi.c
        nrfx/drivers/src/nrfx_systick.c
        nrfx/drivers/src/nrfx_timer.c
        nrfx/drivers/src/nrfx_twi.c
        nrfx/drivers/src/nrfx_twim.c
        nrfx/drivers/src/nrfx_twis.c
        nrfx/drivers/src/nrfx_uart.c
        nrfx/drivers/src/nrfx_uarte.c
        nrfx/drivers/src/nrfx_wdt.c

        nrfx/drivers/src/prs/nrfx_prs.c

        nrfx/hal/nrf_ecb.c
        nrfx/hal/nrf_nvmc.c

        softdevice/common/nrf_sdh.c
        softdevice/common/nrf_sdh_ant.c
        softdevice/common/nrf_sdh_ble.c
        softdevice/common/nrf_sdh_soc.c
)
