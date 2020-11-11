# SPDX-License-Identifier: Apache-2.0

add_subdirectory(device)

if("FRDM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_FRDM)
elseif("HEXIWEAR" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_HEXIWEAR)
elseif("SDT64B" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_SDT64B)
endif()

target_sources(mbed-core
    INTERFACE
        mbed_crc_api.c
        pwmout_api.c
        reset_reason.c
        serial_api.c
        spi_api.c
        trng_api.c
        us_ticker.c
        watchdog_api.c

        drivers/fsl_adc16.c
        drivers/fsl_clock.c
        drivers/fsl_cmp.c
        drivers/fsl_cmt.c
        drivers/fsl_crc.c
        drivers/fsl_dac.c
        drivers/fsl_dmamux.c
        drivers/fsl_dspi.c
        drivers/fsl_dspi_edma.c
        drivers/fsl_edma.c
        drivers/fsl_enet.c
        drivers/fsl_ewm.c
        drivers/fsl_flash.c
        drivers/fsl_flexbus.c
        drivers/fsl_flexcan.c
        drivers/fsl_ftm.c
        drivers/fsl_gpio.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_edma.c
        drivers/fsl_llwu.c
        drivers/fsl_lptmr.c
        drivers/fsl_pdb.c
        drivers/fsl_pit.c
        drivers/fsl_pmc.c
        drivers/fsl_rcm.c
        drivers/fsl_rnga.c
        drivers/fsl_rtc.c
        drivers/fsl_sai.c
        drivers/fsl_sai_edma.c
        drivers/fsl_sdhc.c
        drivers/fsl_sim.c
        drivers/fsl_smc.c
        drivers/fsl_sysmpu.c
        drivers/fsl_uart.c
        drivers/fsl_uart_edma.c
        drivers/fsl_vref.c
        drivers/fsl_wdog.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        drivers
)
