# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("Freescale" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Freescale)
    add_subdirectory(mcr20a-rf-driver)
elseif("Silicon_Labs" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Silicon_Labs)
elseif("STM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(stm-s2lp-rf-driver)
endif()

add_subdirectory(atmel-rf-driver)

target_link_libraries(mbed-nanostack
    INTERFACE
        mbed-802.15.4-rf
)
