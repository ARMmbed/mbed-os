# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("CRYPTOCELL310" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(FEATURE_CRYPTOCELL310)
endif()

if("Cypress" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Cypress)
elseif("NUVOTON" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUVOTON)
elseif("STM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM)
elseif("Samsung" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Samsung)
elseif("Silicon_Labs" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Silicon_Labs)
endif()
