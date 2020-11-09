# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("Freescale" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Freescale)
elseif("NORDIC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NORDIC)     
elseif("STM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM)    
endif()
