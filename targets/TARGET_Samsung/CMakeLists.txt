# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("SIDK_S1SBP6A" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_SIDK_S1SBP6A)
elseif("SIDK_S5JS100" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_SIDK_S5JS100)
endif()
