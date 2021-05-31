# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sdk-15-0 INTERFACE)

# Silence unfixable warning in nrfx
target_compile_options(mbed-sdk-15-0 INTERFACE -Wno-expansion-to-defined)

add_subdirectory(TARGET_SOFTDEVICE_NONE EXCLUDE_FROM_ALL)
add_subdirectory(components EXCLUDE_FROM_ALL)
add_subdirectory(integration EXCLUDE_FROM_ALL)
add_subdirectory(modules EXCLUDE_FROM_ALL)

target_link_libraries(mbed-sdk-15-0 INTERFACE mbed-softdevice-none)
