# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-greentea-io INTERFACE)
target_sources(mbed-greentea-io INTERFACE mbed_io.cpp)
target_link_libraries(mbed-greentea-io INTERFACE mbed-core)
