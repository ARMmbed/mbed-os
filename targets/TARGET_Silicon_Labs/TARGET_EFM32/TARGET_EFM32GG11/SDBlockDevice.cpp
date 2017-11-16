/***************************************************************************//**
 * @file SDBlockDevice.cpp
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
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
 *
 ******************************************************************************/
/* This driver is meant for devices with native SD peripherals */
#if defined(DEVICE_SD)

#include "SDBlockDevice.h"
#include "mbed_debug.h"
#include "em_device.h"
#include "em_cmu.h"

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

#define R1_IDLE_STATE           (1 << 0)
#define R1_ERASE_RESET          (1 << 1)
#define R1_ILLEGAL_COMMAND      (1 << 2)
#define R1_COM_CRC_ERROR        (1 << 3)
#define R1_ERASE_SEQUENCE_ERROR (1 << 4)
#define R1_ADDRESS_ERROR        (1 << 5)
#define R1_PARAMETER_ERROR      (1 << 6)

// Types
//  - v1.x Standard Capacity
//  - v2.x Standard Capacity
//  - v2.x High Capacity
//  - Not recognised as an SD Card
#define SDCARD_FAIL 0
#define SDCARD_V1   1
#define SDCARD_V2   2
#define SDCARD_V2HC 3

#define INT_ERR_ENABLE    (SDIO_IFCR_TARGETRESP  | SDIO_IFCR_ADMAERR         |\
                           SDIO_IFCR_AUTOCMDERR  | SDIO_IFCR_CURRENTLIMITERR |\
                           SDIO_IFCR_DATENDBITERR| SDIO_IFCR_DATCRCERR       |\
                           /*SDIO_IFCR_DATTOUTERR  | */SDIO_IFCR_CMDINDEXERR     |\
                           SDIO_IFCR_CMDENDBITERR| SDIO_IFCR_CMDCRCERR       |\
                           /*SDIO_IFCR_CMDTOUTERR  | */SDIO_IFCR_RETUNINGEVT     |\
                           SDIO_IFCR_ERRINT)
#define INT_STATUS_ENABLE (SDIO_IFCR_CARDINS      | SDIO_IFCR_BFRRDRDY |\
                           SDIO_IFCR_BFRWRRDY     | SDIO_IFCR_DMAINT   |\
                           SDIO_IFCR_BLKGAPEVT    | SDIO_IFCR_TRANCOM  |\
                           SDIO_IFCR_CMDCOM       | SDIO_IFCR_CARDINT  |\
                           SDIO_IFCR_CARDRM)

#define RESPONSE_NONE (SDIO_TFRMODE_RESPTYPESEL_NORESP)
#define RESPONSE_R1  (SDIO_TFRMODE_RESPTYPESEL_RESP48   \
                     | SDIO_TFRMODE_CMDINDXCHKEN        \
                     | SDIO_TFRMODE_CMDCRCCHKEN_ENABLE)
#define RESPONSE_R1B (SDIO_TFRMODE_RESPTYPESEL_BUSYAFTRESP  \
                     | SDIO_TFRMODE_CMDINDXCHKEN            \
                     | SDIO_TFRMODE_CMDCRCCHKEN_ENABLE)     
#define RESPONSE_R2  (SDIO_TFRMODE_RESPTYPESEL_RESP136  \
                     |SDIO_TFRMODE_CMDCRCCHKEN_ENABLE)
#define RESPONSE_R3  (SDIO_TFRMODE_RESPTYPESEL_RESP48)
#define RESPONSE_R6  (SDIO_TFRMODE_RESPTYPESEL_BUSYAFTRESP  \
                     | SDIO_TFRMODE_CMDINDXCHKEN            \
                     | SDIO_TFRMODE_CMDCRCCHKEN_ENABLE)

SDBlockDevice::SDBlockDevice()
    : _is_initialized(0), _dbg(SD_DBG), _address(0)
{
    _init_hw();
}

SDBlockDevice::~SDBlockDevice()
{
    if (_is_initialized) {
        deinit();
        _deinit_hw();
    }
}

void SDBlockDevice::_init_hw(void)
{
    /* Configure clocks */
    CMU_ClockEnable(cmuClock_SDIOREF, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

#if ( CORE_CLOCK_SOURCE != HFXO )
#error "This driver assumes it can run off of the HFXO"
#endif

    CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_SDIO;

    CMU->SDIOCTRL = (CMU->SDIOCTRL & ~_CMU_SDIOCTRL_SDIOCLKSEL_MASK) | CMU_SDIOCTRL_SDIOCLKSEL_HFXO;
    CMU->SDIOCTRL &= ~CMU_SDIOCTRL_SDIOCLKDIS;

    /* Configure I/O */
    /* Enable SD card power */
    GPIO_PinModeSet(gpioPortE, 7u, gpioModePushPull, 1);
    GPIO_PinOutSet(gpioPortE, 7u);

    /* Set up SD pins and route location */
    /* DAT[0:3] */
    GPIO_PinModeSet(gpioPortA, 3u, gpioModePushPull, 1u);
    GPIO_PinModeSet(gpioPortA, 2u, gpioModePushPull, 1u);
    GPIO_PinModeSet(gpioPortA, 1u, gpioModePushPull, 1u);
    GPIO_PinModeSet(gpioPortA, 0u, gpioModePushPull, 1u);

    /* CMD/CLK */
    GPIO_PinModeSet(gpioPortE, 15u, gpioModePushPull, 0u);
    GPIO_PinModeSet(gpioPortE, 14u, gpioModePushPull, 1u);

    /* CD */
    GPIO_PinModeSet(gpioPortB, 10u, gpioModeInput, 0);

    SDIO->ROUTELOC0 = SDIO_ROUTELOC0_CLKLOC_LOC1
                    | SDIO_ROUTELOC0_WPLOC_LOC1
                    | SDIO_ROUTELOC0_CDLOC_LOC1
                    | SDIO_ROUTELOC0_DATLOC_LOC1;

    SDIO->ROUTELOC1 = SDIO_ROUTELOC1_CMDLOC_LOC1;

    SDIO->ROUTEPEN = SDIO_ROUTEPEN_CLKPEN
                   | SDIO_ROUTEPEN_CMDPEN
                   | SDIO_ROUTEPEN_D0PEN
                   | SDIO_ROUTEPEN_D1PEN
                   | SDIO_ROUTEPEN_D2PEN
                   | SDIO_ROUTEPEN_D3PEN;

    /* Apply calibration values */
    SDIO->CTRL = ( 0 << _SDIO_CTRL_ITAPDLYEN_SHIFT)
               | ( 0 << _SDIO_CTRL_ITAPDLYSEL_SHIFT)
               | ( 0 << _SDIO_CTRL_ITAPCHGWIN_SHIFT)
               | ( 1 << _SDIO_CTRL_OTAPDLYEN_SHIFT)
               | ( 8 << _SDIO_CTRL_OTAPDLYSEL_SHIFT);

    SDIO->CFG0 = (0x20 << _SDIO_CFG0_TUNINGCNT_SHIFT)
               | (0x30 << _SDIO_CFG0_TOUTCLKFREQ_SHIFT)
               | (1 << _SDIO_CFG0_TOUTCLKUNIT_SHIFT)
               | (0xD0 << _SDIO_CFG0_BASECLKFREQ_SHIFT)
               | (SDIO_CFG0_MAXBLKLEN_1024B)
               | (1 << _SDIO_CFG0_C8BITSUP_SHIFT)
               | (1 << _SDIO_CFG0_CADMA2SUP_SHIFT)
               | (1 << _SDIO_CFG0_CHSSUP_SHIFT)
               | (1 << _SDIO_CFG0_CSDMASUP_SHIFT)
               | (1 << _SDIO_CFG0_CSUSPRESSUP_SHIFT)
               | (1 << _SDIO_CFG0_C3P3VSUP_SHIFT)
               | (1 << _SDIO_CFG0_C3P0VSUP_SHIFT)
               | (1 << _SDIO_CFG0_C1P8VSUP_SHIFT);

    SDIO->CFG1 = (0 << _SDIO_CFG1_ASYNCINTRSUP_SHIFT)
               | (SDIO_CFG1_SLOTTYPE_EMSDSLOT)
               | (1 << _SDIO_CFG1_CSDR50SUP_SHIFT)
               | (1 << _SDIO_CFG1_CSDR104SUP_SHIFT)
               | (1 << _SDIO_CFG1_CDDR50SUP_SHIFT)
               | (1 << _SDIO_CFG1_CDRVASUP_SHIFT)
               | (1 << _SDIO_CFG1_CDRVCSUP_SHIFT)
               | (1 << _SDIO_CFG1_CDRVDSUP_SHIFT)
               | (1 << _SDIO_CFG1_RETUNTMRCTL_SHIFT)
               | (1 << _SDIO_CFG1_TUNSDR50_SHIFT)
               | (0 << _SDIO_CFG1_RETUNMODES_SHIFT)
               | (1 << _SDIO_CFG1_SPISUP_SHIFT)
               | (1 << _SDIO_CFG1_ASYNCWKUPEN_SHIFT);

    SDIO->CFGPRESETVAL0 = (0 << _SDIO_CFGPRESETVAL0_INITSDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL0_INITCLKGENEN_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL0_INITDRVST_SHIFT)
                        | (0x4 << _SDIO_CFGPRESETVAL0_DSPSDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL0_DSPCLKGENEN_SHIFT)
                        | (0x3 << _SDIO_CFGPRESETVAL0_DSPDRVST_SHIFT);

    SDIO->CFGPRESETVAL1 = (2 << _SDIO_CFGPRESETVAL1_HSPSDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL1_HSPCLKGENEN_SHIFT)
                        | (2 << _SDIO_CFGPRESETVAL1_HSPDRVST_SHIFT)
                        | (4 << _SDIO_CFGPRESETVAL1_SDR12SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL1_SDR12CLKGENEN_SHIFT)
                        | (1 << _SDIO_CFGPRESETVAL1_SDR12DRVST_SHIFT);

    SDIO->CFGPRESETVAL2 = (2 << _SDIO_CFGPRESETVAL2_SDR25SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL2_SDR25CLKGENEN_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL2_SDR25DRVST_SHIFT)
                        | (1 << _SDIO_CFGPRESETVAL2_SDR50SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL2_SDR50CLKGENEN_SHIFT)
                        | (1 << _SDIO_CFGPRESETVAL2_SDR50DRVST_SHIFT);

    SDIO->CFGPRESETVAL3 = (0 << _SDIO_CFGPRESETVAL3_SDR104SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL3_SDR104CLKGENEN_SHIFT)
                        | (2 << _SDIO_CFGPRESETVAL3_SDR104DRVST_SHIFT)
                        | (2 << _SDIO_CFGPRESETVAL3_DDR50SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL3_DDR50CLKGENEN_SHIFT)
                        | (3 << _SDIO_CFGPRESETVAL3_DDR50DRVST_SHIFT);

    //NVIC_EnableIRQ(SDIO_IRQn);

    SDIO->BLKSIZE = (512U) | SDIO_BLKSIZE_HSTSDMABUFSIZE_SIZE4;

    /* Clear interrupts */
    SDIO->IFCR = INT_STATUS_ENABLE;
    SDIO->IFCR = INT_ERR_ENABLE;

    SDIO->IFENC = INT_STATUS_ENABLE | INT_ERR_ENABLE;

    _set_freq(400000);

    SDIO->HOSTCTRL1 |= SDIO_HOSTCTRL1_SDBUSPOWER | SDIO_HOSTCTRL1_SDBUSVOLTSEL_3P3V;
}

void SDBlockDevice::_deinit_hw(void)
{
    //NVIC_DisableIRQ(SDIO_IRQn);
}

void SDBlockDevice::_set_freq(uint32_t freq)
{
    uint16_t divisor;
    /* Disable clock */
    SDIO->CLOCKCTRL &= ~SDIO_CLOCKCTRL_INTCLKEN;

    /* Set new divisor */
    for(divisor = 1; divisor < 2046; divisor++) {
        if((50000000U / divisor) <= freq) {
            divisor--;
            break;
        }
    }
    divisor--;

    SDIO->CLOCKCTRL &= ~(_SDIO_CLOCKCTRL_SDCLKFREQSEL_MASK | _SDIO_CLOCKCTRL_UPPSDCLKFRE_MASK);
    SDIO->CLOCKCTRL |= ((divisor & 0xFFU) << _SDIO_CLOCKCTRL_SDCLKFREQSEL_SHIFT);
    SDIO->CLOCKCTRL |= (((divisor >> 8) & 0x3U) << _SDIO_CLOCKCTRL_UPPSDCLKFRE_SHIFT);
    SDIO->CLOCKCTRL |= SDIO_CLOCKCTRL_INTCLKEN;

    while(!(SDIO->CLOCKCTRL & SDIO_CLOCKCTRL_INTCLKSTABLE));
    SDIO->CLOCKCTRL |= SDIO_CLOCKCTRL_SDCLKEN;
}

int SDBlockDevice::_initialise_card()
{
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
    for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
        _cmd(55, 0, 0, false);
        if (_cmd(41, 0, 0, false) == 0) {
            _block_size = 512;
            debug_if(_dbg, "\n\rInit: SEDCARD_V1\n\r");

            // Set block length to 512 (CMD16)
            if (_cmd(16, 512, 0, false) != 0) {
                debug_if(_dbg, "Set 512-byte block timed out\n");
                return BD_ERROR_DEVICE_ERROR;
            }
            _sectors = _sd_sectors();
            return BD_ERROR_OK;
        }
    }

    debug_if(_dbg, "Timeout waiting for v1.x card\n");
    return BD_ERROR_DEVICE_ERROR;
}

int SDBlockDevice::_initialise_card_v2()
{
    _lock.lock();
    for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
        wait_ms(50);
        _cmd58();
        _cmd(55, 0, 0, false);
        if (_cmd(41, 0x40300000, 0, false) == 0) {
            if(SDIO->RESP0 & 0x80000000U) {
                _cmd58();
                // get through identification mode
                _cmd(2, 0, 0, false);
                _address = 0;
                while(_address == 0) {
                    _cmd(3, 0, 0, false);
                    _address = SDIO->RESP0 & 0xFFFF0000U;
                }
                debug_if(_dbg, "\n\rInit: SDCARD_V2\n\r");
                _sectors = _sd_sectors();

                _cmd(7, _address, 0, false);
                _cmd(55, _address, 0, false);
                _cmd(42, 0, 0, false);
                
                _block_size = 1;
                _lock.unlock();
                return BD_ERROR_OK;
            }
        }
    }

    debug_if(_dbg, "Timeout waiting for v2.x card\n");
    _lock.unlock();
    return BD_ERROR_DEVICE_ERROR;
}

int SDBlockDevice::init()
{
    int err;
    if(!_is_initialized) {
        err = _initialise_card();
        _is_initialized = (err == BD_ERROR_OK);
    } else {
        return BD_ERROR_OK;
    }
    if (!_is_initialized) {
        debug_if(_dbg, "Fail to initialize card\n");
        return err;
    }

    debug_if(_dbg, "init card = %d\n", _is_initialized);

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

    if (!_is_initialized) {
        return SD_BLOCK_DEVICE_ERROR_NO_INIT;
    }

    const uint8_t *buffer = static_cast<const uint8_t*>(b);
    /* set up DMA */
    uint32_t descriptor[2];
    descriptor[1] = (uint32_t)buffer;
    descriptor[0] = size << 16 | 0x23;

    _lock.lock();
    SDIO->ADSADDR = (uint32_t)descriptor;
    SDIO->SDMASYSADDR = (uint32_t)buffer;

    /* issue command */
    if(_cmd(25, ((addr/512)*_block_size), (size % 512 == 0 ? size / 512 : size / 512 + 1), false) != 0) {
        _lock.unlock();
        return BD_ERROR_DEVICE_ERROR;
    }
    _lock.unlock();
    return 0;
}

int SDBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_read(addr, size)) {
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }
    if (!_is_initialized) {
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    if(!(SDIO->PRSSTAT & SDIO_PRSSTAT_CARDINS)) {
        return BD_ERROR_DEVICE_ERROR;
    }
    
    uint8_t *buffer = static_cast<uint8_t *>(b);

    /* set up DMA */
    uint32_t descriptor[2];
    descriptor[1] = (uint32_t)buffer;
    descriptor[0] = size << 16 | 0x23;

    _lock.lock();
    SDIO->ADSADDR = (uint32_t)descriptor;
    SDIO->SDMASYSADDR = (uint32_t)buffer;

    /* issue command */
    if (_cmd(18, ((addr/512)*_block_size), (size % 512 == 0 ? size / 512 : size / 512 + 1), true) != 0) {
        _lock.unlock();
        return BD_ERROR_DEVICE_ERROR;
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
    if(_is_initialized) {
    	sectors = _sectors;
    }
    return 512*sectors;
}

void SDBlockDevice::debug(bool dbg)
{
    _dbg = dbg;
}


// PRIVATE FUNCTIONS
int SDBlockDevice::_cmd(uint32_t cmd, uint32_t arg, uint16_t blkcnt, bool read) {
    uint32_t trf = 0;
    bool istransfer = false;
    if(SDIO->PRSSTAT & SDIO_PRSSTAT_CMDINHIBITCMD) {
        debug_if(_dbg, "Already busy with a command!\n");
        return -1;
    }
    if(SDIO->PRSSTAT & SDIO_PRSSTAT_CMDINHIBITDAT) {
        debug_if(_dbg, "Already busy with data!\n");
        return -1;
    }

    SDIO->BLKSIZE = (SDIO->BLKSIZE & ~_SDIO_BLKSIZE_BLKSCNTFORCURRTFR_MASK)
                  | (blkcnt << _SDIO_BLKSIZE_BLKSCNTFORCURRTFR_SHIFT);
    SDIO->CLOCKCTRL = (SDIO->CLOCKCTRL & ~_SDIO_CLOCKCTRL_DATTOUTCNTVAL_MASK)
                    | (0xE << _SDIO_CLOCKCTRL_DATTOUTCNTVAL_SHIFT);
    SDIO->CMDARG1 = arg;

    trf = (cmd << _SDIO_TFRMODE_CMDINDEX_SHIFT) & _SDIO_TFRMODE_CMDINDEX_MASK;
    switch(cmd) {
        case 0:
            trf |= RESPONSE_NONE;
            break;
        case 1:
            trf |= RESPONSE_R3;
            break;
        case 2:
            trf |= RESPONSE_R2;
            break;
        case 3:
            trf |= RESPONSE_R6;
            istransfer = true;
        case 4:
            trf |= RESPONSE_NONE;
            break;
        case 5:
            trf |= RESPONSE_R1B;
            break;
        case 6:
            trf |= RESPONSE_R1 | SDIO_TFRMODE_DATPRESSEL_DATA;
            break;
        case 7:
            trf |= RESPONSE_R1;
            break;
        case 8:
            trf |= RESPONSE_R1;
            break;
        case 9:
            trf |= RESPONSE_R2;
            break;
        case 11:
        case 10:
        case 12:
        case 13:
        case 16:
            trf |= RESPONSE_R1;
            break;
        case 17:
        case 18:
        case 19:
        case 21:
            trf |= RESPONSE_R1 | SDIO_TFRMODE_DATPRESSEL_DATA;
            istransfer = true;
            break;
        case 23:
        case 24:
        case 25:
            trf |= RESPONSE_R1 | SDIO_TFRMODE_DATPRESSEL_DATA;
            istransfer = true;
            break;
        case 41:
            trf |= RESPONSE_R3;
            break;
        case 42:
            trf |= RESPONSE_R1;
            break;
        case 51:
            trf |= RESPONSE_R1 | SDIO_TFRMODE_DATPRESSEL_DATA;
            istransfer = true;
            break;
        case 52:
        case 55:
            trf |= RESPONSE_R1;
            break;
        default:
            break;
    }

    SDIO->TFRMODE = trf
                  | SDIO_TFRMODE_AUTOCMDEN_ACMD12EN
                  | SDIO_TFRMODE_BLKCNTEN_ENABLE
                  | SDIO_TFRMODE_BLKCNTEN
                  | SDIO_TFRMODE_DMAEN_ENABLE
                  | (read == true ? SDIO_TFRMODE_DATDIRSEL : 0);

    uint32_t flags = 0;
    while(((flags = SDIO->IFCR) & (istransfer ? SDIO_IFCR_TRANCOM | SDIO_IFCR_CMDCOM : SDIO_IFCR_CMDCOM)) != (istransfer ? SDIO_IFCR_TRANCOM | SDIO_IFCR_CMDCOM : SDIO_IFCR_CMDCOM)) {
        if(flags & SDIO_IFCR_ERRINT) {
            SDIO->IFCR = INT_ERR_ENABLE;
            return flags;
        }
        if(flags & SDIO_IFCR_DMAINT) {
            // Don't care about the boundary
            SDIO->SDMASYSADDR = SDIO->SDMASYSADDR;
        }
        if(cmd == 21 || cmd == 19) {
            if(flags & SDIO_IFCR_BFRRDRDY) {
                SDIO->IFCR = SDIO_IFCR_BFRRDRDY;
                return 0;
            }
        }
    }
    while(SDIO->PRSSTAT & (SDIO_PRSSTAT_CMDINHIBITDAT | SDIO_PRSSTAT_CMDINHIBITDAT));
    SDIO->IFCR = SDIO_IFCR_CMDCOM | SDIO_IFCR_TRANCOM | INT_ERR_ENABLE;
    return 0; // error
}
int SDBlockDevice::_cmdx(int cmd, int arg) {
    return _cmd(cmd, arg, 0, false);
}


int SDBlockDevice::_cmd58() {
    return _cmd(58, 0, 0, false);
}

int SDBlockDevice::_cmd8() {
    if(_cmd(8, 0x1AAU, 0, false) == 0) {
        if(SDIO->RESP0 == 0x1AAU) {
            return R1_IDLE_STATE;
        } else {
            return R1_ILLEGAL_COMMAND;
        }
    }
    else
        return -1;
}

int SDBlockDevice::_go_idle_state() {
    int ret = SD_CMD0_INVALID_RESPONSE_TIMEOUT;

    /* Reseting the MCU SPI master may not reset the on-board SDCard, in which
     * case when MCU power-on occurs the SDCard will resume operations as
     * though there was no reset. In this scenario the first CMD0 will
     * not be interpreted as a command and get lost. For some cards retrying
     * the command overcomes this situation. */
    for (int num_retries = 0; ret != 0 && (num_retries < SD_CMD0_GO_IDLE_STATE_RETRIES); num_retries++) {
        /* wait for the R1_IDLE_STATE response */
        for (int i = 0; i < SD_COMMAND_TIMEOUT; i++) {
            int response = _cmd(0,0,0,false);
            /* Explicitly check for the R1_IDLE_STATE response rather that most significant bit
             * being 0 because invalid data can be returned. */
            if ((response == 0)) {
                ret = response;
                break;
            }
            wait_ms(1);
        }
    }
    return ret;
}

static uint32_t ext_bits(unsigned char *data, int msb, int lsb) {
    uint32_t bits = 0;
    uint32_t size = 1 + msb - lsb;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t position = lsb + i;
        uint32_t byte = position >> 3;
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
    uint64_t hc_c_capacity;
    uint32_t blocks;
    _lock.lock();
    // CMD9, Response R2 (R1 byte + 16-byte block read)
    if (_cmdx(9, _address) != 0) {
        debug_if(_dbg, "Didn't get a response from the disk\n");
        _lock.unlock();
        return 0;
    }

    uint8_t csd[16];
    memcpy(&csd[1], (const void*)&SDIO->RESP0, 15);

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
            debug_if(_dbg, "\n\rSDBlockDevice\n\rc_size: %ld \n\rcapacity: %ld \n\rsectors: %ld\n\r", c_size, capacity, blocks);
            break;

        case 1:
            _block_size = 1;
            hc_c_size = ext_bits(csd, 63, 48);
            blocks = (hc_c_size+1)*1024;
            hc_c_capacity = (uint64_t)blocks * 512ULL;
            debug_if(_dbg, "\n\rSDHC Card \n\rhc_c_size: %ld\n\rcapacity: %lld \n\rsectors: %ld\n\r", hc_c_size, hc_c_capacity, blocks);
            break;

        default:
            debug_if(_dbg, "CSD struct unsupported\r\n");
            _lock.unlock();
            return 0;
    };
    _lock.unlock();
    return blocks;
}

#endif  /* DEVICE_SD */
