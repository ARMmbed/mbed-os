/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       Delay functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Service Calls ====

//  Service Calls definitions
SVC0_1(Delay,      osStatus_t, uint32_t)
SVC0_2(DelayUntil, osStatus_t, uint32_t, uint32_t)

/// Wait for Timeout (Time Delay).
/// \note API identical to osDelay
osStatus_t svcRtxDelay (uint32_t ticks) {

  if (ticks == 0U) {
    return osOK;
  }

  osRtxThreadWaitEnter(osRtxThreadWaitingDelay, ticks);

  return osOK;
}

/// Wait until specified time.
/// \note API identical to osDelayUntil
osStatus_t svcRtxDelayUntil (uint32_t ticks_l, uint32_t ticks_h) {
  uint64_t ticks = ((uint64_t)ticks_l) | ((uint64_t)ticks_h << 32);

  ticks -= osRtxInfo.kernel.tick;
  if (ticks >= 0xFFFFFFFFU) {
    EvrRtxThreadError(NULL, osErrorParameter);
    return osErrorParameter;
  }
  if (ticks == 0U) {
    return osOK;
  }

  osRtxThreadWaitEnter(osRtxThreadWaitingDelay, (uint32_t)ticks);

  return osOK;
}


//  ==== Public API ====

/// Wait for Timeout (Time Delay).
osStatus_t osDelay (uint32_t ticks) {
  EvrRtxThreadDelay(ticks);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(NULL, osErrorISR);
    return osErrorISR;
  }
  return __svcDelay(ticks);
}

/// Wait until specified time.
osStatus_t osDelayUntil (uint64_t ticks) {
  EvrRtxThreadDelayUntil(ticks);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxThreadError(NULL, osErrorISR);
    return osErrorISR;
  }
  return __svcDelayUntil((uint32_t)ticks, (uint32_t)(ticks >> 32));
}
