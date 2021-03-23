# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M487.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/M487.sct)
endif()

add_library(mbed-m480 INTERFACE)

target_sources(mbed-m480
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

target_include_directories(mbed-m480
    INTERFACE
        .
        crypto
        device
        device/Reg
        device/StdDriver/inc
)

target_link_libraries(mbed-m480 INTERFACE mbed-nuvoton mbed-flash-cmsis-algo)

mbed_set_linker_script(mbed-m480 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-numaker-iot-m487 INTERFACE)

target_include_directories(mbed-numaker-iot-m487
    INTERFACE
        TARGET_NUMAKER_IOT_M487
)

target_link_libraries(mbed-numaker-iot-m487 INTERFACE mbed-m480)

add_library(mbed-numaker-pfm-m487 INTERFACE)

target_include_directories(mbed-numaker-pfm-m487
    INTERFACE
        TARGET_NUMAKER_PFM_M487
)

target_link_libraries(mbed-numaker-pfm-m487 INTERFACE mbed-m480)
