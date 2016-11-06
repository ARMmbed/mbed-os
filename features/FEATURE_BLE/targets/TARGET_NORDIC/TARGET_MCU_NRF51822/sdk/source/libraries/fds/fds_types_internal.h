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

#ifndef FDS_TYPES_INTERNAL__
#define FDS_TYPES_INTERNAL__

#include "fds.h"
#include <stdint.h>
#include <stdbool.h>
#include "nrf_soc.h"


#define COMMAND_EXECUTING           (NRF_SUCCESS)
#define COMMAND_COMPLETED           (0x1234)
//#define COMMAND_FAILED            (0x1236)

#define FDS_MAGIC_HWORD             (0xF11E)
#define FDS_MAGIC_WORD              (0x15ABE11A)
#define FDS_ERASED_WORD             (0xFFFFFFFF)

#define FDS_PAGE_TAG_SIZE           (4) /**< Page tag size, in 4 byte words. */

#define FDS_VPAGE_ID_UNKNOWN        (0xFFFF)

#define FDS_WRITE_OFFSET_TL         (0) /**< Offset of TL from the record base address, in 4 byte words. */
#define FDS_WRITE_OFFSET_IC         (1) /**< Offset of IC from the record base address, in 4 byte words. */
#define FDS_WRITE_OFFSET_ID         (2) /**< Offset of ID from the record base address, in 4 byte words. */
#define FDS_WRITE_OFFSET_DATA       (3) /**< Offset of the data (chunks) from the record base address, in 4 byte words. */

#define FDS_HEADER_SIZE_TL          (1) /**< Size of the TL part of the header, in 4 byte words. */
#define FDS_HEADER_SIZE_ID          (1) /**< Size of the IC part of the header, in 4 byte words. */
#define FDS_HEADER_SIZE_IC          (1) /**< Size of the IC part of the header, in 4 byte words. */
#define FDS_HEADER_SIZE             (3) /**< Size of the whole header, in 4 byte words. */

#define FDS_CMD_QUEUE_SIZE_INIT     (1)
#define FDS_CMD_QUEUE_SIZE_WRITE    (1)
#define FDS_CMD_QUEUE_SIZE_CLEAR    (1)
#define FDS_CMD_QUEUE_SIZE_UPDATE   (2)
#define FDS_CMD_QUEUE_SIZE_GC       (1)


//static uint8_t m_nested_critical;

/** Macros to enable and disable application interrupts. */
#define CRITICAL_SECTION_ENTER()    //sd_nvic_critical_region_enter(&m_nested_critical)
#define CRITICAL_SECTION_EXIT()     //sd_nvic_critical_region_exit ( m_nested_critical)

/**@brief Page types. */
typedef enum
{
    FDS_PAGE_UNDEFINED, /**< Undefined page type. */
    FDS_PAGE_ERASED,    /**< Page is erased. */
    FDS_PAGE_VALID,     /**< Page is ready for storage. */
    FDS_PAGE_SWAP,      /**< Page is reserved for GC. */
    FDS_PAGE_GC         /**< Page is being garbage collected. */
} fds_page_type_t;


typedef enum
{
    FDS_OP_NONE         = 0x00, /**< No operation. */
    FDS_OP_WRITE_TL,            /**< Write the type and length. */
    FDS_OP_WRITE_ID,            /**< Write the record ID. */
    FDS_OP_WRITE_CHUNK,         /**< Write the record value.  */
    FDS_OP_WRITE_IC,            /**< Write the instance and checksum. */
    FDS_OP_CLEAR_TL,
    FDS_OP_CLEAR_INSTANCE,
    FDS_OP_DONE,
} fds_opcode_t;


typedef enum
{
    FDS_FLAG_INITIALIZING       = (1 << 0),  /**< TODO: Not really needed atm? */
    FDS_FLAG_INITIALIZED        = (1 << 1),  /**< Flag indicating that flash data storage has been initialized. */
    FDS_FLAG_PROCESSING         = (1 << 2),  /**< Flag indicating that queue is being processed. */
    FDS_FLAG_CAN_GC             = (1 << 3),  /**< Flag indicating that fds can regain data by performing garbage collection. */
} fds_flags_t;


typedef struct
{
    uint32_t const    * start_addr;
    uint16_t            vpage_id;             /**< The page logical ID. */
    uint16_t volatile   write_offset;         /**< The page write offset, in 4 bytes words. */
    uint16_t volatile   words_reserved;       /**< The amount of words reserved by fds_write_reserve() on this page. */
    uint16_t volatile   records_open;
    fds_page_type_t     page_type        : 4; /**< The page type. */
} fds_page_t;


typedef struct
{
    fds_cmd_id_t        id            : 4;    /**< The ID of the command. */
    fds_opcode_t        op_code       : 4;
    uint8_t             num_chunks;           /**< Number of operations this command has left in the operation queue. */
    uint16_t            chunk_offset;         /**< Offset used for writing the record value(s), in 4 byte words. */
    uint16_t            vpage_id;             /**< The virtual page ID where we reserved the flash space for this command. */
    fds_record_header_t record_header;
} fds_cmd_t;


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
    fds_cmd_t          cmd[FDS_CMD_QUEUE_SIZE];        /**< Array to maintain flash access operation details. */
    uint8_t   volatile rp;                             /**< The index of the command being executed. */
    uint8_t   volatile count;                          /**< Number of elements in the queue. */
} fds_cmd_queue_t;


typedef struct
{
    fds_record_chunk_t          chunk[FDS_CHUNK_QUEUE_SIZE];
    uint8_t            volatile rp;
    uint8_t            volatile count;
} fds_chunk_queue_t;


typedef enum
{
    NONE,
    BEGIN,
    RESUME,
    GC_PAGE,
    COPY_RECORD,
    READY_SWAP,
    NEW_SWAP,
    INIT_SWAP
} fds_gc_state_t;


typedef struct
{
    uint16_t         cur_page;
    uint16_t         swap_page;
    uint32_t const * p_scan_addr;
    fds_gc_state_t   state;
    bool             do_gc_page[FDS_MAX_PAGES];
} fds_gc_data_t;

#endif // FDS_TYPES_INTERNAL__
