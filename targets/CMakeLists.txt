# SPDX-License-Identifier: Apache-2.0

get_property(target_labels GLOBAL PROPERTY MBED_OS_TARGET_LABELS)
mbed_add_cmake_directory("" "${target_labels}")
