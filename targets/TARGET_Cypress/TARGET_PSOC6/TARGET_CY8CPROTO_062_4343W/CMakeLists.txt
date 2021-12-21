# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy8cproto-062-4343w-bsp-design-modus INTERFACE)
target_include_directories(mbed-cy8cproto-062-4343w-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource
)
target_sources(mbed-cy8cproto-062-4343w-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_clocks.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_connectivity_bt.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_peripherals.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_pins.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_qspi_memslot.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_routing.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_system.c
)

add_library(mbed-cy8cproto-062-4343w-cm0p INTERFACE)
if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE_CM0P COMPONENT_CM0P/TOOLCHAIN_ARM/cy8c6xxa_cm0plus.sct)
    set(STARTUP_FILE_CM0P COMPONENT_CM0P/TOOLCHAIN_ARM/startup_psoc6_02_cm0plus.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE_CM0P COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm0plus.ld)
    set(STARTUP_FILE_CM0P COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm0plus.S)
endif()
target_sources(mbed-cy8cproto-062-4343w-cm0p
    INTERFACE
        COMPONENT_CM0P/system_psoc6_cm0plus.c
        ${STARTUP_FILE_CM0P}
)
mbed_set_linker_script(mbed-cy8cproto-062-4343w-cm0p ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_CM0P})

add_library(mbed-cy8cproto-062-4343w-cm4 INTERFACE)
if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_ARM/cy8c6xxa_cm4_dual.sct)
    set(STARTUP_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_ARM/startup_psoc6_02_cm4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm4_dual.ld)
    set(STARTUP_FILE_CM4 COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S)
endif()
target_sources(mbed-cy8cproto-062-4343w-cm4
    INTERFACE
        COMPONENT_CM4/system_psoc6_cm4.c
        ${STARTUP_FILE_CM4}
)
mbed_set_linker_script(mbed-cy8cproto-062-4343w-cm4 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_CM4})


add_library(mbed-cy8cproto-062-4343w INTERFACE)
target_include_directories(mbed-cy8cproto-062-4343w
    INTERFACE
        .
)
target_sources(mbed-cy8cproto-062-4343w
    INTERFACE
        PeripheralPins.c
        cybsp.c
)
target_link_libraries(mbed-cy8cproto-062-4343w
    INTERFACE
        mbed-cat1a
        mbed-cy8cproto-062-4343w-cm4
        mbed-cy8cproto-062-4343w-bsp-design-modus
        mbed-psoc6
)

if("CM0P_SLEEP" IN_LIST MBED_TARGET_LABELS)
    target_link_libraries(mbed-cy8cproto-062-4343w
        INTERFACE
            mbed-cm0p-sleep
    )
endif()

if("WHD" IN_LIST MBED_TARGET_LABELS)
    target_link_libraries(mbed-cy8cproto-062-4343w
        INTERFACE
            mbed-cy-psoc6-whd
            mbed-cy-psoc6-whd-4343w
            mbed-cy8cproto-062-4343w-whd
    )
endif()

target_compile_definitions(mbed-cy8cproto-062-4343w
    INTERFACE
        "CY8C624ABZI_D44"
        "CYBSP_WIFI_CAPABLE"
)
