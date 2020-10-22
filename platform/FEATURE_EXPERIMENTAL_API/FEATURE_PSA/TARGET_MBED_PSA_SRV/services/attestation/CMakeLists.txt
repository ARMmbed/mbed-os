# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-psa
    INTERFACE
        .
        ./qcbor
        ./qcbor/inc
        ./qcbor/src
        ./tfm_impl
        ./tfm_impl/t_cose/inc
        ./tfm_impl/t_cose/src
)

target_sources(mbed-psa
    INTERFACE
        attest_boot_status_loader.c
        attest_crypto.c
        attest_crypto_keys.c
        attest_iat_claims_loader.c
        attestation_bootloader_data.c
        psa_attest_inject_key.c
        psa_attestation_stubs.c
        psa_initial_attestation_api.c
        psa_inject_attestation_key_impl.c

        qcbor/src/UsefulBuf.c
        qcbor/src/ieee754.c
        qcbor/src/qcbor_decode.c
        qcbor/src/qcbor_encode.c

        tfm_impl/attest_token.c
        tfm_impl/attestation_core.c

        tfm_impl/t_cose/src/t_cose_sign1_sign.c
        tfm_impl/t_cose/src/t_cose_util.c
)
