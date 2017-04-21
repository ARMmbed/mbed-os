/*
 * Copyright (c) 2006-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FLASH_JOURNAL_H__
#define __FLASH_JOURNAL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "storage_abstraction/Driver_Storage.h"

/**
 * General return codes. All Flash-Journal APIs return an int32_t to allow for
 * both error and success status returns. This enumeration contains all
 * possible error status values.
 */
typedef enum _FlashJournal_Status
{
    JOURNAL_STATUS_OK                =   0,
    JOURNAL_STATUS_ERROR             =  -1, ///< Unspecified error
    JOURNAL_STATUS_BUSY              =  -2, ///< Underlying storage is currently unavailable
    JOURNAL_STATUS_TIMEOUT           =  -3, ///< Timeout occurred
    JOURNAL_STATUS_UNSUPPORTED       =  -4, ///< Operation not supported
    JOURNAL_STATUS_PARAMETER         =  -5, ///< Parameter error
    JOURNAL_STATUS_BOUNDED_CAPACITY  =  -6, ///< Attempt to write larger than available capacity
    JOURNAL_STATUS_STORAGE_API_ERROR =  -7, ///< Failure from some Storage API
    JOURNAL_STATUS_STORAGE_IO_ERROR  =  -8, ///< Failure from underlying storage during an IO operation.
    JOURNAL_STATUS_NOT_INITIALIZED   =  -9, ///< journal not initialized
    JOURNAL_STATUS_EMPTY             = -10, ///< There is no further data to read
    JOURNAL_STATUS_SMALL_LOG_REQUEST = -11, ///< log request is smaller than the program_unit of the underlying MTD block.
    JOURNAL_STATUS_NOT_FORMATTED     = -12, ///< need to call xxx_format() before using the journal.
    JOURNAL_STATUS_METADATA_ERROR    = -13, ///< sanity checks for the journal metadata failed.
    JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE = -14, ///< validation or run-time errors arising from the badkend media.
} FlashJournal_Status_t;

/**
 * Command opcodes for flash. Completion callbacks use these codes to refer to
 * completing commands. Refer to \ref ARM_Flash_Callback_t.
 */
typedef enum _FlashJournal_OpCode {
    FLASH_JOURNAL_OPCODE_FORMAT,
    FLASH_JOURNAL_OPCODE_INITIALIZE,
    FLASH_JOURNAL_OPCODE_GET_INFO,
    FLASH_JOURNAL_OPCODE_READ_BLOB,
    FLASH_JOURNAL_OPCODE_LOG_BLOB,
    FLASH_JOURNAL_OPCODE_COMMIT,
    FLASH_JOURNAL_OPCODE_RESET,
} FlashJournal_OpCode_t;

/**
 * @brief Flash Journal information. This contains journal-metadata, and is the
 *     return value from calling GetInfo() on the journal driver.
 */
typedef struct _FlashJournal_Info {
    uint64_t capacity;             ///< Maximum capacity (in octets) of the flash journal--i.e. the largest 'blob' which can be contained as payload.
    uint64_t sizeofJournaledBlob;  ///< size (in octets) of the most recently logged blob.
    uint32_t program_unit;         ///< Minimum programming size (in units of octets) for
                                   ///<   the current storage block--the one which will be used
                                   ///<   for the next log() operation. This value may change as we
                                   ///<   cycle through the blocks of the underlying MTD.
                                   ///<   Callers of FlashJournal_log() should refer to this field
                                   ///<   upon receiving the error JOURNAL_STATUS_SMALL_LOG_REQUEST
                                   ///<   (of when the actual amount of data logged is smaller than
                                   ///<   the requested amount).
} FlashJournal_Info_t;


static const uint32_t FLASH_JOURNAL_HEADER_MAGIC   = 0xA00AEE1DUL;
static const uint32_t FLASH_JOURNAL_HEADER_VERSION = 1;

/**
 * Meta-data placed at the head of a Journal. The actual header would be an
 * extension of this generic header, and would depend on the implementation
 * strategy. Initialization algorithms can expect to find this generic header at
 * the start of every Journal.
 */
typedef struct _FlashJournalHeader {
    uint32_t magic;         /** Journal-header specific magic code */
    uint32_t version;       /** Revision number for this generic journal header. */
    uint64_t totalSize;     /** Total space (in bytes) occupied by the journal, including the header.
                             *    Both 'mtdOffset' and 'mtdOffset + totalSize' should
                             *    lie on erase boundaries. */
    uint32_t sizeofHeader;  /** The size of the journal header; this is expected to be larger than this generic header. */
    uint32_t journalOffset; /** Offset from the start of the journal header to the actual logged journal. */
    uint32_t checksum;      /** CRC32 over the entire flash-journal-header, including the implementation
                             *    specific extension (i.e. over 'sizeofHeader' bytes). The value of the
                             *    field is taken to be 0 for the purpose of computing the checksum. */
} FlashJournalHeader_t;

/**
 * This is the type of the command completion callback handler for the
 * asynchronous flash-journal APIs: initialize(), read(), log(), commit() and
 * reset() (which is nearly all APIs).
 *
 * @param status
 *          A code to indicate the status of the completed operation. For data
 *          transfer operations, the status field is overloaded in case of
 *          success to return the amount of data successfully transferred; this
 *          can be done safely because error codes are negative values.
 *
 * @param cmd_code
 *          The command op-code of type FlashJournal_OpCode_t. This value isn't
 *          essential for the callback, but it is expected that this information
 *          could be a quick and useful filter.
 */
typedef void (*FlashJournal_Callback_t)(int32_t status, FlashJournal_OpCode_t cmd_code);

/* forward declarations. */
struct FlashJournal_t;

/**
 * @ref FlashJournal_t is an abstraction implemented by a table of generic
 * operations (i.e. strategy) together with an opaque, strategy-specific
 * data. Taken together, the FlashJournal_t is an opaque handle containing
 * such top-level metadata.
 *
 * Algorithms depending on the FlashJournal can be generic (i.e. independent of
 * the strategy) in their use of the Flash-Journal APIs. For the sake of being
 * able to allocate a FlashJournal_t for use in such generic algorithms, the
 * FlashJournal_t contains a MAX_SIZE to accommodate the largest of the
 * strategy-specific metadata. The value of this MAX_SIZE may need to be
 * increased if some future journal-strategy needs more metadata.
 */
#define FLASH_JOURNAL_HANDLE_MAX_SIZE 160

/**
 * This is the set of operations offered by the flash-journal abstraction. A set
 * of implementations for these operations defines a logging strategy.
 */

typedef struct FlashJournal_Ops_t {
    /**
     * \brief Initialize the flash journal. Refer to @ref FlashJournal_initialize.
     */
    int32_t               (*initialize)(struct FlashJournal_t           *journal,
                                        ARM_DRIVER_STORAGE       *mtd,
                                        const struct FlashJournal_Ops_t *ops,
                                        FlashJournal_Callback_t   callback);

    /**
     * \brief fetch journal metadata. Refer to @ref FlashJournal_getInfo.
     */
    FlashJournal_Status_t (*getInfo)   (struct FlashJournal_t *journal, FlashJournal_Info_t *info);

    /**
     * @brief Read from the most recently logged blob. Refer to @ref FlashJournal_read.
     */
    int32_t               (*read)      (struct FlashJournal_t *journal, void *buffer, size_t size);

    /**
     * @brief Read from the most recently logged blob from a particular offset. Refer to @ref FlashJournal_readFrom.
     */
    int32_t               (*readFrom)  (struct FlashJournal_t *journal, size_t offset, void *buffer, size_t size);

    /**
     * @brief Start logging a new blob or append to the one currently being logged. Refer to @ref FlashJournal_log.
     */
    int32_t               (*log)       (struct FlashJournal_t *journal, const void *blob, size_t size);

    /**
     * @brief commit a blob accumulated through a non-empty sequence of
     *     previously successful log() operations. Refer to @ref FlashJournal_commit.
     */
    int32_t               (*commit)    (struct FlashJournal_t *journal);

    /**
     * @brief Reset the journal. This has the effect of erasing all valid blobs.
     *     Refer to @ref FlashJournal_reset.
     */
    int32_t               (*reset)     (struct FlashJournal_t *journal);
} FlashJournal_Ops_t;

/**
 * @brief An opaque handle constituting the Flash Journal.
 *
 * @details This structure is intentionally opaque to avoid exposing data
 *     internal to an implementation strategy; this prevents accesses through any
 *     means other than through the defined API.
 *
 * Having a known size for the handle allows the caller to remain malloc-free.
 *
 * @note: There should be static asserts in the code to verify our assumption
 * that the real FlashJournal handle fits within FLASH_JOURNAL_HANDLE_MAX_SIZE
 * bytes.
 *
 * @note: there is a risk of overallocation in case an implementation doesn't
 * need FLASH_JOURNAL_HANDLE_MAX_SIZE bytes, but the impact should be small.
 */
typedef struct FlashJournal_t {
    FlashJournal_Ops_t       ops;

    union {
        ARM_DRIVER_STORAGE  *mtd;
        FlashJournal_Info_t  info;
        void                *pointer;
        uint8_t              octet;
        uint32_t             data[FLASH_JOURNAL_HANDLE_MAX_SIZE / sizeof(uint32_t)];
    } opaque;
} FlashJournal_t;

/**
 * @brief Initialize a flash journal.
 *
 * This is a front-end for @ref FlashJournal_Ops_t::initialize() of the
 * underlying strategy.
 *
 * This function must be called *before* the middle-ware component starts
 * using a journal. As a part of bringing the journal to a ready state, it
 * also discovers the most recently logged blob.
 *
 * Initialize() receives a callback handler to be invoked upon completion of
 * asynchronous operations.
 *
 * @param [out] journal
 *                A caller-supplied buffer large enough to hold an
 *                initialized journal. The internals of the actual journal
 *                are opaque to the caller and depend on the logging
 *                strategy (as defined by the parameter 'ops'). This memory
 *                should be at least as large as 'FLASH_JOURNAL_HANDLE_MAX_SIZE'.
 *                Upon successful return, the journal is setup in an
 *                initialized state.
 *
 * @param  [in] mtd
 *                The underlying Storage_Driver targeted by the journal. MTD
 *                stands for Memory-Technology-Device.
 *
 * @param  [in] ops
 *                This is the set of operations which define the logging strategy.
 *
 * @param  [in] callback
 *                Caller-defined callback to be invoked upon command completion of
 *                initialization; and also for all future invocations of
 *                asynchronous APIs. Use a NULL pointer when no
 *                callback signals are required.
 *
 * @note: this is an asynchronous operation, but it can finish
 * synchronously if the underlying MTD supports that.
 *
 * @return
 *   The function executes in the following ways:
 *   - When the operation is asynchronous, the function only starts the
 *     initialization and control returns to the caller with an
 *     JOURNAL_STATUS_OK before the actual completion of the operation (or
 *     with an appropriate error code in case of failure). When the
 *     operation is completed the command callback is invoked with
 *     1 passed in as the 'status' parameter of the
 *     callback. In case of errors, the completion callback is invoked with
 *     an error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the actual
 *     completion of the operation or the discovery of a failure condition. In
 *     this case, the function returns 1 to signal successful synchronous
 *     completion or an appropriate error code, and no further
 *     invocation of the completion callback should be expected at a later time.
 *
 * @note The user must call an appropriate xxx_format() to format underlying
 *     storage before initializing it for use. If Initialize() is called on
 *     unformatted storage, an error value of JOURNAL_STATUS_NOT_FORMATTED will be
 *     returned.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = FlashJournal_initialize(&journal, MTD, &STRATEGY_SEQUENTIAL, callbackHandler);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         ASSERT(returnValue == 1);
 *         // handle synchronous completion
 *     }
 * \endcode
 */
static inline int32_t FlashJournal_initialize(FlashJournal_t           *journal,
                                              ARM_DRIVER_STORAGE       *mtd,
                                              const FlashJournal_Ops_t *ops,
                                              FlashJournal_Callback_t   callback)
{
    return ops->initialize(journal, mtd, ops, callback);
}

/**
 * @brief Fetch journal metadata. A front-end for @ref FlashJournal_Ops_t::getInfo().
 *
 * @param [in] journal
 *                A previously initialized journal.
 *
 * @param [out] info
 *                A caller-supplied buffer capable of being filled in with an
 *                FlashJournal_Info_t.
 *
 * @return JOURNAL_STATUS_OK if a FlashJournal_Info_t structure containing
 *         top level metadata about the journal is filled into the supplied
 *         buffer, else an appropriate error value.
 *
 * @note It is the caller's responsibility to ensure that the buffer passed in
 *         is able to be initialized with a FlashJournal_Info_t.
 *
 * @note getInfo()s can still be called during a sequence of
 *     log()s.
 *
 * @note This API returns synchronously--it does not result in an invocation
 *     of a completion callback.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     FlashJournal_Info_t info;
 *     int32_t returnValue = FlashJournal_getInfo(&journal, &info);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else {
 *         ASSERT(returnValue == JOURNAL_STATUS_OK);
 *         // work with the 'info'.
 *     }
 * \endcode
 */
static inline FlashJournal_Status_t FlashJournal_getInfo(FlashJournal_t *journal, FlashJournal_Info_t *info)
{
    return journal->ops.getInfo(journal, info);
}

/**
 * @brief Read from the most recently logged blob. A front-end for @ref
 * FlashJournal_Ops_t::read().
 *
 * @details Read off a chunk of the logged blob sequentially. The blob may
 *     be larger than the size of the read (or even of available SRAM), so
 *     multiple calls to read() could be necessary before the entire blob is
 *     read off. The journal maintains a read-pointer internally to allow
 *     reads to continue where the previous one left off.
 *
 * @note: Once the entire blob is read, the final read() returns the error
 *     JOURNAL_STATUS_EMPTY (or passes that value as the status of a
 *     completion callback) and resets the read-pointer to allow re-reading
 *     the blob from the start.
 *
 * @param  [in] journal
 *                A previously initialized journal.
 *
 * @param [out] buffer
 *                The destination of the read operation. The memory is owned
 *                by the caller and should remain valid for the lifetime
 *                of this operation.
 *
 * @param  [in] size
 *                The maximum amount of data which can be read in this
 *                operation. The memory pointed to by 'buffer' should be as
 *                large as this amount.
 *
 * @return
 *    The function executes in the following ways:
 *   - When the operation is asynchronous--i.e. when the underlying MTD's
 *     ARM_STOR_CAPABILITIES::asynchronous_ops is set to 1--and the operation
 *     executed by the journal in a non-blocking (i.e. asynchronous) manner,
 *     control returns to the caller with JOURNAL_STATUS_OK before the actual
 *     completion of the operation (or with an appropriate error code in case of
 *     failure). When the operation completes, the command callback is
 *     invoked with the number of successfully transferred bytes passed in as
 *     the 'status' parameter of the callback. If any error is encountered
 *     after the launch of an asynchronous operation, the completion callback
 *     is invoked with an error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the
 *     actual completion of the operation, or the discovery of a failure
 *     condition. In synchronous mode, the function returns the number
 *     of data items read or an appropriate error code.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set then this operation may execute asynchronously. In the case of
 *     asynchronous operation, the invocation returns early (with
 *     JOURNAL_STATUS_OK) and results in a completion callback later.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set, the journal is not required to operate asynchronously. A Read
 *     operation can be finished synchronously in spite of
 *     ARM_STORAGE_CAPABILITIES::asynchronous_ops being set, returning the
 *     number of data items read to indicate successful completion, or an
 *     appropriate error code. In this case no further invocation of a
 *     completion callback should be expected at a later time.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = FlashJournal_read(&journal, buffer, size);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         ASSERT(returnValue == size);
 *         // handle synchronous completion
 *     }
 * \endcode
 */
static inline int32_t FlashJournal_read(FlashJournal_t *journal, void *blob, size_t n)
{
    return journal->ops.read(journal, blob, n);
}

/**
 * @brief Read from the most recently logged blob at a given offset. A front-end
 *     for @ref FlashJournal_Ops_t::readFrom().
 *
 * @details Read off a chunk of the logged blob from a given offset. The journal
 *     maintains a read-pointer internally to allow reads to continue where the
 *     previous one left off. This call effectively sets the read-counter before
 *     fetching data. Subsequent reads continue sequentially from where the
 *     readFrom() left off.
 *
 * @note: If the given offset stands at (or is beyond) the end of the previously
 *     logged blob, readFrom() returns the error JOURNAL_STATUS_EMPTY (or passes
 *     that value as the status of a completion callback) and resets the read-
 *     pointer to allow re-reading the blob from the start.
 *
 * @param  [in] journal
 *                A previously initialized journal.
 *
 * @param  [in] offset
 *                The logical offset (within the blob) at which to read data from.
 *
 * @param [out] buffer
 *                The destination of the read operation. The memory is owned
 *                by the caller and should remain valid for the lifetime
 *                of this operation.
 *
 * @param  [in] size
 *                The maximum amount of data which can be read in this
 *                operation. The memory pointed to by 'buffer' should be as
 *                large as this amount.
 *
 * @return
 *    The function executes in the following ways:
 *   - When the operation is asynchronous--i.e. when the underlying MTD's
 *     ARM_STOR_CAPABILITIES::asynchronous_ops is set to 1--and the operation
 *     executed by the journal in a non-blocking (i.e. asynchronous) manner,
 *     control returns to the caller with JOURNAL_STATUS_OK before the actual
 *     completion of the operation (or with an appropriate error code in case of
 *     failure). When the operation completes, the command callback is
 *     invoked with the number of successfully transferred bytes passed in as
 *     the 'status' parameter of the callback. If any error is encountered
 *     after the launch of an asynchronous operation, the completion callback
 *     is invoked with an error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the
 *     actual completion of the operation, or the discovery of a failure
 *     condition. In synchronous mode, the function returns the number
 *     of data items read or an appropriate error code.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set then this operation may execute asynchronously. In the case of
 *     asynchronous operation, the invocation returns early (with
 *     JOURNAL_STATUS_OK) and results in a completion callback later.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set, the journal is not required to operate asynchronously. A Read
 *     operation can be finished synchronously in spite of
 *     ARM_STORAGE_CAPABILITIES::asynchronous_ops being set, returning the
 *     number of data items read to indicate successful completion, or an
 *     appropriate error code. In this case no further invocation of a
 *     completion callback should be expected at a later time.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = FlashJournal_readFrom(&journal, offset, buffer, size);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         ASSERT(returnValue == size);
 *         // handle synchronous completion
 *     }
 * \endcode
 */
static inline int32_t FlashJournal_readFrom(struct FlashJournal_t *journal, size_t offset, void *blob, size_t n)
{
    return journal->ops.readFrom(journal, offset, blob, n);
}

/**
 * @brief Start logging a new blob or append to the one currently being logged.
 * A front-end for @ref FlashJournal_Ops_t::log().
 *
 * @details Extend (or start off) the currently logged blob sequentially.
 *     There could be several calls to log() before the entire blob is
 *     accumulated. A sequence of one or more log() must be terminated by a
 *     commit() before the state of the blob is sealed and made persistent.
 *     The journal maintains a log-pointer internally to allow
 *     log()s to continue where the previous one left off.
 *
 * @param [in] journal
 *               A previously initialized journal.
 *
 * @param [in] blob
 *               The source of the log operation. The memory is owned
 *               by the caller and should remain valid for the lifetime
 *               of this operation.
 *
 * @param [in] size
 *               The amount of data being logged in this operation. The
 *               buffer pointed to by 'blob' should be as large as this
 *               amount.
 *
 * @return [please be sure to read notes (below) regarding other return values]
 *   The function executes in the following ways:
 *   - When the operation is asynchronous--i.e. when the underlying MTD's
 *     ARM_STOR_CAPABILITIES::asynchronous_ops is set to 1--and the operation
 *     executed by the journal in a non-blocking (i.e. asynchronous) manner,
 *     control returns to the caller with JOURNAL_STATUS_OK before the actual
 *     completion of the operation (or with an appropriate error code in case of
 *     failure). When the operation completes, the command callback is
 *     invoked with the number of successfully transferred bytes passed in as
 *     the 'status' parameter of the callback. If any error is encountered
 *     after the launch of an asynchronous operation, the completion callback
 *     is invoked with an error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the actual
 *     completion of the operation, or the discovery of a failure condition. In
 *     synchronous mode, the function returns the number of data items
 *     logged, or an appropriate error code.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set then this operation may execute asynchronously. In the case of
 *     asynchronous operation, the invocation returns early (with
 *     JOURNAL_STATUS_OK) and results in a completion callback later.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set, the journal is not required to operate asynchronously. A log
 *     operation can be finished synchronously in spite of
 *     ARM_STORAGE_CAPABILITIES::asynchronous_ops being set, returning the
 *     number of data items logged to indicate successful completion, or an
 *     appropriate error code. In this case no further invocation of a
 *     completion callback should be expected at a later time.
 *
 * @note If a log operation will exceed available capacity, it fails with the
 *     error JOURNAL_STATUS_BOUNDED_CAPACITY.
 *
 * @note The actual size of data transfer (as reported by the status
 *     parameter of the callback or the return value from log() in case of
 *     synchronous operation) may be smaller than the amount requested. This
 *     could be due to the 'program_unit' of the underlying storage block--
 *     i.e. the minimum programmable size. Refer to @ref
 *     FlashJournal_Info_t::program_unit. It is the caller's responsibility
 *     for resubmitting this left-over data in a subsequent call to log.
 *     When logging an arbitrary amount of data, the last of a sequence of
 *     logs may need to be padded in order to align with the
 *     programming unit.
 *
 * @note If the total size requested to be logged is smaller
 *     than the MTD's program_unit, log() fails with an error value of
 *     JOURNAL_STATUS_SMALL_LOG_REQUEST.
 *
 * @note the data being logged isn't made persistent (or available for read-
 *     backs) until a commit. A sequence of log() operations is expected to end
 *     in a commit(). A new sequence of log()s should be initiated by the caller
 *     only after a commit() has completed. If a sequence of logs() is followed
 *     by an operation other than a commit, that operation will very likely
 *     return an error code. getInfo()s can still be called during a sequence of
 *     log()s.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = FlashJournal_log(&journal, buffer, size);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         ASSERT(returnValue <= size);
 *         // handle synchronous completion
 *
 *         if (returnValue < size) {
 *     #if DEBUG
 *             FlashJournal_Info_t info;
 *             int32_t rc = FlashJournal_getInfo(&journal, &info);
 *             ASSERT(rc == JOURNAL_STATUS_OK);
 *             ASSERT(returnValue == (size - (size % info.program_unit)));
 *     #endif
 *             // move the last (size - returnValue) bytes of the buffer to the
 *             // beginning of the buffer to be used for the successive request.
 *         }
 *     }
 * \endcode
 */
static inline int32_t FlashJournal_log(FlashJournal_t *journal, const void *blob, size_t n)
{
    return journal->ops.log(journal, blob, n);
}

/**
 * @brief Commit a blob accumulated through a (possibly empty) sequence of previously
 *     successful log() operations. A front-end for @ref FlashJournal_Ops_t::commit().
 *
 * @param  [in] journal
 *                A previously initialized journal.
 *
 * @return
 *   The function executes in the following ways:
 *   - When the operation is asynchronous--i.e. when the underlying MTD's
 *     ARM_STOR_CAPABILITIES::asynchronous_ops is set to 1--and the operation
 *     executed by the journal in a non-blocking (i.e. asynchronous) manner,
 *     control returns to the caller with JOURNAL_STATUS_OK before the actual
 *     completion of the operation (or with an appropriate error code in case of
 *     failure). When the operation completes, the command callback is invoked
 *     with 1 passed in as the 'status' parameter of the callback to indicate
 *     success. If any error is encountered after the launch of an asynchronous
 *     operation, the completion callback is invoked with an error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the actual
 *     completion of the operation, or the discovery of a failure condition. In
 *     synchronous mode, the function returns 1 to indicate success, or an
 *     appropriate error code.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set then this operation may execute asynchronously. In the case of
 *     asynchronous operation, the invocation returns early (with
 *     JOURNAL_STATUS_OK) and results in a completion callback later.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set, the journal is not required to operate asynchronously. A
 *     commit operation can be finished synchronously in spite of
 *     ARM_STORAGE_CAPABILITIES::asynchronous_ops being set, returning the
 *     total size of the committed blob to indicate successful completion,
 *     or an appropriate error code. In this case no further invocation of a
 *     completion callback should be expected at a later time.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = FlashJournal_commit(&journal);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         // handle synchronous completion
 *         ASSERT(returnValue == 1);
 *         ...
 *     }
 * \endcode
 *
 * @note A sequence of log() operations is expected to end in a commit(). A new
 *     sequence of log()s should be initiated by the caller only after a
 *     commit() has completed. If a sequence of logs() is followed
 *     by an operation other than a commit, that operation will very likely
 *     return an error code.
 */
static inline int32_t FlashJournal_commit(FlashJournal_t *journal)
{
    return journal->ops.commit(journal);
}

/**
 * @brief Reset the journal. This has the effect of erasing all valid blobs. A
 *     front-end for @ref FlashJournal_Ops_t::reset().
 *
 * @param [in] journal
 *               A previously initialized journal.
 *
 * @return
 *   The function executes in the following ways:
 *   - When the operation is asynchronous--i.e. when the underlying MTD's
 *     ARM_STOR_CAPABILITIES::asynchronous_ops is set to 1--and the
 *     operation executed by the journal in a non-blocking (i.e.
 *     asynchronous) manner, control returns to the caller with
 *     JOURNAL_STATUS_OK before the actual completion of the operation (or
 *     with an appropriate error code in case of failure). When the
 *     operation completes, the command callback is invoked with
 *     JOURNAL_STATUS_OK passed in as the 'status' parameter of the
 *     callback. If any error is encountered after the launch of an
 *     asynchronous operation, the completion callback is invoked with an
 *     error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the
 *     actual completion of the operation, or the discovery of a failure
 *     condition. In synchronous mode, the function returns 1 to signal
 *     successful completion, or an appropriate error code.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set then this operation may execute asynchronously. In the case of
 *     asynchronous operation, the invocation returns early (with
 *     JOURNAL_STATUS_OK) and results in a completion callback later.
 *
 * @note If the underlying MTD's ARM_STORAGE_CAPABILITIES::asynchronous_ops
 *     is set, the journal is not required to operate asynchronously. A
 *     reset operation can be finished synchronously in spite of
 *     ARM_STORAGE_CAPABILITIES::asynchronous_ops being set, returning 1 to
 *     indicate successful completion, or an appropriate error code. In this
 *     case no further invocation of a completion callback should be
 *     expected at a later time.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = FlashJournal_reset(&journal);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         ASSERT(returnValue == 1);
 *         // handle synchronous completion
 *     }
 * \endcode
 */
static inline int32_t FlashJournal_reset(FlashJournal_t *journal)
{
    return journal->ops.reset(journal);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __FLASH_JOURNAL_H__ */
