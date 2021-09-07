/**
 * \file mbedtls_svc_key_id.h
 *
 * Excerpted from Mbed TLS for internal use by Mbed TLS's PSK key exchange to
 * interface with generic PSA Crypto implementations.
 *
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef MBEDTLS_SVC_KEY_ID_H
#define MBEDTLS_SVC_KEY_ID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef psa_key_id_t mbedtls_svc_key_id_t;

#define MBEDTLS_SVC_KEY_ID_INIT ( (psa_key_id_t)0 )

/** Check whether a key identifier is null.
 *
 * \param key Key identifier.
 *
 * \return Non-zero if the key identifier is null, zero otherwise.
 */
static inline int mbedtls_svc_key_id_is_null( mbedtls_svc_key_id_t key )
{
    return( key == 0 );
}

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SVC_KEY_ID_H */
