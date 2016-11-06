/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
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
 *
 */

#include "fstorage.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fstorage_config.h"
#include "nrf_error.h"
#include "nrf_soc.h"


#define FS_FLAG_INIT                (1 << 0)    /**< fstorage has been initialized. */
#define FS_FLAG_PROCESSING          (1 << 1)    /**< fstorage is executing queued flash operations. */
#define FS_FLAG_FLASH_REQ_PENDING   (1 << 2)    /**< fstorage is waiting for a flash operation initiated by another module to complete. */


/**@brief Macro invocation that registers section fs_data.
 *
 * @details Required for compilation.
 */
NRF_SECTION_VARS_REGISTER_SECTION(fs_data);


/**@brief Macro invocation that declares symbols used to find the beginning and end of the section fs_data.
 *
 * @details Required for compilation.
 */
NRF_SECTION_VARS_REGISTER_SYMBOLS(fs_config_t, fs_data);


/**@defgroup Section vars helper macros.
 *
 * @details Macros used to manipulate registered section variables.
 */
 /**@brief Get section variable with fstorage configuration by index. */
#define FS_SECTION_VARS_GET(i)          NRF_SECTION_VARS_GET(i, fs_config_t, fs_data)
 /**@brief Get the number of registered section variables. */
#define FS_SECTION_VARS_COUNT           NRF_SECTION_VARS_COUNT(fs_config_t, fs_data)
 /**@brief Get the start address of the registered section variables. */
#define FS_SECTION_VARS_START_ADDR      NRF_SECTION_VARS_START_ADDR(fs_data)
 /**@brief Get the end address of the registered section variables. */
#define FS_SECTION_VARS_END_ADDR        NRF_SECTION_VARS_END_ADDR(fs_data)

/** @} */


/**@brief The command queue element.
 *
 * @details Encapsulate details of a command requested to this module.
 */
typedef struct
{
    fs_config_t const * p_config;           /**< The configuration of the user who requested the operation. */
    uint8_t             op_code;            /**< Operation code. */
    uint32_t const *    p_src;              /**< Pointer to the data to be written to flash. The data must be kept in memory until the operation has finished. */
    uint32_t const *    p_addr;             /**< Destination of the data in flash. */
    fs_length_t         length_words;       /**< Length of the operation */
    fs_length_t         offset;             /**< Offset of the operation if operation is done in chunks */
} fs_cmd_t;


/**@brief Structure that defines the command queue
 *
 * @details This queue holds flash operations requested to the module. 
 *          The data to be written must be kept in memory until the write operation is completed,
 *          i.e., a callback indicating completion is received by the application.
 */
typedef struct
{
    uint8_t     rp;                         /**< The current element being processed. */
    uint8_t     count;                      /**< Number of elements in the queue. */
    fs_cmd_t    cmd[FS_CMD_QUEUE_SIZE];     /**< Array to maintain flash access operation details. */
} fs_cmd_queue_t;


static uint8_t          m_flags;            /**< FStorage status flags. */
static fs_cmd_queue_t   m_cmd_queue;        /**< Flash operation request queue. */
static uint16_t         m_retry_count = 0;  /**< Number of times a single flash operation was retried. */


// Function prototypes
static ret_code_t queue_process(void);
static ret_code_t queue_process_impl(void);
static void app_notify(uint32_t result, fs_cmd_t const * p_cmd);


/**@brief Macro to check that the configuration is non-NULL and within
*         valid section variable memory bounds.
 *
 * @param[in]   config    Configuration to check.
 */
#define FS_CHECK_CONFIG(config) \
    ((FS_SECTION_VARS_START_ADDR < config) && (config < FS_SECTION_VARS_END_ADDR))


/**@brief Function to check that the configuration is non-NULL and within
*         valid section variable memory bounds.
 *
 * @param[in]   config    Configuration to check.
 */
static bool check_config(fs_config_t const * const config)
{
    if (config == NULL)
    {
        return false;
    }

    if ((FS_SECTION_VARS_START_ADDR <= (uint32_t)config) && ((uint32_t)config < FS_SECTION_VARS_END_ADDR))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**@brief Function to initialize the queue. */
static void queue_init(void)
{
    memset(&m_cmd_queue, 0, sizeof(fs_cmd_queue_t));
}


/**@brief Function to reset a queue item to its default values.
 *
 * @param	index	Index of the queue element.
 */
static void cmd_reset(uint32_t index)
{
    memset(&m_cmd_queue.cmd[index], 0, sizeof(fs_cmd_t));
}


/**@brief Function to enqueue flash access command
 *
 * @param[in]   config      Registered configuration.
 * @param[in]   op_code     Operation code.
 * @param[in]   address     Destination of the data.
 * @param[in]   p_src       Source of data or NULL if n/a.
 * @param[in]   length      Length of the data, in 4 byte words.
 *
 * @retval NRF_SUCCESS      Success. Command enqueued.
 * @retval NRF_ERROR_NO_MEM Error. Queue is full.
 * @retval Any error returned by the SoftDevice flash API.
 */
static ret_code_t cmd_enqueue(fs_config_t      const * p_config,
                              uint8_t                  op_code,
                              uint32_t         const * p_addr,
                              uint32_t         const * p_src,
                              fs_length_t              length_words)
{
    fs_cmd_t * p_cmd;
    uint8_t    write_pos;

    if (m_cmd_queue.count == FS_CMD_QUEUE_SIZE - 1)
    {
        return NRF_ERROR_NO_MEM;
    }

    write_pos = (m_cmd_queue.rp + m_cmd_queue.count) % FS_CMD_QUEUE_SIZE;

    p_cmd = &m_cmd_queue.cmd[write_pos];

    p_cmd->p_config     = p_config;
    p_cmd->op_code      = op_code;
    p_cmd->p_src        = p_src;
    p_cmd->p_addr       = p_addr;
    p_cmd->length_words = length_words;

    m_cmd_queue.count++;

    return queue_process();
}


/**@brief Function to consume queue item and notify the return value of the operation.
 *
 * @details This function will report the result and remove the command from the queue after
 *          notification.
 */
static void cmd_consume(uint32_t result, const fs_cmd_t * p_cmd)
{
    // Consume the current item on the queue.
    uint8_t rp = m_cmd_queue.rp;

    m_cmd_queue.count--;
    if (m_cmd_queue.count == 0)
    {
        // There are no elements left. Stop processing the queue.
        m_flags &= ~FS_FLAG_PROCESSING;
    }

    if (++(m_cmd_queue.rp) == FS_CMD_QUEUE_SIZE)
    {
        m_cmd_queue.rp = 0;
    }

    // Notify upon successful operation.
    app_notify(result, p_cmd);

    // Reset the queue element.
    cmd_reset(rp);
}


/**@brief Function to store data to flash.
 *
 * @param[in]   p_cmd   The queue element associated with the operation.
 *
 * @retval NRF_SUCCESS  Success. The request was sent to the SoftDevice.
 * @retval Any error returned by the SoftDevice flash API.
 */
static __INLINE uint32_t store_execute(fs_cmd_t const * const p_cmd)
{
    // Write in chunks if write-size is larger than FS_MAX_WRITE_SIZE.
    fs_length_t const length = ((p_cmd->length_words - p_cmd->offset) < FS_MAX_WRITE_SIZE_WORDS) ?
        (p_cmd->length_words - p_cmd->offset) : FS_MAX_WRITE_SIZE_WORDS;

    return sd_flash_write((uint32_t*)p_cmd->p_addr + p_cmd->offset /* destination */,
                          (uint32_t*)p_cmd->p_src + p_cmd->offset  /* source */,
                          length);
}


/**@brief Function to erase a page.
 *
 * @param[in]   p_cmd   The queue element associated with the operation.
 *
 * @retval NRF_SUCCESS  Success. The request was sent to the SoftDevice.
 * @retval Any error returned by the SoftDevice flash API.
 */
static __INLINE uint32_t erase_execute(fs_cmd_t const * const p_cmd)
{
    // Erase the page.
    return sd_flash_page_erase((uint32_t)(p_cmd->p_addr + p_cmd->offset) / FS_PAGE_SIZE);
}


/**@brief Function to process the current element in the queue and return the result.
 *
 * @retval NRF_SUCCESS          Success.
 * @retval NRF_ERROR_FORBIDDEN  Error. Undefined command.
 * @retval Any error returned by the SoftDevice flash API.
 */
static uint32_t queue_process_impl(void)
{
    uint32_t ret;
    
    fs_cmd_t const * const p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];

    switch (p_cmd->op_code)
    {
        case FS_OP_STORE:
            ret = store_execute(p_cmd);
            break;

        case FS_OP_ERASE:
            ret = erase_execute(p_cmd);
            break;

        case FS_OP_NONE:
            ret = NRF_SUCCESS;
            break;

        default:
            ret = NRF_ERROR_FORBIDDEN;
            break;
    }

    return ret;
}


/**@brief Starts processing the queue if there are no pending flash operations
 *        for which we are awaiting a callback.
 */
static ret_code_t queue_process(void)
{
    ret_code_t ret = NRF_SUCCESS;

    /** If the queue is not being processed, and there are still
     *  some elements in it, then start processing. */
    if ( !(m_flags & FS_FLAG_PROCESSING) &&
          (m_cmd_queue.count > 0))
    {
        m_flags |= FS_FLAG_PROCESSING;

        ret = queue_process_impl();

        /** There is ongoing flash-operation which was not
         *  initiated by fstorage. */
        if (ret == NRF_ERROR_BUSY)
        {
            // Wait for a system callback.
            m_flags |= FS_FLAG_FLASH_REQ_PENDING;

            // Stop processing the queue.
            m_flags &= ~FS_FLAG_PROCESSING;

            ret = NRF_SUCCESS;
        }
        else if (ret != NRF_SUCCESS)
        {
            // Another error has occurred.
            app_notify(ret, &m_cmd_queue.cmd[m_cmd_queue.rp]);
        }
    }

    // If we are already processing the queue, return immediately.
    return ret;
}


/**@brief Flash operation success callback handler.
 *
 * @details     This function updates read/write pointers.
 *              This function resets retry count.
 */
static __INLINE void on_operation_success(void)
{
    fs_cmd_t * const p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];

    m_retry_count = 0;

    switch (p_cmd->op_code)
    {
        case FS_OP_STORE:
            // Update the offset on successful write.
            p_cmd->offset += FS_MAX_WRITE_SIZE_WORDS;
            break;

        case FS_OP_ERASE:
            // Update the offset to correspond to the page that has been erased.
            p_cmd->offset += FS_PAGE_SIZE_WORDS;
            break;
    }

    // If offset is equal to or larger than length, then the operation has finished.
    if (p_cmd->offset >= p_cmd->length_words)
    {
        cmd_consume(NRF_SUCCESS, p_cmd);
    }

    queue_process();
}


/**@brief Flash operation failure callback handler.
 *
 * @details Function to keep track of retries and notify failures.
 */
static __INLINE void on_operation_failure(uint32_t sys_evt)
{
    const fs_cmd_t * p_cmd;
    
    if (++m_retry_count > FS_CMD_MAX_RETRIES)
    {
        p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];
        cmd_consume(NRF_ERROR_TIMEOUT, p_cmd);
    }

    queue_process();
}


/**@brief Function to notify users.
 *
 * @param[in]   result      Result of the flash operation.
 * @param[in]   p_cmd       The command associated with the callback.
 */
static void app_notify(uint32_t result, fs_cmd_t const * const p_cmd)
{
    p_cmd->p_config->cb(p_cmd->op_code, result, p_cmd->p_addr, p_cmd->length_words);
}


ret_code_t fs_init(void)
{
    uint16_t   lowest_index = 0;
    uint16_t   lowest_order = 0xFFFF;
    uint32_t * current_end  = (uint32_t*)FS_PAGE_END_ADDR;
    uint32_t   num_left     = FS_SECTION_VARS_COUNT;

    queue_init();

    /** Assign pages to registered users, beginning with the ones with the lowest
     *  order, which will be assigned pages with the lowest memory address. */
    do
    {
        fs_config_t * p_config;
        for (uint16_t i = 0; i < FS_SECTION_VARS_COUNT; i++)
        {
            p_config = FS_SECTION_VARS_GET(i);

            // Skip the ones which have the end-address already set.
            if (p_config->p_end_addr != NULL)
                continue;

            if (p_config->page_order < lowest_order)
            {
                lowest_order = p_config->page_order;
                lowest_index = i;
            }
        }

        p_config = FS_SECTION_VARS_GET(lowest_index);

        p_config->p_end_addr   = current_end;
        p_config->p_start_addr = p_config->p_end_addr - (p_config->num_pages * FS_PAGE_SIZE_WORDS);

        current_end  = p_config->p_start_addr;
        lowest_order = 0xFFFF;

    } while ( --num_left > 0 );

    m_flags |= FS_FLAG_INIT;

    return NRF_SUCCESS;
}


ret_code_t fs_store(fs_config_t const *       p_config,
                    uint32_t    const *       p_addr,
                    uint32_t    const * const p_data,
                    fs_length_t               length_words)
{
    if ((m_flags & FS_FLAG_INIT) == 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (!check_config(p_config))
    {
        return NRF_ERROR_FORBIDDEN;
    }

    if (!is_word_aligned(p_addr))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    // Check that the erase operation is on pages owned by this user (configuration).
    if ((p_addr < p_config->p_start_addr) || ((p_addr + length_words) > p_config->p_end_addr))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    return cmd_enqueue(p_config, FS_OP_STORE, p_addr, p_data, length_words);
}


ret_code_t fs_erase(fs_config_t const *       p_config,
                    uint32_t          * const p_addr,
                    fs_length_t const         length_words)
{
    if ((m_flags & FS_FLAG_INIT) == 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (!check_config(p_config))
    {
        return NRF_ERROR_FORBIDDEN;
    }

    /** Check that the address is aligned on a page boundary and the length to erase
     *  is a multiple of the page size. */
    if (((uint32_t)p_addr & (FS_PAGE_SIZE - 1)) ||
        (length_words     & (FS_PAGE_SIZE_WORDS - 1)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    // Check that the erase operation is on pages owned by this user (configuration).
    if ((p_addr < p_config->p_start_addr) || ((p_addr + length_words) > p_config->p_end_addr))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    return cmd_enqueue(p_config, FS_OP_ERASE, p_addr, NULL, length_words);
}


/**@brief Function to handle system events from the SoftDevice.
 *
 * @details     This function should be dispatched system events if any of the modules used by
 *              the application rely on FStorage. Examples include @ref Peer Manager and
 *              @ref Flash Data Storage.
 *
 * @param[in]   sys_evt     System Event received.
 */
void fs_sys_event_handler(uint32_t sys_evt)
{
    if (m_flags & FS_FLAG_PROCESSING)
    {
        /** A flash operation was initiated by this module.
         *  Handle its result. */
        switch (sys_evt)
        {
            case NRF_EVT_FLASH_OPERATION_SUCCESS:
                on_operation_success();
                break;

            case NRF_EVT_FLASH_OPERATION_ERROR:
                on_operation_failure(sys_evt);
                break;
        }
    }
    else if ((m_flags & FS_FLAG_FLASH_REQ_PENDING))
    {
        /** A flash operation was initiated outside this module.
         *  We have now receveid a callback which indicates it has
         *  finished. Clear the FS_FLAG_FLASH_REQ_PENDING flag. */
         m_flags &= ~FS_FLAG_FLASH_REQ_PENDING;

         // Resume processing the queue, if necessary.
         queue_process();
    }
}


// Just for testing out section vars (across many compilers).
void fs_debug_print()
{
    printf("fs start address: 0x%08lx\r\n", (unsigned long)FS_SECTION_VARS_START_ADDR);
    printf("fs end address: 0x%08lx\r\n",   (unsigned long)FS_SECTION_VARS_END_ADDR);
    printf("Num items: 0x%08lx\r\n",        (unsigned long)FS_SECTION_VARS_COUNT);
    printf("===== ITEMS %lu =====\r\n",     (unsigned long)FS_SECTION_VARS_COUNT);

    for(uint32_t i = 0; i < FS_SECTION_VARS_COUNT; i++)
    {
        fs_config_t* config = FS_SECTION_VARS_GET(i);
        printf( "Address: 0x%08lx, CB: 0x%08lx\r\n",
                (unsigned long)config, (unsigned long)config->cb );
    }
    printf("\r\n");
}
