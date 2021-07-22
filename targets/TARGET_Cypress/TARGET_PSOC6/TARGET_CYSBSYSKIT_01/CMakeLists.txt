# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_cypress)

add_library(mbed-cysbsyskit-01-bsp-design-modus INTERFACE)
target_include_directories(mbed-cysbsyskit-01-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource
)
target_sources(mbed-cysbsyskit-01-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_connectivity_bt.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_peripherals.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_pins.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_qspi_memslot.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_system.c
)

add_library(mbed-cysbsyskit-01-cm4 INTERFACE)
if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_ARM/cy8c6xxa_cm4_dual.sct)
    set(STARTUP_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_ARM/startup_psoc6_02_cm4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm4_dual.ld)
    set(STARTUP_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S)
endif()
target_sources(mbed-cysbsyskit-01-cm4
    INTERFACE
        COMPONENT_CM4/system_psoc6_cm4.c
        ${STARTUP_FILE_CM4}
)
mbed_set_linker_script(mbed-cysbsyskit-01-cm4 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_CM4})

add_library(mbed-cysbsyskit-01 INTERFACE)
target_include_directories(mbed-cysbsyskit-01
    INTERFACE
        .
)
target_sources(mbed-cysbsyskit-01
    INTERFACE
        PeripheralPins.c
        cybsp.c
)
target_link_libraries(mbed-cysbsyskit-01
    INTERFACE
        mbed-cat1a
        mbed-cysbsyskit-01-cm4
        mbed-cysbsyskit-01-bsp-design-modus
        mbed-psoc6
        mbed-cy-psoc6-scl
        mbed-lwipstack
        mbed-emac
)

target_compile_definitions(mbed-cysbsyskit-01
    INTERFACE
        "CY8C624AFNI_S2D43F"
)

mbed_post_build_psoc6_merge_hex(
    PSOC6_TARGET
        "CYSBSYSKIT_01"
)
