/*
 * Copyright (c) 2014-2018, ARM Limited, All Rights Reserved
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
 * \file pn512_transceive.c
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "pn512_transceive.c"
#endif

#include "stack/nfc_errors.h"

#include "pn512.h"
#include "pn512_transceive.h"
#include "pn512_rf.h"
#include "pn512_irq.h"
#include "pn512_cmd.h"
#include "pn512_registers.h"
#include "pn512_internal.h"


#define TIMEOUT 1000

void pn512_transceive_hw_tx_iteration(pn512_t *pPN512, bool start)
{
    uint16_t irqs_en = pn512_irq_enabled(pPN512);

    if (ac_buffer_reader_readable(&pPN512->writeBuf) > 0) {
        //Fill FIFO
        pn512_fifo_write(pPN512, &pPN512->writeBuf);

        if (ac_buffer_reader_readable(&pPN512->writeBuf) > 0) { //Did not fit in FIFO
            pn512_irq_clear(pPN512, PN512_IRQ_LOW_ALERT);
            //Has low FIFO alert IRQ already been enabled?
            if (!(irqs_en & PN512_IRQ_LOW_ALERT)) {
                irqs_en |= PN512_IRQ_LOW_ALERT;
                pn512_irq_set(pPN512, irqs_en);
            }
        } else {
            if (irqs_en & PN512_IRQ_LOW_ALERT) {
                //Buffer has been fully sent
                irqs_en &= ~PN512_IRQ_LOW_ALERT;
                pn512_irq_set(pPN512, irqs_en);
            }
        }
    }


    if (start) {
        if ((pPN512->transceive.mode == pn512_transceive_mode_transmit) || (pPN512->transceive.mode == pn512_transceive_mode_transmit_and_target_autocoll)) {
            //Update bitframing register
            pn512_register_write(pPN512, PN512_REG_BITFRAMING,
                                 0x00 | ((pPN512->readFirstByteAlign & 0x7) << 4) | (pPN512->writeLastByteLength & 0x7));

            //Use transmit command
            pn512_cmd_exec(pPN512, PN512_CMD_TRANSMIT);
        } else {
            NFC_DBG("Bitframing %02X", 0x80 | ((pPN512->readFirstByteAlign & 0x7) << 4) | (pPN512->writeLastByteLength & 0x7));
            //Update bitframing register to start transmission
            pn512_register_write(pPN512, PN512_REG_BITFRAMING,
                                 0x80 | ((pPN512->readFirstByteAlign & 0x7) << 4) | (pPN512->writeLastByteLength & 0x7));
        }

        //Reset last byte length, first byte align
        pPN512->writeLastByteLength = 8;
        pPN512->readFirstByteAlign = 0;
    }

    //Queue task to process IRQ
    task_init(&pPN512->transceiver.task, EVENT_HW_INTERRUPT | EVENT_TIMEOUT, TIMEOUT, pn512_transceive_hw_tx_task, pPN512);
    nfc_scheduler_queue_task(&pPN512->transceiver.scheduler, &pPN512->transceiver.task);
}

void pn512_transceive_hw_tx_task(uint32_t events, void *pUserData)
{
    pn512_t *pPN512 = (pn512_t *) pUserData;

    if (events & EVENT_ABORTED) {
        //Stop command
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        NFC_ERR("Aborted TX");

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);

        pn512_transceive_callback(pPN512, NFC_ERR_ABORTED);
        return;
    }

    NFC_DBG("TX task");
    if (events & EVENT_TIMEOUT) {
        // Check status
        NFC_DBG("Status = %02X %02X", pn512_register_read(pPN512, PN512_REG_STATUS1), pn512_register_read(pPN512, PN512_REG_STATUS2));

        //Stop command
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        NFC_ERR("Timeout on TX");

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);

        //Call callback
        pn512_transceive_callback(pPN512, NFC_ERR_TIMEOUT);
        return;
    }

    uint16_t irqs_en = pn512_irq_enabled(pPN512);
    uint16_t irqs = pn512_irq_get(pPN512);

    if (irqs & PN512_IRQ_RF_OFF) {
        //Stop command
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);
        NFC_WARN("RF Off");
        pn512_transceive_callback(pPN512, NFC_ERR_FIELD);
        return;
    }
    if (irqs & PN512_IRQ_TX) {
        if (irqs_en & PN512_IRQ_LOW_ALERT) {
            //If the transmission has been completed without us getting a chance to fill the buffer up it means that we had a buffer underflow
            NFC_ERR("Buffer underflow");
            pn512_irq_set(pPN512, PN512_IRQ_NONE);
            pn512_irq_clear(pPN512, PN512_IRQ_ALL);

            pn512_transceive_callback(pPN512, NFC_ERR_UNDERFLOW);
            return;
        }

        //Transmission complete
        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_TX | PN512_IRQ_LOW_ALERT);

        //Start receiving
        NFC_DBG("Transmission complete");
        if (pPN512->transceive.mode != pn512_transceive_mode_transmit) {
            if (pPN512->transceive.mode == pn512_transceive_mode_transmit_and_target_autocoll) {
                //Make sure bitframing reg is clean
                pn512_register_write(pPN512, PN512_REG_BITFRAMING, 0x00);

                pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
                pn512_transceive_hw_rx_start(pPN512);

                //Start autocoll
                pn512_cmd_exec(pPN512, PN512_CMD_AUTOCOLL);
            } else {
                pn512_transceive_hw_rx_start(pPN512);
            }
            return;
        } else {
            pn512_irq_set(pPN512, PN512_IRQ_NONE);
            pn512_irq_clear(pPN512, PN512_IRQ_RX | PN512_IRQ_HIGH_ALERT);

            pn512_transceive_callback(pPN512, NFC_OK);
            return;
        }
    }
    if ((irqs & PN512_IRQ_LOW_ALERT) && (ac_buffer_reader_readable(&pPN512->writeBuf) > 0)) {
        //Continue to fill FIFO
        pn512_irq_clear(pPN512, PN512_IRQ_LOW_ALERT);

        pn512_transceive_hw_tx_iteration(pPN512, false);
        return;
    }

    if (irqs & PN512_IRQ_IDLE) {
        pn512_irq_clear(pPN512, PN512_IRQ_ERR);

        NFC_ERR("Modem went to idle");

        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);
        pn512_transceive_callback(pPN512, NFC_ERR_WRONG_COMM);
        return;
    }

    //Call back function
    pn512_transceive_hw_tx_iteration(pPN512, false);
}

void pn512_transceive_hw_rx_start(pn512_t *pPN512)
{
    uint16_t irqs_en = PN512_IRQ_RX | PN512_IRQ_HIGH_ALERT | PN512_IRQ_ERR;
    if (PN512_FRAMING_IS_TARGET(pPN512->framing)) {
        irqs_en |= PN512_IRQ_RF_OFF;
    }

    pn512_irq_set(pPN512, irqs_en);

    //Reset buffer except if data should be appended to this -- TODO
    ac_buffer_builder_reset(&pPN512->readBufBldr);

    //Queue task to process IRQ
    task_init(&pPN512->transceiver.task, EVENT_HW_INTERRUPT | EVENT_TIMEOUT,
              pPN512->timeout, pn512_transceive_hw_rx_task, pPN512);
    nfc_scheduler_queue_task(&pPN512->transceiver.scheduler,
                             &pPN512->transceiver.task);
}

void pn512_transceive_hw_rx_task(uint32_t events, void *pUserData)
{
    pn512_t *pPN512 = (pn512_t *) pUserData;

    NFC_DBG("RX task");
    if (events & EVENT_ABORTED) {
        //Stop command
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        NFC_ERR("Aborted RX");

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);

        pn512_transceive_callback(pPN512, NFC_ERR_ABORTED);
        return;
    }

    if (events & EVENT_TIMEOUT) {
        NFC_WARN("Timeout");
        //Stop command
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);

        //Call callback
        pn512_transceive_callback(pPN512, NFC_ERR_TIMEOUT);
        return;
    }

    uint16_t irqs = pn512_irq_get(pPN512);
    NFC_DBG("irqs %04x", irqs);
    bool collision_detected = false;
    if (irqs & PN512_IRQ_ERR) {
        pn512_irq_clear(pPN512, PN512_IRQ_ERR);

        uint8_t err_reg = pn512_register_read(pPN512, PN512_REG_ERROR);
        NFC_ERR("Got error - error reg is %02X", err_reg);
        // if err_reg == 0, sticky error that must have been cleared automatically, continue
        if (err_reg != 0) {
            //If it's a collsision, flag it but still carry on with RX procedure
            collision_detected = true;

            if ((err_reg == 0x08) || (err_reg == 0x0A)) { // Collision (and maybe parity) (and no other error)
                irqs &= ~PN512_IRQ_ERR;
                irqs |= PN512_IRQ_RX;
            } else {
                NFC_DBG_BLOCK(
                    //Empty FIFO into buffer
                    pn512_fifo_read(pPN512, &pPN512->readBufBldr);

                    NFC_DBG("Received");
                    ac_buffer_dump(ac_buffer_builder_buffer(&pPN512->readBufBldr));

                    NFC_DBG("Computed CRC = %02X %02X", pn512_register_read(pPN512, PN512_REG_CRCRESULT_MSB), pn512_register_read(pPN512, PN512_REG_CRCRESULT_LSB));

                )

                //Stop command
                pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
                pPN512->transceive.mode = pn512_transceive_mode_idle;

                pn512_irq_set(pPN512, PN512_IRQ_NONE);
                pn512_irq_clear(pPN512, PN512_IRQ_ALL);

                //Call callback
                pn512_transceive_callback(pPN512, NFC_ERR_WRONG_COMM);
                return;
            }
        }
    }
    if ((irqs & PN512_IRQ_RX) || (irqs & PN512_IRQ_HIGH_ALERT)) {
        //Empty FIFO into buffer
        pn512_fifo_read(pPN512, &pPN512->readBufBldr);

        if ((ac_buffer_builder_writable(&pPN512->readBufBldr) == 0) && (pn512_fifo_length(pPN512) > 0)) {
            //Stop command
            pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
            pPN512->transceive.mode = pn512_transceive_mode_idle;

            NFC_WARN("RX buffer overflow");

            pn512_irq_set(pPN512, PN512_IRQ_NONE);
            pn512_irq_clear(pPN512, PN512_IRQ_ALL);
            //Call callback
            pn512_transceive_callback(pPN512, NFC_ERR_BUFFER_TOO_SMALL);
            return; //overflow
        }

        if (irqs & PN512_IRQ_HIGH_ALERT) {
            NFC_DBG("High alert");
            pn512_irq_clear(pPN512, PN512_IRQ_HIGH_ALERT);
        }

        if (irqs & PN512_IRQ_RX) {
            pn512_irq_clear(pPN512, PN512_IRQ_RX);

            size_t last_byte_length = pn512_register_read(pPN512, PN512_REG_CONTROL) & 0x7;
            if (last_byte_length == 0) {
                last_byte_length = 8;
            }
            pPN512->readLastByteLength = last_byte_length;

            pn512_irq_set(pPN512, PN512_IRQ_NONE);
            pn512_irq_clear(pPN512, PN512_IRQ_RX | PN512_IRQ_HIGH_ALERT);

            NFC_DBG("Received:");
            NFC_DBG_BLOCK(ac_buffer_dump(ac_buffer_builder_buffer(&pPN512->readBufBldr));)

            if ((pPN512->transceive.mode == pn512_transceive_mode_target_autocoll) || (pPN512->transceive.mode == pn512_transceive_mode_transmit_and_target_autocoll)) {
                //Check if target was activated
                if (!(pn512_register_read(pPN512, PN512_REG_STATUS2) & 0x10)) {
                    pPN512->transceive.mode = pn512_transceive_mode_idle;

                    pn512_irq_set(pPN512, PN512_IRQ_NONE);
                    pn512_irq_clear(pPN512, PN512_IRQ_ALL);
                    //Call callback
                    pn512_transceive_callback(pPN512, NFC_ERR_PROTOCOL);
                    return;
                }
                //PN512 switches to transceive automatically
                pPN512->transceive.mode = pn512_transceive_mode_transceive;
            } else if (pPN512->transceive.mode == pn512_transceive_mode_receive) {
                pPN512->transceive.mode = pn512_transceive_mode_transceive;
                //pn512_cmd_exec(pPN512, PN512_CMD_IDLE); //Useful?
            }

            if (!collision_detected) {
                pn512_transceive_callback(pPN512, NFC_OK);
            } else {
                pn512_transceive_callback(pPN512, NFC_ERR_COLLISION);
            }

            return;
        }
    }
    if (irqs & PN512_IRQ_RF_OFF) {
        //Stop command
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pPN512->transceive.mode = pn512_transceive_mode_idle;

        pn512_irq_set(pPN512, PN512_IRQ_NONE);
        pn512_irq_clear(pPN512, PN512_IRQ_ALL);

        //Call callback
        pn512_transceive_callback(pPN512, NFC_ERR_FIELD);
        return;
    }

    //Queue task to process IRQ
    task_init(&pPN512->transceiver.task, EVENT_HW_INTERRUPT | EVENT_TIMEOUT,
              pPN512->timeout, pn512_transceive_hw_rx_task, pPN512);
    nfc_scheduler_queue_task(&pPN512->transceiver.scheduler,
                             &pPN512->transceiver.task);
}

void pn512_transceive_hw(pn512_t *pPN512, pn512_transceive_mode_t mode, pn512_cb_t cb)
{
    uint16_t irqs_en;

    //Store callback
    pPN512->transceive.cb = cb;

    //Clear FIFO
    pn512_fifo_clear(pPN512);

    //Clear previous IRQs if present
    pn512_irq_clear(pPN512, PN512_IRQ_RX | PN512_IRQ_TX | PN512_IRQ_HIGH_ALERT | PN512_IRQ_LOW_ALERT | PN512_IRQ_ERR | PN512_IRQ_IDLE | PN512_IRQ_RF_OFF);

    if (PN512_FRAMING_IS_TARGET(pPN512->framing)) {
        //RF off?
        if (!(pn512_register_read(pPN512, PN512_REG_STATUS1) & 0x04)) {
            //Call callback
            pn512_transceive_callback(pPN512, NFC_ERR_FIELD);
            return;
        }
    } else if ((pPN512->transceive.mode != mode) && (mode == pn512_transceive_mode_transceive)) {
        pn512_cmd_exec(pPN512, PN512_CMD_TRANSCEIVE);
    }

    pPN512->transceive.mode = mode;

    if (mode == pn512_transceive_mode_receive) {
        pn512_cmd_exec(pPN512, PN512_CMD_IDLE);
        pn512_transceive_hw_rx_start(pPN512);
        pn512_cmd_exec(pPN512, PN512_CMD_TRANSCEIVE);
    } else if (mode == pn512_transceive_mode_target_autocoll) {
        //Make sure bitframing reg is clean
        pn512_register_write(pPN512, PN512_REG_BITFRAMING, 0x00);

        pn512_transceive_hw_rx_start(pPN512);

        //Start autocoll
        pn512_cmd_exec(pPN512, PN512_CMD_AUTOCOLL);
        return;
    } else {
        NFC_DBG("Sending:");
        NFC_DBG_BLOCK(ac_buffer_dump(&pPN512->writeBuf);)

        //Transmit a frame to remote target/initiator
        irqs_en = PN512_IRQ_TX | PN512_IRQ_IDLE;
        if (PN512_FRAMING_IS_TARGET(pPN512->framing)) {
            irqs_en |= PN512_IRQ_RF_OFF;
        }

        pn512_irq_set(pPN512, irqs_en);

        pn512_transceive_hw_tx_iteration(pPN512, true);
    }
}



