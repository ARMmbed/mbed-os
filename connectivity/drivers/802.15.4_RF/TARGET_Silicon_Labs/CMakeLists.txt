# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("SL_RAIL" IN_LIST MBED_TARGET_LABELS)
    create_mbed_802_15_4_target()
    add_subdirectory(TARGET_SL_RAIL)
endif()
