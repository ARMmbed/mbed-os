# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("HANI_IOT" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_HANI_IOT
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_HANI_IOT/PeripheralPins.c
            TARGET_HANI_IOT/clock_config.c
            TARGET_HANI_IOT/mbed_overrides.c
    )
elseif("LPCXpresso" IN_LIST MBED_TARGET_LABELS)
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

if("M33_NS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M33_NS)
elseif("M33_S" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M33_S)
endif()

target_include_directories(mbed-core
    INTERFACE
        device
        drivers
        partition
)

target_sources(mbed-core
    INTERFACE
        flash_api.c

        device/system_LPC55S69_cm33_core0.c

        drivers/fsl_anactrl.c
        drivers/fsl_casper.c
        drivers/fsl_clock.c
        drivers/fsl_cmp.c
        drivers/fsl_common.c
        drivers/fsl_crc.c
        drivers/fsl_ctimer.c
        drivers/fsl_dma.c
        drivers/fsl_flexcomm.c
        drivers/fsl_gint.c
        drivers/fsl_gpio.c
        drivers/fsl_hashcrypt.c
        drivers/fsl_i2c.c
        drivers/fsl_i2c_dma.c
        drivers/fsl_i2s.c
        drivers/fsl_i2s_dma.c
        drivers/fsl_iap.c
        drivers/fsl_inputmux.c
        drivers/fsl_lpadc.c
        drivers/fsl_mrt.c
        drivers/fsl_ostimer.c
        drivers/fsl_pint.c
        drivers/fsl_plu.c
        drivers/fsl_power.c
        drivers/fsl_powerquad_basic.c
        drivers/fsl_powerquad_data.c
        drivers/fsl_powerquad_filter.c
        drivers/fsl_powerquad_math.c
        drivers/fsl_powerquad_matrix.c
        drivers/fsl_powerquad_transform.c
        drivers/fsl_prince.c
        drivers/fsl_puf.c
        drivers/fsl_reset.c
        drivers/fsl_rng.c
        drivers/fsl_rtc.c
        drivers/fsl_sctimer.c
        drivers/fsl_sdif.c
        drivers/fsl_spi.c
        drivers/fsl_spi_dma.c
        drivers/fsl_sysctl.c
        drivers/fsl_usart.c
        drivers/fsl_usart_dma.c
        drivers/fsl_utick.c
        drivers/fsl_wwdt.c
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LIB_POWER device/TOOLCHAIN_ARMC6/lib_power.ar)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LIB_POWER device/TOOLCHAIN_GCC_ARM/libpower.a)
endif()
target_link_libraries(mbed-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_POWER})
