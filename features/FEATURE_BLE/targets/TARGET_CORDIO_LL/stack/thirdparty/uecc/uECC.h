/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _MICRO_ECC_H_
#define _MICRO_ECC_H_

#include "wsf_types.h"

/* Platform selection options.
If uECC_PLATFORM is not defined, the code will try to guess it based on compiler macros.
Possible values for uECC_PLATFORM are defined below: */
#define uECC_arch_other 0
#define uECC_x86        1
#define uECC_x86_64     2
#define uECC_arm        3
#define uECC_arm_thumb  4
#define uECC_avr        5
#define uECC_arm_thumb2 6

/* If desired, you can define uECC_WORD_SIZE as appropriate for your platform (1, 4, or 8 bytes).
If uECC_WORD_SIZE is not explicitly defined then it will be automatically set based on your
platform. */

/* Inline assembly options.
uECC_asm_none  - Use standard C99 only.
uECC_asm_small - Use GCC inline assembly for the target platform (if available), optimized for
                 minimum size.
uECC_asm_fast  - Use GCC inline assembly optimized for maximum speed. */
#define uECC_asm_none  0
#define uECC_asm_small 1
#define uECC_asm_fast  2
#ifndef uECC_ASM
    #define uECC_ASM uECC_asm_fast
#endif

/* Curve selection options. */
#define uECC_secp160r1 1
#define uECC_secp192r1 2
#define uECC_secp256r1 3
#define uECC_secp256k1 4
#define uECC_secp224r1 5
#ifndef uECC_CURVE
    #define uECC_CURVE uECC_secp256r1
#endif

/* uECC_SQUARE_FUNC - If enabled (defined as nonzero), this will cause a specific function to be
used for (scalar) squaring instead of the generic multiplication function. This will make things
faster by about 8% but increases the code size. */
#ifndef uECC_SQUARE_FUNC
    #define uECC_SQUARE_FUNC 1
#endif

#define uECC_CONCAT1(a, b) a##b
#define uECC_CONCAT(a, b) uECC_CONCAT1(a, b)

#define uECC_size_1 20 /* secp160r1 */
#define uECC_size_2 24 /* secp192r1 */
#define uECC_size_3 32 /* secp256r1 */
#define uECC_size_4 32 /* secp256k1 */
#define uECC_size_5 28 /* secp224r1 */

#define uECC_BYTES uECC_CONCAT(uECC_size_, uECC_CURVE)

#ifdef __cplusplus
extern "C"
{
#endif

/* uECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1 if
'dest' was filled with random data, or 0 if the random data could not be generated.
The filled-in values should be either truly random, or from a cryptographically-secure PRNG.

A correctly functioning RNG function must be set (using uECC_set_rng()) before calling
uECC_make_key() or uECC_sign().

Setting a correctly functioning RNG function improves the resistance to side-channel attacks
for uECC_shared_secret() and uECC_sign_deterministic().

A correct RNG function is set by default when building for Windows, Linux, or OS X.
If you are building on another POSIX-compliant system that supports /dev/random or /dev/urandom,
you can define uECC_POSIX to use the predefined RNG. For embedded platforms there is no predefined
RNG function; you must provide your own.
*/
typedef int (*uECC_RNG_Function)(uint8_t *dest, unsigned size);

/* uECC_set_rng() function.
Set the function that will be used to generate random bytes. The RNG function should
return 1 if the random data was generated, or 0 if the random data could not be generated.

On platforms where there is no predefined RNG function (eg embedded platforms), this must
be called before uECC_make_key() or uECC_sign() are used.

Inputs:
    rng_function - The function that will be used to generate random bytes.
*/
void uECC_set_rng(uECC_RNG_Function rng_function);

/* uECC_make_key() function.
Create a public/private key pair.

Outputs:
    public_key  - Will be filled in with the public key.
    private_key - Will be filled in with the private key.

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
int uECC_make_key(uint8_t public_key[uECC_BYTES*2], uint8_t private_key[uECC_BYTES]);

/* uECC_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key.
Note: It is recommended that you hash the result of uECC_shared_secret() before using it for
symmetric encryption or HMAC.

Inputs:
    public_key  - The public key of the remote party.
    private_key - Your private key.

Outputs:
    secret - Will be filled in with the shared secret value.

Returns 1 if the shared secret was generated successfully, 0 if an error occurred.
*/
int uECC_shared_secret(const uint8_t public_key[uECC_BYTES*2],
                       const uint8_t private_key[uECC_BYTES],
                       uint8_t secret[uECC_BYTES]);

/* uECC_sign() function.
Generate an ECDSA signature for a given hash value.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and pass it in to
this function along with your private key.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.

Outputs:
    signature - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int uECC_sign(const uint8_t private_key[uECC_BYTES],
              const uint8_t message_hash[uECC_BYTES],
              uint8_t signature[uECC_BYTES*2]);

/* uECC_HashContext structure.
This is used to pass in an arbitrary hash function to uECC_sign_deterministic().
The structure will be used for multiple hash computations; each time a new hash
is computed, init_hash() will be called, followed by one or more calls to
update_hash(), and finally a call to finish_hash() to prudoce the resulting hash.

The intention is that you will create a structure that includes uECC_HashContext
followed by any hash-specific data. For example:

typedef struct SHA256_HashContext {
    uECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

void init_SHA256(uECC_HashContext *base) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Init(&context->ctx);
}

void update_SHA256(uECC_HashContext *base,
                   const uint8_t *message,
                   unsigned message_size) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Update(&context->ctx, message, message_size);
}

void finish_SHA256(uECC_HashContext *base, uint8_t *hash_result) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Final(hash_result, &context->ctx);
}

... when signing ...
{
    uint8_t tmp[32 + 32 + 64];
    SHA256_HashContext ctx = {{&init_SHA256, &update_SHA256, &finish_SHA256, 64, 32, tmp}};
    uECC_sign_deterministic(key, message_hash, &ctx.uECC, signature);
}
*/
typedef struct uECC_HashContext {
    void (*init_hash)(struct uECC_HashContext *context);
    void (*update_hash)(struct uECC_HashContext *context,
                        const uint8_t *message,
                        unsigned message_size);
    void (*finish_hash)(struct uECC_HashContext *context, uint8_t *hash_result);
    unsigned block_size; /* Hash function block size in bytes, eg 64 for SHA-256. */
    unsigned result_size; /* Hash function result size in bytes, eg 32 for SHA-256. */
    uint8_t *tmp; /* Must point to a buffer of at least (2 * result_size + block_size) bytes. */
} uECC_HashContext;

/* uECC_sign_deterministic() function.
Generate an ECDSA signature for a given hash value, using a deterministic algorithm
(see RFC 6979). You do not need to set the RNG using uECC_set_rng() before calling
this function; however, if the RNG is defined it will improve resistance to side-channel
attacks.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and pass it in to
this function along with your private key and a hash context.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_context - A hash context to use.

Outputs:
    signature - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int uECC_sign_deterministic(const uint8_t private_key[uECC_BYTES],
                            const uint8_t message_hash[uECC_BYTES],
                            uECC_HashContext *hash_context,
                            uint8_t signature[uECC_BYTES*2]);

/* uECC_verify() function.
Verify an ECDSA signature.

Usage: Compute the hash of the signed data using the same hash as the signer and
pass it to this function along with the signer's public key and the signature values (r and s).

Inputs:
    public_key - The signer's public key
    hash       - The hash of the signed data.
    signature  - The signature value.

Returns 1 if the signature is valid, 0 if it is invalid.
*/
int uECC_verify(const uint8_t private_key[uECC_BYTES*2],
                const uint8_t hash[uECC_BYTES],
                const uint8_t signature[uECC_BYTES*2]);

/* uECC_compress() function.
Compress a public key.

Inputs:
    public_key - The public key to compress.

Outputs:
    compressed - Will be filled in with the compressed public key.
*/
void uECC_compress(const uint8_t public_key[uECC_BYTES*2], uint8_t compressed[uECC_BYTES+1]);

/* uECC_decompress() function.
Decompress a compressed public key.

Inputs:
    compressed - The compressed public key.

Outputs:
    public_key - Will be filled in with the decompressed public key.
*/
void uECC_decompress(const uint8_t compressed[uECC_BYTES+1], uint8_t public_key[uECC_BYTES*2]);

/* uECC_valid_public_key() function.
Check to see if a public key is valid.

Note that you are not required to check for a valid public key before using any other uECC
functions. However, you may wish to avoid spending CPU time computing a shared secret or
verifying a signature using an invalid public key.

Inputs:
    public_key - The public key to check.

Returns 1 if the public key is valid, 0 if it is invalid.
*/
int uECC_valid_public_key(const uint8_t public_key[uECC_BYTES*2]);

/* uECC_compute_public_key() function.
Compute the corresponding public key for a private key.

Inputs:
    private_key - The private key to compute the public key for

Outputs:
    public_key - Will be filled in with the corresponding public key

Returns 1 if the key was computed successfully, 0 if an error occurred.
*/
int uECC_compute_public_key(const uint8_t private_key[uECC_BYTES],
                            uint8_t public_key[uECC_BYTES * 2]);


/* uECC_bytes() function.
Returns the value of uECC_BYTES. Helpful for foreign-interfaces to higher-level languages.
*/
int uECC_bytes(void);

/* uECC_curve() function.
Returns the value of uECC_CURVE. Helpful for foreign-interfaces to higher-level languages.
*/
int uECC_curve(void);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _MICRO_ECC_H_ */
