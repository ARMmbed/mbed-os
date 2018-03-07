/* Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <secure_time_utils.h>
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "secure_time_client_spe.h"
#include "secure_time_impl.h"
#include "secure_time_storage.h"
#include "secure_time_test_utils.h"
#include "nvstore.h"

#ifdef ENABLE_LIBGCOV_PORT
#include "libgcov-embedded.h"
#endif

#if !NVSTORE_ENABLED
#error [NOT_SUPPORTED] NVSTORE needs to be enabled for this test
#endif

using namespace utest::v1;

#define SECURE_TIME_TEST_SECS_PER_MINUTE    (60)
#define SECURE_TIME_TEST_SECS_PER_HOUR      (60 * SECURE_TIME_TEST_SECS_PER_MINUTE)
#define SECURE_TIME_TEST_SECS_PER_DAY       (24 * SECURE_TIME_TEST_SECS_PER_HOUR)

#define SECURE_TIME_TEST_DEFAULT_TIME       (2555562978ULL)

#define EXTRACT_UINT16(buf) ((((uint16_t)(((uint8_t *)(buf))[1])) << 8) + (uint16_t)(((uint8_t *)(buf))[0]))
#define WRITE_UINT16(buf, val)                                       \
    {                                                                \
        ((uint8_t *)(buf))[0] = (uint8_t)(((uint16_t)(val)) & 0xff); \
        ((uint8_t *)(buf))[1] = (uint8_t)(((uint16_t)(val)) >> 8);   \
    }

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t ca_prvkey[] = {
    0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x47, 0x2d, 0x6d, 0x08, 0x7c,
    0xeb, 0x6d, 0x4c, 0xb1, 0xa1, 0x20, 0xb4, 0x80, 0x5f, 0x47, 0x78, 0xd6,
    0xa5, 0x69, 0xf7, 0x34, 0xf2, 0xa2, 0x85, 0xb1, 0x5d, 0xae, 0xfa, 0x53,
    0x57, 0x33, 0x7b, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0x7f, 0x4a, 0x31,
    0xc8, 0x30, 0xbf, 0x71, 0x0a, 0x62, 0x91, 0xd9, 0xef, 0x54, 0xfa, 0x66,
    0xe4, 0xab, 0xe9, 0xfa, 0x80, 0x12, 0x42, 0xdc, 0x16, 0x9f, 0x09, 0x37,
    0x4d, 0xc6, 0x8c, 0x06, 0x03, 0x51, 0x9b, 0x1d, 0xd2, 0x36, 0x69, 0xe6,
    0xc8, 0x30, 0x62, 0x44, 0x5d, 0xe5, 0x15, 0xb4, 0x9c, 0x9f, 0x9b, 0x23,
    0x0a, 0x00, 0x1f, 0x8b, 0x4e, 0x8c, 0x8f, 0x5e, 0x80, 0x46, 0x71, 0xdc,
    0xb4
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t ca_pubkey[] = {
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00, 0x04, 0x7f, 0x4a, 0x31, 0xc8, 0x30, 0xbf, 0x71, 0x0a, 0x62,
    0x91, 0xd9, 0xef, 0x54, 0xfa, 0x66, 0xe4, 0xab, 0xe9, 0xfa, 0x80, 0x12,
    0x42, 0xdc, 0x16, 0x9f, 0x09, 0x37, 0x4d, 0xc6, 0x8c, 0x06, 0x03, 0x51,
    0x9b, 0x1d, 0xd2, 0x36, 0x69, 0xe6, 0xc8, 0x30, 0x62, 0x44, 0x5d, 0xe5,
    0x15, 0xb4, 0x9c, 0x9f, 0x9b, 0x23, 0x0a, 0x00, 0x1f, 0x8b, 0x4e, 0x8c,
    0x8f, 0x5e, 0x80, 0x46, 0x71, 0xdc, 0xb4
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t prvkey1[] = {
    0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x61, 0x93, 0xa8, 0x0f, 0xaf,
    0xd0, 0xc0, 0x60, 0x78, 0x65, 0x40, 0xc8, 0xd7, 0x77, 0x83, 0xda, 0x5a,
    0x12, 0x92, 0x26, 0x94, 0x77, 0xac, 0x1e, 0xb5, 0x69, 0xa2, 0x24, 0x3c,
    0x97, 0x86, 0x93, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0x10, 0x47, 0x96,
    0xc0, 0x7e, 0x64, 0x84, 0x9a, 0x56, 0xd5, 0xf0, 0xf6, 0x8f, 0x08, 0x74,
    0xed, 0x90, 0x87, 0x89, 0x45, 0x41, 0x7a, 0xf4, 0xd0, 0x1f, 0x98, 0xce,
    0xc2, 0xad, 0x0c, 0x62, 0x3e, 0x9d, 0x59, 0xde, 0x0f, 0x45, 0x4d, 0x8f,
    0xa6, 0x54, 0x25, 0x91, 0xf5, 0x93, 0xd5, 0xb1, 0xd7, 0xe7, 0x62, 0x49,
    0x61, 0x21, 0x6b, 0x62, 0xa7, 0x18, 0x4b, 0xc9, 0x73, 0x28, 0xc8, 0x5d,
    0xa9
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t pubkey1[] = {
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00, 0x04, 0x10, 0x47, 0x96, 0xc0, 0x7e, 0x64, 0x84, 0x9a, 0x56,
    0xd5, 0xf0, 0xf6, 0x8f, 0x08, 0x74, 0xed, 0x90, 0x87, 0x89, 0x45, 0x41,
    0x7a, 0xf4, 0xd0, 0x1f, 0x98, 0xce, 0xc2, 0xad, 0x0c, 0x62, 0x3e, 0x9d,
    0x59, 0xde, 0x0f, 0x45, 0x4d, 0x8f, 0xa6, 0x54, 0x25, 0x91, 0xf5, 0x93,
    0xd5, 0xb1, 0xd7, 0xe7, 0x62, 0x49, 0x61, 0x21, 0x6b, 0x62, 0xa7, 0x18,
    0x4b, 0xc9, 0x73, 0x28, 0xc8, 0x5d, 0xa9
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t prvkey2[] = {
    0x30, 0x76, 0x02, 0x01, 0x01, 0x04, 0x1f, 0x47, 0x17, 0x35, 0x1f, 0xb1,
    0xbd, 0xb9, 0x94, 0x83, 0x10, 0x11, 0xee, 0x4a, 0x6f, 0xc1, 0xc7, 0xa9,
    0xab, 0xec, 0xf7, 0xd8, 0x33, 0xb1, 0xf8, 0x63, 0xe5, 0xf2, 0xa7, 0xaa,
    0x68, 0xc9, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03,
    0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xd9, 0xbf, 0xff, 0xcb,
    0xb1, 0x40, 0x0c, 0x9b, 0x8b, 0x7e, 0x0e, 0xbd, 0x69, 0x89, 0x82, 0xa1,
    0xed, 0x33, 0xee, 0x95, 0xa5, 0x34, 0xc8, 0x41, 0xb0, 0x88, 0xf9, 0xcb,
    0xac, 0x15, 0xdf, 0xab, 0x4e, 0x0a, 0x16, 0x45, 0xd5, 0xdf, 0x89, 0x3e,
    0xb3, 0x7f, 0x05, 0xc2, 0x78, 0x28, 0xb3, 0xf5, 0x2f, 0x29, 0xed, 0xa1,
    0x6c, 0x43, 0xf6, 0x7a, 0x59, 0x81, 0x73, 0x1e, 0x1d, 0xa5, 0x22, 0x19
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t pubkey2[] = {
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00, 0x04, 0xd9, 0xbf, 0xff, 0xcb, 0xb1, 0x40, 0x0c, 0x9b, 0x8b,
    0x7e, 0x0e, 0xbd, 0x69, 0x89, 0x82, 0xa1, 0xed, 0x33, 0xee, 0x95, 0xa5,
    0x34, 0xc8, 0x41, 0xb0, 0x88, 0xf9, 0xcb, 0xac, 0x15, 0xdf, 0xab, 0x4e,
    0x0a, 0x16, 0x45, 0xd5, 0xdf, 0x89, 0x3e, 0xb3, 0x7f, 0x05, 0xc2, 0x78,
    0x28, 0xb3, 0xf5, 0x2f, 0x29, 0xed, 0xa1, 0x6c, 0x43, 0xf6, 0x7a, 0x59,
    0x81, 0x73, 0x1e, 0x1d, 0xa5, 0x22, 0x19
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t prvkey3[] = {
    0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0xd2, 0x4a, 0x9c, 0xf3, 0xe2,
    0x26, 0x1e, 0x25, 0xc0, 0x04, 0xdf, 0x90, 0x19, 0xf0, 0xdc, 0xb1, 0xd6,
    0x2d, 0xdb, 0x5a, 0x10, 0x25, 0x2a, 0x48, 0xd2, 0xdd, 0x85, 0x4d, 0x90,
    0xda, 0x3a, 0xbc, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xd8, 0x4a, 0x68,
    0x3a, 0xf1, 0x20, 0xec, 0x8f, 0x55, 0x68, 0x15, 0xbc, 0x9c, 0xc3, 0xb2,
    0x29, 0xcb, 0x91, 0x99, 0xea, 0xd9, 0xe7, 0xa3, 0x6c, 0x58, 0xec, 0xd7,
    0x22, 0x80, 0xc0, 0xfe, 0xf2, 0x63, 0x2a, 0x13, 0x71, 0x93, 0x11, 0xc0,
    0x8b, 0x70, 0x04, 0xd5, 0x2f, 0x95, 0xfa, 0xd7, 0x06, 0x79, 0xc6, 0x32,
    0xdf, 0xca, 0xbb, 0xa8, 0xf5, 0x68, 0xdc, 0x9d, 0x5a, 0x5a, 0x18, 0x6f,
    0xad
};

MBED_ALIGN(4) // a WA for a failure with ARMCC
static const uint8_t pubkey3[] = {
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00, 0x04, 0xd8, 0x4a, 0x68, 0x3a, 0xf1, 0x20, 0xec, 0x8f, 0x55,
    0x68, 0x15, 0xbc, 0x9c, 0xc3, 0xb2, 0x29, 0xcb, 0x91, 0x99, 0xea, 0xd9,
    0xe7, 0xa3, 0x6c, 0x58, 0xec, 0xd7, 0x22, 0x80, 0xc0, 0xfe, 0xf2, 0x63,
    0x2a, 0x13, 0x71, 0x93, 0x11, 0xc0, 0x8b, 0x70, 0x04, 0xd5, 0x2f, 0x95,
    0xfa, 0xd7, 0x06, 0x79, 0xc6, 0x32, 0xdf, 0xca, 0xbb, 0xa8, 0xf5, 0x68,
    0xdc, 0x9d, 0x5a, 0x5a, 0x18, 0x6f, 0xad
};

static uint8_t blob[SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES] = {0};
static uint8_t blob2[SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES] = {0};

static data_buffer_t delegation_pubkeys[] = {
    {pubkey1, sizeof(pubkey1)},
    {pubkey2, sizeof(pubkey2)},
    {pubkey3, sizeof(pubkey3)}
};

static data_buffer_t privkeys[] = {
    {ca_prvkey, sizeof(ca_prvkey)},
    {prvkey1, sizeof(prvkey1)},
    {prvkey2, sizeof(prvkey2)},
    {prvkey3, sizeof(prvkey3)}
};

static void blob_without_delegations(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 0, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SUCCESS,
        secure_time_set_trusted_commit(blob, blob_size)
        );
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
}

static void blob_with_delegations(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 3, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SUCCESS,
        secure_time_set_trusted_commit(blob, blob_size)
        );
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
}

static void malformed_blob_signature(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 1, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    // Break the last byte of the blob which is also the last byte of the blob's signature.
    blob[blob_size - 1] ^= 1;

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SIGNATURE_VERIFICATION_FAILED,
        secure_time_set_trusted_commit(blob, blob_size)
        );
}

static void malformed_delegation_signature(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 1, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    // Break the first byte of the delegation signature.
    blob[
        SECURE_TIME_BLOB_HEADER_SIZE_BYTES +
        SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES +
        delegation_pubkeys[0].size +
        SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES
        ] ^= 1;

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SIGNATURE_VERIFICATION_FAILED,
        secure_time_set_trusted_commit(blob, blob_size)
        );
}

static void malformed_blob_signature_size(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 0, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    // Break blob's signature size field.
    uint16_t delegation_size = EXTRACT_UINT16(blob + SECURE_TIME_DELEGATION_LENGTH_OFFSET);
    uint16_t blob_signature_size = EXTRACT_UINT16(blob + SECURE_TIME_BLOB_HEADER_SIZE_BYTES + delegation_size);
    WRITE_UINT16(blob + SECURE_TIME_BLOB_HEADER_SIZE_BYTES + delegation_size, blob_signature_size - 1);

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_INVALID_BLOB_SIZE,
        secure_time_set_trusted_commit(blob, blob_size)
        );
}

static void malformed_delegation_signature_size(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 2, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    // Break first delegation signature size field.
    size_t signature_offset = SECURE_TIME_BLOB_HEADER_SIZE_BYTES + SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES + delegation_pubkeys[0].size;
    uint16_t signature_size = EXTRACT_UINT16(blob + signature_offset);
    WRITE_UINT16(blob + signature_offset, signature_size + 1);

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SIGNATURE_VERIFICATION_FAILED,
        secure_time_set_trusted_commit(blob, blob_size)
        );
}

static void delegation_size_too_short(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 1, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    // Decrement delegation section size by 1 byte.
    uint16_t delegation_size = EXTRACT_UINT16(blob + SECURE_TIME_DELEGATION_LENGTH_OFFSET);
    WRITE_UINT16(blob + SECURE_TIME_DELEGATION_LENGTH_OFFSET, delegation_size - 1);

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_INVALID_BLOB_SIZE,
        secure_time_set_trusted_commit(blob, blob_size)
        );
}

static void delegation_size_too_long(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 1, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    // Increment delegation section size by 1 byte.
    uint16_t delegation_size = EXTRACT_UINT16(blob + SECURE_TIME_DELEGATION_LENGTH_OFFSET);
    WRITE_UINT16(blob + SECURE_TIME_DELEGATION_LENGTH_OFFSET, delegation_size + 1);

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_INVALID_BLOB_SIZE,
        secure_time_set_trusted_commit(blob, blob_size)
        );
}

void wrong_nonce(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce1 = 0, nonce2 = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce1);
    size_t blob_size1 = create_blob(set_time, nonce1, delegation_pubkeys, privkeys, 0, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    secure_time_set_trusted_init(&nonce2);
    size_t blob_size2 = create_blob(set_time, nonce2, delegation_pubkeys, privkeys, 0, blob2, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_NONCE_NOT_MATCH,
        secure_time_set_trusted_commit(blob, blob_size1)
        );
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SUCCESS,
        secure_time_set_trusted_commit(blob2, blob_size2)
        );
}

void wrong_nonce2(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce1 = 0, nonce2 = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce1);
    size_t blob_size1 = create_blob(set_time, nonce1, delegation_pubkeys, privkeys, 0, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    secure_time_set_trusted_init(&nonce2);
    size_t blob_size2 = create_blob(set_time, nonce2, delegation_pubkeys, privkeys, 0, blob2, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);

    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SUCCESS,
        secure_time_set_trusted_commit(blob2, blob_size2)
        );
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_NONCE_MISSING,
        secure_time_set_trusted_commit(blob, blob_size1)
        );
}

static void replay_blob(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);
    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 0, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_SUCCESS,
        secure_time_set_trusted_commit(blob, blob_size)
        );
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());

    wait(4);

    // Send the blob again
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_NONCE_MISSING,
        secure_time_set_trusted_commit(blob, blob_size)
        );
    TEST_ASSERT_UINT64_WITHIN(7, set_time, secure_time_get());
}
/*
void nonce_timeout(void)
{
    uint64_t set_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t nonce = 0;

    provision_data(ca_pubkey, sizeof(ca_pubkey));

    secure_time_set_trusted_init(&nonce);

    // Create a significant timeout for the nonce to become obsolete.
    wait(SECURE_TIME_NONCE_TIMEOUT_SECONDS + 1);

    size_t blob_size = create_blob(set_time, nonce, delegation_pubkeys, privkeys, 0, blob, SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES);
    TEST_ASSERT_EQUAL_HEX(
        SECURE_TIME_NONCE_TIMEOUT,
        secure_time_set_trusted_commit(blob, blob_size)
        );
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
}
*/
void normal_set_forward_no_storage_update(void)
{
    uint64_t curr_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t set_time = curr_time + (SECURE_TIME_MIN_STORAGE_FORWARD_LATENCY_SEC - 100);
    uint64_t stored_time = 0;

    secure_time_update_boot_time(curr_time);
    secure_time_set_stored_time(curr_time);

    secure_time_set(set_time);
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
    secure_time_get_stored_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(curr_time, stored_time);
}

void normal_set_forward_with_storage_update(void)
{
    uint64_t curr_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t set_time = curr_time + (SECURE_TIME_MIN_STORAGE_FORWARD_LATENCY_SEC + 100);
    uint64_t stored_time = 0;

    secure_time_update_boot_time(curr_time);
    secure_time_set_stored_time(curr_time);

    secure_time_set(set_time);
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
    secure_time_get_stored_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(set_time, stored_time);
}

void normal_set_forward_with_storage_update2(void)
{
    uint64_t curr_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t set_time = curr_time + (SECURE_TIME_MIN_STORAGE_FORWARD_LATENCY_SEC - 100);
    uint64_t stored_time = 0;

    secure_time_update_boot_time(curr_time);
    secure_time_set_stored_time(curr_time - (SECURE_TIME_MIN_STORAGE_IDLE_LATENCY_SEC + 100));

    secure_time_set(set_time);
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
    secure_time_get_stored_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(set_time, stored_time);
}

void normal_set_backward_no_drift(void)
{
    uint64_t curr_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t set_time = curr_time - ((30 * SECURE_TIME_TEST_SECS_PER_MINUTE) + 100);
    uint64_t back_time = set_time - (10 * SECURE_TIME_TEST_SECS_PER_DAY);
    uint64_t stored_time = 0;

    secure_time_update_boot_time(curr_time);
    secure_time_set_stored_time(curr_time);
    secure_time_set_stored_back_time(back_time);

    secure_time_set(set_time);
    TEST_ASSERT_UINT64_WITHIN(3, curr_time, secure_time_get());
    secure_time_get_stored_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(curr_time, stored_time);
    secure_time_get_stored_back_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(back_time, stored_time);
}

void normal_set_backward_with_drift(void)
{
    uint64_t curr_time = SECURE_TIME_TEST_DEFAULT_TIME;
    uint64_t set_time = curr_time - ((30 * SECURE_TIME_TEST_SECS_PER_MINUTE) - 100);
    uint64_t back_time = set_time - (10 * SECURE_TIME_TEST_SECS_PER_DAY);
    uint64_t stored_time = 0;

    secure_time_update_boot_time(curr_time);
    secure_time_set_stored_time(curr_time);

    // Can't set backwards if no STORED_BACK entry in storage
    secure_time_set(set_time);
    TEST_ASSERT_UINT64_WITHIN(3, curr_time, secure_time_get());
    secure_time_get_stored_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(curr_time, stored_time);
    secure_time_get_stored_back_time(&stored_time);
    TEST_ASSERT_EQUAL_UINT64(0, stored_time);

    secure_time_set_stored_back_time(back_time);
    secure_time_get_stored_back_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(back_time, stored_time);

    secure_time_set(set_time);
    TEST_ASSERT_UINT64_WITHIN(3, set_time, secure_time_get());
    secure_time_get_stored_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(set_time, stored_time);
    secure_time_get_stored_back_time(&stored_time);
    TEST_ASSERT(stored_time > 0);
    TEST_ASSERT_EQUAL_UINT64(set_time, stored_time);
}

utest::v1::status_t storage_setup(const Case *const source, const size_t index_of_case)
{
    NVStore &nvstore = NVStore::get_instance();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, nvstore.reset());

    // Call the default handler for proper reporting
    return greentea_case_setup_handler(source, index_of_case);
}

// Test cases
Case cases[] = {
    Case("Set trusted: Blob with an empty delegation list", blob_without_delegations),
    Case("Set trusted: Blob with non-empty delegation list", blob_with_delegations),
    Case("Set trusted: Malformed blob signature", malformed_blob_signature),
    Case("Set trusted: Malformed delegation signature", malformed_delegation_signature),
    Case("Set trusted: Malformed blob signature size", malformed_blob_signature_size),
    Case("Set trusted: Malformed blob signature size", malformed_delegation_signature_size),
    Case("Set trusted: Delegation size too short", delegation_size_too_short),
    Case("Set trusted: Delegation size too long", delegation_size_too_long),
    Case("Set trusted: Wrong nonce #1", wrong_nonce),
    Case("Set trusted: Wrong nonce #2", wrong_nonce2),
    Case("Set trusted: Replay same blob", replay_blob),
    //Case("Set trusted: Nonce timeout", nonce_timeout),
    Case("Set normal: Forward time, no storage update", storage_setup, normal_set_forward_no_storage_update),
    Case("Set normal: Forward time, with storage update #1", storage_setup, normal_set_forward_with_storage_update),
    Case("Set normal: Forward time, with storage update #2", storage_setup, normal_set_forward_with_storage_update2),
    Case("Set normal: Backward time, no clock drift", storage_setup, normal_set_backward_no_drift),
    Case("Set normal: Backward time, clock drift", storage_setup, normal_set_backward_with_drift)
};

utest::v1::status_t test_setup(const size_t number_of_cases)
{
   // Setup Greentea using a reasonable timeout in seconds
#ifndef NO_GREENTEA
   GREENTEA_SETUP(120, "default_auto");
#endif
   return verbose_test_setup_handler(number_of_cases);
}

Specification specification(test_setup, cases);

int main()
{
#ifdef ENABLE_LIBGCOV_PORT
    on_exit(collect_coverage, NULL);
    static_init();
#endif
   !Harness::run(specification);
   return 0;
}
