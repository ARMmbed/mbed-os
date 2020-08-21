/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S1SBP6A_TIMER_H
#define __S1SBP6A_TIMER_H

/****************************************************************************
 * Include
 ****************************************************************************/

#include "s1sbp6a.h"

#define TIMER0_BASE                    (BP_TIMER0_BASE)
#define TIMER_BASE(c)                  (TIMER0_BASE + (c << 12))

#define TIMER_CTRL_OFFSET               0x0000
#define TIMER_VALUE_OFFSET              0x0004
#define TIMER_RELOAD_OFFSET             0x0008
#define TIMER_INTCLR_OFFSET             0x000C
#define TIMER_INT_OFFSET                0x000C
#define TIMER_INT_OFFSET                0x000C
#define TIMER_INT_OFFSET                0x000C
#define TIMER_INT_OFFSET                0x000C

#define DUALTIMER_LOAD_SHIFT           (0)
#define DUALTIMER_VALUE_SHIFT          (0)
#define DUALTIMER_CTRL_EN_SHIFT        (7)
#define DUALTIMER_CTRL_MODE_SHIFT      (6)
#define DUALTIMER_CTRL_INTEN_SHIFT     (5)
#define DUALTIMER_CTRL_PRESCALE_SHIFT  (2)
#define DUALTIMER_CTRL_SIZE_SHIFT      (1)
#define DUALTIMER_CTRL_ONESHOOT_SHIFT  (0)

#define DUALTIMER_LOAD_MASK            (0xFFFFFFFFul << DUALTIMER_LOAD_SHIFT)
#define DUALTIMER_VALUE_MASK           (0xFFFFFFFFul << DUALTIMER_VALUE_SHIFT)
#define DUALTIMER_CTRL_EN_MASK         (0x1ul << DUALTIMER_CTRL_EN_SHIFT)
#define DUALTIMER_CTRL_MODE_MASK       (0x1ul << DUALTIMER_CTRL_MODE_SHIFT)
#define DUALTIMER_CTRL_INTEN_MASK      (0x1ul << DUALTIMER_CTRL_INTEN_SHIFT)
#define DUALTIMER_CTRL_PRESCALE_MASK   (0x3ul << DUALTIMER_CTRL_PRESCALE_SHIFT)
#define DUALTIMER_CTRL_SIZE_MASK       (0x1ul << DUALTIMER_CTRL_SIZE_SHIFT)
#define DUALTIMER_CTRL_ONESHOOT_MASK   (0x1ul << DUALTIMER_CTRL_ONESHOOT_SHIFT)

#define DUALTIMER_LOAD(c)              ((c) << DUALTIMER_LOAD_SHIFT)
#define DUALTIMER_VALUE(c)             ((c) << DUALTIMER_VALUE_SHIFT)
#define DUALTIMER_CTRL_EN(c)           ((c) << DUALTIMER_CTRL_EN_SHIFT)
#define DUALTIMER_CTRL_MODE(c)         ((c) << DUALTIMER_CTRL_MODE_SHIFT)
#define DUALTIMER_CTRL_INTEN(c)        ((c) << DUALTIMER_CTRL_INTEN_SHIFT)
#define DUALTIMER_CTRL_PRESCALE(c)     ((c) << DUALTIMER_CTRL_PRESCALE_SHIFT)
#define DUALTIMER_CTRL_SIZE(c)         ((c) << DUALTIMER_CTRL_SIZE_SHIFT)
#define DUALTIMER_CTRL_ONESHOOT(c)     ((c) << DUALTIMER_CTRL_ONESHOOT_SHIFT)

#define DUALTIMER_INTCLR_SHIFT         (0)
#define DUALTIMER_INTCLR_MASK          (0x01 << DUALTIMER_INTCLR_SHIFT)

#endif //__S1SBP6A_TIMER_H
