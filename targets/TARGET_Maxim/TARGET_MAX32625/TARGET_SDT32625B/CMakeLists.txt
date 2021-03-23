# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sdt32625b INTERFACE)

target_include_directories(mbed-sdt32625b
    INTERFACE
        .
)

target_link_libraries(mbed-sdt32625b INTERFACE mbed-max32625-no-boot)
