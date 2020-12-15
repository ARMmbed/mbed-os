# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if ("ARDUINO_NANO33BLE" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_ARDUINO_NANO33BLE
    )
elseif("EP_AGORA" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_EP_AGORA
    )
    target_sources(mbed-core
        INTERFACE
            TARGET_EP_AGORA/ONBOARD_TELIT_ME910.cpp
    )
elseif("EP_ATLAS" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_EP_ATLAS
    )
# TODO CMAKE this file TARGET_EP_ATLAS/ONBOARD_TELIT_ME310.cpp should probably
#    be moved to connectivity/drivers/cellular/TELIT/ME310, although the mbed-os-example-cellular
#    application builds successfully without this file (the cmake library mbed-mbedtls-cryptocell310
#    needs to be added to the application).

#    target_sources(mbed-core
#        INTERFACE
#            TARGET_EP_ATLAS/ONBOARD_TELIT_ME310.cpp
#    )
elseif("NRF52840_DK" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_NRF52840_DK
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/nRF52840.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_nrf52840.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/NRF52840.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_NRF52840.S)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
        config
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        USBPhy_Nordic.cpp

        device/cmsis_nvic.c
        device/system_nrf52840.c

        ${STARTUP_FILE}
)
