/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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


#include "pstorage.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf_error.h"
#include "nrf_assert.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "app_util.h"
#include "app_error.h"

#define INVALID_OPCODE             0x00                                /**< Invalid op code identifier. */
#define SOC_MAX_WRITE_SIZE         PSTORAGE_FLASH_PAGE_SIZE            /**< Maximum write size allowed for a single call to \ref sd_flash_write as specified in the SoC API. */
#define RAW_MODE_APP_ID            (PSTORAGE_NUM_OF_PAGES + 1)         /**< Application id for raw mode. */

#if defined(NRF52)
#define SD_CMD_MAX_TRIES           1000                                /**< Number of times to try a softdevice flash operatoion, specific for nRF52 to account for longest time of flash page erase*/
#else
#define SD_CMD_MAX_TRIES           3                                   /**< Number of times to try a softdevice flash operation when the @ref NRF_EVT_FLASH_OPERATION_ERROR sys_evt is received. */
#endif /* defined(NRF52) */

#define MASK_TAIL_SWAP_DONE        (1 << 0)                            /**< Flag for checking if the tail restore area has been written to swap page. */     
#define MASK_SINGLE_PAGE_OPERATION (1 << 1)                            /**< Flag for checking if command is a single flash page operation. */
#define MASK_MODULE_INITIALIZED    (1 << 2)                            /**< Flag for checking if the module has been initialized. */
#define MASK_FLASH_API_ERR_BUSY    (1 << 3)                            /**< Flag for checking if flash API returned NRF_ERROR_BUSY. */

/**
 * @defgroup api_param_check API Parameters check macros.
 *
 * @details Macros that verify parameters passed to the module in the APIs. These macros
 *          could be mapped to nothing in final code versions to save execution and size.
 *
 * @{
 */

/**@brief Check if the input pointer is NULL, if so it returns NRF_ERROR_NULL.
 */
#define NULL_PARAM_CHECK(PARAM)                                                                   \
        if ((PARAM) == NULL)                                                                      \
        {                                                                                         \
            return NRF_ERROR_NULL;                                                                \
        }

/**@brief Verifies that the module identifier supplied by the application is within permissible
 *        range.
 */
#define MODULE_ID_RANGE_CHECK(ID)                                                                 \
        if ((((ID)->module_id) >= PSTORAGE_NUM_OF_PAGES) ||                                       \
            (m_app_table[(ID)->module_id].cb == NULL))                                            \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }

/**@brief Verifies that the block identifier supplied by the application is within the permissible
 *        range.
 */
#define BLOCK_ID_RANGE_CHECK(ID)                                                                  \
        if (((ID)->block_id) >= (m_app_table[(ID)->module_id].base_id +                           \
            (m_app_table[(ID)->module_id].block_count * MODULE_BLOCK_SIZE(ID))))                  \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }

/**@brief Verifies that the block size requested by the application can be supported by the module.
 */
#define BLOCK_SIZE_CHECK(X)                                                                       \
        if (((X) > PSTORAGE_MAX_BLOCK_SIZE) || ((X) < PSTORAGE_MIN_BLOCK_SIZE))                   \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }

/**@brief Verifies the block size requested by the application in registration API.
 */
#define BLOCK_COUNT_CHECK(COUNT, SIZE)                                                            \
        if (((COUNT) == 0) ||                                                                     \
            ((m_next_page_addr + ((COUNT) *(SIZE)) > PSTORAGE_SWAP_ADDR)))                        \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }        

/**@brief Verifies the size parameter provided by the application in API.
 */
#define SIZE_CHECK(ID, SIZE)                                                                      \
        if(((SIZE) == 0) || ((SIZE) > MODULE_BLOCK_SIZE(ID)))                                     \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }

/**@brief Verifies the offset parameter provided by the application in API.
 */
#define OFFSET_CHECK(ID, OFFSET, SIZE)                                                            \
        if(((SIZE) + (OFFSET)) > MODULE_BLOCK_SIZE(ID))                                           \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }

#ifdef PSTORAGE_RAW_MODE_ENABLE

/**@brief Verifies the module identifier supplied by the application.
 */
#define MODULE_RAW_HANDLE_CHECK(ID)                                                               \
        if ((((ID)->module_id) != RAW_MODE_APP_ID))                                               \
        {                                                                                         \
            return NRF_ERROR_INVALID_PARAM;                                                       \
        }

#endif // PSTORAGE_RAW_MODE_ENABLE

/**@} */


/**@brief Verify module's initialization status.
 *
 * @details  Verify module's initialization status. Returns NRF_ERROR_INVALID_STATE when a
 *           module API is called without initializing the module.
 */
#define VERIFY_MODULE_INITIALIZED()                                                               \
        do                                                                                        \
        {                                                                                         \
            if (!(m_flags & MASK_MODULE_INITIALIZED))                                             \
            {                                                                                     \
                 return NRF_ERROR_INVALID_STATE;                                                  \
            }                                                                                     \
        } while(0)

/**@brief Macro to fetch the block size registered for the module. */
#define MODULE_BLOCK_SIZE(ID) (m_app_table[(ID)->module_id].block_size)

/**@brief Main state machine of the component. */
typedef enum
{
    STATE_IDLE,                                                        /**< State for being idle (no command execution in progress). */
    STATE_STORE,                                                       /**< State for storing data when using store/update API. */
    STATE_DATA_ERASE_WITH_SWAP,                                        /**< State for erasing the data page when using update/clear API when use of swap page is required. */
    STATE_DATA_ERASE,                                                  /**< State for erasing the data page when using update/clear API without the need to use the swap page. */
    STATE_ERROR                                                        /**< State entered when command processing is terminated abnormally. */
} pstorage_state_t;  

/**@brief Sub state machine contained by @ref STATE_DATA_ERASE_WITH_SWAP super state machine. */
typedef enum
{
    STATE_ERASE_SWAP,                                                  /**< State for erasing the swap page when using the update/clear API. */   
    STATE_WRITE_DATA_TO_SWAP,                                          /**< State for writing the data page into the swap page when using update/clear API. */
    STATE_ERASE_DATA_PAGE,                                             /**< State for erasing data page when using update/clear API. */
    STATE_RESTORE_TAIL,                                                /**< State for restoring tail (end) of backed up data from swap to data page when using update/clear API. */
    STATE_RESTORE_HEAD,                                                /**< State for restoring head (beginning) of backed up data from swap to data page when using update/clear API. */
    SWAP_SUB_STATE_MAX                                                 /**< Enumeration upper bound. */   
} flash_swap_sub_state_t;

/**@brief Application registration information.
 *
 * @details Defines application specific information that the application needs to maintain to be able 
 *          to process requests from each one of them.
 */
typedef struct
{
    pstorage_ntf_cb_t cb;                                              /**< Callback registered with the module to be notified of result of flash access.  */
    pstorage_block_t  base_id;                                         /**< Base block ID assigned to the module. */
    pstorage_size_t   block_size;                                      /**< Size of block for the module. */
    pstorage_size_t   block_count;                                     /**< Number of blocks requested by the application. */
} pstorage_module_table_t;


#ifdef PSTORAGE_RAW_MODE_ENABLE
/**@brief Application registration information.
 *
 * @details Defines application specific information that the application registered for raw mode.
 */
typedef struct
{
    pstorage_ntf_cb_t cb;                                              /**< Callback registered with the module to be notified of the result of flash access.  */
} pstorage_raw_module_table_t;
#endif // PSTORAGE_RAW_MODE_ENABLE


/**@brief Defines command queue element.
 *
 * @details Defines command queue element. Each element encapsulates needed information to process
 *          a flash access command.
 */
typedef struct
{
    uint8_t           op_code;                                         /**< Identifies the flash access operation being queued. Element is free if op-code is INVALID_OPCODE. */
    pstorage_size_t   size;                                            /**< Identifies the size in bytes requested for the operation. */
    pstorage_size_t   offset;                                          /**< Offset requested by the application for the access operation. */
    pstorage_handle_t storage_addr;                                    /**< Address/Identifier for persistent memory. */
    uint8_t *         p_data_addr;                                     /**< Address/Identifier for data memory. This is assumed to be resident memory. */
} cmd_queue_element_t;


/**@brief   Defines command queue, an element is free if the op_code field is not invalid.
 *
 * @details Defines commands enqueued for flash access. At any point in time, this queue has one or
 *          more flash access operations pending if the count field is not zero. When the queue is
 *          not empty, the rp (read pointer) field points to the flash access command in progress 
 *          or, if none is in progress, the command to be requested next. The queue implements a 
 *          simple first in first out algorithm. Data addresses are assumed to be resident.
 */
typedef struct
{
    uint8_t             rp;                                            /**< Read pointer, pointing to flash access that is ongoing or to be requested next. */
    uint8_t             count;                                         /**< Number of elements in the queue.  */
    cmd_queue_element_t cmd[PSTORAGE_CMD_QUEUE_SIZE];                  /**< Array to maintain flash access operation details. */
} cmd_queue_t;

static cmd_queue_t             m_cmd_queue;                            /**< Flash operation request queue. */
static pstorage_size_t         m_next_app_instance;                    /**< Points to the application module instance that can be allocated next. */
static uint32_t                m_next_page_addr;                       /**< Points to the flash address that can be allocated to a module next. This is needed as blocks of a module that can span across flash pages. */
static pstorage_state_t        m_state;                                /**< Main state tracking variable. */
static flash_swap_sub_state_t  m_swap_sub_state;                       /**< Flash swap erase when swap used state tracking variable. */
static uint32_t                m_head_word_size;                       /**< Head restore area size in words. */
static uint32_t                m_tail_word_size;                       /**< Tail restore area size in words. */
static uint32_t                m_current_page_id;                      /**< Variable for tracking the flash page being processed. */
static uint32_t                m_num_of_command_retries;               /**< Variable for tracking flash operation retries upon flash operation failures. */
static pstorage_module_table_t m_app_table[PSTORAGE_NUM_OF_PAGES];     /**< Registered application information table. */
static uint32_t                m_num_of_bytes_written;                 /**< Variable for tracking the number of bytes written by the store operation. */
static uint32_t                m_app_data_size;                        /**< Variable for storing the application command size parameter internally. */
static uint32_t                m_flags = 0;                            /**< Storage for boolean flags for state tracking. */

#ifdef PSTORAGE_RAW_MODE_ENABLE
static pstorage_raw_module_table_t m_raw_app_table;                    /**< Registered application information table for raw mode. */
#endif // PSTORAGE_RAW_MODE_ENABLE

// Required forward declarations.
static void cmd_process(void);
static void store_operation_execute(void);
static void app_notify(uint32_t result, cmd_queue_element_t * p_elem);
static void cmd_queue_element_init(uint32_t index);
static void cmd_queue_dequeue(void);
static void sm_state_change(pstorage_state_t new_state);
static void swap_sub_state_state_change(flash_swap_sub_state_t new_state); 

/**@brief Function for consuming a command queue element.
 *
 * @details Function for consuming a command queue element, which has been fully processed.
 */
static void command_queue_element_consume(void)
{
    // Initialize/free the element as it is now processed.    
    cmd_queue_element_init(m_cmd_queue.rp);

    // Adjust command queue state tracking variables.
    --(m_cmd_queue.count);   
    if (++(m_cmd_queue.rp) == PSTORAGE_CMD_QUEUE_SIZE)
    {
        m_cmd_queue.rp = 0;
    }    
}


/**@brief Function for executing the finalization procedure for the command executed.
 *
 * @details Function for executing the finalization procedure for command executed, which includes 
 *          notifying the application of command completion, consuming the command queue element, 
 *          and changing the internal state.
 */
static void command_end_procedure_run(void)
{    
    app_notify(NRF_SUCCESS, &m_cmd_queue.cmd[m_cmd_queue.rp]);
    
    command_queue_element_consume();
    
    sm_state_change(STATE_IDLE);
}


/**@brief Function for idle state entry actions.
 *
 * @details Function for idle state entry actions, which include resetting relevant state data and 
 *          scheduling any possible queued flash access operation.
 */
static void state_idle_entry_run(void)
{
    m_num_of_command_retries = 0;
    m_num_of_bytes_written   = 0;
    
    // Schedule any possible queued flash access operation.
    cmd_queue_dequeue();
}


/**@brief Function for notifying an application of command completion and transitioning to an error 
 *        state.
 *
 * @param[in] result Result code of the operation for the application.
 */
static void app_notify_error_state_transit(uint32_t result)
{
    app_notify(result, &m_cmd_queue.cmd[m_cmd_queue.rp]);
    sm_state_change(STATE_ERROR);                
}


/**@brief Function for processing flash API error code.
 *
 * @param[in] err_code Error code from the flash API.
 */
static void flash_api_err_code_process(uint32_t err_code)
{
    switch (err_code)
    {
        case NRF_SUCCESS:
            break;
            
        case NRF_ERROR_BUSY:
            // Flash access operation was not accepted and must be reissued upon flash operation 
            // complete event.
            m_flags |= MASK_FLASH_API_ERR_BUSY;        
            break;
            
        default:
            // Complete the operation with appropriate result code and transit to an error state. 
            app_notify_error_state_transit(err_code);
            break;
    }
}

/**@brief Function for writing data to flash.
 *
 * @param[in] p_dst         Pointer to start of flash location to be written.
 * @param[in] p_src         Pointer to buffer with data to be written.
 * @param[in] size_in_words Number of 32-bit words to write. 
 */
static void flash_write(uint32_t * const       p_dst, 
                        uint32_t const * const p_src, 
                        uint32_t               size_in_words)
{
    flash_api_err_code_process(sd_flash_write(p_dst, p_src, size_in_words));    
}


/**@brief Function for writing data to flash upon store command.
 *
 * @details Function for writing data to flash upon executing store command. Data is written to 
 *          flash in reverse order, meaning starting at the end. If the data that is to be written 
 *          is greater than the flash page size, it will be fragmented to fit the flash page size.
 */
static void store_cmd_flash_write_execute(void)
{
    const cmd_queue_element_t * p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];
    
    if (p_cmd->size > SOC_MAX_WRITE_SIZE)    
    {
        const uint32_t offset = p_cmd->size - PSTORAGE_FLASH_PAGE_SIZE;
        flash_write((uint32_t *)(p_cmd->storage_addr.block_id + p_cmd->offset + offset),
                    (uint32_t *)(p_cmd->p_data_addr + offset), 
                    PSTORAGE_FLASH_PAGE_SIZE / sizeof(uint32_t));   

        m_num_of_bytes_written = PSTORAGE_FLASH_PAGE_SIZE;    
    }
    else
    {
        flash_write((uint32_t *)(p_cmd->storage_addr.block_id + p_cmd->offset),
                    (uint32_t *)(p_cmd->p_data_addr), 
                    p_cmd->size / sizeof(uint32_t));   

        m_num_of_bytes_written = p_cmd->size;        
    }    
}


/**@brief Function for store state entry action.
 *
 * @details Function for store state entry action, which includes writing data to a flash page.
 */
static void state_store_entry_run(void)
{
    store_cmd_flash_write_execute();    
}


/**@brief Function for data erase with swap state entry actions.
 *
 * @details Function for data erase with swap state entry actions. This includes adjusting relevant 
 *          state and data variables and transitioning to the correct sub state.
 */
static void state_data_erase_swap_entry_run(void)
{
    m_flags &= ~MASK_TAIL_SWAP_DONE;
    
    const cmd_queue_element_t * p_cmd        = &m_cmd_queue.cmd[m_cmd_queue.rp];
    const pstorage_block_t      cmd_block_id = p_cmd->storage_addr.block_id;
    
    const uint32_t clear_start_page_id = cmd_block_id / PSTORAGE_FLASH_PAGE_SIZE;
    m_current_page_id                  = clear_start_page_id;      
        
    // @note: No need to include p_cmd->offset when calculating clear_end_page_id as:
    // - clear API does not include offset parameter
    // - update and store APIs are limited to operate on single block boundary thus the boolean 
    // clause ((m_head_word_size == 0) && is_more_than_one_page) below in this function  will never 
    // evaluate as true as if is_more_than_one_page == true m_head_word_size is always != 0        
    const uint32_t clear_end_page_id  = (cmd_block_id + p_cmd->size - 1u) / 
                                        PSTORAGE_FLASH_PAGE_SIZE;

    if (clear_start_page_id == clear_end_page_id)
    {
        m_flags |= MASK_SINGLE_PAGE_OPERATION;
    }
    else
    {
        m_flags &= ~MASK_SINGLE_PAGE_OPERATION;
    }
                            
    if ((m_head_word_size == 0) && !(m_flags & MASK_SINGLE_PAGE_OPERATION))
    {        
        // No head restore required and clear/update area is shared by multiple flash pages, which 
        // means the current flash page does not have any tail area to restore. You can proceed with 
        // data page erase directly as no swap is needed for the current flash page.
        swap_sub_state_state_change(STATE_ERASE_DATA_PAGE);        
    }
    else
    {     
        swap_sub_state_state_change(STATE_ERASE_SWAP);
    }               
}


/**@brief Function for erasing flash page.
 *
 * @param[in] page_number Page number of the page to be erased.
 */
static void flash_page_erase(uint32_t page_number)
{
    flash_api_err_code_process(sd_flash_page_erase(page_number));
}


/**@brief Function for data erase state entry action.
 *
 * @details Function for data erase state entry action, which includes erasing the data flash page.
 */
static void state_data_erase_entry_run(void)
{
    flash_page_erase(m_current_page_id);                          
}


/**@brief Function for dispatching the correct application main state entry action.
 */
static void state_entry_action_run(void)
{
    switch (m_state)
    {
        case STATE_IDLE:
            state_idle_entry_run();
            break;

        case STATE_STORE:
            state_store_entry_run();
            break;
            
        case STATE_DATA_ERASE_WITH_SWAP:
            state_data_erase_swap_entry_run();
            break;
            
        case STATE_DATA_ERASE:
            state_data_erase_entry_run();        
            break;
                        
        default:
            // No action needed.
            break;
    }
}


/**@brief Function for changing application main state and dispatching state entry action.
 *
 * @param[in] new_state New application main state to transit to.
 */
static void sm_state_change(pstorage_state_t new_state)
{
    m_state = new_state;
    state_entry_action_run();
}


/**@brief Function for swap erase state entry action.
 *
 * @details Function for swap erase state entry action, which includes erasing swap flash 
 *          page.
 */
static void state_swap_erase_entry_run(void)
{
    flash_page_erase(PSTORAGE_SWAP_ADDR / PSTORAGE_FLASH_PAGE_SIZE);                
}


/**@brief Function for write data to the swap state entry action.
 *
 * @details Function for write data to the swap state entry action, which includes writing the 
 *          current data page to the swap flash page.
 */
static void state_write_data_swap_entry_run(void)
{
    // @note: There is room for further optimization here as there is only need to write the
    // whole flash page to swap area if there is both head and tail area to be restored. In any 
    // other case we can omit some data from the head or end of the page as that is the clear area.
    flash_write((uint32_t *)(PSTORAGE_SWAP_ADDR), 
                (uint32_t *)(m_current_page_id * PSTORAGE_FLASH_PAGE_SIZE), 
                PSTORAGE_FLASH_PAGE_SIZE / sizeof(uint32_t));    
}


/**@brief Function for erase data page state entry action.
 *
 * @details Function for erase data page state entry action, which includes erasing the data flash 
 *          page.
 */
static void state_erase_data_page_entry_run(void)
{
    flash_page_erase(m_current_page_id);
}


/**@brief Function for restore tail state entry action.
 *
 * @details Function for restore tail state entry action, which includes writing the tail section 
 *          back from swap to the data page.
 */
static void state_restore_tail_entry_run(void)
{
    const cmd_queue_element_t * p_cmd        = &m_cmd_queue.cmd[m_cmd_queue.rp];    
    const pstorage_block_t      cmd_block_id = p_cmd->storage_addr.block_id;                            
    
    const uint32_t tail_offset = (cmd_block_id + p_cmd->size + p_cmd->offset) % 
                                 PSTORAGE_FLASH_PAGE_SIZE; 
                                 
    flash_write((uint32_t *)(cmd_block_id + p_cmd->size + p_cmd->offset),
                (uint32_t *)(PSTORAGE_SWAP_ADDR + tail_offset),
                m_tail_word_size);
}


/**@brief Function for restore head state entry action.
 *
 * @details Function for restore head state entry action, which includes writing the head section 
 *          back from swap to the data page.
 */
static void state_restore_head_entry_run(void)
{
    flash_write((uint32_t *)((m_current_page_id - 1u) * PSTORAGE_FLASH_PAGE_SIZE),
                (uint32_t *)PSTORAGE_SWAP_ADDR,
                m_head_word_size);
}


/**@brief Function for dispatching the correct swap sub state entry action.
 */
static void swap_sub_state_entry_action_run(void)
{
    static void (* const swap_sub_state_sm_lut[SWAP_SUB_STATE_MAX])(void) = 
    {
        state_swap_erase_entry_run,
        state_write_data_swap_entry_run,
        state_erase_data_page_entry_run,
        state_restore_tail_entry_run,
        state_restore_head_entry_run
    };
    
    swap_sub_state_sm_lut[m_swap_sub_state]();
}


/**@brief Function for changing the swap sub state and dispatching state entry action.
 *
 * @param[in] new_state New swap sub state to transit to.
 */   
static void swap_sub_state_state_change(flash_swap_sub_state_t new_state)
{
    m_swap_sub_state = new_state;
    swap_sub_state_entry_action_run();    
}


/**@brief Function for initializing the command queue element.
 *
 * @param[in] index Index of the element to be initialized.
 */
static void cmd_queue_element_init(uint32_t index)
{
    // Internal function and checks on range of index can be avoided.
    m_cmd_queue.cmd[index].op_code                = INVALID_OPCODE;
    m_cmd_queue.cmd[index].size                   = 0;
    m_cmd_queue.cmd[index].storage_addr.module_id = PSTORAGE_NUM_OF_PAGES;
    m_cmd_queue.cmd[index].storage_addr.block_id  = 0;
    m_cmd_queue.cmd[index].p_data_addr            = NULL;
    m_cmd_queue.cmd[index].offset                 = 0;
}


/**@brief Function for initializing the command queue.
 */
static void cmd_queue_init(void)
{
    m_cmd_queue.rp    = 0;
    m_cmd_queue.count = 0;

    for (uint32_t cmd_index = 0; cmd_index < PSTORAGE_CMD_QUEUE_SIZE; ++cmd_index)
    {
        cmd_queue_element_init(cmd_index);
    }
}


/**@brief Function for enqueuing, and possibly dispatching, a flash access operation.
 *
 * @param[in] opcode         Identifies the operation requested to be enqueued.
 * @param[in] p_storage_addr Identifies the module and flash address on which the operation is 
 *                           requested.
 * @param[in] p_data_addr    Identifies the data address for flash access.
 * @param[in] size           Size in bytes of data requested for the access operation.
 * @param[in] offset         Offset within the flash memory block at which operation is requested.
 *
 * @retval    NRF_SUCCESS      Upon success.
 * @retval    NRF_ERROR_NO_MEM Upon failure, when no space is available in the command queue. 
 */
static uint32_t cmd_queue_enqueue(uint8_t             opcode,
                                  pstorage_handle_t * p_storage_addr,
                                  uint8_t           * p_data_addr,
                                  pstorage_size_t     size,
                                  pstorage_size_t     offset)
{
    uint32_t err_code;

    if (m_cmd_queue.count != PSTORAGE_CMD_QUEUE_SIZE)
    {
        // Enqueue the command if it the queue is not full.
        uint32_t write_index = m_cmd_queue.rp + m_cmd_queue.count;

        if (write_index >= PSTORAGE_CMD_QUEUE_SIZE) 
        {
            write_index -= PSTORAGE_CMD_QUEUE_SIZE;
        }

        m_cmd_queue.cmd[write_index].op_code      = opcode;
        m_cmd_queue.cmd[write_index].p_data_addr  = p_data_addr;
        m_cmd_queue.cmd[write_index].storage_addr = (*p_storage_addr);
        m_cmd_queue.cmd[write_index].size         = size;
        m_cmd_queue.cmd[write_index].offset       = offset;
               
        m_cmd_queue.count++;
                                
        if (m_state == STATE_IDLE)
        {
            cmd_process(); 
        }            
        
        err_code = NRF_SUCCESS;        
    }
    else
    {
        err_code = NRF_ERROR_NO_MEM;
    }

    return err_code;
}


/**@brief Function for dequeing a possible pending flash access operation.
 */
static void cmd_queue_dequeue(void)
{
    if ((m_cmd_queue.count != 0)) 
    {
        cmd_process();
    }
}


/**@brief Function for notifying an application of command completion.
 *
 * @param[in] result Result code of the operation for the application.
 * @param[in] p_elem Pointer to the command queue element for which this result was received. 
 */
static void app_notify(uint32_t result, cmd_queue_element_t * p_elem)
{
    pstorage_ntf_cb_t ntf_cb;
    const uint8_t     op_code = p_elem->op_code;

#ifdef PSTORAGE_RAW_MODE_ENABLE
    if (p_elem->storage_addr.module_id == RAW_MODE_APP_ID)
    {
        ntf_cb = m_raw_app_table.cb;
    }
    else
#endif // PSTORAGE_RAW_MODE_ENABLE
    {
        ntf_cb = m_app_table[p_elem->storage_addr.module_id].cb;
    }

    ntf_cb(&p_elem->storage_addr, op_code, result, p_elem->p_data_addr, m_app_data_size);
}


/**@brief Function for evaluating if a data page swap is required for the tail section on the 
 *        current page.
 *
 * @retval true  If data page swap is required.
 * @retval false If data page swap is not required.
 */
static bool is_tail_data_page_swap_required(void)
{
    bool ret_value;

    // Extract id of the last page command is executed upon.
    const cmd_queue_element_t * p_cmd        = &m_cmd_queue.cmd[m_cmd_queue.rp];
    const pstorage_block_t      cmd_block_id = p_cmd->storage_addr.block_id;        
    const uint32_t              last_page_id = (cmd_block_id + p_cmd->size + p_cmd->offset - 1u) / 
                                               PSTORAGE_FLASH_PAGE_SIZE;    
        
    // If tail section area exists and the current page is the last page then tail data page swap is 
    // required.    
    if ((m_tail_word_size != 0) && (m_current_page_id == last_page_id))
    {
        ret_value = true;
    }
    else
    {
        ret_value = false;    
    }
    
    return ret_value;
}


/**@brief Function for performing post processing for the update and clear commands.
 *
 * @details Function for performing post processing for the update and clear commands, which implies 
 *          executing the correct execution path depending on the command. 
 */
static void clear_post_processing_run(void)
{
    const cmd_queue_element_t * p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp]; 
    
    if (p_cmd->op_code != PSTORAGE_UPDATE_OP_CODE)
    {
        command_end_procedure_run();    
    }
    else
    {
        store_operation_execute();                    
    }
}


/**@brief Function for doing swap sub state exit action.
 */
static void swap_sub_sm_exit_action_run(void)
{
    clear_post_processing_run();
}


/**@brief Function for evaluating if the page erase operation is required for the current page.
 *
 * @retval true  If page erase is required.
 * @retval false If page erase is not required. 
 */
static bool is_page_erase_required(void)
{
    bool ret;
    
    const cmd_queue_element_t * p_cmd                      = &m_cmd_queue.cmd[m_cmd_queue.rp];
    const pstorage_block_t      cmd_block_id               = p_cmd->storage_addr.block_id;        
    const uint32_t              id_last_page_to_be_cleared = (cmd_block_id + p_cmd->size + 
                                                             p_cmd->offset - 1u) / 
                                                             PSTORAGE_FLASH_PAGE_SIZE;

    // True if:
    // - current page is not the last page OR
    // - current page is the last page AND no tail exists
    if ((m_current_page_id < id_last_page_to_be_cleared) ||
        ((m_current_page_id == id_last_page_to_be_cleared) && (m_tail_word_size == 0)))         
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    
    return ret;
}


/**@brief Function for reissuing the last flash operation request, which was rejected by the flash 
 *        API, in swap sub sate.
 */
static void swap_sub_state_err_busy_process(void)
{
    // Reissue the request by doing a self transition to the current state.    
    m_flags &= ~MASK_FLASH_API_ERR_BUSY;
    swap_sub_state_state_change(m_swap_sub_state);        
}


/**@brief Function for doing restore head state action upon flash operation success event.
 *
 * @details Function for doing restore head state action upon flash operation success event, which 
 *          includes making a state transition depending on the current state.
 */
static void head_restore_state_run(void)
{
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {            
        if (is_tail_data_page_swap_required())
        {
            // Additional data page needs to be swapped for tail section as we are clearing a block, 
            // which is shared between 2 flash pages.
                    
            // Adjust variables to ensure correct state transition path is taken after the tail 
            // section swap has completed.
            m_head_word_size = 0;   
            m_flags         |= MASK_TAIL_SWAP_DONE;        
                  
            swap_sub_state_state_change(STATE_ERASE_SWAP);        
        }
        else if (is_page_erase_required())
        {
            // Additional page erase operation is required.
                    
            // Adjust variable to ensure correct state transition path is taken after the additional 
            // page erase operation has completed.
            m_head_word_size = 0;
            swap_sub_state_state_change(STATE_ERASE_DATA_PAGE);        
        }            
        else if (m_tail_word_size != 0)
        {
            // Proceed with restoring tail from swap to data page. 
            swap_sub_state_state_change(STATE_RESTORE_TAIL);
        }
        else
        {            
            // Swap statemachine execution end reached.
            swap_sub_sm_exit_action_run();        
        }
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        swap_sub_state_err_busy_process();        
    }
}


/**@brief Function for doing restore tail state action upon flash operation success event.
 */
static void tail_restore_state_run(void)
{
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {
        swap_sub_sm_exit_action_run();        
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        swap_sub_state_err_busy_process();        
    }    
}


/**@brief Function for doing data page erase state action upon a flash operation success event.
 *
 * @details Function for doing data page erase state action upon a flash operation success event, 
 *          which includes making a state transit to a new state depending on the current state.
 */
static void data_page_erase_state_run(void)
{            
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {
        ++m_current_page_id;   
                    
        if (m_head_word_size != 0)
        {            
            swap_sub_state_state_change(STATE_RESTORE_HEAD);
        }
        else if (is_page_erase_required())
        {
            // Additional page erase operation is required.    
            swap_sub_state_state_change(STATE_ERASE_DATA_PAGE);                
        }                
        else if (m_tail_word_size != 0)
        {                    
            if (!(m_flags & MASK_TAIL_SWAP_DONE)) 
            {
                // Tail area restore is required and we have not yet written the relevant data page 
                // to swap area. Start the process of writing the data page to swap.
                m_flags |= MASK_TAIL_SWAP_DONE;            

                swap_sub_state_state_change(STATE_ERASE_SWAP);            
            }
            else
            {
                // Tail area restore is required and we have already written the relevant data page 
                // to swap area. Proceed by restoring the tail area.
                swap_sub_state_state_change(STATE_RESTORE_TAIL);            
            }
        }            
        else
        {
            swap_sub_sm_exit_action_run();        
        }        
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        swap_sub_state_err_busy_process();        
    }
}


/**@brief Function for doing data to swap write state action upon flash operation success event.
 */
static void data_to_swap_write_state_run(void)
{        
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {
        // If the operation is executed only on 1 single flash page it automatically means that tail 
        // area is written to the swap, which we store to flags.     
        if (m_flags & MASK_SINGLE_PAGE_OPERATION)
        {
            m_flags |= MASK_TAIL_SWAP_DONE;        
        }

        swap_sub_state_state_change(STATE_ERASE_DATA_PAGE);    
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        swap_sub_state_err_busy_process();        
    }        
}


/**@brief Function for doing swap erase state action upon flash operation success event.
 */
static void swap_erase_state_run(void)
{
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {        
        swap_sub_state_state_change(STATE_WRITE_DATA_TO_SWAP);
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        swap_sub_state_err_busy_process();        
    }
}


/**@brief Function for dispatching the correct state action for data erase with a swap composite 
*         state upon a flash operation success event.
 */
static void swap_sub_state_sm_run(void)
{  
    static void (* const swap_sub_state_sm_lut[SWAP_SUB_STATE_MAX])(void) = 
    {
        swap_erase_state_run,
        data_to_swap_write_state_run,
        data_page_erase_state_run,
        tail_restore_state_run,
        head_restore_state_run
    };
    
    swap_sub_state_sm_lut[m_swap_sub_state]();    
}


/**@brief Function for reissuing the last flash operation request, which was rejected by the flash 
 *        API, in main sate.
 */
static void main_state_err_busy_process(void)
{
    // Reissue the request by doing a self transition to the current state.    
    m_flags &= ~MASK_FLASH_API_ERR_BUSY;
    sm_state_change(m_state);                    
}


/**@brief Function for doing erase state action upon flash operation success event.
 *
 * @details Function for doing erase state action upon flash operation success event, which includes 
 *          making a state transition depending on the current state.
 */
static void erase_sub_state_sm_run(void)
{
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {        
        // Clear operation request has succeeded.
        ++m_current_page_id;                        
                
        if (!is_page_erase_required())
        {
            clear_post_processing_run();
        }
        else
        {   
            // All required flash pages have not yet been erased, issue erase by doing a self 
            // transit. 
            sm_state_change(m_state);        
        }                              
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        main_state_err_busy_process();
    }
}


/**@brief Function for doing store state action upon flash operation success event.
 */
static void store_sub_state_sm_run(void)
{
    if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
    {        
        // As write operation request has succeeded, adjust the size tracking state information 
        // accordingly.
        cmd_queue_element_t * p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];    
        p_cmd->size                -= m_num_of_bytes_written;

        if (p_cmd->size == 0)
        {
            command_end_procedure_run();
        }
        else
        {
            store_cmd_flash_write_execute();
        }
    }
    else
    {
        // As operation request was rejected by the flash API reissue the request.
        main_state_err_busy_process();
    }
}


/**@brief Function for doing action upon flash operation success event.
 */
static void flash_operation_success_run(void)
{    
    switch (m_state)
    {                   
        case STATE_STORE:
            store_sub_state_sm_run();
            break;
                        
        case STATE_DATA_ERASE:
            erase_sub_state_sm_run();
            break;
                        
        case STATE_DATA_ERASE_WITH_SWAP:
            swap_sub_state_sm_run();                        
            break;                        
            
        default:
            // No implementation needed.
            break;
    }                    
}


/**@brief Function for doing action upon flash operation failure event.
 *
 * @details Function for doing action upon flash operation failure event, which includes retrying 
 *          the last operation or if retry count has been reached completing the operation with 
 *          appropriate result code and transitioning to an error state.
 *
 * @note    The command is not removed from the command queue, which will result to stalling of the 
 *          command pipeline and the appropriate application recovery procedure for this is to reset 
 *          the system by issuing @ref pstorage_init which will also result to flushing of the 
 *          command queue.
 */
static void flash_operation_failure_run(void)
{   
    if (++m_num_of_command_retries != SD_CMD_MAX_TRIES)
    {
        // Retry the last operation by doing a self transition to the current state.
            
        if (m_state != STATE_DATA_ERASE_WITH_SWAP)
        {
            sm_state_change(m_state);        
        }
        else
        {
            swap_sub_state_state_change(m_swap_sub_state);
        }
    }
    else
    {
        // Complete the operation with appropriate result code and transit to an error state.     
        app_notify_error_state_transit(NRF_ERROR_TIMEOUT);        
    }   
}


/**@brief Function for handling flash access result events.
 *
 * @param[in] sys_evt System event to be handled.
 */
void pstorage_sys_event_handler(uint32_t sys_evt)
{  
    if (m_state != STATE_IDLE && m_state != STATE_ERROR)
    {        
        switch (sys_evt)
        {
            case NRF_EVT_FLASH_OPERATION_SUCCESS:
                flash_operation_success_run();
                break;
            
            case NRF_EVT_FLASH_OPERATION_ERROR:            
                if (!(m_flags & MASK_FLASH_API_ERR_BUSY))
                {
                    flash_operation_failure_run();
                }
                else
                {
                    // As our last flash operation request was rejected by the flash API reissue the 
                    // request by doing same code execution path as for flash operation sucess 
                    // event. This will promote code reuse in the implementation.                    
                    flash_operation_success_run();
                }                
                break;
                
            default:
                // No implementation needed.
                break;
        }

    }
}


/**@brief Function for calculating the tail area size in number of 32-bit words.
 *
 * @param[in] cmd_end_of_storage_address End of storage area within the scope of the command.
 * @param[in] end_of_storage_address     End of allocated storage area for the application.
 */
static void tail_word_size_calculate(pstorage_size_t cmd_end_of_storage_address, 
                                     pstorage_size_t end_of_storage_address)
{ 
    // Two different cases to resolve when calculating correct size for restore tail section:
    // 1) End of storage area and command end area are in the same page.
    // 2) End of storage area and command end area are not in the same page.
    
    const uint32_t end_of_storage_area_page         = end_of_storage_address     / 
                                                      PSTORAGE_FLASH_PAGE_SIZE;
    const uint32_t command_end_of_storage_area_page = cmd_end_of_storage_address / 
                                                      PSTORAGE_FLASH_PAGE_SIZE;

    if (end_of_storage_area_page == command_end_of_storage_area_page)
    {
        //lint -e{573} suppress "Signed-unsigned mix with divide".
        m_tail_word_size = (end_of_storage_address - cmd_end_of_storage_address) / sizeof(uint32_t);                                           
    }
    else
    {
        //lint -e{573} suppress "Signed-unsigned mix with divide".    
        m_tail_word_size = (PSTORAGE_FLASH_PAGE_SIZE - 
                           (cmd_end_of_storage_address % PSTORAGE_FLASH_PAGE_SIZE)) / 
                           sizeof(uint32_t);               
    }            
}


/**@brief Function for executing the clear operation.
 */
static void clear_operation_execute(void)
{    
    const cmd_queue_element_t * p_cmd        = &m_cmd_queue.cmd[m_cmd_queue.rp];
    const pstorage_block_t      cmd_block_id = p_cmd->storage_addr.block_id;

    const pstorage_size_t  block_size    = m_app_table[p_cmd->storage_addr.module_id].block_size;
    const pstorage_size_t  block_count   = m_app_table[p_cmd->storage_addr.module_id].block_count;
    const pstorage_block_t block_base_id = m_app_table[p_cmd->storage_addr.module_id].base_id;  
      
    const bool is_start_address_page_aligned = (cmd_block_id % PSTORAGE_FLASH_PAGE_SIZE) == 0;
    
    // Calculate the end (1 beyond allocated area) for complete storage area and to the area only 
    // within scope of this command.
    const pstorage_block_t end_of_storage_address     = block_base_id + (block_size * block_count); 
    const pstorage_block_t cmd_end_of_storage_address = cmd_block_id + p_cmd->size + p_cmd->offset;

    // Zero tail to make sure no extra erase is done erroneously.
    m_tail_word_size = 0;        
        
    // If the following is true no swap access is needed:
    // - 1st logical test covers the case of: clear/update 1 complete single page.
    // - 2nd logical test covers the case of: 
    //   1) Clear/update last allocated page and page is not full (page can't be shared between 
    //      multiple clients so the end of the page is unused area).
    //   2) Clear/update all allocated storage.
    if ((is_start_address_page_aligned && (p_cmd->size == PSTORAGE_FLASH_PAGE_SIZE)) ||
        (is_start_address_page_aligned && (cmd_end_of_storage_address == end_of_storage_address) && 
        (p_cmd->offset == 0)) || (p_cmd->storage_addr.module_id == RAW_MODE_APP_ID)) 
    {
        // Nothing to put to the swap and we can just erase the pages(s).         
        
        m_current_page_id = cmd_block_id / PSTORAGE_FLASH_PAGE_SIZE;
        
        sm_state_change(STATE_DATA_ERASE);                        
    }
    else
    {
        // Not all the blocks for the module can be cleared, we need to use swap page for storing 
        // data temporarily.                        
        
        m_head_word_size = ((cmd_block_id + p_cmd->offset) % PSTORAGE_FLASH_PAGE_SIZE) / 
                           sizeof(uint32_t);
        
        const bool is_cmd_end_address_page_aligned = ((cmd_end_of_storage_address % 
                                                      PSTORAGE_FLASH_PAGE_SIZE) == 0);
        if ((cmd_end_of_storage_address != end_of_storage_address) && 
            !is_cmd_end_address_page_aligned)
        {
            // When command area is not equal to end of the storage allocation area and not ending 
            // to page boundary there is a need to restore the tail area.
            tail_word_size_calculate(cmd_end_of_storage_address, end_of_storage_address);
        }

        sm_state_change(STATE_DATA_ERASE_WITH_SWAP);         
    }        
}


/**@brief Function for executing the store operation.
 */
static void store_operation_execute(void)
{    
    sm_state_change(STATE_STORE);
}
 

/**@brief Function for executing the update operation.
 */ 
static void update_operation_execute(void)
{
    clear_operation_execute();
}


/**@brief Function for dispatching the flash access operation.
 */  
static void cmd_process(void)
{
    const cmd_queue_element_t * p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];
    m_app_data_size                   = p_cmd->size;

    switch (p_cmd->op_code)
    {
        case PSTORAGE_STORE_OP_CODE:                   
            store_operation_execute();       
            break;

        case PSTORAGE_CLEAR_OP_CODE:
            clear_operation_execute();
            break;

        case PSTORAGE_UPDATE_OP_CODE:
            update_operation_execute();
            break;

        default:
            // No action required.
            break;
    }
}


uint32_t pstorage_init(void)
{
    cmd_queue_init();

    m_next_app_instance = 0;
    m_next_page_addr    = PSTORAGE_DATA_START_ADDR;
    m_current_page_id   = 0;
    
    for (uint32_t index = 0; index < PSTORAGE_NUM_OF_PAGES; index++)
    {
        m_app_table[index].cb           = NULL;
        m_app_table[index].block_size   = 0;
        m_app_table[index].block_count  = 0;
    }

#ifdef PSTORAGE_RAW_MODE_ENABLE
    m_raw_app_table.cb           = NULL;
#endif //PSTORAGE_RAW_MODE_ENABLE

    m_state                     = STATE_IDLE;
    m_num_of_command_retries    = 0;
    m_flags                     = 0;
    m_num_of_bytes_written      = 0;
    m_flags                    |= MASK_MODULE_INITIALIZED;
       
    return NRF_SUCCESS;
}


uint32_t pstorage_register(pstorage_module_param_t * p_module_param,
                           pstorage_handle_t       * p_block_id)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_module_param);
    NULL_PARAM_CHECK(p_block_id);
    NULL_PARAM_CHECK(p_module_param->cb);
    BLOCK_SIZE_CHECK(p_module_param->block_size);    
    BLOCK_COUNT_CHECK(p_module_param->block_count, p_module_param->block_size);

    if (!((p_module_param->block_size % sizeof(uint32_t)) == 0))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    if (m_next_app_instance == PSTORAGE_NUM_OF_PAGES)
    {
        return NRF_ERROR_NO_MEM;
    }

    p_block_id->module_id = m_next_app_instance;
    p_block_id->block_id  = m_next_page_addr;

    m_app_table[m_next_app_instance].base_id     = p_block_id->block_id;
    m_app_table[m_next_app_instance].cb          = p_module_param->cb;
    m_app_table[m_next_app_instance].block_size  = p_module_param->block_size;
    m_app_table[m_next_app_instance].block_count = p_module_param->block_count;

    // Calculate number of flash pages allocated for the device and adjust next free page address.
    /*lint -save -e666 */
    const uint32_t page_count = CEIL_DIV((p_module_param->block_size * p_module_param->block_count), 
                                         PSTORAGE_FLASH_PAGE_SIZE);
    /*lint -restore */
    m_next_page_addr         += page_count * PSTORAGE_FLASH_PAGE_SIZE;
    
    ++m_next_app_instance;

    return NRF_SUCCESS;
}


uint32_t pstorage_block_identifier_get(pstorage_handle_t * p_base_id,
                                       pstorage_size_t     block_num,
                                       pstorage_handle_t * p_block_id)
{
    pstorage_handle_t temp_id;

    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_base_id);
    NULL_PARAM_CHECK(p_block_id);
    MODULE_ID_RANGE_CHECK(p_base_id);

    temp_id           = (*p_base_id);
    temp_id.block_id += (block_num * MODULE_BLOCK_SIZE(p_base_id));

    BLOCK_ID_RANGE_CHECK(&temp_id);

    (*p_block_id) = temp_id;

    return NRF_SUCCESS;
}


uint32_t pstorage_store(pstorage_handle_t * p_dest,
                        uint8_t           * p_src,
                        pstorage_size_t     size,
                        pstorage_size_t     offset)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_src);
    NULL_PARAM_CHECK(p_dest);
    MODULE_ID_RANGE_CHECK(p_dest);
    BLOCK_ID_RANGE_CHECK(p_dest);
    SIZE_CHECK(p_dest, size);    
    OFFSET_CHECK(p_dest, offset, size);
    
    if ((!is_word_aligned(p_src))                    || 
        (!is_word_aligned((void *)(uint32_t)offset)) || 
        (!is_word_aligned((uint32_t *)p_dest->block_id)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    return cmd_queue_enqueue(PSTORAGE_STORE_OP_CODE, p_dest, p_src, size, offset);
}


uint32_t pstorage_update(pstorage_handle_t * p_dest,
                         uint8_t           * p_src,
                         pstorage_size_t     size,
                         pstorage_size_t     offset)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_src);
    NULL_PARAM_CHECK(p_dest);
    MODULE_ID_RANGE_CHECK(p_dest);
    BLOCK_ID_RANGE_CHECK(p_dest);
    SIZE_CHECK(p_dest, size);
    OFFSET_CHECK(p_dest, offset, size);

    if ((!is_word_aligned(p_src))                    || 
        (!is_word_aligned((void *)(uint32_t)offset)) || 
        (!is_word_aligned((uint32_t *)p_dest->block_id)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    return cmd_queue_enqueue(PSTORAGE_UPDATE_OP_CODE, p_dest, p_src, size, offset);
}


uint32_t pstorage_load(uint8_t           * p_dest,
                       pstorage_handle_t * p_src,
                       pstorage_size_t     size,
                       pstorage_size_t     offset)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_src);
    NULL_PARAM_CHECK(p_dest);
    MODULE_ID_RANGE_CHECK(p_src);
    BLOCK_ID_RANGE_CHECK(p_src);
    SIZE_CHECK(p_src, size);
    OFFSET_CHECK(p_src, offset, size);

    if ((!is_word_aligned(p_dest))                   || 
        (!is_word_aligned((void *)(uint32_t)offset)) || 
        (!is_word_aligned((uint32_t *)p_src->block_id)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    memcpy(p_dest, (((uint8_t *)p_src->block_id) + offset), size);

    m_app_table[p_src->module_id].cb(p_src, PSTORAGE_LOAD_OP_CODE, NRF_SUCCESS, p_dest, size);

    return NRF_SUCCESS;
}


uint32_t pstorage_clear(pstorage_handle_t * p_dest, pstorage_size_t size)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_dest);
    MODULE_ID_RANGE_CHECK(p_dest);
    BLOCK_ID_RANGE_CHECK(p_dest);

    if ((!is_word_aligned((uint32_t *)p_dest->block_id)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    // Check is the area starting from block_id multiple of block_size.
    if (
        !(
            ((p_dest->block_id - m_app_table[p_dest->module_id].base_id) %
             m_app_table[p_dest->module_id].block_size) == 0
            )
        )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Check is requested size multiple of registered block size or 0.
    if (((size % m_app_table[p_dest->module_id].block_size) != 0) || (size == 0))    
    {        
        return NRF_ERROR_INVALID_PARAM;        
    }

    const uint32_t registered_allocation_size = m_app_table[p_dest->module_id].block_size * 
                                                m_app_table[p_dest->module_id].block_count;
    
    const pstorage_block_t clear_request_end_address = p_dest->block_id + size;
    const pstorage_block_t allocation_end_address    = m_app_table[p_dest->module_id].base_id + 
                                                       registered_allocation_size;
    // Check if request would lead to a buffer overrun.                                                       
    if (clear_request_end_address > allocation_end_address)
    {        
        return NRF_ERROR_INVALID_PARAM;            
    }
    
    return cmd_queue_enqueue(PSTORAGE_CLEAR_OP_CODE, p_dest, NULL, size, 0);
}


uint32_t pstorage_access_status_get(uint32_t * p_count)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_count);

    (*p_count) = m_cmd_queue.count;

    return NRF_SUCCESS;
}

#ifdef PSTORAGE_RAW_MODE_ENABLE

uint32_t pstorage_raw_register(pstorage_module_param_t * p_module_param,
                               pstorage_handle_t       * p_block_id)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_module_param);
    NULL_PARAM_CHECK(p_block_id);
    NULL_PARAM_CHECK(p_module_param->cb);

    if (m_raw_app_table.cb != NULL)
    {
        return NRF_ERROR_NO_MEM;
    }

    p_block_id->module_id = RAW_MODE_APP_ID;
    m_raw_app_table.cb    = p_module_param->cb;

    return NRF_SUCCESS;
}


uint32_t pstorage_raw_store(pstorage_handle_t * p_dest,
                            uint8_t           * p_src,
                            pstorage_size_t     size,
                            pstorage_size_t     offset)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_src);
    NULL_PARAM_CHECK(p_dest);
    MODULE_RAW_HANDLE_CHECK(p_dest);
    
    if (size == 0)
    {
        return NRF_ERROR_INVALID_PARAM;        
    }
    
    // Verify word alignment.
    if ((!is_word_aligned(p_src))                    || 
        (!is_word_aligned((void *)(uint32_t)size))   ||     
        (!is_word_aligned((void *)(uint32_t)offset)) || 
        (!is_word_aligned((void *)(p_dest->block_id))))
    {
        return NRF_ERROR_INVALID_ADDR;
    }
       
    return cmd_queue_enqueue(PSTORAGE_STORE_OP_CODE, p_dest, p_src, size, offset);
}


uint32_t pstorage_raw_clear(pstorage_handle_t * p_dest, pstorage_size_t size)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_dest);
    MODULE_RAW_HANDLE_CHECK(p_dest);
    
    if ((!is_word_aligned((uint32_t *)p_dest->block_id)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }    

    return cmd_queue_enqueue(PSTORAGE_CLEAR_OP_CODE, p_dest, NULL, size, 0);
}

#endif // PSTORAGE_RAW_MODE_ENABLE
