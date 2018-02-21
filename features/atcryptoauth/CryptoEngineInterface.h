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

#ifndef CRYPTOENGINEINTERFACE_H
#define CRYPTOENGINEINTERFACE_H

#include <stdint.h>
#include <stddef.h>

/** Interface definition for a Crypto Engine.
 *
 *  Currently interface is only defined for sign and verify functions.
 */
class CryptoEngineInterface
{
public:
    /** Sign message digest(hash) and output signature R & S as concatenated
     *  buffer.
     *
     *  @param keyId    Key Id.
     *  @param hash     Message digest.
     *  @param len      Hash length.
     *  @param sig      Signature output buffer.
     *  @param sig_buf_len
     *                  Signature output buffer length.
     *  @param sig_len  Signature output length.
     *  @return         0 on success else error code.
     */
    virtual int Sign(uint32_t keyId, const uint8_t * hash, size_t len,
                   uint8_t * sig, size_t sig_buf_len, size_t * sig_len) = 0;

    /** Verify input signature against input message digest.
     *
     *  @param pk       Public key in form of concatenated X & Y buffer.
     *  @param pk_len   Public key length.
     *  @param sig      Signature buffer.
     *  @param sig_len  Signature buffer length.
     *  @param msg      Message buffer.
     *  @param msg_len  Message buffer length.
     *  @return         0 on success else error code.
     */
    virtual int Verify(uint8_t * pk, size_t pk_len, const uint8_t * sig,
                     size_t sig_len, const uint8_t * msg, size_t msg_len) = 0;
};

#endif /* CRYPTOENGINEINTERFACE_H */
