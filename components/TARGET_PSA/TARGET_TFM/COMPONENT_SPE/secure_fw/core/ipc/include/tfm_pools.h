/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_POOLS_H__
#define __TFM_POOLS_H__

/*
 * Resource pool - few known size resources allocation/free is required,
 * so pool is more applicable than heap.
 */

/*
 * Pool Instance:
 *  [ Pool Instance ] + N * [ Pool Chunks ]
 */
struct tfm_pool_chunk_t {
    struct tfm_list_node_t list;        /* Chunk list                  */
    void *pool;                         /* Point to the parent pool    */
    uint8_t data[0];                    /* Data indicator              */
};

struct tfm_pool_instance_t {
    size_t chunksz;                     /* Chunks size of pool member   */
    struct tfm_list_node_t chunks_list; /* Chunk list head in pool      */
    struct tfm_pool_chunk_t chunks[0];  /* Data indicator               */
};

/*
 * This will declares a static memory pool variable with chunk memory.
 * Parameters:
 *  name        -   Variable name, will be used when register
 *  chunksz     -   chunk size in bytes
 *  num         -   Number of chunks
 */
#define TFM_POOL_DECLARE(name, chunksz, num)                                \
    static uint8_t name##_pool_buf[((chunksz) +                             \
                                   sizeof(struct tfm_pool_chunk_t)) * (num) \
                                   + sizeof(struct tfm_pool_instance_t)]    \
                                   __attribute__((aligned(4)));            \
    static struct tfm_pool_instance_t *name =                               \
                            (struct tfm_pool_instance_t *)name##_pool_buf

/* Get the head size of memory pool */
#define POOL_HEAD_SIZE (sizeof(struct tfm_pool_instance_t) +                \
                        sizeof(struct tfm_pool_chunk_t))

/* Get the whole size of memory pool */
#define POOL_BUFFER_SIZE(name)          sizeof(name##_pool_buf)

/**
 * \brief Register a memory pool.
 *
 * \param[in] pool              Pointer to memory pool declared by
 *                              \ref TFM_POOL_DECLARE
 * \param[in] poolsz             Size of the pool buffer.
 * \param[in] chunksz           Size of chunks.
 * \param[in] num               Number of chunks.
 *
 * \retval IPC_SUCCESS          Success.
 * \retval IPC_ERROR_BAD_PARAMETERS Parameters error.
 */
int32_t tfm_pool_init(struct tfm_pool_instance_t *pool, size_t poolsz,
                      size_t chunksz, size_t num);

/**
 * \brief Allocate a memory from pool.
 *
 * \param[in] pool              pool pointer decleared by \ref TFM_POOL_DECLARE
 *
 * \retval buffer pointer       Success.
 * \retval NULL                 Failed.
 */
void *tfm_pool_alloc(struct tfm_pool_instance_t *pool);

/**
 * \brief Free the allocated memory.
 *
 * \param[in] ptr               Buffer pointer want to free.
 */
void tfm_pool_free(void *ptr);

#endif
