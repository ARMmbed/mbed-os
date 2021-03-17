# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M2354.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARMC6/M2354.sct)
endif()

add_library(mbed-m2354 INTERFACE)

target_sources(mbed-m2354
    INTERFACE
        analogin_api.c
        analogout_api.c

        device/startup_M2354.c
        device/system_M2354.c
        device/StdDriver/src/m2354_acmp.c
        device/StdDriver/src/m2354_bpwm.c
        device/StdDriver/src/m2354_can.c
        device/StdDriver/src/m2354_clk.c
        device/StdDriver/src/m2354_crc.c
        device/StdDriver/src/m2354_crypto.c
        device/StdDriver/src/m2354_dac.c
        device/StdDriver/src/m2354_dpm.c
        device/StdDriver/src/m2354_eadc.c
        device/StdDriver/src/m2354_ebi.c
        device/StdDriver/src/m2354_ecap.c
        device/StdDriver/src/m2354_epwm.c
        device/StdDriver/src/m2354_ewdt.c
        device/StdDriver/src/m2354_ewwdt.c
        device/StdDriver/src/m2354_fmc.c
        device/StdDriver/src/m2354_fvc.c
        device/StdDriver/src/m2354_gpio.c
        device/StdDriver/src/m2354_i2c.c
        device/StdDriver/src/m2354_i2s.c
        device/StdDriver/src/m2354_keystore.c
        device/StdDriver/src/m2354_lcd.c
        device/StdDriver/src/m2354_pdma.c
        device/StdDriver/src/m2354_qei.c
        device/StdDriver/src/m2354_qspi.c
        device/StdDriver/src/m2354_rng.c
        device/StdDriver/src/m2354_rtc.c
        device/StdDriver/src/m2354_sc.c
        device/StdDriver/src/m2354_scuart.c
        device/StdDriver/src/m2354_sdh.c
        device/StdDriver/src/m2354_spi.c
        device/StdDriver/src/m2354_sys.c
        device/StdDriver/src/m2354_tamper.c
        device/StdDriver/src/m2354_timer.c
        device/StdDriver/src/m2354_timer_pwm.c
        device/StdDriver/src/m2354_uart.c
        device/StdDriver/src/m2354_usbd.c
        device/StdDriver/src/m2354_usci_i2c.c
        device/StdDriver/src/m2354_usci_spi.c
        device/StdDriver/src/m2354_usci_uart.c
        device/StdDriver/src/m2354_wdt.c
        device/StdDriver/src/m2354_wwdt.c

        dma_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        PeripheralPins.c
        pinmap.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
)

target_include_directories(mbed-m2354
    INTERFACE
        .
        device
        device/Reg
        device/StdDriver/inc
)

target_link_libraries(mbed-m2354 INTERFACE mbed-nuvoton)

mbed_set_linker_script(mbed-m2354 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-nu-m2354 INTERFACE)

if("TFM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM)
endif()

target_include_directories(mbed-nu-m2354
    INTERFACE
        TARGET_NU_M2354
)

target_link_libraries(mbed-nu-m2354
    INTERFACE
        mbed-m2354
        mbed-m2354-tfm
)
