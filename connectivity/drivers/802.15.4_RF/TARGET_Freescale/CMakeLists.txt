# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("KW41Z" IN_LIST MBED_TARGET_LABELS)
    create_mbed_802_15_4_target()
    add_subdirectory(TARGET_KW41Z)
endif()
