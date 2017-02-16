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

#if 0
typedef enum {
/******  Cortex-M3 Processor Exceptions Numbers ********/
    NonMaskableInt_IRQn     = -14,  /*!< 2 Non Maskable Interrupt                 */
    HardFault_IRQn          = -13,  /*!< 3 Hard Fault, all classes of Fault       */
    MemoryManagement_IRQn   = -12,  /*!< 4 Cortex-M3 Memory Management Interrupt  */
    BusFault_IRQn           = -11,  /*!< 5 Cortex-M3 Bus Fault Interrupt          */
    UsageFault_IRQn         = -10,  /*!< 6 Cortex-M3 Usage Fault Interrupt        */
    SVCall_IRQn             = -5,   /*!< 11 Cortex-M3 SV Call Interrupt           */
    DebugMonitor_IRQn       = -4,   /*!< 12 Cortex-M3 Debug Monitor Interrupt     */
    PendSV_IRQn             = -2,   /*!< 14 Cortex-M3 Pend SV Interrupt           */
    SysTick_IRQn            = -1,   /*!< 15 Cortex-M3 System Tick Interrupt       */

/******  RTL8195A Specific Interrupt Numbers ************/
    SYSTEM_ON_IRQ           =  0,
    WDG_IRQ                 =  1,
    TIMER0_IRQ              =  2,
    TIMER1_IRQ              =  3,
    I2C3_IRQ                =  4,
    TIMER2_7_IRQ            =  5,
    SPI0_IRQ                =  6,
    GPIO_IRQ                =  7,
    UART0_IRQ               =  8,
    SPI_FLASH_IRQ           =  9,
    USB_OTG_IRQ             =  10,
    SDIO_HOST_IRQ           =  11,
    SDIO_DEVICE_IRQ         =  12,
    I2S0_PCM0_IRQ           =  13,
    I2S1_PCM1_IRQ           =  14,
    WL_DMA_IRQ              =  15,
    WL_PROTOCOL_IRQ         =  16,
    CRYPTO_IRQ              =  17,
    GMAC_IRQ                =  18,
    PERIPHERAL_IRQ          =  19,
    GDMA0_CHANNEL0_IRQ      =  20,
    GDMA0_CHANNEL1_IRQ      =  21,
    GDMA0_CHANNEL2_IRQ      =  22,
    GDMA0_CHANNEL3_IRQ      =  23,
    GDMA0_CHANNEL4_IRQ      =  24,
    GDMA0_CHANNEL5_IRQ      =  25,
    GDMA1_CHANNEL0_IRQ      =  26,
    GDMA1_CHANNEL1_IRQ      =  27,
    GDMA1_CHANNEL2_IRQ      =  28,
    GDMA1_CHANNEL3_IRQ      =  29,
    GDMA1_CHANNEL4_IRQ      =  30,
    GDMA1_CHANNEL5_IRQ      =  31,

/******  RTL8195A Peripheral Interrupt Numbers ************/
    I2C0_IRQ                =  64,// 0 + 64,
    I2C1_IRQ                =  65,// 1 + 64,
    I2C2_IRQ                =  66,// 2 + 64,
    SPI1_IRQ                =  72,// 8 + 64,
    SPI2_IRQ                =  73,// 9 + 64,
    UART1_IRQ               =  80,// 16 + 64,
    UART2_IRQ               =  81,// 17 + 64,
    UART3_IRQ               =  88,// 24 + 64,
    ADC_IRQ                 =  89,// 25 + 64,
    DAC0_IRQ                =  91,// 27 + 64,
    DAC1_IRQ                =  92,// 28 + 64,
    LP_EXTENSION_IRQ        =  93,// 29 + 64

    PTA_TRX_IRQ             =  95,// 31 + 64
    RXI300_IRQ              =  96,// 0  + 32 + 64
    NFC_IRQ                 =  97 // 1  + 32 + 64
} IRQn_Type;


typedef void (*IRQn_Func)(void *data);

typedef struct {
    IRQn_Func handler;
    IRQn_Type irqnum;
    uint32_t  data;
    uint32_t  priority;
} IRQ_Handle;
#endif

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

#if 0
// Interface to ROM functions
extern __longcall void VectorTableInitRtl8195A(uint32_t stackp);
extern __longcall void VectorTableInitForOSRtl8195A(void *svc, void *svh, void *tick);
extern __longcall bool VectorIrqRegisterRtl8195A(IRQ_Handle *handle);
extern __longcall bool VectorIrqUnRegisterRtl8195A(IRQ_Handle *handle);
extern __longcall void VectorIrqEnRtl8195A(IRQ_Handle *handle);
extern __longcall void VectorIrqDisRtl8195A(IRQ_Handle *handle);
extern __longcall void HalPeripheralIntrHandle(void);
extern __longcall void HalHardFaultHandler(uint32_t);
#endif

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
