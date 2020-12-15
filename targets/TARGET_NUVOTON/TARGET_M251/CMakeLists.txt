# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUMAKER_IOT_M252" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUMAKER_IOT_M252)    
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M251.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/M251.sct)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        analogin_api.c
        analogout_api.c
        device/startup_M251.c
        device/StdDriver/src/m251_acmp.c
        device/StdDriver/src/m251_bpwm.c
        device/StdDriver/src/m251_clk.c
        device/StdDriver/src/m251_crc.c
        device/StdDriver/src/m251_crypto.c
        device/StdDriver/src/m251_dac.c
        device/StdDriver/src/m251_eadc.c
        device/StdDriver/src/m251_ebi.c
        device/StdDriver/src/m251_fmc.c
        device/StdDriver/src/m251_gpio.c
        device/StdDriver/src/m251_i2c.c
        device/StdDriver/src/m251_pdma.c
        device/StdDriver/src/m251_pwm.c
        device/StdDriver/src/m251_qspi.c
        device/StdDriver/src/m251_rtc.c
        device/StdDriver/src/m251_sc.c
        device/StdDriver/src/m251_scuart.c
        device/StdDriver/src/m251_spi.c
        device/StdDriver/src/m251_sys.c
        device/StdDriver/src/m251_timer.c
        device/StdDriver/src/m251_timer_pwm.c
        device/StdDriver/src/m251_uart.c
        device/StdDriver/src/m251_usbd.c
        device/StdDriver/src/m251_usci_i2c.c
        device/StdDriver/src/m251_usci_spi.c
        device/StdDriver/src/m251_usci_uart.c
        device/StdDriver/src/m251_wdt.c
        device/StdDriver/src/m251_wwdt.c
        device/system_M251.c

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
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        device
        device/Reg
        device/StdDriver/inc
)
