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


/**@file
 *
 * @defgroup persistent_storage Persistent Storage Interface
 * @{
 * @ingroup app_common
 * @brief Abstracted flash interface.
 *
 * @details  An abstracted interface is provided by the module to easily port the application and 
 *           SDK modules to an alternate option. This ensures that the SDK and application are moved 
 *           to alternate persistent storage instead of the one provided by default.
 */

#ifndef PSTORAGE_H__
#define PSTORAGE_H__

#include "pstorage_platform.h"


/**@defgroup ps_opcode Persistent Storage Access Operation Codes
 * @{
 * @brief    Persistent Storage Access Operation Codes. 
 *
 * @details  Persistent Storage Access Operation Codes are used by Persistent storage operation 
 *           completion callback @ref pstorage_ntf_cb_t to identify the operation type requested by 
 *           the application.
 */
#define PSTORAGE_STORE_OP_CODE    0x01  /**< Store Operation type. */
#define PSTORAGE_LOAD_OP_CODE     0x02  /**< Load Operation type. */
#define PSTORAGE_CLEAR_OP_CODE    0x03  /**< Clear Operation type. */
#define PSTORAGE_UPDATE_OP_CODE   0x04  /**< Update Operation type. */

/**@} */

/**@defgroup pstorage_data_types Persistent Memory Interface Data Types
 * @{
 * @brief Data Types needed for interfacing with persistent memory.
 *
 * @details Data Types needed for interfacing with persistent memory.
 */

/**@brief Persistent storage operation completion callback function type.
 *
 * @details The persistent storage operation completion callback is used by the interface to report
 *          success or failure of a flash operation. Since data is not copied for a store operation, 
 *          a callback is an indication that the resident memory can now be reused or freed.
 * 
 * @param[in] handle   Identifies the module and block for the callback that is received.
 * @param[in] op_code  Identifies the operation for the event that is notified.
 * @param[in] result   Identifies the result of a flash access operation. NRF_SUCCESS implies 
 *                     operation succeeded.
 *
 *                     @note Unmanaged (abnormal behaviour) error codes from the SoftDevice flash 
 *                     access API are forwarded as is and are expected to be handled by the 
 *                     application. For details refer to the implementation file and corresponding 
 *                     SoftDevice flash API documentation.
 *                     
 * @param[in] p_data   Identifies the application data pointer. For a store operation, this points 
 *                     to the resident source of application memory that the application can now 
 *                     free or reuse. When there is a clear operation, this is NULL since no 
 *                     application pointer is needed for this operation.
 * @param[in] data_len Length data the application provided for the operation. 
 */
typedef void (*pstorage_ntf_cb_t)(pstorage_handle_t * p_handle,
                                  uint8_t             op_code,
                                  uint32_t            result,
                                  uint8_t *           p_data,
                                  uint32_t            data_len);

/**@brief Struct containing module registration context. */
typedef struct
{
    pstorage_ntf_cb_t cb;             /**< Persistent storage operation completion callback function @ref pstorage_ntf_cb_t.  */
    pstorage_size_t   block_size;     /**< Desired block size for persistent memory storage. For example, if a module has a table with 10 entries, and each entry is 64 bytes in size,
                                       *   it can request 10 blocks with a block size of 64 bytes. The module can also request one block that is 640 bytes depending 
                                       *   on how it would like to access or alter the memory in persistent memory.
                                       *   The first option is preferred when it is a single entry that needs to be updated often and doesn't impact the other entries.
                                       *   The second option is preferred when table entries are not changed individually but have a common point of loading and storing
                                       *   data. */
    pstorage_size_t   block_count;    /** Number of blocks requested by the module; minimum values is 1. */
} pstorage_module_param_t;

/**@} */

/**@defgroup pstorage_routines Persistent Storage Access Routines
 * @{
 * @brief Functions/Interface SDK modules used to persistently store data.
 *
 * @details Interface for the Application and SDK modules to load/store information persistently.
 *          Note: While implementation of each of the persistent storage access functions
 *          depends on the system and is specific to system/solution, the signature of the
 *          interface routines should not be altered.
 */

/**@brief Function for initializing the module.
 *
 * @details Function for initializing the module. This function is called once before any other APIs 
 *          of the module are used.
 *
 * @retval     NRF_SUCCESS             Operation success.
 */
uint32_t pstorage_init(void);

/**@brief Function for registering with persistent storage interface.
 *
 * @param[in]  p_module_param Module registration parameter.
 * @param[out] p_block_id     Block identifier to identify persistent memory blocks when 
 *                            registration succeeds. Application is expected to use the block IDs 
 *                            for subsequent operations on requested persistent memory. Maximum 
 *                            registrations permitted is determined by the configuration of the 
 *                            parameter PSTORAGE_NUM_OF_PAGES. If more than one memory block is 
 *                            requested, the identifier provided here is the base identifier for the 
 *                            first block and used to identify the subsequent block. The application 
 *                            uses \@ref pstorage_block_identifier_get with this base identifier and 
 *                            block number. Therefore if 10 blocks of size 64 are requested and the 
 *                            application wishes to store memory in the 6th block, it shall use
 *                            \@ref pstorage_block_identifier_get with the base ID and provide a 
 *                            block number of 5. This way the application is only expected to 
 *                            remember the base block identifier.
 *
 * @retval     NRF_SUCCESS             Operation success.
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. Additional registrations can't be 
 *                                     supported.
 */
uint32_t pstorage_register(pstorage_module_param_t * p_module_param,
                           pstorage_handle_t *       p_block_id);

/**@brief Function for getting block ID with reference to base block identifier provided at the time 
 *        of registration.
 *
 * @details Function to get the block ID with reference to base block identifier provided at the 
 *          time of registration.
 *          If more than one memory block was requested when registering, the identifier provided 
 *          here is the base identifier for the first block which is used to identify subsequent
 *          blocks. The application shall use this routine to get the block identifier, providing 
 *          input as base identifier and block number. Therefore, if 10 blocks of size 64 are 
 *          requested and the application wishes to store memory in the 6th block, it shall use
 *          \@ref pstorage_block_identifier_get with the base ID and provide a block number of 5.
 *          This way the application is only expected to remember the base block identifier.
 *
 * @param[in]  p_base_id  Base block ID received at the time of registration.
 * @param[in]  block_num  Block Number, with first block numbered zero.
 * @param[out] p_block_id Block identifier for the block number requested when the API succeeds.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 */
uint32_t pstorage_block_identifier_get(pstorage_handle_t * p_base_id,
                                       pstorage_size_t     block_num,
                                       pstorage_handle_t * p_block_id);

/**@brief Function for persistently storing data of length 'size' contained in the 'p_src' address
 *        in the storage module at 'p_dest' address. Equivalent to Storage Write.
 *
 * @param[in]  p_dest Destination address where data is to be stored persistently.
 * @param[in]  p_src  Source address containing data to be stored. API assumes this to be resident
 *                    memory and no intermediate copy of data is made by the API. Must be word 
 *                    aligned.
 * @param[in]  size   Size of data to be stored expressed in bytes. Must be word aligned and size + 
 *                    offset must be <= block size.                      
 * @param[in]  offset Offset in bytes to be applied when writing to the block.
 *                    For example, if within a block of 100 bytes, the application wishes to
 *                    write 20 bytes at an offset of 12, then this field should be set to 12.
 *                    Must be word aligned.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_INVALID_ADDR  Operation failure. Parameter is not aligned.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 *
 * @warning    No copy of the data is made, meaning memory provided for the data source that is to 
 *             be written to flash cannot be freed or reused by the application until this procedure
 *             is complete. The application is notified when the procedure is finished using the
 *             notification callback registered by the application.
 */
uint32_t pstorage_store(pstorage_handle_t * p_dest,
                        uint8_t *           p_src,
                        pstorage_size_t     size,
                        pstorage_size_t     offset);

/**@brief Function for updating persistently stored data of length 'size' contained in the 'p_src' 
 *        address in the storage module at 'p_dest' address.
 *
 * @param[in]  p_dest Destination address where data is to be updated.
 * @param[in]  p_src  Source address containing data to be stored. API assumes this to be resident
 *                    memory and no intermediate copy of data is made by the API.
 * @param[in]  size   Size of data to be stored expressed in bytes. Must be word aligned and size + 
 *                    offset must be <= block size.
 * @param[in]  offset Offset in bytes to be applied when writing to the block.
 *                    For example, if within a block of 100 bytes, the application wishes to
 *                    write 20 bytes at an offset of 12 bytes, then this field should be set to 12.
 *                    Must be word aligned.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_INVALID_ADDR  Operation failure. Parameter is not aligned.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 *
 * @warning    No copy of the data is made, meaning memory provided for the data source that is to 
 *             be written to flash cannot be freed or reused by the application until this procedure
 *             is complete. The application is notified when the procedure is finished using the
 *             notification callback registered by the application.
 */
uint32_t pstorage_update(pstorage_handle_t * p_dest,
                         uint8_t *           p_src,
                         pstorage_size_t     size,
                         pstorage_size_t     offset);

/**@brief Function for loading persistently stored data of length 'size' from 'p_src' address
 *        to 'p_dest' address. Equivalent to Storage Read.
 *
 * @param[in]  p_dest Destination address where persistently stored data is to be loaded.
 * @param[in]  p_src  Source where data is loaded from persistent memory.
 * @param[in]  size   Size of data to be loaded from persistent memory expressed in bytes.
 *                    Should be word aligned.
 * @param[in]  offset Offset in bytes, to be applied when loading from the block.
 *                    For example, if within a block of 100 bytes, the application wishes to
 *                    load 20 bytes from offset of 12 bytes, then this field should be set to 12.
 *                    Should be word aligned.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_INVALID_ADDR  Operation failure. Parameter is not aligned.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 */
uint32_t pstorage_load(uint8_t *           p_dest,
                       pstorage_handle_t * p_src,
                       pstorage_size_t     size,
                       pstorage_size_t     offset);

/**@brief Function for clearing data in persistent memory.
 *
 * @param[in]  p_base_id Base block identifier in persistent memory that needs to be cleared;
 *                       equivalent to an Erase Operation.
 * @param[in]  size      Size of data to be cleared from persistent memory expressed in bytes.
 *                       This parameter is to provision for clearing of certain blocks
 *                       of memory, or all memory blocks in a registered module. If the total size 
 *                       of the application module is used (blocks * block size) in combination with
 *                       the identifier for the first block in the module, all blocks in the 
 *                       module will be erased. Must be multiple of block size.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_INVALID_ADDR  Operation failure. Parameter is not aligned.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 *
 * @note       Clear operations may take time. This API however, does not block until the clear
 *             procedure is complete. The application is notified of procedure completion using
 *             a notification callback registered by the application. The 'result' parameter of the
 *             callback indicates if the procedure was successful or not.
 */
uint32_t pstorage_clear(pstorage_handle_t * p_base_id, pstorage_size_t size);

/**@brief Function for getting the number of pending operations with the module.
 *
 * @param[out] p_count Number of storage operations pending with the module. If 0, there are no 
 *                     outstanding requests.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 */
uint32_t pstorage_access_status_get(uint32_t * p_count);

#ifdef PSTORAGE_RAW_MODE_ENABLE

/**@brief Function for registering with the persistent storage interface.
 *
 * @param[in]  p_module_param Module registration parameter.
 * @param[out] p_block_id     Block identifier used to identify persistent memory blocks upon 
 *                            successful registration. The application is expected to use the block 
 *                            IDs for subsequent operations on requested persistent memory. When 
 *                            more than one memory block is requested, this identifier is the base 
 *                            identifier for the first block and used to identify subsequent blocks. 
 *                            The application shall use \@ref pstorage_block_identifier_get with 
 *                            this base identifier and block number. Therefore if 10 blocks of size 
 *                            64 are requested and the application wishes to store memory in the 6th 
 *                            block, it shall use \@ref pstorage_block_identifier_get with the base 
 *                            ID and provide a block number of 5. Therefore, the application is only 
 *                            expected to remember the base block identifier.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 */
uint32_t pstorage_raw_register(pstorage_module_param_t * p_module_param,
                               pstorage_handle_t *       p_block_id);

/**@brief Function for persistently storing data of length 'size' contained in 'p_src' address in 
 *        storage module at 'p_dest' address. Equivalent to Storage Write.
 *
 * @param[in]  p_dest Destination address where data is to be stored persistently.
 * @param[in]  p_src  Source address containing data to be stored. The API assumes this is resident
 *                    memory and no intermediate copy of data is made by the API. Must be word 
 *                    aligned.
 * @param[in]  size   Size of data to be stored expressed in bytes. Must be word aligned.
 * @param[in]  offset Offset in bytes to be applied when writing to the block.
 *                    For example, if within a block of 100 bytes, the application wishes to
 *                    write 20 bytes at an offset of 12 bytes, this field should be set to 12.
 *                    Must be word aligned.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_INVALID_ADDR  Operation failure. Parameter is not aligned.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 *
 * @warning    No copy of the data is made, meaning memory provided for data source that is to be 
 *             written to flash cannot be freed or reused by the application until this procedure
 *             is complete. The application is notified when the procedure is finished using the
 *             notification callback registered by the application.
 */
uint32_t pstorage_raw_store(pstorage_handle_t * p_dest,
                            uint8_t *           p_src,
                            pstorage_size_t     size,
                            pstorage_size_t     offset);

/**@brief Function for clearing data in persistent memory in raw mode.
 *
 * @param[in]  p_dest Base block identifier in persistent memory that needs to be cleared.
 *                    Equivalent to an Erase Operation.
 * @param[in]  size   Size of data to be cleared from persistent memory expressed in bytes. 
 *                    Not used.
 *
 * @retval     NRF_SUCCESS             Operation success. 
 * @retval     NRF_ERROR_INVALID_STATE Operation failure. API is called without module 
 *                                     initialization.
 * @retval     NRF_ERROR_NULL          Operation failure. NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM Operation failure. Invalid parameter has been passed.
 * @retval     NRF_ERROR_NO_MEM        Operation failure. No storage space available.
 *
 * @note       Clear operations may take time. This API, however, does not block until the clear
 *             procedure is complete. The application is notified of procedure completion using
 *             a notification callback registered by the application. The 'result' parameter of the
 *             callback indicates if the procedure was successful or not.
 */
uint32_t pstorage_raw_clear(pstorage_handle_t * p_dest, pstorage_size_t size);

#endif // PSTORAGE_RAW_MODE_ENABLE

/**@} */
/**@} */

#endif // PSTORAGE_H__

