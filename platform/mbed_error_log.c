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

mbed_error_status_t mbed_log_put_error(mbed_error_ctx *error_ctx)
{
    //Return error if error_ctx is NULL
    if(NULL == error_ctx) {
        return ERROR_INVALID_ARGUMENT;
    }
    
    core_util_critical_section_enter();
    error_log_count++;
    memcpy(&mbed_error_ctx_log[error_log_count % MBED_CONF_ERROR_LOG_SIZE], error_ctx, sizeof(mbed_error_ctx) );
    core_util_critical_section_exit(); 
    
    return ERROR_SUCCESS;
}

mbed_error_status_t mbed_log_get_error(int index, mbed_error_ctx *error_ctx)
{
    //Return error if index is more than max log size
    if(index >= MBED_CONF_ERROR_LOG_SIZE) {
        return ERROR_INVALID_ARGUMENT;
    }
    
    core_util_critical_section_enter();
    //calculate the index where we want to pick the ctx
    if(error_log_count >= MBED_CONF_ERROR_LOG_SIZE) {
        index = (error_log_count + index + 1) % MBED_CONF_ERROR_LOG_SIZE;
    }
    core_util_critical_section_exit(); 
    memcpy(error_ctx, &mbed_error_ctx_log[index % MBED_CONF_ERROR_LOG_SIZE], sizeof(mbed_error_ctx) );
        
    return ERROR_SUCCESS;
}

mbed_error_ctx *mbed_log_get_entry(void)
{
    core_util_critical_section_enter();
    error_log_count++;
    mbed_error_ctx *ctx = &mbed_error_ctx_log[error_log_count % MBED_CONF_ERROR_LOG_SIZE];
    core_util_critical_section_exit(); 
    
    return ctx;
}

mbed_error_status_t mbed_log_get_last_error(mbed_error_ctx *error_ctx)
{
    if(-1 == error_log_count) {
        return ERROR_ITEM_NOT_FOUND;
    }
    core_util_critical_section_enter();
    memcpy(error_ctx, &mbed_error_ctx_log[error_log_count % MBED_CONF_ERROR_LOG_SIZE], sizeof(mbed_error_ctx) );
    core_util_critical_section_exit(); 
    
    return ERROR_SUCCESS;
}

int mbed_log_get_error_log_count()
{
    return (error_log_count >= MBED_CONF_ERROR_LOG_SIZE? MBED_CONF_ERROR_LOG_SIZE:error_log_count+1);
}

mbed_error_status_t mbed_log_reset()
{
    core_util_critical_section_enter();
    error_log_count = -1;
    core_util_critical_section_exit(); 
    
    return ERROR_SUCCESS;
}

#endif
