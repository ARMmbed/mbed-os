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
#ifndef MBED_DEVICEKEY_H
#define MBED_DEVICEKEY_H

#include "stddef.h"
#include "stdint.h"
#include "platform/NonCopyable.h"

#if (NVSTORE_ENABLED) || defined(DOXYGEN_ONLY)

namespace mbed {
/** \addtogroup drivers */

#define DEVICE_KEY_16BYTE 16
#define DEVICE_KEY_32BYTE 32

enum DeviceKeyStatus {
    DEVICEKEY_SUCCESS                     =  0,
    DEVICEKEY_INVALID_KEY_SIZE            = -1,
    DEVICEKEY_INVALID_KEY_TYPE            = -2,
    DEVICEKEY_SAVE_FAILED                 = -3,
    DEVICEKEY_ALREADY_EXIST               = -4,
    DEVICEKEY_NOT_FOUND                   = -5,
    DEVICEKEY_READ_FAILED                 = -6,
    DEVICEKEY_NVSTORE_UNPREDICTED_ERROR   = -7,
    DEVICEKEY_ERR_CMAC_GENERIC_FAILURE    = -8,
    DEVICEKEY_BUFFER_TOO_SMALL            = -9,
    DEVICEKEY_NO_KEY_INJECTED             = -10,
    DEVICEKEY_INVALID_PARAM               = -11,
    DEVICEKEY_TRNG_ERROR                  = -12,
};

/** Use this singleton if you need to derive a new key from the device root of trust.
 *
 * @note Synchronization level: Thread safe
 * @ingroup drivers
 */

class DeviceKey : private mbed::NonCopyable<DeviceKey> {
public:

    /**
     * @brief As a singleton, return the single instance of the class.
     *        Reason for this class being a singleton is the following:
     *        - Ease the use for users of this class not having to coordinate instantiations.
     *        - Lazy instantiation of internal data (which we can't achieve with simple static classes).
     *
     * @returns Singleton instance reference.
     */
    static DeviceKey& get_instance()
    {
        // Use this implementation of singleton (Meyer's) rather than the one that allocates
        // the instance on the heap, as it ensures destruction at program end (preventing warnings
        // from memory checking tools such as valgrind).
        static DeviceKey instance;
        return instance;
    }

    ~DeviceKey();

    /** Derive a new key based on the salt string. key type can be with values 16 bytes and 32 bytes
     * @param isalt input buffer used to create the new key. Same input will generate always the same key
     * @param isalt_size size of the data in salt buffer
     * @param output buffer to receive the derived key. Size must be 16 bytes or 32 bytes
     *               according to the ikey_type parameter
     * @param ikey_type type of the required key. Type must be 16 bytes or 32 bytes.
     * @return 0 on success, negative error code on failure
     */
    int generate_derived_key(const unsigned char *isalt, size_t isalt_size, unsigned char *output, uint16_t ikey_type);

    /** Set a device key into the NVStore. In case TRNG support is missing, Call this method
     *  before calling device_key_derived_key. This method should be called only once!
     * @param value input buffer contain the key.
     * @param isize size of the supplied key. Must be 16 bytes or 32 bytes.
     * @return 0 on success, negative error code on failure
     */
    int device_inject_root_of_trust(uint32_t *value, size_t isize);

private:
    // Private constructor, as class is a singleton
    DeviceKey();

    /** Read a device key from the NVStore
     * @param output buffer for the returned key.
     * @param size input: the size of the output buffer.
     *             output: the actual size of the written data
     * @return 0 on success, negative error code on failure
     */
    int read_key_from_nvstore(uint32_t *output, size_t& size);

    /** Set a device key into the NVStore
     * @param input input buffer contain the key.
     * @param isize the size of the input buffer.
     * @return 0 on success, negative error code on failure
     */
    int write_key_to_nvstore(uint32_t *input, size_t isize);

    /** Get a derived key base on a salt string. The methods implements
     * Section 5.1 in NIST SP 800-108, Recommendation for Key Derivation Using Pseudorandom Functions
     * @param ikey_buff input buffer holding the ROT key
     * @param ikey_size size of the input key. Must be 16 bytes or 32 bytes.
     * @param isalt input buffer contain some string.
     * @param isalt_size size of the supplied input string.
     * @param output buffer for the derived key result.
     * @param ikey_type the requested key size. Must be 16 bytes or 32 bytes.
     * @return 0 on success, negative error code on failure
     */
    int get_derived_key(uint32_t *ikey_buff, size_t ikey_size, const unsigned char *isalt, size_t isalt_size,
                        unsigned char *output, uint32_t ikey_type);

    /** Generate a random ROT key by using TRNG
     * @param output output buffer for the generated key.
     * @param size input: the size of the buffer. if size is less
     *                    then 16 bytes the method will generate an
     *                    error. 16-31 bytes will create a 16 byte key.
     *                    32 or higher will generate a 32 bytes key
     *             output: the actual written size to the buffer
     * @return 0 on success, negative error code on failure
     */
    int generate_key_by_trng(uint32_t *output, size_t size);

};
/** @}*/

}

#endif //NVSTORE_ENABLED
#endif
