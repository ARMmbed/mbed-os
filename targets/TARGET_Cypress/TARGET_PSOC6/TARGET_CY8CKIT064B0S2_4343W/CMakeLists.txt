# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_cypress)

add_library(mbed-cy8ckit064b0s2-4343w-bsp-design-modus INTERFACE)
target_include_directories(mbed-cy8ckit064b0s2-4343w-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource
)
target_sources(mbed-cy8ckit064b0s2-4343w-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_clocks.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_peripherals.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_pins.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_qspi_memslot.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_routing.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_system.c
)

add_library(mbed-cy8ckit064b0s2-4343w-cm0p INTERFACE)
if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE_CM0P device/COMPONENT_CM0P/TOOLCHAIN_ARM/startup_psoc6_02_cm0plus.S)
    set(LINKER_FILE_CM0P device/COMPONENT_CM0P/TOOLCHAIN_ARM/cyb06xxa_cm0plus.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE_CM0P device/COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm0plus.S)
    set(LINKER_FILE_CM0P device/COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/cyb06xxa_cm0plus.ld)
endif()
target_sources(mbed-cy8ckit064b0s2-4343w-cm0p
    INTERFACE
        device/COMPONENT_CM0P/system_psoc6_cm0plus.c
        ${STARTUP_FILE_CM0P}
)
mbed_set_linker_script(mbed-cy8ckit064b0s2-4343w-cm0p ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_CM0P})

add_library(mbed-cy8ckit064b0s2-4343w-cm4 INTERFACE)
if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_ARM/startup_psoc6_02_cm4.S)
    set(LINKER_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_ARM/cyb06xxa_cm4_dual.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S)
    set(LINKER_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cyb06xxa_cm4_dual.ld)
endif()
target_sources(mbed-cy8ckit064b0s2-4343w-cm4
    INTERFACE
        device/COMPONENT_CM4/system_psoc6_cm4.c
        ${STARTUP_FILE_CM4}
)
mbed_set_linker_script(mbed-cy8ckit064b0s2-4343w-cm4 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_CM4})

add_library(mbed-cy8ckit064b0s2-4343w INTERFACE)
target_include_directories(mbed-cy8ckit064b0s2-4343w
    INTERFACE
        .
        device
)
target_sources(mbed-cy8ckit064b0s2-4343w
    INTERFACE
        PeripheralPins.c
        cybsp.c
)
target_link_libraries(mbed-cy8ckit064b0s2-4343w
    INTERFACE
        mbed-cat1a
        mbed-cy8ckit064b0s2-4343w-cm4
        mbed-cy8ckit064b0s2-4343w-bsp-design-modus
        mbed-cm0p-secure
        mbed-psoc6
)

if("WHD" IN_LIST MBED_TARGET_LABELS)
    target_link_libraries(mbed-cy8ckit064b0s2-4343w
        INTERFACE
            mbed-cy-psoc6-whd
            mbed-cy-psoc6-whd-4343w
            mbed-cy8ckit-064b0s2-4343w-whd
    )
endif()

target_compile_definitions(mbed-cy8ckit064b0s2-4343w
    INTERFACE
        "CYB0644ABZI_S2D44"
        "CY_IPC_DEFAULT_CFG_DISABLE"
        "CYBSP_WIFI_CAPABLE"
)

mbed_post_build_psoc6_sign_image(
    "psoc6_02_cm0p_secure.hex"
    "CY8CKIT064B0S2_4343W"
    "policy_single_CM0_CM4.json"
    "single_image"
    "1"
    "16"
    "${CMAKE_CURRENT_SOURCE_DIR}/device/COMPONENT_CM4/hex/psoc6_02_cm0p_secure.hex"
)
