/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "rtc_api.h"
#include "cmsis.h"
#include "W7500x_pwm.h"

time_t wiz_rtc_time;
char rtc_enabled = 0;

#ifdef __cplusplus
extern "C"{
#endif
void PWM3_Handler(void)
{

    wiz_rtc_time++;
    PWM_CH3_ClearOverflowInt();

}
#ifdef __cplusplus
}
#endif



void rtc_init(void) {
    PWM_TimerModeInitTypeDef TimerModeStructure;
    *(volatile uint32_t *)(0x410010e0) = 0x03;

    /* Timer mode configuration */    
    TimerModeStructure.PWM_CHn_PR = 7;
    TimerModeStructure.PWM_CHn_MR = 1;
    TimerModeStructure.PWM_CHn_LR = 0xF4240;
    TimerModeStructure.PWM_CHn_UDMR = PWM_CHn_UDMR_UpCount;
    TimerModeStructure.PWM_CHn_PDMR = PWM_CHn_PDMR_Periodic;
    
    PWM_TimerModeInit(PWM_CH3, &TimerModeStructure); 
    
    /* PWM interrupt configuration */
    PWM_IntConfig(PWM_CH3, ENABLE);
    PWM_CHn_IntConfig(PWM_CH3, PWM_CHn_IER_OIE, ENABLE);
    
    /* PWM channel 0 start */
    PWM_CHn_Start(PWM_CH3);
    NVIC_SetVector(PWM3_IRQn, (uint32_t)PWM3_Handler);
    NVIC_EnableIRQ(PWM3_IRQn);		
    rtc_enabled = 1;
}

void rtc_free(void) {
    // [TODO]
}


int rtc_isenabled(void) {
	return rtc_enabled;
}


time_t rtc_read(void) {
    return wiz_rtc_time;
}

void rtc_write(time_t t) {
    //*(volatile uint32_t *)(0x41001008) = 0x42; // timer disable, interrupt disable
    wiz_rtc_time = t; 
    //*(volatile uint32_t *)(0x41001008) = 0x72; // timer enable interrupt enable  
}
