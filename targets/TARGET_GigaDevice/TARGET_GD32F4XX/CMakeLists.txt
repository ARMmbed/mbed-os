# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-gd32-f450zi INTERFACE)

target_include_directories(mbed-gd32-f450zi
    INTERFACE
        TARGET_GD32F450ZI
)

target_sources(mbed-gd32-f450zi
    INTERFACE
        TARGET_GD32F450ZI/PeripheralPins.c
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/gd32f450zi.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_gd32f450.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/GD32F450xI.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_gd32f450.S)
endif()

add_library(mbed-gd32f4xx INTERFACE)

target_include_directories(mbed-gd32f4xx
    INTERFACE
        .
        device
        GD32F4xx_standard_peripheral/Include
)

target_sources(mbed-gd32f4xx
    INTERFACE
        analogin_api.c
        analogout_api.c
        can_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        mbed_overrides.c
        pinmap.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        trng_api.c
        us_ticker.c

        device/system_gd32f4xx.c

        GD32F4xx_standard_peripheral/Source/gd32f4xx_adc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_can.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_crc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_ctc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_dac.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_dbg.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_dci.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_dma.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_enet.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_exmc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_exti.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_fmc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_fwdgt.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_gpio.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_i2c.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_ipa.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_iref.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_misc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_pmu.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_rcu.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_rtc.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_sdio.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_spi.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_syscfg.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_timer.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_tli.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_trng.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_usart.c
        GD32F4xx_standard_peripheral/Source/gd32f4xx_wwdgt.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-gd32f4xx ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-gd32f4xx INTERFACE mbed-gigadevice)
target_link_libraries(mbed-gd32-f450zi INTERFACE mbed-gd32f4xx)
