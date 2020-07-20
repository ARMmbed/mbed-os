# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        LowPowerTickerWrapper.cpp
        mbed_compat.c
        mbed_critical_section_api.c
        mbed_flash_api.c
        mbed_gpio.c
        mbed_gpio_irq.c
        mbed_itm_api.c
        mbed_lp_ticker_api.c
        mbed_lp_ticker_wrapper.cpp
        mbed_pinmap_common.c
        mbed_pinmap_default.cpp
        mbed_ticker_api.c
        mbed_us_ticker_api.c
        static_pinmap.cpp

        mpu/mbed_mpu_v7m.c
        mpu/mbed_mpu_v8m.c
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(mbed-os
    PUBLIC
        usb
)
