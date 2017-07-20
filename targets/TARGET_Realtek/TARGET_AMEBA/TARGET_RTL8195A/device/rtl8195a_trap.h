/*
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
#ifndef MBED_RTL8195A_TRAP_H
#define MBED_RTL8195A_TRAP_H

typedef void (*VECTOR_Func)(void *data);

typedef struct {
    void (*RamStartFun)(void);
} RAM_START_FUNCTION;

typedef struct {
    void (*RamStartFun)(void);
    void (*RamWakeupFun)(void);
    void (*RamPatchFun0)(void);
    void (*RamPatchFun1)(void);
    void (*RamPatchFun2)(void);
} RAM_START_TABLE;

extern uint8_t * __ram_table_start__;

extern __longcall void HalWdgIntrHandle(void);
extern __longcall void HalHardFaultHandler(u32);


#define __TRAP_Init                 VectorTableInitForOSRtl8195A
#define __TRAP_SetVector            VectorTableInitRtl8195A
#define __TRAP_HardFaultHandler     HalHardFaultHandler
#define __INTR_Register             VectorIrqRegisterRtl8195A
#define __INTR_Unregister           VectorIrqUnRegisterRtl8195A
#define __INTR_Enable               VectorIrqEnRtl8195A
#define __INTR_Disable              VectorIrqDisRtl8195A
#define __INTR_WatchdogHandler      HalWdgIntrHandle

// Interface for HAL layer
static inline void TRAP_Init(void *svc, void *svh, void *tick)
{
    __TRAP_Init(svc, svh, tick);
}

static inline void TRAP_SetVector(uint32_t stackp)
{
    __TRAP_SetVector(stackp);
}

static inline bool INTR_Register(IRQ_Handle *handle)
{
    return __INTR_Register(handle);
}

static inline bool INTR_Unregister(IRQ_Handle *handle)
{
    return __INTR_Unregister(handle);
}

static inline void INTR_Enable(IRQ_Handle *handle)
{
    __INTR_Enable(handle);
}

static inline void INTR_Disable(IRQ_Handle *handle)
{
    __INTR_Disable(handle);
}
static inline void INTR_WatchdogHandler(void)
{
    __INTR_WatchdogHandler();
}

#endif

