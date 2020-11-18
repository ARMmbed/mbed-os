# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NU_PREBUILD_SECURE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NU_PREBUILD_SECURE)
endif()
