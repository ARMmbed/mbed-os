# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-mbedtls-nuvoton OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")

mbed_link_component(mbed-os-mbedtls-nuvoton mbed-os-mbedtls)
