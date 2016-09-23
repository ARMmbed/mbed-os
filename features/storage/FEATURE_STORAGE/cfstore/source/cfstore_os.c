/* @file cfstore_os.c
 *
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
 * Platform OS specific code implementing coarse grain locking above the
 * portable configuration_store implementation.
 */

#include "cfstore_os.h"
#include "cfstore_debug.h"
#include "configuration_store.h"


/* cfstore_os context holding state for this layer */
typedef struct cfstore_os_ctx_t {
    cfstore_os_mutex_t lock;
    ARM_CFSTORE_CALLBACK client_callback;
    void* client_context;

#ifndef STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS
    cfstore_os_semaphore_t async_sem;
#endif  /* STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS */

} cfstore_os_ctx_t;

static cfstore_os_ctx_t cfstore_os_ctx_g;

/*
 * Portable synchronisation function implementation
 */
#ifdef __MBED__


void cfstore_os_mutex_init(cfstore_os_mutex_t* lock)
{
    lock->_osMutexDef.mutex = lock->_mutex_data;
    lock->_osMutexId = osMutexCreate(&lock->_osMutexDef);
}

void cfstore_os_mutex_lock(cfstore_os_mutex_t* lock)
{
    osMutexWait(lock->_osMutexId, osWaitForever);
}

void cfstore_os_mutex_unlock(cfstore_os_mutex_t* lock)
{
    osMutexRelease(lock->_osMutexId);
}

void cfstore_os_semaphore_init(cfstore_os_semaphore_t *sem)
{
    memset(sem->os_semaphore_cb, 0, sizeof(uint32_t)*2);
    sem->os_semaphore_def.semaphore = sem->os_semaphore_cb;
    sem->id = osSemaphoreCreate(&sem->os_semaphore_def, 0);
}

void cfstore_os_semaphore_lock(cfstore_os_semaphore_t *sem)
{
    osSemaphoreWait(sem->id, osWaitForever);
}

void cfstore_os_semaphore_unlock(cfstore_os_semaphore_t *sem)
{
    osSemaphoreRelease(sem->id);
}


#else

/* null implementation */
void cfstore_os_mutex_init(cfstore_os_mutex_t* lock) {(void) lock;}
void cfstore_os_mutex_lock(cfstore_os_mutex_t* lock) {(void) lock;}
void cfstore_os_mutex_unlock(cfstore_os_mutex_t* lock) {(void) lock;}
void cfstore_os_semaphore_init(cfstore_os_semaphore_t *sem) {(void) sem;}
void cfstore_os_semaphore_lock(cfstore_os_semaphore_t *sem) {(void) sem;}
void cfstore_os_semaphore_unlock(cfstore_os_semaphore_t *sem) {(void) sem;}

#endif   /* __MBED__ */


/*
 * OS platform helper functions.
 */
static inline cfstore_os_ctx_t *cfstore_ctx_get(void)
{
    return &cfstore_os_ctx_g;
}

/*
 * OS platform ARM_CFSTORE_DRIVER dispatch methods.
 */

#ifndef STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS

static void cfstore_os_callback(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *client_context, ARM_CFSTORE_HANDLE handle)
{
    (void) status;
    (void) cmd_code;
    cfstore_os_ctx_t *ctx = (cfstore_os_ctx_t *) client_context;
    (void) handle;

    CFSTORE_FENTRYLOG("%s:entered: status=%d, cmd_code=%d handle=%p\n", __func__, (int) status, (int) cmd_code, handle);
    switch(cmd_code) {
        case CFSTORE_OPCODE_INITIALIZE:
        case CFSTORE_OPCODE_FLUSH:
        case CFSTORE_OPCODE_UNINITIALIZE:
            CFSTORE_DBGLOG("%s: before async unlock\n", __func__);
            cfstore_os_semaphore_unlock(&ctx->async_sem);
            CFSTORE_DBGLOG("%s: after async unlock\n", __func__);
            break;

        case CFSTORE_OPCODE_CLOSE:
        case CFSTORE_OPCODE_CREATE:
        case CFSTORE_OPCODE_DELETE:
        case CFSTORE_OPCODE_FIND:
        case CFSTORE_OPCODE_GET_KEY_NAME:
        case CFSTORE_OPCODE_GET_STATUS:
        case CFSTORE_OPCODE_GET_VALUE_LEN:
        case CFSTORE_OPCODE_OPEN:
        case CFSTORE_OPCODE_POWER_CONTROL:
        case CFSTORE_OPCODE_READ:
        case CFSTORE_OPCODE_RSEEK:
        case CFSTORE_OPCODE_WRITE:
        default:
            CFSTORE_DBGLOG("%s:debug: received asynchronous notification for opcode=%d\n", __func__, cmd_code);
    }
    if (ctx->client_callback) {
        ctx->client_callback(status, cmd_code, ctx->client_context, handle);
    }
    return;
}


#else

static void cfstore_os_callback(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *client_context, ARM_CFSTORE_HANDLE handle)
{
    (void) status;
    (void) cmd_code;
    cfstore_os_ctx_t *ctx = (cfstore_os_ctx_t *) client_context;
    (void) handle;

    CFSTORE_FENTRYLOG("%s:entered: status=%d, cmd_code=%d (%s) handle=%p\n", __func__, (int) status, (int) cmd_code, cfstore_test_opcode_str[cmd_code], handle);
    if (ctx->client_callback) {
        ctx->client_callback(status, cmd_code, ctx->client_context, handle);
    }
    return;
}


#endif /* STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS */

static int32_t cfstore_os_close(ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Close(hkey);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_create(const char *key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Create(key_name, value_len, kdesc, hkey);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_delete(ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Delete(hkey);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_find(const char *key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Find(key_name_query, previous, next);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_flush(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Flush();
    cfstore_os_semaphore_lock(&ctx->async_sem);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

ARM_CFSTORE_CAPABILITIES cfstore_os_get_capabilities(void)
{
    return cfstore_driver.GetCapabilities();
}

static int32_t cfstore_os_get_key_name(ARM_CFSTORE_HANDLE hkey, char *key_name, uint8_t *key_name_len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.GetKeyName(hkey, key_name, key_name_len);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static ARM_CFSTORE_STATUS cfstore_os_get_status(void)
{
    return cfstore_driver.GetStatus();
}

static int32_t cfstore_os_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.GetValueLen(hkey, value_len);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

ARM_DRIVER_VERSION cfstore_os_get_version(void)
{
    return cfstore_driver.GetVersion();
}

static int32_t cfstore_os_initialise(ARM_CFSTORE_CALLBACK client_callback, void* client_context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ctx->client_callback = client_context;
    ctx->client_context = client_context;
    CFSTORE_DBGLOG("%s: before Initialize()\n", __func__);
    ret = cfstore_driver.Initialize(cfstore_os_callback, ctx);
    CFSTORE_DBGLOG("%s: after Initialize() before async lock (ret=%d\n", __func__, (int) ret);
    cfstore_os_semaphore_lock(&ctx->async_sem);
    CFSTORE_DBGLOG("%s: after async lock\n", __func__);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_open(const char *key_name, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Open(key_name, flags, hkey);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_power_control(ARM_POWER_STATE state)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.PowerControl(state);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Read(hkey, data, len);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Rseek(hkey, offset);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_uninitialise(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Uninitialize();
    cfstore_os_semaphore_lock(&ctx->async_sem);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}

static int32_t cfstore_os_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    cfstore_os_mutex_lock(&ctx->lock);
    ret = cfstore_driver.Write(hkey, data, len);
    cfstore_os_mutex_unlock(&ctx->lock);
    return ret;
}


/* dispatch block with OS specific support e.g. for locking */
ARM_CFSTORE_DRIVER cfstore_os_mbed_driver = {
    .Close = cfstore_os_close,
    .Create = cfstore_os_create,
    .Delete= cfstore_os_delete,
    .Find = cfstore_os_find,
    .Flush = cfstore_os_flush,
    .GetCapabilities = cfstore_os_get_capabilities,
    .GetKeyName = cfstore_os_get_key_name,
    .GetStatus = cfstore_os_get_status,
    .GetValueLen = cfstore_os_get_value_len,
    .GetVersion = cfstore_os_get_version,
    .Initialize = cfstore_os_initialise,
    .Open = cfstore_os_open,
    .PowerControl = cfstore_os_power_control,
    .Read = cfstore_os_read,
    .Rseek = cfstore_os_rseek,
    .Uninitialize = cfstore_os_uninitialise,
    .Write = cfstore_os_write,
};


/** @brief  Initialize the context associated with this OS platform for the locking layer */
int32_t cfstore_os_init()
{
    cfstore_os_ctx_t *ctx = cfstore_ctx_get();

    memset(ctx, 0, sizeof(cfstore_os_ctx_t));
    cfstore_os_mutex_init(&ctx->lock);
    cfstore_os_semaphore_init(&ctx->async_sem);
    return ARM_DRIVER_OK;
}
