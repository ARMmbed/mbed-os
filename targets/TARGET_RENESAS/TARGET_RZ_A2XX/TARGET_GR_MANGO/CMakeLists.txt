# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MBRZA1H.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_RZ_A2M.S)
    set(WEAK_HANDLER_FILE device/TOOLCHAIN_ARM_STD/weak_handler.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/RZA2M.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_RZA2M.S)
    set(WEAK_HANDLER_FILE device/TOOLCHAIN_GCC_ARM/weak_handler.S)
endif()

add_library(mbed-gr-mango INTERFACE)

target_include_directories(mbed-gr-mango
    INTERFACE
        .
        device
        device/inc
        device/inc/iodefine
        device/inc/iodefine/iobitmasks
        device/inc/iodefine/iodefines
)

target_sources(mbed-gr-mango
    INTERFACE
        PeripheralPins.c

        device/RZ_A2_Init.c
        device/cmsis_nvic.c
        device/mbed_sf_boot.c
        device/mmu_RZ_A2M.c
        device/nvic_wrapper.c
        device/octaram_init.c
        device/os_tick_ostm.c
        device/system_RZ_A2M.c

        ${STARTUP_FILE}
        ${WEAK_HANDLER_FILE}
)

mbed_set_linker_script(mbed-gr-mango ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-gr-mango INTERFACE mbed-rz-a2xx)
