# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FRDM EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MKL43Z256xxx4.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MKL43Z4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MKL43Z256xxx4.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MKL43Z4.S)
endif()

add_library(mbed-kl43z INTERFACE)

target_include_directories(mbed-kl43z
    INTERFACE
        .
        device
        drivers
)

target_sources(mbed-kl43z
    INTERFACE
        pwmout_api.c
        serial_api.c
        spi_api.c
        us_ticker.c

        drivers/fsl_adc16.c
        drivers/fsl_clock.c
        drivers/fsl_cmp.c
        drivers/fsl_cop.c
        drivers/fsl_dac.c
        drivers/fsl_dma.c
        drivers/fsl_dmamux.c
        drivers/fsl_flash.c
        drivers/fsl_flexio.c
        drivers/fsl_flexio_i2c_master.c
        drivers/fsl_flexio_i2s.c
        drivers/fsl_flexio_i2s_dma.c
        drivers/fsl_flexio_spi.c
        drivers/fsl_flexio_spi_dma.c
        drivers/fsl_flexio_uart.c
        drivers/fsl_flexio_uart_dma.c
        drivers/fsl_gpio.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_dma.c
        drivers/fsl_llwu.c
        drivers/fsl_lptmr.c
        drivers/fsl_lpuart.c
        drivers/fsl_lpuart_dma.c
        drivers/fsl_pit.c
        drivers/fsl_pmc.c
        drivers/fsl_rcm.c
        drivers/fsl_rtc.c
        drivers/fsl_sai.c
        drivers/fsl_sai_dma.c
        drivers/fsl_sim.c
        drivers/fsl_slcd.c
        drivers/fsl_smc.c
        drivers/fsl_spi.c
        drivers/fsl_spi_dma.c
        drivers/fsl_tpm.c
        drivers/fsl_uart.c
        drivers/fsl_uart_dma.c
        drivers/fsl_vref.c

        device/system_MKL43Z4.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-kl43z INTERFACE mbed-freescale-mcuxpresso-mcus)

mbed_set_linker_script(mbed-kl43z ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-kl43z INTERFACE mbed-frdm-kl43z)
