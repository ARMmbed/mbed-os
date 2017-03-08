/*******************************************************************************
 *Copyright (c) 2013-2016 Realtek Semiconductor Corp, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 * 
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); 
 * you may not use this file except in compliance with the License.
 * 
 * You may obtain a copy of the License at https://www.mbed.com/licenses/PBL-1.0
 * 
 * See the License for the specific language governing permissions and limitations under the License.
 *******************************************************************************
 */
#ifndef _HAL_API_H_
#define _HAL_API_H_

#include "basic_types.h"
#include "hal_irqn.h"

#define HAL_READ32(base, addr)            \
        rtk_le32_to_cpu(*((volatile u32*)(base + addr)))
    
#define HAL_WRITE32(base, addr, value32)  \
        ((*((volatile u32*)(base + addr))) = rtk_cpu_to_le32(value32))


#define HAL_READ16(base, addr)            \
        rtk_le16_to_cpu(*((volatile u16*)(base + addr)))
        
#define HAL_WRITE16(base, addr, value)  \
        ((*((volatile u16*)(base + addr))) = rtk_cpu_to_le16(value))
    

#define HAL_READ8(base, addr)            \
        (*((volatile u8*)(base + addr)))
            
#define HAL_WRITE8(base, addr, value)  \
        ((*((volatile u8*)(base + addr))) = value)

#if 0
// These "extern _LONG_CALL_" function declaration are for RAM code building only
// For ROM code building, thses code should be marked off
extern _LONG_CALL_ u8 
HalPinCtrlRtl8195A(
    IN u32  Function, 
    IN u32  PinLocation, 
    IN BOOL   Operation
    );

extern _LONG_CALL_ VOID 
HalSerialPutcRtl8195a(
    IN  u8 c
    );

extern _LONG_CALL_ u8 
HalSerialGetcRtl8195a(
    IN  BOOL    PullMode
    );

extern _LONG_CALL_ u32
HalSerialGetIsrEnRegRtl8195a(VOID);

extern _LONG_CALL_ VOID
HalSerialSetIrqEnRegRtl8195a (
    IN  u32 SetValue
    );

extern _LONG_CALL_  VOID
VectorTableInitForOSRtl8195A(
    IN  VOID *PortSVC,
    IN  VOID *PortPendSVH,
    IN  VOID *PortSysTick    
    );

extern _LONG_CALL_ BOOL
VectorIrqRegisterRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
    );

extern _LONG_CALL_ BOOL
VectorIrqUnRegisterRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
    );

extern _LONG_CALL_ VOID
VectorIrqEnRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
    );

extern _LONG_CALL_  VOID
VectorIrqDisRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
    );
#endif

extern BOOLEAN SpicFlashInitRtl8195A(u8 SpicBitMode);
extern VOID InitWDGIRQ(VOID);

#define PinCtrl HalPinCtrlRtl8195A

#define DiagPutChar	HalSerialPutcRtl8195a
#define DiagGetChar HalSerialGetcRtl8195a
#define DiagGetIsrEnReg HalSerialGetIsrEnRegRtl8195a
#define DiagSetIsrEnReg HalSerialSetIrqEnRegRtl8195a

#define InterruptForOSInit VectorTableInitForOSRtl8195A
#define InterruptRegister VectorIrqRegisterRtl8195A
#define InterruptUnRegister  VectorIrqUnRegisterRtl8195A

#define InterruptEn VectorIrqEnRtl8195A
#define InterruptDis VectorIrqDisRtl8195A

#define SpicFlashInit SpicFlashInitRtl8195A
#define Calibration32k En32KCalibration
#define WDGInit InitWDGIRQ

enum  _HAL_Status
{
  HAL_OK            = 0x00,
  HAL_BUSY          = 0x01,
  HAL_TIMEOUT       = 0x02,
  HAL_ERR_PARA      = 0x03,     // error with invaild parameters 
  HAL_ERR_MEM       = 0x04,     // error with memory allocation failed
  HAL_ERR_HW        = 0x05,     // error with hardware error

  HAL_ERR_UNKNOWN   = 0xee      // unknown error
  
};
typedef  uint32_t HAL_Status;


#endif //_HAL_API_H_

