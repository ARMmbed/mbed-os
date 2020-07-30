# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

add_subdirectory(atmel-rf-driver)
add_subdirectory(mcr20a-rf-driver)
add_subdirectory(stm-s2lp-rf-driver)
