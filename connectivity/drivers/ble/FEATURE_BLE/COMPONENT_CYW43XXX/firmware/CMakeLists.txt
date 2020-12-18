# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("43012" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_43012)
endif()

if("43438" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_43438)
endif()

if("4343W" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_4343W)
endif()

if("CYW9P62S1_43012EVB_01" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CYW9P62S1_43012EVB_01)
endif()
