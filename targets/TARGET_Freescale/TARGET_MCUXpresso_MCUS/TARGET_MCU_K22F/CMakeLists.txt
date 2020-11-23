# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MCU_K22F512" IN_LIST MBED_TARGET_LABELS)
    if("FRDM" IN_LIST MBED_TARGET_LABELS)
        target_include_directories(mbed-core
            INTERFACE
                TARGET_MCU_K22F512/TARGET_FRDM
        )

        target_sources(mbed-core
            INTERFACE
                TARGET_MCU_K22F512/TARGET_FRDM/PeripheralPins.c
                TARGET_MCU_K22F512/TARGET_FRDM/fsl_clock_config.c
                TARGET_MCU_K22F512/TARGET_FRDM/mbed_overrides.c
        )
    endif()

    target_include_directories(mbed-core
        INTERFACE
            TARGET_MCU_K22F512/device
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_MCU_K22F512/device/system_MK22F51212.c
    )

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE TARGET_MCU_K22F512/device/TOOLCHAIN_ARM_STD/MK22FN512xxx12.sct)
        set(STARTUP_FILE TARGET_MCU_K22F512/device/TOOLCHAIN_ARM_STD/startup_MK22F51212.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE TARGET_MCU_K22F512/device/TOOLCHAIN_GCC_ARM/MK22FN512xxx12.ld)
        set(STARTUP_FILE TARGET_MCU_K22F512/device/TOOLCHAIN_GCC_ARM/startup_MK22F51212.S)
    endif()
endif()

set_property(
    GLOBAL PROPERTY
    MBED_TARGET_LINKER_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
        drivers
)

target_sources(mbed-core
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

        ${STARTUP_FILE}
)
