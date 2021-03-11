# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-stm32f439xl INTERFACE)

target_include_directories(mbed-mbedtls-stm32f439xl
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-stm32f439xl INTERFACE mbed-mbedtls-stm)

target_link_libraries(mbed-mbedtls PRIVATE mbed-mbedtls-stm32f439xl)

# Expose include paths for an application and components
# mbedtls_device.h should be publicly available
target_include_directories(mbed-mbedtls
    PUBLIC
        $<TARGET_PROPERTY:mbed-mbedtls-stm32f439xl,INTERFACE_INCLUDE_DIRECTORIES>
)
