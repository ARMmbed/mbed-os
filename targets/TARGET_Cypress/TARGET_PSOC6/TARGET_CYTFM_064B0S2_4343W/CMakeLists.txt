# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_cypress)

add_library(mbed-cytfm-064b0s2-4343w-bsp-design-modus INTERFACE)
target_include_directories(mbed-cytfm-064b0s2-4343w-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource
)
target_sources(mbed-cytfm-064b0s2-4343w-bsp-design-modus
    INTERFACE
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_clocks.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_peripherals.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_pins.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_qspi_memslot.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_routing.c
        COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_system.c
)

add_library(mbed-cytfm-064b0s2-4343w-cm4 INTERFACE)
if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_ARM/cyb06xxa_cm4_dual.sct)
    set(STARTUP_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_ARM/startup_psoc6_02_cm4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cyb06xxa_cm4_dual.ld)
    set(STARTUP_FILE_CM4 device/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S)
endif()
target_include_directories(mbed-cytfm-064b0s2-4343w-cm4
    INTERFACE
        device/COMPONENT_CM4
)
target_sources(mbed-cytfm-064b0s2-4343w-cm4
    INTERFACE
        device/COMPONENT_CM4/system_psoc6_cm4.c
        device/COMPONENT_CM4/device_definition.c
        ${STARTUP_FILE_CM4}
)
mbed_set_linker_script(mbed-cytfm-064b0s2-4343w-cm4 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_CM4})

add_library(mbed-cytfm-064b0s2-4343w INTERFACE)
target_include_directories(mbed-cytfm-064b0s2-4343w
    INTERFACE
        .
        device
        partition
)
target_sources(mbed-cytfm-064b0s2-4343w
    INTERFACE
        PeripheralPins.c
        cybsp.c
        cytfm_flash_info.c
)
target_link_libraries(mbed-cytfm-064b0s2-4343w
    INTERFACE
        mbed-cat1a
        mbed-cytfm-064b0s2-4343w-cm4
        mbed-cytfm-064b0s2-4343w-bsp-design-modus
        mbed-psoc6
)

if("WHD" IN_LIST MBED_TARGET_LABELS)
    target_link_libraries(mbed-cytfm-064b0s2-4343w
        INTERFACE
            mbed-cy-psoc6-whd
            mbed-cy-psoc6-whd-4343w
            mbed-cytfm-064b0s2-4343w-whd
    )
endif()

target_compile_definitions(mbed-cytfm-064b0s2-4343w
    INTERFACE
        "CYB0644ABZI_S2D44"
        "CY_IPC_DEFAULT_CFG_DISABLE"
        "CYBSP_WIFI_CAPABLE"
        "MXCRYPTO_DISABLED"
        "TFM_MULTI_CORE_MULTI_CLIENT_CALL=1"
)

mbed_post_build_psoc6_sign_image(
    "tfm_s.hex"
    "CYTFM_064B0S2_4343W"
    "policy_multi_CM0_CM4_tfm.json"
    "multi_image"
    "1"
    "16"
    ${CMAKE_CURRENT_SOURCE_DIR}/COMPONENT_TFM_S_FW/tfm_s.hex
)
