# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-m2354-tfm INTERFACE)

if("NU_M2354" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NU_M2354)
endif()

target_include_directories(mbed-m2354-tfm
    INTERFACE
        .
)

target_sources(mbed-m2354-tfm
    INTERFACE
        platform_extra_secure.c
        platform_extra_secure_compat.c
        tfm_ns_interface.c
)
