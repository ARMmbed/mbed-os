# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("PN512" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(PN512)
endif()

if("M24SR" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_M24SR)
endif()
