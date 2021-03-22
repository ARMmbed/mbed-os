# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FRDM EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MK82FN256xxx15.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MK82F25615.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MK82FN256xxx15.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MK82F25615.S)
endif()

add_library(mbed-k82f INTERFACE)

target_include_directories(mbed-k82f
    INTERFACE
        .
        device
        drivers
)

target_sources(mbed-k82f
    INTERFACE
        pwmout_api.c
        serial_api.c
        spi_api.c
        trng_api.c
        us_ticker.c

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
        drivers/fsl_ewm.c
        drivers/fsl_flash.c
        drivers/fsl_flexbus.c
        drivers/fsl_flexio.c
        drivers/fsl_flexio_camera.c
        drivers/fsl_flexio_camera_edma.c
        drivers/fsl_flexio_i2c_master.c
        drivers/fsl_flexio_i2s.c
        drivers/fsl_flexio_i2s_edma.c
        drivers/fsl_flexio_spi.c
        drivers/fsl_flexio_spi_edma.c
        drivers/fsl_flexio_uart.c
        drivers/fsl_flexio_uart_edma.c
        drivers/fsl_ftm.c
        drivers/fsl_gpio.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_edma.c
        drivers/fsl_llwu.c
        drivers/fsl_lmem_cache.c
        drivers/fsl_lptmr.c
        drivers/fsl_lpuart.c
        drivers/fsl_lpuart_edma.c
        drivers/fsl_ltc.c
        drivers/fsl_ltc_edma.c
        drivers/fsl_mpu.c
        drivers/fsl_pdb.c
        drivers/fsl_pit.c
        drivers/fsl_pmc.c
        drivers/fsl_qspi.c
        drivers/fsl_qspi_edma.c
        drivers/fsl_rcm.c
        drivers/fsl_rtc.c
        drivers/fsl_sai.c
        drivers/fsl_sai_edma.c
        drivers/fsl_sdhc.c
        drivers/fsl_sdramc.c
        drivers/fsl_sim.c
        drivers/fsl_smartcard_emvsim.c
        drivers/fsl_smartcard_phy_emvsim.c
        drivers/fsl_smartcard_phy_ncn8025.c
        drivers/fsl_smartcard_phy_tda8035.c
        drivers/fsl_smc.c
        drivers/fsl_tpm.c
        drivers/fsl_trng.c
        drivers/fsl_tsi_v4.c
        drivers/fsl_vref.c
        drivers/fsl_wdog.c

        device/system_MK82F25615.c
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-k82f ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-k82f INTERFACE mbed-freescale-mcuxpresso-mcus mbed-frdm-k82f)
