/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _MICRO_ECC_LL_H_
#define _MICRO_ECC_LL_H_

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
void uECC_set_rng_ll(uECC_RNG_Function rng_function);

/* uECC_make_key() function.
Create a public/private key pair.

Outputs:
    public_key  - Will be filled in with the public key.
    private_key - Will be filled in with the private key.

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
void uECC_make_key_start(const uint8_t private_key[uECC_BYTES]);
int uECC_make_key_continue(void);
void uECC_make_key_complete(uint8_t public_key[uECC_BYTES*2], uint8_t private_key[uECC_BYTES]);

/* uECC_valid_public_key() function.
Check to see if a public key is valid.

Note that you are not required to check for a valid public key before using any other uECC
functions. However, you may wish to avoid spending CPU time computing a shared secret or
verifying a signature using an invalid public key.

Inputs:
    public_key - The public key to check.

Returns 1 if the public key is valid, 0 if it is invalid.
*/
int uECC_valid_public_key_ll(const uint8_t public_key[uECC_BYTES*2]);

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
void uECC_shared_secret_start(const uint8_t public_key[uECC_BYTES*2],
                              const uint8_t private_key[uECC_BYTES]);
int uECC_shared_secret_continue(void);
void uECC_shared_secret_complete(uint8_t secret[uECC_BYTES]);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _MICRO_ECC_LL_H_ */
