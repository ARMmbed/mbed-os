# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MPS2_M0" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MPS2_M0/device
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_MPS2_M0/device/cmsis_nvic.c
    )

    set(SYSTEM_FILE TARGET_MPS2_M0/device/system_CMSDK_CM0.c)

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE TARGET_MPS2_M0/device/TOOLCHAIN_ARM_STD/MPS2.sct)
        set(STARTUP_FILE TARGET_MPS2_M0/device/TOOLCHAIN_ARM_STD/startup_MPS2.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE TARGET_MPS2_M0/device/TOOLCHAIN_GCC_ARM/MPS2.ld)
        set(STARTUP_FILE TARGET_MPS2_M0/device/TOOLCHAIN_GCC_ARM/startup_MPS2.S)
    endif()
elseif("MPS2_M0P" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MPS2_M0P/device
    )

    set(SYSTEM_FILE TARGET_MPS2_M0P/device/system_CMSDK_CM0plus.c)

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE TARGET_MPS2_M0P/device/TOOLCHAIN_ARM_STD/MPS2.sct)
        set(STARTUP_FILE TARGET_MPS2_M0P/device/TOOLCHAIN_ARM_STD/startup_MPS2.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE TARGET_MPS2_M0P/device/TOOLCHAIN_GCC_ARM/MPS2.ld)
        set(STARTUP_FILE TARGET_MPS2_M0P/device/TOOLCHAIN_GCC_ARM/startup_MPS2.S)
    endif()
elseif("MPS2_M3" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MPS2_M3/device
    )

    set(SYSTEM_FILE TARGET_MPS2_M3/device/system_CMSDK_CM3.c)

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE TARGET_MPS2_M3/device/TOOLCHAIN_ARM_STD/MPS2.sct)
        set(STARTUP_FILE TARGET_MPS2_M3/device/TOOLCHAIN_ARM_STD/startup_MPS2.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE TARGET_MPS2_M3/device/TOOLCHAIN_GCC_ARM/MPS2.ld)
        set(STARTUP_FILE TARGET_MPS2_M3/device/TOOLCHAIN_GCC_ARM/startup_MPS2.S)
    endif()
elseif("MPS2_M4" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MPS2_M4/device
    )

    set(SYSTEM_FILE TARGET_MPS2_M4/device/system_CMSDK_CM4.c)

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE TARGET_MPS2_M4/device/TOOLCHAIN_ARM_STD/MPS2.sct)
        set(STARTUP_FILE TARGET_MPS2_M4/device/TOOLCHAIN_ARM_STD/startup_MPS2.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE TARGET_MPS2_M4/device/TOOLCHAIN_GCC_ARM/MPS2.ld)
        set(STARTUP_FILE TARGET_MPS2_M4/device/TOOLCHAIN_GCC_ARM/startup_MPS2.S)
    endif()
elseif("MPS2_M7" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MPS2_M7/device
    )

    set(SYSTEM_FILE TARGET_MPS2_M7/device/system_CMSDK_CM7.c)

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE TARGET_MPS2_M7/device/TOOLCHAIN_ARM_STD/MPS2.sct)
        set(STARTUP_FILE TARGET_MPS2_M7/device/TOOLCHAIN_ARM_STD/startup_CMSDK_CM7.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE TARGET_MPS2_M7/device/TOOLCHAIN_GCC_ARM/MPS2.ld)
        set(STARTUP_FILE TARGET_MPS2_M7/device/TOOLCHAIN_GCC_ARM/startup_MPS2.S)
    endif()
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
        SDK
)

target_sources(mbed-core
    INTERFACE
        analogin_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        serial_api.c
        spi_api.c
        us_ticker.c

        SDK/ETH_MPS2.c
        SDK/fpga.c
        SDK/mps2_ethernet_api.c

        ${SYSTEM_FILE}
        ${STARTUP_FILE}
)
