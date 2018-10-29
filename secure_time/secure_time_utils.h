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
#ifndef __SECURE_TIME_INTERNAL_H__
#define __SECURE_TIME_INTERNAL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*< Maximal allowed blob size in bytes. */
#define SECURE_TIME_MAX_BLOB_SIZE_BYTES                 (10 * 1024UL)

/*< Timestamp size in bytes. */
#define SECURE_TIME_TIMESTAMP_SIZE_BYTES                (8UL)

/*< Nonce size in bytes. */
#define SECURE_TIME_NONCE_SIZE_BYTES                    (8UL)

/*< The size of delegation length in bytes. */
#define SECURE_TIME_DELEGATION_LENGTH_SIZE_BYTES        (2UL)

/*< The size of public key length field in bytes. */
#define SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES            (2UL)

/*< The size of signature length field in bytes. */
#define SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES         (2UL)

/*< The size of public key length field in bytes. */
#define SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES            (2UL)

/*< The size of the constant length blob header. */
#define SECURE_TIME_BLOB_HEADER_SIZE_BYTES              \
    (                                                   \
    SECURE_TIME_TIMESTAMP_SIZE_BYTES +                  \
    SECURE_TIME_NONCE_SIZE_BYTES +                      \
    SECURE_TIME_DELEGATION_LENGTH_SIZE_BYTES            \
    )

/*< The location of the delegation length field in the blob. */
#define SECURE_TIME_DELEGATION_LENGTH_OFFSET            \
    (                                                   \
    SECURE_TIME_TIMESTAMP_SIZE_BYTES +                  \
    SECURE_TIME_NONCE_SIZE_BYTES                        \
    )

#define SECURE_TIME_MIN_RTC_LATENCY_SEC                 (100UL)

/*
 * Return the device boot time in seconds since EPOCH.
 *
 * @return 64-bit value of seconds.
 */
uint64_t secure_time_get_boot_time(void);

/*
 * Update the device boot time according to the new time.
 *
 * @param[in] new_time  Time value in seconds since EPOCH to update boot time.\n
 *                      updated_boot_time = new_time - seconds_since_last_boot
 */
void secure_time_update_boot_time(uint64_t new_time);

/*
 * Return how many seconds have passed since last boot.
 *
 * @return 64-bit value of seconds.
 */
uint64_t secure_time_get_seconds_since_boot(void);

/*
 * Update the device RTC time according to the new time.
 *
 * @param[in] new_time  Time value in seconds since EPOCH.
 */
void secure_time_update_rtc_time(uint64_t new_time);

/*
 * Return the device RTC time in seconds since EPOCH.
 *
 * @return 64-bit value of seconds, 0 if RTC not supported.
 */
uint64_t secure_time_get_rtc_time(void);

#ifdef __cplusplus
}
#endif

#endif // __SECURE_TIME_INTERNAL_H__
