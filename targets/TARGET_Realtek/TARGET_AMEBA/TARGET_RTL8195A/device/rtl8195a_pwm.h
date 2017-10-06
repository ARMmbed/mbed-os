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


#ifndef _RTL8195A_PWM_H_
#define _RTL8195A_PWM_H_

extern void
HAL_Pwm_SetDuty_8195a(
    HAL_PWM_ADAPTER *pPwmAdapt,
    u32 period,
    u32 pulse_width
);

extern HAL_Status 
HAL_Pwm_Init_8195a(
    HAL_PWM_ADAPTER *pPwmAdapt
);

extern void 
HAL_Pwm_Enable_8195a(
    HAL_PWM_ADAPTER *pPwmAdapt
);

extern void 
HAL_Pwm_Disable_8195a(
    HAL_PWM_ADAPTER *pPwmAdapt
);


#ifdef CONFIG_CHIP_E_CUT
extern _LONG_CALL_ void
HAL_Pwm_SetDuty_8195a_V04(
    HAL_PWM_ADAPTER *pPwmAdapt,
    u32 period,
    u32 pulse_width
);

extern _LONG_CALL_ HAL_Status 
HAL_Pwm_Init_8195a_V04(
    HAL_PWM_ADAPTER *pPwmAdapt
);

extern _LONG_CALL_ void 
HAL_Pwm_Enable_8195a_V04(
    HAL_PWM_ADAPTER *pPwmAdapt
);

extern _LONG_CALL_ void 
HAL_Pwm_Disable_8195a_V04(
    HAL_PWM_ADAPTER *pPwmAdapt
);
#endif

#endif
