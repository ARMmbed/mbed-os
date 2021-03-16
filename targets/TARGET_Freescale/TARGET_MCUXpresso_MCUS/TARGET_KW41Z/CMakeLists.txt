# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FRDM EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MKW41Z512xxx4.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MKW41Z4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MKW41Z512xxx4.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MKW41Z4.S)
endif()

add_library(mbed-kw41z INTERFACE)

target_include_directories(mbed-kw41z
    INTERFACE
        .
        device
        drivers
)

target_sources(mbed-kw41z
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
        drivers/fsl_cop.c
        drivers/fsl_dac.c
        drivers/fsl_dcdc.c
        drivers/fsl_dmamux.c
        drivers/fsl_dspi.c
        drivers/fsl_dspi_edma.c
        drivers/fsl_edma.c
        drivers/fsl_flash.c
        drivers/fsl_gpio.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_edma.c
        drivers/fsl_llwu.c
        drivers/fsl_lptmr.c
        drivers/fsl_lpuart.c
        drivers/fsl_lpuart_edma.c
        drivers/fsl_ltc.c
        drivers/fsl_ltc_edma.c
        drivers/fsl_pit.c
        drivers/fsl_pmc.c
        drivers/fsl_rcm.c
        drivers/fsl_rtc.c
        drivers/fsl_sim.c
        drivers/fsl_smc.c
        drivers/fsl_tpm.c
        drivers/fsl_trng.c
        drivers/fsl_tsi_v4.c
        drivers/fsl_vref.c

        device/system_MKW41Z4.c
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-kw41z ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-kw41z INTERFACE mbed-freescale-mcuxpresso-mcus mbed-framework-5-3-3 mbed-frdm-kw41z)
