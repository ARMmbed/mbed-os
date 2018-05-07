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
#ifndef MBED_ERROR_LOG_H
#define MBED_ERROR_LOG_H

#ifndef MBED_CONF_ERROR_LOG_SIZE
    #define MBED_CONF_ERROR_LOG_SIZE  5
#else
    #if MBED_CONF_ERROR_LOG_SIZE == 0
        #define MBED_CONF_ERROR_LOG_SIZE  1
    #endif    
#endif

#ifdef __cplusplus
extern "C" {
#endif
MbedErrorStatus mbed_log_put_error(mbed_error_ctx *error_ctx);
MbedErrorStatus mbed_log_get_error(int index, mbed_error_ctx *error_ctx);
mbed_error_ctx *mbed_log_get_entry(void);
MbedErrorStatus mbed_log_get_last_error(mbed_error_ctx *error_ctx);
int mbed_log_get_error_log_count(void);
MbedErrorStatus mbed_log_reset(void);
MbedErrorStatus mbed_save_error_log(const char *path);    
    
#ifdef __cplusplus
}
#endif

#endif


