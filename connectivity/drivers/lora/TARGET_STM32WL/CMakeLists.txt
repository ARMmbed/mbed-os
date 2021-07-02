# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_WL55JC" IN_LIST MBED_TARGET)
    add_subdirectory(TARGET_NUCLEO_WL55JC)
endif()

if("LORA_E5" IN_LIST MBED_TARGET)
    add_subdirectory(TARGET_LORA_E5)
endif()

if("RAK3172" IN_LIST MBED_TARGET)
    add_subdirectory(TARGET_RAK3172)
endif()
