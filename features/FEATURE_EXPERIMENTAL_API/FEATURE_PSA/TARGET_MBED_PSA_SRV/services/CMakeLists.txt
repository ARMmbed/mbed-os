# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(attestation)
add_subdirectory(storage)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

target_sources(mbed-os
    PRIVATE
        platform/platform_emul.c
        platform/platform_srv_impl.c
)
