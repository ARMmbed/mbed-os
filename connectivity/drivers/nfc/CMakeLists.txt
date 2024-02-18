# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("COMPONENT_NXP_PN512=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_NXP_PN512)
endif()

if("COMPONENT_STM_M24SR=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_STM_M24SR)
endif()
