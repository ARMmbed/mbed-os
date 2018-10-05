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

#ifndef _RTL8195A_WDT_H_
#define _RTL8195A_WDT_H_

#define WDGTIMERELY  (10*1024)  //us

typedef struct _WDG_REG_ {
    u16     WdgScalar;
    u8      WdgEnByte;
    u8      WdgClear:1;
    u8      WdgCunLimit:4;
    u8      Rsvd:1;
    u8      WdgMode:1;
    u8      WdgToISR:1;
}WDG_REG, *PWDG_REG;

typedef struct _WDG_ADAPTER_ {

    WDG_REG             Ctrl;
    IRQ_HANDLE          IrqHandle;
    TIMER_ADAPTER       WdgGTimer;
    VOID (*UserCallback)(u32 callback_id);    // User callback function
    u32                 callback_id;
}WDG_ADAPTER, *PWDG_ADAPTER;

enum _WDG_CNTLMT_ {
    CNT1H    = 0,
    CNT3H    = 1,
    CNT7H    = 2,
    CNTFH    = 3,
    CNT1FH   = 4,
    CNT3FH   = 5,
    CNT7FH   = 6,
    CNTFFH   = 7,
    CNT1FFH  = 8,
    CNT3FFH  = 9,
    CNT7FFH  = 10,
    CNTFFFH  = 11
};
typedef uint32_t WDG_CNTLMT;
typedef uint32_t *PWDG_CNTLMT;


enum _WDG_MODE_ {
    INT_MODE    = 0,
    RESET_MODE  = 1
};
typedef uint32_t WDG_MODE;
typedef uint32_t *PWDG_MODE;

extern VOID
WDGInitial(
    IN  u32 Period
);

extern VOID
WDGIrqInitial(
    VOID
);

extern VOID
WDGIrqInitial(
    VOID
);

extern VOID
WDGStop(
    VOID
);

extern VOID
WDGRefresh(
    VOID
);

extern VOID
WDGIrqCallBackReg(
    IN VOID *CallBack,
    IN u32   Id
);

#endif //_RTL8195A_WDT_H_
