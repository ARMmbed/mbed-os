# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nrf52-dk INTERFACE)
add_library(mbed-sdt52832b INTERFACE)

target_include_directories(mbed-nrf52-dk
    INTERFACE
        TARGET_NRF52_DK
)

target_include_directories(mbed-sdt52832b
    INTERFACE
        TARGET_SDT52832B
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/nRF52832.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_nrf52832.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/NRF52832.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_NRF52832.S)
endif()

add_library(mbed-mcu-nrf52832 INTERFACE)

target_include_directories(mbed-mcu-nrf52832
    INTERFACE
        .
        config
        device
)

target_sources(mbed-mcu-nrf52832
    INTERFACE
        PeripheralPins.c

        device/cmsis_nvic.c
        device/system_nrf52.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-mcu-nrf52832 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-mcu-nrf52832 INTERFACE mbed-nrf52 mbed-sdk-15-0)
target_link_libraries(mbed-nrf52-dk INTERFACE mbed-mcu-nrf52832)
target_link_libraries(mbed-sdt52832b INTERFACE mbed-mcu-nrf52832)
