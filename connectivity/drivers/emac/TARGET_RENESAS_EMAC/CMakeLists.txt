# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("RZ_A1XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_RZ_A1XX)
elseif("RZ_A2XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_RZ_A2XX)
endif()
