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

/* redefine tr_debug() to a printf() equivalent to emit trace */
#define tr_debug(...)


void StorageVolume::setup(uint64_t _addr, uint64_t _size, StorageVolumeManager *_volumeManager)
{
    volumeOffset  = _addr;
    volumeSize    = _size;
    volumeManager = _volumeManager;
    allocated     = true;
}

ARM_DRIVER_VERSION StorageVolume::GetVersion(void)
{
    ARM_DRIVER_VERSION bad_ver = {0, 0};

    if (!allocated) {
        return bad_ver;
    }
    return volumeManager->getStorage()->GetVersion();
}

ARM_STORAGE_CAPABILITIES StorageVolume::GetCapabilities(void)
{
    ARM_STORAGE_CAPABILITIES bad_cap;

    if (!allocated) {
        memset(&bad_cap, 0, sizeof(ARM_STORAGE_CAPABILITIES));
        return bad_cap;
    }
    return volumeManager->getStorage()->GetCapabilities();
}

int32_t StorageVolume::Initialize(ARM_Storage_Callback_t _callback)
{
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }

    callback = _callback; /* nothing else to do since we've already initialized the storage */
    return 1; /* synchronous completion. */
}

int32_t StorageVolume::Uninitialize(void)
{
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }

    return 1; /* synchronous completion. */
}

int32_t StorageVolume::PowerControl(ARM_POWER_STATE state)
{
    tr_debug("called powerControl(%u)", state);
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }
    if (volumeManager->activeVolume != NULL) {
        return ARM_DRIVER_ERROR_BUSY;
    }

    volumeManager->activeVolume = this;
    int32_t rc = volumeManager->getStorage()->PowerControl(state);
    if (rc != ARM_DRIVER_OK) {
        volumeManager->activeVolume = NULL; /* we're certain that there is no more pending asynch. activity */
    }
    return rc;
}

int32_t StorageVolume::ReadData(uint64_t addr, void *data, uint32_t size)
{
    tr_debug("called ReadData(%" PRIu32 ", %" PRIu32 ")", (uint32_t)addr, size);
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }
    if (volumeManager->activeVolume != NULL) {
        return ARM_DRIVER_ERROR_BUSY;
    }
    if ((size > volumeSize) || ((addr + size) > volumeSize)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    volumeManager->activeVolume = this;
    int32_t rc = volumeManager->getStorage()->ReadData(volumeOffset + addr, data, size);
    if (rc != ARM_DRIVER_OK) {
        volumeManager->activeVolume = NULL; /* we're certain that there is no more pending asynch. activity */
    }
    return rc;
}

int32_t StorageVolume::ProgramData(uint64_t addr, const void *data, uint32_t size)
{
    tr_debug("called ProgramData(%" PRIu32 ", %" PRIu32 ")", (uint32_t)addr, size);
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }
    if (volumeManager->activeVolume != NULL) {
        return ARM_DRIVER_ERROR_BUSY;
    }
    if ((size > volumeSize) || ((addr + size) > volumeSize)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    volumeManager->activeVolume = this;
    int32_t rc = volumeManager->getStorage()->ProgramData(volumeOffset + addr, data, size);
    if (rc != ARM_DRIVER_OK) {
        volumeManager->activeVolume = NULL; /* we're certain that there is no more pending asynch. activity */
    }
    return rc;
}

int32_t StorageVolume::Erase(uint64_t addr, uint32_t size)
{
    tr_debug("called erase(%" PRIu32 ", %" PRIu32 ")", (uint32_t)addr, size);
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }
    if (volumeManager->activeVolume != NULL) {
        return ARM_DRIVER_ERROR_BUSY;
    }
    if ((size > volumeSize) || ((addr + size) > volumeSize)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    volumeManager->activeVolume = this;
    int32_t rc = volumeManager->getStorage()->Erase(volumeOffset + addr, size);
    if (rc != ARM_DRIVER_OK) {
        volumeManager->activeVolume = NULL; /* we're certain that there is no more pending asynch. activity */
    }
    return rc;
}

int32_t StorageVolume::EraseAll(void)
{
    tr_debug("called eraseAll");
    if (!allocated) {
        return STORAGE_VOLUME_MANAGER_STATUS_ERROR_VOLUME_NOT_ALLOCATED;
    }
    if (volumeManager->activeVolume != NULL) {
        return ARM_DRIVER_ERROR_BUSY;
    }

    int32_t rc;

    /* Allow EraseAll() only if the volume spans the entire storage. */
    {
        ARM_STORAGE_INFO info;
        rc = volumeManager->getStorage()->GetInfo(&info);
        if (rc != ARM_DRIVER_OK) {
            return ARM_DRIVER_ERROR;
        }

        if ((volumeOffset != 0) || (volumeSize != info.total_storage)) {
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
    }

    volumeManager->activeVolume = this;
    rc = volumeManager->getStorage()->EraseAll();
    if (rc != ARM_DRIVER_OK) {
        volumeManager->activeVolume = NULL; /* we're certain that there is no more pending asynch. activity */
    }
    return rc;
}

ARM_STORAGE_STATUS StorageVolume::GetStatus(void)
{
    const uint32_t busy = ((volumeManager->activeVolume != NULL) ? (uint32_t)1 : (uint32_t)0);
    ARM_STORAGE_STATUS status = {0, 0};
    status.busy = busy;
    return status;
}

int32_t StorageVolume::GetInfo(ARM_STORAGE_INFO *infoP)
{
    int32_t rc;
    rc = volumeManager->getStorage()->GetInfo(infoP);
    if (rc != ARM_DRIVER_OK) {
        return ARM_DRIVER_ERROR;
    }

    infoP->total_storage = volumeSize;
    return ARM_DRIVER_OK;
}

uint32_t StorageVolume::ResolveAddress(uint64_t addr) {
    return (uint32_t)(volumeOffset + addr);
}

int32_t StorageVolume::GetNextBlock(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP)
{
    int32_t rc;
    ARM_STORAGE_BLOCK tmpBlock;
    do {
        /* iterate forward */
        rc = volumeManager->getStorage()->GetNextBlock(prevP, &tmpBlock);
        if (rc != ARM_DRIVER_OK) {
            return rc;
        }

        /* Stop iteration if we have progressed past the boundary of this volume. */
        if (tmpBlock.addr >= (volumeOffset + volumeSize)) {
            return ARM_DRIVER_ERROR;
        }
    } while (!this->overlapsWithBlock(&tmpBlock));

    if (nextP) {
        memcpy(nextP, &tmpBlock, sizeof(ARM_STORAGE_BLOCK));
        transformBlockToVolume(nextP);
    }
    return ARM_DRIVER_OK;
}

int32_t StorageVolume::GetBlock(uint64_t addr, ARM_STORAGE_BLOCK *blockP)
{
    ARM_STORAGE_BLOCK tmpBlock;
    int32_t rc = volumeManager->getStorage()->GetBlock(ResolveAddress(addr), &tmpBlock);
    if (rc != ARM_DRIVER_OK) {
        return rc;
    }
    if (!this->overlapsWithBlock(&tmpBlock)) {
        return ARM_DRIVER_ERROR;
    }

    if (blockP) {
        memcpy(blockP, &tmpBlock, sizeof(ARM_STORAGE_BLOCK));
        transformBlockToVolume(blockP);
    }
    return ARM_DRIVER_OK;
}
