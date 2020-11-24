# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F0" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F0)
elseif("STM32F1" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F1)
elseif("STM32F2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F2)
elseif("STM32F3" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F3)
elseif("STM32F4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F4)
elseif("STM32F7" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F7)
elseif("STM32G0" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G0)
elseif("STM32G4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G4)
elseif("STM32H7" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H7)
elseif("STM32L0" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L0)
elseif("STM32L1" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L1)
elseif("STM32L4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L4)
elseif("STM32L5" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L5)
elseif("STM32WB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32WB)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        USBPhy_STM32.cpp
        analogin_api.c
        analogout_api.c
        can_api.c
        gpio_api.c
        gpio_irq_api.c
        hal_tick_overrides.c
        i2c_api.c
        lp_ticker.c
        mbed_crc_api.c
        mbed_overrides.c
        pinmap.c
        port_api.c
        pwmout_api.c
        qspi_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        stm_spi_api.c
        trng_api.c
        us_ticker.c
        watchdog_api.c
)
