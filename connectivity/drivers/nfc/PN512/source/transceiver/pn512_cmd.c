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
 * \file pn512_cmd.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details Format and execute PN512 commands
 * \internal
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "pn512_cmd.c"
#endif
#include "stack/nfc_errors.h"

#include "pn512_cmd.h"

#define PN512_FIFO_SIZE 64

#include "pn512.h"
#include "pn512_registers.h"
#include "pn512_irq.h"
#include "pn512_hw.h"

/** \addtogroup PN512
 *  \internal
 *  @{
 *  \name Commands
 *  @{
 */

/** \internal Initialize underlying pn512_cmd_t structure
 * \param pPN512 pointer to pn512_t structure
 */
void pn512_cmd_init(pn512_t *pPN512)
{
    (void) pPN512;
}

//Fifo read / write
/** \internal Write bytes to FIFO
 * \param pPN512 pointer to pn512_t structure
 * \param pData buffer to write
 */
void pn512_fifo_write(pn512_t *pPN512, ac_buffer_t *pData)
{
    uint8_t fifo_space = pn512_fifo_space(pPN512); //Do not call this fn twice
    size_t len = ac_buffer_reader_readable(pData);
    len = MIN(fifo_space, len);

    pn512_register_switch_page(pPN512, PN512_REG_FIFODATA);
    pn512_hw_write_buffer(pPN512, PN512_REG_FIFODATA, pData, len);
}

/** \internal Read bytes from FIFO
 * \param pPN512 pointer to pn512_t structure
 * \param pData buffer in which to read
 */
void pn512_fifo_read(pn512_t *pPN512, ac_buffer_builder_t *pData)
{
    uint8_t fifo_len = pn512_fifo_length(pPN512); //Do not call this fn twice
    size_t len = ac_buffer_builder_writable(pData);
    len = MIN(fifo_len, len);

    pn512_register_switch_page(pPN512, PN512_REG_FIFODATA);
    pn512_hw_read_buffer(pPN512, PN512_REG_FIFODATA, pData, len);
}

/** \internal Clear FIFO
 * Removes any bytes left in FIFO
 * \param pPN512 pointer to pn512_t structure
 */
void pn512_fifo_clear(pn512_t *pPN512)
{
    pn512_register_write(pPN512, PN512_REG_FIFOLEVEL, 0x80); //Flush FIFO
}

/** \internal Get space in FIFO
 * \param pPN512 pointer to pn512_t structure
 * \return number of bytes that can be written to FIFO
 */
size_t pn512_fifo_space(pn512_t *pPN512)
{
    return PN512_FIFO_SIZE - pn512_register_read(pPN512, PN512_REG_FIFOLEVEL);
}

/** \internal Get FIFO length
 * \param pPN512 pointer to pn512_t structure
 * \return number of bytes that can be read from FIFO
 */
size_t pn512_fifo_length(pn512_t *pPN512)
{
    return pn512_register_read(pPN512, PN512_REG_FIFOLEVEL);
}

/** \internal Execute command
 * \param pPN512 pointer to pn512_t structure
 * \param cmd PN512 command to execute
 */
void pn512_cmd_exec(pn512_t *pPN512, uint8_t cmd)
{
    pn512_register_write(pPN512, PN512_REG_COMMAND, cmd);
}

/** \internal Wait for command completion
 * \param pPN512 pointer to pn512_t structure
 * \param timeout timeout in milliseconds or -1 for blocking mode
 * \return NFC_OK on success or NFC_ERR_TIMEOUT on timeout
 */
nfc_err_t pn512_cmd_wait_idle(pn512_t *pPN512, int timeout)
{
    (void) timeout;
    while (pn512_cmd_get(pPN512) != PN512_CMD_IDLE) {

    }
    return NFC_OK;
}


/** \internal Read executed command
 * \param pPN512 pointer to pn512_t structure
 * \return PN512 command being executed
 */
uint8_t pn512_cmd_get(pn512_t *pPN512)
{
    return pn512_register_read(pPN512, PN512_REG_COMMAND) & PN512_CMD_REG_MASK;
}

/**
 * @}
 * @}
 * */

