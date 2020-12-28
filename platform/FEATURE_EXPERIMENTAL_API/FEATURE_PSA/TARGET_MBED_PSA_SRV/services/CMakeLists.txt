# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(attestation)
add_subdirectory(storage)

target_include_directories(mbed-psa
    INTERFACE
        ./platform
)

target_sources(mbed-psa
    INTERFACE
        platform/platform_emul.c
        platform/platform_srv_impl.c
)
