# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all optional platform libraries available.
add_library(mbed-os-psa INTERFACE)


add_subdirectory(cxxsupport)
add_subdirectory(mbed-trace)
add_subdirectory(randlib)
add_subdirectory(source)

target_include_directories(mbed-os
    PUBLIC
        include
        include/platform
        include/platform/internal
)
