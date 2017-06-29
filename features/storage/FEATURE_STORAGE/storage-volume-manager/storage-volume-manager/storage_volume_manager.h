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

#ifndef __STORAGE_VOLUME_MANAGER_H__
#define __STORAGE_VOLUME_MANAGER_H__

#ifndef __cplusplus
#error "This abstraction requires a C++ toolchain"
#endif // __cplusplus

#include "storage_abstraction/Driver_Storage.h"
#include "mbed_toolchain.h"                     /* required for MBED_DEPRECATED_SINCE */

#if !defined(YOTTA_CFG_STORAGE_VOLUME_MANAGER_MAX_VOLUMES)
#define MAX_VOLUMES 4
#else
#define MAX_VOLUMES YOTTA_CFG_STORAGE_VOLUME_MANAGER_MAX_VOLUMES
#endif
/**<
 * A static assert to ensure that the size of SequentialJournal is smaller than
 * FlashJournal_t. The caller will only allocate a FlashJournal_t and expect the
 * Sequential Strategy to reuse that space for a SequentialFlashJournal_t.
 */
#ifndef TOOLCHAIN_IAR
typedef char AssertStorageVolumeManagerMaxVolumesIsSane[(((MAX_VOLUMES) > 0) && ((MAX_VOLUMES) <= 8)) ? 0:-1];
#endif

#define CONCATENATE(A, B) A ## B
#define EXPAND(X) X /* this adds a level of indirection needed to allow macro-expansion following a token-paste operation (see use of CONCATENATE() below). */

#define STORAGE_API_EXTERN_C_DECLARATIONS_FOR_VOLUME(N)                                                               \
    extern "C" ARM_DRIVER_VERSION       GetVersion_ ## N(void);                                                       \
    extern "C" ARM_STORAGE_CAPABILITIES GetCapabilities_ ## N(void);                                                  \
    extern "C" int32_t                  Initialize_ ## N(ARM_Storage_Callback_t callback);                            \
    extern "C" int32_t                  Uninitialize_ ## N(void);                                                     \
    extern "C" int32_t                  PowerControl_ ## N(ARM_POWER_STATE state);                                    \
    extern "C" int32_t                  ReadData_ ## N(uint64_t addr, void *data, uint32_t size);                     \
    extern "C" int32_t                  ProgramData_ ## N(uint64_t addr, const void *data, uint32_t size);            \
    extern "C" int32_t                  Erase_ ## N(uint64_t addr, uint32_t size);                                    \
    extern "C" int32_t                  EraseAll_ ## N(void);                                                         \
    extern "C" ARM_STORAGE_STATUS       GetStatus_ ## N(void);                                                        \
    extern "C" int32_t                  GetInfo_ ## N(ARM_STORAGE_INFO *infoP);                                       \
    extern "C" uint32_t                 ResolveAddress_ ## N(uint64_t addr);                                          \
    extern "C" int32_t                  GetNextBlock_ ## N(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP); \
    extern "C" int32_t                  GetBlock_ ## N(uint64_t addr, ARM_STORAGE_BLOCK *blockP);

#define STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_1 STORAGE_API_EXTERN_C_DECLARATIONS_FOR_VOLUME(0)
#define STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_2 STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_1 STORAGE_API_EXTERN_C_DECLARATIONS_FOR_VOLUME(1)
#define STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_3 STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_2 STORAGE_API_EXTERN_C_DECLARATIONS_FOR_VOLUME(2)
#define STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_4 STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_3 STORAGE_API_EXTERN_C_DECLARATIONS_FOR_VOLUME(3)
/* ... add more of the above if ever needed */

#define STORAGE_API_EXTERN_C_DECLARATIONS_LIST(N) EXPAND(CONCATENATE(STORAGE_API_EXTERN_C_DECLARATIONS_LIST_FOR_, N))

STORAGE_API_EXTERN_C_DECLARATIONS_LIST(MAX_VOLUMES);

/**
 * Error return codes specific to the Storage volume manager. These extend the
 * common error codes from ARM_DRIVER_STORAGE. All Volume-manager APIs return an
 * int32_t to allow for both error and success status returns. This enumeration
 * contains all possible error status values.
 */
typedef enum _StorageVolumeManager_Status
{
    STORAGE_VOLUME_MANAGER_STATUS_ERROR_EXHASTED_VOLUMES     = -7,  ///< exhausted the supply of available volumes
    STORAGE_VOLUME_MANAGER_STATUS_ERROR_NOT_ERASABLE         = -8,  ///< Part (or all) of the range provided to Erase() isn't erasable.
    STORAGE_VOLUME_MANAGER_STATUS_ERROR_NOT_PROGRAMMABLE     = -9,  ///< Part (or all) of the range provided to ProgramData() isn't programmable.
    STORAGE_VOLUME_MANAGER_STATUS_ERROR_PROTECTED            = -10, ///< Part (or all) of the range to Erase() or ProgramData() is protected.
    STORAGE_VOLUME_MANAGER_STATUS_ERROR_NOT_INITIALIZED      = -11, ///< underlying storage not initialized
    STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED = -12, ///< attempt to operate on an unallocated volume
} StorageVolumeManager_Status_t;

typedef void (*InitializeCallback_t)(int32_t status);
class StorageVolumeManager; /* forward declaration */

class StorageVolume {
public:
    MBED_DEPRECATED_SINCE("mbed-os-5.5", "StorageVolume is deprecated. "
                          "Use MBRBlockDevice for volumes instead")
    StorageVolume() : allocated(false) { /* empty */ }

public:
    void setup(uint64_t addr, uint64_t size, StorageVolumeManager *volumeManager);

    /*
     * Mimic the API of ARM_DRIVER_STORAGE
     */
public:
    ARM_DRIVER_VERSION       GetVersion(void);
    ARM_STORAGE_CAPABILITIES GetCapabilities(void);
    int32_t                  Initialize(ARM_Storage_Callback_t callback);
    int32_t                  Uninitialize(void);
    int32_t                  PowerControl(ARM_POWER_STATE state);
    int32_t                  ReadData(uint64_t addr, void *data, uint32_t size);
    int32_t                  ProgramData(uint64_t addr, const void *data, uint32_t size);
    int32_t                  Erase(uint64_t addr, uint32_t size);
    int32_t                  EraseAll(void);
    ARM_STORAGE_STATUS       GetStatus(void);
    int32_t                  GetInfo(ARM_STORAGE_INFO *infoP);
    uint32_t                 ResolveAddress(uint64_t addr);
    int32_t                  GetNextBlock(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP);
    int32_t                  GetBlock(uint64_t addr, ARM_STORAGE_BLOCK *blockP);

public:
    bool isAllocated(void) const {
        return allocated;
    }

    void deallocate(void) {
        allocated = false;
    }

    /*
     * Accessor methods.
     */

    uint64_t getVolumeOffset(void) const {
        return volumeOffset;
    }
    uint64_t getVolumeSize(void) const {
        return volumeSize;
    }
    const ARM_Storage_Callback_t &getCallback(void) const {
        return callback;
    }

private:
    bool overlapsWithBlock(const ARM_STORAGE_BLOCK* blockP) const {
        return (((blockP->addr + blockP->size) <= volumeOffset) || ((volumeOffset + volumeSize) <= blockP->addr)) ? false : true;
    }

    void transformBlockToVolume(ARM_STORAGE_BLOCK *blockP) const {
        if (blockP->addr < volumeOffset) {
            blockP->addr = volumeOffset;
        }
        if ((blockP->addr + blockP->size) > (volumeOffset + volumeSize)) {
            blockP->size = (volumeOffset + volumeSize) - blockP->addr;
        }

        blockP->addr -= volumeOffset;
    }

private:
    bool                    allocated;
    uint64_t                volumeOffset;
    uint64_t                volumeSize;
    ARM_Storage_Callback_t  callback;
    StorageVolumeManager   *volumeManager;
};

class StorageVolumeManager {
public:
    MBED_DEPRECATED_SINCE("mbed-os-5.5", "StorageVolumeManager is deprecated. "
                          "Use MBRBlockDevice to manage volumes instead")
    StorageVolumeManager()  { /* empty */ }
    ~StorageVolumeManager() { /* empty */ }

    /**
     * Initialize the storage MTD and prepare it for operation within the context of the volume manager.
     *
     * @param[in] storage
     *              The underlying MTD.
     * @param[in] callback
     *              A callback to be invoked upon completion of initialization.
     *
     * @return If asynchronous activity is launched, an invocation returns
     *     ARM_DRIVER_OK, and the caller can expect to receive a
     *     callback in the future with a status value of ARM_DRIVER_OK or an error-
     *     code. In the case of synchronous execution, control returns after
     *     completion with a value of 1. Return values less than
     *     ARM_DRIVER_OK (0) signify errors.
     */
    int32_t initialize(ARM_DRIVER_STORAGE *mtd, InitializeCallback_t callback);

    int32_t addVolume(uint64_t addr, uint64_t size, StorageVolume **volumePP);
    int32_t addVolume_C(uint64_t addr, uint64_t size, _ARM_DRIVER_STORAGE *mtd);
    int32_t lookupVolume(uint64_t addr, StorageVolume **volumePP);

    /*
     * Accessor methods.
     */

    bool isInitialized() const {
        return initialized;
    }
    ARM_DRIVER_STORAGE *getStorage(void) const {
        return storage;
    }
    const ARM_STORAGE_INFO &getStorageInfo(void) const {
        return storageInfo;
    }
    const ARM_STORAGE_CAPABILITIES &getStorageCapabilities(void) const {
        return storageCapabilities;
    }
    StorageVolume *volumeAtIndex(size_t index) {
        return &volumes[index];
    }

public:
    static void storageCallback(int32_t status, ARM_STORAGE_OPERATION operation);

private:
    friend int32_t StorageVolume::PowerControl(ARM_POWER_STATE state);
    friend int32_t StorageVolume::ReadData(uint64_t addr, void *data, uint32_t size);
    friend int32_t StorageVolume::ProgramData(uint64_t addr, const void *data, uint32_t size);
    friend int32_t StorageVolume::Erase(uint64_t addr, uint32_t size);
    friend int32_t StorageVolume::EraseAll(void);
    friend ARM_STORAGE_STATUS StorageVolume::GetStatus(void);
    StorageVolume *activeVolume; /* This state-variable is set to point to a volume
                                  * while there is pending activity. It tracks
                                  * the volume which is at the source of the
                                  * activity. Once the activity finishes, this
                                  * variable is reset. Having this variable set
                                  * might be used as an indication that the
                                  * underlying storage is busy. */

#define FRIEND_DECLARATIONS_FOR_VOLUME(N) \
  friend ARM_DRIVER_VERSION       GetVersion_ ## N(void);                                                       \
  friend ARM_STORAGE_CAPABILITIES GetCapabilities_ ## N(void);                                                  \
  friend int32_t                  Initialize_ ## N(ARM_Storage_Callback_t callback);                            \
  friend int32_t                  Uninitialize_ ## N(void);                                                     \
  friend int32_t                  PowerControl_ ## N(ARM_POWER_STATE state);                                    \
  friend int32_t                  ReadData_ ## N(uint64_t addr, void *data, uint32_t size);                     \
  friend int32_t                  ProgramData_ ## N(uint64_t addr, const void *data, uint32_t size);            \
  friend int32_t                  Erase_ ## N(uint64_t addr, uint32_t size);                                    \
  friend int32_t                  EraseAll_ ## N(void);                                                         \
  friend ARM_STORAGE_STATUS       GetStatus_ ## N(void);                                                        \
  friend int32_t                  GetInfo_ ## N(ARM_STORAGE_INFO *infoP);                                       \
  friend uint32_t                 ResolveAddress_ ## N(uint64_t addr);                                          \
  friend int32_t                  GetNextBlock_ ## N(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP); \
  friend int32_t                  GetBlock_ ## N(uint64_t addr, ARM_STORAGE_BLOCK *blockP);

#define FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_1 FRIEND_DECLARATIONS_FOR_VOLUME(0)
#define FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_2 FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_1 FRIEND_DECLARATIONS_FOR_VOLUME(1)
#define FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_3 FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_2 FRIEND_DECLARATIONS_FOR_VOLUME(2)
#define FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_4 FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_3 FRIEND_DECLARATIONS_FOR_VOLUME(3)
/* ... add more of the above if ever needed */

#define FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES(N) EXPAND(CONCATENATE(FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES_FOR_, N))

    //todo: remove FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES(MAX_VOLUMES);
    FRIEND_DECLARATIONS_FOR_STORAGE_API_INSTANCES(MAX_VOLUMES)

private:
    size_t findIndexOfUnusedVolume(void) const;

private:
    bool                      initialized;
    ARM_DRIVER_STORAGE       *storage;
    ARM_STORAGE_INFO          storageInfo;
    ARM_STORAGE_CAPABILITIES  storageCapabilities;
    StorageVolume             volumes[MAX_VOLUMES];
};

#endif /* __STORAGE_VOLUME_MANAGER_H__ */
