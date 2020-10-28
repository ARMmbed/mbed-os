# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F3" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F3)
elseif("STM32F4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F4)
elseif("STM32L4" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L4)
endif()

target_include_directories(mbed-core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
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
