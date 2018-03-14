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

#ifndef MBED_NVSTORE_H
#define MBED_NVSTORE_H

// These addresses need to be configured according to board (in mbed_lib.json)
#ifndef DEVICE_FLASH
#undef NVSTORE_ENABLED
#define NVSTORE_ENABLED 0
#endif

#if (NVSTORE_ENABLED) || defined(DOXYGEN_ONLY)
#include <stdint.h>
#include <stdio.h>
#include "platform/NonCopyable.h"
#include "PlatformMutex.h"
#include "FlashIAP.h"

typedef enum {
    NVSTORE_SUCCESS                =  0,
    NVSTORE_READ_ERROR             = -1,
    NVSTORE_WRITE_ERROR            = -2,
    NVSTORE_NOT_FOUND              = -3,
    NVSTORE_DATA_CORRUPT           = -4,
    NVSTORE_BAD_VALUE              = -5,
    NVSTORE_BUFF_TOO_SMALL         = -6,
    NVSTORE_FLASH_AREA_TOO_SMALL   = -7,
    NVSTORE_OS_ERROR               = -8,
    NVSTORE_ALREADY_EXISTS         = -9,
} nvstore_status_e;

#ifndef NVSTORE_MAX_KEYS
#define NVSTORE_MAX_KEYS 16
#endif

// defines 2 areas - active and nonactive, not configurable
#define NVSTORE_NUM_AREAS        2

/** NVStore class
 *
 *  Class for storing data by keys in the internal flash
 */

class NVStore : private mbed::NonCopyable<NVStore> {
public:

    /**
    * @brief As a singleton, return the single instance of the class.
    *        Reason for this class being a singleton is the following:
    *        - Ease the use for users of this class not having to coordinate instantiations.
    *        - Lazy instantiation of internal data (which we can't achieve with simple static classes).
    *
    * @returns Singleton instance reference.
    */
    static NVStore& get_instance()
    {
        // Use this implementation of singleton (Meyer's) rather than the one that allocates
        // the instance on the heap because it ensures destruction at program end (preventing warnings
        // from memory checking tools).
        static NVStore instance;
        return instance;
    }

    virtual ~NVStore();

    /**
     * @brief Returns number of keys.
     *
     * @returns Number of keys.
     */
    uint16_t get_max_keys() const;

    /**
     * @brief Set number of keys.
     *
     * @returns None.
     */
    void set_max_keys(uint16_t num_keys);

    /**
     * @brief Return maximal possible number of keys (in this flash configuration).
     *
     * @returns Max possible number of keys.
     */
    uint16_t get_max_possible_keys();

    /**
     * @brief Returns one item of data programmed on Flash, given key.
     *
     * @param[in]  key                  Key of stored item.
     * @param[in]  buf_size             Length of input buffer in bytes.
     * @param[in]  buf                  Buffer to store data on.
     *
     * @param[out] actual_size          Actual size of returned data.
     *
     * @returns NVSTORE_SUCCESS           Value was found on Flash.
     *          NVSTORE_NOT_FOUND         Value was not found on Flash.
     *          NVSTORE_READ_ERROR        Physical error reading data.
     *          NVSTORE_DATA_CORRUPT      Data on Flash is corrupt.
     *          NVSTORE_BAD_VALUE         Bad value in any of the parameters.
     *          NVSTORE_BUFF_TOO_SMALL    Not enough memory in user buffer.
     */
    int get(uint16_t key, uint16_t buf_size, void *buf, uint16_t &actual_size);

    /**
     * @brief Returns size of the data programmed on Flash, given key.
     *
     * @param[in]  key                  Key of stored item.
     * @param[out] actual_size          Actual size of item
     *
     * @returns NVSTORE_SUCCESS           Value was found on Flash.
     *          NVSTORE_NOT_FOUND         Value was not found on Flash.
     *          NVSTORE_READ_ERROR        Physical error reading data.
     *          NVSTORE_DATA_CORRUPT      Data on Flash is corrupt.
     *          NVSTORE_BAD_VALUE         Bad value in any of the parameters.
     */
    int get_item_size(uint16_t key, uint16_t &actual_size);


    /**
     * @brief Programs one item of data on Flash, given key.
     *
     * @param[in]  key                  Key of stored item.
     * @param[in]  buf_size             Item size in bytes.
     * @param[in]  buf                  Buffer containing data.
     *
     * @returns NVSTORE_SUCCESS           Value was successfully written on Flash.
     *          NVSTORE_WRITE_ERROR       Physical error writing data.
     *          NVSTORE_BAD_VALUE         Bad value in any of the parameters.
     *          NVSTORE_FLASH_AREA_TOO_SMALL
     *                                    Not enough space in Flash area.
     *          NVSTORE_ALREADY_EXISTS    Item set with write once API already exists.
     *
     */
    int set(uint16_t key, uint16_t buf_size, const void *buf);

    /**
     * @brief Programs one item of data on Flash, given key, allowing no consequent sets to this key.
     *
     * @param[in]  key                  Key of stored item.
     * @param[in]  buf_size             Item size in bytes.
     * @param[in]  buf                  Buffer containing data.
     *
     * @returns NVSTORE_SUCCESS           Value was successfully written on Flash.
     *          NVSTORE_WRITE_ERROR       Physical error writing data.
     *          NVSTORE_BAD_VALUE         Bad value in any of the parameters.
     *          NVSTORE_FLASH_AREA_TOO_SMALL
     *                                    Not enough space in Flash area.
     *          NVSTORE_ALREADY_EXISTS    Item set with write once API already exists.
     *
     */
    int set_once(uint16_t key, uint16_t buf_size, const void *buf);


    /**
     * @brief Remove an item from flash.
     *
     * @param[in]  key                  Key of stored item.
     *
     * @returns NVSTORE_SUCCESS           Value was successfully written on Flash.
     *          NVSTORE_WRITE_ERROR       Physical error writing data.
     *          NVSTORE_BAD_VALUE         Bad value in any of the parameters.
     *          NVSTORE_FLASH_AREA_TOO_SMALL
     *                                    Not enough space in Flash area.
     *
     */
    int remove(uint16_t key);

    /**
     * @brief Initializes NVStore component.
     *
     * @returns NVSTORE_SUCCESS       Initialization completed successfully.
     *          NVSTORE_READ_ERROR    Physical error reading data.
     *          NVSTORE_WRITE_ERROR   Physical error writing data (on recovery).
     *          NVSTORE_FLASH_AREA_TOO_SMALL
     *                                Not enough space in Flash area.
     */
    int init();

    /**
     * @brief Deinitializes NVStore component.
     *        Warning: This function is not thread safe and should not be called
     *        concurrently with other NVStore functions.
     *
     * @returns NVSTORE_SUCCESS       Deinitialization completed successfully.
     */
    int deinit();

    /**
     * @brief Reset Flash NVStore areas.
     *        Warning: This function is not thread safe and should not be called
     *        concurrently with other NVStore functions.
     *
     * @returns NVSTORE_SUCCESS       Reset completed successfully.
     *          NVSTORE_READ_ERROR    Physical error reading data.
     *          NVSTORE_WRITE_ERROR   Physical error writing data.
     */
    int reset();

    /**
     * @brief Return NVStore size (area size).
     *
     * @returns NVStore size.
     */
    size_t size();

    /**
     * @brief Return address and size of an NVStore area.
     *
     * @param[in]  area                   Area.
     * @param[out] address                Area address.
     * @param[out] size                   Area size (bytes).
     *
     * @returns NVSTORE_SUCCESS           Success.
     *          NVSTORE_BAD_VALUE         Bad area parameter.
     */
    int get_area_params(uint8_t area, uint32_t &address, size_t &size);


private:
    typedef struct {
        uint32_t address;
        size_t   size;
    } nvstore_area_data_t;

    int _init_done;
    uint32_t _init_attempts;
    uint8_t _active_area;
    uint16_t _max_keys;
    uint16_t _active_area_version;
    uint32_t _free_space_offset;
    size_t _size;
    PlatformMutex *_mutex;
    uint32_t *_offset_by_key;
    nvstore_area_data_t _flash_area_params[NVSTORE_NUM_AREAS];
    static nvstore_area_data_t initial_area_params[NVSTORE_NUM_AREAS];
    mbed::FlashIAP *_flash;
    uint32_t _min_prog_size;
    uint8_t *_page_buf;

    // Private constructor, as class is a singleton
    NVStore();

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
    int flash_read_area(uint8_t area, uint32_t offset, uint32_t size, void *buf);

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
    int flash_write_area(uint8_t area, uint32_t offset, uint32_t size, const void *buf);

    /**
     * @brief Erase an area.
     *
     * @param[in]  area                   Area.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int flash_erase_area(uint8_t area);

    /**
     * @brief Calculate addresses and sizes of areas (in case no user configuration is given),
     *        or validate user configuration (if given).
     */
    void calc_validate_area_params();

    /**
     * @brief Calculate empty (unprogrammed) continuous space at the end of the area.
     *
     * @param[in]  area                   Area.
     * @param[out] offset                 Offset of empty space.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int calc_empty_space(uint8_t area, uint32_t &offset);

    /**
     * @brief Read an NVStore record from a given location.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset of record in area.
     * @param[in]  buf_size               Buffer size (bytes).
     * @param[in]  buf                    Output Buffer.
     * @param[out] actual_size            Actual data size (bytes).
     * @param[in]  validate_only          Just validate (without reading to buffer).
     * @param[out] valid                  Is the record valid.
     * @param[out] key                    Record key.
     * @param[out] flags                  Record flags.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int read_record(uint8_t area, uint32_t offset, uint16_t buf_size, void *buf,
                    uint16_t &actual_size, int validate_only, int &valid,
                    uint16_t &key, uint16_t &flags, uint32_t &next_offset);

    /**
     * @brief Write an NVStore record from a given location.
     *
     * @param[in]  area                   Area.
     * @param[in]  offset                 Offset of record in area.
     * @param[in]  key                    Record key.
     * @param[in]  flags                  Record flags.
     * @param[in]  data_size              Data size (bytes).
     * @param[in]  data_buf               Data buffer.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int write_record(uint8_t area, uint32_t offset, uint16_t key, uint16_t flags,
                     uint32_t data_size, const void *data_buf, uint32_t &next_offset);

    /**
     * @brief Write a master record of a given area.
     *
     * @param[in]  area                   Area.
     * @param[in]  version                Area version.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int write_master_record(uint8_t area, uint16_t version, uint32_t &next_offset);

    /**
     * @brief Copy a record from one area to the other one.
     *
     * @param[in]  from_area              Area to copy record from.
     * @param[in]  from_offset            Offset in source area.
     * @param[in]  to_offset              Offset in destination area.
     * @param[out] next_offset            Offset of next record.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int copy_record(uint8_t from_area, uint32_t from_offset, uint32_t to_offset,
                    uint32_t &next_offset);

    /**
     * @brief Garbage collection (compact all records from active area to nonactive ones).
     *        All parameters belong to a record that needs to be written before the process.
     *
     * @param[in]  key                    Record key.
     * @param[in]  flags                  Record flags.
     * @param[in]  buf_size               Data size (bytes).
     * @param[in]  buf                    Data buffer.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int garbage_collection(uint16_t key, uint16_t flags, uint16_t buf_size, const void *buf);

    /**
     * @brief Actual logics of get API (covers also get size API).
     *
     * @param[in]  key                    key.
     * @param[in]  buf_size               Buffer size (bytes).
     * @param[in]  buf                    Output Buffer.
     * @param[out] actual_size            Actual data size (bytes).
     * @param[in]  validate_only          Just validate (without reading to buffer).
     *
     * @returns 0 for success, nonzero for failure.
     */
    int do_get(uint16_t key, uint16_t buf_size, void *buf, uint16_t &actual_size,
               int validate_only);

    /**
     * @brief Actual logics of set API (covers also set_once and remove APIs).
     *
     * @param[in]  key                    key.
     * @param[in]  buf_size               Buffer size (bytes).
     * @param[in]  buf                    Input Buffer.
     * @param[in]  flags                  Record flags.
     *
     * @returns 0 for success, nonzero for failure.
     */
    int do_set(uint16_t key, uint16_t buf_size, const void *buf, uint16_t flags);

};
/** @}*/

#endif // NVSTORE_ENABLED

#endif
