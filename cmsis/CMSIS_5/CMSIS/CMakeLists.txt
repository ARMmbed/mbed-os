# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("CORTEX_A" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORTEX_A)
elseif("CORTEX_M" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORTEX_M)
endif()

