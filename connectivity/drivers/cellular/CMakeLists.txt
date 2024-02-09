# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("COMPONENT_STMOD_CELLULAR=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(COMPONENT_STMOD_CELLULAR)
endif()

add_subdirectory(Altair)
add_subdirectory(GEMALTO)
add_subdirectory(GENERIC)
add_subdirectory(MultiTech)
add_subdirectory(QUECTEL)
add_subdirectory(RiotMicro)
add_subdirectory(TELIT)