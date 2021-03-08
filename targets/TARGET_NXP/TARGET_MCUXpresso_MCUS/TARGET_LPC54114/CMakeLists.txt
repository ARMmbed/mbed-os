# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(device/TARGET_LPC54114_M4 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPCXpresso EXCLUDE_FROM_ALL)

add_library(mbed-lpc54114 INTERFACE)

target_include_directories(mbed-lpc54114
    INTERFACE
        device
        drivers
)

target_sources(mbed-lpc54114
    INTERFACE
        flash_api.c

        drivers/fsl_adc.c
        drivers/fsl_clock.c
        drivers/fsl_common.c
        drivers/fsl_crc.c
        drivers/fsl_ctimer.c
        drivers/fsl_dma.c
        drivers/fsl_dmic.c
        drivers/fsl_dmic_dma.c
        drivers/fsl_flashiap.c
        drivers/fsl_flexcomm.c
        drivers/fsl_fmeas.c
        drivers/fsl_gint.c
        drivers/fsl_gpio.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_dma.c
        drivers/fsl_i2s.c
        drivers/fsl_i2s_dma.c
        drivers/fsl_inputmux.c
        drivers/fsl_mrt.c
        drivers/fsl_pint.c
        drivers/fsl_power.c
        drivers/fsl_reset.c
        drivers/fsl_rtc.c
        drivers/fsl_sctimer.c
        drivers/fsl_spi.c
        drivers/fsl_spi_dma.c
        drivers/fsl_usart.c
        drivers/fsl_usart_dma.c
        drivers/fsl_utick.c
        drivers/fsl_wwdt.c
)

target_link_libraries(mbed-lpc54114
    INTERFACE
        mbed-mcuxpresso-mcus
        mbed-lpc5114-xpresso
        mbed-lpc
        mbed-lpc54114-m4
)
