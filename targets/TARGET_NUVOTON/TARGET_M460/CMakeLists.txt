# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M467.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/M467.sct)
endif()

add_library(mbed-m460 INTERFACE)

target_sources(mbed-m460
    INTERFACE
        analogin_api.c
        analogout_api.c

        crypto/crypto-misc.cpp

        device/startup_M460.c
        device/system_M460.c
        device/StdDriver/src/m460_acmp.c
        device/StdDriver/src/m460_bmc.c
        device/StdDriver/src/m460_bpwm.c
        device/StdDriver/src/m460_canfd.c
        device/StdDriver/src/m460_ccap.c
        device/StdDriver/src/m460_clk.c
        device/StdDriver/src/m460_crc.c
        device/StdDriver/src/m460_crypto.c
        device/StdDriver/src/m460_dac.c
        device/StdDriver/src/m460_eadc.c
        device/StdDriver/src/m460_ebi.c
        device/StdDriver/src/m460_ecap.c
        device/StdDriver/src/m460_epwm.c
        device/StdDriver/src/m460_eqei.c
        device/StdDriver/src/m460_fmc.c
        device/StdDriver/src/m460_gpio.c
        device/StdDriver/src/m460_hbi.c
        device/StdDriver/src/m460_hsusbd.c
        device/StdDriver/src/m460_i2c.c
        device/StdDriver/src/m460_i2s.c
        device/StdDriver/src/m460_keystore.c
        device/StdDriver/src/m460_kpi.c
        device/StdDriver/src/m460_pdma.c
        device/StdDriver/src/m460_qspi.c
        device/StdDriver/src/m460_rng.c
        device/StdDriver/src/m460_rtc.c
        device/StdDriver/src/m460_sc.c
        device/StdDriver/src/m460_scuart.c
        device/StdDriver/src/m460_sdh.c
        device/StdDriver/src/m460_spi.c
        device/StdDriver/src/m460_spim.c
        device/StdDriver/src/m460_sys.c
        device/StdDriver/src/m460_timer.c
        device/StdDriver/src/m460_timer_pwm.c
        device/StdDriver/src/m460_trng.c
        device/StdDriver/src/m460_uart.c
        device/StdDriver/src/m460_usbd.c
        device/StdDriver/src/m460_usci_i2c.c
        device/StdDriver/src/m460_usci_spi.c
        device/StdDriver/src/m460_usci_uart.c
        device/StdDriver/src/m460_wdt.c
        device/StdDriver/src/m460_wwdt.c

        dma_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        PeripheralPins.c
        pinmap.c
        port_api.c
        pwmout_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        trng_api.cpp
        us_ticker.c
        watchdog_api.c
)

target_include_directories(mbed-m460
    INTERFACE
        .
        crypto
        device
        device/Reg
        device/StdDriver/inc
)

target_link_libraries(mbed-m460 INTERFACE mbed-nuvoton mbed-flash-cmsis-algo)

mbed_set_linker_script(mbed-m460 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-numaker-iot-m467 INTERFACE)

target_include_directories(mbed-numaker-iot-m467
    INTERFACE
        TARGET_NUMAKER_IOT_M467
)

target_link_libraries(mbed-numaker-iot-m467 INTERFACE mbed-m460)
