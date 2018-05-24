/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "DeviceKey.h"
#include "mbedtls/config.h"
#include "mbedtls/cmac.h"
#include "nvstore.h"
#include "trng_api.h"
#include "mbed_wait_api.h"
#include "stdlib.h"

#if !defined(MBEDTLS_CMAC_C)
#error [NOT_SUPPORTED] MBEDTLS_CMAC_C needs to be enabled for this driver
#else

#if NVSTORE_ENABLED

namespace mbed {

#define DEVKEY_WRITE_UINT32_LE( dst, src )                              \
    do                                                                  \
    {                                                                   \
        (dst)[0] = ( (src) >> 0 ) & 0xFF;                               \
        (dst)[1] = ( (src) >> 8 ) & 0xFF;                               \
        (dst)[2] = ( (src) >> 16 ) & 0xFF;                              \
        (dst)[3] = ( (src) >> 24 ) & 0xFF;                              \
    } while( 0 )

#define DEVKEY_WRITE_UINT8_LE( dst, src )                               \
    do                                                                  \
    {                                                                   \
        (dst)[0] = (src) & 0xFF;                                        \
    } while( 0 )


DeviceKey::DeviceKey()
{
    return;
}

DeviceKey::~DeviceKey()
{
    return;
}

int DeviceKey::generate_derived_key(const unsigned char *salt, size_t isalt_size, unsigned char *output,
                                    uint16_t ikey_type)
{
    uint32_t key_buff[DEVICE_KEY_32BYTE / sizeof(uint32_t)];
    size_t actual_size = DEVICE_KEY_32BYTE;

    if (DEVICE_KEY_16BYTE != ikey_type && DEVICE_KEY_32BYTE != ikey_type) {
        return DEVICEKEY_INVALID_KEY_TYPE;
    }

    //First try to read the key from NVStore
    int ret = read_key_from_nvstore(key_buff, actual_size);
    if (DEVICEKEY_SUCCESS != ret && DEVICEKEY_NOT_FOUND != ret) {
        return ret;
    }

    if (DEVICE_KEY_16BYTE != actual_size && DEVICE_KEY_32BYTE != actual_size) {
        return DEVICEKEY_READ_FAILED;
    }

    //If the key was not found in NVStore we will create it by using TRNG and then save it to NVStore
    if (DEVICEKEY_NOT_FOUND == ret) {
        ret = generate_key_by_trng(key_buff, actual_size);
        if (DEVICEKEY_SUCCESS != ret) {
            return ret;
        }

        ret = device_inject_root_of_trust(key_buff, actual_size);
        if (DEVICEKEY_SUCCESS != ret) {
            return ret;
        }
    }

    ret = get_derived_key(key_buff, actual_size, salt, isalt_size, output, ikey_type);
    return ret;
}

int DeviceKey::device_inject_root_of_trust(uint32_t *value, size_t isize)
{
    return write_key_to_nvstore(value, isize);
}

int DeviceKey::write_key_to_nvstore(uint32_t *input, size_t isize)
{
    if (DEVICE_KEY_16BYTE != isize && DEVICE_KEY_32BYTE != isize) {
        return DEVICEKEY_INVALID_KEY_SIZE;
    }

    //First we read if key exist. If it is exists, we return DEVICEKEY_ALREADY_EXIST error
    uint32_t read_key[DEVICE_KEY_32BYTE / sizeof(uint32_t)] = {0};
    size_t read_size = DEVICE_KEY_32BYTE;
    int ret = read_key_from_nvstore(read_key, read_size);
    if (DEVICEKEY_SUCCESS == ret) {
        return DEVICEKEY_ALREADY_EXIST;
    }
    if (DEVICEKEY_NOT_FOUND != ret) {
        return ret;
    }

    NVStore& nvstore = NVStore::get_instance();
    ret = nvstore.set(NVSTORE_DEVICEKEY_KEY, (uint16_t)isize, input);
    if (NVSTORE_WRITE_ERROR == ret || NVSTORE_BUFF_TOO_SMALL == ret) {
        return DEVICEKEY_SAVE_FAILED;
    }

    if (NVSTORE_SUCCESS != ret) {
        return DEVICEKEY_NVSTORE_UNPREDICTED_ERROR;
    }

    return DEVICEKEY_SUCCESS;
}

int DeviceKey::read_key_from_nvstore(uint32_t *output, size_t& size)
{
    if (size > (uint16_t)-1) {
        return DEVICEKEY_INVALID_PARAM;
    }

    uint16_t in_size = size;
    uint16_t out_size = 0;
    NVStore& nvstore = NVStore::get_instance();
    int nvStatus = nvstore.get(NVSTORE_DEVICEKEY_KEY, in_size, output, out_size);
    if (NVSTORE_NOT_FOUND == nvStatus) {
        return DEVICEKEY_NOT_FOUND;
    }

    if (NVSTORE_READ_ERROR == nvStatus || NVSTORE_BUFF_TOO_SMALL == nvStatus) {
        return DEVICEKEY_READ_FAILED;
    }

    if (NVSTORE_SUCCESS != nvStatus) {
        return DEVICEKEY_NVSTORE_UNPREDICTED_ERROR;
    }

    size = out_size;
    return DEVICEKEY_SUCCESS;
}

int DeviceKey::get_derived_key(uint32_t *ikey_buff, size_t ikey_size, const unsigned char *isalt,
                               size_t isalt_size, unsigned char *output, uint32_t ikey_type)
{
    //KDF in counter mode implementation as described in Section 5.1
    //of NIST SP 800-108, Recommendation for Key Derivation Using Pseudorandom Functions
    int ret;
    size_t counter = 0;
    char separator = 0x00;
    mbedtls_cipher_context_t ctx;
    unsigned char output_len_enc[ 4 ] = {0};
    unsigned char counter_enc[ 1 ] = {0};

    DEVKEY_WRITE_UINT32_LE(output_len_enc, ikey_type);

    mbedtls_cipher_type_t mbedtls_cipher_type = MBEDTLS_CIPHER_AES_128_ECB;
    if (DEVICE_KEY_32BYTE == ikey_size) {
        mbedtls_cipher_type = MBEDTLS_CIPHER_AES_256_ECB;
    }

    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(mbedtls_cipher_type);

    do {

    	mbedtls_cipher_init(&ctx);
    	ret = mbedtls_cipher_setup(&ctx, cipher_info);
    	if (ret != 0) {
    	    goto finish;
    	}

        ret = mbedtls_cipher_cmac_starts(&ctx, (unsigned char *)ikey_buff, ikey_size * 8);
        if (ret != 0) {
            goto finish;
        }

        DEVKEY_WRITE_UINT8_LE(counter_enc, (counter+1));

        ret = mbedtls_cipher_cmac_update(&ctx, (unsigned char *)counter_enc, sizeof(counter_enc));
        if (ret != 0) {
            goto finish;
        }

        ret = mbedtls_cipher_cmac_update(&ctx, isalt, isalt_size);
        if (ret != 0) {
            goto finish;
        }

        ret = mbedtls_cipher_cmac_update(&ctx, (unsigned char *)&separator, sizeof(char));
        if (ret != 0) {
            goto finish;
        }

        ret = mbedtls_cipher_cmac_update(&ctx, (unsigned char *)&output_len_enc, sizeof(output_len_enc));
        if (ret != 0) {
            goto finish;
        }

        ret = mbedtls_cipher_cmac_finish(&ctx, output + (DEVICE_KEY_16BYTE * (counter)));
        if (ret != 0) {
            goto finish;
        }

        mbedtls_cipher_free( &ctx );

        counter++;

    } while (DEVICE_KEY_16BYTE * counter < ikey_type);

finish:
    if (DEVICEKEY_SUCCESS != ret) {
    	mbedtls_cipher_free( &ctx );
        return DEVICEKEY_ERR_CMAC_GENERIC_FAILURE;
    }

    return DEVICEKEY_SUCCESS;
}

int DeviceKey::generate_key_by_trng(uint32_t *output, size_t size)
{
#if defined(DEVICE_TRNG)
    size_t in_size;
    size_t ongoing_size;
    trng_t trng_obj;
    int ret = DEVICEKEY_SUCCESS;
    unsigned char *pBuffer = (unsigned char *)output;

    memset(output, 0, size);

    if (DEVICE_KEY_16BYTE > size) {
        return DEVICEKEY_BUFFER_TOO_SMALL;
    } else if (DEVICE_KEY_16BYTE != size && DEVICE_KEY_32BYTE != size) {
        return DEVICEKEY_INVALID_PARAM;
    }

    trng_init(&trng_obj);

    in_size = size;
    while (in_size > 0) {

        ongoing_size = 0;
        ret = trng_get_bytes(&trng_obj, (unsigned char *)pBuffer, in_size, &ongoing_size);
        if (0 != ret || ongoing_size > in_size) {
            ret = DEVICEKEY_TRNG_ERROR;
            goto finish;
        }

        pBuffer += ongoing_size;
        in_size -= ongoing_size;
    }

    ret = DEVICEKEY_SUCCESS;

finish:
    trng_free(&trng_obj);
    return ret;

#else
    return DEVICEKEY_NO_KEY_INJECTED;
#endif
}

} // namespace mbed

#endif //NVSTORE_ENABLED
#endif


