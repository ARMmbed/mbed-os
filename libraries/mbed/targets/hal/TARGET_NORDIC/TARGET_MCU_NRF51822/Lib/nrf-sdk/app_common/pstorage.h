/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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

/**@file
 *
 * @defgroup persistent_storage Persistent Storage Interface
 * @{
 * @ingroup app_common
 * @brief Abstracted flash interface.
 *
 * @details In order to ensure that the SDK and application be moved to alternate persistent storage
 *          options other than the default provided with NRF solution, an abstracted interface is provided
 *          by the module to ensure SDK modules and application can be ported to alternate option with ease.
 */

#ifndef PSTORAGE_H__
#define PSTORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* #ifdef __cplusplus */

#include "pstorage_platform.h"


/**@defgroup ps_opcode Persistent Storage Access Operation Codes
 * @{
 * @brief    Persistent Storage Access Operation Codes. These are used to report any error during
 *           a persistent storage access operation or any general error that may occur in the
 *           interface.
 *
 * @details  Persistent Storage Access Operation Codes used in error notification callback
 *           registered with the interface to report any error during an persistent storage access
 *           operation or any general error that may occur in the interface.
 */
#define PSTORAGE_ERROR_OP_CODE    0x01  /**< General Error Code */
#define PSTORAGE_STORE_OP_CODE    0x02  /**< Error when Store Operation was requested */
#define PSTORAGE_LOAD_OP_CODE     0x03  /**< Error when Load Operation was requested */
#define PSTORAGE_CLEAR_OP_CODE    0x04  /**< Error when Clear Operation was requested */
#define PSTORAGE_UPDATE_OP_CODE   0x05  /**< Update an already touched storage block */

/**@} */

/**@defgroup pstorage_data_types Persistent Memory Interface Data Types
 * @{
 * @brief Data Types needed for interfacing with persistent memory.
 *
 * @details Data Types needed for interfacing with persistent memory.
 */

/**@brief Persistent Storage Error Reporting Callback
 *
 * @details Persistent Storage Error Reporting Callback that is used by the interface to report
 *          success or failure of a flash operation. Therefore, for any operations, application
 *          can know when the procedure was complete. For store operation, since no data copy
 *          is made, receiving a success or failure notification, indicated by the reason
 *          parameter of callback is an indication that the resident memory could now be reused
 *          or freed, as the case may be.
 *
 * @param[in] handle   Identifies module and block for which callback is received.
 * @param[in] op_code  Identifies the operation for which the event is notified.
 * @param[in] result   Identifies the result of flash access operation.
 *                     NRF_SUCCESS implies, operation succeeded.
 * @param[in] p_data   Identifies the application data pointer. In case of store operation, this
 *                     points to the resident source of application memory that application can now
 *                     free or reuse. In case of clear, this is NULL as no application pointer is
 *                     needed for this operation.
 * @param[in] data_len Length data application had provided for the operation.
 *
 */
typedef void (*pstorage_ntf_cb_t)(pstorage_handle_t *  p_handle,
                                  uint8_t              op_code,
                                  uint32_t             result,
                                  uint8_t *            p_data,
                                  uint32_t             data_len);


typedef struct
{
    pstorage_ntf_cb_t cb;             /**< Callback registered with the module to be notified of any error occurring in persistent memory management */
    pstorage_size_t   block_size;     /**< Desired block size for persistent memory storage, for example, if a module has a table with 10 entries, each entry is size 64 bytes,
                                       *   it can request 10 blocks with block size 64 bytes. On the other hand, the module can also request one block of size 640 based on
                                       *   how it would like to access or alter memory in persistent memory.
                                       *   First option is preferred when single entries that need to be updated often when having no impact on the other entries.
                                       *   While second option is preferred when entries of table are not changed on individually but have common point of loading and storing
                                       *   data. */
    pstorage_size_t   block_count;    /** Number of blocks requested by the module, minimum values is 1. */
} pstorage_module_param_t;

/**@} */

/**@defgroup pstorage_routines Persistent Storage Access Routines
 * @{
 * @brief Functions/Interface SDK modules use to persistently store data.
 *
 * @details Interface for Application & SDK module to load/store information persistently.
 *          Note: that while implementation of each of the persistent storage access function
 *          depends on the system and can specific to system/solution, the signature of the
 *          interface routines should not be altered.
 */

/**@brief Module Initialization Routine.
 *
 * @details Initializes module. To be called once before any other APIs of the module are used.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 */
uint32_t pstorage_init(void);


/**@brief Register with persistent storage interface.
 *
 * @param[in]  p_module_param Module registration param.
 * @param[out] p_block_id     Block identifier to identify persistent memory blocks in case
 *                            registration succeeds. Application is expected to use the block ids
 *                            for subsequent operations on requested persistent memory. Maximum
 *                            registrations permitted is determined by configuration parameter
 *                            PSTORAGE_MAX_APPLICATIONS.
 *             In case more than one memory blocks are requested, the identifier provided here is
 *             the base identifier for the first block and to identify subsequent block,
 *             application shall use \@ref pstorage_block_identifier_get with this base identifier
 *             and block number. Therefore if 10 blocks of size 64 are requested and application
 *             wishes to store memory in 6th block, it shall use
 *             \@ref pstorage_block_identifier_get with based id and provide a block number of 5.
 *             This way application is only expected to remember the base block identifier.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_NO_MEM        in case no more registrations can be supported.
 */
uint32_t pstorage_register(pstorage_module_param_t * p_module_param,
                           pstorage_handle_t *       p_block_id);


/**
 * @brief Function to get block id with reference to base block identifier provided at time of
 *        registration.
 *
 * @details Function to get block id with reference to base block identifier provided at time of
 *          registration.
 *          In case more than one memory blocks were requested when registering, the identifier
 *          provided here is the base identifier for the first block and to identify subsequent
 *          block, application shall use this routine to get block identifier providing input as
 *          base identifier and block number. Therefore if 10 blocks of size 64 are requested and
 *          application wishes to store memory in 6th block, it shall use
 *          \@ref pstorage_block_identifier_get with based id and provide a block number of 5.
 *          This way application is only expected to remember the base block identifier.
 *
 * @param[in]  p_base_id  Base block id received at the time of registration.
 * @param[in]  block_num  Block Number, with first block numbered zero.
 * @param[out] p_block_id Block identifier for the block number requested in case the API succeeds.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 */
uint32_t pstorage_block_identifier_get(pstorage_handle_t * p_base_id,
                                       pstorage_size_t     block_num,
                                       pstorage_handle_t * p_block_id);


/**@brief Routine to persistently store data of length 'size' contained in 'p_src' address
 *        in storage module at 'p_dest' address; Equivalent to Storage Write.
 *
 * @param[in]  p_dest Destination address where data is to be stored persistently.
 * @param[in]  p_src  Source address containing data to be stored. API assumes this to be resident
 *                    memory and no intermediate copy of data is made by the API.
 * @param[in]  size   Size of data to be stored expressed in bytes. Should be word aligned.
 * @param[in]  offset Offset in bytes to be applied when writing to the block.
 *                    For example, if within a block of 100 bytes, application wishes to
 *                    write 20 bytes at offset of 12, then this field should be set to 12.
 *                    Should be word aligned.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_INVALID_ADDR  in case data address 'p_src' is not aligned.
 * @retval     NRF_ERROR_NO_MEM        in case request cannot be processed.
 *
 * @warning    No copy of the data is made, and hence memory provided for data source to be written
 *             to flash cannot be freed or reused by the application until this procedure
 *             is complete. End of this procedure is notified to the application using the
 *             notification callback registered by the application.
 */
uint32_t pstorage_store(pstorage_handle_t * p_dest,
                        uint8_t *           p_src,
                        pstorage_size_t     size,
                        pstorage_size_t     offset);

/**@brief Routine to update persistently stored data of length 'size' contained in 'p_src' address
 *        in storage module at 'p_dest' address.
 *
 * @param[in]  p_dest Destination address where data is to be updated.
 * @param[in]  p_src  Source address containing data to be stored. API assumes this to be resident
 *                    memory and no intermediate copy of data is made by the API.
 * @param[in]  size   Size of data to be stored expressed in bytes. Should be word aligned.
 * @param[in]  offset Offset in bytes to be applied when writing to the block.
 *                    For example, if within a block of 100 bytes, application wishes to
 *                    write 20 bytes at offset of 12, then this field should be set to 12.
 *                    Should be word aligned.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_INVALID_ADDR  in case data address 'p_src' is not aligned.
 * @retval     NRF_ERROR_NO_MEM        in case request cannot be processed.
 *
 * @warning    No copy of the data is made, and hence memory provided for data source to be written
 *             to flash cannot be freed or reused by the application until this procedure
 *             is complete. End of this procedure is notified to the application using the
 *             notification callback registered by the application.
 */
uint32_t pstorage_update(pstorage_handle_t * p_dest,
                         uint8_t *           p_src,
                         pstorage_size_t     size,
                         pstorage_size_t     offset);

/**@brief Routine to load persistently stored data of length 'size' from 'p_src' address
 *        to 'p_dest' address; Equivalent to Storage Read.
 *
 * @param[in]  p_dest Destination address where persistently stored data is to be loaded.
 * @param[in]  p_src  Source from where data is to be loaded from persistent memory.
 * @param[in]  size   Size of data to be loaded from persistent memory expressed in bytes.
 *                    Should be word aligned.
 * @param[in]  offset Offset in bytes to be applied when loading from the block.
 *                    For example, if within a block of 100 bytes, application wishes to
 *                    load 20 bytes from offset of 12, then this field should be set to 12.
 *                    Should be word aligned.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_INVALID_ADDR  in case data address 'p_dst' is not aligned.
 * @retval     NRF_ERROR_NO_MEM        in case request cannot be processed.
 */
uint32_t pstorage_load(uint8_t *           p_dest,
                       pstorage_handle_t * p_src,
                       pstorage_size_t     size,
                       pstorage_size_t     offset);

/**@brief Routine to clear data in persistent memory.
 *
 * @param[in]  p_base_id Base block identifier in persistent memory that needs to cleared;
 *                       Equivalent to an Erase Operation.
 *
 * @param[in]  size      Size of data to be cleared from persistent memory expressed in bytes.
 *                       This parameter is to provision for clearing of certain blocks
 *                       of memory, or all memory blocks in a registered module. If the total size
 *                       of the application module is used (blocks * block size) in combination with
 *                       the identifier for the first block in the module, all blocks in the
 *                       module will be erased.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_INVALID_ADDR  in case data address 'p_dst' is not aligned.
 * @retval     NRF_ERROR_NO_MEM        in case request cannot be processed.
 *
 * @note       Clear operations may take time. This API however, does not block until the clear
 *             procedure is complete. Application is notified of procedure completion using
 *             notification callback registered by the application. 'result' parameter of the
 *             callback suggests if the procedure was successful or not.
 */
uint32_t pstorage_clear(pstorage_handle_t * p_base_id, pstorage_size_t size);

/**
 * @brief API to get status of number of pending operations with the module.
 *
 * @param[out] p_count Number of storage operations pending with the module, if 0,
 *                     there are no outstanding requests.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 */
uint32_t pstorage_access_status_get(uint32_t * p_count);

#ifdef PSTORAGE_RAW_MODE_ENABLE

/**@brief      Function for registering with persistent storage interface.
 *
 * @param[in]  p_module_param Module registration param.
 * @param[out] p_block_id     Block identifier to identify persistent memory blocks in case
 *                            registration succeeds. Application is expected to use the block ids
 *                            for subsequent operations on requested persistent memory.
 *             In case more than one memory blocks are requested, the identifier provided here is
 *             the base identifier for the first block and to identify subsequent block,
 *             application shall use \@ref pstorage_block_identifier_get with this base identifier
 *             and block number. Therefore if 10 blocks of size 64 are requested and application
 *             wishes to store memory in 6th block, it shall use
 *             \@ref pstorage_block_identifier_get with based id and provide a block number of 5.
 *             This way application is only expected to remember the base block identifier.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_NO_MEM        in case no more registrations can be supported.
 */
uint32_t pstorage_raw_register(pstorage_module_param_t * p_module_param,
                               pstorage_handle_t *       p_block_id);

/**@brief     Raw mode function for persistently storing data of length 'size' contained in 'p_src'
 *            address in storage module at 'p_dest' address; Equivalent to Storage Write.
 *
 * @param[in]  p_dest Destination address where data is to be stored persistently.
 * @param[in]  p_src  Source address containing data to be stored. API assumes this to be resident
 *                    memory and no intermediate copy of data is made by the API.
 * @param[in]  size   Size of data to be stored expressed in bytes. Should be word aligned.
 * @param[in]  offset Offset in bytes to be applied when writing to the block.
 *                    For example, if within a block of 100 bytes, application wishes to
 *                    write 20 bytes at offset of 12, then this field should be set to 12.
 *                    Should be word aligned.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_INVALID_ADDR  in case data address 'p_src' is not aligned.
 * @retval     NRF_ERROR_NO_MEM        in case request cannot be processed.
 *
 * @warning    No copy of the data is made, and hence memory provided for data source to be written
 *             to flash cannot be freed or reused by the application until this procedure
 *             is complete. End of this procedure is notified to the application using the
 *             notification callback registered by the application.
 */
uint32_t pstorage_raw_store(pstorage_handle_t * p_dest,
                            uint8_t *           p_src,
                            pstorage_size_t     size,
                            pstorage_size_t     offset);

/**@brief      Function for clearing data in persistent memory in raw mode.
 *
 * @param[in]  p_dest Base block identifier in persistent memory that needs to cleared;
 *                    Equivalent to an Erase Operation.
 * @param[in]  size   Size of data to be cleared from persistent memory expressed in bytes.
 *                    This is currently unused. And a clear would mean clearing all blocks,
 *                    however, this parameter is to provision for clearing of certain blocks
 *                    of memory only and not all if need be.
 *
 * @retval     NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval     NRF_ERROR_INVALID_STATE is returned is API is called without module initialization.
 * @retval     NRF_ERROR_NULL          if NULL parameter has been passed.
 * @retval     NRF_ERROR_INVALID_PARAM if invalid parameters are passed to the API.
 * @retval     NRF_ERROR_NO_MEM        in case request cannot be processed.
 *
 * @note       Clear operations may take time. This API however, does not block until the clear
 *             procedure is complete. Application is notified of procedure completion using
 *             notification callback registered by the application. 'result' parameter of the
 *             callback suggests if the procedure was successful or not.
 */
uint32_t pstorage_raw_clear(pstorage_handle_t * p_dest, pstorage_size_t size);

#endif // PSTORAGE_RAW_MODE_ENABLE

#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */


/**@} */
/**@} */

#endif // PSTORAGE_H__

