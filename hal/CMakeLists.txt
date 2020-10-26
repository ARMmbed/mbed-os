# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("FLASH_CMSIS_ALGO" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_FLASH_CMSIS_ALGO)
endif()

add_subdirectory(usb)

target_include_directories(mbed-core
    INTERFACE
        include
        include/hal
)

target_sources(mbed-core
    INTERFACE
        source/LowPowerTickerWrapper.cpp
        source/mbed_compat.c
        source/mbed_critical_section_api.c
        source/mbed_flash_api.c
        source/mbed_gpio.c
        source/mbed_gpio_irq.c
        source/mbed_itm_api.c
        source/mbed_lp_ticker_api.c
        source/mbed_lp_ticker_wrapper.cpp
        source/mbed_pinmap_common.c
        source/mbed_pinmap_default.cpp
        source/mbed_ticker_api.c
        source/mbed_us_ticker_api.c
        source/static_pinmap.cpp

        source/mpu/mbed_mpu_v7m.c
        source/mpu/mbed_mpu_v8m.c
)
