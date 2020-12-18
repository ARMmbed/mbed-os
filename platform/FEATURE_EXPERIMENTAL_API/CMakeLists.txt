# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("PSA" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(FEATURE_PSA)
endif()
