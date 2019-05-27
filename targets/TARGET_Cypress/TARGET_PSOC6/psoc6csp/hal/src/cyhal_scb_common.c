/***************************************************************************//**
* \file cyhal_scb_types.h
*
* \brief
* Provides a struct definitions for configuration resources in the SCB (UART, I2C, SPI).
* 
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "cyhal_scb_common.h"

#ifdef CY_IP_MXSCB

CySCB_Type* CY_SCB_BASE_ADDRESSES[CY_IP_MXSCB_INSTANCES] =
{
#ifdef SCB0
    SCB0,
#endif
#ifdef SCB1
    SCB1,
#endif
#ifdef SCB2
    SCB2,
#endif
#ifdef SCB3
    SCB3,
#endif
#ifdef SCB4
    SCB4,
#endif
#ifdef SCB5
    SCB5,
#endif
#ifdef SCB6
    SCB6,
#endif
#ifdef SCB7
    SCB7,
#endif
#ifdef SCB8
    SCB8,
#endif
#ifdef SCB9
    SCB9,
#endif
#ifdef SCB10
    SCB10,
#endif
#ifdef SCB11
    SCB11,
#endif
#ifdef SCB12
    SCB12,
#endif
#ifdef SCB13
    SCB13,
#endif
#ifdef SCB14
    SCB14,
#endif
#ifdef SCB15
    SCB15,
#endif
};

IRQn_Type CY_SCB_IRQ_N[CY_IP_MXSCB_INSTANCES] =
{
#ifdef SCB0
    scb_0_interrupt_IRQn,
#endif
#ifdef SCB1
    scb_1_interrupt_IRQn,
#endif
#ifdef SCB2
    scb_2_interrupt_IRQn,
#endif
#ifdef SCB3
    scb_3_interrupt_IRQn,
#endif
#ifdef SCB4
    scb_4_interrupt_IRQn,
#endif
#ifdef SCB5
    scb_5_interrupt_IRQn,
#endif
#ifdef SCB6
    scb_6_interrupt_IRQn,
#endif
#ifdef SCB7
    scb_7_interrupt_IRQn,
#endif
#ifdef SCB8
    scb_8_interrupt_IRQn,
#endif
#ifdef SCB9
    scb_9_interrupt_IRQn,
#endif
#ifdef SCB10
    scb_10_interrupt_IRQn,
#endif
#ifdef SCB11
    scb_11_interrupt_IRQn,
#endif
#ifdef SCB12
    scb_12_interrupt_IRQn,
#endif
#ifdef SCB13
    scb_13_interrupt_IRQn,
#endif
#ifdef SCB14
    scb_14_interrupt_IRQn,
#endif
#ifdef SCB15
    scb_15_interrupt_IRQn,
#endif
};

#endif /* CY_IP_MXSCB */
