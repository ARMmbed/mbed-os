#include "secure_time_test_utils.h"
#include "secure_time_client_spe.h"
#include "unity.h"
#include <string.h>

#include "mbedtls/pk.h"
#include "mbedtls/md.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include <stdio.h>

mbedtls_entropy_context entropy = {0};

void provision_data(
    const uint8_t *ca_pubkey,
    size_t ca_pubkey_size
    )
{
    TEST_ASSERT_EQUAL_HEX(0, secure_time_set_stored_public_key(ca_pubkey, ca_pubkey_size));
}

static void sign_data(
    const uint8_t *data,
    size_t data_size,
    const uint8_t *prvkey,
    size_t prvkey_size,
    uint8_t *sign,
    size_t *sign_size
    )
{
    mbedtls_pk_context pk = {0};
    mbedtls_pk_init(&pk);
    TEST_ASSERT_EQUAL_HEX(0, mbedtls_pk_parse_key(&pk, prvkey, prvkey_size, NULL, 0));

    mbedtls_md_context_t md_ctx = {0};
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    // The below variables were made static to prevent stack overflow when compiling with ARMCC.
    static unsigned char hash[MBEDTLS_MD_MAX_SIZE];
    static mbedtls_ctr_drbg_context ctr_drbg;
    memset(hash, 0, sizeof(hash));
    memset(&ctr_drbg, 0, sizeof(ctr_drbg));

    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(md_type);
    TEST_ASSERT_NOT_NULL(md_info);

    mbedtls_md_init(&md_ctx);
    TEST_ASSERT_EQUAL_HEX(0, mbedtls_md_setup(&md_ctx, md_info, 0));
    TEST_ASSERT_EQUAL_HEX(0, mbedtls_md_starts(&md_ctx));
    TEST_ASSERT_EQUAL_HEX(0, mbedtls_md_update(&md_ctx, data, data_size));
    TEST_ASSERT_EQUAL_HEX(0, mbedtls_md_finish(&md_ctx, hash));
    mbedtls_md_free(&md_ctx);

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    TEST_ASSERT_EQUAL_HEX(0, mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0));

    TEST_ASSERT_EQUAL_HEX(0, mbedtls_pk_sign(&pk, md_type, hash, 0, sign, sign_size, mbedtls_ctr_drbg_random, &ctr_drbg));
}

static uint8_t signature[SECURE_TIME_TESTS_MAX_SIGN_SIZE_BYTES] = {0}; // define it global static to avoid stack overflow

static size_t create_delegation_record(
    const uint8_t *delegation_pubkey,
    size_t delegation_pubkey_size,
    const uint8_t *prvkey,
    size_t prvkey_size,
    uint8_t *record_buffer,
    size_t record_buffer_size
)
{
    uint8_t *delegation_record_start = record_buffer;
    size_t signature_size = 0;

    size_t record_size = SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES;
    TEST_ASSERT(record_size <= record_buffer_size);
    record_buffer[0] = (uint8_t)(delegation_pubkey_size & 0xff);
    record_buffer[1] = (uint8_t)(delegation_pubkey_size >> 8);
    record_buffer += SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES;

    record_size += delegation_pubkey_size;
    TEST_ASSERT(record_size <= record_buffer_size);
    memcpy(record_buffer, delegation_pubkey, delegation_pubkey_size);
    record_buffer += delegation_pubkey_size;

    sign_data(
        delegation_record_start,
        SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES + delegation_pubkey_size,
        prvkey,
        prvkey_size,
        signature,
        &signature_size
        );
    TEST_ASSERT(SECURE_TIME_TESTS_MAX_SIGN_SIZE_BYTES >= signature_size);

    record_size += SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;
    TEST_ASSERT(record_size <= record_buffer_size);
    record_buffer[0] = (uint8_t)(signature_size & 0xff);
    record_buffer[1] = (uint8_t)(signature_size >> 8);
    record_buffer += SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;

    record_size += signature_size;
    TEST_ASSERT(record_size <= record_buffer_size);
    memcpy(record_buffer, signature, signature_size);

    return record_size;
}

size_t create_blob(
    uint64_t time,
    uint64_t nonce,
    const data_buffer_t *delegation_pubkeys,
    const data_buffer_t *prvkeys,
    uint32_t delegation_list_length,
    uint8_t *blob_buffer,
    size_t blob_buffer_size
    )
{
    uint32_t blob_size = 0;

    // Make sure the necessary buffers aren't NULL (delegation_pubkeys is allowed to be NULL).
    TEST_ASSERT(NULL != prvkeys);
    TEST_ASSERT(NULL != blob_buffer);

    // Construct the blob.
    uint8_t *blob_start = blob_buffer;
    size_t signature_size = 0;

    // Add the timestamp top the blob.
    blob_size += SECURE_TIME_TIMESTAMP_SIZE_BYTES;
    TEST_ASSERT(blob_size <= blob_buffer_size);
    memcpy(blob_buffer, &time, SECURE_TIME_TIMESTAMP_SIZE_BYTES);
    blob_buffer += SECURE_TIME_TIMESTAMP_SIZE_BYTES;

    // Add nonce to the blob.
    blob_size += SECURE_TIME_NONCE_SIZE_BYTES;
    TEST_ASSERT(blob_size <= blob_buffer_size);
    memcpy(blob_buffer, &nonce, SECURE_TIME_NONCE_SIZE_BYTES);
    blob_buffer += SECURE_TIME_NONCE_SIZE_BYTES;

    // Reserve space in the blob for the overall delegation record size which is still unknown.
    blob_size += SECURE_TIME_DELEGATION_LENGTH_SIZE_BYTES;
    TEST_ASSERT(blob_size <= blob_buffer_size);
    uint8_t *blob_delegation_length = blob_buffer;
    blob_buffer += SECURE_TIME_DELEGATION_LENGTH_SIZE_BYTES;

    // Add all the delegation records to the blob.
    uint8_t *delegation_section_start = blob_buffer;
    uint32_t i = 0;
    size_t delegation_record_length = 0;
    for (i = 0; delegation_pubkeys != NULL && i < delegation_list_length; i++) {
        delegation_record_length = create_delegation_record(
            delegation_pubkeys[i].data,
            delegation_pubkeys[i].size,
            prvkeys[i].data,
            prvkeys[i].size,
            blob_buffer,
            blob_buffer_size - blob_size
            );
        blob_size += delegation_record_length;
        blob_buffer += delegation_record_length;
    }

    // Write the overall delegation section size to the blob.
    uint16_t delegation_length = blob_buffer - delegation_section_start;
    blob_delegation_length[0] = (uint8_t)(delegation_length & 0xff);
    blob_delegation_length[1] = (uint8_t)(delegation_length >> 8);

    // Sign the blob.
    sign_data(
        blob_start,
        blob_size,
        prvkeys[i].data,
        prvkeys[i].size,
        signature,
        &signature_size
        );
    TEST_ASSERT(SECURE_TIME_TESTS_MAX_SIGN_SIZE_BYTES >= signature_size);

    blob_size += SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;
    TEST_ASSERT(blob_size <= blob_buffer_size);
    blob_buffer[0] = (uint8_t)(signature_size & 0xff);
    blob_buffer[1] = (uint8_t)(signature_size >> 8);
    blob_buffer += SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;

    blob_size += signature_size;
    TEST_ASSERT(blob_size <= blob_buffer_size);
    memcpy(blob_buffer, signature, signature_size);

    return blob_size;
}
