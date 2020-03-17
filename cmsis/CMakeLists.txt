# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

target_sources(mbed-os PRIVATE RTE_Components.h)

target_include_directories(mbed-os PUBLIC .)
