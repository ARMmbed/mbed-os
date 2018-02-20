/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef ATCAECCCONSTANTS_H
#define ATCAECCCONSTANTS_H

/** Datasheet defined constants */
#define ATCA_ECC_508A_I2C_ADDR          0xC0
#define ATCA_ECC_508A_REVISION          {0x00, 0x00, 0x50, 0x00}
#define ATCA_ECC_WORD_SZ                4
#define ATCA_ECC_STATUS_RESP_LEN        4
#define ATCA_ECC_RESP_STATUS_IDX        1
#define ATCA_ECC_NUM_PRIV_KEY_SLOTS     8
#define ATCA_ECC_ECC_PK_LEN             64
#define ATCA_ECC_CONFIG_ZONE_SZ         128
#define ATCA_KEY_ID_0                   0
#define ATCA_ECC_HASH_256_LEN           32
#define ATCA_ECC_SIG_LEN                64
#define ATCA_ECC_CRC_POLYNOMIAL         0x8005

/** Device functions */
#define ATCA_ECC_FUNC_RST_IO_ADDR       0x00
#define ATCA_ECC_FUNC_SLEEP             0x01
#define ATCA_ECC_FUNC_IDLE              0x02
#define ATCA_ECC_FUNC_COMMAND           0x03


/** EEPROM Zones */
enum ATCAZone
{
    ATCA_ECC_ZONE_CONFIG              = 0x0

    /* Data and OTP zones are not defined here as this driver does not make
     * use of them. */
};

/** Command opcodes */
enum ATCAOpCode
{
    ATCA_ECC_CMD_OPCODE_READ        = 0x02,
    ATCA_ECC_CMD_OPCODE_WRITE       = 0x12,
    ATCA_ECC_CMD_OPCODE_LOCK        = 0x17,
    ATCA_ECC_CMD_OPCODE_GENKEY      = 0x40,
    ATCA_ECC_CMD_OPCODE_NONCE       = 0x16,
    ATCA_ECC_CMD_OPCODE_SIGN        = 0x41,
    ATCA_ECC_CMD_OPCODE_VERIFY      = 0x45,
};

/** Key ID/Slot no. Type */
typedef uint32_t ATCAKeyID;
#define ATCA_ECC_KEY_ID_0               0 
#define ATCA_ECC_KEY_ID_INVALID         0xFFFFFFFF

/** Command and responce length */
#define ATCA_ECC_CMD_OFFSET             2   /* Command offset in Tx buffer */
#define ATCA_ECC_RESP_OFFSET            1   /* Response offset in Rx buffer */
#define ATCA_ECC_CRC_INPUT_OFFSET       1   /* CRC input data offset in Tx buffer */
#define ATCA_ECC_MAX_CMD_LEN            132 /* Bytes device allocates for constructing command */
#define ATCA_ECC_MAX_RESP_LEN           67  /* Bytes device allocates for constructing responce */
#define ATCA_ECC_FUNCTION_LEN           1   /* Device function (Write address) */
#define ATCA_ECC_CMD_IO_WRAPER_LEN      3   /* Count + CRC */
#define ATCA_ECC_CMD_READ_LEN           4
#define ATCA_ECC_RESP_READ_LEN          7 
#define ATCA_ECC_CMD_WRITE_LEN          8
#define ATCA_ECC_RESP_WRITE_LEN         4
#define ATCA_ECC_CMD_WRITE_LEN          8
#define ATCA_ECC_RESP_WRITE_LEN         4
#define ATCA_ECC_CMD_LOCK_LEN           4
#define ATCA_ECC_STATUS_LEN             4
#define ATCA_ECC_CMD_GENKEY_LEN         4
#define ATCA_ECC_RESP_GENKEY_LEN        67
#define ATCA_ECC_CMD_NONCE_LEN          36
#define ATCA_ECC_CMD_SIGN_LEN           4
#define ATCA_ECC_RESP_SIGN_LEN          67
#define ATCA_ECC_CMD_VERIFY_LEN         132

/** EEPROM addresses */
#define ATCA_ECC_CFG_ADDR_REVISION      4
#define ATCA_ECC_CFG_ADDR_SLOT_CFG      20
#define ATCA_ECC_CFG_ADDR_KEY_CFG       96
#define ATCA_ECC_CFG_ADDR_LCK_CFG       87

/** Flags */
#define ATCA_ECC_FLG_READ_SZ_32         0x80

/** Devices Delays */
#define ATCA_ECC_DELAY_TPU_US           100
#define ATCA_ECC_DELAY_TWLO_US          60
#define ATCA_ECC_DELAY_TWHI_US          1500

/** Command execution times */
#define ATCA_ECC_EXEC_TIME_READ_TYP_US      100
#define ATCA_ECC_EXEC_TIME_READ_MAX_US      1000
#define ATCA_ECC_EXEC_TIME_WRITE_TYP_US     7000
#define ATCA_ECC_EXEC_TIME_WRITE_MAX_US     26000
#define ATCA_ECC_EXEC_TIME_LOCK_TYP_US      8000
#define ATCA_ECC_EXEC_TIME_LOCK_MAX_US      32000
#define ATCA_ECC_EXEC_TIME_GENKEY_TYP_US    11000
#define ATCA_ECC_EXEC_TIME_GENKEY_MAX_US    115000
#define ATCA_ECC_EXEC_TIME_NONCE_TYP_US     100
#define ATCA_ECC_EXEC_TIME_NONCE_MAX_US     7000
#define ATCA_ECC_EXEC_TIME_SIGN_TYP_US      42000
#define ATCA_ECC_EXEC_TIME_SIGN_MAX_US      50000
#define ATCA_ECC_EXEC_TIME_VERIFY_TYP_US    38000
#define ATCA_ECC_EXEC_TIME_VERIFY_MAX_US    58000

/** Driver defined constants */
#define ATCA_ECC_I2C_FREQUENCY          10000

#endif /* ATCAECCCONSTANTS_H */
