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

#ifndef ATCAKEY_H
#define ATCAKEY_H

#include "ATCADevice.h"

/** Key object for Public key crypto.
 */
class ATCAKey {
private:
    ATCADevice &    device;
    ATCAKeyID       keyId;
    uint8_t         pk[ATCA_ECC_ECC_PK_LEN];
public:
    /** Create a key instance.
     */
    ATCAKey(ATCADevice & dev, ATCAKeyID keyId_in, uint8_t * pk);

    /** Sign function
     */
    ATCAError Sign( const uint8_t * hash, size_t hash_len, uint8_t * sig,
                    size_t sig_buf_len, size_t * sig_len);
    /** Verify function
     */
    ATCAError Verify( const unsigned char *hash, size_t hash_len,
                      const unsigned char *sig,
                      size_t sig_len );
};

#endif /* ATCAKEY_H */

