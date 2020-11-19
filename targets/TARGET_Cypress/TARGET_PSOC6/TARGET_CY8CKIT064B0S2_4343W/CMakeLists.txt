# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("BSP_DESIGN_MODUS" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource
    )

    target_sources(mbed-core
        INTERFACE
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg.c
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_clocks.c
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_peripherals.c
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_pins.c
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_qspi_memslot.c
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_routing.c
            COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_system.c
    )
endif()

if("CM0P" IN_LIST MBED_TARGET_LABELS)
    set(SYSTEM_SOURCE device/COMPONENT_CM0P/system_psoc6_cm0plus.c)
    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE device/COMPONENT_CM0P/TOOLCHAIN_ARM/startup_psoc6_02_cm0plus.S)
        set(LINKER_FILE device/COMPONENT_CM0P/TOOLCHAIN_ARM/cyb06xxa_cm0plus.sct)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE device/COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm0plus.S)
        set(LINKER_FILE device/COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/cyb06xxa_cm0plus.ld)
    endif()
elseif("CM4" IN_LIST MBED_TARGET_LABELS)
    set(SYSTEM_SOURCE device/COMPONENT_CM4/system_psoc6_cm4.c)
    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE device/COMPONENT_CM4/TOOLCHAIN_ARM/startup_psoc6_02_cm4.S)
        set(LINKER_FILE device/COMPONENT_CM4/TOOLCHAIN_ARM/cyb06xxa_cm4_dual.sct)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE device/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S)
        set(LINKER_FILE device/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cyb06xxa_cm4_dual.ld)
    endif()
endif()

set_property(
    GLOBAL PROPERTY
    MBED_TARGET_LINKER_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        cybsp.c
        ${SYSTEM_SOURCE}
        ${STARTUP_FILE}
)
