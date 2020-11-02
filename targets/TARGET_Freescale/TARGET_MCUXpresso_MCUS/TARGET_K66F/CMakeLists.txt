# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("FRDM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_FRDM)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MK66F18.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MK66FN2M0xxx18.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MK66F18.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MK66FN2M0xxx18.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    set(STARTUP_FILE device/TOOLCHAIN_IAR/startup_MK66F18.S)
    set(LINKER_FILE device/TOOLCHAIN_IAR/MK66FN2M0xxx18.icf)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
        drivers
        device
)

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE}
        pwmout_api.c
        serial_api.c
        spi_api.c
        trng_api.c
        us_ticker.c

        device/system_MK66F18.c

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
        drivers/fsl_lmem_cache.c
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
        drivers/fsl_sdhc.c
        drivers/fsl_sdramc.c
        drivers/fsl_sim.c
        drivers/fsl_smc.c
        drivers/fsl_sysmpu.c
        drivers/fsl_tpm.c
        drivers/fsl_tsi_v4.c
        drivers/fsl_uart.c
        drivers/fsl_uart_edma.c
        drivers/fsl_vref.c
        drivers/fsl_wdog.c
)
