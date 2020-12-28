# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MCU_NRF52840" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_NRF52840)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
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
