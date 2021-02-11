# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_ARCH_PRO EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MBED_LPC1768 EXCLUDE_FROM_ALL)

add_library(mbed-lpc176x INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_LPC17xx.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/LPC1768.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_LPC17xx.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/LPC1768.ld)
endif()

target_include_directories(mbed-lpc176x
    INTERFACE
        .
        device
)

target_sources(mbed-lpc176x
    INTERFACE
        analogin_api.c
        analogout_api.c
        can_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
        watchdog_api.c

        device/CRP.c
        device/flash_api.c
        device/system_LPC17xx.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-lpc176x INTERFACE mbed-nxp)

mbed_set_linker_script(mbed-arch-pro ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})
mbed_set_linker_script(mbed-lpc1768 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})
