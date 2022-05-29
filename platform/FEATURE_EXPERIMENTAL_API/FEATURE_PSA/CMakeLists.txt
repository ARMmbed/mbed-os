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
    PUBLIC
        inc
        inc/psa
)

target_sources(mbed-psa
    PRIVATE
        src/psa_hrng.c
)

target_link_libraries(mbed-psa PRIVATE mbed-core-flags)