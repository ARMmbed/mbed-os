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

#include "fds.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "fds_config.h"
#include "fds_types_internal.h"
#include "fstorage.h"
#include "nrf_error.h"
#include "app_util.h"

static void fs_callback(uint8_t             op_code,
                        uint32_t            result,
                        uint32_t    const * p_data,
                        fs_length_t         length_words);

/** Our fstorage configuration.
 *  The other fields will be assigned automatically during compilation. */
FS_SECTION_VARS_ADD(fs_config_t fs_config) = { .cb = fs_callback, .num_pages = FDS_MAX_PAGES };

static uint32_t const fds_page_tag_swap[]   = {FDS_PAGE_TAG_WORD_0_SWAP, FDS_PAGE_TAG_WORD_1,
                                               FDS_PAGE_TAG_WORD_2,      FDS_PAGE_TAG_WORD_3};

static uint32_t const fds_page_tag_valid[]  = {FDS_PAGE_TAG_WORD_0_VALID, FDS_PAGE_TAG_WORD_1,
                                               FDS_PAGE_TAG_WORD_2,       FDS_PAGE_TAG_WORD_3};

static uint32_t const fds_page_tag_gc       = FDS_PAGE_TAG_WORD_3_GC;

static fds_tl_t const m_fds_tl_invalid      = { .type = FDS_TYPE_ID_INVALID,
                                                .length_words = 0xFFFF };

/**@brief Internal status flags. */
static uint8_t           volatile m_flags;       

static uint8_t                    m_users;
static fds_cb_t                   m_cb_table[FDS_MAX_USERS];

/**@brief The last record ID. Setup page by page_scan() during pages_init(). */
static fds_record_id_t            m_last_rec_id;

/**@brief The internal queues. */
static fds_cmd_queue_t            m_cmd_queue;   
static fds_chunk_queue_t          m_chunk_queue;

/**@brief Holds the state of pages. Setup by fds_init(). */
static fds_page_t                 m_pages[FDS_MAX_PAGES];
static bool                       m_swap_page_avail = false;

static fds_gc_data_t              m_gc;
static uint16_t                   m_gc_runs;

static uint8_t          volatile  m_counter;


static void app_notify(ret_code_t       result,
                       fds_cmd_id_t     cmd,
                       fds_record_id_t  record_id,
                       fds_record_key_t record_key)
{
    for (uint8_t user = 0; user < FDS_MAX_USERS; user++)
    {
        if (m_cb_table[user] != NULL)
        {
            m_cb_table[user](result, cmd, record_id, record_key);
        }
    }
}


static void atomic_counter_inc()
{
    CRITICAL_SECTION_ENTER();
    m_counter++;
    CRITICAL_SECTION_EXIT();
}


static void atomic_counter_dec()
{
    CRITICAL_SECTION_ENTER();
    m_counter--;
    CRITICAL_SECTION_EXIT();
}


static bool atomic_counter_is_zero()
{
    bool ret;
    CRITICAL_SECTION_ENTER();
    ret = (m_counter == 0);
    CRITICAL_SECTION_EXIT();
    return ret;
}


static void flag_set(fds_flags_t flag)
{
    CRITICAL_SECTION_ENTER();
    m_flags |= flag;
    CRITICAL_SECTION_EXIT();
}


static void flag_clear(fds_flags_t flag)
{
    CRITICAL_SECTION_ENTER();
    m_flags &= ~(flag);
    CRITICAL_SECTION_EXIT();
}


static bool flag_is_set(fds_flags_t flag)
{
    bool ret;
    CRITICAL_SECTION_ENTER();
    ret = (m_flags & flag);
    CRITICAL_SECTION_EXIT();
    return ret;
}


/**@brief Function to check if a header has valid information. */
static __INLINE bool header_is_valid(fds_header_t const * const p_header)
{
    return ((p_header->tl.type     != FDS_TYPE_ID_INVALID) &&
            (p_header->ic.instance != FDS_INSTANCE_ID_INVALID));
}


static bool address_within_page_bounds(uint32_t const * const p_addr)
{
    return (p_addr >= fs_config.p_start_addr) &&
           (p_addr <= fs_config.p_end_addr) &&
           (is_word_aligned(p_addr));
}


/**@brief Internal function to identify the page type. */
static fds_page_type_t page_identify(uint16_t page_number)
{
    uint32_t const * const p_page_addr = m_pages[page_number].start_addr;

    uint32_t const word0 = *(p_page_addr);
    uint32_t const word1 = *(p_page_addr + 1);
    uint32_t const word2 = *(p_page_addr + 2);
    uint32_t const word3 = *(p_page_addr + 3);

    if (word1 != FDS_PAGE_TAG_WORD_1)
    {
        return FDS_PAGE_UNDEFINED;
    }

    if (word2 != FDS_PAGE_TAG_WORD_2)
    {
        return FDS_PAGE_UNDEFINED;
    }

    if (word3 == FDS_PAGE_TAG_WORD_3)
    {
        if (word0 == FDS_PAGE_TAG_WORD_0_SWAP)
        {
            return FDS_PAGE_SWAP;
        }

        if (word0 == FDS_PAGE_TAG_WORD_0_VALID)
        {
            return FDS_PAGE_VALID;
        }
    }
    else if (word3 == FDS_PAGE_TAG_WORD_3_GC)
    {
        if (word0 == FDS_PAGE_TAG_WORD_0_SWAP || word0 == FDS_PAGE_TAG_WORD_0_VALID)
        {
            return FDS_PAGE_GC;
        }
    }

    return FDS_PAGE_UNDEFINED;
}


static uint16_t page_by_addr(uint32_t const * const p_addr)
{
    if (p_addr == NULL)
    {
        return 0;
    }

    // Compute the BYTES offset from the beginning of the first page.
    uint32_t const byte_offset = (uint32_t)p_addr - (uint32_t)m_pages[0].start_addr;

// See nrf.h.
#if defined (NRF51)
    return byte_offset >> 10; // Divide by page size (1024).
#elif defined (NRF52)
    return byte_offset >> 12; // Divide by page size (4096).
#else
    #error "Device family must be defined. See nrf.h."
#endif
}


// NOTE: depends on m_pages.write_offset to function.
static bool page_has_space(uint16_t page, fds_length_t length_words)
{
    if (page >= FDS_MAX_PAGES)
    {
        return false;
    }

    CRITICAL_SECTION_ENTER();
    length_words +=  m_pages[page].write_offset;
    length_words +=  m_pages[page].words_reserved;
    CRITICAL_SECTION_EXIT();

    return (length_words < FS_PAGE_SIZE_WORDS);
}


/**@brief This function scans a page to determine how many words have
 *        been written to it. This information is used to set the page
 *        write offset during initialization (mount). Additionally, this
 *        function will update the last known record ID as it proceeds.
 */
static void page_scan(uint16_t page, uint16_t volatile * words_written)
{
    uint32_t const * p_addr = (m_pages[page].start_addr + FDS_PAGE_TAG_SIZE);

    *words_written = FDS_PAGE_TAG_SIZE;

    // A corrupt TL might cause problems.
    while ((p_addr < m_pages[page].start_addr + FS_PAGE_SIZE_WORDS) &&
           (*p_addr != FDS_ERASED_WORD))
    {
        fds_header_t const * const p_header = (fds_header_t*)p_addr;

        /** Note: DO NOT check for the validity of the header using
         *  header_is_valid() here. If an header has an invalid type (0x0000)
         *  or a missing instance (0xFFFF) then we WANT to skip it.
         */

         // Update the last known record id.
         if (p_header->id > m_last_rec_id)
         {
            m_last_rec_id = p_header->id;
         }

         // Jump to the next record.
         p_addr         += (FDS_HEADER_SIZE + p_header->tl.length_words);
         *words_written += (FDS_HEADER_SIZE + p_header->tl.length_words);
    }
}


static bool page_is_empty(uint16_t page)
{
    uint32_t const * const p_addr = m_pages[page].start_addr;

    for (uint16_t i = 0; i < FS_PAGE_SIZE_WORDS; i++)
    {
        if (*(p_addr + i) != FDS_ERASED_WORD)
        {
            return false;
        }
    }

    return true;
}


static ret_code_t page_id_from_virtual_id(uint16_t vpage_id, uint16_t * p_page_id)
{
    for (uint16_t i = 0; i < FDS_MAX_PAGES; i++)
    {
        if (m_pages[i].vpage_id == vpage_id)
        {
            *p_page_id = i;
            return NRF_SUCCESS;
        }
    }

    return NRF_ERROR_NOT_FOUND;
}


static ret_code_t page_from_virtual_id(uint16_t vpage_id, fds_page_t ** p_page)
{
    for (uint16_t i = 0; i < FDS_MAX_PAGES; i++)
    {
        if (m_pages[i].vpage_id == vpage_id)
        {
            *p_page = &m_pages[i];
            return NRF_SUCCESS;
        }
    }

    return NRF_ERROR_NOT_FOUND;
}


static uint32_t record_id_new()
{
    return ++m_last_rec_id;
}


/**@brief Tags a page as swap, i.e., reserved for GC. */
static ret_code_t page_tag_write_swap(uint16_t page)
{
    return fs_store(&fs_config,
                    m_pages[page].start_addr,
                    (uint32_t const *)&fds_page_tag_swap,
                    FDS_PAGE_TAG_SIZE);
}


/**@brief Tags a page as valid, i.e, ready for storage. */
static ret_code_t page_tag_write_valid(uint16_t page)
{
    return fs_store(&fs_config,
                    m_pages[page].start_addr,
                    (uint32_t const *)&fds_page_tag_valid,
                    FDS_PAGE_TAG_SIZE);
}


/**@brief Tags a valid page as being garbage collected. */
static ret_code_t page_tag_write_gc(uint16_t page)
{
    return fs_store(&fs_config,
                    m_pages[page].start_addr + 3,
                    (uint32_t const *)&fds_page_tag_gc,
                    1 /*Words*/);
}


/**@brief Given a page and a record, finds the next valid record. */
static ret_code_t scan_next_valid(uint16_t page, uint32_t const ** p_record)
{
    uint32_t const * p_next_rec = (*p_record);

    if (p_next_rec == NULL)
    {
        // This if the first invocation on this page, start from the beginning.
        p_next_rec = m_pages[page].start_addr + FDS_PAGE_TAG_SIZE;
    }
    else
    {
        // Jump to the next record.
        p_next_rec += (FDS_HEADER_SIZE + ((fds_header_t*)(*p_record))->tl.length_words);
    }

    // Scan until we find a valid record or until the end of the page.

    /** README: We might seek until the write_offset is reached, but it might not
     *  known at this point. */
    while ((p_next_rec < (m_pages[page].start_addr + FS_PAGE_SIZE_WORDS)) &&
           (*p_next_rec != FDS_ERASED_WORD)) // Did we jump to an erased word?
    {
        fds_header_t const * const p_header = (fds_header_t*)p_next_rec;

        if (header_is_valid(p_header))
        {
            // Bingo!
            *p_record = p_next_rec;
            return NRF_SUCCESS;
        }
        else
        {
            // The item is not valid, jump to the next.
            p_next_rec += (FDS_HEADER_SIZE + (p_header->tl.length_words));
        }
    }

    return NRF_ERROR_NOT_FOUND;
}


static ret_code_t seek_record(fds_record_desc_t * const p_desc)
{
    uint32_t const * p_record;
    uint16_t         page;
    bool             seek_all_pages = false;

    if ((p_desc->ptr_magic == FDS_MAGIC_HWORD) &&
        (p_desc->gc_magic  == m_gc_runs))
    {
        // No need to seek the file.
        return NRF_SUCCESS;
    }

    /** The pointer in the descriptor is not initialized, or GC
     *  has been run since the last time it was retrieved.
     *  We must seek the record again. */

    // Obtain the physical page ID.
    if (page_id_from_virtual_id(p_desc->vpage_id, &page) != NRF_SUCCESS)
    {
        page = 0;
        seek_all_pages = true;
    }

    do {
        // Let's find the address from where we should start seeking the record.
        p_record = m_pages[page].start_addr + FDS_PAGE_TAG_SIZE;

        /** Seek for a record with matching ID.
         *  We might get away with seeking to the page write offset, if it is known. */

        while ((p_record < (m_pages[page].start_addr + FS_PAGE_SIZE_WORDS)) &&
               (*p_record != FDS_ERASED_WORD))
        {
            fds_header_t const * const p_header = (fds_header_t*)p_record;

            if ((p_header->id != p_desc->record_id) ||
                (!header_is_valid(p_header)))
            {
                // ID doesnt't match or the record has been cleared. Jump to the next record.
                p_record += FDS_HEADER_SIZE + p_header->tl.length_words;
            }
            else
            {
                // Update the pointer in the descriptor.
                p_desc->p_rec     = p_record;
                p_desc->ptr_magic = FDS_MAGIC_HWORD;
                p_desc->gc_magic  = m_gc_runs;

                return NRF_SUCCESS;
            }
        }
    } while (seek_all_pages ? page++ < FDS_MAX_PAGES : 0);

    return NRF_ERROR_NOT_FOUND;
}


static ret_code_t find_record(fds_type_id_t     const * const p_type,
                              fds_instance_id_t const * const p_inst,
                              fds_record_desc_t       * const p_desc,
                              fds_find_token_t        * const p_token)
{
    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    // Here we distinguish between the first invocation and the and the others.
    if ((p_token->magic != FDS_MAGIC_WORD) ||
        !address_within_page_bounds(p_token->p_addr)) // Is the address is really okay?
    {
        // Initialize the token.
        p_token->magic    = FDS_MAGIC_WORD;
        p_token->vpage_id = 0;
        p_token->p_addr   = NULL;
    }
    else
    {
        // Look past the last record address.
         p_token->p_addr += (FDS_HEADER_SIZE + ((fds_header_t*)p_token->p_addr)->tl.length_words);
    }

    // Begin (or resume) searching for a record.
    for (; p_token->vpage_id < FDS_MAX_PAGES; p_token->vpage_id++)
    {
        uint16_t page = 0;

        // Obtain the physical page ID.
        page_id_from_virtual_id(p_token->vpage_id, &page);

        if (m_pages[page].page_type != FDS_PAGE_VALID)
        {
            // Skip this page.
            continue;
        }

        if (p_token->p_addr == NULL)
        {
            // If it's the first time the function is run, initialize the pointer.
            p_token->p_addr = m_pages[page].start_addr + FDS_PAGE_TAG_SIZE;
        }

        // Seek a valid record on this page, starting from the address stored in the token.
        while ((p_token->p_addr < (m_pages[page].start_addr + FS_PAGE_SIZE_WORDS)) &&
               (*p_token->p_addr != FDS_ERASED_WORD)) // Did we jump to an erased word?
        {
            fds_header_t const * const p_header = (fds_header_t*)p_token->p_addr;

            if (header_is_valid(p_header))
            {
                // A valid record was found, check its header for a match.
                bool item_match = false;

                if (p_type != NULL)
                {
                    if (p_header->tl.type == *p_type)
                    {
                        item_match = true;
                    }
                }

                if (p_inst != NULL)
                {
                    if (p_header->ic.instance == *p_inst)
                    {
                        item_match = (p_type == NULL) ? true : item_match && true;
                    }
                    else
                    {
                        item_match = false;
                    }
                }

                if (item_match)
                {
                    // We found the record! Update the descriptor.
                    p_desc->vpage_id  = m_pages[page].vpage_id;
                    p_desc->record_id = p_header->id;

                    p_desc->p_rec     = p_token->p_addr;
                    p_desc->ptr_magic = FDS_MAGIC_HWORD;
                    p_desc->gc_magic  = m_gc_runs;

                    return NRF_SUCCESS;
                }
            }
            // Jump to the next record.
            p_token->p_addr += (FDS_HEADER_SIZE + (p_header->tl.length_words));
        }

        /** We have seeked an entire page. Set the address in the token to NULL
         *  so that it will be set again on the next iteration. */
        p_token->p_addr = NULL;
    }

    /** If we couldn't find the record, zero the token structure
     *  so that it can be reused. */
    p_token->magic = 0x00;

    return NRF_ERROR_NOT_FOUND;
}


static void gc_init()
{
    // Set which pages to GC.
    for (uint16_t i = 0; i < FDS_MAX_PAGES; i++)
    {
        m_gc.do_gc_page[i] = (m_pages[i].page_type == FDS_PAGE_VALID);
    }
}


static void gc_reset()
{
    m_gc.state       = BEGIN;
    m_gc.cur_page    = 0;
    m_gc.p_scan_addr = NULL;
}


static void gc_set_state(fds_gc_state_t new_state)
{
    m_gc.state = new_state;
}


static ret_code_t gc_get_next_page(uint16_t * const next_page)
{
    for (uint16_t i = 0; i < FDS_MAX_PAGES; i++)
    {        
        if (m_gc.do_gc_page[i])
        {
            uint16_t records_open;

            CRITICAL_SECTION_ENTER();
            records_open = m_pages[i].records_open;
            CRITICAL_SECTION_EXIT();

            // Do not attempt to GC this page anymore.
            m_gc.do_gc_page[i] = false;

            // Only GC pages with no open records.
            if (records_open == 0)
            {
                *next_page = i;
                return NRF_SUCCESS;
            }   
        }
    }

    return NRF_ERROR_NOT_FOUND;
}


static ret_code_t gc_page()
{
    ret_code_t ret;

    ret = gc_get_next_page(&m_gc.cur_page);

    // No pages left to GC. GC has terminated. Reset GC data.
    if (ret != NRF_SUCCESS)
    {    
        gc_reset();

        return COMMAND_COMPLETED;
    }

    // Prepare to GC the page.
    gc_set_state(GC_PAGE);

    // Flag the page as being garbage collected.
    ret = page_tag_write_gc(m_gc.cur_page);

    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    return COMMAND_EXECUTING;
}


static ret_code_t gc_copy_record()
{
    ret_code_t fs_ret;

    // We have found a record to copy.
    fds_record_t const * const p_record = (fds_record_t*)m_gc.p_scan_addr;

    gc_set_state(COPY_RECORD);

    // Copy the item to swap.
    fs_ret = fs_store(&fs_config,
                      m_pages[m_gc.swap_page].start_addr + m_pages[m_gc.swap_page].write_offset,
                      (uint32_t*)p_record,
                      FDS_HEADER_SIZE + p_record->header.tl.length_words);

    if (fs_ret != NRF_SUCCESS)
    {
        // Oops :(
        // This is an error. Can we recover?
    }

    // Remember to update the swap page write offset.
    m_pages[m_gc.swap_page].write_offset += (FDS_HEADER_SIZE + p_record->header.tl.length_words);

    return COMMAND_EXECUTING;
}


static ret_code_t gc_ready_swap_page()
{
    ret_code_t fs_ret;

    /** A page has been scanned through. All valid records found were copied to swap.
     *  The swap page can now be flagged as a valid page. */
    gc_set_state(READY_SWAP);

    fs_ret = page_tag_write_valid(m_gc.swap_page);
    if (fs_ret != NRF_SUCCESS)
    {
        return fs_ret;
    }

    /** Do not update the page type in the internal page structure (m_pages)
     *  right away. (why?) */
    return COMMAND_EXECUTING;
}


static ret_code_t gc_seek_record()
{
    // Let's find a valid record which has not been copied yet.
    if (scan_next_valid(m_gc.cur_page, &m_gc.p_scan_addr) == NRF_SUCCESS)
    {
        /** The record is guaranteed to fit in the destination page,
         *  so we don't need to check its size. */
        return gc_copy_record();
    }
    else
    {
        /** No more (uncopied) records left on this page.
         *  The swap page can now be marked as a valid page. */
        return gc_ready_swap_page();
    }
}


static ret_code_t gc_new_swap_page()
{
    ret_code_t fs_ret;
    uint16_t   vpage_id;

    gc_set_state(NEW_SWAP);

    // Save the swap page virtual page ID.
    vpage_id = m_pages[m_gc.swap_page].vpage_id;

    /** The swap page has been marked as valid in Flash. We copy the GC'ed page
     *  write_offset and virtual page ID. */
    m_pages[m_gc.swap_page].page_type      = FDS_PAGE_VALID;
    m_pages[m_gc.swap_page].vpage_id       = m_pages[m_gc.cur_page].vpage_id;
    m_pages[m_gc.swap_page].words_reserved = m_pages[m_gc.cur_page].words_reserved;

    // The new swap page is now the page we just GC.
    m_gc.swap_page = m_gc.cur_page;

    // Update the write_offset, words_reserved and vpage_id fields for the new swap page.
    m_pages[m_gc.swap_page].page_type      = FDS_PAGE_SWAP;
    m_pages[m_gc.swap_page].vpage_id       = vpage_id;
    m_pages[m_gc.swap_page].write_offset   = FDS_PAGE_TAG_SIZE;
    m_pages[m_gc.swap_page].words_reserved = 0;

    /** Finally, erase the new swap page. Remember we still have to flag this
     *  new page as swap, but we'll wait the callback for this operation to do so. */
    fs_ret = fs_erase(&fs_config,
                      (uint32_t*)m_pages[m_gc.swap_page].start_addr,
                      FS_PAGE_SIZE_WORDS);

    if (fs_ret != NRF_SUCCESS)
    {
        return fs_ret;
    }

    return COMMAND_EXECUTING;
}


static ret_code_t gc_new_swap_page_init()
{
    ret_code_t fs_ret;

    gc_set_state(INIT_SWAP);

    fs_ret = page_tag_write_swap(m_gc.swap_page);
    if (fs_ret != NRF_SUCCESS)
    {
        return fs_ret;
    }

    return COMMAND_EXECUTING;
}


static ret_code_t gc_execute(uint32_t result)
{
    // TODO: Handle resuming GC.

    ret_code_t ret;

    if (result != NRF_SUCCESS)
    {
        // An operation failed. Report to the application.
        return result;
    }

    switch (m_gc.state)
    {
        case BEGIN:
        {
            // Increment the number of times the GC has been run.
            m_gc_runs++;
            // Sets up a list of pages to GC.
            gc_init();
            // Go !
            ret = gc_page();
        } break;

        case GC_PAGE:
            /** A page has been successfully flagged as being GC.
             *  Look for valid records to copy. */
            ret = gc_seek_record();
            break;

        case COPY_RECORD:
            /** A record has been copied to swap.
             *  Look for more records to copy. */
            ret = gc_seek_record();
            break;

        case READY_SWAP:
            /** The swap page has been flagged as 'valid' (ready).
             *  Let's prepare a new swap page. */
            ret = gc_new_swap_page();
            break;

        case NEW_SWAP:
            // A new swap page has been prepared. Let's flag it as swap.
            ret = gc_new_swap_page_init();
            break;

        case INIT_SWAP:
            /** The swap was flagged as swap in flash. Let's compress another page.
             *  Be sure to update the address where to scan from. */
            m_gc.p_scan_addr = NULL;
            ret = gc_page();
            break;

        default:
            // Should really not happen.
            ret = NRF_ERROR_INTERNAL;
            break;
    }

    return ret;
}


/**@brief Function for initializing the command queue. */
static void queues_init(void)
{
    memset(&m_cmd_queue,   0, sizeof(fds_cmd_queue_t));
    memset(&m_chunk_queue, 0, sizeof(fds_chunk_queue_t));
}


void cmd_queue_next(fds_cmd_t ** pp_cmd)
{
    if (*pp_cmd != &m_cmd_queue.cmd[FDS_CMD_QUEUE_SIZE - 1])
    {
        (*pp_cmd)++;
        return;
    }

    *pp_cmd = &m_cmd_queue.cmd[0];
}


void chunk_queue_next(fds_record_chunk_t ** pp_chunk)
{
    if ((*pp_chunk) != &m_chunk_queue.chunk[FDS_CHUNK_QUEUE_SIZE - 1])
    {
        (*pp_chunk)++;
        return;
    }

    *pp_chunk = &m_chunk_queue.chunk[0];
}


/**@brief Advances one position in the command queue. Returns true if the queue is not empty. */
static bool cmd_queue_advance(void)
{
    // Reset the current element.
    memset(&m_cmd_queue.cmd[m_cmd_queue.rp], 0, sizeof(fds_cmd_t));

    CRITICAL_SECTION_ENTER();
    if (m_cmd_queue.count != 0)
    {
        // Advance in the queue, wrapping around if necessary.
        m_cmd_queue.rp = (m_cmd_queue.rp + 1) % FDS_CMD_QUEUE_SIZE;
        m_cmd_queue.count--;
    }
    CRITICAL_SECTION_EXIT();

    return m_cmd_queue.count != 0;
}


/**@brief Returns the current chunk, and advances to the next in the queue. */
static bool chunk_queue_get_and_advance(fds_record_chunk_t ** pp_chunk)
{
    bool chunk_popped = false;

    CRITICAL_SECTION_ENTER();
    if (m_chunk_queue.count != 0)
    {
        // Point to the current chunk and advance the queue.
        *pp_chunk = &m_chunk_queue.chunk[m_chunk_queue.rp];

        m_chunk_queue.rp = (m_chunk_queue.rp + 1) % FDS_CHUNK_QUEUE_SIZE;
        m_chunk_queue.count--;

        chunk_popped = true;
    }
    CRITICAL_SECTION_EXIT();

    return chunk_popped;
}


static bool chunk_queue_skip(uint8_t num_op)
{
    bool chunk_skipped = false;

    CRITICAL_SECTION_ENTER();
    if (num_op <= m_chunk_queue.count)
    {
        m_chunk_queue.count -= num_op;
        chunk_skipped = true;
    }
    CRITICAL_SECTION_EXIT();

    return chunk_skipped;
}


/**@brief Reserves resources on both queues. */
static ret_code_t queue_reserve(uint8_t               num_cmd,
                                uint8_t               num_chunks,
                                fds_cmd_t          ** pp_cmd,
                                fds_record_chunk_t ** pp_chunk)
{
    uint8_t cmd_index;
    uint8_t chunk_index;

    // This is really just being safe.
    if (pp_cmd == NULL || ((pp_chunk == NULL) && (num_chunks != 0)))
    {
        return NRF_ERROR_NULL;
    }

    if (num_cmd == 0)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    CRITICAL_SECTION_ENTER();

    // Ensure there is enough space in the queues.
    if ((m_cmd_queue.count   > FDS_CMD_QUEUE_SIZE - num_cmd) ||
        (m_chunk_queue.count > FDS_CHUNK_QUEUE_SIZE - num_chunks))
    {
        CRITICAL_SECTION_EXIT();
        return NRF_ERROR_BUSY;
    }

    // Find the write position in the commands queue.
    cmd_index  = m_cmd_queue.count;
    cmd_index += m_cmd_queue.rp;
    cmd_index  = cmd_index % FDS_CMD_QUEUE_SIZE;

    *pp_cmd = &m_cmd_queue.cmd[cmd_index];
    m_cmd_queue.count += num_cmd;

    /* If no operations are associated with the command, such as is the case 
     * for initialization and compression, pp_chunk can be NULL. */
    if (num_chunks != 0)
    {
        chunk_index  = m_chunk_queue.count;
        chunk_index += m_chunk_queue.rp;
        chunk_index  = chunk_index % FDS_CHUNK_QUEUE_SIZE;

        *pp_chunk = &m_chunk_queue.chunk[chunk_index];
        m_chunk_queue.count += num_chunks;
    }

    CRITICAL_SECTION_EXIT();

    return NRF_SUCCESS;
}


/**@brief Cancel the reservation on resources on queues. */
static void queue_reserve_cancel(uint8_t num_cmd, uint8_t num_chunks)
{
    CRITICAL_SECTION_ENTER();
    m_cmd_queue.count   -= num_cmd;
    m_chunk_queue.count -= num_chunks;
    CRITICAL_SECTION_EXIT();
}


static void pages_init(uint16_t * const p_pages_avail,
                       bool     * const p_write_page_tag,
                       bool     * const p_resume_comp)
{
    *p_pages_avail    = 0;
    *p_write_page_tag = false;
    *p_resume_comp    = false;

    /** Scan pages and setup page data.
     *  This function does NOT perform write operations in flash. */
    for (uint16_t i = 0; i < FDS_MAX_PAGES; i++)
    {
        // Initialize page data. Note that start_addr must be set BEFORE invoking page_identify().
        m_pages[i].start_addr     = fs_config.p_start_addr + (i * FS_PAGE_SIZE_WORDS);
        m_pages[i].write_offset   = FDS_PAGE_TAG_SIZE;
        m_pages[i].vpage_id       = i;
        m_pages[i].records_open   = 0;
        m_pages[i].words_reserved = 0;

        m_pages[i].page_type      = page_identify(i);

        switch (m_pages[i].page_type)
        {
            case FDS_PAGE_UNDEFINED:
            {
                if (page_is_empty(i))
                {
                    /* We have found an erased page, which can be initialized.
                     * This will require a write in flash. */
                    m_pages[i].page_type = FDS_PAGE_ERASED;
                    *p_write_page_tag = true;
                }
            } break;

            case FDS_PAGE_VALID:
            {
                /** If a page is valid, we update its write offset.
                 *  Additionally, page_scan will update the last known record ID. */
                page_scan(i, &m_pages[i].write_offset);
                (*p_pages_avail)++;
            } break;

            case FDS_PAGE_SWAP:
            {
                m_gc.swap_page    = i;
                m_swap_page_avail = true;
            } break;

            case FDS_PAGE_GC:
            {
                /** There is an ongoing garbage collection.
                 *  We should resume the operation, which we don't yet. */
                m_gc.cur_page   = i;
                m_gc.state      = GC_PAGE;
                *p_resume_comp  = true;
            } break;

            default:
                break;
        }
    }
}


// NOTE: Adds FDS_HEADER_SIZE automatically.
static ret_code_t write_space_reserve(uint16_t length_words, uint16_t * vpage_id)
{
    bool     space_reserved  = false;
    uint16_t total_len_words = length_words + FDS_HEADER_SIZE;

    if (total_len_words >= FS_PAGE_SIZE_WORDS - FDS_PAGE_TAG_SIZE)
    {
        return NRF_ERROR_INVALID_LENGTH;
    }

    for (uint16_t page = 0; page < FDS_MAX_PAGES; page++)
    {
        if ((m_pages[page].page_type == FDS_PAGE_VALID) &&
            (page_has_space(page, total_len_words)))
        {
            space_reserved = true;
            *vpage_id      = m_pages[page].vpage_id;

            CRITICAL_SECTION_ENTER();
            m_pages[page].words_reserved += total_len_words;
            CRITICAL_SECTION_EXIT();
            
            break;
        }
    }

    return space_reserved ? NRF_SUCCESS : NRF_ERROR_NO_MEM;
}


static bool chunk_is_aligned(fds_record_chunk_t const * const p_chunk, uint8_t num_parts)
{
    for (uint8_t i = 0; i < num_parts; i++)
    {
        if (!is_word_aligned(p_chunk[i].p_data))
        {
            return false;
        }
    }

    return true;
}


static ret_code_t init_execute(uint32_t result, uint32_t const * p_page_addr)
{
    uint16_t   cur_page;
    bool       page_tag_written = false;

    if (result != NRF_SUCCESS)
    {
        // Oops. Error.
        return result;
    }

    // Here we just distinguish between the first invocation and the others.
    cur_page = p_page_addr == NULL ? 0 : page_by_addr(p_page_addr) + 1;

    if (cur_page == FDS_MAX_PAGES)
    {
        // We have finished. We'd need to set some flags.
        flag_set(FDS_FLAG_INITIALIZED);
        flag_clear(FDS_FLAG_INITIALIZING);

        return COMMAND_COMPLETED;
    }

    while (cur_page < FDS_MAX_PAGES && !page_tag_written)
    {
        if (m_pages[cur_page].page_type == FDS_PAGE_ERASED)
        {
            page_tag_written = true;

            if (m_swap_page_avail)
            {
                if (page_tag_write_valid(cur_page) != NRF_SUCCESS)
                {
                    // Oops. Error.
                }
                // Update the page type.
                m_pages[cur_page].page_type = FDS_PAGE_VALID;
            }
            else
            {
                if (page_tag_write_swap(cur_page) != NRF_SUCCESS)
                {
                    // Oops. Error.
                }
                // Update the page type.
                m_pages[cur_page].page_type = FDS_PAGE_SWAP;

                /** Update compression data. We set this information in init_pages
                 *  if it is available, otherwise, we should set it here. */
                m_swap_page_avail = true;
                m_gc.swap_page = cur_page;
            }
        }

        cur_page++;
    }

    if (!page_tag_written)
    {
        if (m_swap_page_avail)
        {
            return COMMAND_COMPLETED;
        }
        else
        {
            // There is no empty space to use as swap.
            // Notify user that no compression is available?
        }
    }

    return COMMAND_EXECUTING;
}


/**@brief Function to execute write and update commands.
 *
 */
static ret_code_t store_execute(uint32_t result, fds_cmd_t * const p_cmd)
{
    ret_code_t           fs_ret;
    fds_record_chunk_t * p_chunk = NULL;
    fds_page_t         * p_page  = NULL;
    uint32_t           * p_write_addr;

    // Using virtual page IDs allows other operations to be queued even if GC has been requested.
    page_from_virtual_id(p_cmd->vpage_id, &p_page);

    if (result != NRF_SUCCESS)
    {
        // The previous operation has failed, update the page data.
        p_page->write_offset   += (FDS_HEADER_SIZE + (p_cmd->chunk_offset - FDS_WRITE_OFFSET_DATA));
        p_page->words_reserved -= (FDS_HEADER_SIZE + (p_cmd->chunk_offset - FDS_WRITE_OFFSET_DATA));

        return result;
    }

    // Compute the write address (just syntatic sugar).
    p_write_addr = (uint32_t*)(p_page->start_addr + p_page->write_offset);

    // Execute the operation.
    switch (p_cmd->op_code)
    {
        case FDS_OP_WRITE_TL:
        {
            fs_ret = fs_store(&fs_config,
                              p_write_addr + FDS_WRITE_OFFSET_TL,
                              (uint32_t*)&p_cmd->record_header.tl,
                              FDS_HEADER_SIZE_TL /*Words*/);

            // Set the next operation to be executed.
            p_cmd->op_code = FDS_OP_WRITE_ID;

        } break;

        case FDS_OP_WRITE_ID:
        {
            fs_ret = fs_store(&fs_config,
                              p_write_addr + FDS_WRITE_OFFSET_ID,
                              (uint32_t*)&p_cmd->record_header.id,
                              FDS_HEADER_SIZE_ID /*Words*/);

            p_cmd->op_code = FDS_OP_WRITE_CHUNK;

        } break;

        case FDS_OP_WRITE_CHUNK:
        {
            // Decrement the number of chunks left to write.
            p_cmd->num_chunks--;

            // Retrieve the chunk to be written.
            chunk_queue_get_and_advance(&p_chunk);

            fs_ret = fs_store(&fs_config,
                              p_write_addr + p_cmd->chunk_offset,
                              p_chunk->p_data,
                              p_chunk->length_words);

            // Accumulate the offset.
            p_cmd->chunk_offset += p_chunk->length_words;

            if (p_cmd->num_chunks == 0)
            {
                /** We have written all the record chunks; we'll write
                 *  IC last as a mean to 'validate' the record. */
                p_cmd->op_code = FDS_OP_WRITE_IC;
            }

        } break;

        case FDS_OP_WRITE_IC:
        {
            fs_ret = fs_store(&fs_config,
                              p_write_addr + FDS_WRITE_OFFSET_IC,
                              (uint32_t*)&p_cmd->record_header.ic,
                              FDS_HEADER_SIZE_IC /*Words*/);

            // This is the final operation.
            p_cmd->op_code = FDS_OP_DONE;

        } break;

        case FDS_OP_DONE:
        {
            // We have successfully written down the IC. The command has completed successfully.
            p_page->write_offset   += (FDS_HEADER_SIZE + (p_cmd->chunk_offset - FDS_WRITE_OFFSET_DATA));
            p_page->words_reserved -= (FDS_HEADER_SIZE + (p_cmd->chunk_offset - FDS_WRITE_OFFSET_DATA));

            return COMMAND_COMPLETED;

        };

        default:
            fs_ret = NRF_ERROR_INTERNAL;
            break;
    }

    // If fs_store did not succeed, the command has failed.
    if (fs_ret != NRF_SUCCESS)
    {
        /** We're not going to receive a callback from fstorage
         *  so we update the page data right away. */
        p_page->write_offset   += (FDS_HEADER_SIZE + (p_cmd->chunk_offset - FDS_WRITE_OFFSET_DATA));
        p_page->words_reserved -= (FDS_HEADER_SIZE + (p_cmd->chunk_offset - FDS_WRITE_OFFSET_DATA));

        // We should propagate the error from fstorage.
        return fs_ret;
    }

    // An operation has successfully been executed. Wait for the callback.
    return COMMAND_EXECUTING;
}


static ret_code_t clear_execute(ret_code_t result, fds_cmd_t * const p_cmd)
{
    ret_code_t        ret;
    fds_record_desc_t desc;

    // This must persist across calls.
    static fds_find_token_t tok;

    if (result != NRF_SUCCESS)
    {
        // A previous operation has failed. Propagate the error.
        return result;
    }

    switch (p_cmd->op_code)
    {
        case FDS_OP_CLEAR_TL:
        {
            // We were provided a descriptor for the record.
            desc.vpage_id  = p_cmd->vpage_id;
            desc.record_id = p_cmd->record_header.id;

            /** Unfortunately, we always seek the record in this case,
             *  because we don't buffer an entire record descriptor in the
             *  fds_cmd_t structure. Keep in mind though, that we will
             *  seek one page at most. */
            if (seek_record(&desc) != NRF_SUCCESS)
            {
                // The record never existed, or it is already cleared.
                ret = NRF_ERROR_NOT_FOUND;
            }
            else
            {
                // Copy the record key, so that it may be returned in the callback.
                p_cmd->record_header.tl.type     = ((fds_header_t*)desc.p_rec)->tl.type;
                p_cmd->record_header.ic.instance = ((fds_header_t*)desc.p_rec)->ic.instance;

                ret = fs_store(&fs_config,
                               desc.p_rec,
                               (uint32_t*)&m_fds_tl_invalid,
                               FDS_HEADER_SIZE_TL);
            }

            p_cmd->op_code = FDS_OP_DONE;

        } break;

        case FDS_OP_CLEAR_INSTANCE:
        {
            if (find_record(NULL, &p_cmd->record_header.ic.instance,
                            &desc, &tok) != NRF_SUCCESS)
            {
                // No more records to be found.
                p_cmd->op_code = FDS_OP_DONE;

                // Zero the token, so that we may reuse it.
                memset(&tok, 0, sizeof(fds_find_token_t));

                /** We won't receive a callback, since no flash operation
                 *  was initiated. The command has finished. */
                ret = COMMAND_COMPLETED;
            }
            else
            {
                ret = fs_store(&fs_config,
                               desc.p_rec,
                               (uint32_t*)&m_fds_tl_invalid,
                               FDS_HEADER_SIZE_TL);
            }
        } break;

        case FDS_OP_DONE:
        {
            /** The last operation completed successfully.
             *  The command has finished. Return. */
            ret = COMMAND_COMPLETED;
        } break;

        default:
            ret = NRF_ERROR_INVALID_DATA;
            break;
    }

    // Await for the operation result.
    return ret;
}


static ret_code_t cmd_queue_process(void)
{
    ret_code_t        ret;
    fds_cmd_t * const p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];

    switch (p_cmd->id)
    {
        case FDS_CMD_INIT:
            ret = init_execute(NRF_SUCCESS, NULL);
            break;

        case FDS_CMD_WRITE:
        case FDS_CMD_UPDATE:
            ret = store_execute(NRF_SUCCESS, p_cmd);
            break;

        case FDS_CMD_CLEAR:
        case FDS_CMD_CLEAR_INST:
            ret = clear_execute(NRF_SUCCESS, p_cmd);
            break;

        case FDS_CMD_GC:
            ret = gc_execute(NRF_SUCCESS);
            break;

        default:
            ret = NRF_ERROR_FORBIDDEN;
            break;
    }

    if ((ret == COMMAND_EXECUTING) || (ret == COMMAND_COMPLETED))
    {
        return NRF_SUCCESS;
    }

    // This is an error.
    return ret;
}


static ret_code_t cmd_queue_process_start(void)
{
    bool start_processing = false;

    if (!flag_is_set(FDS_FLAG_PROCESSING))
    {
        flag_set(FDS_FLAG_PROCESSING);
        start_processing = true;
    }

    if (!start_processing)
    {
        // We are awaiting a callback, so there is no need to manually start queue processing.
        return NRF_SUCCESS;
    }

    return cmd_queue_process();
}


static void fs_callback(uint8_t           op_code,
                        uint32_t          result,
                        uint32_t  const * p_data,
                        fs_length_t       length)
{
    ret_code_t         ret;
    fds_cmd_t        * p_cmd = &m_cmd_queue.cmd[m_cmd_queue.rp];
    fds_record_key_t   record_key;

    switch (p_cmd->id)
    {
        case FDS_CMD_INIT:
            ret = init_execute(result, p_data);
            break;

        case FDS_CMD_WRITE:
        case FDS_CMD_UPDATE:
            ret = store_execute(result, p_cmd);
            break;

        case FDS_CMD_CLEAR:
        case FDS_CMD_CLEAR_INST:
            ret = clear_execute(result, p_cmd);
            break;

        case FDS_CMD_GC:
            ret = gc_execute(result);
            break;

        default:
            // Should not happen.
            ret = NRF_ERROR_INTERNAL;
            break;
    }

    if (ret == COMMAND_EXECUTING /*=NRF_SUCCESS*/)
    {
        /** The current command is still being processed.
         *  The command queue does not need to advance. */
        return;
    }

    // Initialize the fds_record_key_t structure needed for the callback.
    record_key.type     = p_cmd->record_header.tl.type;
    record_key.instance = p_cmd->record_header.ic.instance;

    // The command has either completed or an operation (and thus the command) has failed.
    if (ret == COMMAND_COMPLETED)
    {
        // The command has completed successfully. Notify the application.
        app_notify(NRF_SUCCESS, p_cmd->id, p_cmd->record_header.id, record_key);
    }
    else
    {
        /** An operation has failed. This is fatal for the execution of a command.
         *  Skip other operations associated with the current command.
         *  Notify the user of the failure.  */
        chunk_queue_skip(p_cmd->num_chunks);
        app_notify(ret /*=result*/, p_cmd->id, p_cmd->record_header.id, record_key);
    }

    // Advance the command queue, and if there is still something in the queue, process it.
    if (cmd_queue_advance())
    {
        /** Only process the queue if there are no pending commands being queued, since they
         *  will begin to process the queue on their own. Be sure to clear
         *  the flag FDS_FLAG_PROCESSING though ! */
        if (atomic_counter_is_zero())
        {
            cmd_queue_process();
        }
        else
        {
            flag_clear(FDS_FLAG_PROCESSING);
        }
    }
    else
    {
        /** No more elements in the queue. Clear the FDS_FLAG_PROCESSING flag,
         *  so that new commands can start the queue processing. */
        flag_clear(FDS_FLAG_PROCESSING);
    }
}


ret_code_t fds_init()
{
    ret_code_t   fs_ret;
    fds_cmd_t  * p_cmd;
    uint16_t     pages_avail;
    bool         write_page_tag;
    bool         resume_compression;

    fds_record_key_t const dummy_key = {.type     = FDS_TYPE_ID_INVALID,
                                        .instance = FDS_INSTANCE_ID_INVALID};

    if (flag_is_set(FDS_FLAG_INITIALIZED))
    {
        // Notify immediately.
        app_notify(NRF_SUCCESS, FDS_CMD_INIT, 0 /*unused*/, dummy_key /*unused*/);
        return NRF_SUCCESS;
    }

    if (flag_is_set(FDS_FLAG_INITIALIZING))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    fs_ret = fs_init();
    if (fs_ret != NRF_SUCCESS)
    {
        // fs_init() failed, propagate the error.
        return fs_ret;
    }

    queues_init();

    /** Initialize the last known record to zero.
     *  Its value will be updated by page_scan() called in pages_init(). */
    m_last_rec_id = 0;

    // Initialize the page table containing all info on pages (address, type etc).
    pages_init(&pages_avail, &write_page_tag, &resume_compression);

    if (pages_avail == 0 && !write_page_tag)
    {
        return NRF_ERROR_NO_MEM;
    }

    /** This flag means fds_init() has been called. However,
     *  the module is NOT yet initialized. */
    flag_set(FDS_FLAG_INITIALIZING);

    if (resume_compression)
    {
        return NRF_SUCCESS;
    }

    if (write_page_tag)
    {
        if (queue_reserve(FDS_CMD_QUEUE_SIZE_INIT, 0, &p_cmd, NULL) != NRF_SUCCESS)
        {
            // Should never happen.
            return NRF_ERROR_BUSY;
        }

        // Initialize the command in the queue.
        p_cmd->id = FDS_CMD_INIT;

        return cmd_queue_process_start();
    }
    else
    {
        /* No flash operation is necessary for initialization.
         * We can notify the application immediately. */
        flag_set  (FDS_FLAG_INITIALIZED);
        flag_clear(FDS_FLAG_INITIALIZING);
        app_notify(NRF_SUCCESS, FDS_CMD_INIT, 0 /*unused*/, dummy_key /*unused*/);
    }

    return NRF_SUCCESS;
}


ret_code_t fds_open(fds_record_desc_t * const p_desc,
                    fds_record_t      * const p_record)
{
    uint16_t page;

    if (p_desc == NULL || p_record == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (page_id_from_virtual_id(p_desc->vpage_id, &page) != NRF_SUCCESS)
    {
        // Should not happen.
        return NRF_ERROR_INVALID_DATA;
    }

    // Seek the record if necessary.
    if (seek_record(p_desc) == NRF_SUCCESS)
    {
        if (header_is_valid((fds_header_t*)p_desc->p_rec))
        {
            CRITICAL_SECTION_ENTER();
            m_pages[page].records_open++;
            CRITICAL_SECTION_EXIT();

            p_record->header = *((fds_header_t*)p_desc->p_rec);
            p_record->p_data = (p_desc->p_rec + FDS_HEADER_SIZE);

            return NRF_SUCCESS;
        }
    }

    /** The record could not be found.
     *  It either never existed or it has been cleared. */
    return NRF_ERROR_NOT_FOUND;
}


ret_code_t fds_close(fds_record_desc_t const * const p_desc)
{
    uint16_t page;

    if (p_desc == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (page_id_from_virtual_id(p_desc->vpage_id, &page) != NRF_SUCCESS)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    CRITICAL_SECTION_ENTER();
    m_pages[page].records_open--;
    CRITICAL_SECTION_EXIT();

    return NRF_SUCCESS;
}


static ret_code_t write_enqueue(fds_record_desc_t        * const p_desc,
                                fds_record_key_t                 key,
                                uint8_t                          num_chunks,
                                fds_record_chunk_t               chunks[],
                                fds_write_token_t  const * const p_tok,
                                bool                             do_update)
{
    ret_code_t           ret;
    fds_cmd_t          * p_cmd;
    fds_record_chunk_t * p_chunk = NULL;
    uint16_t             vpage_id;
    uint16_t             length_words = 0;
    uint8_t              cmd_queue_elems;

    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if ((key.type     == FDS_TYPE_ID_INVALID) ||
        (key.instance == FDS_INSTANCE_ID_INVALID))
    {
        return NRF_ERROR_INVALID_DATA;
    }

    if (!chunk_is_aligned(chunks, num_chunks))
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    cmd_queue_elems = do_update ? FDS_CMD_QUEUE_SIZE_UPDATE : FDS_CMD_QUEUE_SIZE_WRITE;

    // Reserve space on both queues, and obtain pointers to the first elements reserved.
    ret = queue_reserve(cmd_queue_elems,
                        num_chunks,
                        &p_cmd,
                        &p_chunk);

    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    // No space was previously reserved for this operation.
    if (p_tok == NULL)
    {
        // Compute the total length of the record.
        for (uint8_t i = 0; i < num_chunks; i++)
        {
            length_words += chunks[i].length_words;
        }

        /** Find a page where we can write the data. Reserve the space necessary
         *  to write the metadata as well. */
        ret = write_space_reserve(length_words, &vpage_id);
        if (ret != NRF_SUCCESS)
        {
            // If there is no space available, cancel the queue reservation.
            queue_reserve_cancel(cmd_queue_elems, num_chunks);
            return ret;
        }
    }
    else
    {
        length_words = p_tok->length_words;
        vpage_id     = p_tok->vpage_id;
    }

    // Initialize the command.
    p_cmd->id           = do_update ? FDS_CMD_UPDATE : FDS_CMD_WRITE;
    p_cmd->op_code      = FDS_OP_WRITE_TL;
    p_cmd->num_chunks   = num_chunks;
    p_cmd->chunk_offset = FDS_WRITE_OFFSET_DATA;
    p_cmd->vpage_id     = vpage_id;

    // Fill in the header information.
    p_cmd->record_header.id              = record_id_new();
    p_cmd->record_header.tl.type         = key.type;
    p_cmd->record_header.tl.length_words = length_words;
    p_cmd->record_header.ic.instance     = key.instance;
    p_cmd->record_header.ic.checksum     = 0;

    // Buffer the record chunks in the queue.
    for (uint8_t i = 0; i < num_chunks; i++)
    {
        p_chunk->p_data       = chunks[i].p_data;
        p_chunk->length_words = chunks[i].length_words;
        chunk_queue_next(&p_chunk);
    }

    if (do_update)
    {
        // Clear
        cmd_queue_next(&p_cmd);
        p_cmd->id      = FDS_CMD_CLEAR;
        p_cmd->op_code = FDS_OP_CLEAR_TL;

        p_cmd->vpage_id         = p_desc->vpage_id;
        p_cmd->record_header.id = p_desc->record_id;
    }

    // Initialize the record descriptor, if provided.
    if (p_desc != NULL)
    {
        p_desc->vpage_id  = vpage_id;
        // Don't invoke record_id_new() again.
        p_desc->record_id = p_cmd->record_header.id;
    }

    return cmd_queue_process_start();
}


ret_code_t fds_reserve(fds_write_token_t * const p_tok, uint16_t length_words)
{
    uint16_t vpage_id;

    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_tok == NULL)
    {
        return NRF_ERROR_NULL;
    }

    // Reserve space on the page. write_space_reserve() accounts for the header.
    if (write_space_reserve(length_words, &vpage_id) == NRF_SUCCESS)
    {
        p_tok->vpage_id     = vpage_id;
        p_tok->length_words = length_words;

        return NRF_SUCCESS;
    }

    return NRF_ERROR_NO_MEM;
}


ret_code_t fds_reserve_cancel(fds_write_token_t * const p_tok)
{
    fds_page_t * p_page;

    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_tok == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (page_from_virtual_id(p_tok->vpage_id, &p_page) != NRF_SUCCESS)
    {
        // Could not find the virtual page. This shouldn't happen.
        return NRF_ERROR_INVALID_DATA;
    }

    if ((p_page->words_reserved - p_tok->length_words) < 0)
    {
        /** We are trying to cancel a reservation for more words than how many are
         *  currently reserved on the page. This is shouldn't happen. */
        return NRF_ERROR_INVALID_DATA;
    }

    // Free the space which had been reserved.
    p_page->words_reserved -= p_tok->length_words;

    // Clean the token.
    p_tok->vpage_id     = 0;
    p_tok->length_words = 0;

    return NRF_SUCCESS;
}


ret_code_t fds_write(fds_record_desc_t  * const p_desc,
                     fds_record_key_t           key,
                     uint8_t                    num_chunks,
                     fds_record_chunk_t         chunks[])
{
    ret_code_t ret;
    atomic_counter_inc();
    ret = write_enqueue(p_desc, key, num_chunks, chunks, NULL, false /*not an update*/);
    atomic_counter_dec();
    return ret;
}


ret_code_t fds_write_reserved(fds_write_token_t  const * const p_tok,
                              fds_record_desc_t        * const p_desc,
                              fds_record_key_t                 key,
                              uint8_t                          num_chunks,
                              fds_record_chunk_t               chunks[])
{
    ret_code_t ret;
    atomic_counter_inc();
    ret = write_enqueue(p_desc, key, num_chunks, chunks, p_tok, false /*not an update*/);
    atomic_counter_dec();
    return ret;
}


static ret_code_t clear_enqueue(fds_record_desc_t * const p_desc)
{
    ret_code_t   ret;
    fds_cmd_t  * p_cmd;

    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_desc == NULL)
    {
        return NRF_ERROR_NULL;
    }

    ret = queue_reserve(FDS_CMD_QUEUE_SIZE_CLEAR, 0, &p_cmd, NULL);

    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    // Initialize the command.
    p_cmd->id        = FDS_CMD_CLEAR;
    p_cmd->op_code   = FDS_OP_CLEAR_TL;

    p_cmd->record_header.id = p_desc->record_id;
    p_cmd->vpage_id         = p_desc->vpage_id;

    return cmd_queue_process_start();
}


ret_code_t fds_clear(fds_record_desc_t * const p_desc)
{
    ret_code_t ret;
    atomic_counter_inc();
    ret = clear_enqueue(p_desc);
    atomic_counter_dec();
    return ret;
}


static ret_code_t clear_by_instance_enqueue(fds_instance_id_t instance)
{
    ret_code_t   ret;
    fds_cmd_t  * p_cmd;

    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    ret  = queue_reserve(FDS_CMD_QUEUE_SIZE_CLEAR, 0, &p_cmd, NULL);

    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    p_cmd->id      = FDS_CMD_CLEAR_INST;
    p_cmd->op_code = FDS_OP_CLEAR_INSTANCE;

    p_cmd->record_header.ic.instance = instance;

    return cmd_queue_process_start();
}

ret_code_t fds_clear_by_instance(fds_instance_id_t instance)
{
    ret_code_t ret;
    atomic_counter_inc();
    ret = clear_by_instance_enqueue(instance);
    atomic_counter_dec();
    return ret;
}


ret_code_t fds_update(fds_record_desc_t  * const p_desc,
                      fds_record_key_t           key,
                      uint8_t                    num_chunks,
                      fds_record_chunk_t         chunks[])
{
    ret_code_t ret;
    atomic_counter_inc();
    ret = write_enqueue(p_desc, key, num_chunks, chunks, NULL, true /*update*/);
    atomic_counter_dec();
    return ret;
}


static ret_code_t gc_enqueue()
{
    ret_code_t  ret;
    fds_cmd_t * p_cmd;

    if (!flag_is_set(FDS_FLAG_INITIALIZED))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    ret = queue_reserve(FDS_CMD_QUEUE_SIZE_GC, 0, &p_cmd, NULL);
    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    p_cmd->id = FDS_CMD_GC;

    // Set compression parameters.
    m_gc.state = BEGIN;

    return cmd_queue_process_start();
}


ret_code_t fds_gc()
{
    ret_code_t ret;
    atomic_counter_inc();
    ret = gc_enqueue();
    atomic_counter_dec();
    return ret;   
}


ret_code_t fds_find(fds_type_id_t             type,
                    fds_instance_id_t         instance,
                    fds_record_desc_t * const p_desc,
                    fds_find_token_t  * const p_token)
{
    if (p_desc == NULL || p_token == NULL)
    {
        return NRF_ERROR_NULL;
    }

    return find_record(&type, &instance, p_desc, p_token);
}


ret_code_t fds_find_by_type(fds_type_id_t             type,
                            fds_record_desc_t * const p_desc,
                            fds_find_token_t  * const p_token)
{
    if (p_desc == NULL || p_token == NULL)
    {
        return NRF_ERROR_NULL;
    }

    return find_record(&type, NULL, p_desc, p_token);
}


ret_code_t fds_find_by_instance(fds_instance_id_t         instance,
                                fds_record_desc_t * const p_desc,
                                fds_find_token_t  * const p_token)
{
    if (p_desc == NULL || p_token == NULL)
    {
        return NRF_ERROR_NULL;
    }

    return find_record(NULL, &instance, p_desc, p_token);
}


ret_code_t fds_register(fds_cb_t cb)
{
    if (m_users == FDS_MAX_USERS)
    {
        return NRF_ERROR_NO_MEM;
    }

    m_cb_table[m_users] = cb;
    m_users++;

    return NRF_SUCCESS;
}


bool fds_descriptor_match(fds_record_desc_t const * const p_desc1,
                          fds_record_desc_t const * const p_desc2)
{
    if ((p_desc1 == NULL) || (p_desc2 == NULL))
    {
        return false;
    }

    return (p_desc1->record_id == p_desc2->record_id);
}


ret_code_t fds_descriptor_from_rec_id(fds_record_desc_t * const p_desc,
                                      fds_record_id_t           record_id)
{
    if (p_desc == NULL)
    {
        return NRF_ERROR_NULL;
    }

    p_desc->record_id = record_id;
    p_desc->vpage_id  = FDS_VPAGE_ID_UNKNOWN;

    return NRF_SUCCESS;
}

ret_code_t fds_record_id_from_desc(fds_record_desc_t const * const p_desc,
                                   fds_record_id_t         * const p_record_id)
{
    if (p_desc == NULL || p_record_id == NULL)
    {
        return NRF_ERROR_NULL;
    }

    *p_record_id = p_desc->record_id;

    return NRF_SUCCESS;
}

