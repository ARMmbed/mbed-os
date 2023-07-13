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
#ifndef _SPI_NOR_H_
#define _SPI_NOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAX_ADDR_LEN   4
#define MAX_ID_LEN     6
#define BUFFER_SIZE    0x132

typedef int (*spi_read_t)(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len);
typedef int (*spi_write_t)(uint8_t *tx_buf, uint32_t tx_len);

struct nor_cmd_packet {
    uint8_t cmd;
    uint8_t addr[MAX_ADDR_LEN];
    uint8_t addr_len;
    uint8_t dummy_len;
    uint8_t *data;
    uint32_t data_len;
    struct {
        uint8_t tx_buf[BUFFER_SIZE];
        uint32_t tx_len;
        uint8_t rx_buf[BUFFER_SIZE];
        uint32_t rx_len;
    } cmd_buffer;
};

struct spi_nor {
    uint8_t id[MAX_ID_LEN];
    uint8_t read_dummy_cycle;
    struct nor_cmd_packet cmd_packet;
    spi_read_t read;
    spi_write_t write;
};

typedef struct nor_cmd_packet nor_cmd_packet_t;
typedef struct spi_nor spi_nor_t;

int32_t spi_read(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len);
int32_t spi_write(uint8_t *tx_buf, uint32_t tx_len);

int32_t spi_nor_init(spi_read_t spi_read, spi_write_t spi_write);
/**
 * \brief Enable software reset.
 *
 * \return: 0 on success, -1 if not
 */
int32_t spi_nor_reset_enable(void);
/**
 * \brief Perform a software reset.
 *
 * \return: 0 on success, -1 if not
 */
int32_t spi_nor_reset(void);
/**
 * \brief Set write enable latch.
 *
 * \return: 0 on success, -1 if not
 */
int32_t spi_nor_write_enable(void);
/**
 * \brief Send write disable instruction to the chip.
 *
 * \return: 0 on success, -1 if not
 */
int32_t spi_nor_write_disable(void);
/**
 * \brief Read data from flash.
 * \param[in] buffer  pointer to destination buffer
 * \param[in] addr    Address to read from
 * \param[in] size    Number of bytes to read
 * \return: number of bytes read successfully, -1 otherwise
 */
int32_t spi_nor_read(uint8_t *buffer, uint32_t addr, uint32_t size);
/**
 * \brief Write data to flash.
 * \param[in] buffer  Pointer to source buffer
 * \param[in] addr    Address to write to
 * \param[in] size    Number of bytes to write
 * \return: number of bytes written successfully, -1 otherwise
 */
int32_t spi_nor_program(uint8_t *buffer, uint32_t addr, uint32_t size);
/**
 * \brief Erase an address range on the nor chip.
 * \param[in] addr  Address to erase
 * \param[in] size  Erase size
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_erase(uint32_t addr, uint32_t size);
/**
 * \brief Read the JEDEC ID.
 * \param[in] id    Pointer to buffer where the value of JEDEC ID will be written
 * \param[in] size  ID size
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_read_id(uint8_t *id, uint32_t size);
/**
 * \brief Read the status register.
 * \param[in] sr    Pointer to buffer where the value of status register will be written
 * \param[in] size  SR size
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_read_sr(uint8_t *sr, uint32_t size);
/**
 * \brief Read the configuration register.
 * \param[in] cr    Pointer to buffer where the value of configuration register will be written
 * \param[in] size  CR size
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_read_cr(uint8_t *cr, uint32_t size);
/**
 * \brief Read the security register.
 * \param[in] scur  Pointer to buffer where the value of security register will be written
 * \param[in] size  SCUR size
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_read_scur(uint8_t *scur, uint32_t size);
/**
 * \brief Enter secured OTP.
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_enter_secured_OTP(void);
/**
 * \brief Exit secured OTP.
 * \return: 0 on success, -1 otherwise
 */
int32_t spi_nor_exit_secured_OTP(void);
/**
 * \brief Polling for nor flash ready for program.
 *
 * \return: 0 if flash is ready for program, -1 otherwise
 */
int32_t spi_nor_polling_for_wr_ready(void);
/**
 * \brief Polling for nor flash ready for read.
 *
 * \return: 0 if flash is ready for read, -1 otherwise
 */
int32_t spi_nor_polling_for_out_ready(void);

#ifdef __cplusplus
}
#endif

#endif /* _SPI_NOR_H_ */
