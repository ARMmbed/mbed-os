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

#include "KVStore.h"
#include "platform/PlatformMutex.h"
#include "platform/SingletonPtr.h"
#include "BlockDevice.h"
#include "FileSystem.h"

namespace mbed {

#define  MAX_ATTACHED_KVS 3

typedef struct {
    KVStore *kvstore_main_instance;
    KVStore *internal_store;
    KVStore *external_store;
    BlockDevice *internal_bd;
    BlockDevice *external_bd;
    FileSystem *external_fs;
    uint32_t flags_mask;
} kvstore_config_t;

typedef struct {
    char *partition_name;
    kvstore_config_t *kv_config;
} kv_map_entry_t;

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
     * @brief attach a KVStore partition configuration, and add it to the KVMap array
     *
     * @param partition_name string parameter contains the partition name.
     * @param kv_config a configuration struct created by the kv_config or by the user.
     * @return 0 on success, negative error code on failure
     */
    int attach(const char *partition_name, kvstore_config_t *kv_config);

    /**
     * @brief detach a KVStore partition configuration from the KVMap array,
     *        and deinitialize its components
     *
     * @param partition_name string parameter contains the partition name.
     * @return 0 on success, negative error code on failure
     */
    int detach(const char *partition_name);

    /**
     * @brief deinitialize the KVMap array, and deinitialize all the attached partitions.
     *
     * @return 0 on success, negative error code on failure
     */
    int deinit();

    /**
     * @brief full name lookup, and then break it into KVStore instance and key
     *
     * @param full_name  string parameter contains the /partition name/key.
     * @param kv_instance the main KVStore instance associated with the required partition name.
     * @param key_index an index to the first character of the key.
     * @param flags_mask return the flag masking for the current configuration
     * @return 0 on success, negative error code on failure
     */
    int lookup(const char *full_name, mbed::KVStore **kv_instance, size_t *key_index, uint32_t *flags_mask = NULL);

    /**
     * @brief full name lookup, and then break it into KVStore instance and key
     *
     * @param name string parameter contains the /partition name/.
     *
     * @return pointer to the internal kvstore on success,
     *         NULL on failure or if not exist
     */
    KVStore *get_internal_kv_instance(const char *name);
    /**
     * @brief full name lookup, and then break it into KVStore instance and key
     *
     * @param name string parameter contains the /partition name/.
     *
     * @return pointer to the external kvstore on success,
     *         NULL on failure or if not exist
     */
    KVStore *get_external_kv_instance(const char *name);
    /**
     * @brief full name lookup, and then break it into KVStore instance and key
     *
     * @param name string parameter contains the /partition name/.
     *
     * @return pointer to the main kvstore on success,
     *         NULL on failure or if not exist
     */
    KVStore *get_main_kv_instance(const char *name);
    /**
     * @brief full name lookup, and then break it into KVStore instance and key
     *
     * @param name string parameter contains the /partition name/.
     *
     * @return pointer to the internal BlockDevice on success,
     *         NULL on failure or if not exist
     */
    BlockDevice *get_internal_blockdevice_instance(const char *name);
    /**
     * @brief full name lookup, and then break it into KVStore instance and key
     *
     * @param name string parameter contains the /partition name/.
     *
     * @return pointer to the external BlockDevice on success,
     *         NULL on failure or if not exist
     */
    BlockDevice *get_external_blockdevice_instance(const char *name);
    /**
     * @brief full name lookup, and then break it into KVStore instance and key or if not exist
     *
     * @param name string parameter contains the /partition name/.
     *
     * @return pointer to the external FileSystem on success,
     *         NULL on failure or if not exist
     */
    FileSystem *get_external_filesystem_instance(const char *name);

#if !defined(DOXYGEN_ONLY)
private:

    /**
     * @brief deinitialize  all components of a partition configuration struct.
     *
     * @param partition  partition configuration struct.
     */
    void deinit_partition(kv_map_entry_t *partition);

    /**
     * @brief full name lookup, and then break it into KVStore config and key
     *
     * @param full_name  string parameter contains the /partition name/key.
     * @param kv_config the configuration struct associated with the partition name
     * @param key_index an index to the first character of the key.
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
