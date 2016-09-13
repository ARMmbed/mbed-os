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

#include "storage-volume-manager/storage_volume_manager.h"
#include <string.h>
#include <inttypes.h>

/* redefine these macros to a printf() equivalent to emit trace */
#define tr_debug(...)
#define tr_error(...)

/*
 * The following variable records the volume-manager instance which last setup
 * StorageVolumeManager::storageCallback() as a callback for a storage driver.
 * We use this value in the callback handler to de-multiplex the callback into a
 * volume-manager.
 */
static StorageVolumeManager *activeVolumeManager;

InitializeCallback_t initializeCallback;

#define STORAGE_API_DEFINITIONS_FOR_VOLUME(N)                                          \
extern "C" ARM_DRIVER_VERSION GetVersion_ ## N(void) {                                 \
    return activeVolumeManager->volumes[(N)].GetVersion();                             \
}                                                                                      \
ARM_STORAGE_CAPABILITIES GetCapabilities_ ## N(void) {                                 \
    return activeVolumeManager->volumes[(N)].GetCapabilities();                        \
}                                                                                      \
int32_t Initialize_ ## N(ARM_Storage_Callback_t callback) {                            \
    return activeVolumeManager->volumes[(N)].Initialize(callback);                     \
}                                                                                      \
int32_t Uninitialize_ ## N(void) {                                                     \
    return activeVolumeManager->volumes[(N)].Uninitialize();                           \
}                                                                                      \
int32_t PowerControl_ ## N(ARM_POWER_STATE state) {                                    \
    return activeVolumeManager->volumes[(N)].PowerControl(state);                      \
}                                                                                      \
int32_t ReadData_ ## N(uint64_t addr, void *data, uint32_t size) {                     \
    return activeVolumeManager->volumes[(N)].ReadData(addr, data, size);               \
}                                                                                      \
int32_t ProgramData_ ## N(uint64_t addr, const void *data, uint32_t size) {            \
    return activeVolumeManager->volumes[(N)].ProgramData(addr, data, size);            \
}                                                                                      \
int32_t Erase_ ## N(uint64_t addr, uint32_t size) {                                    \
    return activeVolumeManager->volumes[(N)].Erase(addr, size);                        \
}                                                                                      \
int32_t EraseAll_ ## N(void) {                                                         \
    return activeVolumeManager->volumes[(N)].EraseAll();                               \
}                                                                                      \
ARM_STORAGE_STATUS GetStatus_ ## N(void) {                                             \
    return activeVolumeManager->volumes[(N)].GetStatus();                              \
}                                                                                      \
int32_t GetInfo_ ## N(ARM_STORAGE_INFO *infoP) {                                       \
    return activeVolumeManager->volumes[(N)].GetInfo(infoP);                           \
}                                                                                      \
uint32_t ResolveAddress_ ## N(uint64_t addr) {                                         \
    return activeVolumeManager->volumes[(N)].ResolveAddress(addr);                     \
}                                                                                      \
int32_t GetNextBlock_ ## N(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP) { \
    return activeVolumeManager->volumes[(N)].GetNextBlock(prevP, nextP);               \
}                                                                                      \
int32_t GetBlock_ ## N(uint64_t addr, ARM_STORAGE_BLOCK *blockP) {                     \
    return activeVolumeManager->volumes[(N)].GetBlock(addr, blockP);                   \
}                                                                                      \
ARM_DRIVER_STORAGE VIRTUAL_MTD_ ## N = {                                               \
    .GetVersion      = GetVersion_ ## N,                                               \
    .GetCapabilities = GetCapabilities_ ## N,                                          \
    .Initialize      = Initialize_ ## N,                                               \
    .Uninitialize    = Uninitialize_ ## N,                                             \
    .PowerControl    = PowerControl_ ## N,                                             \
    .ReadData        = ReadData_ ## N,                                                 \
    .ProgramData     = ProgramData_ ## N,                                              \
    .Erase           = Erase_ ## N,                                                    \
    .EraseAll        = EraseAll_ ## N,                                                 \
    .GetStatus       = GetStatus_ ## N,                                                \
    .GetInfo         = GetInfo_ ## N,                                                  \
    .ResolveAddress  = ResolveAddress_ ## N,                                           \
    .GetNextBlock    = GetNextBlock_ ## N,                                             \
    .GetBlock        = GetBlock_ ## N,                                                 \
};

#define STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_1 STORAGE_API_DEFINITIONS_FOR_VOLUME(0)
#define STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_2 STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_1 STORAGE_API_DEFINITIONS_FOR_VOLUME(1)
#define STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_3 STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_2 STORAGE_API_DEFINITIONS_FOR_VOLUME(2)
#define STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_4 STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_3 STORAGE_API_DEFINITIONS_FOR_VOLUME(3)
/* ... add more of the above if ever needed */

#define STORAGE_API_DEFINITIONS_FOR_VOLUMES(N) EXPAND(CONCATENATE(STORAGE_API_DEFINITIONS_LIST_FOR_VOLUME_, N))

STORAGE_API_DEFINITIONS_FOR_VOLUMES(MAX_VOLUMES);

int32_t StorageVolumeManager::initialize(ARM_DRIVER_STORAGE *mtd, InitializeCallback_t callback)
{
    activeVolume        = NULL;
    initializeCallback  = callback;

    storage             = mtd;
    storageCapabilities = mtd->GetCapabilities();

    int32_t rc = mtd->GetInfo(&storageInfo);
    if (rc != ARM_DRIVER_OK) {
        tr_error("StorageVolumeManager::initialize: call to GetInfo() failed with %" PRId32, rc);
        return ARM_DRIVER_ERROR;
    }

    rc = mtd->Initialize(storageCallback);
    if (rc < ARM_DRIVER_OK) {
        tr_error("Initialize() failed with error %" PRId32, rc);
        return ARM_DRIVER_ERROR;
    }

    activeVolumeManager = this;
    if (rc == ARM_DRIVER_OK) {
        /* there is pending asynchronous activity which will result in a callback later */
        return ARM_DRIVER_OK;
    }

    /* Clear previously allocated volumes */
    for (size_t i = 0; i < MAX_VOLUMES; i++) {
        if (volumes[i].isAllocated()) {
            volumes[i].deallocate();
        }
    }

    /* synchronous completion */
    initialized = true;
    return 1;
}

int32_t StorageVolumeManager::addVolume(uint64_t addr, uint64_t size, StorageVolume **volumePP)
{
    tr_debug("StorageVolumeManager_addVolume: addr = %" PRIu32 ", size = %" PRIu32, (uint32_t)addr, (uint32_t)size);

    *volumePP = NULL;

    /*
     * sanity checks for arguments
     */
    ARM_STORAGE_INFO info;
    int32_t rc;
    rc = storage->GetInfo(&info);
    if (rc != ARM_DRIVER_OK) {
        tr_error("StorageVolumeManager_addVolume: storage->GetInfo() failed with %" PRId32, rc);
        return ARM_DRIVER_ERROR;
    }
    if (size > info.total_storage) {
        tr_error("StorageVolumeManager_addVolume: 'size' parameter too large: %" PRIu32, (uint32_t)size);
        return ARM_DRIVER_ERROR;
    }
    ARM_STORAGE_BLOCK firstBlock;
    rc = storage->GetNextBlock(NULL, &firstBlock);
    if (rc != ARM_DRIVER_OK) {
        tr_error("StorageVolumeManager_addVolume: storage->GetNextBlock() failed with %" PRId32, rc);
        return ARM_DRIVER_ERROR;
    }
    if ((addr < firstBlock.addr) || ((addr + size) > (firstBlock.addr + info.total_storage))) {
        tr_error("StorageVolumeManager_addVolume: given range [%" PRIu32 ", %" PRIu32 ") isn't entirely contained within available storage range [%" PRIu32 ", %" PRIu32 ")",
            (uint32_t)addr, (uint32_t)(addr + size), (uint32_t)firstBlock.addr, (uint32_t)(firstBlock.addr + info.total_storage));
        return ARM_DRIVER_ERROR;
    }

    /* Find an unused volume. */
    uint32_t index = findIndexOfUnusedVolume();
    if (index == MAX_VOLUMES) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_EXHASTED_VOLUMES;
    }

    /* setup volume */
    StorageVolume *volumeP = &volumes[index];
    volumeP->setup(addr, size, this);
    *volumePP = volumeP;
    tr_debug("StorageVolumeManager_addVolume: setup volume at index %" PRIu32, index);

    return ARM_DRIVER_OK;
}

int32_t StorageVolumeManager::addVolume_C(uint64_t addr, uint64_t size, _ARM_DRIVER_STORAGE *mtd)
{
    int32_t rc;
    StorageVolume *volumeP;
    if ((rc = addVolume(addr, size, &volumeP)) < ARM_DRIVER_OK) {
        return rc;
    }

    /* locate index of the allocated volume */
    size_t index;
    for (index = 0; index < MAX_VOLUMES; index++) {
        if (volumes[index].isAllocated() && (&volumes[index] == volumeP)) {
            break;
        }
    }
    if (index == MAX_VOLUMES) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_EXHASTED_VOLUMES;
    }

    if (index == 0) {
        *mtd = VIRTUAL_MTD_0;
    } else if (index == 1) {
        *mtd = VIRTUAL_MTD_1;
    } else if (index == 2) {
        *mtd = VIRTUAL_MTD_2;
    } else if (index == 3) {
        *mtd = VIRTUAL_MTD_3;
    } else {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

int32_t StorageVolumeManager::lookupVolume(uint64_t addr, StorageVolume **volumePP)
{
    /*
     * Traverse the volumes in reverse order of creation; this allows newly created volumes to supersede the older ones.
     */
    for (size_t index = MAX_VOLUMES - 1; index > 0; --index) {
        StorageVolume *volume = &volumes[index];
        if ((addr >= volume->getVolumeOffset()) && (addr < (volume->getVolumeOffset() + volume->getVolumeSize()))) {
            *volumePP = volume;
            return ARM_DRIVER_OK;
        }
    }

    return ARM_DRIVER_ERROR;
}

void StorageVolumeManager::storageCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    tr_debug("StorageVolumeManager_callback: operation = %u", operation);
    StorageVolumeManager *volumeManager = activeVolumeManager;

    switch (operation) {
        case ARM_STORAGE_OPERATION_INITIALIZE:
            volumeManager->initialized = true;
            if (initializeCallback != NULL) {
                initializeCallback(status);
            }
            break;

        case ARM_STORAGE_OPERATION_POWER_CONTROL:
        case ARM_STORAGE_OPERATION_READ_DATA:
        case ARM_STORAGE_OPERATION_PROGRAM_DATA:
        case ARM_STORAGE_OPERATION_ERASE:
        case ARM_STORAGE_OPERATION_ERASE_ALL:
            if (volumeManager->activeVolume != NULL) {
                /* Reset activeVolume and invoke callback. We reset activeVolume before the
                 * callback because the callback may attempt to launch another asynchronous
                 * operation, which requires 'activeVolume' to be NULL. */
                StorageVolume *callbackVolume = volumeManager->activeVolume; /* remember the volume which will receive the callback. */
                volumeManager->activeVolume   = NULL;

                if (callbackVolume->isAllocated() && callbackVolume->getCallback()) {
                    (callbackVolume->getCallback())(status, operation);
                }
            }
            break;

        default:
            tr_error("StorageVolumeManager_callback: unknown operation %u", operation);
            break;
    }
}

size_t StorageVolumeManager::findIndexOfUnusedVolume(void) const {
    size_t index;
    for (index = 0; index < MAX_VOLUMES; index++) {
        if (!volumes[index].isAllocated()) {
            break;
        }
    }
    return index;
}
