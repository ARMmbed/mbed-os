/**************************************************************************//**
 * @file    spim.c
 * @version V1.00
 * $Revision:1$
 * $Date:14/5/5 5:45p$
 * @brief   M480 series SPIM driver
 *
 * @note
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "M480.h"


/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_SPIM_Driver SPIM Driver
  @{
*/

/** @addtogroup M480_SPIM_EXPORTED_FUNCTIONS SPIM Exported Functions
  @{
*/


/// @cond HIDDEN_SYMBOLS

//#define SPIM_DRV_DEBUG     printf
#define SPIM_DRV_DEBUG(...)


static volatile uint8_t  g_MID = MFGID_UNKNOW;  // Manufacture ID

static void  N_delay(int n)
{
    while (n-- > 0)
        __NOP();
}

static void SwitchNBitOutput(uint32_t u32NBit)
{
    switch (u32NBit) {
    case 1:
        _SPIM_ENABLE_SING_OUTPUT_MODE();        // 1-bit, Output.
        break;

    case 2:
        _SPIM_ENABLE_DUAL_OUTPUT_MODE();        // 2-bit, Output.
        break;

    case 4:
        _SPIM_ENABLE_QUAD_OUTPUT_MODE();        // 4-bit, Output.
        break;
    }
}

static void SwitchNBitInput(uint32_t u32NBit)
{
    switch (u32NBit) {
    case 1:
        _SPIM_ENABLE_SING_INPUT_MODE();         // 1-bit, Input.
        break;

    case 2:
        _SPIM_ENABLE_DUAL_INPUT_MODE();         // 2-bit, Input.
        break;

    case 4:
        _SPIM_ENABLE_QUAD_INPUT_MODE();         // 4-bit, Input.
        break;
    }
}



/**
  * @brief      Write data to SPI slave.
  * @param      pu8TxBuf    Transmit buffer.
  * @param      u32NTx      Number of bytes to transmit.
  * @return     None.
  */
static void spim_write(const uint8_t *pu8TxBuf, uint32_t u32NTx)
{
    while (u32NTx) {
        unsigned int dataNum = 0, dataNum2;

        if (u32NTx >= 16) {
            dataNum = 4;
        } else if (u32NTx >= 12) {
            dataNum = 3;
        } else if (u32NTx >= 8) {
            dataNum = 2;
        } else if (u32NTx >= 4) {
            dataNum = 1;
        }

        dataNum2 = dataNum;
        while (dataNum2) {
            uint32_t tmp;

            memcpy(&tmp, pu8TxBuf, 4);
            pu8TxBuf += 4;
            u32NTx -= 4;

            dataNum2 --;
            *((__O uint32_t *) &SPIM->TX0 + dataNum2) = tmp;
        }

        if (dataNum) {
            _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_IO);    // Switch to Normal mode.
            _SPIM_SET_DATA_WIDTH(32);
            _SPIM_SET_DATA_NUM(dataNum);
            _SPIM_SET_GO();
            _SPIM_WAIT_FREE();
        }

        if (u32NTx && u32NTx < 4) {
            int rmn = u32NTx;

            uint32_t tmp;
            memcpy(&tmp, pu8TxBuf, u32NTx);
            pu8TxBuf += u32NTx;
            u32NTx = 0;
            SPIM->TX0 = tmp;

            _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_IO);    // Switch to Normal mode.
            _SPIM_SET_DATA_WIDTH(rmn * 8);
            _SPIM_SET_DATA_NUM(1);
            _SPIM_SET_GO();
            _SPIM_WAIT_FREE();
        }
    }
}

/**
  * @brief      Read data from SPI slave.
  * @param      pu8TxBuf    Receive buffer.
  * @param      u32NRx      Size of receive buffer in bytes.
  * @return     None.
  */
static void spim_read(uint8_t *pu8RxBuf, uint32_t u32NRx)
{
    while (u32NRx) {
        unsigned int dataNum = 0;   // number of words

        if (u32NRx >= 16) {
            dataNum = 4;
        } else if (u32NRx >= 12) {
            dataNum = 3;
        } else if (u32NRx >= 8) {
            dataNum = 2;
        } else if (u32NRx >= 4) {
            dataNum = 1;
        }

        if (dataNum) {
            _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_IO);    // Switch to Normal mode.
            _SPIM_SET_DATA_WIDTH(32);
            _SPIM_SET_DATA_NUM(dataNum);
            _SPIM_SET_GO();
            _SPIM_WAIT_FREE();
        }

        while (dataNum) {
            uint32_t tmp;

            tmp = *((__I uint32_t *) &SPIM->RX0 + dataNum - 1);
            memcpy(pu8RxBuf, &tmp, 4);
            pu8RxBuf += 4;
            dataNum --;
            u32NRx -= 4;
        }

        if (u32NRx && (u32NRx < 4)) {
            uint32_t tmp;

            _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_IO);    // Switch to Normal mode.
            _SPIM_SET_DATA_WIDTH(u32NRx * 8);
            _SPIM_SET_DATA_NUM(1);
            _SPIM_SET_GO();
            _SPIM_WAIT_FREE();

            tmp = SPIM->RX0;
            memcpy(pu8RxBuf, &tmp, u32NRx);
            pu8RxBuf += u32NRx;
            u32NRx = 0;
        }
    }
}

/**
  * @brief      Issue Write Status Register #1 command.
  * @param      dataBuf     Transmit buffer.
  * @param      u32NTx      Size of transmit buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_WriteStatusRegister(uint8_t *dataBuf, unsigned u32NTx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_WRSR, 0x00};                         // 1-byte Write Status Register #1 command + 1-byte data.
    cmdBuf[1] = dataBuf[0];

    _SPIM_SET_SS_EN(1);                         // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                         // CS deactivated.
}

/**
  * @brief      Issue Read Status Register #1 command.
  * @param      dataBuf     Receive buffer.
  * @param      u32NRx      Size of receive buffer.
 * @param       u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_ReadStatusRegister(uint8_t *dataBuf, unsigned u32NRx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_RDSR};           // 1-byte Read Status Register #1 command.

    _SPIM_SET_SS_EN(1);                         // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(u32NBit);
    spim_read(dataBuf, u32NRx);
    _SPIM_SET_SS_EN(0);                         // CS deactivated.
}

/**
  * @brief      Issue Write Status Register #2 command.
  * @param      dataBuf     Transmit buffer.
  * @param      u32NTx      Size of transmit buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_WriteStatusRegister2(uint8_t *dataBuf, unsigned u32NTx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_WRSR2, 0x00};    // 1-byte Write Status Register #2 command + 1-byte data.
    cmdBuf[1] = dataBuf[0];

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/**
  * @brief      Issue Read Status Register #2 command.
  * @param      dataBuf     Receive buffer.
  * @param      u32NRx      Size of receive buffer.
 * @param       u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_ReadStatusRegister2(uint8_t *dataBuf, unsigned u32NRx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_RDSR2};          // 1-byte Read Status Register #1 command.

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(u32NBit);
    spim_read(dataBuf, sizeof (dataBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/**
  * @brief      Issue Write Status Register #3 command.
  * @param      dataBuf     Transmit buffer.
  * @param      u32NTx      Size of transmit buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_WriteStatusRegister3(uint8_t *dataBuf, unsigned u32NTx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_WRSR3, 0x00};    // 1-byte Write Status Register #2 command + 1-byte data.
    cmdBuf[1] = dataBuf[0];

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/**
  * @brief      Issue Read Status Register #3 command.
  * @param      dataBuf     Receive buffer.
  * @param      u32NRx      Size of receive buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_ReadStatusRegister3(uint8_t *dataBuf, unsigned u32NRx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_RDSR3};          // 1-byte Read Status Register #1 command.

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(u32NBit);
    spim_read(dataBuf, sizeof (dataBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/**
  * @brief      Issue Write Security Register command.
  * @param      dataBuf     Transmit buffer.
  * @param      u32NTx      Size of transmit buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_WriteSecurityRegister(uint8_t *dataBuf, unsigned u32NTx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_WRSCUR, 0x00};   // 1-byte Write Status Register #2 command + 1-byte data.
    cmdBuf[1] = dataBuf[0];

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/**
  * @brief      Issue Read Security Register command.
  * @param      dataBuf     Receive buffer.
  * @param      u32NRx      Size of receive buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_ReadSecurityRegister(uint8_t *dataBuf, unsigned u32NRx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {OPCODE_RDSCUR};         // 1-byte Read Status Register #1 command.

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(u32NBit);
    spim_read(dataBuf, sizeof (dataBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/**
  * @brief      Check if Erase/Write is done.
  * @return     0: Not done. 1: Done.
  */
static int spim_is_write_done(uint32_t u32NBit)
{
    uint8_t status[1];
    SPIM_ReadStatusRegister(status, sizeof (status), u32NBit);
    return ! (status[0] & SR_WIP);
}

/**
  * @brief      Wait until Erase/Write done.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     0           SPIM write done.
  */
static int spim_wait_write_done(uint32_t u32NBit)
{
    int  count;

    for (count = 0; count < 0x1000000; count++) {
        if (spim_is_write_done(u32NBit))
            return 0;
    }
    SPIM_DRV_DEBUG("spim_wait_write_done time-out!!\n");
    return -1;
}

/**
  * @brief      Issue Write Enable/disable command.
  * @param      isEn        Enable/disable.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
static void spim_set_write_enable(int isEn, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = {0}; // 1-byte Write Enable command.
    cmdBuf[0] = isEn ? OPCODE_WREN : OPCODE_WRDI;

    _SPIM_SET_SS_EN(1);                             // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                             // CS deactivated.
}

/// @endcond HIDDEN_SYMBOLS

/**
  * @brief      Initialize SPIM flash.
  * @param      clrWP       Clear Write Protect or not.
  * @return     0    Success.
  * @return     -1   Unrecognised manufacture ID or failed on reading manufacture ID.
  */
int SPIM_InitFlash(int clrWP)
{
    uint8_t     idBuf[3];
    uint8_t     cmdBuf[1];

    _SPI_SET_SS_ACTLVL(0);

    // Because not sure in SPI or QPI mode, do QPI reset and then SPI reset.
    // QPI Reset Enable
    cmdBuf[0] = OPCODE_RSTEN;
    _SPIM_SET_SS_EN(1);                     // CS activated.
    _SPIM_ENABLE_QUAD_OUTPUT_MODE();        // 1-bit, Output.
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                     // CS deactivated.

    // QPI Reset
    cmdBuf[0] = OPCODE_RST;
    _SPIM_SET_SS_EN(1);                     // CS activated.
    _SPIM_ENABLE_QUAD_OUTPUT_MODE();        // 1-bit, Output.
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                     // CS deactivated.

    // SPI ResetEnable
    cmdBuf[0] = OPCODE_RSTEN;
    _SPIM_SET_SS_EN(1);                     // CS activated.
    _SPIM_ENABLE_SING_OUTPUT_MODE();        // 1-bit, Output.
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                     // CS deactivated.

    // SPI Reset
    cmdBuf[0] = OPCODE_RST;
    _SPIM_SET_SS_EN(1);                     // CS activated.
    _SPIM_ENABLE_SING_OUTPUT_MODE();        // 1-bit, Output.
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                     // CS deactivated.

    if (clrWP) {
        uint8_t dataBuf[] = {0x00};

        spim_set_write_enable(1, 1);                // Clear Block Protect.
        SPIM_WriteStatusRegister(dataBuf, sizeof (dataBuf), 1);
        spim_wait_write_done(1);
    }

    SPIM_ReadJedecId(idBuf, sizeof (idBuf), 1);

    //printf("ID: 0x%x, 0x%x, px%x\n", idBuf[0], idBuf[1], idBuf[2]);

    g_MID = idBuf[0];

    if ((g_MID != MFGID_WINBOND) &&
            (g_MID != MFGID_MXIC) &&
            (g_MID != MFGID_EON) &&
            (g_MID != MFGID_ISSI) &&
            (g_MID != MFGID_SPANSION)) {
        SPIM_DRV_DEBUG("Flash initialize failed!! 0x%x\n", g_MID);
        g_MID = MFGID_UNKNOW;
        return -1;
    }
    return 0;
}

/**
  * @brief      Issue JEDEC ID command.
  * @param      idBuf       ID buffer.
  * @param      u32NRx      Size of ID buffer.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_ReadJedecId(uint8_t *idBuf, unsigned u32NRx, uint32_t u32NBit)
{
    uint8_t cmdBuf[] = { OPCODE_RDID };         // 1-byte JEDEC ID command.

    _SPIM_SET_SS_EN(1);                         // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(u32NBit);
    spim_read(idBuf, u32NRx);
    _SPIM_SET_SS_EN(0);                         // CS deactivated.
}

/// @cond HIDDEN_SYMBOLS

static void spim_enable_spansion_quad_mode(int isEn)
{
    uint8_t cmdBuf[3];
    uint8_t dataBuf[1], status1;

    cmdBuf[0] = 0x5;                            // Read Status Register-1

    _SPIM_SET_SS_EN(1);
    SwitchNBitOutput(1);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(1);
    spim_read(dataBuf, sizeof (dataBuf));
    _SPIM_SET_SS_EN(0);

    //SPIM_DRV_DEBUG("SR1 = 0x%x\n", dataBuf[0]);

    status1 = dataBuf[0];

    cmdBuf[0] = 0x35;                           // Read Configuration Register-1

    _SPIM_SET_SS_EN(1);
    SwitchNBitOutput(1);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(1);
    spim_read(dataBuf, sizeof (dataBuf));
    _SPIM_SET_SS_EN(0);

//    SPIM_DRV_DEBUG("CR1 = 0x%x\n", dataBuf[0]);

    spim_set_write_enable(1, 1);

    cmdBuf[0] =  0x1;                           // Write register
    cmdBuf[1] =  status1;

    if (isEn)
        cmdBuf[2] = dataBuf[0] | 0x2;           // set QUAD
    else
        cmdBuf[2] = dataBuf[0] & ~0x2;          // clear QUAD

    _SPIM_SET_SS_EN(1);
    SwitchNBitOutput(1);
    spim_write(cmdBuf, 3);
    _SPIM_SET_SS_EN(0);

    spim_set_write_enable(0, 1);


    cmdBuf[0] = 0x35;                           // Read Configuration Register-1

    _SPIM_SET_SS_EN(1);
    SwitchNBitOutput(1);
    spim_write(cmdBuf, sizeof (cmdBuf));
    SwitchNBitInput(1);
    spim_read(dataBuf, sizeof (dataBuf));
    _SPIM_SET_SS_EN(0);

    SPIM_DRV_DEBUG("CR1 = 0x%x\n", dataBuf[0]);
    N_delay(10000);
}

/// @endcond HIDDEN_SYMBOLS

/**
  * @brief      Set Quad Enable/disable.
  * @param      isEn        Enable/disable.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_SetQuadEnable(int isEn, uint32_t u32NBit)
{
    uint8_t idBuf[3];

    SPIM_ReadJedecId(idBuf, sizeof (idBuf), u32NBit);

    //SPIM_DRV_DEBUG("Flash ID is 0x%x\n", idBuf[0]);

    switch (idBuf[0]) {
    case MFGID_WINBOND: {                   // Winbond SPI flash.
        uint8_t dataBuf[1];
        spim_set_write_enable(1, u32NBit);      // Write Enable.
        dataBuf[0] = isEn ? SR2_QE : 0;
        SPIM_WriteStatusRegister2(dataBuf, sizeof (dataBuf), u32NBit);
        spim_wait_write_done(u32NBit);
        break;
    }

    case MFGID_MXIC:                        // MXIC SPI flash.
    case MFGID_EON:
    case MFGID_ISSI: {                      // ISSI SPI flash.
        uint8_t dataBuf[1];

        spim_set_write_enable(1, u32NBit);      // Write Enable.
        dataBuf[0] = isEn ? SR_QE : 0;
        SPIM_WriteStatusRegister(dataBuf, sizeof (dataBuf), u32NBit);
        spim_wait_write_done(u32NBit);
        break;
    }

    case MFGID_SPANSION:
        spim_enable_spansion_quad_mode(isEn);
        return;
    }
}

/**
  * @brief      Enter/exit QPI mode.
  * @param      isEn        Enable/disable.
  * @return     None.
  */
static void spim_eon_set_qpi_mode(int isEn)
{
    uint8_t cmdBuf[1];                                  // 1-byte command.

    uint8_t status[1];
    SPIM_ReadStatusRegister(status, sizeof (status), 1);
    SPIM_DRV_DEBUG("Status: 0x%x\n", status[0]);

    if (isEn) {                                         // Assume in SPI mode.
        cmdBuf[0] = OPCODE_ENQPI;

        _SPIM_SET_SS_EN(1);                             // CS activated.
        SwitchNBitOutput(1);
        spim_write(cmdBuf, sizeof (cmdBuf));
        _SPIM_SET_SS_EN(0);                             // CS deactivated.
    } else {                                            // Assume in QPI mode.
        cmdBuf[0] = OPCODE_EXQPI;

        _SPIM_SET_SS_EN(1);                             // CS activated.
        SwitchNBitOutput(4);
        spim_write(cmdBuf, sizeof (cmdBuf));
        _SPIM_SET_SS_EN(0);                             // CS deactivated.
    }

    SPIM_ReadStatusRegister(status, sizeof (status), 1);
    SPIM_DRV_DEBUG("Status: 0x%x\n", status[0]);
}


static void SPIM_SPANSION_4Bytes_Enable(int isEn, uint32_t u32NBit)
{
    uint8_t cmdBuf[2];
    uint8_t dataBuf[1];

    cmdBuf[0] = OPCODE_BRRD;
    _SPIM_SET_SS_EN(1);                         // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, 1);
    SwitchNBitInput(1);
    spim_read(dataBuf, 1);
    _SPIM_SET_SS_EN(0);                         // CS deactivated.

    SPIM_DRV_DEBUG("Bank Address register= 0x%x\n", dataBuf[0]);

    cmdBuf[0] =  OPCODE_BRWR;

    if (isEn)
        cmdBuf[1] = dataBuf[0] | 0x80;          // set EXTADD
    else
        cmdBuf[1] = dataBuf[0] & ~0x80;         // clear EXTADD

    _SPIM_SET_SS_EN(1);                         // CS activated.
    SwitchNBitOutput(1);
    spim_write(cmdBuf, 2);
    _SPIM_SET_SS_EN(0);                         // CS deactivated.
}

/// @cond HIDDEN_SYMBOLS

/**
  * @brief      Query 4-byte address mode enabled or not.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     0: 4-byte address mode disabled. 1: 4-byte address mode enabled.
  */
static int SPIM_Is4ByteModeEnable(uint32_t u32NBit)
{
    int isEn = 0;
    uint8_t idBuf[3];

    SPIM_ReadJedecId(idBuf, sizeof (idBuf), u32NBit);

    // Based on Flash size, check if 4-byte address mode is supported.
    {
        int isSupt = 0;
        switch (idBuf[0]) {
        case MFGID_WINBOND:
        case MFGID_MXIC:
        case MFGID_EON:
            isSupt = (idBuf[2] < 0x19) ? 0 : 1;
            break;

        case MFGID_ISSI:
            isSupt = (idBuf[2] < 0x49) ? 0 : 1;
            break;
        }

        if (! isSupt) {
            return isEn;
        }
    }

    do {
        if (idBuf[0] == MFGID_WINBOND) {    // Winbond SPI flash.
            uint8_t dataBuf[1];
            SPIM_ReadStatusRegister3(dataBuf, sizeof (dataBuf), u32NBit);
            isEn = !! (dataBuf[0] & SR3_ADR);
        } else if (idBuf[0] == MFGID_MXIC || idBuf[0] ==MFGID_EON) { // MXIC/EON SPI flash.
            uint8_t dataBuf[1];
            SPIM_ReadSecurityRegister(dataBuf, sizeof (dataBuf), u32NBit);
            isEn = !! (dataBuf[0] & SCUR_4BYTE);
        }
    } while (0);

    return isEn;
}

/// @endcond HIDDEN_SYMBOLS


/**
  * @brief      Enter/Exit 4-byte address mode.
  * @param      isEn        Enable/disable.
  * @param      u32NBit     N-bit transmit/receive.
  * @return     None.
  */
void SPIM_Enable_4Bytes_Mode(int isEn, uint32_t u32NBit)
{
    uint8_t idBuf[3];
    SPIM_ReadJedecId(idBuf, sizeof (idBuf), u32NBit);

    // Based on Flash size, check if 4-byte address mode is supported.
    {
        int isSupt = 0;
        switch (idBuf[0]) {
        case MFGID_WINBOND:
        case MFGID_MXIC:
        case MFGID_EON:
            isSupt = (idBuf[2] < 0x19) ? 0 : 1;
            break;

        case MFGID_ISSI:
            isSupt = (idBuf[2] < 0x49) ? 0 : 1;
            break;

        case MFGID_SPANSION:
            SPIM_SPANSION_4Bytes_Enable(isEn, u32NBit);
            return;
        }

        if (! isSupt) {
            return;
        }
    }

    do {
        uint8_t cmdBuf[1];  // 1-byte Enter/Exit 4-Byte Mode command.
        cmdBuf[0] = isEn ? OPCODE_EN4B : OPCODE_EX4B;

        _SPIM_SET_SS_EN(1);                             // CS activated.
        SwitchNBitOutput(u32NBit);
        spim_write(cmdBuf, sizeof (cmdBuf));
        _SPIM_SET_SS_EN(0);                             // CS deactivated.

        // FIXME: Per test, 4BYTE Indicator bit doesn't set after EN4B, which doesn't match spec(MX25L25635E),
        // so skip the check below.
        if (idBuf[0] == MFGID_MXIC) {
            break;
        }

        if (isEn) {
            while (! SPIM_Is4ByteModeEnable(u32NBit));
        } else {
            while (SPIM_Is4ByteModeEnable(u32NBit));
        }
    } while (0);
}

/**
  * @brief      Erase whole chip.
  * @param      u32NBit     N-bit transmit/receive.
  * @param      isSync      Block or not.
  * @return     None.
  */
void SPIM_ChipErase(uint32_t u32NBit, int isSync)
{
    uint8_t cmdBuf[] = { OPCODE_CHIP_ERASE };      // 1-byte Chip Erase command.

    spim_set_write_enable(1, u32NBit);             // Write Enable.

    _SPIM_SET_SS_EN(1);                        // CS activated.
    SwitchNBitOutput(u32NBit);
    spim_write(cmdBuf, sizeof (cmdBuf));
    _SPIM_SET_SS_EN(0);                        // CS deactivated.

    if (isSync) {
        spim_wait_write_done(u32NBit);
    }
}

/**
  * @brief      Erase one block.
  * @param      u32Addr         Block to erase which contains the u32Addr.
  * @param      is4ByteAddr     4-byte u32Address or not.
  * @param      u32ErsCmd       Erase command.
  * @param      u32NBit         N-bit transmit/receive.
  * @param      isSync      Block or not.
  * @return     None.
  */
void SPIM_EraseBlock(uint32_t u32Addr, int is4ByteAddr, uint32_t u32ErsCmd, uint32_t u32NBit, int isSync)
{
    spim_set_write_enable(1, u32NBit); // Write Enable.

    do {
        uint8_t cmdBuf[16];
        uint8_t *cmdBufInd = (uint8_t *) cmdBuf;

        *cmdBufInd ++ = u32ErsCmd;

        if (is4ByteAddr) {
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 24);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 16);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 8);
            *cmdBufInd ++ = (uint8_t) u32Addr;
        } else {
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 16);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 8);
            *cmdBufInd ++ = (uint8_t) u32Addr;
        }

        _SPIM_SET_SS_EN(1);                 // CS activated.
        SwitchNBitOutput(u32NBit);
        spim_write(cmdBuf, cmdBufInd - (uint8_t *) cmdBuf);
        _SPIM_SET_SS_EN(0);                 // CS deactivated.
    } while (0);

    if (isSync)
        spim_wait_write_done(u32NBit);
}


/// @cond HIDDEN_SYMBOLS

/**
  * @brief      Write data in the same page by I/O mode.
  * @param      u32Addr     Start u32Address to write.
  * @param      is4ByteAddr 4-byte u32Address or not.
  * @param      u32NTx      Number of bytes to write.
  * @param      pu8TxBuf    Transmit buffer.
  * @param      wrCmd       Write command.
  * @param      u32NBitCmd  N-bit transmit command.
  * @param      u32NBitAddr N-bit transmit u32Address.
  * @param      u32NBitDat  N-bit transmit/receive data.
  * @param      isSync      Block or not.
  * @return     None.
  */
static void SPIM_WriteInPageDataByIo(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NTx, uint8_t *pu8TxBuf, uint32_t wrCmd,
                                     uint32_t u32NBitCmd, uint32_t u32NBitAddr, uint32_t u32NBitDat, int isSync)
{
    spim_set_write_enable(1, u32NBitCmd);                          // Write Enable.

    do {
        uint8_t cmdBuf[16];
        uint8_t *cmdBufOrig = (uint8_t *) cmdBuf;
        uint8_t *cmdBufInd = (uint8_t *) cmdBuf;

        _SPIM_SET_SS_EN(1);                         // CS activated.

        SwitchNBitOutput(u32NBitCmd);
        *cmdBufInd ++ = wrCmd;
        spim_write(cmdBufOrig, cmdBufInd - cmdBufOrig);   // Write out command.
        cmdBufOrig = cmdBufInd;

        if (is4ByteAddr) {
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 24);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 16);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 8);
            *cmdBufInd ++ = (uint8_t) u32Addr;
        } else {
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 16);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 8);
            *cmdBufInd ++ = (uint8_t) u32Addr;
        }
        SwitchNBitOutput(u32NBitAddr);
        spim_write(cmdBufOrig, cmdBufInd - cmdBufOrig);       // Write out u32Address.
        cmdBufOrig = cmdBufInd;

        SwitchNBitOutput(u32NBitDat);
        spim_write(pu8TxBuf, u32NTx);                               // Write out data.

        _SPIM_SET_SS_EN(0);                         // CS deactivated.
    } while (0);

    if (isSync) {
        spim_wait_write_done(u32NBitCmd);
    }
}

/**
  * @brief      Write data in the same page by Page Write mode.
  * @param      u32Addr     Start u32Address to write.
  * @param      is4ByteAddr 4-byte u32Address or not.
  * @param      u32NTx      Number of bytes to write.
  * @param      pu8TxBuf    Transmit buffer.
  * @param      wrCmd       Write command.
  * @param      isSync      Block or not.
  * @return     None.
  */
static void SPIM_WriteInPageDataByPageWrite(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NTx, uint8_t *pu8TxBuf,
        uint32_t wrCmd, int isSync)
{
    if (wrCmd == CMD_QUAD_PAGE_PROGRAM_WINBOND ||
            wrCmd == CMD_QUAD_PAGE_PROGRAM_MXIC) {
        SPIM_SetQuadEnable(1, 1);                             // Set Quad Enable.
    } else if (wrCmd == CMD_QUAD_PAGE_PROGRAM_EON) {
        SPIM_SetQuadEnable(1, 1);                             // Set Quad Enable.
        spim_eon_set_qpi_mode(1);                                   // Enter QPI mode.
    }

    _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_PAGEWRITE);         // Switch to Page Write mode.
    _SPIM_SET_SPIM_MODE(wrCmd);                               // SPIM mode.
    _SPIM_SET_4BYTE_ADDR_EN(is4ByteAddr);                     // Enable/disable 4-Byte Address.

    SPIM->SRAMADDR = (uint32_t) pu8TxBuf;                              // SRAM u32Address.
    SPIM->DMACNT = u32NTx;                                           // Transfer length.
    SPIM->FADDR = u32Addr;                                        // Flash u32Address.
    _SPIM_SET_GO();                                             // Go.

    if (isSync) {
        _SPIM_WAIT_FREE();
    }

    if (wrCmd == CMD_QUAD_PAGE_PROGRAM_EON) {
        spim_eon_set_qpi_mode(0);                                   // Exit QPI mode.
    }
}

/// @endcond HIDDEN_SYMBOLS

/**
  * @brief      Write data to SPI Flash by sending commands manually (I/O mode).
  * @param      u32Addr: Start u32Address to write.
  * @param      is4ByteAddr: 4-byte u32Address or not.
  * @param      u32NTx: Number of bytes to write.
  * @param      pu8TxBuf: Transmit buffer.
  * @param      wrCmd: Write command.
  * @param      u32NBitCmd: N-bit transmit command.
  * @param      u32NBitAddr: N-bit transmit u32Address.
  * @param      u32NBitDat: N-bit transmit/receive data.
  * @return     None.
  */
void SPIM_IO_Write(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NTx, uint8_t *pu8TxBuf, uint32_t wrCmd,
                   uint32_t u32NBitCmd, uint32_t u32NBitAddr, uint32_t u32NBitDat)
{
    do {
        uint32_t pageOffset = u32Addr % 256;

        if ((pageOffset + u32NTx) <= 256) {                    // Do all the bytes fit onto one page ?
            SPIM_WriteInPageDataByIo(u32Addr, is4ByteAddr, u32NTx, pu8TxBuf, wrCmd, u32NBitCmd, u32NBitAddr, u32NBitDat, 1);
        } else {
            uint32_t toWr = 256 - pageOffset;               // Size of data remaining on the first page.

            SPIM_WriteInPageDataByIo(u32Addr, is4ByteAddr, toWr, pu8TxBuf, wrCmd, u32NBitCmd, u32NBitAddr, u32NBitDat, 1);
            u32Addr += toWr;                                   // Advance indicator.
            u32NTx -= toWr;
            pu8TxBuf += toWr;

            while (u32NTx) {
                toWr = 256;
                if (toWr > u32NTx) {
                    toWr = u32NTx;
                }

                SPIM_WriteInPageDataByIo(u32Addr, is4ByteAddr, toWr, pu8TxBuf, wrCmd, u32NBitCmd, u32NBitAddr, u32NBitDat, 1);
                u32Addr += toWr;                               // Advance indicator.
                u32NTx -= toWr;
                pu8TxBuf += toWr;
            }
        }
    } while (0);
}

/**
  * @brief      Read data from SPI Flash by sending commands manually (I/O mode).
  * @param      u32Addr     Start u32Address to read.
  * @param      is4ByteAddr 4-byte u32Address or not.
  * @param      u32NRx      Number of bytes to read.
  * @param      pu8RxBuf    Receive buffer.
  * @param      u32RdCmd    Read command.
  * @param      u32NBitCmd  N-bit transmit command.
  * @param      u32NBitAddr N-bit transmit u32Address.
  * @param      u32NBitDat  N-bit transmit/receive data.
  * @param      u32NDummy   Number of dummy bytes following address.
  * @return     None.
  */
void SPIM_IO_Read(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NRx, uint8_t *pu8RxBuf, uint32_t u32RdCmd,
                  uint32_t u32NBitCmd, uint32_t u32NBitAddr, uint32_t u32NBitDat, int u32NDummy)
{
    do {
        uint8_t cmdBuf[16];
        uint8_t *cmdBufInd = (uint8_t *) cmdBuf;
        uint8_t *cmdBufOrig = (uint8_t *) cmdBuf;

        _SPIM_SET_SS_EN(1);                         // CS activated.

        *cmdBufInd ++ = u32RdCmd;
        SwitchNBitOutput(u32NBitCmd);
        spim_write(cmdBufOrig, cmdBufInd - cmdBufOrig);       // Write out command.
        cmdBufOrig = cmdBufInd;

        if (is4ByteAddr) {
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 24);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 16);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 8);
            *cmdBufInd ++ = (uint8_t) u32Addr;
        } else {
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 16);
            *cmdBufInd ++ = (uint8_t) (u32Addr >> 8);
            *cmdBufInd ++ = (uint8_t) u32Addr;
        }
        SwitchNBitOutput(u32NBitAddr);
        spim_write(cmdBufOrig, cmdBufInd - cmdBufOrig);       // Write out u32Address.
        cmdBufOrig = cmdBufInd;

        while (u32NDummy --) {
            *cmdBufInd ++ = 0x00;
        }
        // Same bit mode as above.
        spim_write(cmdBufOrig, cmdBufInd - cmdBufOrig);       // Write out dummy bytes.
        cmdBufOrig = cmdBufInd;

        SwitchNBitInput(u32NBitDat);
        spim_read(pu8RxBuf, u32NRx);                                // Read back data.

        _SPIM_SET_SS_EN(0);                     // CS deactivated.
    } while (0);
}

/**
  * @brief      Write data to SPI Flash by Page Write mode.
  * @param      u32Addr     Start address to write.
  * @param      is4ByteAddr 4-byte address or not.
  * @param      u32NTx      Number of bytes to write.
  * @param      pu8TxBuf    Transmit buffer.
  * @param      wrCmd       Write command.
  * @return     None.
  */
void SPIM_DMA_Write(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NTx, uint8_t *pu8TxBuf, uint32_t wrCmd)
{
    do {
        uint32_t pageOffset = u32Addr % 256;

        if ((pageOffset + u32NTx) <= 256) {            // Do all the bytes fit onto one page ?
            SPIM_WriteInPageDataByPageWrite(u32Addr, is4ByteAddr, u32NTx, pu8TxBuf, wrCmd, 1);
        } else {
            uint32_t toWr = 256 - pageOffset;       // Size of data remaining on the first page.

            SPIM_WriteInPageDataByPageWrite(u32Addr, is4ByteAddr, toWr, pu8TxBuf, wrCmd, 1);

            u32Addr += toWr;                                           // Advance indicator.
            u32NTx -= toWr;
            pu8TxBuf += toWr;

            while (u32NTx) {
                toWr = 256;
                if (toWr > u32NTx) {
                    toWr = u32NTx;
                }

                SPIM_WriteInPageDataByPageWrite(u32Addr, is4ByteAddr, toWr, pu8TxBuf, wrCmd, 1);

                u32Addr += toWr;                                           // Advance indicator.
                u32NTx -= toWr;
                pu8TxBuf += toWr;
            }
        }
    } while (0);
}

/**
  * @brief      Read data from SPI Flash by Page Read mode.
  * @param      u32Addr     Start address to read.
  * @param      is4ByteAddr 4-byte u32Address or not.
  * @param      u32NRx      Number of bytes to read.
  * @param      pu8RxBuf    Receive buffer.
  * @param      u32RdCmd    Read command.
  * @param      isSync      Block or not.
  * @return     None.
  */
void SPIM_DMA_Read(uint32_t u32Addr, int is4ByteAddr, uint32_t u32NRx, uint8_t *pu8RxBuf,
                   uint32_t u32RdCmd, int isSync)
{
    if ((u32RdCmd == CMD_DMA_FAST_QUAD_READ) || (u32RdCmd == CMD_DMA_NORMAL_QUAD_READ)) {
        SPIM_SetQuadEnable(1, 1);                   // Set Quad Enable.
    }

    _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_PAGEREAD);    // Switch to Page Read mode.
    _SPIM_SET_SPIM_MODE(u32RdCmd);                  // SPIM mode.
    _SPIM_SET_4BYTE_ADDR_EN(is4ByteAddr);           // Enable/disable 4-Byte Address.

    SPIM->SRAMADDR = (uint32_t) pu8RxBuf;           // SRAM u32Address.
    SPIM->DMACNT = u32NRx;                          // Transfer length.
    SPIM->FADDR = u32Addr;                          // Flash u32Address.
    _SPIM_SET_GO();                                 // Go.

    if (isSync) {
        _SPIM_WAIT_FREE();                          // Wait for DMA done.
    }
}

/**
  * @brief      Enter Direct Map mode.
  * @param      is4ByteAddr     4-byte u32Address or not.
  * @param      u32RdCmd        Read command.
  * @param      u32IdleIntvl    Idle interval.
  * @return     None.
  */
void SPIM_EnterDirectMapMode(int is4ByteAddr, uint32_t u32RdCmd, uint32_t u32IdleIntvl)
{
    if ((u32RdCmd == CMD_DMA_FAST_QUAD_READ) || (u32RdCmd == CMD_DMA_NORMAL_QUAD_READ)) {
        SPIM_SetQuadEnable(1, 1);               // Set Quad Enable.
    }

    _SPIM_SET_4BYTE_ADDR_EN(is4ByteAddr);       // Enable/disable 4-byte u32Address.
    _SPIM_SET_SPIM_MODE(u32RdCmd);              // SPIM mode.
    _SPI_SET_IDL_INTVL(u32IdleIntvl);           // Idle interval.
    _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_DIRECTMAP);     // Switch to Direct Map mode.
}

/**
  * @brief      Exit Direct Map mode.
  * @return     None.
  */
void SPIM_ExitDirectMapMode()
{
    _SPIM_SET_OPMODE(SPIM_CTL0_OPMODE_IO);  // Switch back to Normal mode.
}


/*@}*/ /* end of group M480_SPIM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_SPIM_Driver */

/*@}*/ /* end of group M480_Device_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
