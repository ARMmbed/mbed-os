# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MCU_K22F512 EXCLUDE_FROM_ALL)

add_library(mbed-mcu-k22f INTERFACE)

target_include_directories(mbed-mcu-k22f
    INTERFACE
        .
        drivers
)

target_sources(mbed-mcu-k22f
    INTERFACE
        pwmout_api.c
        serial_api.c
        spi_api.c
        trng_api.c
        us_ticker.c

        drivers/fsl_adc16.c
        drivers/fsl_clock.c
        drivers/fsl_cmp.c
        drivers/fsl_crc.c
        drivers/fsl_dac.c
        drivers/fsl_dmamux.c
        drivers/fsl_dspi.c
        drivers/fsl_dspi_edma.c
        drivers/fsl_edma.c
        drivers/fsl_ewm.c
        drivers/fsl_flash.c
        drivers/fsl_flexbus.c
        drivers/fsl_ftm.c
        drivers/fsl_gpio.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_edma.c
        drivers/fsl_llwu.c
        drivers/fsl_lptmr.c
        drivers/fsl_lpuart.c
        drivers/fsl_lpuart_edma.c
        drivers/fsl_pdb.c
        drivers/fsl_pit.c
        drivers/fsl_pmc.c
        drivers/fsl_rcm.c
        drivers/fsl_rnga.c
        drivers/fsl_rtc.c
        drivers/fsl_sai.c
        drivers/fsl_sai_edma.c
        drivers/fsl_sim.c
        drivers/fsl_smc.c
        drivers/fsl_uart.c
        drivers/fsl_uart_edma.c
        drivers/fsl_vref.c
        drivers/fsl_wdog.c
)

target_link_libraries(mbed-mcu-k22f INTERFACE mbed-freescale-mcuxpresso-mcus)
