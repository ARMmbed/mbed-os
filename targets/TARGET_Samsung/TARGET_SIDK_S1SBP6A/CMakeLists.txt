# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/s1sbp6a.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_s1sbp6a.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/s1sbp6a.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_s1sbp6a.S)
endif()

add_library(mbed-s1sbp6a INTERFACE)

target_include_directories(mbed-s1sbp6a
    INTERFACE
        .
        device
)

target_sources(mbed-s1sbp6a
    INTERFACE
        PeripheralPins.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        rtc_api.c
        serial_api.c
        sleep_api.c
        spi_api.c
        us_ticker.c
        watchdog_api.c

        device/s1sbp6a_cmu.c
        device/s1sbp6a_gpio.c
        device/s1sbp6a_i2c.c
        device/s1sbp6a_pmu.c
        device/s1sbp6a_rtc.c
        device/s1sbp6a_wdog.c
        device/system_s1sbp6a.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-s1sbp6a ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-s1sbp6a INTERFACE mbed-cmsis-cortex-m)
