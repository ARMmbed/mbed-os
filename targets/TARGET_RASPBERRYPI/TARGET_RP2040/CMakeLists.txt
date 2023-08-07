# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_RASPBERRY_PI_PICO EXCLUDE_FROM_ALL)

set(LINKER_FILE TOOLCHAIN_GCC_ARM/memmap_default_mbed.ld)

target_include_directories(mbed-rp2040
    INTERFACE
        .
)

target_sources(mbed-rp2040
    INTERFACE
        analogin_api.c
        flash_api.c
        gpio_api.c
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        PeripheralPins.c
        pwmout_api.c
        reset_reason.c
        serial_api.c
        spi_api.c
        rtc_api.c
        us_ticker.c
        USBPhy_RP2040.cpp
        watchdog_api.c
        TOOLCHAIN_GCC_ARM/crt0_mbed.S
)

mbed_set_linker_script(mbed-rp2040 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-rp2040 INTERFACE mbed-raspberrypi)