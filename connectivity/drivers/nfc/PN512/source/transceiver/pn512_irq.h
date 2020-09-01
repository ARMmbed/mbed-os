/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
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
/**
 * \file pn512_irq.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef PN512_IRQ_H_
#define PN512_IRQ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "pn512.h"
#include "pn512_registers.h"

#define PN512_IRQ_TX          (1<<6)
#define PN512_IRQ_RX          (1<<5)
#define PN512_IRQ_IDLE        (1<<4)
#define PN512_IRQ_HIGH_ALERT  (1<<3)
#define PN512_IRQ_LOW_ALERT   (1<<2)
#define PN512_IRQ_ERR         (1<<1)
#define PN512_IRQ_TIMER       (1<<0)

#define PN512_IRQ_SIGIN       (1<<(4+8))
#define PN512_IRQ_MODE        (1<<(3+8))
#define PN512_IRQ_CRC         (1<<(2+8))
#define PN512_IRQ_RF_ON       (1<<(1+8))
#define PN512_IRQ_RF_OFF      (1<<(0+8))

#define PN512_IRQ_NONE        0x00
#define PN512_IRQ_ALL         0x1F7F

#define PN512_REG_COMIEN_MASK 0x7F
#define PN512_REG_COMIEN_VAL  0x00

#define PN512_REG_DIVIEN_MASK 0x1F
#define PN512_REG_DIVIEN_VAL  0x80

#define PN512_REG_COMIRQ_MASK 0x7F
#define PN512_REG_COMIRQ_CLEAR  0x00

#define PN512_REG_DIVIRQ_MASK 0x1F
#define PN512_REG_DIVIRQ_CLEAR  0x00

/** \internal Set IRQ enable registers
 * \param pPN512 pointer to pn512_t structure
 * \param irqs MSB is DIVIEN value, LSB is COMIEN value
 */
static inline void pn512_irq_set(pn512_t *pPN512, uint16_t irqs) //ORed
{
    pn512_register_write(pPN512, PN512_REG_COMIEN, PN512_REG_COMIEN_VAL | (PN512_REG_COMIEN_MASK & (irqs & 0xFF)));
    pn512_register_write(pPN512, PN512_REG_DIVIEN, PN512_REG_DIVIEN_VAL | (PN512_REG_DIVIEN_MASK & (irqs >> 8)));
    pPN512->irqsEn = irqs;
}

/** \internal Get IRQ enable registers
 * \param pPN512 pointer to pn512_t structure
 * \return MSB is DIVIEN value, LSB is COMIEN value
 */
static inline uint16_t pn512_irq_enabled(pn512_t *pPN512) //ORed
{
    return pPN512->irqsEn /*(pn512_register_read(pPN512, PN512_REG_COMIEN_VAL) & PN512_REG_COMIEN_MASK)
      | ((pn512_register_read(pPN512, PN512_REG_DIVIEN_VAL) & PN512_REG_DIVIEN_MASK) << 8)*/;
}

/** \internal Get IRQ status registers (masked with enabled IRQ register)
 * \param pPN512 pointer to pn512_t structure
 * \return MSB is DIVIRQ value, LSB is COMIRQ value
 */
static inline uint16_t pn512_irq_get(pn512_t *pPN512) //ORed
{
    return ((pn512_register_read(pPN512, PN512_REG_COMIRQ) & PN512_REG_COMIEN_MASK)
            | ((pn512_register_read(pPN512, PN512_REG_DIVIRQ) & PN512_REG_DIVIEN_MASK) << 8)) & pPN512->irqsEn;
}

/** \internal Clear some interrupts
 * \param pPN512 pointer to pn512_t structure
 * \param irqs MSB is DIVIEN value, LSB is COMIEN value
 */
static inline void pn512_irq_clear(pn512_t *pPN512, uint16_t irqs)
{
    pn512_register_write(pPN512, PN512_REG_COMIRQ, PN512_REG_COMIRQ_CLEAR | (PN512_REG_COMIRQ_MASK & (irqs & 0xFF)));
    pn512_register_write(pPN512, PN512_REG_DIVIRQ, PN512_REG_DIVIRQ_CLEAR | (PN512_REG_DIVIRQ_MASK & (irqs >> 8)));
}

#ifdef __cplusplus
}
#endif

#endif /* PN512_IRQ_H_ */
