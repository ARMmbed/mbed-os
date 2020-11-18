# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("M23_NS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M23_NS)
elseif("M23_S" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M23_S)
elseif("NU_PFM_M2351" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NU_PFM_M2351)
endif()

target_sources(mbed-core
    INTERFACE
        dma_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        gpio_object.h
        hal_secure.h
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        objects.h
        PeripheralNames.h
        PeripheralPins.c
        PeripheralPins.h
        pinmap.c
        PinNamesCommon.h
        PortNames.h
        port_api.c
        pwmout_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        trng_api.cpp
        us_ticker.c
        trng_api.cpp

        crypto/crypto-misc.cpp

        device/StdDriver/src/m2351_acmp.c
        device/StdDriver/src/m2351_bpwm.c
        device/StdDriver/src/m2351_can.c
        device/StdDriver/src/m2351_clk.c
        device/StdDriver/src/m2351_crc.c
        device/StdDriver/src/m2351_crypto.c
        device/StdDriver/src/m2351_dac.c
        device/StdDriver/src/m2351_eadc.c
        device/StdDriver/src/m2351_ebi.c
        device/StdDriver/src/m2351_ecap.c
        device/StdDriver/src/m2351_epwm.c
        device/StdDriver/src/m2351_fmc.c
        device/StdDriver/src/m2351_gpio.c
        device/StdDriver/src/m2351_i2c.c
        device/StdDriver/src/m2351_i2s.c
        device/StdDriver/src/m2351_pdma.c
        device/StdDriver/src/m2351_qei.c
        device/StdDriver/src/m2351_qspi.c
        device/StdDriver/src/m2351_rtc.c
        device/StdDriver/src/m2351_sc.c
        device/StdDriver/src/m2351_scuart.c
        device/StdDriver/src/m2351_sdh.c
        device/StdDriver/src/m2351_spi.c
        device/StdDriver/src/m2351_sys.c
        device/StdDriver/src/m2351_timer.c
        device/StdDriver/src/m2351_timer_pwm.c
        device/StdDriver/src/m2351_uart.c
        device/StdDriver/src/m2351_usbd.c
        device/StdDriver/src/m2351_usci_i2c.c
        device/StdDriver/src/m2351_usci_spi.c
        device/StdDriver/src/m2351_usci_uart.c
        device/StdDriver/src/m2351_wdt.c
        device/StdDriver/src/m2351_wwdt.c
        device/stddriver_secure.c
        device/stddriver_secure.h
        device/system_core_M2351.h
        device/system_M2351.c
        device/system_M2351.h
        device/startup_M2351.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        device
        device/Reg
        device/StdDriver/inc
        crypto
)
