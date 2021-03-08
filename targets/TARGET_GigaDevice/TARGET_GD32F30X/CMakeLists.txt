# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-gd32-f307vg INTERFACE)

target_include_directories(mbed-gd32-f307vg
    INTERFACE
        TARGET_GD32F307VG
        TARGET_GD32F307VG/device
)

target_sources(mbed-gd32-f307vg
    INTERFACE
        TARGET_GD32F307VG/PeripheralPins.c

        TARGET_GD32F307VG/device/system_gd32f30x.c
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE TARGET_GD32F307VG/device/TOOLCHAIN_ARM_STD/gd32f307vg.sct)
    set(STARTUP_FILE TARGET_GD32F307VG/device/TOOLCHAIN_ARM_STD/startup_gd32f30x_cl.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE TARGET_GD32F307VG/device/TOOLCHAIN_GCC_ARM/GD32F307xG.ld)
    set(STARTUP_FILE TARGET_GD32F307VG/device/TOOLCHAIN_GCC_ARM/startup_gd32f30x_cl.S)
endif()

add_library(mbed-gd32f30x INTERFACE)

target_include_directories(mbed-gd32f30x
    INTERFACE
        .
        GD32F30x_standard_peripheral/Include
)

target_sources(mbed-gd32f30x
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
        us_ticker.c

        GD32F30x_standard_peripheral/Source/gd32f30x_adc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_bkp.c
        GD32F30x_standard_peripheral/Source/gd32f30x_can.c
        GD32F30x_standard_peripheral/Source/gd32f30x_ctc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_crc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_dac.c
        GD32F30x_standard_peripheral/Source/gd32f30x_dbg.c
        GD32F30x_standard_peripheral/Source/gd32f30x_dma.c
        GD32F30x_standard_peripheral/Source/gd32f30x_enet.c
        GD32F30x_standard_peripheral/Source/gd32f30x_exmc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_exti.c
        GD32F30x_standard_peripheral/Source/gd32f30x_fmc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_fwdgt.c
        GD32F30x_standard_peripheral/Source/gd32f30x_gpio.c
        GD32F30x_standard_peripheral/Source/gd32f30x_i2c.c
        GD32F30x_standard_peripheral/Source/gd32f30x_misc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_pmu.c
        GD32F30x_standard_peripheral/Source/gd32f30x_rcu.c
        GD32F30x_standard_peripheral/Source/gd32f30x_rtc.c
        GD32F30x_standard_peripheral/Source/gd32f30x_sdio.c
        GD32F30x_standard_peripheral/Source/gd32f30x_spi.c
        GD32F30x_standard_peripheral/Source/gd32f30x_timer.c
        GD32F30x_standard_peripheral/Source/gd32f30x_usart.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-gd32f30x ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-gd32f30x INTERFACE mbed-gigadevice)
target_link_libraries(mbed-gd32-f307vg INTERFACE mbed-gd32f30x)
