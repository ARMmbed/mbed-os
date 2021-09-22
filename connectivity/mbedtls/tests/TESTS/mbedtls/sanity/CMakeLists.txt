# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

# Note: The test's source also checks MBEDTLS_PSA_CRYPTO_C from
# mbedtls/config.h which can't be checked directly from CMake,
# but it is enabled by default on all PSA targets.
if((NOT "EXPERIMENTAL_API" IN_LIST MBED_TARGET_LABELS)
    OR (NOT "PSA" IN_LIST MBED_TARGET_LABELS))
    set(TEST_SKIPPED "Only targets with PSA enabled are supported")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-mbedtls-sanity
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-mbedtls
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
