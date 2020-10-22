# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("ARCH_MAX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_ARCH_MAX)
elseif("NUCLEO_F429ZI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F429ZI)
elseif("NUCLEO_F439ZI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F439ZI)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)
