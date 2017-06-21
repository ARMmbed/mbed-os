/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* Introduction
 * ------------
 * SD and MMC cards support a number of interfaces, but common to them all
 * is one based on SPI. This is the one I'm implmenting because it means
 * it is much more portable even though not so performant, and we already
 * have the mbed SPI Interface!
 *
 * The main reference I'm using is Chapter 7, "SPI Mode" of:
 *  http://www.sdcard.org/developers/tech/sdcard/pls/Simplified_Physical_Layer_Spec.pdf
 *
 * SPI Startup
 * -----------
 * The SD card powers up in SD mode. The start-up procedure is complicated
 * by the requirement to support older SDCards in a backwards compatible
 * way with the new higher capacity variants SDHC and SDHC.
 *
 * The following figures from the specification with associated text describe
 * the SPI mode initialisation process:
 *  - Figure 7-1: SD Memory Card State Diagram (SPI mode)
 *  - Figure 7-2: SPI Mode Initialization Flow
 *
 * Firstly, a low initial clock should be selected (in the range of 100-
 * 400kHZ). After initialisation has been completed, the switch to a
 * higher clock speed can be made (e.g. 1MHz). Newer cards will support
 * higher speeds than the default _transfer_sck defined here.
 *
 * Next, note the following from the SDCard specification (note to
 * Figure 7-1):
 *
 *  In any of the cases CMD1 is not recommended because it may be difficult for the host
 *  to distinguish between MultiMediaCard and SD Memory Card
 *
 * Hence CMD1 is not used for the initialisation sequence.
 *
 * The SPI interface mode is selected by asserting CS low and sending the
 * reset command (CMD0). The card will respond with a (R1) response.
 * In practice many cards initially respond with 0xff or invalid data
 * which is ignored. Data is read until a valid response is received
 * or the number of re-reads has exceeded a maximim count. If a valid
 * response is not received then the CMD0 can be retried. This
 * has been found to successfully initialise cards where the SPI master
 * (on MCU) has been reset but the SDCard has not, so the first
 * CMD0 may be lost.
 *
 * CMD8 is optionally sent to determine the voltage range supported, and
 * indirectly determine whether it is a version 1.x SD/non-SD card or
 * version 2.x. I'll just ignore this for now.
 *
 * ACMD41 is repeatedly issued to initialise the card, until "in idle"
 * (bit 0) of the R1 response goes to '0', indicating it is initialised.
 *
 * You should also indicate whether the host supports High Capicity cards,
 * and check whether the card is high capacity - i'll also ignore this
 *
 * SPI Protocol
 * ------------
 * The SD SPI protocol is based on transactions made up of 8-bit words, with
 * the host starting every bus transaction by asserting the CS signal low. The
 * card always responds to commands, data blocks and errors.
 *
 * The protocol supports a CRC, but by default it is off (except for the
 * first reset CMD0, where the CRC can just be pre-calculated, and CMD8)
 * I'll leave the CRC off I think!
 *
 * Standard capacity cards have variable data block sizes, whereas High
 * Capacity cards fix the size of data block to 512 bytes. I'll therefore
 * just always use the Standard Capacity cards with a block size of 512 bytes.
 * This is set with CMD16.
 *
 * You can read and write single blocks (CMD17, CMD25) or multiple blocks
 * (CMD18, CMD25). For simplicity, I'll just use single block accesses. When
 * the card gets a read command, it responds with a response token, and then
 * a data token or an error.
 *
 * SPI Command Format
 * ------------------
 * Commands are 6-bytes long, containing the command, 32-bit argument, and CRC.
 *
 * +---------------+------------+------------+-----------+----------+--------------+
 * | 01 | cmd[5:0] | arg[31:24] | arg[23:16] | arg[15:8] | arg[7:0] | crc[6:0] | 1 |
 * +---------------+------------+------------+-----------+----------+--------------+
 *
 * As I'm not using CRC, I can fix that byte to what is needed for CMD0 (0x95)
 *
 * All Application Specific commands shall be preceded with APP_CMD (CMD55).
 *
 * SPI Response Format
 * -------------------
 * The main response format (R1) is a status byte (normally zero). Key flags:
 *  idle - 1 if the card is in an idle state/initialising
 *  cmd  - 1 if an illegal command code was detected
 *
 *    +-------------------------------------------------+
 * R1 | 0 | arg | addr | seq | crc | cmd | erase | idle |
 *    +-------------------------------------------------+
 *
 * R1b is the same, except it is followed by a busy signal (zeros) until
 * the first non-zero byte when it is ready again.
 *
 * Data Response Token
 * -------------------
 * Every data block written to the card is acknowledged by a byte
 * response token
 *
 * +----------------------+
 * | xxx | 0 | status | 1 |
 * +----------------------+
 *              010 - OK!
 *              101 - CRC Error
 *              110 - Write Error
 *
 * Single Block Read and Write
 * ---------------------------
 *
 * Block transfers have a byte header, followed by the data, followed
 * by a 16-bit CRC. In our case, the data will always be 512 bytes.
 *
 * +------+---------+---------+- -  - -+---------+-----------+----------+
 * | 0xFE | data[0] | data[1] |        | data[n] | crc[15:8] | crc[7:0] |
 * +------+---------+---------+- -  - -+---------+-----------+----------+
 */

/* If the target has no SPI support then SDCard is not supported */
#ifdef DEVICE_SPI

#include "SDBlockDevice.h"
#include "mbed_debug.h"

#define SD_COMMAND_TIMEOUT                       5000    /*!< Number of times to query card for correct result */
#define SD_CMD0_GO_IDLE_STATE_RETRIES            3       /*!< Number of retries for sending CMDO*/
#define SD_CMD0_GO_IDLE_STATE                    0x00    /*!< CMD0 code value */
#define SD_CMD0_INVALID_RESPONSE_TIMEOUT         -1      /*!< CMD0 received invalid responses and timed out */
#define SD_DBG                                   1

#define SD_BLOCK_DEVICE_ERROR_WOULD_BLOCK        -5001	/*!< operation would block */
#define SD_BLOCK_DEVICE_ERROR_UNSUPPORTED        -5002	/*!< unsupported operation */
#define SD_BLOCK_DEVICE_ERROR_PARAMETER          -5003	/*!< invalid parameter */
#define SD_BLOCK_DEVICE_ERROR_NO_INIT            -5004	/*!< uninitialized */
#define SD_BLOCK_DEVICE_ERROR_NO_DEVICE          -5005	/*!< device is missing or not connected */
#define SD_BLOCK_DEVICE_ERROR_WRITE_PROTECTED    -5006	/*!< write protected */
#define SD_BLOCK_DEVICE_ERROR_UNUSABLE           -5007  /*!< unusable card */
#define SD_BLOCK_DEVICE_ERROR_NO_RESPONSE        -5008  /*!< No response from device */
#define SD_BLOCK_DEVICE_ERROR_CRC                -5009  /*!< CRC Error */
#define SD_BLOCK_DEVICE_ERROR_R1_OTHER           -5010  /*!< See R1 response to know more about error */

#define BLOCK_SIZE_HC                            512    /*!< Block size supported for SD card is 512 bytes  */
#define WRITE_BL_PARTIAL                         0      /*!< Partial block write - Not supported */
#define CRC_SUPPORT                              0      /*!< CRC - Not supported */
#define SPI_CMD(x) (0x40 | (x & 0x3f))

/* R1 Response Format */
#define R1_NO_RESPONSE          (0xFF)
#define R1_RESPONSE_RECV        (0x80)
#define R1_IDLE_STATE           (1 << 0)
#define R1_ERASE_RESET          (1 << 1)
#define R1_ILLEGAL_COMMAND      (1 << 2)
#define R1_COM_CRC_ERROR        (1 << 3)
#define R1_ERASE_SEQUENCE_ERROR (1 << 4)
#define R1_ADDRESS_ERROR        (1 << 5)
#define R1_PARAMETER_ERROR      (1 << 6)

// Types
#define SDCARD_None              0           /**< No card is present */
#define SDCARD_V1                1           /**< v1.x Standard Capacity */
#define SDCARD_V2                2           /**< v2.x Standard capacity SD card */
#define SDCARD_V2HC              3           /**< v2.x High capacity SD card */
#define CARD_UNKNOWN             4           /**< Unknown or unsupported card */
                                             
/* SIZE in Bytes */                          
#define PACKET_SIZE              6           /*!< SD Packet size CMD+ARG+CRC */
#define R1_RESPONSE_SIZE         1           /*!< Size of R1 response */
#define R2_RESPONSE_SIZE         2           /*!< Size of R2 response */
#define R3_R7_RESPONSE_SIZE      5           /*!< Size of R3/R7 response */

/* R1b Response */
#define DEVICE_BUSY             (0x00)

/* R2 Response Format */
#define R2_CARD_LOCKED          (1 << 0)
#define R2_CMD_FAILED           (1 << 1)
#define R2_ERROR                (1 << 2)
#define R2_CC_ERROR             (1 << 3)
#define R2_CC_FAILED            (1 << 4)
#define R2_WP_VIOLATION         (1 << 5)
#define R2_ERASE_PARAM          (1 << 6)
#define R2_OUT_OF_RANGE         (1 << 7)

/* R3 Response : OCR Register */
#define OCR_HCS_CCS             (0x1 << 30)
#define OCR_LOW_VOLTAGE         (0x01 << 24)
#define OCR_3_3V                (0x1 << 20)

/* R7 response pattern for CMD8 */
#define CMD8_PATTERN             (0xAA)

/*  CRC Enable  */
#define CRC_ENABLE               (0)         /*!< CRC 1 - Enable 0 - Disable */

/* Control Tokens   */
#define SPI_DATA_ACCEPTED        (0xE5)
#define SPI_DATA_CRC_ERROR       (0xEB)
#define SPI_DATA_WRITE_ERROR     (0xED)
#define SPI_START_BLOCK          (0xFE)      /*!< For Single Block Read/Write and Multiple Block Read */
#define SPI_START_BLK_MUL_WRITE  (0xFC)      /*!< Start Multi-block write */
#define SPI_STOP_TRAN            (0xFD)      /*!< Stop Multi-block write */

#define SPI_DATA_READ_ERROR_MASK (0xF)       /*!< Data Error Token: 4 LSB bits */
#define SPI_READ_ERROR           (0x1 << 0)  /*!< Error */
#define SPI_READ_ERROR_CC        (0x1 << 1)  /*!< CC Error*/
#define SPI_READ_ERROR_ECC_C     (0x1 << 2)  /*!< Card ECC failed */
#define SPI_READ_ERROR_OFR       (0x1 << 3)  /*!< Out of Range */

SDBlockDevice::SDBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName cs)
    : _spi(mosi, miso, sclk), _cs(cs), _is_initialized(0)
{
    _cs = 1;
    _card_type = SDCARD_None;
    // Set default to 100kHz for initialisation and 1MHz for data transfer
    _init_sck = 100000;
    _transfer_sck = 1000000;
}

SDBlockDevice::~SDBlockDevice()
{
    if (_is_initialized) {
        deinit();
    }
}

int SDBlockDevice::_initialise_card()
{
    _dbg = SD_DBG;
    _spi_init();

    /* Transition from SD Card mode to SPI mode by sending CMD0 GO_IDLE_STATE command */
    if (_go_idle_state() != R1_IDLE_STATE) {
        debug_if(_dbg, "No disk, or could not put SD card in to SPI idle state\n");
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE;
    }

    // send CMD8 to determine whether it is ver 2.x
    int r = _cmd8();
    if (r == R1_IDLE_STATE) {
        return _initialise_card_v2();
    } else if (r == (R1_IDLE_STATE | R1_ILLEGAL_COMMAND)) {
        return _initialise_card_v1();
    } else {
        debug_if(_dbg, "Not in idle state after sending CMD8 (not an SD card?)\n");
        return BD_ERROR_DEVICE_ERROR;
    }
}

int SDBlockDevice::_initialise_card_v1()
{
    uint32_t response;
    for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
        _cmd(CMD55_APP_CMD, 0);
        _cmd(ACMD41_SD_SEND_OP_COND, 0, &response);
        if (response == 0) {
            _block_size = 512;
            debug_if(_dbg, "\n\rInit: SEDCARD_V1\n\r");
            return BD_ERROR_OK;
        }
    }

    debug_if(_dbg, "Timeout waiting for v1.x card\n");
    return BD_ERROR_DEVICE_ERROR;
}

int SDBlockDevice::_initialise_card_v2()
{
    uint32_t response;
    for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
        wait_ms(50);
        _cmd58();
        _cmd(CMD55_APP_CMD, 0);
        _cmd(ACMD41_SD_SEND_OP_COND, 0x40000000, &response);
        if (response == 0) {
            _cmd58();
            debug_if(_dbg, "\n\rInit: SDCARD_V2\n\r");
            _block_size = 1;
            return BD_ERROR_OK;
        }
    }

    debug_if(_dbg, "Timeout waiting for v2.x card\n");
    return BD_ERROR_DEVICE_ERROR;
}

int SDBlockDevice::init()
{
    _lock.lock();
    int err = _initialise_card();
    _is_initialized = (err == BD_ERROR_OK);
    if (!_is_initialized) {
        debug_if(_dbg, "Fail to initialize card\n");
        _lock.unlock();
        return err;
    }
    debug_if(_dbg, "init card = %d\n", _is_initialized);
    _sectors = _sd_sectors();

    // Set block length to 512 (CMD16)
    if (_cmd(CMD16_SET_BLOCKLEN, 512) != 0) {
        debug_if(_dbg, "Set 512-byte block timed out\n");
        _lock.unlock();
        return BD_ERROR_DEVICE_ERROR;
    }

    // Set SCK for data transfer
    _spi.frequency(_transfer_sck);
    _lock.unlock();
    return BD_ERROR_OK;
}

int SDBlockDevice::deinit()
{
    return 0;
}

int SDBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_program(addr, size)) {
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    _lock.lock();
    if (!_is_initialized) {
        _lock.unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_INIT;
    }

    const uint8_t *buffer = static_cast<const uint8_t*>(b);
    while (size > 0) {
        bd_addr_t block = addr / 512;
        // set write address for single block (CMD24)
        if (_cmd(CMD24_WRITE_BLOCK, block * _block_size) != 0) {
            _lock.unlock();
            return BD_ERROR_DEVICE_ERROR;
        }

        // send the data block
        _write(buffer, 512);
        buffer += 512;
        addr += 512;
        size -= 512;
    }
    _lock.unlock();
    return 0;
}

int SDBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_read(addr, size)) {
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    _lock.lock();
    if (!_is_initialized) {
        _lock.unlock();
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }
    
    uint8_t *buffer = static_cast<uint8_t *>(b);
    while (size > 0) {
        bd_addr_t block = addr / 512;
        // set read address for single block (CMD17)
        if (_cmd(CMD17_READ_SINGLE_BLOCK, block * _block_size) != 0) {
            _lock.unlock();
            return BD_ERROR_DEVICE_ERROR;
        }
        
        // receive the data
        _read(buffer, 512);
        buffer += 512;
        addr += 512;
        size -= 512;
    }
    _lock.unlock();
    return 0;
}

int SDBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t SDBlockDevice::get_read_size() const
{
    return 512;
}

bd_size_t SDBlockDevice::get_program_size() const
{
    return 512;
}

bd_size_t SDBlockDevice::get_erase_size() const
{
    return 512;
}

bd_size_t SDBlockDevice::size() const
{
    bd_size_t sectors = 0;
    _lock.lock();
    if(_is_initialized) {
    	sectors = _sectors;
    }
    _lock.unlock();
    return 512*sectors;
}

void SDBlockDevice::debug(bool dbg)
{
    _dbg = dbg;
}

// PRIVATE FUNCTIONS
uint8_t SDBlockDevice::_cmd_spi(SDBlockDevice::cmdSupported cmd, uint32_t arg) {
    uint8_t response;
    char cmdPacket[PACKET_SIZE];

    // Prepare the command packet
    cmdPacket[0] = SPI_CMD(cmd);
    cmdPacket[1] = (arg >> 24);
    cmdPacket[2] = (arg >> 16);
    cmdPacket[3] = (arg >> 8);
    cmdPacket[4] = (arg >> 0);
    // CMD0 is executed in SD mode, hence should have correct CRC
    // CMD8 CRC verification is always enabled
    switch(cmd) {
        case CMD0_GO_IDLE_STATE:
            cmdPacket[5] = 0x95;
            break;
        case CMD8_SEND_IF_COND:
            cmdPacket[5] = 0x87;
            break;
        default:
            cmdPacket[5] = 0xFF;    // Make sure bit 0-End bit is high
            break;
    }

    // send a command
    for (int i = 0; i < PACKET_SIZE; i++) {
        _spi.write(cmdPacket[i]);
    }
    // wait for the response (response[7] == 0)
    for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
        response = _spi.write(0xFF);
        // Got the response
        if (!(response & R1_RESPONSE_RECV)) {
            break;
        }
    }
    return response;
}

int SDBlockDevice::_cmd(SDBlockDevice::cmdSupported cmd, uint32_t arg, uint32_t *resp) {
    int32_t status = BD_ERROR_OK;
    uint8_t response;
    // Select card
    _select();

    // Send command over SPI interface
    response = _cmd_spi(cmd, arg);
    // Pass the response to the command call if required
    if(NULL != resp) {
        *resp = response;
    }

    // Process the response R1
    if(R1_NO_RESPONSE == response) {
        status = SD_BLOCK_DEVICE_ERROR_NO_DEVICE;        // No device
    }else if(response & R1_COM_CRC_ERROR) {
        debug_if(SD_DBG, "CRC Error: CMD: %d\n", cmd);
        status = SD_BLOCK_DEVICE_ERROR_CRC;             // Retry for CRC
    }

    // Get rest of the response part for other commands
    switch(cmd) {
        case CMD8_SEND_IF_COND:             // Response R7
            // Illegal command is for Ver1 or not SD Card
            if(response & R1_ILLEGAL_COMMAND) {
                debug_if(SD_DBG, "Illegal command response - CMD8\n");
                _card_type = CARD_UNKNOWN;
            }
            else {
                debug_if(SD_DBG, "V2-Version Card\n");
                _card_type = SDCARD_V2;
            }
            // Note: No break here, need to read rest of the response
        case CMD58_READ_OCR:                // Response R3
            if(NULL == resp) {
                status = SD_BLOCK_DEVICE_ERROR_PARAMETER;
                break;
            }
            *resp  = (_spi.write(0xFF) << 24);
            *resp |= (_spi.write(0xFF) << 16);
            *resp |= (_spi.write(0xFF) << 8);
            *resp |= (_spi.write(0xFF) << 0);
            debug_if(SD_DBG, "CMD:%d \t arg:0x%x \t Response:0x%x 0x%x \n", cmd, arg, response, *resp);
            break;
        case CMD12_STOP_TRANSMISSION:       // Response R1b
        case CMD38_ERASE:                   // TODO:
            debug_if(SD_DBG, "CMD:%d \t arg:0x%x \t Response:0x%x \n", cmd, arg, response);
            break;
        case CMD13_SEND_STATUS:             // Response R2
            if(NULL == resp) {
                status = SD_BLOCK_DEVICE_ERROR_PARAMETER;
                break;
            }
            *resp = _spi.write(0xFF);
            debug_if(SD_DBG, "CMD:%d \t arg:0x%x \t Response:0x%x 0x%x \n", cmd, arg, response, *resp);
            break;
        default:                            // Response R1
            debug_if(SD_DBG, "CMD:%d \t arg:0x%x \t Response:0x%x \n", cmd, arg, response);
            break;
    }

    // Deselect card
    _deselect();
    return status;
}

int SDBlockDevice::_cmd58() {
    int arg = 0;
    _select();
    // send a command
    _spi.write(0x40 | 58);
    _spi.write(arg >> 24);
    _spi.write(arg >> 16);
    _spi.write(arg >> 8);
    _spi.write(arg >> 0);
    _spi.write(0x95);

    // wait for the response (response[7] == 0)
    for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
        int response = _spi.write(0xFF);
        if (!(response & 0x80)) {
            int ocr = _spi.write(0xFF) << 24;
            ocr |= _spi.write(0xFF) << 16;
            ocr |= _spi.write(0xFF) << 8;
            ocr |= _spi.write(0xFF) << 0;
            _deselect();
            return response;
        }
    }
    _deselect();
    return -1; // timeout
}

int SDBlockDevice::_cmd8() {
    _select();

    // send a command
    _spi.write(0x40 | 8); // CMD8
    _spi.write(0x00);     // reserved
    _spi.write(0x00);     // reserved
    _spi.write(0x01);     // 3.3v
    _spi.write(0xAA);     // check pattern
    _spi.write(0x87);     // crc

    // wait for the response (response[7] == 0)
    for (int i = 0; i < SD_COMMAND_TIMEOUT * 1000; i++) {
        char response[5];
        response[0] = _spi.write(0xFF);
        if (!(response[0] & 0x80)) {
            for (int j = 1; j < 5; j++) {
                response[i] = _spi.write(0xFF);
            }
            _deselect();
            return response[0];
        }
    }
    _deselect();
    return -1; // timeout
}

int SDBlockDevice::_go_idle_state() {
    int cmd_arg = 0;    /* CMD0 argument is just "stuff bits" */
    int ret = SD_CMD0_INVALID_RESPONSE_TIMEOUT;

    _select();
    /* Reseting the MCU SPI master may not reset the on-board SDCard, in which
     * case when MCU power-on occurs the SDCard will resume operations as
     * though there was no reset. In this scenario the first CMD0 will
     * not be interpreted as a command and get lost. For some cards retrying
     * the command overcomes this situation. */
    for (int num_retries = 0; ret != R1_IDLE_STATE && (num_retries < SD_CMD0_GO_IDLE_STATE_RETRIES); num_retries++) {
        /* send a CMD0 */
        _spi.write(0x40 | SD_CMD0_GO_IDLE_STATE);
        _spi.write(cmd_arg >> 24);
        _spi.write(cmd_arg >> 16);
        _spi.write(cmd_arg >> 8);
        _spi.write(cmd_arg >> 0);
        _spi.write(0x95);

        /* wait for the R1_IDLE_STATE response */
        for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
            int response = _spi.write(0xFF);
            /* Explicitly check for the R1_IDLE_STATE response rather that most significant bit
             * being 0 because invalid data can be returned. */
            if ((response == R1_IDLE_STATE)) {
                ret = response;
                break;
            }
            wait_ms(1);
        }
    }
    _deselect();
    return ret;
}

int SDBlockDevice::_read(uint8_t *buffer, uint32_t length) {
    _select();

    // read until start byte (0xFF)
    while (_spi.write(0xFF) != 0xFE);

    // read data
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = _spi.write(0xFF);
    }
    _spi.write(0xFF); // checksum
    _spi.write(0xFF);

    _deselect();
    return 0;
}

int SDBlockDevice::_write(const uint8_t*buffer, uint32_t length) {
    _select();

    // indicate start of block
    _spi.write(0xFE);

    // write the data
    for (uint32_t i = 0; i < length; i++) {
        _spi.write(buffer[i]);
    }

    // write the checksum
    _spi.write(0xFF);
    _spi.write(0xFF);

    // check the response token
    if ((_spi.write(0xFF) & 0x1F) != 0x05) {
        _deselect();
        return 1;
    }

    // wait for write to finish
    while (_spi.write(0xFF) == 0);

    _deselect();
    return 0;
}

static uint32_t ext_bits(unsigned char *data, int msb, int lsb) {
    uint32_t bits = 0;
    uint32_t size = 1 + msb - lsb;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t position = lsb + i;
        uint32_t byte = 15 - (position >> 3);
        uint32_t bit = position & 0x7;
        uint32_t value = (data[byte] >> bit) & 1;
        bits |= value << i;
    }
    return bits;
}

uint32_t SDBlockDevice::_sd_sectors() {
    uint32_t c_size, c_size_mult, read_bl_len;
    uint32_t block_len, mult, blocknr, capacity;
    uint32_t hc_c_size;
    uint32_t blocks;

    // CMD9, Response R2 (R1 byte + 16-byte block read)
    if (_cmd(CMD9_SEND_CSD, 0) != 0) {
        debug_if(_dbg, "Didn't get a response from the disk\n");
        return 0;
    }

    uint8_t csd[16];
    if (_read(csd, 16) != 0) {
        debug_if(_dbg, "Couldn't read csd response from disk\n");
        return 0;
    }

    // csd_structure : csd[127:126]
    // c_size        : csd[73:62]
    // c_size_mult   : csd[49:47]
    // read_bl_len   : csd[83:80] - the *maximum* read block length

    int csd_structure = ext_bits(csd, 127, 126);

    switch (csd_structure) {
        case 0:
            _block_size = 512;
            c_size = ext_bits(csd, 73, 62);
            c_size_mult = ext_bits(csd, 49, 47);
            read_bl_len = ext_bits(csd, 83, 80);

            block_len = 1 << read_bl_len;
            mult = 1 << (c_size_mult + 2);
            blocknr = (c_size + 1) * mult;
            capacity = blocknr * block_len;
            blocks = capacity / 512;
            debug_if(_dbg, "\n\rSDBlockDevice\n\rc_size: %d \n\rcapacity: %ld \n\rsectors: %lld\n\r", c_size, capacity, blocks);
            break;

        case 1:
            _block_size = 1;
            hc_c_size = ext_bits(csd, 63, 48);
            blocks = (hc_c_size+1)*1024;
            debug_if(_dbg, "\n\rSDHC Card \n\rhc_c_size: %d\n\rcapacity: %lld \n\rsectors: %lld\n\r", hc_c_size, blocks*512, blocks);
            break;

        default:
            debug_if(_dbg, "CSD struct unsupported\r\n");
            return 0;
    };
    return blocks;
}

void SDBlockDevice::_spi_init() {
    _spi.lock();
    // Set to SCK for initialization, and clock card with cs = 1
    _spi.frequency(_init_sck);
    _spi.format(8, 0);
    // Initial 74 cycles required for few cards, before selecting SPI mode
    _cs = 1;
    for (int i = 0; i < 10; i++) {
        _spi.write(0xFF);
    }
    _spi.unlock();
}


void SDBlockDevice::_select() {
    _spi.lock();
    _cs = 0;
}

void SDBlockDevice::_deselect() {
    _cs = 1;
    _spi.write(0xFF);
    _spi.unlock();
}
#endif  /* DEVICE_SPI */
