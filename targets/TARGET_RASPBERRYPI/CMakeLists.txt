# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("RPI2010" IN_LIST MBED_TARGET_LABELS)
  add_subdirectory(TARGET_RPI2040)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)
