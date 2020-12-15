# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("M2351" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M2351)
elseif("M251" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M251)
elseif("M261" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M261)
elseif("M451" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M451)
elseif("M480" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M480)
elseif("NANO100" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NANO100)
elseif("NUC472" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUC472)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        nu_miscutil.c
        nu_modutil.c
        nu_timer.c
        USBEndpoints_Nuvoton.cpp
)
