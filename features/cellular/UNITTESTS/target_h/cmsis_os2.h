/*
 * Copyright (c) , Arm Limited and affiliates.
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

#ifndef __CMSIS_OS2_H__
#define __CMSIS_OS2_H__

//If conflicts, then remove these, copied from cmsis_os.h
typedef int32_t                  osStatus;

#define osOK 0



//These are from cmsis_os2.h
typedef void *osSemaphoreId_t;

typedef struct {
  const char                   *name;   ///< name of the semaphore
  uint32_t                 attr_bits;   ///< attribute bits
  void                      *cb_mem;    ///< memory for control block
  uint32_t                   cb_size;   ///< size of provided memory for control block
} osSemaphoreAttr_t;

//Thread
typedef enum {
  osPriorityNormal        = 24       ///< Priority: normal
} osPriority_t;

typedef void *osThreadId_t;

/// Attributes structure for thread.
typedef struct {
} osThreadAttr_t;

#define osWaitForever         0xFFFFFFFFU


#endif
