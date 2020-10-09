/*
 * pppcrypt.c - PPP/DES linkage for MS-CHAP and EAP SRP-SHA1
 *
 * Extracted from chap_ms.c by James Carlson.
 *
 * Copyright (c) 1995 Eric Rosenquist.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name(s) of the authors of this software must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission.
 *
 * THE AUTHORS OF THIS SOFTWARE DISCLAIM ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "ppp_opts.h"
#if PPP_SUPPORT /* don't build if not configured for use in ppp_opts.h */

/* This header file is included in all PPP modules needing hashes and/or ciphers */

#ifndef PPPCRYPT_H
#define	PPPCRYPT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Map hashes and ciphers functions to PolarSSL
 */
#if !PPP_USE_EXTERNAL_MBEDTLS

#include "polarssl/md4.h"
#define MD4_context md4_context
#define MD4_init(context)
#define MD4_starts md4_starts
#define MD4_update md4_update
#define MD4_finish md4_finish
#define MD4_free(context)

#include "polarssl/md5.h"
#define MD5_context md5_context
#define MD5_init(context)
#define MD5_starts md5_starts
#define MD5_update md5_update
#define MD5_finish md5_finish
#define MD5_free(context)

#include "polarssl/sha1.h"
#define SHA1_context sha1_context
#define SHA1_init(context)
#define SHA1_starts sha1_starts
#define SHA1_update sha1_update
#define SHA1_finish sha1_finish
#define SHA1_free(context)

#include "polarssl/des.h"
#define Des_context des_context
#define Des_init(context)
#define Des_setkey_enc des_setkey_enc
#define Des_crypt_ecb des_crypt_ecb
#define Des_free(context)

#include "polarssl/arc4.h"
#define ARC4_context arc4_context
#define ARC4_init(context)
#define ARC4_setup arc4_setup
#define ARC4_crypt arc4_crypt
#define ARC4_free(context)

#endif /* !PPP_USE_EXTERNAL_MBEDTLS */

/*
 * Map hashes and ciphers functions to mbed TLS
 */
#if PPP_USE_EXTERNAL_MBEDTLS

#include "mbedtls/md5.h"

#define MD4_context mbedtls_md4_context
#define MD4_init mbedtls_md4_init
#define MD4_starts mbedtls_md4_starts
#define MD4_update mbedtls_md4_update
#define MD4_finish mbedtls_md4_finish
#define MD4_free mbedtls_md4_free

#define MD5_context mbedtls_md5_context
#define MD5_init mbedtls_md5_init
#define MD5_starts mbedtls_md5_starts
#define MD5_update mbedtls_md5_update
#define MD5_finish mbedtls_md5_finish
#define MD5_free mbedtls_md5_free

#define SHA1_context mbedtls_sha1_context
#define SHA1_init mbedtls_sha1_init
#define SHA1_starts mbedtls_sha1_starts
#define SHA1_update mbedtls_sha1_update
#define SHA1_finish mbedtls_sha1_finish
#define SHA1_free mbedtls_sha1_free

#define Des_context mbedtls_des_context
#define Des_init mbedtls_des_init
#define Des_setkey_enc mbedtls_des_setkey_enc
#define Des_crypt_ecb mbedtls_des_crypt_ecb
#define Des_free mbedtls_des_free

#define ARC4_context mbedtls_arc4_context
#define ARC4_init mbedtls_arc4_init
#define ARC4_setup mbedtls_arc4_setup
#define ARC4_crypt(context, buffer, length) mbedtls_arc4_crypt(context, length, buffer, buffer)
#define ARC4_free mbedtls_arc4_free

#endif /* PPP_USE_EXTERNAL_MBEDTLS */

void pppcrypt_56_to_64_bit_key(u_char *key, u_char *des_key);

#ifdef __cplusplus
}
#endif

#endif /* PPPCRYPT_H */

#endif /* PPP_SUPPORT */
