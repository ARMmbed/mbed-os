# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    if("NU_XRAM_SUPPORTED" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE_NU_XRAM_SUPPORTED device/TOOLCHAIN_GCC_ARM/TARGET_NU_XRAM_SUPPORTED/NUC472.ld)
    elseif("NU_XRAM_UNSUPPORTED" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE_NU_XRAM_UNSUPPORTED device/TOOLCHAIN_GCC_ARM/TARGET_NU_XRAM_UNSUPPORTED/NUC472.ld)
    endif()
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    if("NU_XRAM_SUPPORTED" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE_NU_XRAM_SUPPORTED device/TOOLCHAIN_ARM_STD/TARGET_NU_XRAM_SUPPORTED/NUC472.sct)
    elseif("NU_XRAM_UNSUPPORTED" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE_NU_XRAM_UNSUPPORTED device/TOOLCHAIN_ARM_STD/TARGET_NU_XRAM_UNSUPPORTED/NUC472.sct)
    endif()
endif()

add_library(mbed-nu-xram-supported INTERFACE)
mbed_set_linker_script(mbed-nu-xram-supported ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_NU_XRAM_SUPPORTED})

add_library(mbed-nu-xram-unsupported INTERFACE)
mbed_set_linker_script(mbed-nu-xram-unsupported ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_NU_XRAM_UNSUPPORTED})



add_library(mbed-nuc472 INTERFACE)

target_sources(mbed-nuc472
    INTERFACE
        crypto/crypto-misc.cpp

        analogin_api.c
        can_api.c
        crypto/crypto-misc.cpp
        device/startup_NUC472_442.c
        device/StdDriver/nuc472_acmp.c
        device/StdDriver/nuc472_adc.c
        device/StdDriver/nuc472_can.c
        device/StdDriver/nuc472_clk.c
        device/StdDriver/nuc472_crypto.c
        device/StdDriver/nuc472_eadc.c
        device/StdDriver/nuc472_ebi.c
        device/StdDriver/nuc472_emac.c
        device/StdDriver/nuc472_fmc.c
        device/StdDriver/nuc472_gpio.c
        device/StdDriver/nuc472_i2c.c
        device/StdDriver/nuc472_i2s.c
        device/StdDriver/nuc472_pdma.c
        device/StdDriver/nuc472_ps2.c
        device/StdDriver/nuc472_pwm.c
        device/StdDriver/nuc472_rtc.c
        device/StdDriver/nuc472_sc.c
        device/StdDriver/nuc472_scuart.c
        device/StdDriver/nuc472_sd.c
        device/StdDriver/nuc472_spi.c
        device/StdDriver/nuc472_sys.c
        device/StdDriver/nuc472_timer.c
        device/StdDriver/nuc472_uart.c
        device/StdDriver/nuc472_usbd.c
        device/StdDriver/nuc472_wdt.c
        device/StdDriver/nuc472_wwdt.c
        device/system_NUC472_442.c

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
        wait_ns.c
        watchdog_api.c
)

target_include_directories(mbed-nuc472
    INTERFACE
        .
        crypto
        device
        device/StdDriver
)

target_link_libraries(mbed-nuc472 INTERFACE mbed-nuvoton)

add_library(mbed-numaker-pfm-nuc472 INTERFACE)

target_link_libraries(mbed-numaker-pfm-nuc472 INTERFACE mbed-nuc472 mbed-nu-xram-supported mbed-flash-cmsis-algo)
