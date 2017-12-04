/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 */

// This file is deprecated so deprecation warnings when building it are silenced
#if   defined ( __CC_ARM )
#pragma diag_suppress 1361  // Deprecated declaration
#elif defined ( __GNUC__ )
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdint.h>
#include <Driver_Common.h>
#include "storage_volume_manager.h"
#include "cfstore_config.h"
#include "cfstore_debug.h"
#include "cfstore_svm.h"

/** @file   cfstore_svm.cpp
 *
 * This module is provides a C wrapper to the C++ storage-volume-manager.h API,
 * so it can be called by the C-HAL implementation configuration_store.c
 */

#ifndef CFSTORE_SVM_VOL_01_START_OFFSET
#define CFSTORE_SVM_VOL_01_START_OFFSET       0x80000UL
#endif

#ifndef CFSTORE_SVM_VOL_01_SIZE
#define CFSTORE_SVM_VOL_01_SIZE               0x80000UL
#endif

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;
static ARM_DRIVER_STORAGE *cfstore_svm_storage_drv = &ARM_Driver_Storage_MTD_K64F;

/* the storage volume manager instance used to generate virtual mtd descriptors */
StorageVolumeManager volumeManager;

/* used only for the initialization of the volume-manager. */
static void cfstore_svm_volume_manager_initialize_callback(int32_t status)
{
    CFSTORE_FENTRYLOG("%s: with status %d" , __func__, (int) status);
}

static void cfstore_svm_journal_mtc_callback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    CFSTORE_FENTRYLOG("%s: operation %d with status %d" , __func__, (int) operation, (int) status);
}

int32_t cfstore_svm_init(struct _ARM_DRIVER_STORAGE *storage_mtd)
{
    int32_t ret = ARM_DRIVER_OK;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    ret = volumeManager.initialize(cfstore_svm_storage_drv, cfstore_svm_volume_manager_initialize_callback);
    if(ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:debug: volume-manager::initialize() failed for storage_mtd=%p (ret=%d)", __func__, storage_mtd, (int) ret);
        return ret;
    }
    ret = volumeManager.addVolume_C(CFSTORE_SVM_VOL_01_START_OFFSET, CFSTORE_SVM_VOL_01_SIZE, storage_mtd);
    if(ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:debug: volume-manager::addVolume_C() failed for storage_mtd=%p (ret=%d)", __func__, storage_mtd, (int) ret);
        return ret;
    }
    ret = storage_mtd->Initialize(cfstore_svm_journal_mtc_callback);
    if(ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:debug: storage_mtd->initialize() failed for storage_mtd=%p (ret=%d)", __func__, storage_mtd, (int) ret);
        return ret;
    }
    return ret;
}

#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

