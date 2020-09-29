# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-mbedtls-nuvoton OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")

target_link_libraries(mbed-os-mbedtls-nuvoton mbed-os-mbedtls)
