# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NU_PFM_M2351_NPSA_NS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NU_PFM_M2351_NPSA_NS)
elseif("NU_PFM_M2351_NS" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NU_PFM_M2351_NS)
elseif("TFM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM)  
endif()

add_subdirectory(device)

target_sources(mbed-core
    INTERFACE
        wait_ns.c
)
