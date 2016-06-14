/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __CFSTORE_UVISOR_H__
#define __CFSTORE_UVISOR_H__

/* target specifc ACLs */
#if   defined(TARGET_LIKE_FRDM_K64F)

#define CFSTORE_UVISOR_LED_ON  false
#define CFSTORE_UVISOR_LED_OFF true
#define CFSTORE_UVISOR_MAIN_LED LED_BLUE
#define CFSTORE_UVISOR_MAIN_BTN SW2
#define CFSTORE_UVISOR_MAIN_BTN_PUPD PullUp
#define CFSTORE_UVISOR_MAIN_ACL(acl_list_name) \
    static const UvisorBoxAclItem acl_list_name[] = {     \
        {MCG,    sizeof(*MCG),    UVISOR_TACLDEF_PERIPH}, \
        {SIM,    sizeof(*SIM),    UVISOR_TACLDEF_PERIPH}, \
        {PORTB,  sizeof(*PORTB),  UVISOR_TACLDEF_PERIPH}, \
        {PORTC,  sizeof(*PORTC),  UVISOR_TACLDEF_PERIPH}, \
        {RTC,    sizeof(*RTC),    UVISOR_TACLDEF_PERIPH}, \
        {LPTMR0, sizeof(*LPTMR0), UVISOR_TACLDEF_PERIPH}, \
        {PIT,    sizeof(*PIT),    UVISOR_TACLDEF_PERIPH}, \
        {SMC,    sizeof(*SMC),    UVISOR_TACLDEF_PERIPH}, \
        {UART0,  sizeof(*UART0),  UVISOR_TACLDEF_PERIPH}, \
    }

#elif defined(TARGET_LIKE_STM32F429I_DISCO)

#define CFSTORE_UVISOR_LED_ON  false
#define CFSTORE_UVISOR_LED_OFF true
#define CFSTORE_UVISOR_MAIN_LED LED1
#define CFSTORE_UVISOR_MAIN_BTN USER_BUTTON
#define CFSTORE_UVISOR_MAIN_BTN_PUPD PullDown
#define CFSTORE_UVISOR_MAIN_ACL(acl_list_name)                           \
    static const UvisorBoxAclItem acl_list_name[] = {     \
        {TIM2,   sizeof(*TIM2),   UVISOR_TACLDEF_PERIPH}, \
        {TIM5,   sizeof(*TIM5),   UVISOR_TACLDEF_PERIPH}, \
        {GPIOA,  sizeof(*GPIOA),  UVISOR_TACLDEF_PERIPH}, \
        {GPIOG,  sizeof(*GPIOG),  UVISOR_TACLDEF_PERIPH}, \
        /* FIXME: secure RCC/EXTI/SYSCFG/FLASH */         \
        {RCC,    sizeof(*RCC),    UVISOR_TACLDEF_PERIPH}, \
        {EXTI,   sizeof(*EXTI),   UVISOR_TACLDEF_PERIPH}, \
        {SYSCFG, sizeof(*SYSCFG), UVISOR_TACLDEF_PERIPH}, \
        {FLASH,  sizeof(*FLASH),  UVISOR_TACLDEF_PERIPH}, \
        {PWR,    sizeof(*PWR),    UVISOR_TACLDEF_PERIPH}, \
        {USART1, sizeof(*USART1), UVISOR_TACLDEF_PERIPH}, \
        {(void *) 0x42470000, 0x1000, UVISOR_TACLDEF_PERIPH}, \
    }

#elif  defined(TARGET_LIKE_EFM32GG_STK) \
    || defined(TARGET_LIKE_EFM32LG_STK) \
    || defined(TARGET_LIKE_EFM32WG_STK)

#define CFSTORE_UVISOR_LED_ON  false
#define CFSTORE_UVISOR_LED_OFF true
#define CFSTORE_UVISOR_MAIN_LED LED1
#define CFSTORE_UVISOR_MAIN_BTN BTN0
#define CFSTORE_UVISOR_MAIN_BTN_PUPD PullUp
#define CFSTORE_UVISOR_MAIN_ACL(acl_list_name)                              \
    static const UvisorBoxAclItem acl_list_name[] = {        \
        {GPIO,      sizeof(*GPIO),   UVISOR_TACLDEF_PERIPH}, \
        {UART0,     sizeof(*UART0),  UVISOR_TACLDEF_PERIPH}, \
        {TIMER0,    sizeof(*TIMER0), UVISOR_TACLDEF_PERIPH}, \
        /* FIXME: Secure CMU */                              \
        {CMU,       sizeof(*CMU),    UVISOR_TACLDEF_PERIPH}, \
        {RTC,       sizeof(*RTC),    UVISOR_TACLDEF_PERIPH}, \
        /* FIXME: Secure MSC */                              \
        {MSC,       sizeof(*MSC),    UVISOR_TACLDEF_PERIPH}, \
        /* mbed-hal-silabs requires the DI page to be readable */  \
        {(void*) 0x0FE08000, 0x1000, UVISOR_TACLDEF_SECURE_CONST}, \
    }

#elif defined(TARGET_LIKE_EFM32PG_STK)

#define CFSTORE_UVISOR_LED_ON  false
#define CFSTORE_UVISOR_LED_OFF true
#define CFSTORE_UVISOR_MAIN_LED LED1
#define CFSTORE_UVISOR_MAIN_BTN BTN0
#define CFSTORE_UVISOR_MAIN_BTN_PUPD PullUp
#define CFSTORE_UVISOR_MAIN_ACL(acl_list_name)                              \
    static const UvisorBoxAclItem acl_list_name[] = {        \
        {GPIO,      sizeof(*GPIO),   UVISOR_TACLDEF_PERIPH}, \
        {USART0,    sizeof(*USART0), UVISOR_TACLDEF_PERIPH}, \
        {TIMER0,    sizeof(*TIMER0), UVISOR_TACLDEF_PERIPH}, \
        /* FIXME: Secure CMU */                              \
        {CMU,       sizeof(*CMU),    UVISOR_TACLDEF_PERIPH}, \
        {EMU,       sizeof(*EMU),    UVISOR_TACLDEF_PERIPH}, \
        {RTCC,      sizeof(*RTCC),   UVISOR_TACLDEF_PERIPH}, \
        /* FIXME: Secure MSC */                              \
        {MSC,       sizeof(*MSC),    UVISOR_TACLDEF_PERIPH}, \
        /* mbed-hal-silabs requires the DI page to be readable */  \
        {(void*) 0x0FE08000, 0x1000, UVISOR_TACLDEF_SECURE_CONST}, \
    }

#else

#define CFSTORE_UVISOR_LED_ON  true
#define CFSTORE_UVISOR_LED_OFF false
#define CFSTORE_UVISOR_MAIN_LED NC
#define CFSTORE_UVISOR_MAIN_BTN NC
#define CFSTORE_UVISOR_MAIN_BTN_PUPD PullNone
#define CFSTORE_UVISOR_MAIN_ACL(acl_list_name) \
    static const UvisorBoxAclItem acl_list_name[] = {}

#endif

#endif
