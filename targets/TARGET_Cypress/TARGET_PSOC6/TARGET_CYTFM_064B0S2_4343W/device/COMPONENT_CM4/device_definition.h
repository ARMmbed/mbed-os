/*
 * Copyright (c) 2017-2018 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file device_definition.h
 * \brief The structure definitions in this file are exported based on the peripheral
 * definitions from device_cfg.h.
 * This retarget file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#ifndef __DEVICE_DEFINITION_H__
#define __DEVICE_DEFINITION_H__

#include "device_cfg.h"


/* ======= Defines peripheral configuration structures ======= */
/* ======= and includes generic driver headers if necessary ======= */
/* CMSDK Timer driver structures */
#ifdef CMSDK_TIMER0_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_S;
#endif
#ifdef CMSDK_TIMER0_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_NS;
#endif

#ifdef CMSDK_TIMER1_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_S;
#endif
#ifdef CMSDK_TIMER1_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_NS;
#endif

#if defined(CY_TCPWM0_TIMER0_S) || defined(CY_TCPWM0_TIMER1_NS)
#include "cy_tcpwm_counter.h"
typedef struct tfm_timer_irq_test_dev {
    bool        is_initialized;
    TCPWM_Type *tcpwm_base;
    uint32_t    tcpwm_counter_num;
    uint32_t    timer_match_value;
    cy_stc_tcpwm_counter_config_t   *tcpwm_config;
} tfm_timer_irq_test_dev_t;
#endif
#ifdef CY_TCPWM0_TIMER0_S
#include "cy_sysint.h"
extern cy_stc_sysint_t CY_TCPWM_NVIC_CFG_S;
extern void TFM_TIMER0_IRQ_Handler(void);
extern tfm_timer_irq_test_dev_t CY_TCPWM0_TIMER0_DEV_S;
#define TIMER0_MATCH (1000000 / 8)  /* About 1 seconds (CM0+: 50MHz) */
#endif
#ifdef CY_TCPWM0_TIMER1_NS
extern tfm_timer_irq_test_dev_t CY_TCPWM0_TIMER1_DEV_NS;
#define TIMER1_MATCH (1000000 / 8)  /* About 2 seconds (CM4: 100MHz) */
#endif

#endif  /* __DEVICE_DEFINITION_H__ */
