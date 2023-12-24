# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-apm32f407ig INTERFACE)

target_include_directories(mbed-apm32-f407ig
    INTERFACE
        .
        device
        APM32F4xx_StdPeriphDriver/inc
        TARGET_APM32F407IG
)

target_sources(mbed-apm32-f407ig
    INTERFACE
        TARGET_APM32F407IG/PeripheralPins.c
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/apm32f407xg.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_apm32f40x.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/APM32F407xG_FLASH.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_apm32f40x.S)
endif()

add_library(mbed-apm32f4xx INTERFACE)

target_sources(mbed-apm32f4xx
    INTERFACE
        analogin_api.c
        analogout_api.c
        can_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        mbed_crc_api.c
        mbed_overrides.c
        pinmap.c
        port_api.c
        pwmout_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        trng_api.c
        us_ticker.c
        USBPhy_APM32.cpp
        watchdog_api.c

        device/system_apm32f4xx.c

        APM32F4xx_StdPeriphDriver/src/apm32f4xx_adc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_can.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_comp.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_crc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_cryp.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_cryp_aes.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_cryp_des.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_cryp_tdes.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_dac.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_dbgmcu.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_dci.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_dma.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_dmc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_eint.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_eth.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_fmc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_gpio.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_hash.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_hash_md5.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_hash_sha1.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_i2c.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_iwdt.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_misc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_pmu.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_qspi.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_rcm.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_rng.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_rtc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_sdio.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_smc.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_spi.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_syscfg.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_tmr.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_usart.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_usb.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_usb_device.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_usb_host.c
        APM32F4xx_StdPeriphDriver/src/apm32f4xx_wwdt.c

        ${STARTUP_FILE}
)

target_link_libraries(mbed-apm32f4xx INTERFACE mbed-geehy)
target_link_libraries(mbed-apm32-f407ig INTERFACE mbed-apm32f4xx)
