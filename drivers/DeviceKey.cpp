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

#include "drivers/DeviceKey.h"
#include "mbedtls/config.h"
#include "mbedtls/cmac.h"
#include "nvstore.h"
#include "trng_api.h"

#if !defined(MBEDTLS_CMAC_C)
#error [NOT_SUPPORTED] MBEDTLS_CMAC_C needs to be enabled for this driver
#else

#if NVSTORE_ENABLED

namespace mbed {

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
        return DEVICEKEY_NVSTORE_UNPREDICTABLE_ERROR;
    }

    return DEVICEKEY_SUCCESS;
}

int DeviceKey::read_key_from_nvstore(uint32_t *output, size_t& size)
{
    if (size > UINT16_MAX) {
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
        return DEVICEKEY_NVSTORE_UNPREDICTABLE_ERROR;
    }

    size = out_size;
    return DEVICEKEY_SUCCESS;
}

// Calculate CMAC functions - wrapper for mbedtls start/update and finish
int DeviceKey::calculate_cmac(const unsigned char *input, size_t isize, uint32_t *ikey_buff, int ikey_size,
                              unsigned char *output)
{
    int ret;
    mbedtls_cipher_context_t ctx;

    mbedtls_cipher_type_t mbedtls_cipher_type = MBEDTLS_CIPHER_AES_128_ECB;
    if (DEVICE_KEY_32BYTE == ikey_size) {
        mbedtls_cipher_type = MBEDTLS_CIPHER_AES_256_ECB;
    }

    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(mbedtls_cipher_type);

    mbedtls_cipher_init(&ctx);
    ret = mbedtls_cipher_setup(&ctx, cipher_info);
    if (ret != 0) {
        goto finish;
    }

    ret = mbedtls_cipher_cmac_starts(&ctx, (unsigned char *)ikey_buff, ikey_size * 8);
    if (ret != 0) {
        goto finish;
    }

    ret = mbedtls_cipher_cmac_update(&ctx, input, isize);
    if (ret != 0) {
        goto finish;
    }

    ret = mbedtls_cipher_cmac_finish(&ctx, output);
    if (ret != 0) {
        goto finish;
    }

    return DEVICEKEY_SUCCESS;

finish:
    mbedtls_cipher_free( &ctx );
    return ret;
}

int DeviceKey::get_derived_key(uint32_t *ikey_buff, size_t ikey_size, const unsigned char *isalt,
                               size_t isalt_size, unsigned char *output, uint32_t ikey_type)
{
    int ret;
    unsigned char *double_size_salt = NULL;

    if (DEVICE_KEY_16BYTE == ikey_type) {
        ret = calculate_cmac(isalt, isalt_size, ikey_buff, ikey_size, output);
        if (DEVICEKEY_SUCCESS != ret) {
            goto finish;
        }
    }

    if (DEVICE_KEY_32BYTE == ikey_type) {
        ret = this->calculate_cmac(isalt, isalt_size, ikey_buff, ikey_size, output);
        if (DEVICEKEY_SUCCESS != ret) {
            goto finish;
        }

        //Double the salt size cause cmac always return just 16 bytes
        double_size_salt = new unsigned char[isalt_size * 2];
        memcpy(double_size_salt, isalt, isalt_size);
        memcpy(double_size_salt + isalt_size, isalt, isalt_size);

        ret = this->calculate_cmac(double_size_salt, isalt_size * 2, ikey_buff, ikey_size, output + 16);
    }

finish:
    if (double_size_salt != NULL) {
        delete[] double_size_salt;
    }

    if (DEVICEKEY_SUCCESS != ret) {
        return DEVICEKEY_ERR_CMAC_GENERIC_FAILURE;
    }

    return DEVICEKEY_SUCCESS;
}

int DeviceKey::generate_key_by_trng(uint32_t *output, size_t& size)
{
#if defined(DEVICE_TRNG)
    size_t in_size;
    trng_t trng_obj;

    memset(output, 0, size);

    if (DEVICE_KEY_16BYTE > size) {
        return DEVICEKEY_BUFFER_TO_SMALL;
    } else if (DEVICE_KEY_16BYTE <= size && DEVICE_KEY_32BYTE > size) {
        in_size = DEVICE_KEY_16BYTE;
    } else {
        in_size = DEVICE_KEY_32BYTE;
    }

    trng_init(&trng_obj);

    int ret = trng_get_bytes(&trng_obj, (unsigned char *)output, in_size, &size);
    if (DEVICEKEY_SUCCESS != ret || in_size != size) {
        return DEVICEKEY_TRNG_ERROR;
    }

    trng_free(&trng_obj);
    return DEVICEKEY_SUCCESS;
#else
    return DEVICEKEY_NO_KEY_INJECTED;
#endif
}

} // namespace mbed

#endif //NVSTORE_ENABLED
#endif


