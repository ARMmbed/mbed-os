# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("M480" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M480)
elseif("NUC472" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUC472)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        numaker_emac.cpp
)
