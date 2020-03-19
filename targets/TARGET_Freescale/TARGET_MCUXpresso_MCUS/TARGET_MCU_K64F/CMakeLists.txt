# SPDX-License-Identifier: Apache-2.0

add_subdirectory(device)

mbed_add_cmake_directory_if_labels("")

target_sources(mbed-os PRIVATE
    dma_reqs.h
    mbed_crc_api.c
    peripheral_clock_defines.h
    pwmout_api.c
    reset_reason.c
    serial_api.c
    spi_api.c
    trng_api.c
    us_ticker.c
    us_ticker_defines.h
    watchdog_api.c

    drivers/fsl_adc16.c
    drivers/fsl_adc16.h
    drivers/fsl_clock.c
    drivers/fsl_clock.h
    drivers/fsl_cmp.c
    drivers/fsl_cmp.h
    drivers/fsl_cmt.c
    drivers/fsl_cmt.h
    drivers/fsl_common.h
    drivers/fsl_crc.c
    drivers/fsl_crc.h
    drivers/fsl_dac.c
    drivers/fsl_dac.h
    drivers/fsl_dmamux.c
    drivers/fsl_dmamux.h
    drivers/fsl_dspi.c
    drivers/fsl_dspi.h
    drivers/fsl_dspi_edma.c
    drivers/fsl_dspi_edma.h
    drivers/fsl_edma.c
    drivers/fsl_edma.h
    drivers/fsl_enet.c
    drivers/fsl_enet.h
    drivers/fsl_ewm.c
    drivers/fsl_ewm.h
    drivers/fsl_flash.c
    drivers/fsl_flash.h
    drivers/fsl_flexbus.c
    drivers/fsl_flexbus.h
    drivers/fsl_flexcan.c
    drivers/fsl_flexcan.h
    drivers/fsl_ftm.c
    drivers/fsl_ftm.h
    drivers/fsl_gpio.c
    drivers/fsl_gpio.h
    drivers/fsl_i2c.c
    drivers/fsl_i2c.h
    drivers/fsl_i2c_edma.c
    drivers/fsl_i2c_edma.h
    drivers/fsl_llwu.c
    drivers/fsl_llwu.h
    drivers/fsl_lptmr.c
    drivers/fsl_lptmr.h
    drivers/fsl_pdb.c
    drivers/fsl_pdb.h
    drivers/fsl_pit.c
    drivers/fsl_pit.h
    drivers/fsl_pmc.c
    drivers/fsl_pmc.h
    drivers/fsl_port.h
    drivers/fsl_rcm.c
    drivers/fsl_rcm.h
    drivers/fsl_rnga.c
    drivers/fsl_rnga.h
    drivers/fsl_rtc.c
    drivers/fsl_rtc.h
    drivers/fsl_sai.c
    drivers/fsl_sai.h
    drivers/fsl_sai_edma.c
    drivers/fsl_sai_edma.h
    drivers/fsl_sdhc.c
    drivers/fsl_sdhc.h
    drivers/fsl_sim.c
    drivers/fsl_sim.h
    drivers/fsl_smc.c
    drivers/fsl_smc.h
    drivers/fsl_sysmpu.c
    drivers/fsl_sysmpu.h
    drivers/fsl_uart.c
    drivers/fsl_uart.h
    drivers/fsl_uart_edma.c
    drivers/fsl_uart_edma.h
    drivers/fsl_vref.c
    drivers/fsl_vref.h
    drivers/fsl_wdog.c
    drivers/fsl_wdog.h
)

target_include_directories(mbed-os PUBLIC drivers)
target_include_directories(mbed-os PUBLIC .)



