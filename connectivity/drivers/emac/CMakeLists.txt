# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("ARM_FM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_ARM_FM)
elseif("ARM_SSG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_ARM_SSG)
elseif("Cypress" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Cypress)
elseif("Freescale_EMAC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Freescale_EMAC)
elseif("GD_EMAC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_GD_EMAC)
elseif("NUVOTON_EMAC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUVOTON_EMAC)
elseif("NXP_EMAC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NXP_EMAC)
elseif("RDA_EMAC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_RDA_EMAC)
elseif("RENESAS_EMAC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_RENESAS_EMAC)
elseif("STM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM)
elseif("Silicon_Labs" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Silicon_Labs)
endif()

if("WHD" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_WHD)
endif()

target_link_libraries(mbed-emac
    INTERFACE
        mbed-netsocket
)
