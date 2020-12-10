# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("LPCXpresso" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_LPCXpresso
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_LPCXpresso/PeripheralPins.c
            TARGET_LPCXpresso/clock_config.c
            TARGET_LPCXpresso/mbed_overrides.c
    )
endif()

if("LPC54114_M4" IN_LIST MBED_TARGET_LABELS)
    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE device/TARGET_LPC54114_M4/TOOLCHAIN_ARM_STD/startup_LPC54114_cm4.S)
        set(LINKER_FILE device/TARGET_LPC54114_M4/TOOLCHAIN_ARM_STD/LPC54114J256_cm4.sct)
        set(LIB_POWER device/TARGET_LPC54114_M4/TOOLCHAIN_ARM_STD/libpower.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE device/TARGET_LPC54114_M4/TOOLCHAIN_GCC_ARM/startup_LPC54114_cm4.S)
        set(LINKER_FILE device/TARGET_LPC54114_M4/TOOLCHAIN_GCC_ARM/LPC54114J256_cm4_flash.ld)
        set(LIB_POWER device/TARGET_LPC54114_M4/TOOLCHAIN_GCC_ARM/libpower.a)
    endif()

    target_link_libraries(mbed-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_POWER})

    target_include_directories(mbed-core
        INTERFACE
            device/TARGET_LPC54114_M4
    )

    target_sources(mbed-core
        INTERFACE
            device/TARGET_LPC54114_M4/system_LPC54114_cm4.c
    )
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        device
        drivers
)

target_sources(mbed-core
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

        ${STARTUP_FILE}
)
