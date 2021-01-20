# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

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
    INTERFACE
        src/psa_hrng.c
)

target_link_libraries(mbed-core
    INTERFACE
        mbed-psa
)
