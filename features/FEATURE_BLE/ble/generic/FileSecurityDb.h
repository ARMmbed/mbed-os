/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GENERIC_FILE_SECURITY_DB_H_
#define GENERIC_FILE_SECURITY_DB_H_

#include "SecurityDb.h"

#include <stdio.h>

namespace ble {
namespace generic {

/** Filesystem implementation */
class FileSecurityDb : public SecurityDb {
private:

    struct entry_t {
        SecurityDistributionFlags_t flags;
        sign_count_t peer_sign_counter;
        size_t file_offset;
    };

    static const size_t MAX_ENTRIES = 5;

    static entry_t* as_entry(entry_handle_t db_handle) {
        return reinterpret_cast<entry_t*>(db_handle);
    }

    template<class T>
    void db_read(T *value, long int offset) {
        fseek(_db_file, offset, SEEK_SET);
        fread(value, sizeof(T), 1, _db_file);
    }

    template<class T>
    void db_write(T *value, long int offset) {
        fseek(_db_file, offset, SEEK_SET);
        fwrite(value, sizeof(T), 1, _db_file);
    }

public:
    FileSecurityDb(FILE *db_file);
    virtual ~FileSecurityDb();

    /**
     * Validates or creates a file for the security database.
     * @param db_path path to the file
     * @return FILE handle open and ready for use by the database or NULL if unavailable
     */
    static FILE* open_db_file(const char *db_path);

    virtual SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t db_handle
    );


    /* local keys */

    /* set */
    virtual void set_entry_local_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    );

    virtual void set_entry_local_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /* peer's keys */

    /* set */

    virtual void set_entry_peer_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    );

    virtual void set_entry_peer_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    );

    virtual void set_entry_peer_irk(
        entry_handle_t db_handle,
        const irk_t &irk
    );

    virtual void set_entry_peer_bdaddr(
        entry_handle_t db_handle,
        bool address_is_public,
        const address_t &peer_address
    );

    virtual void set_entry_peer_csrk(
        entry_handle_t db_handle,
        const csrk_t &csrk
    );

    virtual void set_entry_peer_sign_counter(
        entry_handle_t db_handle,
        sign_count_t sign_counter
    );

    /* saving and loading from nvm */

    virtual void restore();

    virtual void sync(entry_handle_t db_handle);

    virtual void set_restore(bool reload);

private:
    virtual uint8_t get_entry_count();

    virtual SecurityDistributionFlags_t* get_entry_handle_by_index(uint8_t index);

    virtual void reset_entry(entry_handle_t db_handle);

    virtual SecurityEntryIdentity_t* read_in_entry_peer_identity(entry_handle_t db_handle);
    virtual SecurityEntryKeys_t* read_in_entry_peer_keys(entry_handle_t db_handle);
    virtual SecurityEntryKeys_t* read_in_entry_local_keys(entry_handle_t db_handle);
    virtual SecurityEntrySigning_t* read_in_entry_peer_signing(entry_handle_t db_handle);

    /**
     * Zero the db file.
     * @param db_file filehandle for file to erase
     * @return filehandle when successful, otherwise NULL
     */
    static FILE* erase_db_file(FILE* db_file);

private:
    entry_t _entries[MAX_ENTRIES];
    FILE *_db_file;
    uint8_t _buffer[sizeof(SecurityEntryKeys_t)];
};

} /* namespace pal */
} /* namespace ble */

#endif /*GENERIC_FILE_SECURITY_DB_H_*/
