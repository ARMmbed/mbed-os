# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FRDM EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_HEXIWEAR EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SDT64B EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MK64FN1M0xxx12.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MK64F12.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MK64FN1M0xxx12.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MK64F12.S)
endif()

add_library(mbed-mcu-k64f INTERFACE)

target_include_directories(mbed-mcu-k64f
    INTERFACE
        .
        device
        drivers
)

target_sources(mbed-mcu-k64f
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

        device/system_MK64F12.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-mcu-k64f INTERFACE mbed-freescale-mcuxpresso-mcus)

mbed_set_linker_script(mbed-mcu-k64f ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-k64f INTERFACE)

target_link_libraries(mbed-k64f INTERFACE mbed-mcu-k64f mbed-frdm-k64f)
