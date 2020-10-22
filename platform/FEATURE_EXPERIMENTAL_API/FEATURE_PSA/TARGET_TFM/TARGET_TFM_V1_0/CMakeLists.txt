# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TFM_DUALCPU" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM_DUALCPU)
endif()

if("TFM_V8M" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_TFM_V8M)
endif()

target_include_directories(mbed-psa
    INTERFACE
        ./include
        ./include/psa
        ./include/psa_manifest
)

target_sources(mbed-psa
    INTERFACE
        src/tfm_crypto_ipc_api.c
        src/tfm_initial_attestation_ipc_api.c
        src/tfm_its_ipc_api.c
        src/tfm_platform_ipc_api.c
        src/tfm_sst_ipc_api.c
)
