# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/qcbor
        ${CMAKE_CURRENT_SOURCE_DIR}/qcbor/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/qcbor/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_impl
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_impl/t_cose/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_impl/t_cose/src
)

target_sources(mbed-os
    PRIVATE
        attest_boot_status_loader.c
        attest_crypto.c
        attest_crypto_keys.c
        attest_iat_claims_loader.c
        attestation_bootloader_data.c
        psa_attest_inject_key.c
        psa_attestation_stubs.c
        psa_initial_attestation_api.c

        qcbor/src/UsefulBuf.c
        qcbor/src/ieee754.c
        qcbor/src/qcbor_decode.c
        qcbor/src/qcbor_encode.c

        tfm_impl/attest_token.c
        tfm_impl/attestation_core.c

        tfm_impl/t_cose/src/t_cose_sign1_sign.c
        tfm_impl/t_cose/src/t_cose_util.c
)
