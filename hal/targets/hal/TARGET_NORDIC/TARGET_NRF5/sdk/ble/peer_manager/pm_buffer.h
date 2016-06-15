/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#ifndef BUFFER_H__
#define BUFFER_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "pm_mutex.h"


/**
 * @cond NO_DOXYGEN
 * @defgroup pm_buffer Buffer
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. This module provides a simple buffer.
 */


#define BUFFER_INVALID_ID 0xFF

#define PM_BUFFER_INIT(p_buffer, n_blocks, block_size, err_code)    \
do                                                                  \
{                                                                   \
    static uint8_t buffer_memory[(n_blocks) * (block_size)];        \
    static uint8_t mutex_memory[MUTEX_STORAGE_SIZE(n_blocks)];      \
    err_code = pm_buffer_init((p_buffer),                           \
                               buffer_memory,                       \
                              (n_blocks) * (block_size),            \
                               mutex_memory,                        \
                               MUTEX_STORAGE_SIZE(n_blocks),        \
                              (n_blocks),                           \
                              (block_size));                        \
} while(0)


typedef struct
{
    uint8_t * p_memory;   /**< The storage for all buffer entries. The size of the buffer must be n_blocks*block_size. */
    uint8_t * p_mutex;    /**< A mutex group with one mutex for each buffer entry. */
    uint32_t  n_blocks;   /**< The number of allocatable blocks in the buffer. */
    uint32_t  block_size; /**< The size of each block in the buffer. */
} pm_buffer_t;

/**@brief Function for initializing a buffer instance.
 *
 * @param[out] p_buffer            The buffer instance to initialize.
 * @param[in]  p_buffer_memory     The memory this buffer will use.
 * @param[in]  buffer_memory_size  The size of p_buffer_memory. This must be at least
 *                                 n_blocks*block_size.
 * @param[in]  p_mutex_memory      The memory for the mutexes. This must be at least
 *                                 @ref MUTEX_STORAGE_SIZE(n_blocks).
 * @param[in]  mutex_memory_size   The size of p_mutex_memory.
 * @param[in]  n_blocks            The number of blocks in the buffer.
 * @param[in]  block_size          The size of each block.
 *
 * @retval NRF_SUCCESS              Successfully initialized buffer instance.
 * @retval NRF_ERROR_INVALID_PARAM  A parameter was 0 or NULL or a size was too small.
 */
ret_code_t pm_buffer_init(pm_buffer_t * p_buffer,
                          uint8_t     * p_buffer_memory,
                          uint32_t      buffer_memory_size,
                          uint8_t     * p_mutex_memory,
                          uint32_t      mutex_memory_size,
                          uint32_t      n_blocks,
                          uint32_t      block_size);


/**@brief Function for acquiring a buffer block in a buffer.
 *
 * @param[in]  p_buffer  The buffer instance acquire from.
 * @param[in]  n_blocks  The number of contiguous blocks to acquire.
 *
 * @return The id of the acquired block, if successful.
 * @retval BUFFER_INVALID_ID  If unsuccessful.
 */
uint8_t pm_buffer_block_acquire(pm_buffer_t * p_buffer, uint32_t n_blocks);


/**@brief Function for getting a pointer to a specific buffer block.
 *
 * @param[in]  p_buffer  The buffer instance get from.
 * @param[in]  id        The id of the buffer to get the pointer for.
 *
 * @return A pointer to the buffer for the specified id, if the id is valid.
 * @retval NULL  If the id is invalid.
 */
uint8_t * pm_buffer_ptr_get(pm_buffer_t * p_buffer, uint8_t id);


/**@brief Function for releasing a buffer block.
 *
 * @param[in]  p_buffer  The buffer instance containing the block to release.
 * @param[in]  id        The id of the block to release.
 */
void pm_buffer_release(pm_buffer_t * p_buffer, uint8_t id);


#endif // BUFFER_H__

/**
 * @}
 * @endcond
 */
