# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-psa
    INTERFACE
        inc
        inc/psa
        mbedtls
        services/platform
        services/attestation
        services/attestation/qcbor
        services/attestation/qcbor/inc
        services/attestation/qcbor/src
        services/attestation/tfm_impl
        services/attestation/tfm_impl/t_cose/inc
        services/attestation/tfm_impl/t_cose/src
        services/storage/common
        services/storage/its
)

target_sources(mbed-psa
    INTERFACE
        mbedtls/psa_crypto.c
        mbedtls/psa_crypto_se.c
        mbedtls/psa_crypto_driver_wrappers.c
        mbedtls/psa_crypto_slot_management.c
        mbedtls/psa_crypto_storage.c
        mbedtls/psa_its_file.c

        services/attestation/attest_boot_status_loader.c
        services/attestation/attest_crypto.c
        services/attestation/attest_crypto_keys.c
        services/attestation/attest_iat_claims_loader.c
        services/attestation/attestation_bootloader_data.c
        services/attestation/psa_attest_inject_key.c
        services/attestation/psa_attestation_stubs.c
        services/attestation/psa_initial_attestation_api.c
        services/attestation/psa_inject_attestation_key_impl.c

        services/attestation/qcbor/src/UsefulBuf.c
        services/attestation/qcbor/src/ieee754.c
        services/attestation/qcbor/src/qcbor_decode.c
        services/attestation/qcbor/src/qcbor_encode.c

        services/attestation/tfm_impl/attest_token.c
        services/attestation/tfm_impl/attestation_core.c

        services/attestation/tfm_impl/t_cose/src/t_cose_sign1_sign.c
        services/attestation/tfm_impl/t_cose/src/t_cose_util.c

        services/platform/platform_emul.c
        services/platform/platform_srv_impl.c

        services/storage/common/psa_storage_common_impl.cpp

        services/storage/its/pits_impl.cpp
        services/storage/its/psa_prot_internal_storage.cpp

        services/storage/ps/protected_storage.cpp

        src/client.c
        src/default_random_seed.cpp
)

target_link_libraries(mbed-psa
    INTERFACE
        mbed-mbedtls
        mbed-storage-kvstore
        mbed-storage-tdbstore
        mbed-storage-kv-global-api
        mbed-device_key
)

add_subdirectory(test_abstraction_layers)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(BUILD_GREENTEA_TESTS)
        add_subdirectory(TESTS)
    endif()
endif()
