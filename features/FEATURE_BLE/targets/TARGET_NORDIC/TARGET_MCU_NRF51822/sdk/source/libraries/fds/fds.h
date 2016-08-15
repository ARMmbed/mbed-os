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

#ifndef FDS_H__
#define FDS_H__

/**
 * @defgroup flash_data_storage Flash Data Storage
 * @ingroup app_common
 * @{
 * @brief Flash Data Storage (FDS).
 *
 * @details Flash Data Storage (FDS) is a minimalistic filesystem for the on-chip flash.
 *          It can be used to manipulate @e records, which consist of a piece of data, made up
 *          of one or more chunks, and an associated key pair. 
 */

#include <stdint.h>
#include <stdbool.h>
#include "sdk_errors.h"


/**@brief */
#define SIZEOF_WORDS(val)       (sizeof(val) / 4)

/**@brief Reserved type key used to flag cleared records.
 *        May not be used as a record key by the application. */
#define FDS_TYPE_ID_INVALID     (0x0000)
/**@brief Reserved instance key used to check for missing or corrupted metadata.
 *        May not be used as a record key by the application. */
#define FDS_INSTANCE_ID_INVALID (0xFFFF)


typedef uint32_t fds_record_id_t;
typedef uint16_t fds_type_id_t;
typedef uint16_t fds_length_t;
typedef uint16_t fds_instance_id_t;
typedef uint16_t fds_checksum_t;


/**@brief A piece of a record metadata, keeping information about one of its keys (type) and its
 *        lenght, expressed in 4 byte words. */
typedef struct 
{
    fds_type_id_t type;           /**< The record type ID. */
    fds_length_t  length_words;   /**< Length of the record's data, in 4 byte words. */
} fds_tl_t;


/**@brief A piece of a record metadata, keeping information about one of its keys (instance) and
 *        its checksum. */
typedef struct
{
    fds_instance_id_t instance;       /**< The record instance ID. */
    fds_checksum_t    checksum;       /**< Checksum of the entire record, including the metadata. */
} fds_ic_t;


/**@brief The record metadata. */
typedef struct
{
    fds_tl_t        tl;     /**< See @ref fds_tl_t. */
    fds_ic_t        ic;     /**< See @ref fds_ic_t. */
    fds_record_id_t id;     /**< The unique record ID (32 bits). */
} fds_header_t;


typedef fds_header_t fds_record_header_t;

/**@brief The record descriptor structure, used to manipulate a record.
 * @note  This structure is meant to be opaque to the user, who does not need to access
 *        any of its fields.
 * @note    This structure does not need special initialization.
 * @warning Do not reuse the same descriptor for different records. If you do, be sure to set
 *          its fields to zero. */
typedef struct
{
    uint32_t         record_id;     /**< The unique record ID. */
    uint32_t const * p_rec;         /**< The last known record address in flash. */
    uint16_t         vpage_id;      /**< The virtual page ID in which the record is stored. */
    uint16_t         gc_magic;      /**< Number of times the GC algorithm has been run. */
    uint16_t         ptr_magic;     /**< Used to verify the validity of p_rec. */
} fds_record_desc_t;


/**@brief The record key, used to lookup records.
 * @note  The uniqueness of either field is not enforced by the system. */
typedef struct
{
    uint16_t type;
    uint16_t instance;
} fds_record_key_t;


/**@brief Structure used for reading a record back from flash memory. */
typedef struct
{
    // TODO: the header should be a pointer.
    fds_header_t         header;        /**< The record header (metadata), as stored in flash. */
    uint32_t     const * p_data;        /**< The record data. */
} fds_record_t;


/**@brief A record chunk, containing a piece of data to be stored in a record.
 *
 * @note  p_data must be aligned on a (4 bytes) word boundary.
 */
typedef struct
{
    void         const * p_data;           /**< Pointer to the data to store. Must be word aligned. */
    fds_length_t         length_words;     /**< Length of data pointed by p_data, in 4 byte words. */
} fds_record_chunk_t;


/**@brief A token to a reserved space in flash, created by @ref fds_reserve.
 *        Use @ref fds_write_reserved to write the record in the reserved space,
 *        or @ref fds_reserve_cancel to cancel the reservation.
 */
typedef struct
{
    uint16_t            vpage_id;       /**< The virtual ID of the page where space was reserved. */
    fds_length_t        length_words;   /**< The amount of space reserved, in 4 byte words. */
} fds_write_token_t;


/**@brief A token to keep information about the progress of @ref fds_find, @ref fds_find_by_type
 *        and @ref fds_find_by_instance operations.
 * @note  This structure is meant to be opaque to the user, who does not need to access any of its
 *        fields.
 * @note  The token does not need special initialization.
 * @warning Do not reuse the same token to search for different records. If you do, be sure to set
 *          its fields to zero. */
typedef struct
{
    uint32_t const * p_addr;
    uint32_t         magic;
    uint16_t         vpage_id;
} fds_find_token_t;


typedef enum
{
    FDS_CMD_NONE,       /**< No command. */
    FDS_CMD_INIT,       /**< Module initialization commnad. Used in @ref fds_init */
    FDS_CMD_WRITE,      /**< Write command. Used in @ref fds_write and @ref fds_write_reserved. */
    FDS_CMD_UPDATE,     /**< Update command. Used in @ref fds_update. */
    FDS_CMD_CLEAR,      /**< Clear record command. Used in @ref fds_clear and @ref fds_update. */
    FDS_CMD_CLEAR_INST, /**< Clear instance command. Used in @ref fds_clear_by_instance. */
    FDS_CMD_GC          /**< Garbage collection. Used in @ref fds_gc. */
} fds_cmd_id_t;

 
/**@brief Flash data storage callback function.
 *
 * @param result     Result of the command.
 * @param cmd        The command associated with the callback.
 * @param record_id  The unique ID of the record associated with the callback.
 * @param record_key The key pair of the record associated with the callback.
 */
typedef void (*fds_cb_t)(ret_code_t       result,
                         fds_cmd_id_t     cmd,
                         fds_record_id_t  record_id,
                         fds_record_key_t record_key);


/**@brief       Function to register a callback for the module events.
 * @details     The maximum amount of callback which can be registered can be configured by
 *              changing the FDS_MAX_USERS macro in fds_config.h.
 * 
 * @param[in]   cb The callback function.
 *
 *
 * @retval      NRF_SUCCESS      Success.
 * @retval      NRF_ERROR_NO_MEM Error. Maximum number of registered callbacks reached.
 */
ret_code_t fds_register(fds_cb_t cb);


/**@brief Function to initialize the module.
 *
 * @details This function initializes the module and installs the filesystem, if it is not
 *          installed yet.
 *
 * @note    This function is asynchronous. Completion is reported with a callback through the
 *          registered event handler. To be able to receive such callback, be sure to call
 *          @ref fds_register before calling @ref fds_init.
 *
 * @retval  NRF_SUCCESS                 Success. The command was queued.
 * @retval  NRF_ERROR_INVALID_STATE     Error. The module is currently undergoing initialization.
 * @retval  NRF_ERROR_NO_MEM            Error. Insufficient space to install the filesystem, or
 *                                      insufficient resources to perform the installation.
 */
ret_code_t fds_init(void);


/**@brief Function to write a record to flash.
 *
 * @details This function can be used to write a record to flash. A record data consists of
 *          multiple chunks and is supplied to the function as an array of fds_record_chunk_t
 *          structures. The maximum lenght of a record data may not exceed the size of one flash
 *          page minus FDS_HEADER_SIZE words.
 *            
 * @note This function is asynchronous, therefore, completion is reported with a callback
 *       through the registered event handler.
 *
 * @note The record data must be aligned on a 4 byte boundary, and because it is not buffered
 *       internally, it must be kept in memory by the application until the callback for the
 *       command has been received, i.e., the command completed.
 *
 * @param[out] p_desc     The record descriptor. It may be NULL.
 * @param[in]  key        The record key pair.
 * @param[in]  num_chunks The number of elements in the chunks array.
 * @param[in]  chunks     An array of chunks making up the record data.
 *
 * @retval NRF_SUCCESS               Success. The command was queued.
 * @retval NRF_ERROR_INVALID_STATE   Error. The module is not initialized.
 * @retval NRF_ERROR_INVALID_DATA    Error. The key contains an invalid type or instance.
 * @retval NRF_ERROR_INVALID_ADDR    Error. The record data is not aligned on a 4 byte boundary.
 * @retval NRF_ERROR_INVALID_LENGTH  Error. The record length exceeds the maximum lenght.
 * @retval NRF_ERROR_BUSY            Error. Insufficient internal resources to queue the operation.
 * @retval NRF_ERROR_NO_MEM          Error. No flash space available to store the record.
 */
ret_code_t fds_write(fds_record_desc_t * const p_desc,
                     fds_record_key_t          key,
                     uint8_t                   num_chunks,
                     fds_record_chunk_t        chunks[]);


/**@brief Function to reserve space for a record.
 *
 * @details This function can be used to reserve flash space to store a record, which can be
 *          later written down using @ref fds_write_reserved. It is possible to cancel a
 *          reservation by using @ref fds_reserve_cancel.
 *
 * @param[out] p_tok        A token which can be used to write a record in the reserved space
 *                          using @ref fds_write_reserved.
 * @param[in]  length_words The lenght of the record data, in 4 byte words.
 *
 * @retval  NRF_SUCCESS             Success. Flash space was successfully reserved.
 * @retval  NRF_ERROR_NULL          Error. p_tok is NULL.
 * @retval  NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval  NRF_ERROR_NO_MEM        Error. Insufficient space.
 */
ret_code_t fds_reserve(fds_write_token_t * const p_tok, uint16_t length_words);


/**@brief Function to cancel a space reservation.
 *
 * @param[in] p_tok The token produced by @ref fds_reserve, identifying the reservation to cancel.
 *
 * @retval NRF_SUCCESS             Success. The reservation was canceled.
 * @retval NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval NRF_ERROR_NULL          Error. p_tok is NULL.
 * @retval NRF_ERROR_INVALID_DATA  Error. p_tok contains invalid data.
 */
ret_code_t fds_reserve_cancel(fds_write_token_t * const p_tok);


/**@brief Function to write a record to flash, the space for which has been previously reserved
 *        using @ref fds_reserve.
 *
 * @details This function behaves similarly to @ref fds_write, with the exception that it never
 *          fails with NRF_ERROR_NO_MEM.
 *
 * @note This function is asynchronous, therefore, completion is reported with a callback
 *       through the registered event handler.
 *
 * @note The record data must be aligned on a 4 byte boundary, and because it is not buffered
 *       internally, it must be kept in memory by the application until the callback for the
 *       command has been received, i.e., the command completed.
 *
 * @param[in]  p_tok      The token return by @ref fds_reserve.
 * @param[out] p_desc     The record descriptor. It may be NULL.
 * @param[in]  key        The record key pair.
 * @param[in]  num_chunks The number of elements in the chunks array.
 * @param[in]  chunks     An array of chunks making up the record data.
 *
 * @retval NRF_SUCCESS               Success. The command was queued.
 * @retval NRF_ERROR_INVALID_STATE   Error. The module is not initialized.
 * @retval NRF_ERROR_INVALID_DATA    Error. The key contains an invalid type or instance.
 * @retval NRF_ERROR_INVALID_ADDR    Error. The record data is not aligned on a 4 byte boundary.
 * @retval NRF_ERROR_INVALID_LENGTH  Error. The record length exceeds the maximum lenght.
 * @retval NRF_ERROR_BUSY            Error. Insufficient internal resources to queue the operation.
 */
ret_code_t fds_write_reserved(fds_write_token_t  const * const p_tok,
                              fds_record_desc_t        * const p_desc,
                              fds_record_key_t                 key,
                              uint8_t                          num_chunks,
                              fds_record_chunk_t               chunks[]);


/**@brief Function to clear a record.
 *
 * @details Clearing a record has the effect of preventing the system from retrieving the record
 *          descriptor using the @ref fds_find, @ref fds_find_by_type and @ref fds_find_by_instance
 *          functions. Additionally, @ref fds_open calls shall fail when supplied a descritpor for
 *          a record which has been cleared. Clearing a record does not free the space it occupies
 *          in flash. The reclaim flash space used by cleared records, use @ref fds_gc.
 *
 * @note    This function is asynchronous, therefore, completion is reported with a callback
 *          through the registered event handler.
 *
 * @param[in] p_desc The descriptor of the record to clear.
 *
 * @retval NRF_SUCCESS             Success. The command was queued.
 * @retval NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval NRF_ERROR_NULL          Error. p_desc is NULL.
 * @retval NRF_ERROR_BUSY          Error. Insufficient internal resources to queue the operation.
 */
ret_code_t fds_clear(fds_record_desc_t * const p_desc);


/**@brief Function to clear all records with a given instance.
 *
 * @details Clearing a record has the effect of preventing the system from retrieving the record
 *          descriptor using the @ref fds_find, @ref fds_find_by_type and @ref fds_find_by_instance
 *          functions. Additionally, @ref fds_open calls shall fail when supplied a descritpor for
 *          a record which has been cleared. Clearing a record does not free the space it occupies
 *          in flash. The reclaim flash space used by cleared records, use @ref fds_gc.
 *
 * @note This function is asynchronous, therefore, completion is reported with a callback
 *       through the registered event handler. Only one callback will be issued. The record
 *       instance ID in the key parameter of the callback will contain the instance ID passed as
 *       parameter to this function. The record ID parameter will be zero, and the type ID equal
 *       to FDS_TYPE_ID_INVALID.
 *
 * @param[in] instance The instance ID of the records to clear.
 *
 * @retval NRF_SUCCESS             Success. The command was queued.
 * @retval NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval NRF_ERROR_NULL          Error. p_desc is NULL.
 * @retval NRF_ERROR_BUSY          Error. Insufficient internal resources to queue the operation.
 */
ret_code_t fds_clear_by_instance(fds_instance_id_t instance);


/**@brief Function to update an existing record.
 *
 * @details Updating a record writes a new record with the given key and data in flash, and then
 *          clears the old record.
 *
 * @note This function is asynchronous, therefore, completion is reported with a callback
 *       through the registered event handler. Two callbacks will be issued, one to signal that
 *       the updated record has been written down, and another to signal that the old one has been
 *       cleared.
 *       
 * @note The record data must be aligned on a 4 byte boundary, and because it is not buffered
 *       internally, it must be kept in memory by the application until the callback for the
 *       command has been received, i.e., the command completed.
 * 
 * @param[in, out] p_desc The descriptor of the record to update. The descriptor of the updated
 *                        record, after the function has returned with NRF_SUCCESS.
 * @param[in] key         The record new key pair.
 * @param[in] num_chunks  The number of elements in the chunks array.
 * @param[in] chunks      An array of chunks making up the record new data.
 *
 * @retval NRF_SUCCESS               Success. The command was queued.
 * @retval NRF_ERROR_INVALID_STATE   Error. The module is not initialized.
 * @retval NRF_ERROR_INVALID_DATA    Error. The key contains an invalid type or instance.
 * @retval NRF_ERROR_INVALID_ADDR    Error. The record data is not aligned on a 4 byte boundary.
 * @retval NRF_ERROR_INVALID_LENGTH  Error. The record length exceeds the maximum lenght.
 * @retval NRF_ERROR_BUSY            Error. Insufficient internal resources to queue the operation.
 * @retval NRF_ERROR_NO_MEM          Error. No flash space available to store the record.
 */
ret_code_t fds_update(fds_record_desc_t  * const p_desc,
                      fds_record_key_t           key,
                      uint8_t                    num_chunks,
                      fds_record_chunk_t         chunks[]);


/**@brief Function to search for records with a given key pair.
 *
 * @details Because types are not unique, to search for the next record with the given key call
 *          the function again and supply the same fds_find_token_t structure to resume searching
 *          from the last record found.
 *
 * @param[in]  type     The record type ID.
 * @param[in]  instance The record instance ID.
 * @param[out] p_desc   The descriptor of the record found.
 * @param[out] p_token  A token containing information about the progress of the operation.
 *
 * @retval NRF_SUCCESS             Success. A record was found.
 * @retval NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval NRF_ERROR_NULL          Error. Either p_desc or p_token are NULL.
 * @retval NRF_ERROR_NOT_FOUND     Error. No record with the given key pair was found.
 */
ret_code_t fds_find(fds_type_id_t             type, 
                    fds_instance_id_t         instance, 
                    fds_record_desc_t * const p_desc,
                    fds_find_token_t  * const p_token);


/**@brief Function to search for records with a given type.
 *
 * @details Because types are not unique, to search for the next record with the given key call
 *          the function again and supply the same fds_find_token_t structure to resume searching
 *          from the last record found.
 *
 * @param[in]  type    The type ID in the record key.
 * @param[out] p_desc  The descriptor of the record found.
 * @param[out] p_token A token containing information about the progress of the operation.
 *
 * @retval NRF_SUCCESS             Success. A record was found.
 * @retval NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval NRF_ERROR_NULL          Error. Either p_desc or p_token are NULL.
 * @retval NRF_ERROR_NOT_FOUND     Error. No record with the given type was found.
 */
 ret_code_t fds_find_by_type(fds_type_id_t             type,
                             fds_record_desc_t * const p_desc,
                             fds_find_token_t  * const p_token);


/**@brief Function to search for records with a given instance.
 *
 * @details Because types are not unique, to search for the next record with the given key call
 *          the function again and supply the same fds_find_token_t structure to resume searching
 *          from the last record found.
 *
 * @param[in]  instance The instance ID in the record key.
 * @param[out] p_desc   The descriptor of the record found.
 * @param[out] p_token  A token containing information about the progress of the operation.
 *
 * @retval NRF_SUCCESS             Success. A record was found.
 * @retval NRF_ERROR_INVALID_STATE Error. The module is not initialized.
 * @retval NRF_ERROR_NULL          Error. Either p_desc or p_token are NULL.
 * @retval NRF_ERROR_NOT_FOUND     Error. No record with the given instance was found.
 */
ret_code_t fds_find_by_instance(fds_instance_id_t         instance,
                                fds_record_desc_t * const p_desc,
                                fds_find_token_t  * const p_token);


/**@brief Function to open a record for reading.
 *
 * @details Function to read a record which has been written to flash. This function initializes
 *          a fds_record_t structure which can be used to access the record data as well as
 *          its associated metadata. The pointers provided in the fds_record_t structure are
 *          pointers to flash memory. Opening a record with @ref fds_open prevents the garbage
 *          collection to run on the flash page in which record is stored, therefore the contents
 *          of the memory pointed by the fds_record_t p_data field is guaranteed to remain
 *          unmodified, as long as the record is kept open.
 *
 * @note When you are done reading a record, close it using @ref fds_close so that successive
 *       garbage collections can reclaim space on the page where the record is stored, if necessary.
 *
 * @param[in]  p_desc   The descriptor of the record to open.
 * @param[out] p_record The record data and metadata, as stored in flash.
 *
 * @retval NRF_SUCCESS            Success. The record was opened.
 * @retval NRF_ERROR_NOT_FOUND    Error. The record was not found. It may have been cleared, or it
 *                                may have not been written yet.
 * @retval NRF_ERROR_INVALID_DATA Error. The descriptor contains invalid data.
 * @retval NRF_ERROR_NULL         Error. Either p_desc or p_record are NULL.
 */
ret_code_t fds_open(fds_record_desc_t * const p_desc,
                    fds_record_t      * const p_record);


/**@brief Function to close a record, after its contents have been read.
 *
 * @details Closing a record allows garbage collection to be run on the page in which the
 *          record being closed is stored (if no other records remain open on that page).
 *
 * @note Closing a record, does NOT invalidate its descriptor, which can be safely supplied to
 *       all functions which accept a descriptor as a parameter.
 *
 * @param[in] p_desc The descriptor of the record to close.
 *
 * @retval NRF_SUCCESS            Success. The record was closed.
 * @retval NRF_ERROR_NULL         Error. p_desc is NULL.
 * @retval NRF_ERROR_INVALID_DATA Error. The descriptor contains invalid data.
 */
ret_code_t fds_close(fds_record_desc_t const * const p_desc);


/**@brief Function to perform a garbage collection.
 *
 * @details Garbage collection reclaims the flash space occupied by records which have been cleared
 *          using @ref fds_clear.
 *
 * @note    This function is asynchronous, therefore, completion is reported with a callback
 *          through the registered event handler.
 */
ret_code_t fds_gc(void);


/**@brief Function to compare two record descriptors.
 *
 * @param[in] p_desc_one First descriptor.
 * @param[in] p_desc_two Second descriptor.
 *
 * @retval true  If the descriptors identify the same record.
 * @retval false Otherwise.
 */
bool fds_descriptor_match(fds_record_desc_t const * const p_desc_one,
                          fds_record_desc_t const * const p_desc_two);


/**@brief Function to obtain a descriptor from a record ID.
 *
 * @details This function can be used to reconstruct a descriptor from a record ID, such as the
 *          one passed to the callback function.
 *
 * @warning This function does not check if a record with the given record ID exists or not. If a
 *          non-existing record ID is supplied, the resulting descriptor will cause other functions
 *          to fail when used as parameter.
 *
 * @param[out] p_desc    The descriptor of the record with given record ID.
 * @param[in]  record_id The record ID for which to provide a descriptor.
 *
 * @retval NRF_SUCCESS         Success.
 * @retval NRF_ERROR_NULL      Error. p_desc is NULL.
 */
ret_code_t fds_descriptor_from_rec_id(fds_record_desc_t * const p_desc,
                                      fds_record_id_t           record_id);

/**@brief Function to obtain a record ID from a record descriptor.
 *
 * @details This function can be used to extract a record ID from a descriptor. It may be used
 *          in the callback function to determine which record the callback is associated to, if
 *          you have its descriptor.
 *
 * @warning This function does not check the record descriptor sanity. If the descriptor is
 *          uninitialized, or has been tampered with, the resulting record ID may be invalid.
 *
 * @param[in]  p_desc      The descriptor from which to extract the record ID.
 * @param[out] p_record_id The record ID contained in the given descriptor.
 *
 * @retval NRF_SUCCESS    Success.
 * @retval NRF_ERROR_NULL Error. Either p_desc is NULL or p_record_id is NULL.
 */
ret_code_t fds_record_id_from_desc(fds_record_desc_t const * const p_desc,
                                   fds_record_id_t         * const p_record_id);

/** @} */
                                         
#endif // FDS_H__
