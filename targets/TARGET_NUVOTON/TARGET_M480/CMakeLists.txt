# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUMAKER_IOT_M487" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUMAKER_IOT_M487)
elseif("NUMAKER_PFM_M487" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUMAKER_PFM_M487) 
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M487.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/M487.sct)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        analogin_api.c
        analogout_api.c
        can_api.c

        crypto/crypto-misc.cpp
        device/startup_M480.c
        device/system_M480.c
        device/StdDriver/src/m480_acmp.c
        device/StdDriver/src/m480_bpwm.c
        device/StdDriver/src/m480_can.c
        device/StdDriver/src/m480_ccap.c
        device/StdDriver/src/m480_clk.c
        device/StdDriver/src/m480_crc.c
        device/StdDriver/src/m480_crypto.c
        device/StdDriver/src/m480_dac.c
        device/StdDriver/src/m480_eadc.c
        device/StdDriver/src/m480_ebi.c
        device/StdDriver/src/m480_ecap.c
        device/StdDriver/src/m480_emac.c
        device/StdDriver/src/m480_epwm.c
        device/StdDriver/src/m480_fmc.c
        device/StdDriver/src/m480_gpio.c
        device/StdDriver/src/m480_hsusbd.c
        device/StdDriver/src/m480_i2c.c
        device/StdDriver/src/m480_i2s.c
        device/StdDriver/src/m480_pdma.c
        device/StdDriver/src/m480_qei.c
        device/StdDriver/src/m480_qspi.c
        device/StdDriver/src/m480_rtc.c
        device/StdDriver/src/m480_sc.c
        device/StdDriver/src/m480_scuart.c
        device/StdDriver/src/m480_sdh.c
        device/StdDriver/src/m480_spi.c
        device/StdDriver/src/m480_spim.c
        device/StdDriver/src/m480_sys.c
        device/StdDriver/src/m480_timer.c
        device/StdDriver/src/m480_timer_pwm.c
        device/StdDriver/src/m480_trng.c
        device/StdDriver/src/m480_uart.c
        device/StdDriver/src/m480_usbd.c
        device/StdDriver/src/m480_usci_i2c.c
        device/StdDriver/src/m480_usci_spi.c
        device/StdDriver/src/m480_usci_uart.c
        device/StdDriver/src/m480_wdt.c
        device/StdDriver/src/m480_wwdt.c

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
        us_ticker.c
        watchdog_api.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        crypto
        device
        device/Reg
        device/StdDriver/inc
)
