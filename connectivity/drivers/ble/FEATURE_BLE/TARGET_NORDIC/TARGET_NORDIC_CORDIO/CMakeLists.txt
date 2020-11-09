# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MCU_NRF52840" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_NRF52840)
elseif("NRF5x" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NRF5x)
endif()
