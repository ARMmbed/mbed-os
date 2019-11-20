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
#ifndef _KV_MAP
#define _KV_MAP

#include "features/storage/kvstore/include/KVStore.h"
#include "platform/PlatformMutex.h"
#include "platform/SingletonPtr.h"
#include "features/storage/blockdevice/BlockDevice.h"
#include "features/storage/filesystem/FileSystem.h"

namespace mbed {

#define  MAX_ATTACHED_KVS 3

/**
 * This structure represent a KVStore partition configuration
 */
typedef struct {
    /**
     * A Pointer to main instance of the KVStore partition.
     * This is also the instance KVStore global API should work with.
     * This must not be NULL in a working partition configuration.
     */
    KVStore *kvstore_main_instance;
    /**
     * A pointer Internal store of the KVStore partition.
     * If no rollback protection is required the pointer may be NULL.
     */
    KVStore *internal_store;
    /**
     * A pointer external store of the KVStore partition.
     * The pointer can be NULL if external store has been omitted
     */
    KVStore *external_store;
    /**
     * A pointer Internal FlashIAP BlockDevice of the KVStore partition.
     * The pointer can be NULL if internal store has been omitted
     */
    BlockDevice *internal_bd;
    /**
     * A pointer external BlockDevice of the KVStore partition.
     * The pointer can be NULL if external store has been omitted
     */
    BlockDevice *external_bd;
    /**
     * A pointer external FileSystem of the KVStore partition.
     * The pointer can be NULL if FileSystemStore has not been configured.
     */
    FileSystem *external_fs;
    /**
     * This is a flag masking value for the KVStore global API.
     * The Global API will mask the input flags base on this value to
     * prevent errors in case the user choose an different security level.
     */
    uint32_t flags_mask;
} kvstore_config_t;

/**
 * This structure maps between a string name and a partition configuration.
 */
typedef struct {
    /**
      * Partition name string
      */
    char *partition_name;
    /**
     * Configuration struct.
     */
    kvstore_config_t *kv_config;
} kv_map_entry_t;

/** KVMap class
 *
 *  Singleton class to manage the mapping of KVStore partition and its naming.
 */
class KVMap : private mbed::NonCopyable<KVMap> {
public:

    /**
     * @brief As a singleton, return the single instance of the class.
     *        This class is a singleton for the following reasons:
     *        - Ease of use, so you don't have to coordinate instantiations.
     *        - Lazy instantiation of internal data, (which we can't achieve with simple static classes).
     *
     * @returns Singleton instance reference.
     */
    static KVMap &get_instance()
    {
        // Use this implementation of singleton (Meyer's) rather than the one that allocates
        // the instance on the heap because it ensures destruction at program end (preventing warnings
        // from memory checking tools, such as valgrind).
        static KVMap instance;
        return instance;
    }

    ~KVMap();

    /**
     * @brief Initializes KVMap
     *
     * @return 0 on success, negative error code on failure
     */
    int init();

    /**
     * @brief Attach a KVStore partition configuration, and add it to the KVMap array
     *
     * @param partition_name String parameter contains the partition name.
     * @param kv_config A configuration struct created by the kv_config or by the user.
     * @return 0 on success, negative error code on failure
     */
    int attach(const char *partition_name, kvstore_config_t *kv_config);

    /**
     * @brief Detach a KVStore partition configuration from the KVMap array,
     *        and deinitialize its components
     *
     * @param partition_name String parameter contains the partition name.
     * @return 0 on success, negative error code on failure
     */
    int detach(const char *partition_name);

    /**
     * @brief Deinitialize the KVMap array, and deinitialize all the attached partitions.
     *
     * @return 0 on success, negative error code on failure
     */
    int deinit();

    /**
     * @brief Full name lookup, and then break it into KVStore instance and key
     *
     * @param[in] full_name  String parameter contains the partition name to look for.
     *                   The String should be formated as follow "/partition name/key". The key is optional.
     * @param[out] kv_instance Returns the main KVStore instance associated with the required partition name.
     * @param[out] key_index Returns an index to the first character of the key.
     * @param[out] flags_mask Return the flag masking for the current configuration
     * @return 0 on success, negative error code on failure
     */
    int lookup(const char *full_name, mbed::KVStore **kv_instance, size_t *key_index, uint32_t *flags_mask = NULL);

    /**
     * @brief Getter for the internal KVStore instance.
     *
     * @param name String parameter contains the /partition name/.
     *
     * @return Pointer to the internal kvstore on success,
     *         NULL on failure or if not exist
     */
    KVStore *get_internal_kv_instance(const char *name);
    /**
     * @brief Getter for the external KVStore instance.
     *
     * @param name String parameter contains the /partition name/.
     *
     * @return Pointer to the external kvstore on success,
     *         NULL on failure or if not exist
     */
    KVStore *get_external_kv_instance(const char *name);
    /**
     * @brief Getter for the main KVStore instance.
     *
     * @param name String parameter contains the /partition name/.
     *
     * @return Pointer to the main kvstore on success,
     *         NULL on failure or if not exist
     */
    KVStore *get_main_kv_instance(const char *name);
    /**
     * @brief Getter for the internal BlockDevice instance.
     *
     * @param name String parameter contains the /partition name/.
     *
     * @return Pointer to the internal BlockDevice on success,
     *         NULL on failure or if not exist
     */
    BlockDevice *get_internal_blockdevice_instance(const char *name);
    /**
     * @brief Getter for the external BlockDevice instance.
     *
     * @param name String parameter contains the /partition name/.
     *
     * @return Pointer to the external BlockDevice on success,
     *         NULL on failure or if not exist
     */
    BlockDevice *get_external_blockdevice_instance(const char *name);
    /**
     * @brief Getter for the external FileSystem instance.
     *
     * @param name String parameter contains the /partition name/.
     *
     * @return Pointer to the external FileSystem on success,
     *         NULL on failure or if not exist
     */
    FileSystem *get_external_filesystem_instance(const char *name);

#if !defined(DOXYGEN_ONLY)
private:

    /**
     * @brief Deinitialize all components of a partition configuration struct.
     *
     * @param partition  Partition configuration struct.
     */
    void deinit_partition(kv_map_entry_t *partition);

    /**
     * @brief Full name lookup, and then break it into KVStore config and key
     *
     * @param[in] full_name  String parameter contains the /partition name/key.
     * @param[out] kv_config Returns The configuration struct associated with the partition name
     * @param[out] key_index Returns an index to the first character of the key.
     * @return 0 on success, negative error code on failure
     */
    int config_lookup(const char *full_name, kvstore_config_t **kv_config, size_t *key_index);

    // Attachment table
    kv_map_entry_t _kv_map_table[MAX_ATTACHED_KVS];
    int _kv_num_attached_kvs;
    int _is_initialized;
    SingletonPtr<PlatformMutex> _mutex;
#endif
};
}
#endif
