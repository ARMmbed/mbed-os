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

#ifndef _HAL_VECTOR_TABLE_H_
#define _HAL_VECTOR_TABLE_H_

extern _LONG_CALL_ROM_ VOID
VectorTableInitRtl8195A(
    IN  u32 StackP
);

extern _LONG_CALL_ROM_ VOID
VectorTableInitForOSRtl8195A(
    IN  VOID *PortSVC,
    IN  VOID *PortPendSVH,
    IN  VOID *PortSysTick
);

extern _LONG_CALL_ROM_ BOOL
VectorIrqRegisterRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
);

extern _LONG_CALL_ROM_ BOOL
VectorIrqUnRegisterRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
);

extern _LONG_CALL_ROM_ VOID
VectorIrqEnRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
);

extern _LONG_CALL_ROM_ VOID
VectorIrqDisRtl8195A(
    IN  PIRQ_HANDLE pIrqHandle
);

extern _LONG_CALL_ROM_ VOID
HalPeripheralIntrHandle(VOID);

#endif //_HAL_VECTOR_TABLE_H_

