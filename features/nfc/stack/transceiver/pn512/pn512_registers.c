
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
 * \file pn512_registers.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details Access to PN512 registers
 */


#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "pn512_registers.c"
#endif
#include "stack/nfc_errors.h"

#include "pn512_registers.h"
#include "pn512_hw.h"
#include "pn512.h"

#define REGISTER_PAGE(x) ((x)>>4)
#define REGISTER_ADDR(x) ((x)&0xF)

/** \addtogroup PN512
 *  \internal
 *  @{
 *  \name Registers
 *  @{
 */

static void pn512_register_switch_page_intl(pn512_t *pPN512, uint8_t page);

/** \internal Initialize underlying pn512_registers_t structure
 * \param pPN512 pointer to pn512_t structure
 */
void pn512_registers_init(pn512_t *pPN512)
{
    pPN512->registers.registers_page = 0;
}

#define PN512_CFG_INIT_LEN 9
static const uint8_t PN512_CFG_INIT_REGS[] = {
    PN512_REG_DIVIEN,
    PN512_REG_MODE,
    PN512_REG_GSNOFF,
    PN512_REG_RFCFG,
    PN512_REG_CWGSP,
    PN512_REG_MIFNFC,
    PN512_REG_FELNFC2,
    PN512_REG_RXSEL,
    PN512_REG_TYPEB
};
static const uint8_t PN512_CFG_INIT_VALS[] = {
    0x80,
    0x3F,
    0xF2,
    0x68,
    0x3F,
    0x62,
    0x80,
    0x84,
    0x00
}; //Timer: For now max prescaler, max reload value

/** \internal Switch to known (0) registers page, reset registers state
 * \param pPN512 pointer to pn512_t structure
 */
void pn512_registers_reset(pn512_t *pPN512)
{
    pn512_register_switch_page_intl(pPN512, 0);
    for (int i = 0; i < PN512_CFG_INIT_LEN; i++) {
        pn512_register_write(pPN512, PN512_CFG_INIT_REGS[i], PN512_CFG_INIT_VALS[i]);
    }
}

/** \internal Write register
 * \param pPN512 pointer to pn512_t structure
 * \param address register address
 * \param data value to write in register
 */
void pn512_register_write(pn512_t *pPN512, uint8_t address, uint8_t data)
{
    NFC_DBG("Write [%02x] << %02x", address, data);
    if (REGISTER_PAGE(address) != pPN512->registers.registers_page) {
        pn512_register_switch_page_intl(pPN512, REGISTER_PAGE(address));
    }
    address = REGISTER_ADDR(address);
    pn512_hw_write(pPN512, address, &data, 1);
}

/** \internal Read register
 * \param pPN512 pointer to pn512_t structure
 * \param address register address
 * \return data value read from register
 */
uint8_t pn512_register_read(pn512_t *pPN512, uint8_t address)
{
    uint8_t data;
    NFC_DBG_BLOCK(
        uint8_t __dbg_addr;
        __dbg_addr = address; //FIXME
    )
    if (REGISTER_PAGE(address) != pPN512->registers.registers_page) {
        pn512_register_switch_page_intl(pPN512, REGISTER_PAGE(address));
    }
    address = REGISTER_ADDR(address);
    pn512_hw_read(pPN512, address, &data, 1);
    NFC_DBG("Read  [%02x] >> %02x", __dbg_addr, data);
    return data;
}

void pn512_register_switch_page(pn512_t *pPN512, uint8_t address)
{
    if (REGISTER_PAGE(address) != pPN512->registers.registers_page) {
        pn512_register_switch_page_intl(pPN512, REGISTER_PAGE(address));
    }
}

/** \internal Switch registers page
 * \param pPN512 pointer to pn512_t structure
 * \param page registers page
 */
void pn512_register_switch_page_intl(pn512_t *pPN512, uint8_t page)
{
    uint8_t pageRegValue;
    pageRegValue = (1 << 7) | page;

    pn512_hw_write(pPN512, PN512_REG_PAGE, &pageRegValue, 1);

    pPN512->registers.registers_page = page;
}



/**
 * @}
 * @}
 * */
