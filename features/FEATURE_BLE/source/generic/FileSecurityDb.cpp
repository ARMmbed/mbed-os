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

#define DB_STORE_OFFSET_FLAGS         (0)
#define DB_STORE_OFFSET_LOCAL_KEYS    (DB_STORE_OFFSET_FLAGS + sizeof(SecurityDistributionFlags_t))
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
    PAD4( \
        sizeof(SecurityDistributionFlags_t) + \
        sizeof(SecurityEntryKeys_t) + \
        sizeof(SecurityEntryKeys_t) + \
        sizeof(SecurityEntryIdentity_t) + \
        sizeof(SecurityEntrySigning_t) \
    )

#define DB_SIZE_STORES \
    (FileSecurityDb::MAX_ENTRIES * DB_SIZE_STORE)

#define DB_OFFSET_VERSION          (0)
#define DB_OFFSET_RESTORE          (DB_OFFSET_VERSION + sizeof(DB_VERSION))
#define DB_OFFSET_LOCAL_IDENTITY   (DB_OFFSET_RESTORE + sizeof(bool))
#define DB_OFFSET_LOCAL_CSRK       (DB_OFFSET_LOCAL_IDENTITY + sizeof(SecurityEntryIdentity_t))
#define DB_OFFSET_LOCAL_SIGN_COUNT (DB_OFFSET_LOCAL_CSRK + sizeof(csrk_t))
#define DB_OFFSET_STORES           (DB_OFFSET_LOCAL_SIGN_COUNT + sizeof(sign_count_t))
#define DB_OFFSET_MAX              (DB_OFFSET_STORES + DB_SIZE_STORES)
#define DB_SIZE                    PAD4(DB_OFFSET_MAX)

typedef SecurityDb::entry_handle_t entry_handle_t;

FileSecurityDb::FileSecurityDb(FILE *db_file)
    : SecurityDb(),
      _db_file(db_file) {
    /* init the offset in entries so they point to file positions */
    for (size_t i = 0; i < get_entry_count(); i++) {
        _entries[i].file_offset = DB_OFFSET_STORES + i * DB_SIZE_STORE;
    }
}

FileSecurityDb::~FileSecurityDb() {
    fclose(_db_file);
}

FILE* FileSecurityDb::open_db_file(const char *db_path) {
    if (!db_path) {
        return NULL;
    }

    /* try to open an existing file */
    FILE *db_file = fopen(db_path, "rb+");

    if (!db_file) {
        /* file doesn't exist, create it */
        db_file = fopen(db_path, "wb+");
    }

    if (!db_file) {
        /* failed to create a file, abort */
        return NULL;
    }

    /* we will check the db file and if the version or size doesn't match
     * what we expect we will blank it */
    bool init = false;
    uint16_t version;

    fseek(db_file, DB_OFFSET_VERSION, SEEK_SET);

    if ((fread(&version, sizeof(version), 1, db_file) == 1) &&
        (version == DB_VERSION)) {
        /* if file size differs from database size init the file */
        fseek(db_file, 0, SEEK_END);
        if (ftell(db_file) != DB_SIZE) {
            init = true;
        }
    } else {
        init = true;
    }

    if (init) {
        return erase_db_file(db_file);
    }

    return db_file;
}

FILE* FileSecurityDb::erase_db_file(FILE* db_file) {
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

    return db_file;
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

    entry->flags.ltk_sent = true;

    db_write(&ltk, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS_LTK);
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

    db_write(&ediv, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS_EDIV);
    db_write(&rand, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS_RAND);
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

    entry->flags.ltk_stored = true;

    db_write(&ltk, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS_LTK);
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

    db_write(&ediv, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS_EDIV);
    db_write(&rand, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS_RAND);
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

    db_write(&irk, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY_IRK);
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

    db_write(&peer_address, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY_ADDRESS);
    db_write(&address_is_public, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY_ADDRESS_IS_PUBLIC);
}

void FileSecurityDb::set_entry_peer_csrk(
    entry_handle_t db_handle,
    const csrk_t &csrk
) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    entry->flags.csrk_stored = true;

    db_write(&csrk, entry->file_offset + DB_STORE_OFFSET_PEER_SIGNING);
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
    /* restore if requested */
    bool restore_toggle = false;
    db_read(&restore_toggle, DB_OFFSET_RESTORE);

    if (!restore_toggle) {
        erase_db_file(_db_file);

        db_write(&DB_VERSION, DB_OFFSET_VERSION);
        return;
    }

    db_read(&_local_identity, DB_OFFSET_LOCAL_IDENTITY);
    db_read(&_local_csrk, DB_OFFSET_LOCAL_CSRK);
    db_read(&_local_sign_counter, DB_OFFSET_LOCAL_SIGN_COUNT);

    /* read flags and sign counters */
    for (size_t i = 0; i < get_entry_count(); i++) {
        db_read(&_entries[i].flags, _entries[i].file_offset + DB_STORE_OFFSET_FLAGS);
        db_read(&_entries[i].peer_sign_counter, _entries[i].file_offset + DB_STORE_OFFSET_PEER_SIGNING_COUNT);
    }

}

void FileSecurityDb::sync(entry_handle_t db_handle) {
    entry_t *entry = as_entry(db_handle);
    if (!entry) {
        return;
    }

    db_write(&entry->peer_sign_counter, entry->file_offset + DB_STORE_OFFSET_PEER_SIGNING_COUNT);
    db_write(&entry->flags, entry->file_offset + DB_STORE_OFFSET_FLAGS);
}

void FileSecurityDb::set_restore(bool reload) {
    db_write(&reload, DB_OFFSET_RESTORE);
}

/* helper functions */

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

    SecurityEntryIdentity_t* identity = reinterpret_cast<SecurityEntryIdentity_t*>(_buffer);
    db_read(identity, entry->file_offset + DB_STORE_OFFSET_PEER_IDENTITY);

    return identity;
};

SecurityEntryKeys_t* FileSecurityDb::read_in_entry_peer_keys(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }

    SecurityEntryKeys_t* keys = reinterpret_cast<SecurityEntryKeys_t*>(_buffer);
    db_read(keys, entry->file_offset + DB_STORE_OFFSET_PEER_KEYS);

    return keys;
};

SecurityEntryKeys_t* FileSecurityDb::read_in_entry_local_keys(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }

    SecurityEntryKeys_t* keys = reinterpret_cast<SecurityEntryKeys_t*>(_buffer);
    db_read(keys, entry->file_offset + DB_STORE_OFFSET_LOCAL_KEYS);

    return keys;
};

SecurityEntrySigning_t* FileSecurityDb::read_in_entry_peer_signing(entry_handle_t db_entry) {
    entry_t *entry = as_entry(db_entry);
    if (!entry) {
        return NULL;
    }

    /* only read in the csrk */
    csrk_t* csrk = reinterpret_cast<csrk_t*>(_buffer);
    db_read(csrk, entry->file_offset + DB_STORE_OFFSET_PEER_SIGNING);


    /* use the counter held in memory */
    SecurityEntrySigning_t* signing = reinterpret_cast<SecurityEntrySigning_t*>(_buffer);
    signing->counter = entry->peer_sign_counter;

    return signing;
};

} /* namespace pal */
} /* namespace ble */
