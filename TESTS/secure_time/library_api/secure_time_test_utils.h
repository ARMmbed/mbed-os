#ifndef __SECURE_TIME_TEST_UTILS__
#define __SECURE_TIME_TEST_UTILS__

#include <stdint.h>
#include <stdlib.h>

#define SECURE_TIME_TESTS_MAX_SIGN_SIZE_BYTES     (128UL)
#define SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES     (1024UL)

typedef struct data_buffer {
    const uint8_t *data;
    size_t size;
} data_buffer_t;

void provision_data(
    const uint8_t *ca_pubkey,
    size_t ca_pubkey_size
    );

size_t create_blob(
    uint64_t time,
    uint64_t nonce,
    const data_buffer *delegation_pubkeys,
    const data_buffer *prvkeys,
    uint32_t pub_key_number,
    uint8_t *blob_buffer,
    size_t blob_buffer_size
    );

#endif // __SECURE_TIME_TEST_UTILS__
