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

#include "secure_time_utils.h"
#include "secure_time_impl.h"
#include "secure_time_client_spe.h"
#include "secure_time_storage.h"
#include "secure_time_crypto.h"
#include "mbed_error.h"
#include <string.h>

#if SECURE_TIME_ENABLED

#define SECURE_TIME_NONCE_GENERATION_TIME_INVALID       UINT64_MAX

#define EXTRACT_UINT16(buf) ((((uint16_t)(((uint8_t *)(buf))[1])) << 8) + (uint16_t)(((uint8_t *)(buf))[0]))

/*
 * Enumeration for the possible directions for setting the time.
 */
typedef enum {
    SECURE_TIME_FORWARD = 1,
    SECURE_TIME_BACKWARDS = 2
} SecureTimeDirection;

/*
 * Structure used during delegation record parsiong.
 */
typedef struct delegation_record_info {
    const uint8_t *record_start;
    uint16_t pubkey_size;
    const uint8_t *pubkey;
    uint16_t signature_size;
    const uint8_t *signature;
} delegation_record_info_t;

/*
 * Structure containing context of the NONCE.
 */
typedef struct nonce_ctx {
    uint64_t generation_time;  /* Timestamp of last generated NONCE. */
    uint64_t nonce;            /* The last generated nonce. */
} nonce_data_t;

static nonce_data_t g_trusted_time_nonce = {
    .generation_time = SECURE_TIME_NONCE_GENERATION_TIME_INVALID,
    .nonce = 0
};

static void invalidate_nonce(nonce_data_t *nonce_data)
{
    nonce_data->generation_time = SECURE_TIME_NONCE_GENERATION_TIME_INVALID;
    nonce_data->nonce = 0;
}

static bool is_nonce_valid(const nonce_data_t *nonce)
{
    return (SECURE_TIME_NONCE_GENERATION_TIME_INVALID != nonce->generation_time);
}

static int32_t extract_nonce_and_verify(const uint8_t *blob, size_t blob_size)
{
    if (!is_nonce_valid(&g_trusted_time_nonce)) {
        return SECURE_TIME_NONCE_MISSING;
    }

    uint64_t blob_nonce;
    memcpy(&blob_nonce, blob + SECURE_TIME_TIMESTAMP_SIZE_BYTES, SECURE_TIME_NONCE_SIZE_BYTES);

    if (blob_nonce != g_trusted_time_nonce.nonce) {
        return SECURE_TIME_NONCE_NOT_MATCH;
    }

    if ((secure_time_get_impl() - g_trusted_time_nonce.generation_time) > SECURE_TIME_NONCE_TIMEOUT_SECONDS) {
        // If invalidation timeout expired, invalidate SPE nonce.
        invalidate_nonce(&g_trusted_time_nonce);
        return SECURE_TIME_NONCE_TIMEOUT;
    }

    return SECURE_TIME_SUCCESS;
}

static size_t parse_delegation_record(
    const uint8_t *record_ptr,
    uint32_t bytes_left,
    delegation_record_info_t *record_info
    )
{
    record_info->record_start = record_ptr;

    // Make sure the delegation record is big enough for the public key length field.
    if (bytes_left < SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES) {
        return 0;
    }

    // Extract the public key length.
    record_info->pubkey_size = EXTRACT_UINT16(record_ptr);
    bytes_left -= SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES;
    record_ptr += SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES;

    // Make sure there're enough bytes left for the key.
    if (record_info->pubkey_size == 0 || record_info->pubkey_size > bytes_left) {
        return 0;
    }

    // Remember the key location.
    record_info->pubkey = record_ptr;
    bytes_left -= record_info->pubkey_size;
    record_ptr += record_info->pubkey_size;

    // Make sure the delegation record is big enough for the signature length field.
    if (bytes_left < SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES) {
        return 0;
    }

    // Extract the signature length.
    record_info->signature_size = EXTRACT_UINT16(record_ptr);
    bytes_left -= SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;
    record_ptr += SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;
    
    // Make sure there're enough bytes left for the signature.
    if (record_info->signature_size == 0 || record_info->signature_size > bytes_left) {
        return 0;
    }

    // Remember the signature location.
    record_info->signature = record_ptr;
    record_ptr += record_info->signature_size;
    
    // Return the record size.
    return record_ptr - record_info->record_start;
}

static int32_t verify_blob(
    const uint8_t *blob,
    size_t blob_size,
    const uint8_t *ca_pubkey,
    size_t ca_pubkey_size
    )
{
    int rc = SECURE_TIME_SUCCESS;

    // Make sure that the blob is big enough to contain at least the header.
    if (blob_size < SECURE_TIME_BLOB_HEADER_SIZE_BYTES || blob_size > SECURE_TIME_MAX_BLOB_SIZE_BYTES) {
        return SECURE_TIME_INVALID_BLOB_SIZE;
    }

    // Extract the size of the delegation section.
    uint16_t delegation_size = EXTRACT_UINT16(blob + SECURE_TIME_DELEGATION_LENGTH_OFFSET);

    // Make sure the delegation section followed by the signature length field fits within the blob.
    if (SECURE_TIME_BLOB_HEADER_SIZE_BYTES +
        delegation_size +
        SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES > blob_size) {
        return SECURE_TIME_INVALID_BLOB_SIZE;
    }

    // Go over all the keys in the delegation records of the blob and verify
    // their signatures.
    const uint8_t *delegation_start = blob + SECURE_TIME_BLOB_HEADER_SIZE_BYTES;
    const uint8_t *delegation_end = delegation_start + delegation_size;
    uint32_t bytes_left = 0;
    delegation_record_info_t prev_record_info = {0};
    prev_record_info.pubkey = ca_pubkey;
    prev_record_info.pubkey_size = ca_pubkey_size;
    delegation_record_info_t cur_record_info = {0};
    size_t cur_record_size = 0;

    while (delegation_start < delegation_end) {
        bytes_left = delegation_end - delegation_start;

        cur_record_size = parse_delegation_record(delegation_start, bytes_left, &cur_record_info);
        if (cur_record_size == 0) {
            return SECURE_TIME_INVALID_BLOB_SIZE;
        }

        rc = secure_time_verify_signature(
            cur_record_info.record_start,
            cur_record_info.pubkey_size + SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES,
            cur_record_info.signature,
            cur_record_info.signature_size,
            prev_record_info.pubkey,
            prev_record_info.pubkey_size
            );

        if (SECURE_TIME_SUCCESS != rc) {
            return SECURE_TIME_SIGNATURE_VERIFICATION_FAILED;
        }

        delegation_start += cur_record_size;
        prev_record_info = cur_record_info;
    }

    // Extract the size of the blob's signature.
    uint16_t blob_signature_size = EXTRACT_UINT16(delegation_end);
    const uint8_t *blob_signature = delegation_end + SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES;

    // Make sure the number of the remaining bytes is exactly as the blob's signature size.
    if ((blob_size -
        SECURE_TIME_BLOB_HEADER_SIZE_BYTES -
        delegation_size -
        SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES) != blob_signature_size) {
            return SECURE_TIME_INVALID_BLOB_SIZE;
        }

    // Verify the overall blob signature.
    // At this stage prev_record_info contains the public key that should be used for the verification.
    rc = secure_time_verify_signature(
        blob,
        SECURE_TIME_BLOB_HEADER_SIZE_BYTES + delegation_size,
        blob_signature,
        blob_signature_size,
        prev_record_info.pubkey,
        prev_record_info.pubkey_size
        );

    if (SECURE_TIME_SUCCESS != rc) {
        return SECURE_TIME_SIGNATURE_VERIFICATION_FAILED;
    }

    return rc;
}

int32_t secure_time_set_trusted_init_impl(uint64_t *nonce)
{
    if (!nonce) {
        error("nonce is NULL!");
    }

    // Invalidate any existing nonce
    invalidate_nonce(&g_trusted_time_nonce);

    secure_time_generate_random_bytes(sizeof(uint64_t), &g_trusted_time_nonce.nonce);
    *nonce = g_trusted_time_nonce.nonce;
    g_trusted_time_nonce.generation_time = secure_time_get_impl();
    return SECURE_TIME_SUCCESS;
}

int32_t secure_time_set_trusted_commit_impl(const void *blob, size_t blob_size)
{
    int rc = SECURE_TIME_SUCCESS;
    if (!blob || (0 == blob_size)) {
        error("blob is NULL or size 0!");
    }

    // Read the CA public key from secure storage.
    size_t ca_pubkey_size = 0;

    rc = secure_time_get_stored_public_key_size(&ca_pubkey_size);
    if (SECURE_TIME_SUCCESS != rc) {
        error("Failed to read the CA public key size! (rc=%d)", rc);
    }
    uint8_t *ca_pubkey = (uint8_t *)malloc(ca_pubkey_size);
    if (!ca_pubkey) {
        error("Failed to allocate memory for CA public key data!");
    }
    rc = secure_time_get_stored_public_key(ca_pubkey, ca_pubkey_size, &ca_pubkey_size);
    if (SECURE_TIME_SUCCESS != rc) {
        error("Failed to read the CA public key! (rc=%d)", rc);
    }

    // Verify the blob's correctness.
    rc = verify_blob(blob, blob_size, ca_pubkey, ca_pubkey_size);
    free(ca_pubkey);    

    if (SECURE_TIME_SUCCESS == rc) {
        // Extract the new time value from the blob according to the schema.
        uint64_t new_time;
        memcpy(&new_time, blob, SECURE_TIME_TIMESTAMP_SIZE_BYTES);

        // Extract the nonce from the blob and verify its' correctness and freshness.
        // In case the the nonce is different than the last generated nonce or is too old,
        // the call is ignored.
        rc = extract_nonce_and_verify((const uint8_t *)blob, blob_size);
        if (SECURE_TIME_SUCCESS == rc) {
            // Get current RTC time.
            uint64_t rtc_time = (uint64_t)time(NULL);

            // Set RTC with new time if it is around 1-2 minutes forward/backward
            // than current RTC time.
            if(llabs(new_time - rtc_time) > SECURE_TIME_MIN_RTC_LATENCY_SEC) {
                secure_time_update_rtc_time(new_time);
            }

            // Read the current stored time from secure storage.
            uint64_t stored_time = 0;
            secure_time_get_stored_time(&stored_time);

            SecureTimeDirection direction = (new_time > stored_time) ?
                                            SECURE_TIME_FORWARD :
                                            SECURE_TIME_BACKWARDS;
            bool set_storage = false;

            // If new time is less than 1 day forward or less than 1-2 minutes backwards
            // do not update secure storage.
            if (SECURE_TIME_FORWARD == direction) {
                set_storage = (new_time - stored_time) > SECURE_TIME_MIN_STORAGE_FORWARD_LATENCY_SEC;
            } else {
                set_storage = (stored_time - new_time) > SECURE_TIME_MIN_STORAGE_BACKWARD_LATENCY_SEC;
            }

            if (set_storage) {
                // Write the new time to secure storage entry of last backwards time.
                secure_time_set_stored_back_time(new_time);

                // Write the new time to secure storage entry of current stored time.
                secure_time_set_stored_time(new_time);
            }

            // Update the SPE delta value as the new time minus current SPE tick count.
            secure_time_update_boot_time(new_time);

            // Invalidate nonce
            invalidate_nonce(&g_trusted_time_nonce);
        }
    }

    return rc;
}

static void set_time_forward(uint64_t new_time, uint64_t curr_os_time)
{
    // Update the SPE delta value as the new time minus current SPE tick count.
    secure_time_update_boot_time(new_time);

    // Set RTC with new time if it is around 1-2 minutes forward than current time.
    uint64_t rtc_time = secure_time_get_rtc_time();
    if((new_time - rtc_time) > SECURE_TIME_MIN_RTC_LATENCY_SEC) {
        secure_time_update_rtc_time(new_time);
    }

    // Write new time to secure storage entry of current stored time if it's more than 1 day forward
    // than current OS time.
    bool set_storage = (new_time - curr_os_time) > SECURE_TIME_MIN_STORAGE_FORWARD_LATENCY_SEC;
    if (set_storage) {
        secure_time_set_stored_time(new_time);
    }
}

static int32_t set_time_backwards(uint64_t new_time, uint64_t curr_os_time)
{
    uint64_t stored_back_time = 0;
    secure_time_get_stored_back_time(&stored_back_time);
    bool stored_back_time_exist = (stored_back_time > 0);

    // For each day between stored_back_time and new_time we can move backwards by up to 3 minutes:
    // Which is same as up to 480 seconds for each second in this interval.
    // So: (A) MAX_BACK_SECONDS = (new_time - stored_back_time)/480
    //     (B) (curr_os_time - new_time) <= MAX_BACK_SECONDS
    //     (A & B) (curr_os_time - new_time) <= (new_time - stored_back_time)/480
    bool set_back = ( stored_back_time_exist &&
                      (new_time > stored_back_time) &&
                      ((curr_os_time - new_time) <= (new_time - stored_back_time)/480) );
    if (set_back) {
        // Update the SPE delta value as the new time minus current SPE tick count.
        secure_time_update_boot_time(new_time);

        // Write the new time to secure storage entry of last backwards time and current stored time.
        secure_time_set_stored_back_time(new_time);
        secure_time_set_stored_time(new_time);
        return SECURE_TIME_SUCCESS;
    }
    return SECURE_TIME_NOT_ALLOWED;
}

int32_t secure_time_set_impl(uint64_t new_time)
{
    // Get the current time in the device.
    uint64_t curr_os_time = secure_time_get_impl();
    SecureTimeDirection direction = (new_time > curr_os_time) ?
                                    SECURE_TIME_FORWARD :
                                    SECURE_TIME_BACKWARDS;

    if (SECURE_TIME_FORWARD == direction) {
        set_time_forward(new_time, curr_os_time);
    } else {
        return set_time_backwards(new_time, curr_os_time);
    }

    uint64_t stored_time = 0;
    secure_time_get_stored_time(&stored_time);

    // Write the new time to secure storage entry of current stored time
    // if new time is more than around 5-6 days forward than current stored time.
    if ( (new_time > stored_time) &&
         ((new_time - stored_time) > SECURE_TIME_MIN_STORAGE_IDLE_LATENCY_SEC) ) {
        secure_time_set_stored_time(new_time);
    }
    return SECURE_TIME_SUCCESS;
}

#define SECURE_TIME_MAX(A, B)   ((A) > (B) ? (A) : (B))

uint64_t secure_time_get_impl(void)
{
    uint64_t boot_time = secure_time_get_boot_time();
    uint64_t secs_since_boot = secure_time_get_seconds_since_boot();

    // If boot_time is valid (not 0), we can return boot_time + secs_since_boot as current time.
    // Otherwise, the best estimation we have is the latest between the RTC and the stored time.
    if (boot_time > 0) {
        return boot_time + secs_since_boot;
    } else {
        // Read the current stored time from secure storage
        uint64_t stored_time = 0;
        secure_time_get_stored_time(&stored_time);

        // Get current RTC time
        uint64_t rtc_time = secure_time_get_rtc_time();
        
        // Set new time according to the latest between the RTC and the stored time
        uint64_t new_time = SECURE_TIME_MAX(stored_time, rtc_time);

        // Update the latest boot time value for next calculations
        secure_time_update_boot_time(new_time);

        return new_time;
    }
}

#endif // SECURE_TIME_ENABLED
