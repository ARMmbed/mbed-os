# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("ARM_FM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_ARM_FM)
elseif("Freescale" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Freescale)
elseif("NORDIC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NORDIC)
elseif("NUVOTON" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUVOTON)
elseif("STM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM)
elseif("WICED" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_WICED)
endif()
