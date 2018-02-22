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

#include "mbedtls/pk_info.h"
#include "ATCAConstants.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Setup opaque PK context for ATCA crypto engine.
 *
 * @param ctx       Key context
 * @param keyId     Private key Id representing a hardware Key.
 *
 * @retval          0 if successful, or appropriate Mbed TLS error code.
 */
int mbedtls_atca_pk_setup( mbedtls_pk_context * ctx, ATCAKeyID keyId );

/** Setup transparent PK context using Public key from ATCA crypto engine.
 *
 * @param ctx       Key context
 * @param keyId     Private key Id representing a hardware Key.
 *
 * @retval          0 if successful, or appropriate Mbed TLS error code.
 */
int mbedtls_atca_transparent_pk_setup( mbedtls_pk_context * ctx,
                                       ATCAKeyID keyId );

/** Encode ECC Public key in octet string as defined in SEC1 §2.3.3–2.3.4.
 *
 *  @param ecc_pk       ECC Public key input.
 *  @param asn_out      ASN.1 out buffer.
 *  @param asn_out_len  Out buffer length.
 *  @return             0 on success, -1 on failure
 */
int ecc_key_to_octet_string( uint8_t * ecc_pk, uint8_t * asn_out, size_t asn_len,
                     size_t * asn_out_len );

#ifdef __cplusplus
}
#endif

