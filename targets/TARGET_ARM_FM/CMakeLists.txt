# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(ARM_FM INTERFACE)

add_subdirectory(TARGET_FVP_MPS2 EXCLUDE_FROM_ALL)

target_include_directories(ARM_FM
    INTERFACE
        .
)
