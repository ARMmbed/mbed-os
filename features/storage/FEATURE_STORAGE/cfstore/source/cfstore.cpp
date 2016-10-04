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

/** @file  cfstore.cpp
 *
 * C++ Interface to Configuration Store Implementation.
 *
 */


#include "cfstore_config.h"
#include "cfstore_debug.h"
#include "cfstore_os.h"
#include "cfstore.h"
#include <cstdio>
#include <stdio.h>
#include <stdarg.h>

namespace cfstore
{

#define CFSTORE_SEMAPHORE_SLOTS  0


int32_t Cfstore::initialised_count = 0;
cfstore_driver_t *Cfstore::driver;
cfstore_capabilities_t Cfstore::caps;
cfstore_driver_version_t Cfstore::version;
Mutex Cfstore::mtx_lock;
int32_t Cfstore::key_count = 0;
static Mutex cfstore_log_lock;

void cfstore_debug_log(const char* fmt, ...)
{
#ifdef CFSTORE_DEBUG
    va_list args;

    va_start (args, fmt);
    cfstore_log_lock.lock();
    vprintf (fmt, args);
    cfstore_log_lock.unlock();
    va_end (args);
#else
    (void) fmt;
#endif /* CFSTORE_DEBUG */
}

void cfstore_error_log(const char* fmt, ...)
{
#ifdef CFSTORE_DEBUG
    va_list args;

    va_start (args, fmt);
    cfstore_log_lock.lock();
    vprintf (fmt, args);
    cfstore_log_lock.unlock();
    va_end (args);
#else
    (void) fmt;
#endif /* CFSTORE_DEBUG */
}

Cfstore::Cfstore(void)
{
    cfstore_debug_log("%s:entered:\n", __func__);
    driver = &cfstore_os_mbed_driver;
    caps = driver->GetCapabilities();
    version = driver->GetVersion();
}

Cfstore::~Cfstore(void)
{
    cfstore_debug_log("%s:entered:\n", __func__);
}

/** @brief Initialize the configuration store */
int32_t Cfstore::initialize(void)
{
    int32_t ret = ARM_DRIVER_ERROR;

    cfstore_debug_log("%s:entered:\n", __func__);
    mtx_lock.lock();
    if (initialised_count == 0) {
        initialised_count++;
        mtx_lock.unlock();

        ret = cfstore_os_init();
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: CFSTORE os platform initialisation failed (%d)", __func__, (int) ret);
            return ret;
        }

        /* The underlying storage driver is operating in synchronous mode. */
        ret = driver->Initialize(NULL, NULL);
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: Initialize() failed (ret=%d)", __func__, (int) ret);
            return ret;
        }
        ret = driver->PowerControl(ARM_POWER_FULL);
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: PowerControl() failed (ret=%d)", __func__, (int) ret);
            driver->Uninitialize();
            return ret;
        }
    } else {
        mtx_lock.unlock();
        ret = ARM_DRIVER_OK;
    }

    return ret;
}

/** @brief Uninitialize the configuration store */
int32_t Cfstore::uninitialize(void)
{
    int32_t ret = ARM_DRIVER_ERROR;

    cfstore_debug_log("%s:entered:\n", __func__);
    mtx_lock.lock();
    if (--initialised_count == 0) {
        if (key_count > 0) {
            cfstore_error_log("%s:Warning: All Cfstore() created CfstoreKey() instances have not been deleted (possible memory leak) (key_count=%d)", __func__, (int) key_count);
        }
        mtx_lock.unlock();

        ret = driver->PowerControl(ARM_POWER_OFF);
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: PowerControl() failed (ret=%d)", __func__, (int) ret);
            /* continue with uninitialisation*/
        }
        ret = driver->Uninitialize();
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: Uninitialize() failed (ret=%d)", __func__, (int) ret);
            return ret;
        }
    } else {
        mtx_lock.unlock();
        ret = ARM_DRIVER_OK;
    }
    return ret;
}

/** @brief  Create a new key in the store (which doesnt already exist). */
CfstoreKey* Cfstore::create(const char* key_name, size_t value_len, const ARM_CFSTORE_KEYDESC* kdesc)
{
    int32_t ret = ARM_DRIVER_ERROR;
    CfstoreKey *kv = new CfstoreKey();

    cfstore_debug_log("%s:entered:\n", __func__);

    kv->owner = this;
    ret = driver->Create(key_name, value_len, kdesc, (ARM_CFSTORE_HANDLE) kv->hkey);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: Create() failed (ret=%d)", __func__, (int) ret);
        delete kv;
        kv = NULL;
    }
    mtx_lock.lock();
    this->key_count++;
    mtx_lock.unlock();
    return kv;
}

/** @brief   method to walk the cfstore to find key name matching the specified pattern
 *
 * On success, the method returns a valid pointer to the next KV matching the query.
 * The caller is responsible for deleting the return CfstoreKey object.
 * Otherwise the method returns NULL.
 */
CfstoreKey* Cfstore::find(const char* key_name_query, CfstoreKey* kv_previous)
{
    int32_t ret = ARM_DRIVER_ERROR;
    CfstoreKey *kv_next = new CfstoreKey();

    cfstore_debug_log("%s:entered:\n", __func__);
    kv_next->owner = this;
    ret = driver->Find(key_name_query, kv_previous->hkey, kv_next->hkey);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: Find() failed (ret=%d)", __func__, (int) ret);
        delete kv_next;
        kv_next = NULL;
    }
    return kv_next;
}

/** @brief  Flush the keys from the in-memory cache to nv store */
int32_t Cfstore::flush(void)
{
    cfstore_debug_log("%s:entered:\n", __func__);
    return driver->Flush();
}

/** @brief  Open a pre-existing key in the store */
CfstoreKey* Cfstore::open(const char* key_name, ARM_CFSTORE_FMODE flags)
{
    int32_t ret = ARM_DRIVER_ERROR;
    CfstoreKey *kv = new CfstoreKey();

    cfstore_debug_log("%s:entered:\n", __func__);
    kv->owner = this;
    ret = driver->Open(key_name, flags, (ARM_CFSTORE_HANDLE) kv->hkey);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: Open() failed (ret=%d)", __func__, (int) ret);
        delete kv;
        kv = NULL;
    }
    return kv;
}

/*
 * CfstoreKey methods
 */

CfstoreKey::CfstoreKey(void)
{
    cfstore_debug_log("%s:entered:\n", __func__);
    memset(hkey, 0, CFSTORE_HANDLE_BUFSIZE);
    owner = NULL;
}

CfstoreKey::~CfstoreKey(void)
{
    cfstore_debug_log("%s:entered:\n", __func__);
    owner->mtx_lock.lock();
    owner->key_count--;
    owner->mtx_lock.unlock();
}

int32_t CfstoreKey::close(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->Close(this->hkey);
    return ret;
}

int32_t CfstoreKey::destroy(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->Delete(this->hkey);
    return ret;
}

int32_t CfstoreKey::get_key_name(char* key_name, uint8_t *key_len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->GetKeyName(this->hkey, key_name, key_len);
    return ret;
}

int32_t CfstoreKey::get_value_len(size_t *value_len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->GetValueLen(this->hkey, value_len);
    return ret;
}

int32_t CfstoreKey::read(void* data, size_t* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->Read(this->hkey, data, len);
    return ret;
}

int32_t CfstoreKey::rseek(size_t offset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->Rseek(this->hkey, offset);
    return ret;
}

int32_t CfstoreKey::write(const char* data, size_t* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_driver_t *driver = this->owner->driver;

    cfstore_debug_log("%s:entered:\n", __func__);
    ret = driver->Write(this->hkey, data, len);
    return ret;
}


}	/* namespace cfstore */

