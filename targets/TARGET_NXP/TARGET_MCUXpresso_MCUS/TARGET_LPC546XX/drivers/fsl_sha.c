/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_sha.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*!< SHA-1 and SHA-256 block size  */
#define SHA_BLOCK_SIZE 64

/*!< Use standard C library memcpy  */
#define sha_memcpy memcpy

/*! Internal states of the HASH creation process */
typedef enum _sha_algo_state
{
    kSHA_HashInit = 1u, /*!< Init state, the NEW bit in SHA Control register has not been written yet. */
    kSHA_HashUpdate, /*!< Update state, DIGEST registers contain running hash, NEW bit in SHA control register has been
                        written. */
} sha_algo_state_t;

/*! 64-byte block represented as byte array of 16 32-bit words */
typedef union _sha_hash_block
{
    uint32_t w[SHA_BLOCK_SIZE / 4]; /*!< array of 32-bit words */
    uint8_t b[SHA_BLOCK_SIZE];      /*!< byte array */
} sha_block_t;

/*! internal sha context structure */
typedef struct _sha_ctx_internal
{
    sha_block_t blk;        /*!< memory buffer. only full 64-byte blocks are written to SHA during hash updates */
    size_t blksz;           /*!< number of valid bytes in memory buffer */
    sha_algo_t algo;        /*!< selected algorithm from the set of supported algorithms */
    sha_algo_state_t state; /*!< finite machine state of the hash software process */
    size_t fullMessageSize; /*!< track message size during SHA_Update(). The value is used for padding. */
} sha_ctx_internal_t;

/*!< SHA-1 and SHA-256 digest length in bytes  */
enum _sha_digest_len
{
    kSHA_OutLenSha1 = 20u,
    kSHA_OutLenSha256 = 32u,
};

/*!< macro for checking build time condition. It is used to assure the sha_ctx_internal_t can fit into sha_ctx_t */
#define BUILD_ASSERT(condition, msg) extern int msg[1 - 2 * (!(condition))] __attribute__((unused))

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief LDM to SHA engine INDATA and ALIAS registers.
 *
 * This function writes 16 words starting from the src address (must be word aligned)
 * to the dst address. Dst address does not increment (destination is peripheral module register INDATA).
 * Src address increments to load 16 consecutive words.
 *
 * @param dst peripheral register address (word aligned)
 * @param src address of the input 512-bit block (16 words) (word aligned)
 *
 */
__STATIC_INLINE void sha_ldm_stm_16_words(volatile uint32_t *dst, const uint32_t *src)
{
    for (int i = 0; i < 8; i++)
    {
        dst[i] = src[i];
    }
    src += 8u;
    for (int i = 0; i < 8; i++)
    {
        dst[i] = src[i];
    }
}

/*!
 * @brief Swap bytes withing 32-bit word.
 *
 * This function changes endianess of a 32-bit word.
 *
 * @param in 32-bit unsigned integer
 * @return 32-bit unsigned integer with different endianess (big endian to little endian and vice versa).
 */
static uint32_t swap_bytes(uint32_t in)
{
    return (((in & 0x000000ffu) << 24) | ((in & 0x0000ff00u) << 8) | ((in & 0x00ff0000u) >> 8) |
            ((in & 0xff000000u) >> 24));
}

/*!
 * @brief Check validity of algoritm.
 *
 * This function checks the validity of input argument.
 *
 * @param algo Tested algorithm value.
 * @return kStatus_Success if valid, kStatus_InvalidArgument otherwise.
 */
static status_t sha_check_input_alg(sha_algo_t algo)
{
    if ((algo != kSHA_Sha1) && (algo != kSHA_Sha256))
    {
        return kStatus_InvalidArgument;
    }
    return kStatus_Success;
}

/*!
 * @brief Check validity of input arguments.
 *
 * This function checks the validity of input arguments.
 *
 * @param base SHA peripheral base address.
 * @param ctx Memory buffer given by user application where the SHA_Init/SHA_Update/SHA_Finish store context.
 * @param algo Tested algorithm value.
 * @return kStatus_Success if valid, kStatus_InvalidArgument otherwise.
 */
static status_t sha_check_input_args(SHA_Type *base, sha_ctx_t *ctx, sha_algo_t algo)
{
    /* Check validity of input algorithm */
    if (kStatus_Success != sha_check_input_alg(algo))
    {
        return kStatus_InvalidArgument;
    }

    if ((NULL == ctx) || (NULL == base))
    {
        return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

/*!
 * @brief Check validity of internal software context.
 *
 * This function checks if the internal context structure looks correct.
 *
 * @param ctxInternal Internal context.
 * @param message Input message address.
 * @return kStatus_Success if valid, kStatus_InvalidArgument otherwise.
 */
static status_t sha_check_context(sha_ctx_internal_t *ctxInternal, const uint8_t *message)
{
    if ((NULL == message) || (NULL == ctxInternal) || (kStatus_Success != sha_check_input_alg(ctxInternal->algo)))
    {
        return kStatus_InvalidArgument;
    }
    return kStatus_Success;
}

/*!
 * @brief Initialize the SHA engine for new hash.
 *
 * This function sets NEW and MODE fields in SHA Control register to start new hash.
 *
 * @param base SHA peripheral base address.
 * @param ctxInternal Internal context.
 */
static void sha_engine_init(SHA_Type *base, sha_ctx_internal_t *ctxInternal)
{
    uint32_t shaCtrl;

    if (kSHA_Sha1 == ctxInternal->algo)
    {
        shaCtrl = SHA_CTRL_MODE(1) | SHA_CTRL_NEW(1);
    }
    else
    {
        shaCtrl = SHA_CTRL_MODE(2) | SHA_CTRL_NEW(1);
    }
    base->CTRL = shaCtrl;
}

/*!
 * @brief Load 512-bit block (16 words) into SHA engine.
 *
 * This function aligns the input block and moves it into SHA engine INDATA.
 * CPU polls the WAITING bit and then moves data by using LDM and STM instructions.
 *
 * @param base SHA peripheral base address.
 * @param blk 512-bit block
 */
static void sha_one_block(SHA_Type *base, const uint8_t *blk)
{
    uint32_t temp[SHA_BLOCK_SIZE / sizeof(uint32_t)];
    const uint32_t *actBlk;

    /* make sure the 512-bit block is word aligned */
    if ((uintptr_t)blk & 0x3u)
    {
        sha_memcpy(temp, blk, SHA_BLOCK_SIZE);
        actBlk = (const uint32_t *)(uintptr_t)temp;
    }
    else
    {
        actBlk = (const uint32_t *)(uintptr_t)blk;
    }

    /* poll waiting. */
    while (0 == (base->STATUS & SHA_STATUS_WAITING_MASK))
    {
    }
    /* feed INDATA (and ALIASes). use STM instruction. */
    sha_ldm_stm_16_words(&base->INDATA, actBlk);
}

/*!
 * @brief Adds message to current hash.
 *
 * This function merges the message to fill the internal buffer, empties the internal buffer if
 * it becomes full, then process all remaining message data.
 *
 *
 * @param base SHA peripheral base address.
 * @param ctxInternal Internal context.
 * @param message Input message.
 * @param messageSize Size of input message in bytes.
 * @return kStatus_Success.
 */
static status_t sha_process_message_data(SHA_Type *base,
                                         sha_ctx_internal_t *ctxInternal,
                                         const uint8_t *message,
                                         size_t messageSize)
{
    /* first fill the internal buffer to full block */
    size_t toCopy = SHA_BLOCK_SIZE - ctxInternal->blksz;
    sha_memcpy(&ctxInternal->blk.b[ctxInternal->blksz], message, toCopy);
    message += toCopy;
    messageSize -= toCopy;

    /* process full internal block */
    sha_one_block(base, &ctxInternal->blk.b[0]);

    /* process all full blocks in message[] */
    while (messageSize >= SHA_BLOCK_SIZE)
    {
        sha_one_block(base, message);
        message += SHA_BLOCK_SIZE;
        messageSize -= SHA_BLOCK_SIZE;
    }

    /* copy last incomplete message bytes into internal block */
    sha_memcpy(&ctxInternal->blk.b[0], message, messageSize);
    ctxInternal->blksz = messageSize;
    return kStatus_Success;
}

/*!
 * @brief Finalize the running hash to make digest.
 *
 * This function empties the internal buffer, adds padding bits, and generates final digest.
 *
 * @param base SHA peripheral base address.
 * @param ctxInternal Internal context.
 * @return kStatus_Success.
 */
static status_t sha_finalize(SHA_Type *base, sha_ctx_internal_t *ctxInternal)
{
    sha_block_t lastBlock;

    memset(&lastBlock, 0, sizeof(sha_block_t));

    /* this is last call, so need to flush buffered message bytes along with padding */
    if (ctxInternal->blksz <= 55u)
    {
        /* last data is 440 bits or less. */
        sha_memcpy(&lastBlock.b[0], &ctxInternal->blk.b[0], ctxInternal->blksz);
        lastBlock.b[ctxInternal->blksz] = (uint8_t)0x80U;
        lastBlock.w[SHA_BLOCK_SIZE / 4 - 1] = swap_bytes(8u * ctxInternal->fullMessageSize);
        sha_one_block(base, &lastBlock.b[0]);
    }
    else
    {
        if (ctxInternal->blksz < SHA_BLOCK_SIZE)
        {
            ctxInternal->blk.b[ctxInternal->blksz] = (uint8_t)0x80U;
            for (uint32_t i = ctxInternal->blksz + 1u; i < SHA_BLOCK_SIZE; i++)
            {
                ctxInternal->blk.b[i] = 0;
            }
        }
        else
        {
            lastBlock.b[0] = (uint8_t)0x80U;
        }

        sha_one_block(base, &ctxInternal->blk.b[0]);
        lastBlock.w[SHA_BLOCK_SIZE / 4 - 1] = swap_bytes(8u * ctxInternal->fullMessageSize);
        sha_one_block(base, &lastBlock.b[0]);
    }
    /* poll wait for final digest */
    while (0 == (base->STATUS & SHA_STATUS_DIGEST_MASK))
    {
    }
    return kStatus_Success;
}

/*!
 * @brief Read DIGEST registers.
 *
 * This function copies DIGEST to output buffer.
 *
 * @param base SHA peripheral base address.
 * @param[out] output Output buffer.
 * @param Number of bytes to copy.
 * @return kStatus_Success.
 */
static void sha_get_digest(SHA_Type *base, uint8_t *output, size_t outputSize)
{
    uint32_t digest[8];

    for (int i = 0; i < 8; i++)
    {
        digest[i] = swap_bytes(base->DIGEST[i]);
    }

    if (outputSize > sizeof(digest))
    {
        outputSize = sizeof(digest);
    }
    sha_memcpy(output, digest, outputSize);
}

status_t SHA_Init(SHA_Type *base, sha_ctx_t *ctx, sha_algo_t algo)
{
    status_t status;

    sha_ctx_internal_t *ctxInternal;
    /* compile time check for the correct structure size */
    BUILD_ASSERT(sizeof(sha_ctx_t) >= sizeof(sha_ctx_internal_t), sha_ctx_t_size);
    uint32_t i;

    status = sha_check_input_args(base, ctx, algo);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* set algorithm in context struct for later use */
    ctxInternal = (sha_ctx_internal_t *)ctx;
    ctxInternal->algo = algo;
    ctxInternal->blksz = 0u;
    for (i = 0; i < sizeof(ctxInternal->blk.w) / sizeof(ctxInternal->blk.w[0]); i++)
    {
        ctxInternal->blk.w[0] = 0u;
    }
    ctxInternal->state = kSHA_HashInit;
    ctxInternal->fullMessageSize = 0;
    return status;
}

status_t SHA_Update(SHA_Type *base, sha_ctx_t *ctx, const uint8_t *message, size_t messageSize)
{
    bool isUpdateState;
    status_t status;
    sha_ctx_internal_t *ctxInternal;
    size_t blockSize;

    if (messageSize == 0)
    {
        return kStatus_Success;
    }

    ctxInternal = (sha_ctx_internal_t *)ctx;
    status = sha_check_context(ctxInternal, message);
    if (kStatus_Success != status)
    {
        return status;
    }

    ctxInternal->fullMessageSize += messageSize;
    blockSize = SHA_BLOCK_SIZE;
    /* if we are still less than 64 bytes, keep only in context */
    if ((ctxInternal->blksz + messageSize) <= blockSize)
    {
        sha_memcpy((&ctxInternal->blk.b[0]) + ctxInternal->blksz, message, messageSize);
        ctxInternal->blksz += messageSize;
        return status;
    }
    else
    {
        isUpdateState = ctxInternal->state == kSHA_HashUpdate;
        if (!isUpdateState)
        {
            /* start NEW hash */
            sha_engine_init(base, ctxInternal);
            ctxInternal->state = kSHA_HashUpdate;
        }
    }

    /* process message data */
    status = sha_process_message_data(base, ctxInternal, message, messageSize);
    return status;
}

status_t SHA_Finish(SHA_Type *base, sha_ctx_t *ctx, uint8_t *output, size_t *outputSize)
{
    size_t algOutSize = 0;
    status_t status;
    sha_ctx_internal_t *ctxInternal;
    uint32_t *ctxW;
    uint32_t i;

    ctxInternal = (sha_ctx_internal_t *)ctx;
    status = sha_check_context(ctxInternal, output);
    if (kStatus_Success != status)
    {
        return status;
    }

    if (ctxInternal->state == kSHA_HashInit)
    {
        sha_engine_init(base, ctxInternal);
    }

    size_t outSize = 0u;

    /* compute algorithm output length */
    switch (ctxInternal->algo)
    {
        case kSHA_Sha1:
            outSize = kSHA_OutLenSha1;
            break;
        case kSHA_Sha256:
            outSize = kSHA_OutLenSha256;
            break;
        default:
            break;
    }
    algOutSize = outSize;

    /* flush message last incomplete block, if there is any, and add padding bits */
    status = sha_finalize(base, ctxInternal);

    if (outputSize)
    {
        if (algOutSize < *outputSize)
        {
            *outputSize = algOutSize;
        }
        else
        {
            algOutSize = *outputSize;
        }
    }

    sha_get_digest(base, &output[0], algOutSize);

    ctxW = (uint32_t *)ctx;
    for (i = 0; i < SHA_CTX_SIZE; i++)
    {
        ctxW[i] = 0u;
    }
    return status;
}
