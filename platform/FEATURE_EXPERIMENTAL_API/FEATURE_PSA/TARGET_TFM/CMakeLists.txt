# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TFM_V1_0" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM_V1_0)
endif()

if("TFM_LATEST" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM_LATEST)
endif()
