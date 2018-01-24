/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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

#include "objects.h"
#include "timer_api.h"

#if CONFIG_TIMER_EN

extern HAL_TIMER_OP HalTimerOp;

extern HAL_Status HalTimerInitRtl8195a_Patch(
    IN  VOID    *Data
);

static void gtimer_timeout_handler (uint32_t tid)
{
    gtimer_t *obj = (gtimer_t *)tid;
    gtimer_irq_handler handler;
    u8 timer_id = obj->hal_gtimer_adp.TimerId;
    
    if (obj->handler != NULL) {
        handler = (gtimer_irq_handler)obj->handler;
        handler(obj->hid);
    }

    if (!obj->is_periodcal) {
        gtimer_stop(obj);
    }

    if(timer_id < 2) {
        // Timer0 | Timer1: clear ISR here
        // Timer 2~7 ISR will be cleared in HAL
        HalTimerClearIsr(timer_id);
    }
}

void gtimer_init (gtimer_t *obj, uint32_t tid)
{
    PTIMER_ADAPTER pTimerAdapter = &(obj->hal_gtimer_adp);

    if ((tid == 1) || (tid == 6) || (tid == 7)) {
        DBG_TIMER_ERR("gtimer_init: This timer is reserved for HAL driver\r\n", tid);
        return;
    }

    if (tid > GTIMER_MAX) {
        DBG_TIMER_ERR("gtimer_init: Invalid TimerId=%d\r\n", tid);
        return;
    }
    
    pTimerAdapter->IrqDis = 0;    // Enable Irq @ initial
    pTimerAdapter->IrqHandle.IrqFun = (IRQ_FUN) gtimer_timeout_handler;
    if(tid == 0) {
        pTimerAdapter->IrqHandle.IrqNum = TIMER0_IRQ;
    } else if(tid == 1) {
        pTimerAdapter->IrqHandle.IrqNum = TIMER1_IRQ;
    } else {
        pTimerAdapter->IrqHandle.IrqNum = TIMER2_7_IRQ;
    }
    pTimerAdapter->IrqHandle.Priority = 0;
    pTimerAdapter->IrqHandle.Data = (u32)obj;
    pTimerAdapter->TimerId = (u8)tid;
    pTimerAdapter->TimerIrqPriority = 0;
    pTimerAdapter->TimerLoadValueUs = 0xFFFFFFFF;   // Just a whatever value
    pTimerAdapter->TimerMode = USER_DEFINED;
    
    HalTimerInit ((VOID*) pTimerAdapter);
}

void gtimer_deinit (gtimer_t *obj)
{
    PTIMER_ADAPTER pTimerAdapter = &(obj->hal_gtimer_adp);
    
    HalTimerDeInit((void*)pTimerAdapter);
}

uint32_t gtimer_read_tick (gtimer_t *obj)
{
    PTIMER_ADAPTER pTimerAdapter = &obj->hal_gtimer_adp;
    
    return (HalTimerOp.HalTimerReadCount(pTimerAdapter->TimerId));
}

uint64_t gtimer_read_us (gtimer_t *obj)
{
    uint64_t time_us;

    time_us = gtimer_read_tick(obj)*1000000/32768;

    return (time_us);
}

void gtimer_reload (gtimer_t *obj, uint32_t duration_us)
{
    PTIMER_ADAPTER pTimerAdapter = &obj->hal_gtimer_adp;

    HalTimerReLoad(pTimerAdapter->TimerId, duration_us);
}


void gtimer_start (gtimer_t *obj)
{
    PTIMER_ADAPTER pTimerAdapter = &obj->hal_gtimer_adp;
    u8 TimerId = pTimerAdapter->TimerId;

  HalTimerEnable(TimerId); 
}

void gtimer_start_one_shout (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid)
{
    obj->is_periodcal = _FALSE;
    obj->handler = handler;
    obj->hid = hid;
    gtimer_reload(obj, duration_us);
    gtimer_start(obj);
}

void gtimer_start_periodical (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid)
{
    obj->is_periodcal = _TRUE;
    obj->handler = handler;
    obj->hid = hid;
    if (duration_us > GTIMER_TICK_US) {
        // reload will takes extra 1 tick
        duration_us -= GTIMER_TICK_US;
    }
    gtimer_reload(obj, duration_us);
    gtimer_start(obj);
}

void gtimer_stop (gtimer_t *obj)
{
    PTIMER_ADAPTER pTimerAdapter = &obj->hal_gtimer_adp;
    HalTimerDisable(pTimerAdapter->TimerId);    
}

#endif  // end of "#if CONFIG_TIMER_EN"

