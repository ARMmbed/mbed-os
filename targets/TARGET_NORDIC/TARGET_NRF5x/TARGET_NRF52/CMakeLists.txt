# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MCU_NRF52832 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCU_NRF52840 EXCLUDE_FROM_ALL)

add_library(mbed-nrf52 INTERFACE)

target_include_directories(mbed-nrf52
    INTERFACE
        .
)

target_sources(mbed-nrf52
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
        subtarget_init.c
        trng_api.c
        us_ticker.c
        watchdog_api.c
)

target_link_libraries(mbed-nrf52 INTERFACE mbed-nrf5x)
