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

#include "mbed.h"
#include "ATCADevice.h"
#include "ATCAConfig.h"


const uint8_t ATCADevice::expected_revision[] = ATCA_ECC_508A_REVISION;

ATCAError ATCADevice::Init()
{
    /* Send wakeup token */
    ATCAError err = Wakeup();
    if (err != ATCA_SUCCESS)
        return err;

    /* Check revision.
     * Revision check is to make sure that assumptions about default
     * configuration are correct. */
    uint8_t resp[] = {0x00, 0x00, 0x00, 0x00};
    ReadConfig(ATCA_ECC_CFG_ADDR_REVISION, sizeof(resp), resp);
    if (memcmp(resp, expected_revision, sizeof(resp)) != 0)
        return ATCA_ERR_UNSUPPORTED_DEVICE_REVISION;
    return ATCA_SUCCESS;
}

ATCAError ATCADevice::Wakeup()
{
    /* Disconnect I2C */
    plt.Deinit();
    /* Wait for Power up */
    plt.WaitUs(ATCA_ECC_DELAY_TPU_US);
    /* Pull SDA low for Twlo */
    plt.SDALow(ATCA_ECC_DELAY_TWLO_US);
    /* Wait Twhi */
    plt.WaitUs(ATCA_ECC_DELAY_TWHI_US);

    ATCAError err = plt.Init();
    if (err != ATCA_SUCCESS)
        return err;
    uint8_t resp[] = {0x00, 0x00, 0x00, 0x00};
    uint32_t retries = 0;
    do{
        err = plt.Read(resp, sizeof(resp));
        plt.WaitUs(ATCA_ECC_DELAY_WAKE_TOKEN_RETRY_US);
    }while (err != ATCA_SUCCESS && retries++ < comm_retries);
    if (err != ATCA_SUCCESS)
        return err;
    if (resp[1] != (uint8_t)ATCA_ERR_WAKE_TOKEN_RECVD)
        return ATCA_ERR_DEVICE_ERROR;
    return ATCA_SUCCESS;
}

ATCAError ATCADevice::ResetWatchdog()
{
    /* Send Idle command */
    uint8_t func = ATCA_ECC_FUNC_IDLE;
    plt.Write(&func, sizeof(func));
    /* Write status is not checked as device might have already gone in sleep.
     * Send Wakeup token. */
    ATCAError err = Wakeup();
    return err;
}

ATCAError ATCADevice::RunCommand(ATCACmdInfo * info)
{
    ATCAError err = ATCA_ERR_DEVICE_ERROR;
    uint16_t crc = GetCrc16(info->crc_input(), info->crc_input_len());
    uint32_t retries = 0;

    /* Device is little endian */
    info->cmd()[info->cmd_len] = (crc & 0xff);
    info->cmd()[info->cmd_len + 1] = crc >> 8;

    while ( (err != ATCA_SUCCESS) && ( (retries++) < comm_retries))
    {
        err = plt.Write(info->tx_buf, info->cmd_len +
                ATCA_ECC_FUNCTION_LEN + ATCA_ECC_CMD_IO_WRAPER_LEN);
        if (err != ATCA_SUCCESS)
        {
            ResetWatchdog();
            continue;
        }

        /* Wait for execution time */
        plt.WaitUs(info->typical_exec_time);

        /* Trying reading after typical exec time. */
        err = plt.Read(info->rx_buf, info->resp_len);
        if (err != ATCA_SUCCESS)
        {
            /* Retry reading after max execution time */
            plt.WaitUs(info->max_exec_time);
            err = plt.Read(info->rx_buf, info->resp_len);
        }

        /* Reset device if error persists */
        if (err != ATCA_SUCCESS)
        {
            ResetWatchdog();
            continue;
        }
        uint8_t count = info->rx_buf[0];
        if (count > info->resp_len)
        {
            err = ATCA_ERR_SMALL_BUFFER;
            break;
        }

        /* Check for errors */
        if (count != info->resp_len)
        {
            /* Try to recover from errors like watchdog expiry or invalid count */
            err = (ATCAError)info->resp()[0];
            if (err == ATCA_ERR_WAKE_TOKEN_RECVD        ||
                err == ATCA_ERR_COMMUNICATION)
                /* Wake token received or temporary communication error. Retry */
                continue;
            else if (err == ATCA_ERR_WATCHDOG_WILL_EXPIRE)
            {
                if (ResetWatchdog() == ATCA_SUCCESS)
                    continue;
            }
            else if (err == ATCA_SUCCESS)
            {
                /* Response isn't loaded in SRAM.
                 * Retry reading after max execution time */
                plt.WaitUs(info->max_exec_time);
                continue;
            }
        }
    }

    return err;
}

ATCAError ATCADevice::ReadCommand(ATCAZone zone, uint16_t address,
                                  uint8_t * word, size_t wlen)
{
    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_READ, &cmd_info);

    /* Assume word size reads only */
    if (wlen != ATCA_ECC_WORD_SZ)
        return ATCA_ERR_SMALL_BUFFER;

    /* Fill the command */
    cmd[1] = zone;
    cmd[2] = address & 0xFF; /* Device is little endian */
    cmd[3] = (address >> 8) & 0xFF;

    ATCAError err = RunCommand(&cmd_info);
    if (err == ATCA_SUCCESS)
    {
        memcpy(word, cmd_info.resp(), ATCA_ECC_WORD_SZ);
    }
    return err;
}

ATCAError ATCADevice::WriteCommand(ATCAZone zone, uint16_t address,
                                   const uint8_t * word, size_t wlen)
{
    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_WRITE, &cmd_info);

    /* Assume word size reads only */
    if (wlen != ATCA_ECC_WORD_SZ)
        return ATCA_ERR_SMALL_BUFFER;

    /* Fill the command */
    cmd[1] = zone;
    cmd[2] = address & 0xFF;
    cmd[3] = (address >> 8) & 0xFF;
    cmd[4] = word[0];
    cmd[5] = word[1];
    cmd[6] = word[2];
    cmd[7] = word[3];

    ATCAError err = RunCommand(&cmd_info);
    return err;
}

ATCAError ATCADevice::LockCommand(uint8_t * config, size_t config_len)
{
    ATCACmdInfo cmd_info;

    if (config_len != ATCA_ECC_CONFIG_ZONE_SZ)
        return ATCA_ERR_INVALID_PARAM;

    /* Update first 4 blocks of 4 bytes device specific data in the config
     * from the device */
    for (int block = 0; block < ATCA_ECC_NUM_DEVICE_DATA_BLOCKS; block++)
    {
        ATCAError err = ReadCommand(ATCA_ECC_ZONE_CONFIG, block,
                config + block * ATCA_ECC_WORD_SZ, ATCA_ECC_WORD_SZ);
        if (err != ATCA_SUCCESS)
            return err;
    }

    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_LOCK, &cmd_info);
    uint16_t crc = GetCrc16(config, config_len);

    /* Fill the command */
    cmd[1] = 0x00; // Lock config zone and provide config CRC
    cmd[2] = crc & 0xFF; // LSB first.
    cmd[3] = (crc >> 8) & 0xFF;

    ATCAError err = RunCommand(&cmd_info);
    if (err == ATCA_SUCCESS)
        err = (ATCAError)cmd_info.resp()[0];
    return err;
}

ATCAError ATCADevice::GenPrivateKey(ATCAKeyID keyId, uint8_t *pk,
                                    size_t pk_buf_len, size_t * pk_len)
{
    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_GENKEY, &cmd_info);

    if (keyId >= ATCA_ECC_NUM_PRIV_KEY_SLOTS)
        return ATCA_ERR_INVALID_PARAM;
    if ( pk_buf_len < ATCA_ECC_ECC_PK_LEN )
        return ATCA_ERR_SMALL_BUFFER;

    /* Fill the command */
    cmd[1] = 0x04; /* Generate a random ECC Private key in slot keyId. */
    cmd[2] = keyId & 0xFF;
    cmd[3] = (keyId >> 8) & 0xFF;

    ATCAError err = RunCommand(&cmd_info);
    if ( err == ATCA_SUCCESS )
    {
        *pk_len = ATCA_ECC_ECC_PK_LEN;
        memcpy( pk, cmd_info.resp(), ATCA_ECC_ECC_PK_LEN );
    }

    return err;
}

ATCAError ATCADevice::GenPubKey(ATCAKeyID keyId, uint8_t *pk,
                                size_t pk_buf_len, size_t * pk_len)
{
    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_GENKEY, &cmd_info);

    if (keyId >= ATCA_ECC_NUM_PRIV_KEY_SLOTS)
        return ATCA_ERR_INVALID_PARAM;
    if ( pk_buf_len < ATCA_ECC_ECC_PK_LEN )
        return ATCA_ERR_SMALL_BUFFER;

    /* Fill the command */
    cmd[1] = 0x00; /* Generate Public key from the Private key in slot<keyId> */
    cmd[2] = keyId & 0xFF;
    cmd[3] = (keyId >> 8) & 0xFF;

    ATCAError err = RunCommand(&cmd_info);
    if ( err == ATCA_SUCCESS )
    {
        *pk_len = ATCA_ECC_ECC_PK_LEN;
        memcpy( pk, cmd_info.resp(), ATCA_ECC_ECC_PK_LEN );
    }

    return err;
}

ATCAError ATCADevice::Nonce(const uint8_t * message, size_t len)
{
    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_NONCE, &cmd_info);

    if (len != 32)
        return ATCA_ERR_INVALID_PARAM;

    /* Fill the command */
    cmd[1] = 0x03; /* Passthrough mode to load external message in device TempKey */
    cmd[2] = 0x00; /* Param2 is 0 for Passthrough mode. */
    cmd[3] = 0x00;
    memcpy(cmd + 4, message, len);

    ATCAError err = RunCommand(&cmd_info);
    return err;
}

int ATCADevice::Sign(ATCAKeyID keyId, const uint8_t * hash, size_t len,
                           uint8_t * sig, size_t sig_buf_len, size_t * sig_len)
{
    if (len != ATCA_ECC_HASH_256_LEN )
        return (int)ATCA_ERR_INVALID_PARAM;

    if ( sig_buf_len < ATCA_ECC_SIG_LEN )
        return (int)ATCA_ERR_SMALL_BUFFER;

    ATCAError err = Nonce(hash, len);
    if (err != ATCA_SUCCESS)
        return (int)err;

    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_SIGN, &cmd_info);

    /* Fill the command */
    cmd[1] = 0x80; /* Mode indicating external message loaded in TempKey. */
    cmd[2] = keyId & 0xFF;
    cmd[3] = (keyId >> 8) & 0xFF;

    err = RunCommand(&cmd_info);
    if ( err == ATCA_SUCCESS )
    {
        *sig_len = ATCA_ECC_SIG_LEN;
        memcpy( sig, cmd_info.resp(), ATCA_ECC_SIG_LEN );
    }

    return (int)err;
}

int ATCADevice::Verify(uint8_t * pk, size_t pk_len, const uint8_t * sig,
                             size_t sig_len, const uint8_t * msg,
                             size_t msg_len)
{
    if (pk_len != ATCA_ECC_ECC_PK_LEN ||
        sig_len != ATCA_ECC_SIG_LEN ||
        msg_len != ATCA_ECC_HASH_256_LEN )
        return (int)ATCA_ERR_INVALID_PARAM;

    /* Load message in TempKey */
    ATCAError err = Nonce(msg, msg_len);
    if (err != ATCA_SUCCESS)
        return (int)err;

    ATCACmdInfo cmd_info;
    uint8_t * cmd = GetCmdBuffer(ATCA_ECC_CMD_OPCODE_VERIFY, &cmd_info);

    /* Fill the command */
    cmd[1] = 0x02; /* External Public Key mode */
    /* For external mode KeyID param should be encoded identical to KeyType in
     * KeyConfig i.e. contain the curve type = P256 NIST ECC key = 0b100. */
    cmd[2] = 0x04;
    cmd[3] = 0x00;
    memcpy(cmd + 4, sig, 64);
    memcpy(cmd + 4 + 64, pk, 64);

    err = RunCommand(&cmd_info);
    if (err != ATCA_SUCCESS)
    {
        err = (ATCAError)cmd_info.resp()[0];
    }
    return (int)err;
}

ATCAError ATCADevice::ReadConfig(uint8_t byte_address, size_t len, uint8_t * obuf)
{
    /* This function assumes 4 byte reads at word boundary */
    if (byte_address + len >= ATCA_ECC_CONFIG_ZONE_SZ  ||
        len > ATCA_ECC_WORD_SZ ||
        (byte_address % ATCA_ECC_WORD_SZ) != 0)
        return ATCA_ERR_INVALID_PARAM;

    /* Find address on 4 byte word boundary */
    uint8_t address = byte_address / ATCA_ECC_WORD_SZ;

    return ReadCommand(ATCA_ECC_ZONE_CONFIG, address, obuf, len);
}

ATCAError ATCADevice::WriteConfig(uint8_t byte_address, size_t len, uint8_t * data)
{
    /* Find offset from word boundary. */
    uint8_t offset = byte_address % ATCA_ECC_WORD_SZ;
    /* Find address on 4 byte word boundary. */
    uint8_t address = (byte_address - offset) / ATCA_ECC_WORD_SZ;
    uint8_t word[] = {0x00, 0x00, 0x00, 0x00};

    /* This function assumes 4 byte write not exceeding word boundary */
    if ( (byte_address + len >= ATCA_ECC_CONFIG_ZONE_SZ)  ||
        (offset + len > ATCA_ECC_WORD_SZ))
        return ATCA_ERR_INVALID_PARAM;

    ATCAError err = ReadCommand(ATCA_ECC_ZONE_CONFIG, address, word, sizeof(word));
    if (err != ATCA_SUCCESS)
        return err;
    memcpy(word + offset, data, len);
    return WriteCommand(ATCA_ECC_ZONE_CONFIG, address, word, sizeof(word));
}

ATCAError ATCADevice::ConfigPrivKey(ATCAKeyID keyId)
{
    ATCAError err;
    SlotConfig sc;
    sc.SetAsPrivKey();
    sc.EnableGenKey();
    sc.EnableExtMsgSig();

    KeyConfig kc;
    kc.SetPrivate();
    kc.SetECCKeyType();
    kc.EnablePubKeyGen();

    uint16_t config;

    config = sc.Get();
    uint8_t data[2];
    data[0] = config & 0xFF;
    data[1] = config >> 8;
    err = WriteConfig(ATCA_ECC_CFG_ADDR_SLOT_CFG + keyId * 2, sizeof(data), data);
    if (err != ATCA_SUCCESS)
        return err;

    config = kc.Get();
    data[0] = config & 0xFF;
    data[1] = config >> 8;
    err = WriteConfig(ATCA_ECC_CFG_ADDR_KEY_CFG + keyId * 2, sizeof(data), data);
    return err;
}

ATCAError ATCADevice::ConfigPubKey(ATCAKeyID keyId)
{
    ATCAError err;
    SlotConfig sc; /* Slot config for Public key needs no flags set. */

    KeyConfig kc;
    kc.SetECCKeyType();

    uint16_t config;

    config = sc.Get();
    uint8_t data[2];
    data[0] = config & 0xFF;
    data[1] = config >> 8;
    err = WriteConfig(ATCA_ECC_CFG_ADDR_SLOT_CFG + keyId * 2, sizeof(data), data);
    if (err != ATCA_SUCCESS)
        return err;

    config = kc.Get();
    data[0] = config & 0xFF;
    data[1] = config >> 8;
    err = WriteConfig(ATCA_ECC_CFG_ADDR_KEY_CFG + keyId * 2, sizeof(data), data);
    return err;
}

ATCAError ATCADevice::ConfigCertStore()
{
    ATCAError err;
    SlotConfig sc; /* Slot config for Public key needs no flags set. */

    KeyConfig kc;
    kc.SetNonECCKeyType();

    uint16_t config;

    config = sc.Get();
    uint8_t data[2];
    data[0] = config & 0xFF;
    data[1] = config >> 8;
    err = WriteConfig(ATCA_ECC_CFG_ADDR_SLOT_CFG + 8 * 2, sizeof(data), data);
    if (err != ATCA_SUCCESS)
        return err;

    config = kc.Get();
    data[0] = config & 0xFF;
    data[1] = config >> 8;
    err = WriteConfig(ATCA_ECC_CFG_ADDR_KEY_CFG + 8 * 2, sizeof(data), data);
    return err;
}


uint16_t ATCADevice::GetCrc16(const uint8_t *data, uint8_t length)
{
    uint8_t counter;
    uint16_t crc_register = 0;
    uint8_t shift_register;
    uint8_t data_bit, crc_bit;

    for (counter = 0; counter < length; counter++) {
        for (shift_register = 0x01; shift_register > 0x00; shift_register <<= 1) {
            data_bit = (data[counter] & shift_register) ? 1 : 0;
            crc_bit = crc_register >> 15;
            crc_register <<= 1;
            if (data_bit != crc_bit)
                crc_register ^= polynom;
        }
    }
    return crc_register;
}

ATCAError ATCADevice::CheckConfigLock(bool * locked)
{
    uint8_t word[] = {0x00, 0x00, 0x00, 0x00};
    uint8_t offset = ATCA_ECC_CFG_ADDR_LCK_CFG % ATCA_ECC_WORD_SZ;
    uint8_t word_addr = ATCA_ECC_CFG_ADDR_LCK_CFG - offset;
    ATCAError err = ReadConfig( word_addr, sizeof(word), word);
    *locked = word[offset] == 0x00;
    return err;
}

void ATCADevice::DumpConfig()
{
    ATCAError err = ATCA_SUCCESS;
    uint8_t word[ATCA_ECC_WORD_SZ] = {0x00, 0x00, 0x00, 0x00};

    for (int i = 0; i < ATCA_ECC_CONFIG_ZONE_SZ/ATCA_ECC_WORD_SZ; i++)
    {
        err = ReadCommand(ATCA_ECC_ZONE_CONFIG, i, word, sizeof(word));
        if (err != ATCA_SUCCESS)
            printf ("Error reading 0x%x\r\n", err);
        else
            printf ("0x%02x: %02X %02X %02X %02X\r\n", i * ATCA_ECC_WORD_SZ,
                    word[0], word[1], word[2], word[3]);
    }
}

uint8_t * ATCADevice::GetCmdBuffer(ATCAOpCode op, ATCACmdInfo * info)
{
    uint8_t offset = 0;

    info->tx_buf = tx_buf;
    info->rx_buf = rx_buf;
    info->tx_buf[offset++] = ATCA_ECC_FUNC_COMMAND;

    switch (op)
    {
        case ATCA_ECC_CMD_OPCODE_READ:
            info->cmd_len = ATCA_ECC_CMD_READ_LEN;
            info->resp_len = ATCA_ECC_RESP_READ_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_READ_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_READ_MAX_US;
            break;
        case ATCA_ECC_CMD_OPCODE_WRITE:
            info->cmd_len = ATCA_ECC_CMD_WRITE_LEN;
            info->resp_len = ATCA_ECC_RESP_WRITE_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_WRITE_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_WRITE_MAX_US;
            break;
        case ATCA_ECC_CMD_OPCODE_LOCK:
            info->cmd_len = ATCA_ECC_CMD_LOCK_LEN;
            info->resp_len = ATCA_ECC_STATUS_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_LOCK_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_LOCK_MAX_US;
            break;
        case ATCA_ECC_CMD_OPCODE_GENKEY:
            info->cmd_len = ATCA_ECC_CMD_GENKEY_LEN;
            info->resp_len = ATCA_ECC_RESP_GENKEY_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_GENKEY_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_GENKEY_MAX_US;
            break;
        case ATCA_ECC_CMD_OPCODE_NONCE:
            info->cmd_len = ATCA_ECC_CMD_NONCE_LEN;
            info->resp_len = ATCA_ECC_STATUS_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_NONCE_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_NONCE_MAX_US;
            break;
        case ATCA_ECC_CMD_OPCODE_SIGN:
            info->cmd_len = ATCA_ECC_CMD_SIGN_LEN;
            info->resp_len = ATCA_ECC_RESP_SIGN_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_SIGN_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_SIGN_MAX_US;
            break;
        case ATCA_ECC_CMD_OPCODE_VERIFY:
            info->cmd_len = ATCA_ECC_CMD_VERIFY_LEN;
            info->resp_len = ATCA_ECC_STATUS_LEN;
            info->typical_exec_time = ATCA_ECC_EXEC_TIME_VERIFY_TYP_US;
            info->max_exec_time = ATCA_ECC_EXEC_TIME_VERIFY_MAX_US;
            break;
        default:
            assert(0);
    }
    assert(info->cmd_len <= ATCA_ECC_MAX_CMD_LEN);
    assert(info->resp_len <= ATCA_ECC_MAX_RESP_LEN);
    info->tx_buf[offset++] = info->cmd_len + ATCA_ECC_CMD_IO_WRAPER_LEN;
    info->tx_buf[offset] = op;

    return info->cmd();
}

ATCAError ATCADevice::GetKeyToken(ATCAKeyID keyId, ATCAKey *& key)
{
    size_t pk_len = 0;
    uint8_t pk[ATCA_ECC_ECC_PK_LEN];

    ATCAError err = GenPubKey(keyId, pk, sizeof(pk), &pk_len);
    if (err != ATCA_SUCCESS )
    {
        return err;
    }
    if ( pk_len != ATCA_ECC_ECC_PK_LEN )
    {
        return ATCA_ERR_DEVICE_ERROR;
    }

    key = new ATCAKey( *this, keyId, pk );
    if ( key == NULL )
        return ATCA_ERR_MEM_ALLOC_FAILURE;
    return ATCA_SUCCESS;
}

