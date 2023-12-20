/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
 */
#include <stddef.h>
#include <string.h>
#include "spi_nor.h"

#define STD_INST_PP              0x02
#define STD_INST_READ            0x03
#define STD_INST_ERASE_SECTOR    0x20
#define STD_INST_RDSCUR          0x2B
#define STD_INST_RDCR            0x15
#define STD_INST_RDCR2           0x71
#define STD_INST_RDSR            0x05
#define STD_INST_RDID            0x9F

#define STD_INST_RSTEN           0x66
#define STD_INST_RST             0x99
#define STD_INST_RSTQPI          0xF5

#define STD_INST_WREN            0x06
#define STD_INST_FREAD           0x0B
#define STD_INST_ENSO            0xB1
#define STD_INST_EXSO            0xC1

/* status register definition */
#define  SR_WIP                  0x01
#define  SR_WEL                  0x02
#define  SR_QE                   0x40   /* Quad-IO enable bit */
#define  SR_BP0                  0x04   /* Block protect 0 */
#define  SR_BP1                  0x08   /* Block protect 1 */
#define  SR_BP2                  0x10   /* Block protect 2 */
#define  SR_BP3                  0x20   /* Block protect 3 */
#define  SR_BP_BIT_OFFSET        2      /* Offset to Block protect 0 */
#define  SR_BP_BIT_MASK          (SR_BP3 | SR_BP2 | SR_BP1 | SR_BP0)
#define  SR_SRWD                 0x80   /* SR write protect */

#define  SCUR_ORDY               0x10   /* Security packet output ready bit */

#define SUCCESS                  0
#define FAILURE                  -1
#define BUFFER_SIZE              0x132
#define MAX_POLLING_TIMES        1000

static spi_nor_t spi_nor = {};

static void wait_for_us(uint32_t microsec)
{
    //TODO
}

static void wait_for_ms(uint32_t millisec)
{
    //TODO
}

static void cmd_packing(nor_cmd_packet_t *cmd_packet)
{
    uint32_t send_packet_size = 0;

    cmd_packet->cmd_buffer.tx_buf[0] = cmd_packet->cmd;
    send_packet_size++;
    if (cmd_packet->addr_len > 0) {
        memcpy(&cmd_packet->cmd_buffer.tx_buf[send_packet_size], cmd_packet->addr, cmd_packet->addr_len);
        send_packet_size += cmd_packet->addr_len;
    }
    if (cmd_packet->dummy_len > 0) {
        memset(&cmd_packet->cmd_buffer.tx_buf[send_packet_size], 0xFF, cmd_packet->dummy_len);
        send_packet_size += cmd_packet->dummy_len;
    }
    if (cmd_packet->data != NULL) {
        memcpy(&cmd_packet->cmd_buffer.tx_buf[send_packet_size], cmd_packet->data, cmd_packet->data_len);
        send_packet_size += cmd_packet->data_len;
    }    
    cmd_packet->cmd_buffer.tx_len = send_packet_size;    
}

int32_t spi_read(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len)
{
    spi_nor.read(tx_buf, tx_len, rx_buf, rx_len);    
    return 0;
}

int32_t spi_write(uint8_t *tx_buf, uint32_t tx_len)    
{
    spi_nor.write(tx_buf, tx_len);
    return 0;   
}

int32_t spi_nor_reset_enable(void)
{    
    spi_nor.cmd_packet.cmd = STD_INST_RSTEN;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);    
    return spi_nor.write(spi_nor.cmd_packet.cmd_buffer.tx_buf, spi_nor.cmd_packet.cmd_buffer.tx_len);    
}

int32_t spi_nor_reset(void)
{    
    spi_nor.cmd_packet.cmd = STD_INST_RST;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);    
    return spi_nor.write(spi_nor.cmd_packet.cmd_buffer.tx_buf, spi_nor.cmd_packet.cmd_buffer.tx_len);
    
}

int32_t spi_nor_write_enable(void)
{    
    spi_nor.cmd_packet.cmd = STD_INST_WREN;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);
    return spi_nor.write(spi_nor.cmd_packet.cmd_buffer.tx_buf, spi_nor.cmd_packet.cmd_buffer.tx_len);
}

int32_t spi_nor_write_disable(void)
{
    //TODO
    return 0;
}

int32_t spi_nor_read(uint8_t *buffer, uint32_t addr, uint32_t size)
{
    //TODO
    return 0;
}

int32_t spi_nor_program(uint8_t *buffer, uint32_t addr, uint32_t size)
{
    //TODO
    return 0;
}

int32_t spi_nor_erase(uint32_t addr, uint32_t size)
{
    //TODO
    return 0;
}

int32_t spi_nor_read_id(uint8_t *id, uint32_t size)
{
    spi_nor.cmd_packet.cmd = STD_INST_RDID;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);
    return spi_nor.read(spi_nor.cmd_packet.cmd_buffer.tx_buf,
                    spi_nor.cmd_packet.cmd_buffer.tx_len,
                    id, size);
}

int32_t spi_nor_read_sr(uint8_t *sr, uint32_t size)
{    
    spi_nor.cmd_packet.cmd = STD_INST_RDSR;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);    
    return spi_nor.read(spi_nor.cmd_packet.cmd_buffer.tx_buf,
                    spi_nor.cmd_packet.cmd_buffer.tx_len,
                    sr, size);
}

int32_t spi_nor_read_cr(uint8_t *cr, uint32_t size)
{
    spi_nor.cmd_packet.cmd = STD_INST_RDCR;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);
    return spi_nor.read(spi_nor.cmd_packet.cmd_buffer.tx_buf,
                    spi_nor.cmd_packet.cmd_buffer.tx_len,
                    cr, size);
}

int32_t spi_nor_read_cr2(uint8_t *cr, uint32_t size)
{
    //TODO
    return 0;
}

int32_t spi_nor_read_scur(uint8_t *scur, uint32_t size)
{
    spi_nor.cmd_packet.cmd = STD_INST_RDSCUR;
    spi_nor.cmd_packet.addr_len = 0;
    spi_nor.cmd_packet.dummy_len = 0;
    spi_nor.cmd_packet.data = NULL;
    cmd_packing(&spi_nor.cmd_packet);
    return spi_nor.read(spi_nor.cmd_packet.cmd_buffer.tx_buf,
                    spi_nor.cmd_packet.cmd_buffer.tx_len,
                    scur, size);
}

int32_t spi_nor_enter_secured_OTP(void)
{
    //TODO
    return 0;
}

int32_t spi_nor_exit_secured_OTP(void)
{
    //TODO
    return 0;
}

int32_t spi_nor_software_reset(void)
{
    if (spi_nor_reset_enable() != SUCCESS) {
        return -1;
    }
    if (spi_nor_reset() != SUCCESS) {
        return -1;
    }
    wait_for_ms(1);//stm32l562 hal driver not support us delay
    return 0;
}

int32_t spi_nor_polling_for_wr_ready(void)
{
    uint32_t cnt = 0;
    uint8_t status_reg;
    /* Polling for secure Flash ready for program */    
    do {
        cnt++;
        if (spi_nor_read_sr(&status_reg, 1) != SUCCESS) {            
            return -1;
        }
        wait_for_ms(1);        
    } while ((status_reg & SR_WIP) && (cnt < MAX_POLLING_TIMES));

    if (cnt >= MAX_POLLING_TIMES) {        
        return -1;
    }
    return 0;
}
int32_t spi_nor_polling_for_out_ready(void)
{
    uint32_t cnt = 0;
    uint8_t status_reg, scur_reg;
    
    /* Polling for secure Flash ready for program */
    do {
        cnt++;        
        if (spi_nor_read_scur(&scur_reg, 1) != SUCCESS) {
            return -1;
        }        
        if (spi_nor_read_sr(&status_reg, 1) != SUCCESS) {
            return -1;
        }        
        wait_for_ms(1);
    } while (((status_reg & SR_WIP) || !(scur_reg & SCUR_ORDY)) && (cnt < MAX_POLLING_TIMES));

    if (cnt >= MAX_POLLING_TIMES) {        
        return -1;
    }    
    return 0;
}

int32_t spi_nor_init(spi_read_t spi_read, spi_write_t spi_write)
{
    if (spi_read == NULL || spi_write == NULL) {
        printf("ERROR: spi_ctx == NULL(%08X, %08X)\r\n", spi_read, spi_write);
    }
    spi_nor.read = spi_read;
    spi_nor.write = spi_write;    
    
    return 0;
}
