# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("BlueNRG_2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_BlueNRG_2)
endif()

if("BlueNRG_MS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_BlueNRG_MS)
endif()

if("CYW43XXX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_CYW43XXX)
endif()

if("Ambiq_Micro" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Ambiq_Micro)
elseif("CY8C63XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CY8C63XX)
elseif("NORDIC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NORDIC)
elseif("STM32WB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32WB)
endif()
