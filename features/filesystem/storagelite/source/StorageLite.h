/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_STORAGELITE_H
#define MBED_STORAGELITE_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "nvstore.h"

#define STORAGELITE_ENABLED 1

#if (!NVSTORE_ENABLED) || !defined(MBEDTLS_ENTROPY_C)
#undef STORAGELITE_ENABLED
#define STORAGELITE_ENABLED 0
#endif

#if (STORAGELITE_ENABLED) || defined(DOXYGEN_ONLY)
#include <stdint.h>
#include <stdio.h>
#include "platform/NonCopyable.h"
#include "PlatformMutex.h"
#include "BlockDevice.h"

typedef enum {
    STORAGELITE_SUCCESS                =  0,
    STORAGELITE_READ_ERROR             = -1,
    STORAGELITE_WRITE_ERROR            = -2,
    STORAGELITE_NOT_FOUND              = -3,
    STORAGELITE_DATA_CORRUPT           = -4,
    STORAGELITE_BAD_VALUE              = -5,
    STORAGELITE_BUFF_TOO_SMALL         = -6,
    STORAGELITE_NO_SPACE_ON_BD         = -7,
    STORAGELITE_OS_ERROR               = -8,
    STORAGELITE_UNINITIALIZED          = -9,
    STORAGELITE_MAX_FILES_REACHED      = -10,
    STORAGELITE_NOT_SUPPORTED          = -11,
    STORAGELITE_RB_PROTECT_ERROR       = -12,
} storagelite_status_e;

#ifndef STORAGELITE_MAX_FILES
#define STORAGELITE_MAX_FILES 64
#endif

// defines 2 areas - active and nonactive, not configurable
#define STORAGELITE_NUM_AREAS        2

/** StorageLite class
 *
 *  Key value storage over a block device
 */

class StorageLite : mbed::NonCopyable<StorageLite> {
public:

    StorageLite();

    virtual ~StorageLite();

    /**
     * @brief Returns maximal number of files.
     *
     * @returns Number of files.
     */
    size_t get_max_files() const;

    /**
     * @brief Returns file data, given file name (as a binary blob).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     * @param[in]  data_buf             Buffer to store data on.
     * @param[in]  data_buf_size        Data buffer size (bytes).
     * @param[out] actual_data_size     Actual data size (bytes).
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     *          STORAGELITE_BUFF_TOO_SMALL    Not enough memory in user buffer.
     */
    int get(const void *file_name, size_t file_name_size, void *data_buf,
            size_t data_buf_size, size_t& actual_data_size);

    /**
     * @brief Returns file data, given file name (as a string).
     *
     * @param[in]  file_name            File name buffer (string).
     * @param[in]  data_buf             Buffer to store data on.
     * @param[in]  data_buf_size        Data buffer size (bytes).
     * @param[out] actual_data_size     Actual data size (bytes).
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     *          STORAGELITE_BUFF_TOO_SMALL    Not enough memory in user buffer.
     */
    int get(const char *file_name, void *data_buf,
            size_t data_buf_size, size_t& actual_data_size);

    /**
     * @brief Returns file size, given file name (as a binary blob).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     * @param[out] actual_data_size     Actual data size (bytes).
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     */
    int get_file_size(const void *file_name, size_t file_name_size, size_t& actual_data_size);

    /**
     * @brief Returns file size, given file name (as a string).
     *
     * @param[in]  file_name            File name buffer (string).
     * @param[out] actual_data_size     Actual data size (bytes).
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     */
    int get_file_size(const char *file_name, size_t& actual_data_size);

    /**
     * @brief Returns file flags, given file name (as a binary blob).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     * @param[out] flags                Flags.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     */
    int get_file_flags(const void *file_name, size_t file_name_size, uint32_t& flags);

    /**
     * @brief Returns file flags, given file name (as a string).
     *
     * @param[in]  file_name            File name buffer (string).
     * @param[out] flags                Flags.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     */
    int get_file_flags(const char *file_name, uint32_t& flags);

    /**
     * @brief Checks whether file exists, given file name (as a binary blob).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     *
     * @returns STORAGELITE_SUCCESS           Success (file found).
     *          STORAGELITE_NOT_FOUND         File was not found.
     */
    int file_exists(const void *file_name, size_t file_name_size);

    /**
     * @brief Checks whether file exists, given file name (as a string).
     *
     * @param[in]  file_name            File name buffer (string).
     *
     * @returns STORAGELITE_SUCCESS           Success (file found).
     *          STORAGELITE_NOT_FOUND         File was not found.
     */
    int file_exists(const char *file_name);

    /**
     * @brief Sets file data, given file name (as a binary blob) and data.
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     * @param[in]  data_buf             Buffer to store data on.
     * @param[in]  data_buf_size        Data buffer size (bytes).
     * @param[in]  flags                Flags.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_WRITE_ERROR       Physical error writing data.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     *          STORAGELITE_NO_SPACE_ON_BD    Not enough space on BD.
     */
    int set(const void *file_name, size_t file_name_size, const void *data_buf,
            size_t data_buf_size, uint32_t flags);

    /**
     * @brief Sets file data, given file name (as a string) and data.
     *
     * @param[in]  file_name            File name buffer (string).
     * @param[in]  data_buf             Buffer to store data on.
     * @param[in]  data_buf_size        Data buffer size (bytes).
     * @param[in]  flags                Flags.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_WRITE_ERROR       Physical error writing data.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     *          STORAGELITE_NO_SPACE_ON_BD    Not enough space on BD.
     */
    int set(const char *file_name, const void *data_buf,
            size_t data_buf_size, uint32_t flags);

    /**
     * @brief Remove a file, given a file name (as a binary blob).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     */
    int remove(const void *file_name, size_t file_name_size);

    /**
     * @brief Remove a file, given a file name (as a string).
     *
     * @param[in]  file_name            File name buffer (string).
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found.
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     */
    int remove(const char *file_name);

    /**
     * @brief Returns name of first file and a search handle for next iterations.
     *
     * @param[in]  file_name             File name buffer.
     * @param[in]  file_name_buf_size    File name buffer size.
     * @param[out] actual_file_name_size Actual file name size.
     * @param[out] handle                Iteration handle.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found (no existing files).
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     *          STORAGELITE_BUFF_TOO_SMALL    Not enough memory in user buffer.
     */
    int get_first_file(void *file_name, size_t file_name_buf_size,
                       size_t& actual_file_name_size, uint32_t& handle);

    /**
     * @brief Returns name of next file given iteration handle (handle is updated).
     *        Function return is STORAGELITE_NOT_FOUND at the end of the search.
     *
     * @param[in]  file_name             File name buffer.
     * @param[in]  file_name_buf_size    File name buffer size.
     * @param[out] actual_file_name_size Actual file name size.
     * @param[out] handle                Iteration handle.
     *
     * @returns STORAGELITE_SUCCESS           Success.
     *          STORAGELITE_NOT_FOUND         File was not found (past last file).
     *          STORAGELITE_READ_ERROR        Physical error reading data.
     *          STORAGELITE_DATA_CORRUPT      Data is corrupt.
     *          STORAGELITE_BAD_VALUE         Bad value in any of the parameters.
     *          STORAGELITE_BUFF_TOO_SMALL    Not enough memory in user buffer.
     */
    int get_next_file(void *file_name, size_t file_name_buf_size,
                      size_t& actual_file_name_size, uint32_t& handle);

    /**
     * @brief Initializes StorageLite component.
     *
     * @param[in]  bd                     Underlying block device.
     * @param[in]  max_files              Maximal number of files.
     *
     * @returns STORAGELITE_SUCCESS       Initialization completed successfully.
     *          STORAGELITE_READ_ERROR    Physical error reading data.
     *          STORAGELITE_WRITE_ERROR   Physical error writing data (on recovery).
     */
    int init(BlockDevice *bd, size_t max_files = STORAGELITE_MAX_FILES);

    /**
     * @brief Deinitializes StorageLite component.
     *        Warning: This function is not thread safe and should not be called
     *        concurrently with other StorageLite functions.
     *
     * @returns STORAGELITE_SUCCESS       Deinitialization completed successfully.
     */
    int deinit();

    /**
     * @brief Reset StorageLite areas (delete all data).
     *
     * @returns STORAGELITE_SUCCESS       Reset completed successfully.
     *          STORAGELITE_READ_ERROR    Physical error reading data.
     *          STORAGELITE_WRITE_ERROR   Physical error writing data.
     */
    int reset();

    /**
     * @brief Reset StorageLite to factory defaults. i.e. copy all files defined as
     *        backup copies on top of their active copies.
     *
     * @returns STORAGELITE_SUCCESS       Reset completed successfully.
     *          STORAGELITE_READ_ERROR    Physical error reading data.
     *          STORAGELITE_WRITE_ERROR   Physical error writing data.
     */
    int factory_reset();

    /**
     * @brief Return StorageLite size (area size).
     *
     * @returns StorageLite size.
     */
    size_t size() const;

    /**
     * @brief Return StorageLite free size.
     *
     * @returns StorageLite free size.
     */
    size_t free_size() const;

    static const uint32_t rollback_protect_flag;
    static const uint32_t encrypt_flag;
    static const uint32_t update_backup_flag;

    static const size_t max_name_size = 1024;
    static const size_t max_data_size = 256 * 1024;

private:
    typedef struct {
        uint32_t address;
        size_t   size;
    } storagelite_area_data_t;

    BlockDevice *_bd;
    BlockDevice *_user_bd, *_buff_bd;
    bool _init_done;
    uint32_t _init_attempts;
    uint8_t _active_area;
    uint16_t _max_files;
    uint16_t _active_area_version;
    uint32_t _free_space_offset;
    size_t _size;
    PlatformMutex *_mutex;
    void *_entropy;
    uint16_t _num_ram_table_entries;
    void *_ram_table;
    storagelite_area_data_t _area_params[STORAGELITE_NUM_AREAS];
    uint32_t _prog_align_size;
    uint8_t *_work_buf;
    uint8_t *_encrypt_key;
    uint8_t *_auth_key;
    bool _variant_bd_erase_unit_size;

    /**
     * @brief Read a block from an area.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset in area.
     * @param[in]  size                   Number of bytes to read.
     * @param[in]  buf                    Output buffer.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int read_area(uint8_t area, uint32_t offset, uint32_t size, void *buf);

    /**
     * @brief Write a block to an area.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset in area.
     * @param[in]  size                   Number of bytes to write.
     * @param[in]  buf                    Input buffer.
     *
     * @returns 0 for success, non-zero for failure.
     */
    int write_area(uint8_t area, uint32_t offset, uint32_t size, const void *buf);

    /**
     * @brief Erase an area.
     *
     * @param[in]  area                   Area.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int erase_area(uint8_t area);

    /**
     * @brief Erase an erase unit.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset in area.
     *
     * @returns 0 for success, non-zero for failure.
     */
    int erase_erase_unit(uint8_t area, uint32_t offset);

    /**
     * @brief Calculate addresses and sizes of areas.
     */
    void calc_area_params();

    /**
     * @brief Read an StorageLite record from a given location.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset of record in area.
     * @param[in]  name_buf               file name buffer.
     * @param[in]  name_buf_size          file name buffer size (bytes).
     * @param[out] actual_name_size       Actual file name size (bytes).
     * @param[in]  data_buf               Data buffer.
     * @param[in]  data_buf_size          Data buffer size (bytes).
     * @param[out] actual_data_size       Actual data size (bytes).
     * @param[in]  copy_name              Copy name to user buffer.
     * @param[in]  allow_partial_name     Allow returning partial name to name buffer.
     * @param[in]  check_expected_name    Check whether file name belongs to this record.
     * @param[in]  copy_data              Copy data to user buffer.
     * @param[in]  calc_hash              Calculate hash (on file name).
     * @param[out] hash                   Calculated hash.
     * @param[out] flags                  Record flags.
     * @param[out] nvstore_key            NVstore key (in rollback protection case).
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int read_record(uint8_t area, uint32_t offset, void *name_buf, uint16_t name_buf_size,
                    uint16_t& actual_name_size, void *data_buf, uint32_t data_buf_size,
                    uint32_t& actual_data_size, bool copy_name, bool allow_partial_name,
                    bool check_expected_name, bool copy_data, bool calc_hash, uint32_t& hash,
                    uint32_t& flags, uint16_t& nvstore_key, uint32_t& next_offset);

    /**
     * @brief Write an StorageLite record to a given location.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset of record in area.
     * @param[in]  name_buf               Name.
     * @param[in]  name_buf_size          Name size (bytes).
     * @param[in]  data_buf               Data buffer.
     * @param[in]  data_buf_size          Data size (bytes).
     * @param[in]  flags                  Record flags.
     * @param[in]  nvstore_key            NVStore key to store in rollback protection case.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int write_record(uint8_t area, uint32_t offset, const void *name_buf,
                     uint16_t name_buf_size, const void *data_buf, uint32_t data_buf_size,
                     uint32_t flags, uint16_t nvstore_key, uint32_t& next_offset);

    /**
     * @brief Write a master record of a given area.
     *
     * @param[in]  area                   Area.
     * @param[in]  version                Area version.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int write_master_record(uint8_t area, uint16_t version, uint32_t& next_offset);

    /**
     * @brief Copy a record from one area to the opposite one.
     *
     * @param[in]  from_area              Area to copy record from.
     * @param[in]  from_offset            Offset in source area.
     * @param[in]  to_offset              Offset in destination area.
     * @param[out] to_next_offset         Offset of next record in destination area.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int copy_record(uint8_t from_area, uint32_t from_offset, uint32_t to_offset,
                    uint32_t& to_next_offset);

    /**
     * @brief As part of GC process, copy all records in RAM table to the opposite area.
     *
     * @param[in]  is_backup              Copy records according to is_backup flag in RAM table.
     * @param[in]  from_area              Area to copy record from.
     * @param[in]  to_offset              Offset in destination area.
     * @param[out] to_next_offset         Offset of next record in destination area.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int copy_all_records(bool is_backup, uint8_t from_area, uint32_t to_offset, uint32_t& to_next_offset);

    /**
     * @brief Garbage collection (compact all records from active area to the standby one).
     *
     * @param[in]  factory_reset_mode     Only copy backup copies of files (factory reset).
     *
     * @returns 0 for success, nonzero for failure.
     */
    int garbage_collection(bool factory_reset_mode);


    /**
     * @brief Find a file according to name and set it.
     *
     * @param[in]  area                   Area to write to.
     * @param[in]  offset                 Offset in target area.
     * @param[in]  file_name              File name buffer (binary).
     * @param[in]  file_name_size         File name buffer size.
     * @param[in]  data_buf               Buffer to store data on.
     * @param[in]  data_buf_size          Data buffer size (bytes).
     * @param[in]  flags                  Flags.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int find_and_set(uint8_t area, uint32_t offset, const void *file_name,
                     uint16_t file_name_size, const void *data_buf, uint32_t data_buf_size,
                     uint32_t flags, uint32_t& next_offset);
    /**
     * @brief Actual logics of get API (covers also all other get APIs).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     * @param[in]  copy_data            Copy data to user buffer.
     * @param[in]  data_buf             Buffer to store data on.
     * @param[in]  data_buf_size        Data buffer size (bytes).
     * @param[out] actual_data_size     Actual data size (bytes).
     * @param[out] flags                Flags.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int do_get(const void *file_name, uint16_t file_name_size,  bool copy_data,
               void *data_buf, uint32_t data_buf_size, uint32_t& actual_data_size,
               uint32_t& flags);

    /**
     * @brief Actual logics of set API (covers also the remove API).
     *
     * @param[in]  file_name            File name buffer (binary).
     * @param[in]  file_name_size       File name buffer size.
     * @param[in]  data_buf             Buffer to store data on.
     * @param[in]  data_buf_size        Data buffer size (bytes).
     * @param[in]  flags                Flags.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int do_set(const void *file_name, uint16_t file_name_size,
               const void *data_buf, uint32_t data_buf_size, uint32_t flags);

    /**
     * @brief Build RAM table and update _free_space_offset (scanning all the records in the area).
     *
     * @param[in]  backup_only            Bring only records with update backup flag set.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int build_ram_table(bool backup_only);

    /**
     * @brief Free all allocated rollback protect keys (or only non backup ones).
     *
     * @param[in]  non_backup_only        Free just the ones that are not backup copies.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int free_rollback_protect_keys(bool non_backup_only);

    /**
     * @brief Calculate offset from start of erase unit.
     *
     * @param[in]  area                  Area.
     * @param[in]  offset                Offset in area.
     * @param[out] offset_from_start     Offset from start of erase unit.
     * @param[out] dist_to_end           Distance to end of erase unit.
     *
     * @returns offset in erase unit.
     */
    void offset_in_erase_unit(uint8_t area, uint32_t offset, uint32_t& offset_from_start,
                              uint32_t& dist_to_end);

    /**
     * @brief Check whether erase unit is erased (from offset until end of unit).
     *
     * @param[in]  area                  Area.
     * @param[in]  offset                Offset in area.
     * @param[out] erased                Unit is erased.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int is_erase_unit_erased(uint8_t area, uint32_t offset, bool& erased);

    /**
     * @brief Before writing a record, check whether we are crossing an erase unit.
     *        If we do, check if it's erased, and erase it if not.
     *
     * @param[in]  area                  Area.
     * @param[in]  offset                Offset in area.
     * @param[in]  size                  Write size.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int check_erase_before_write(uint8_t area, uint32_t offset, uint32_t size);

    /**
     * @brief Manipulate RAM flags and offset according to record flags.
     *
     * @param[in]  flags                 Record flags.
     * @param[in]  ram_table_ind         RAM table index.
     * @param[in]  check_in_table        Check value in RAM table.
     * @param[out] ram_flags             Modified RAM flags.
     * @param[out] offset                Modified offset.
     *
     * @returns none.
     */
    void ram_flags_and_offset_by_rec_flags(uint32_t flags, uint16_t ram_table_ind, bool check_in_table,
                                           uint32_t& ram_flags, uint32_t& offset);

};
/** @}*/

#endif // STORAGELITE_ENABLED

#endif
