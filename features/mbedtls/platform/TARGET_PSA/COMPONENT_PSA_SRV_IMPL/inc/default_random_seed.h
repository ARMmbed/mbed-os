

#ifndef DEFAULT_RANDOM_SEED_H
#define DEFAULT_RANDOM_SEED_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif



/** Read seed from the secure storage.
 *
 *  This function will be the default function for reading the Random seed.
 *
 *   @param buf[out] buffer to hold the seed value from the secure storage
 *   @param buf_len[in] input buffer length
 *
 * @returns
 * secure storage API return value.
 *
 */
int mbed_default_seed_read(unsigned char *buf, size_t buf_len);

/** Writes seed to the secure storage.
 *
 *  This function will be the default function for writing the Random seed.
 *
 *   @param buf[in] buffer to the seed value
 *   @param buf_len[in] input buffer length
 *
 * @returns
 * secure storage API return value.
 */
int mbed_default_seed_write(unsigned char *buf, size_t buf_len);


#ifdef __cplusplus
}
#endif

#endif /* DEFAULT_RANDOM_SEED_H */
