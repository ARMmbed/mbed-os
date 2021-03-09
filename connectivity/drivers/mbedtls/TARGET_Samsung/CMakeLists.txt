# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-samsung)

target_include_directories(mbed-mbedtls-samsung
    INTERFACE
        .
        ./sha
)

target_sources(mbed-mbedtls-samsung
    INTERFACE
        sha/sha256_alt.c
        sha/sha512_alt.c
)

target_link_libraries(mbed-mbedtls PUBLIC mbed-mbedtls-samsung)

# Expose include paths for an application and components
# mbedtls_device.h should be publicly available
target_include_directories(mbed-mbedtls
    PUBLIC
        $<TARGET_PROPERTY:mbed-mbedtls-samsung,INTERFACE_INCLUDE_DIRECTORIES>
)
