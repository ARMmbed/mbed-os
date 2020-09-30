# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-nordic OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")

target_link_libraries(mbed-os-ble-nordic-cordio INTERFACE mbed-os-ble)
