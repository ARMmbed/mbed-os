# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arduino-nano33ble INTERFACE)
add_library(mbed-ep-agora INTERFACE)
add_library(mbed-ep-atlas INTERFACE)
add_library(mbed-nrf52840-dk INTERFACE)

target_include_directories(mbed-arduino-nano33ble
    INTERFACE
        TARGET_ARDUINO_NANO33BLE
)

target_include_directories(mbed-ep-agora
    INTERFACE
        TARGET_EP_AGORA
)
target_sources(mbed-ep-agora
    INTERFACE
        TARGET_EP_AGORA/ONBOARD_TELIT_ME910.cpp
)

target_include_directories(mbed-ep-atlas
    INTERFACE
        TARGET_EP_ATLAS
)

target_sources(mbed-ep-atlas
    INTERFACE
        TARGET_EP_ATLAS/ONBOARD_TELIT_ME310.cpp
        TARGET_EP_ATLAS/usb_stdio.cpp
        TARGET_EP_ATLAS/atlas_init.c
)

target_include_directories(mbed-nrf52840-dk
    INTERFACE
        TARGET_NRF52840_DK
)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/nRF52840.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_nrf52840.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/NRF52840.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_NRF52840.S)
endif()

add_library(mbed-mcu-nrf52840 INTERFACE)

target_include_directories(mbed-mcu-nrf52840
    INTERFACE
        .
        config
        device
)

target_sources(mbed-mcu-nrf52840
    INTERFACE
        PeripheralPins.c
        USBPhy_Nordic.cpp

        device/cmsis_nvic.c
        device/system_nrf52840.c

        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-mcu-nrf52840 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-mcu-nrf52840 INTERFACE mbed-nrf52 mbed-sdk-15-0)
target_link_libraries(mbed-arduino-nano33ble INTERFACE mbed-mcu-nrf52840)
target_link_libraries(mbed-ep-agora INTERFACE mbed-mcu-nrf52840)
target_link_libraries(mbed-ep-atlas INTERFACE mbed-mcu-nrf52840)
target_link_libraries(mbed-nrf52840-dk INTERFACE mbed-mcu-nrf52840)
