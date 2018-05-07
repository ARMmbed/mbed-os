/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 */
#include <stdlib.h>
#include <stdarg.h>
#include "device.h"
#include "platform/mbed_error.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"

#ifndef MBED_CONF_ERROR_LOG_DISABLED
#include "platform/mbed_error_log.h"

static mbed_error_ctx mbed_error_ctx_log[MBED_CONF_ERROR_LOG_SIZE] = {0};
static int error_log_count = -1;

static void mbed_log_lock()
{
    core_util_critical_section_enter();
}

static void mbed_log_unlock()
{
    core_util_critical_section_exit(); 
}

MbedErrorStatus mbed_log_put_error(mbed_error_ctx *error_ctx)
{
    //Return error if error_ctx is NULL
    if(NULL == error_ctx) {
        return ERROR_INVALID_ARGUMENT;
    }
    
    mbed_log_lock();
    error_log_count++;
    memcpy(&mbed_error_ctx_log[error_log_count % MBED_CONF_ERROR_LOG_SIZE], error_ctx, sizeof(mbed_error_ctx) );
    mbed_log_unlock();
    
    return ERROR_SUCCESS;
}

MbedErrorStatus mbed_log_get_error(int index, mbed_error_ctx *error_ctx)
{
    //Return error if index is more than max log size
    if(index >= MBED_CONF_ERROR_LOG_SIZE) {
        return ERROR_INVALID_ARGUMENT;
    }
    
    mbed_log_lock();
    //calculate the index where we want to pick the ctx
    if(error_log_count >= MBED_CONF_ERROR_LOG_SIZE) {
        index = (error_log_count + index + 1) % MBED_CONF_ERROR_LOG_SIZE;
    }
    mbed_log_unlock();
    memcpy(error_ctx, &mbed_error_ctx_log[index % MBED_CONF_ERROR_LOG_SIZE], sizeof(mbed_error_ctx) );
        
    return ERROR_SUCCESS;
}

mbed_error_ctx *mbed_log_get_entry(void)
{
    mbed_log_lock();
    error_log_count++;
    mbed_error_ctx *ctx = &mbed_error_ctx_log[error_log_count % MBED_CONF_ERROR_LOG_SIZE];
    mbed_log_unlock();
    
    return ctx;
}

MbedErrorStatus mbed_log_get_last_error(mbed_error_ctx *error_ctx)
{
    if(-1 == error_log_count) {
        return ERROR_NOT_FOUND;
    }
    mbed_log_lock();
    memcpy(error_ctx, &mbed_error_ctx_log[error_log_count % MBED_CONF_ERROR_LOG_SIZE], sizeof(mbed_error_ctx) );
    mbed_log_unlock();
    
    return ERROR_SUCCESS;
}

int mbed_log_get_error_log_count()
{
    return (error_log_count >= MBED_CONF_ERROR_LOG_SIZE? MBED_CONF_ERROR_LOG_SIZE:error_log_count+1);
}

MbedErrorStatus mbed_log_reset()
{
    mbed_log_lock();
    error_log_count = -1;
    mbed_log_unlock();
    
    return ERROR_SUCCESS;
}

#if DEVICE_LOCALFILESYSTEM
MbedErrorStatus mbed_log_save_error_log(const char *path)
{
    MbedErrorStatus ret = ERROR_SUCCESS;
    mbed_error_ctx ctx = {0};
    int log_count = mbed_log_get_error_log_count();
    FILE *error_log_file = NULL;
    
    //Open the file for saving the error log info
    if((error_log_file = fopen( path, "w" ) ) == NULL){
        ret = MAKE_ERROR(ENTITY_PLATFORM, ERROR_CODE_OPEN_FAILED);
        goto exit;
    }

    //first line of file will be error log count
    if(fprintf(error_log_file, "\nError Log Count = %d\n", log_count)){
        ret = MAKE_ERROR(ENTITY_PLATFORM, ERROR_CODE_WRITE_FAILED);
        goto exit;
    }
    
    //Update with error log info
    while(log_count >= 0) {
        mbed_log_get_error(log_count, &ctx);
        //first line of file will be error log count
        if(fprintf(error_log_file, "\n%d: Status:0x%x ThreadId:0x%x Address:0x%x File:%s+%d\n", log_count, ctx.error_status, ctx.thread_id, ctx.error_address)) {
            ret = MAKE_ERROR(ENTITY_PLATFORM, ERROR_CODE_WRITE_FAILED);
            goto exit;
        }
        log_count--;
    }

    ret = 0;

exit:
    fclose(error_log_file);
        
    return ret;
}
#endif
#endif
