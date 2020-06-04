# SPDX-License-Identifier: Apache-2.0

get_property(target_labels GLOBAL PROPERTY MBED_OS_TARGET_LABELS)
mbed_add_cmake_directory("TARGET" "${target_labels}")

target_sources(mbed-os PRIVATE RTE_Components.h)

target_include_directories(mbed-os PUBLIC .)
