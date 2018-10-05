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
 * \file pn512_cmd.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef PN512_CMD_H_
#define PN512_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "pn512.h"

#define PN512_CMD_IDLE        0x00 //No action, cancels current command execution
#define PN512_CMD_MEM         0x01 //Stores 25 bytes into the internal buffer
#define PN512_CMD_CONFIG      0x01 //Configures the PN512 for FeliCa, MIFARE and NFCIP-1 communication
#define PN512_CMD_RNDIDG      0x02 //Generates a 10-byte random ID number
#define PN512_CMD_CRC         0x03 //Activates the CRC coprocessor or performs a self test
#define PN512_CMD_TRANSMIT    0x04 //Transmits data from the FIFO buffer
#define PN512_CMD_NOCHANGE    0x07 //No command change
#define PN512_CMD_RECEIVE     0x08 //Activates the receiver circuits
#define PN512_CMD_TRANSCEIVE  0x0C //Transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
#define PN512_CMD_AUTOCOLL    0x0D //Handles FeliCa polling (Card Operation mode only) and MIFARE anticollision (Card Operation mode only)
#define PN512_CMD_MFAUTH      0x0E //Performs the MIFARE standard authentication as a reader
#define PN512_CMD_SOFTRST     0x0F //Resets the PN512

#define PN512_CMD_REG_MASK    0x0F

void pn512_cmd_init(pn512_t *pPN512);

//Fifo read / write

void pn512_fifo_write(pn512_t *pPN512, ac_buffer_t *pData);
void pn512_fifo_read(pn512_t *pPN512, ac_buffer_builder_t *pData);

//Fifo clear
void pn512_fifo_clear(pn512_t *pPN512);

//Fifo bytes read
size_t pn512_fifo_space(pn512_t *pPN512);
size_t pn512_fifo_length(pn512_t *pPN512);

//Execute command
void pn512_cmd_exec(pn512_t *pPN512, uint8_t cmd);

//Wait for command completion
nfc_err_t pn512_cmd_wait_idle(pn512_t *pPN512, int timeout);

//Read executed command
uint8_t pn512_cmd_get(pn512_t *pPN512);


#ifdef __cplusplus
}
#endif

#endif /* PN512_CMD_H_ */
