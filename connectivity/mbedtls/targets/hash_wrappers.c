#include "mbedtls/md2.h"
#include "mbedtls/md4.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

#if defined(MBEDTLS_MD2_ALT) && defined(MBEDTLS_MD2_C)

/**
 * \brief          MD2 context setup
 *
 * \deprecated     Superseded by mbedtls_md2_starts_ret() in 2.7.0
 *
 * \param ctx      context to be initialized
 *
 * \warning        MD2 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md2_starts( mbedtls_md2_context *ctx )
{
    mbedtls_md2_starts_ret( ctx );
}

/**
 * \brief          MD2 process buffer
 *
 * \deprecated     Superseded by mbedtls_md2_update_ret() in 2.7.0
 *
 * \param ctx      MD2 context
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 *
 * \warning        MD2 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md2_update( mbedtls_md2_context *ctx,
                         const unsigned char *input,
                         size_t ilen )
{
    mbedtls_md2_update_ret( ctx, input, ilen );
}

/**
 * \brief          MD2 final digest
 *
 * \deprecated     Superseded by mbedtls_md2_finish_ret() in 2.7.0
 *
 * \param ctx      MD2 context
 * \param output   MD2 checksum result
 *
 * \warning        MD2 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md2_finish( mbedtls_md2_context *ctx,
                         unsigned char output[16] )
{
    mbedtls_md2_finish_ret( ctx, output );
}

#endif /* defined(MBEDTLS_MD2_ALT) */

#if defined(MBEDTLS_MD4_ALT) && defined(MBEDTLS_MD4_C)

/**
 * \brief          MD4 context setup
 *
 * \deprecated     Superseded by mbedtls_md4_starts_ret() in 2.7.0
 *
 * \param ctx      context to be initialized
 *
 * \warning        MD4 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md4_starts( mbedtls_md4_context *ctx )
{
    mbedtls_md4_starts_ret( ctx );
}

/**
 * \brief          MD4 process buffer
 *
 * \deprecated     Superseded by mbedtls_md4_update_ret() in 2.7.0
 *
 * \param ctx      MD4 context
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 *
 * \warning        MD4 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md4_update( mbedtls_md4_context *ctx,
                         const unsigned char *input,
                         size_t ilen )
{
    mbedtls_md4_update_ret( ctx, input, ilen );
}

/**
 * \brief          MD4 final digest
 *
 * \deprecated     Superseded by mbedtls_md4_finish_ret() in 2.7.0
 *
 * \param ctx      MD4 context
 * \param output   MD4 checksum result
 *
 * \warning        MD4 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md4_finish( mbedtls_md4_context *ctx,
                         unsigned char output[16] )
{
    mbedtls_md4_finish_ret( ctx, output );
}

#endif /* defined(MBEDTLS_MD4_ALT) */

#if defined(MBEDTLS_MD5_ALT) && defined(MBEDTLS_MD5_C)

/**
 * \brief          MD5 context setup
 *
 * \deprecated     Superseded by mbedtls_md5_starts_ret() in 2.7.0
 *
 * \param ctx      context to be initialized
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md5_starts( mbedtls_md5_context *ctx )
{
    mbedtls_md5_starts_ret( ctx );
}

/**
 * \brief          MD5 process buffer
 *
 * \deprecated     Superseded by mbedtls_md5_update_ret() in 2.7.0
 *
 * \param ctx      MD5 context
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md5_update( mbedtls_md5_context *ctx,
                         const unsigned char *input,
                         size_t ilen )
{
    mbedtls_md5_update_ret( ctx, input, ilen );
}

/**
 * \brief          MD5 final digest
 *
 * \deprecated     Superseded by mbedtls_md5_finish_ret() in 2.7.0
 *
 * \param ctx      MD5 context
 * \param output   MD5 checksum result
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md5_finish( mbedtls_md5_context *ctx,
                         unsigned char output[16] )
{
    mbedtls_md5_finish_ret( ctx, output );
}

#endif /* defined(MBEDTLS_MD5_ALT) */

#if defined(MBEDTLS_SHA1_ALT) && defined(MBEDTLS_SHA1_C)

/**
 * \brief          SHA-1 context setup
 *
 * \deprecated     Superseded by mbedtls_sha1_starts_ret() in 2.7.0
 *
 * \param ctx      The SHA-1 context to be initialized.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_sha1_starts( mbedtls_sha1_context *ctx )
{
    mbedtls_sha1_starts_ret( ctx );
}

/**
 * \brief          SHA-1 process buffer
 *
 * \deprecated     Superseded by mbedtls_sha1_update_ret() in 2.7.0
 *
 * \param ctx      The SHA-1 context.
 * \param input    The buffer holding the input data.
 * \param ilen     The length of the input data.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_sha1_update( mbedtls_sha1_context *ctx,
                          const unsigned char *input,
                          size_t ilen )
{
    mbedtls_sha1_update_ret( ctx, input, ilen );
}

/**
 * \brief          SHA-1 final digest
 *
 * \deprecated     Superseded by mbedtls_sha1_finish_ret() in 2.7.0
 *
 * \param ctx      The SHA-1 context.
 * \param output   The SHA-1 checksum result.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_sha1_finish( mbedtls_sha1_context *ctx,
                          unsigned char output[20] )
{
    mbedtls_sha1_finish_ret( ctx, output );
}

#endif /* defined(MBEDTLS_SHA1_ALT) */

#if defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C)

/**
 * \brief          This function starts a SHA-256 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha256_starts_ret() in 2.7.0.
 *
 * \param ctx      The SHA-256 context to initialize.
 * \param is224    Determines which function to use.
 *                 <ul><li>0: Use SHA-256.</li>
 *                 <li>1: Use SHA-224.</li></ul>
 */
void mbedtls_sha256_starts( mbedtls_sha256_context *ctx,
                            int is224 )
{
    mbedtls_sha256_starts_ret( ctx, is224 );
}

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SHA-256 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha256_update_ret() in 2.7.0.
 *
 * \param ctx      The SHA-256 context to initialize.
 * \param input    The buffer holding the data.
 * \param ilen     The length of the input data.
 */
void mbedtls_sha256_update( mbedtls_sha256_context *ctx,
                            const unsigned char *input,
                            size_t ilen )
{
    mbedtls_sha256_update_ret( ctx, input, ilen );
}

/**
 * \brief          This function finishes the SHA-256 operation, and writes
 *                 the result to the output buffer.
 *
 * \deprecated     Superseded by mbedtls_sha256_finish_ret() in 2.7.0.
 *
 * \param ctx      The SHA-256 context.
 * \param output   The SHA-224or SHA-256 checksum result.
 */
void mbedtls_sha256_finish( mbedtls_sha256_context *ctx,
                            unsigned char output[32] )
{
    mbedtls_sha256_finish_ret( ctx, output );
}

#endif /* defined(MBEDTLS_SHA256_ALT) */

#if defined(MBEDTLS_SHA512_ALT) && defined(MBEDTLS_SHA512_C)

/**
 * \brief          This function starts a SHA-384 or SHA-512 checksum
 *                 calculation.
 *
 * \deprecated     Superseded by mbedtls_sha512_starts_ret() in 2.7.0
 *
 * \param ctx      The SHA-512 context to initialize.
 * \param is384    Determines which function to use.
 *                 <ul><li>0: Use SHA-512.</li>
 *                 <li>1: Use SHA-384.</li></ul>
 */
void mbedtls_sha512_starts( mbedtls_sha512_context *ctx,
                            int is384 )
{
    mbedtls_sha512_starts_ret( ctx, is384 );
}

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SHA-512 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha512_update_ret() in 2.7.0
 *
 * \param ctx      The SHA-512 context.
 * \param input    The buffer holding the data.
 * \param ilen     The length of the input data.
 */
void mbedtls_sha512_update( mbedtls_sha512_context *ctx,
                            const unsigned char *input,
                            size_t ilen )
{
    mbedtls_sha512_update_ret( ctx, input, ilen );
}

/**
 * \brief          This function finishes the SHA-512 operation, and writes
 *                 the result to the output buffer.
 *
 * \deprecated     Superseded by mbedtls_sha512_finish_ret() in 2.7.0
 *
 * \param ctx      The SHA-512 context.
 * \param output   The SHA-384 or SHA-512 checksum result.
 */
void mbedtls_sha512_finish( mbedtls_sha512_context *ctx,
                            unsigned char output[64] )
{
    mbedtls_sha512_finish_ret( ctx, output );
}

#endif /* defined(MBEDTLS_SHA512_ALT) */

