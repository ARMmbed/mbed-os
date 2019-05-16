/* Copyright (c) 2019 Unisoc Communications Inc.
 * SPDX-License-Identifier: Apache-2.0
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

/******************************************************************************
 * @file   : maclib_task.h
 * @brief  : WiFi MACLib task header file
 * @version: V1.0
 * @date   : 6. May 2017
 *
 * @note   :
 *
 ******************************************************************************/

#ifndef _MACLIB_TASK_H_
#define _MACLIB_TASK_H_

#define MAC_LIB_MAX_FLEN    1536

/**
 * Enums
 */
typedef enum {
    MACLIB_MSG_EVNT_HNDL,
    MACLIB_MSG_WLAND_XMIT_PKT,
    MACLIB_MSG_LWIP_XMIT_PKT
} MACLIB_MSG_TYPE_T;

/**
 * Structures
 */
typedef struct {
    MACLIB_MSG_TYPE_T type;
    void *msg;
    int is_free;
} maclib_msg_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     : MACLib main task [MACLib api]
 * @param[in] : pvParameters(pointer to enet data)
 * @param[out]:
 * @return    :
 */
extern void maclib_task(void *pvParameters);
extern void mbed_event_handle_cb(unsigned int event);
extern void mbed_mltask_sleep_cb(void);
extern void mbed_mltask_wakeup_cb(void);

#ifdef __cplusplus
}
#endif

#endif /* _MACLIB_TASK_H_ */
