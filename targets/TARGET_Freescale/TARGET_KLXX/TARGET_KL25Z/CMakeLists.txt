# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MKL25Z4.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MKL25Z4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MKL25Z4.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MKL25Z4.S)
endif()

add_library(mbed-kl25z INTERFACE)

target_include_directories(mbed-kl25z
    INTERFACE
        .
        device
)

target_sources(mbed-kl25z
    INTERFACE
        PeripheralPins.c
        gpio_irq_api.c
        mbed_overrides.c
        serial_api.c
        spi_api.c

        device/system_MKL25Z4.c
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-kl25z ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-kl25z INTERFACE mbed-klxx)
