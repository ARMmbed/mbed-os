# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_H747I" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_H747I)
elseif("PORTENTA_H7" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_PORTENTA_H7)
elseif("NUCLEO_H743ZI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_H743ZI)
elseif("NUCLEO_H743ZI2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_H743ZI2)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
        ./lan8742
)

target_sources(mbed-emac
    INTERFACE
        lan8742/lan8742.c
)
