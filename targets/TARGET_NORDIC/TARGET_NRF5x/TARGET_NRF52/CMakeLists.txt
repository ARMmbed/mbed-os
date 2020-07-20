# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        PeripheralPinsDefault.c
        analogin_api.c
        common_rtc.c
        critical_section_api.c
        flash_api.c
        gpio_api.c
        i2c_api.c
        itm_api.c
        object_owners.c
        pinmap_ex.c
        pwmout_api.c
        reloc_vector_table.c
        serial_api.c
        sleep.c
        spi_api.c
        trng_api.c
        us_ticker.c
        watchdog_api.c
)

mbed_add_cmake_directory_if_labels("TARGET")
