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

#ifndef ATCADEVICE_H
#define ATCADEVICE_H

#include "ATCAPlatformInterface.h"
#include "ATCAError.h"
#include "ATCAConstants.h"
#include "CryptoEngineInterface.h"
#include "ATCAKey.h"
#include <stdint.h>
#include <stddef.h>

/** Command parameters.
 */
struct ATCACmdInfo
{
    uint32_t    typical_exec_time;  /** Typical execution time */
    uint32_t    max_exec_time;  /** Maximum execution time */
    uint8_t *   tx_buf;         /** Transmit buffer */
    uint8_t *   rx_buf;         /** Receive buffer */
    uint8_t     cmd_len;        /** Command length */
    uint8_t     resp_len;       /** Response length */

    /** Return point in tx_buf where command starts */
    uint8_t * cmd(){ return tx_buf + ATCA_ECC_CMD_OFFSET; }
    /** Return point in tx_buf where crc should start */
    uint8_t * crc_input(){ return tx_buf + ATCA_ECC_CRC_INPUT_OFFSET; }
    /** Returns length of data on which CRC is calculated */
    size_t    crc_input_len(){ return cmd_len + 1; }
    /** Return point in rx_buf where response starts */
    uint8_t * resp(){ return rx_buf + ATCA_ECC_RESP_OFFSET; }
};

/** ATCAxxxx device driver class
 */
class ATCADevice : public CryptoEngineInterface
{
private:
    /** Platform interface for I2C and timer access  */
    ATCAPlatformInterface & plt;
    /** CRC Polynomial for ATCAECC508A */
    uint16_t                polynom;
    /** ATCAECC508A device revision */
    static const uint8_t    expected_revision[];
    /** Tx buffer */
    uint8_t                 tx_buf[ATCA_ECC_MAX_CMD_LEN];
    /** Rx buffer */
    uint8_t                 rx_buf[ATCA_ECC_MAX_RESP_LEN];
    /** No. of retries on communication errors */
    const uint32_t          comm_retries;
public:
    /** Instantiate ATCAECC508A device driver class with platform reference.
     *
     *  Platform reference provides interface with I2C bus and platform
     *  delay functions.
     *
     *  @param plt_in   Platform object reference.
     */
    ATCADevice(ATCAPlatformInterface & plt_in)
        : plt(plt_in), polynom(ATCA_ECC_CRC_POLYNOMIAL), comm_retries(3) {
    }

    virtual ~ATCADevice() {}

    /** Initialise ATCA Device.
     */
    ATCAError Init();

    /** Configure keyID (Slot) as an ECC Private key.
     *
     *  @param keyId    Key Id/Slot number in device data zone.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError ConfigPrivKey(ATCAKeyID keyId);

    /** Configure keyID (Slot) as an ECC Public key.
     *
     *  @param keyId    Key Id/Slot number in device data zone.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError ConfigPubKey(ATCAKeyID keyId);

    /** Configure keyID (Slot) as a data store (Generally as x509 cert).
     *
     *  @return         Error code from enum ATCAError.
     */
    ATCAError ConfigCertStore();

    /** Lock configuration zone. Configuration zone locking is required
     *  for using crypto functions of the device.
     *
     *  \note Locking is irreversible. Once locked device behaviour
     *  CAN NOT be changed forever!
     *      In order to protect against most accidental errors, the device
     *  compares configuration zone data upto a supplied CRC calculated on
     *  the desired config supplied to this function.
     *      The first 16 bytes of the configuration contain the device unique
     *  identifier. The rest does not depend on the device. Hence this function
     *  reads the first 16 bytes from the device, replaces the corresponding
     *  part of the reference configuration, and calculates the CRC from that.
     *
     *  @param config       Desired confiuration zone data.
     *  @param config_len   Configuration data length.
     *  @return             Error code from enum ATCAError.
     */
    ATCAError LockCommand(uint8_t * config, size_t config_len);

    /** Genarate Private key in given slot (Key Id).
     *
     *  @param keyId    Key Id of Private slot.
     *  @param pk       Public key output buffer. It should be at least 64 bytes
     *                  long to store concatenated X & Y components of an ECC
     *                  Public key.
     *  @param pk_buf_len
     *                  Public key output buffer length.
     *  @param pk_len   Public key output length.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError GenPrivateKey(ATCAKeyID keyId, uint8_t *pk, size_t pk_buf_len, size_t * pk_len);

    /** Calculate Public key from corresponding Private Key Id in the device
     *  and output it.
     *
     *  @param keyId    Key Id of Private slot.
     *  @param pk       Public key output buffer. It should be at least 64 bytes
     *                  long to store concatenated X & Y components of an ECC
     *                  Public key.
     *  @param pk_buf_len
     *                  Public key output buffer length.
     *  @param pk_len   Public key output length.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError GenPubKey(ATCAKeyID keyId, uint8_t *pk, size_t pk_buf_len, size_t * pk_len);

    /** Sign message digest(hash) and output ECDSA signature.
     *
     *  @param keyId    Key Id of Private slot.
     *  @param hash     Message digest. ATCAECC508A only accepts SHA256 digests.
     *  @param len      Hash length.
     *  @param sig      Signature output buffer. It should be at least 64 bytes
     *                  long to store concatenated R & S components of an ECDSA
     *                  signature.
     *  @param sig_buf_len
     *                  Signature output buffer length.
     *  @param sig_len  Signature output length.
     *  @return         0 for success else Error code from enum ATCAError.
     */
    virtual int Sign(uint32_t keyId, const uint8_t * hash, size_t len,
                           uint8_t * sig, size_t sig_buf_len, size_t * sig_len);

    /** Verify input signature against input message digest.
     *
     *  @param pk       ECC Public key in form of concatenated X & Y buffer.
     *  @param pk_len   Public key length.
     *  @param sig      Signature buffer.
     *  @param sig_len  Signature buffer length.
     *  @param msg      Message buffer.
     *  @param msg_len  Message buffer length.
     *  @return         0 for success else Error code from enum ATCAError.
     */
    virtual int Verify(uint8_t * pk, size_t pk_len, const uint8_t * sig,
                             size_t sig_len, const uint8_t * msg, size_t msg_len);

    /** Check if configuration zone is locked.
     *
     *  @param locked   Output parameter. True if locked else False.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError CheckConfigLock(bool * locked);

    /** Get a key pair object for a requested Private key slot.
     *  Key pair object of ATCAKey is allocated on heap.
     *  It is callers responsibility to delete the pointer after use.
     *
     *  @param keyId    Key Id of Private slot.
     *  @param key      Reference to ATCAKey pointer.
     *                  Assigned by the function on success.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError GetKeyToken(ATCAKeyID keyId, ATCAKey *& key);

    /** Dump config zone registers on standard output.
     */
    void      DumpConfig();

private:
    /** Perform device wakeup procedure. See datasheet section 6.1.1.
     *
     * Wakeup procedure consists of:
     *  - Wait Tpu for device Up
     *  - Pull SDA low for Twlo
     *  - SDA back high
     *  - Wait Twhi
     *  - Ready. A following read should get status 0x11(Wake token received)
     */
    ATCAError   Wakeup();
    /** Reset watchdog timer.
     *
     *  Device goes to sleep on Twatchdog expiry. This driver uses default
     *  timer value that is 1.3 sec. It is possible that the timer can expire
     *  in the middle of processing a command. On expiry command processing is
     *  halted and command context/data is lost.
     *
     *  In order to ensure completion of a command it is important to reset the
     *  watchdog timer if it is about to expire. Device alerts the driver of
     *  Twatchdog's near exipry by returning err ATCA_ERR_WATCHDOG_WILL_EXPIRE.
     *
     *  This function should be called on receiving ATCA_ERR_WATCHDOG_WILL_EXPIRE
     *  It first puts device in the Idle mode that pauses processing but retains
     *  the data. Then it wakes up the device and processing is resumed.
     */
    ATCAError   ResetWatchdog();

    /** Execute a command.
     *
     *  This function performs command execution based on datasheet section 9.1:
     *      - Constructs command tx buffer based on device IO protocol.
     *      - Transmit tx buffer.
     *      - Receive response.
     *      - Check response status.
     *      - Recover from watchdog expiry or sync failure.
     *      - Returns response and status back to caller.
     *
     *  @param info Command parameters
     *  @return     Error code from enum ATCAError.
     */
    ATCAError   RunCommand(ATCACmdInfo * info);

    /** Execute a read command. This function limits the read data to 4 bytes.
     *
     *  @param zone     Device EEPROM zone.
     *  @param address  Byte address within a zone.
     *  @param word     Output buffer of length bytes.
     *  @param wlen     Output buffer length. Must be 4.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError   ReadCommand(ATCAZone zone, uint16_t address,
                            uint8_t * word, size_t wlen);

    /** Execute a write command. This function limits the write data to be 4 bytes.
     *
     *  @param zone     Device EEPROM zone.
     *  @param address  Byte address within a zone.
     *  @param word     Input buffer of length bytes.
     *  @param wlen     Input buffer length. Must be 4.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError   WriteCommand(ATCAZone zone, uint16_t address,
                             const uint8_t * word, size_t wlen);

    /** Execute a Nonce command.
     *
     *  It loads data into the Device Tempkey buffer that is used as an input
     *  in the subsequent Sign or Verify commands.
     *
     *  @param message  Data(generally message digest) to load in Tempkey.
     *  @param len      Message buffer length.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError   Nonce(const uint8_t * message, size_t len);

    /** Read 4 bytes from configuration zone.
     *
     *  Byte address is required to be 4 byte aligned.
     *
     *  @param address  Byte address in Data zone. Should be 4 byte aligned.
     *  @param len      Output buffer length. Must be 4.
     *  @param obuf     Output buffer of len bytes.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError   ReadConfig(uint8_t byte_address, size_t len, uint8_t * obuf);

    /** Write 4 bytes or less in configuration zone. Writing across 4 byte word
     *  boundary is not allowed. Hence the length must be <= 4.
     *
     *
     *  @param address  Byte address in data zone. May not be 4 byte aligned.
     *  @param len      Input buffer length. Must be <= 4.
     *  @param data     Input buffer of length bytes.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError   WriteConfig(uint8_t byte_address, size_t len, uint8_t * data);

    /** Calculates CRC16 on input data.
     *
     *  Device specified polynomial 0x8005 is used.
     *
     *  @param data     Input buffer.
     *  @param len      Input buffer length.
     *  @return         Error code from enum ATCAError.
     */
    uint16_t    GetCrc16(const uint8_t *data, uint8_t length);

    /** Fill command parameters in input ATCACmdInfo and return pointer to
     *  command.
     *
     *  @param op       Operation code to select parameters.
     *  @param info     Command parameters out pointer.
     *  @return         Pointer to command within tx_buf.
     */
    uint8_t *   GetCmdBuffer(ATCAOpCode op, ATCACmdInfo * info);
};

#endif /* ATCADEVICE_H */
