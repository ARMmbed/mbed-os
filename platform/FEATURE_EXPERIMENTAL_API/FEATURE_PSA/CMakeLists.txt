# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-psa STATIC EXCLUDE_FROM_ALL)

if("MBED_PSA_SRV" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MBED_PSA_SRV)
endif()

if("TFM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM)
endif()

target_include_directories(mbed-psa
    INTERFACE
        inc
        inc/psa
)

target_sources(mbed-psa
    PRIVATE
        src/psa_hrng.c
)

# For now, on devices with PSA, PSA functions are used by the HAL so PSA needs to be included in the core Mbed build.
target_link_libraries(mbed-core-flags INTERFACE mbed-psa)