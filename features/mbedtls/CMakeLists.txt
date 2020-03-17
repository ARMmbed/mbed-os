# SPDX-License-Identifier: Apache-2.0

add_subdirectory(mbed-crypto)

mbed_add_cmake_directory_if_labels("platform/TARGET")

target_sources(mbed-os PRIVATE platform/src/mbed_trng.cpp)

target_include_directories(mbed-os PRIVATE .)
target_include_directories(mbed-os PRIVATE inc)
