# SPDX-License-Identifier: Apache-2.0

# Add target directories by appending the prefixes below to the labels defined
# by MBED_OS_TARGET_LABELS in target.cmake
mbed_add_cmake_directory_if_labels("")
mbed_add_cmake_directory_if_labels("TARGET")
