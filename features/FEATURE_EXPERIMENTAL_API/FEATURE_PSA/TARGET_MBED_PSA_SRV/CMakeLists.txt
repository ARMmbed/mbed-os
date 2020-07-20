# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(services)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/psa
        ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls
        ${CMAKE_CURRENT_SOURCE_DIR}/pal
)

target_sources(mbed-os
    PRIVATE
        mbedtls/psa_crypto.c
        mbedtls/psa_crypto_se.c
        mbedtls/psa_crypto_slot_management.c
        mbedtls/psa_crypto_storage.c
        mbedtls/psa_its_file.c

        pal/pal_attestation_eat.c
        pal/pal_attestation_intf.c
        pal/pal_client_api_empty_intf.c
        pal/pal_client_api_intf.c
        pal/pal_internal_trusted_storage_intf.c
        pal/pal_mbed_os_intf.cpp
        pal/pal_protected_storage_intf.c

        src/client.c
        src/default_random_seed.cpp
)
