# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/psa
        ${CMAKE_CURRENT_SOURCE_DIR}/include/psa_manifest
)

target_sources(mbed-os
    PRIVATE
        src/tfm_crypto_ipc_api.c
        src/tfm_initial_attestation_ipc_api.c
        src/tfm_its_ipc_api.c
        src/tfm_platform_ipc_api.c
        src/tfm_sst_ipc_api.c
)
