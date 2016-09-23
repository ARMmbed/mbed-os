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

#ifndef __FLASH_JOURNAL_PRIVATE_H__
#define __FLASH_JOURNAL_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "flash-journal/flash_journal.h"

static inline uint32_t roundUp_uint32(uint32_t N, uint32_t BOUNDARY) {
    return ((((N) + (BOUNDARY) - 1) / (BOUNDARY)) * (BOUNDARY));
}
static inline uint32_t roundDown_uint32(uint32_t N, uint32_t BOUNDARY) {
    return (((N) / (BOUNDARY)) * (BOUNDARY));
}

#define LCM_OF_ALL_ERASE_UNITS 4096 /* Assume an LCM of erase_units for now. This will be generalized later. */

static const uint32_t SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER = 0xFFFFFFFFUL;
static const uint32_t SEQUENTIAL_FLASH_JOURNAL_MAGIC                       = 0xCE02102AUL;
static const uint32_t SEQUENTIAL_FLASH_JOURNAL_VERSION                     = 1;
static const uint32_t SEQUENTIAL_FLASH_JOURNAL_HEADER_MAGIC                = 0xCEA00AEEUL;
static const uint32_t SEQUENTIAL_FLASH_JOURNAL_HEADER_VERSION              = 1;


typedef enum {
    SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED,
    SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS,
    SEQUENTIAL_JOURNAL_STATE_INITIALIZED,
    SEQUENTIAL_JOURNAL_STATE_RESETING,
    SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE,
    SEQUENTIAL_JOURNAL_STATE_LOGGING_HEAD,
    SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY,
    SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL,
    SEQUENTIAL_JOURNAL_STATE_READING,
} SequentialFlashJournalState_t;

/**
 * Meta-data placed at the head of a Journal. The actual header would be an
 * extension of this generic header, and would depend on the implementation
 * strategy. Initialization algorithms can expect to find this generic header at
 * the start of every Journal.
 */
typedef struct _SequentialFlashJournalHeader {
    FlashJournalHeader_t genericHeader; /** Generic meta-data placed at the head of a Journal; common to all journal types. */
    uint32_t             magic;         /** Sequential journal header specific magic code. */
    uint32_t             version;       /** Revision number for this sequential journal header. */
    uint32_t             numSlots;      /** Maximum number of logged blobs; i.e. maximum number of versions of the journaled payload. */
    uint32_t             sizeofSlot;    /** Slot size. Each slot holds a header, blob-payload, and a tail. */
} SequentialFlashJournalHeader_t;

/**
 * Meta-data placed at the head of a sequential-log entry.
 */
typedef struct _SequentialFlashJournalLogHead {
    uint32_t version;
    uint32_t magic;
    uint32_t sequenceNumber;
    uint32_t reserved;
} SequentialFlashJournalLogHead_t;

#define SEQUENTIAL_JOURNAL_VALID_HEAD(PTR) \
    (((PTR)->version == SEQUENTIAL_FLASH_JOURNAL_VERSION) && ((PTR)->magic == SEQUENTIAL_FLASH_JOURNAL_MAGIC))

/**
 * Meta-data placed at the tail of a sequential-log entry.
 *
 * @note the most crucial items (the ones which play a role in the validation of
 *     the log-entry) are placed at the end of this structure; this ensures that
 *     a partially written log-entry-tail won't be accepted as valid.
 */
typedef struct _SequentialFlashJournalLogTail {
    uint32_t sizeofBlob; /**< the size of the payload in this blob. */
    uint32_t magic;
    uint32_t sequenceNumber;
    uint32_t crc32;      /**< This field contains the CRC of the header, body (only including logged data),
                          *   and the tail. The 'CRC32' field is assumed to hold 0x0 for the purpose of
                          *   computing the CRC */
} SequentialFlashJournalLogTail_t;

#define SEQUENTIAL_JOURNAL_VALID_TAIL(TAIL_PTR) ((TAIL_PTR)->magic == SEQUENTIAL_FLASH_JOURNAL_MAGIC)

typedef struct _SequentialFlashJournal_t {
    FlashJournal_Ops_t             ops;                /**< the mandatory OPS table defining the strategy. */
    FlashJournal_Callback_t        callback;           /**< command completion callback. */
    FlashJournal_Info_t            info;               /**< the info structure returned from GetInfo(). */
    ARM_DRIVER_STORAGE            *mtd;                /**< The underlying Memory-Technology-Device. */
    ARM_STORAGE_CAPABILITIES       mtdCapabilities;    /**< the return from mtd->GetCapabilities(); held for quick reference. */
    uint64_t                       mtdStartOffset;     /**< the start of the address range maintained by the underlying MTD. */
    uint32_t                       firstSlotOffset;  /** Offset from the start of the journal header to the actual logged journal. */
    uint32_t                       numSlots;           /** Maximum number of logged blobs; i.e. maximum number of versions of the journaled payload. */
    uint32_t                       sizeofSlot;         /**< size of the log stride. */
    uint32_t                       nextSequenceNumber; /**< the next valid sequence number to be used when logging the next blob. */
    uint32_t                       currentBlobIndex;   /**< index of the most recently written blob. */
    SequentialFlashJournalState_t  state;              /**< state of the journal. SEQUENTIAL_JOURNAL_STATE_INITIALIZED being the default. */
    FlashJournal_OpCode_t          prevCommand;        /**< the last command issued to the journal. */

    /**
     * The following is a union of sub-structures meant to keep state relevant
     * to the commands during their execution.
     */
    union {
        /** state relevant to initialization. */
        struct {
            uint64_t currentOffset;
            struct {
                uint32_t                        headSequenceNumber;
                SequentialFlashJournalLogTail_t tail;
            };
        } initScan;

        /** state relevant to logging of data. */
        struct {
            const uint8_t *blob;           /**< the original buffer holding source data. */
            size_t         sizeofBlob;
            union {
                struct {
                    uint64_t mtdEraseOffset;
                };
                struct {
                    uint64_t       mtdOffset;       /**< the current Storage offset at which data will be written. */
                    uint64_t       mtdTailOffset;   /**< Storage offset at which the SequentialFlashJournalLogTail_t will be logged for this log-entry. */
                    const uint8_t *dataBeingLogged; /**< temporary pointer aimed at the next data to be logged. */
                    size_t         amountLeftToLog;
                    union {
                        SequentialFlashJournalLogHead_t head;
                        SequentialFlashJournalLogTail_t tail;
                    };
                };
            };
        } log;

        /** state relevant to read-back of data. */
        struct {
            const uint8_t *blob;               /**< the original buffer holding source data. */
            size_t         sizeofBlob;
            uint64_t       mtdOffset;          /**< the current Storage offset from which data is being read. */
            uint8_t       *dataBeingRead;      /**< temporary pointer aimed at the next data to be read-into. */
            size_t         amountLeftToRead;
            size_t         logicalOffset;      /**< the logical offset within the blob at which the next read will occur. */
        } read;
    };
} SequentialFlashJournal_t;

/**<
 * A static assert to ensure that the size of SequentialJournal is smaller than
 * FlashJournal_t. The caller will only allocate a FlashJournal_t and expect the
 * Sequential Strategy to reuse that space for a SequentialFlashJournal_t.
 */
typedef char AssertSequentialJournalSizeLessThanOrEqualToGenericJournal[sizeof(SequentialFlashJournal_t)<=sizeof(FlashJournal_t)?1:-1];

#define SLOT_ADDRESS(JOURNAL, INDEX) ((JOURNAL)->mtdStartOffset + (JOURNAL)->firstSlotOffset + ((INDEX) * (JOURNAL)->sizeofSlot))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __FLASH_JOURNAL_PRIVATE_H__ */
