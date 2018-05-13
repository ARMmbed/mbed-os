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

#include "FileSecurityDb.h"

namespace ble {
namespace generic {

const uint16_t DB_VERSION = 1;

#define DB_STORE_OFFSET_LOCAL_KEYS    (0)
#define DB_STORE_OFFSET_PEER_KEYS     (DB_STORE_OFFSET_LOCAL_KEYS + sizeof(SecurityEntryKeys_t))
#define DB_STORE_OFFSET_PEER_IDENTITY (DB_STORE_OFFSET_PEER_KEYS + sizeof(SecurityEntryKeys_t))
#define DB_STORE_OFFSET_PEER_SIGNING  (DB_STORE_OFFSET_PEER_IDENTITY + sizeof(SecurityEntryIdentity_t))

#define DB_STORE_OFFSET_LOCAL_KEYS_LTK  (DB_STORE_OFFSET_LOCAL_KEYS)
#define DB_STORE_OFFSET_LOCAL_KEYS_EDIV (DB_STORE_OFFSET_LOCAL_KEYS_LTK + sizeof(ltk_t))
#define DB_STORE_OFFSET_LOCAL_KEYS_RAND (DB_STORE_OFFSET_LOCAL_KEYS_EDIV + sizeof(ediv_t))

#define DB_STORE_OFFSET_PEER_KEYS_LTK  (DB_STORE_OFFSET_PEER_KEYS)
#define DB_STORE_OFFSET_PEER_KEYS_EDIV (DB_STORE_OFFSET_PEER_KEYS_LTK + sizeof(ltk_t))
#define DB_STORE_OFFSET_PEER_KEYS_RAND (DB_STORE_OFFSET_PEER_KEYS_EDIV + sizeof(ediv_t))

#define DB_STORE_OFFSET_PEER_IDENTITY_ADDRESS (DB_STORE_OFFSET_PEER_IDENTITY)
#define DB_STORE_OFFSET_PEER_IDENTITY_IRK     (DB_STORE_OFFSET_PEER_IDENTITY + sizeof(address_t))
#define DB_STORE_OFFSET_PEER_IDENTITY_ADDRESS_IS_PUBLIC (DB_STORE_OFFSET_PEER_IDENTITY_IRK + sizeof(irk_t))

#define DB_STORE_OFFSET_PEER_SIGNING_COUNT (DB_STORE_OFFSET_PEER_SIGNING + sizeof(csrk_t))

/* make size multiple of 4 */
#define PAD4(value) ((((value - 1) / 4) * 4) + 4)

#define DB_SIZE_STORE \
    PAD4(sizeof(SecurityEntryKeys_t) + \
    sizeof(SecurityEntryKeys_t) + \
    sizeof(SecurityEntryIdentity_t) + \
    sizeof(SecurityEntrySigning_t) + \
    sizeof(sign_count_t))

/* without the size of the file offset as we don't store it */
#define DB_SIZE_ENTRY \
    (sizeof(SecurityDistributionFlags_t) + sizeof(sign_count_t))

#define DB_SIZE_ENTRIES \
    (FileSecurityDb::MAX_ENTRIES * DB_SIZE_ENTRY)

#define DB_SIZE_STORES \
    (FileSecurityDb::MAX_ENTRIES * DB_SIZE_STORE)

#define DB_OFFSET_VERSION          (0)
#define DB_OFFSET_RESTORE          (DB_OFFSET_VERSION + sizeof(DB_VERSION))
#define DB_OFFSET_LOCAL_IDENTITY   (DB_OFFSET_RESTORE + sizeof(bool))
#define DB_OFFSET_LOCAL_CSRK       (DB_OFFSET_LOCAL_IDENTITY + sizeof(SecurityEntryIdentity_t))
#define DB_OFFSET_LOCAL_SIGN_COUNT (DB_OFFSET_LOCAL_CSRK + sizeof(csrk_t))
#define DB_OFFSET_ENTRIES          (DB_OFFSET_LOCAL_SIGN_COUNT + sizeof(sign_count_t))
#define DB_OFFSET_STORES           (DB_OFFSET_ENTRIES + DB_SIZE_ENTRIES)
#define DB_OFFSET_MAX              (DB_OFFSET_STORES + DB_SIZE_STORES)
#define DB_SIZE                    PAD4(DB_OFFSET_MAX)

typedef SecurityDb::entry_handle_t entry_handle_t;

FileSecurityDb::FileSecurityDb(FILE *db_file)
    : SecurityDb(),
      _db_file(db_file) {
    fseek(_db_file, DB_OFFSET_RESTORE, SEEK_SET);

    /* restore if requested */
    bool restore;
    if ((fread(&restore, sizeof(bool), 1, _db_file) == 1) && restore) {
        fseek(_db_file, DB_OFFSET_LOCAL_IDENTITY, SEEK_SET);
        fread(&_local_identity, sizeof(_local_identity), 1, _db_file);

        fseek(_db_file, DB_OFFSET_LOCAL_CSRK, SEEK_SET);
        fread(&_local_csrk, sizeof(_local_csrk), 1, _db_file);

        fseek(_db_file, DB_OFFSET_LOCAL_SIGN_COUNT, SEEK_SET);
        fread(&_local_sign_counter, sizeof(_local_sign_counter), 1, _db_file);

        fseek(_db_file, DB_OFFSET_ENTRIES, SEEK_SET);
        /* we read the entries partially and fill the offsets ourselves*/
        for (size_t i = 0; i < get_entry_count(); i++) {
            fread(&_entries[i], DB_SIZE_ENTRY, 1, _db_file);
        }
    }

    /* init the offset in entries so they point to file positions */
    for (size_t i = 0; i < get_entry_count(); i++) {
        _entries[i].file_offset = DB_OFFSET_STORES + i * DB_SIZE_STORE;
    }
}

FileSecurityDb::~FileSecurityDb() {
    fclose(_db_file);
}

FILE* FileSecurityDb::open_db_file(const char *db_path) {
    FILE *db_file = fopen(db_path, "wb+");
    if (db_file) {
        /* we will check the db file and if the version or size doesn't match
         * what we expect we will blank it */
        bool init = false;
        uint16_t version;

        fseek(db_file, DB_OFFSET_VERSION, SEEK_SET);

        if ((fread(&version, sizeof(version), 1, db_file) == 1) &&
            (version == DB_VERSION)) {
            /* version checks out, try the size */
            fseek(db_file, DB_SIZE - 1, SEEK_SET);
            /* read one byte and expect to hit EOF */
            if ((fread(&version, 1, 1, db_file) != 1) || !feof(db_file)) {
                init = true;
            }
        } else {
            init = true;
        }

        if (init) {
            fseek(db_file, 0, SEEK_SET);

            /* zero the file */
            const uint32_t zero = 0;
            size_t count = DB_SIZE / 4;
            while (count--) {
                if (fwrite(&zero, sizeof(zero), 1, db_file) != 1) {
                    fclose(db_file);
                    return NULL;
                }
            }

            if (fflush(db_file)) {
                fclose(db_file);
                return NULL;
            }
        }

        return db_file;
    }
    return NULL;
}

SecurityDistributionFlags_t* FileSecurityDb::get_distribution_flags(
    entry_handle_t db_handle
) {
    return reinterpret_cast<SecurityDistributionFlags_t*>(db_handle);
}

/* local keys */

/* set */
void FileSecurityDb::set_entry_local_ltk(
    entry_handle_t db_handle,
    const ltk_t &ltk
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS_LTK, SEEK_SET);
    fwrite(&ltk, sizeof(ltk_t), 1, _db_file);
}

void FileSecurityDb::set_entry_local_ediv_rand(
    entry_handle_t db_handle,
    const ediv_t &ediv,
    const rand_t &rand
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS_EDIV, SEEK_SET);
    fwrite(&ediv, sizeof(ediv_t), 1, _db_file);
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS_RAND, SEEK_SET);
    fwrite(&rand, sizeof(rand_t), 1, _db_file);
}

/* peer's keys */

/* set */

void FileSecurityDb::set_entry_peer_ltk(
    entry_handle_t db_handle,
    const ltk_t &ltk
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS_LTK, SEEK_SET);
    fwrite(&ltk, sizeof(ltk_t), 1, _db_file);
}

void FileSecurityDb::set_entry_peer_ediv_rand(
    entry_handle_t db_handle,
    const ediv_t &ediv,
    const rand_t &rand
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS_EDIV, SEEK_SET);
    fwrite(&ediv, sizeof(ediv_t), 1, _db_file);
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS_RAND, SEEK_SET);
    fwrite(&rand, sizeof(rand_t), 1, _db_file);
}

void FileSecurityDb::set_entry_peer_irk(
    entry_handle_t db_handle,
    const irk_t &irk
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    entry->flags.irk_stored = true;

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY_IRK, SEEK_SET);
    fwrite(&irk, sizeof(irk_t), 1, _db_file);
}

void FileSecurityDb::set_entry_peer_bdaddr(
    entry_handle_t db_handle,
    bool address_is_public,
    const address_t &peer_address
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY_ADDRESS, SEEK_SET);
    fwrite(&peer_address, sizeof(address_t), 1, _db_file);
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY_ADDRESS_IS_PUBLIC, SEEK_SET);
    fwrite(&address_is_public, sizeof(bool), 1, _db_file);
}

void FileSecurityDb::set_entry_peer_csrk(
    entry_handle_t db_handle,
    const csrk_t &csrk
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_SIGNING, SEEK_SET);
    /* only write in the csrk */
    fwrite(&csrk, sizeof(csrk_t), 1, _db_file);
}

void FileSecurityDb::set_entry_peer_sign_counter(
    entry_handle_t db_handle,
    sign_count_t sign_counter
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        entry->peer_sign_counter = sign_counter;
    }
}

/* saving and loading from nvm */

void FileSecurityDb::restore() {
}

void FileSecurityDb::sync(entry_handle_t db_handle) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_SIGNING_COUNT, SEEK_SET);
    fwrite(&entry->peer_sign_counter, sizeof(sign_count_t), 1, _db_file);
}

void FileSecurityDb::set_restore(bool reload) {
}

uint8_t FileSecurityDb::get_entry_count() {
    return MAX_ENTRIES;
}

SecurityDistributionFlags_t* FileSecurityDb::get_entry_handle_by_index(uint8_t index) {
    if (index < MAX_ENTRIES) {
        return &_entries[index].flags;
    } else {
        return NULL;
    }
}

void FileSecurityDb::reset_entry(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return;
    }

    fseek(_db_file, entry->file_offset, SEEK_SET);
    const uint32_t zero = 0;
    size_t count = DB_SIZE_STORE / 4;
    while (count--) {
        fwrite(&zero, sizeof(zero), 1, _db_file);
    }

    entry->flags = SecurityDistributionFlags_t();
    entry->peer_sign_counter = 0;
}

SecurityEntryIdentity_t* FileSecurityDb::read_in_entry_peer_identity(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY, SEEK_SET);
    fread(&_buffer, sizeof(SecurityEntryIdentity_t), 1, _db_file);
    return reinterpret_cast<SecurityEntryIdentity_t*>(_buffer);
};

SecurityEntryKeys_t* FileSecurityDb::read_in_entry_peer_keys(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS, SEEK_SET);
    fread(&_buffer, sizeof(SecurityEntryKeys_t), 1, _db_file);
    return reinterpret_cast<SecurityEntryKeys_t*>(_buffer);
};

SecurityEntryKeys_t* FileSecurityDb::read_in_entry_local_keys(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS, SEEK_SET);
    fread(&_buffer, sizeof(SecurityEntryKeys_t), 1, _db_file);
    return reinterpret_cast<SecurityEntryKeys_t*>(_buffer);
};

SecurityEntrySigning_t* FileSecurityDb::read_in_entry_peer_signing(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }
    fseek(_db_file, entry->file_offset + DB_STORE_OFFSET_PEER_SIGNING, SEEK_SET);

    /* only read in the csrk */
    fread(&_buffer, sizeof(csrk_t), 1, _db_file);
    SecurityEntrySigning_t* signing = reinterpret_cast<SecurityEntrySigning_t*>(_buffer);
    /* use the counter held in memory */
    signing->counter = entry->peer_sign_counter;

    return signing;
};

} /* namespace pal */
} /* namespace ble */
