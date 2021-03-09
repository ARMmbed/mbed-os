# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbedtls-stm32l562xe INTERFACE)

target_include_directories(mbed-mbedtls-stm32l562xe
    INTERFACE
        .
)

target_link_libraries(mbed-mbedtls-stm32l562xe INTERFACE mbed-mbedtls-stm)

target_link_libraries(mbed-mbedtls PRIVATE mbed-mbedtls-stm32l562xe)

# Expose include paths for an application and components
# mbedtls_device.h should be publicly available
target_include_directories(mbed-mbedtls
    PUBLIC
        $<TARGET_PROPERTY:mbed-mbedtls-stm32l562xe,INTERFACE_INCLUDE_DIRECTORIES>
)
