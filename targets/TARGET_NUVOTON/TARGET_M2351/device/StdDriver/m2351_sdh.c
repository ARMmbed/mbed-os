/**************************************************************************//**
 * @file     SDH.c
 * @version  V1.00
 * @brief    M2351 SDH driver source file
 *
 * @note
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "M2351.h"

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup SDH_Driver SDH Driver
  @{
*/

/** @addtogroup SDH_EXPORTED_FUNCTIONS SDH Exported Functions
  @{
*/

#define SDH_BLOCK_SIZE   512

//#define DEBUG_PRINTF printf
#define DEBUG_PRINTF(...)

/// @cond HIDDEN_SYMBOLS

// global variables
// For response R3 (such as ACMD41, CRC-7 is invalid; but SD controller will still
// calculate CRC-7 and get an error result, software should ignore this error and clear SDISR [CRC_IF] flag
// _sd_uR3_CMD is the flag for it. 1 means software should ignore CRC-7 error
uint32_t _SDH_uR3_CMD = 0;
uint32_t _SDH_uR7_CMD = 0;
uint8_t volatile _SDH_SDDataReady = FALSE;

uint8_t *_SDH_pSDHCBuffer;
uint32_t _SDH_ReferenceClock;

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment = 4
uint8_t _SDH_ucSDHCBuffer[512];
#else
__attribute__((aligned)) uint8_t _SDH_ucSDHCBuffer[512];
#endif


int SDH_ok = 0;

SDH_INFO_T SD0;

void SDH_CheckRB(SDH_T *sdh)
{
    while(1)
    {
        sdh->CTL |= SDH_CTL_CLK8OEN_Msk;
        while(sdh->CTL & SDH_CTL_CLK8OEN_Msk);
        if(sdh->INTSTS & SDH_INTSTS_DAT0STS_Msk)
            break;
    }
}


int SDH_SDCommand(SDH_T *sdh, uint8_t ucCmd, uint32_t uArg)
{
    volatile int buf;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    sdh->CMDARG = uArg;
    buf = (sdh->CTL & (~SDH_CTL_CMDCODE_Msk)) | (ucCmd << 8) | (SDH_CTL_COEN_Msk);
    sdh->CTL = buf;

    while(sdh->CTL & SDH_CTL_COEN_Msk)
    {
        if(pSD->IsCardInsert == FALSE)
            return SDH_NO_SD_CARD;
    }
    return Successful;
}


int SDH_SDCmdAndRsp(SDH_T *sdh, uint8_t ucCmd, uint32_t uArg, int ntickCount)
{
    volatile int buf;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    sdh->CMDARG = uArg;
    buf = (sdh->CTL & (~SDH_CTL_CMDCODE_Msk)) | (ucCmd << 8) | (SDH_CTL_COEN_Msk | SDH_CTL_RIEN_Msk);
    sdh->CTL = buf;

    if(ntickCount > 0)
    {
        while(sdh->CTL & SDH_CTL_RIEN_Msk)
        {
            if(ntickCount-- == 0)
            {
                sdh->CTL |= SDH_CTL_CTLRST_Msk; // reset SD engine
                return 2;
            }
            if(pSD->IsCardInsert == FALSE)
                return SDH_NO_SD_CARD;
        }
    }
    else
    {
        while(sdh->CTL & SDH_CTL_RIEN_Msk)
        {
            if(pSD->IsCardInsert == FALSE)
                return SDH_NO_SD_CARD;
        }
    }

    if(_SDH_uR7_CMD)
    {
        if(((sdh->RESP1 & 0xff) != 0x55) && ((sdh->RESP0 & 0xf) != 0x01))
        {
            _SDH_uR7_CMD = 0;
            return SDH_CMD8_ERROR;
        }
    }

    if(!_SDH_uR3_CMD)
    {
        if(sdh->INTSTS & SDH_INTSTS_CRC7_Msk)      // check CRC7
            return Successful;
        else
        {
            return SDH_CRC7_ERROR;
        }
    }
    else     // ignore CRC error for R3 case
    {
        _SDH_uR3_CMD = 0;
        sdh->INTSTS = SDH_INTSTS_CRCIF_Msk;
        return Successful;
    }
}


int SDH_Swap32(int val)
{
    int buf;

    buf = val;
    val <<= 24;
    val |= (buf << 8U) & 0xff0000;
    val |= (buf >> 8U) & 0xff00;
    val |= (buf >> 24U) & 0xff;
    return val;
}

// Get 16 bytes CID or CSD
int SDH_SDCmdAndRsp2(SDH_T *sdh, uint8_t ucCmd, uint32_t uArg, uint32_t *puR2ptr)
{
    unsigned int i, buf;
    unsigned int tmpBuf[5];
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    sdh->CMDARG = uArg;
    buf = (sdh->CTL & (~SDH_CTL_CMDCODE_Msk)) | (ucCmd << 8U) | (SDH_CTL_COEN_Msk | SDH_CTL_R2EN_Msk);
    sdh->CTL = buf;

    while(sdh->CTL & SDH_CTL_R2EN_Msk)
    {
        if(pSD->IsCardInsert == FALSE)
            return SDH_NO_SD_CARD;
    }

    if(sdh->INTSTS & SDH_INTSTS_CRC7_Msk)
    {
        for(i = 0; i < 5; i++)
        {
            tmpBuf[i] = SDH_Swap32(sdh->FB[i]);
        }
        for(i = 0; i < 4; i++)
            *puR2ptr++ = ((tmpBuf[i] & 0x00ffffff) << 8) | ((tmpBuf[i + 1] & 0xff000000) >> 24);
        return Successful;
    }
    else
        return SDH_CRC7_ERROR;
}


int SDH_SDCmdAndRspDataIn(SDH_T *sdh, uint8_t ucCmd, uint32_t uArg)
{
    volatile int buf;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    sdh->CMDARG = uArg;
    buf = (sdh->CTL & (~SDH_CTL_CMDCODE_Msk)) | (ucCmd << 8) |
          (SDH_CTL_COEN_Msk | SDH_CTL_RIEN_Msk | SDH_CTL_DIEN_Msk);

    sdh->CTL = buf;

    while(sdh->CTL & SDH_CTL_RIEN_Msk)
    {
        if(pSD->IsCardInsert == FALSE)
            return SDH_NO_SD_CARD;
    }

    while(sdh->CTL & SDH_CTL_DIEN_Msk)
    {
        if(pSD->IsCardInsert == FALSE)
            return SDH_NO_SD_CARD;
    }

    if(!(sdh->INTSTS & SDH_INTSTS_CRC7_Msk))       // check CRC7
    {
        return SDH_CRC7_ERROR;
    }

    if(!(sdh->INTSTS & SDH_INTSTS_CRC16_Msk))      // check CRC16
    {
        return SDH_CRC16_ERROR;
    }
    return 0;
}

// there are 8 bits for divider0, maximum is 256
#define SDH_CLK_DIV0_MAX     256

void SDH_Set_clock(SDH_T *sdh, uint32_t sd_clock_khz)
{
    if(!(__PC() & (1 << 28)))
    {
        uint32_t rate, div1;
        static uint32_t u32SD_ClkSrc = 0;

        /* M2351 is only support SDH0 */
        u32SD_ClkSrc = (CLK->CLKSEL0 & CLK_CLKSEL0_SDH0SEL_Msk);
        if(u32SD_ClkSrc == CLK_CLKSEL0_SDH0SEL_HXT)
        {
            _SDH_ReferenceClock = (CLK_GetHXTFreq() / 1000);
        }
        else if(u32SD_ClkSrc == CLK_CLKSEL0_SDH0SEL_HIRC)
        {
            _SDH_ReferenceClock = (__HIRC / 1000);
        }
        else if(u32SD_ClkSrc == CLK_CLKSEL0_SDH0SEL_PLL)
        {
            _SDH_ReferenceClock = (CLK_GetPLLClockFreq() / 1000);
        }
        else if(u32SD_ClkSrc == CLK_CLKSEL0_SDH0SEL_HCLK)
        {
            _SDH_ReferenceClock = (CLK_GetHCLKFreq() / 1000);
        }

        if(sd_clock_khz >= 50000)
        {
            sd_clock_khz = 50000;
        }
        rate = _SDH_ReferenceClock / sd_clock_khz;

        // choose slower clock if system clock cannot divisible by wanted clock
        if(_SDH_ReferenceClock % sd_clock_khz != 0)
            rate++;

        if(rate >= SDH_CLK_DIV0_MAX)
        {
            rate = SDH_CLK_DIV0_MAX;
        }

        //--- calculate the second divider CLKDIV0[SDHOST_N]
        if (rate == 0)
        {
            div1 = 0;
        }
        else
        {
            div1 = ((rate - 1) & 0xFF);
        }

        //--- setup register
        /* M2351 is only support SDH0 */
        CLK->CLKDIV0 &= ~CLK_CLKDIV0_SDH0DIV_Msk;
        CLK->CLKDIV0 |= (div1 << CLK_CLKDIV0_SDH0DIV_Pos);
    }
}

//uint32_t SDH_CardDetection(SDH_T *sdh)
unsigned int SDH_CardDetection(SDH_T *sdh)
{
    uint32_t i;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    if(sdh->INTEN & SDH_INTEN_CDSRC0_Msk)   // Card detect pin from GPIO
    {
        if(sdh->INTSTS & SDH_INTSTS_CDSTS0_Msk)   // Card remove
        {
            pSD->IsCardInsert = FALSE;
            return FALSE;
        }
        else
            pSD->IsCardInsert = TRUE;
    }
    else if(!(sdh->INTEN & SDH_INTEN_CDSRC0_Msk))
    {
        sdh->CTL |= SDH_CTL_CLKKEEP0_Msk;
        for(i = 0; i < 5000; i++);

        if(sdh->INTSTS & SDH_INTSTS_CDSTS0_Msk) // Card insert
            pSD->IsCardInsert = TRUE;
        else
        {
            pSD->IsCardInsert = FALSE;
            return FALSE;
        }

        sdh->CTL &= ~SDH_CTL_CLKKEEP0_Msk;
    }

    return TRUE;
}


// Initial
int SDH_Init(SDH_T *sdh)
{
    int volatile i, status;
    unsigned int resp;
    unsigned int CIDBuffer[4];
    unsigned int volatile u32CmdTimeOut;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    // set the clock to 300KHz
    SDH_Set_clock(sdh, 300);

    // power ON 74 clock
    sdh->CTL |= SDH_CTL_CLK74OEN_Msk;

    while(sdh->CTL & SDH_CTL_CLK74OEN_Msk)
    {
        if(pSD->IsCardInsert == FALSE)
            return SDH_NO_SD_CARD;
    }

    SDH_SDCommand(sdh, 0, 0);        // reset all cards
    for(i = 0x1000; i > 0; i--);

    // initial SDHC
    _SDH_uR7_CMD = 1;
    //u32CmdTimeOut = 5000;
    u32CmdTimeOut = 0xFFFFF;
    //u32CmdTimeOut = 0;

    i = SDH_SDCmdAndRsp(sdh, 8, 0x00000155, u32CmdTimeOut);
    if(i == Successful)
    {
        // SD 2.0
        SDH_SDCmdAndRsp(sdh, 55, 0x00, u32CmdTimeOut);
        _SDH_uR3_CMD = 1;
        SDH_SDCmdAndRsp(sdh, 41, 0x40ff8000, u32CmdTimeOut); // 2.7v-3.6v
        resp = sdh->RESP0;

        while(!(resp & 0x00800000))         // check if card is ready
        {
            SDH_SDCmdAndRsp(sdh, 55, 0x00, u32CmdTimeOut);
            _SDH_uR3_CMD = 1;
            SDH_SDCmdAndRsp(sdh, 41, 0x40ff8000, u32CmdTimeOut); // 3.0v-3.4v
            resp = sdh->RESP0;
        }
        if(resp & 0x00400000)
            pSD->CardType = SDH_TYPE_SD_HIGH;
        else
            pSD->CardType = SDH_TYPE_SD_LOW;
    }
    else
    {
        // SD 1.1
        SDH_SDCommand(sdh, 0, 0);        // reset all cards
        for(i = 0x100; i > 0; i--);

        i = SDH_SDCmdAndRsp(sdh, 55, 0x00, u32CmdTimeOut);
        if(i == 2)      // MMC memory
        {

            SDH_SDCommand(sdh, 0, 0);        // reset
            for(i = 0x100; i > 0; i--);

            _SDH_uR3_CMD = 1;

            if(SDH_SDCmdAndRsp(sdh, 1, 0x40ff8000, u32CmdTimeOut) != 2)     // eMMC memory
            {
                resp = sdh->RESP0;
                while(!(resp & 0x00800000))         // check if card is ready
                {
                    _SDH_uR3_CMD = 1;

                    SDH_SDCmdAndRsp(sdh, 1, 0x40ff8000, u32CmdTimeOut);      // high voltage
                    resp = sdh->RESP0;
                }

                if(resp & 0x00400000)
                    pSD->CardType = SDH_TYPE_EMMC;
                else
                    pSD->CardType = SDH_TYPE_MMC;
            }
            else
            {
                pSD->CardType = SDH_TYPE_UNKNOWN;
                return SDH_ERR_DEVICE;
            }
        }
        else if(i == 0)      // SD Memory
        {
            _SDH_uR3_CMD = 1;
            SDH_SDCmdAndRsp(sdh, 41, 0x00ff8000, u32CmdTimeOut); // 3.0v-3.4v
            resp = sdh->RESP0;
            while(!(resp & 0x00800000))         // check if card is ready
            {
                SDH_SDCmdAndRsp(sdh, 55, 0x00, u32CmdTimeOut);
                _SDH_uR3_CMD = 1;
                SDH_SDCmdAndRsp(sdh, 41, 0x00ff8000, u32CmdTimeOut); // 3.0v-3.4v
                resp = sdh->RESP0;
            }
            pSD->CardType = SDH_TYPE_SD_LOW;
        }
        else
        {
            pSD->CardType = SDH_TYPE_UNKNOWN;
            return SDH_INIT_ERROR;
        }
    }

    // CMD2, CMD3
    if(pSD->CardType != SDH_TYPE_UNKNOWN)
    {
        SDH_SDCmdAndRsp2(sdh, 2, 0x00, CIDBuffer);
        if((pSD->CardType == SDH_TYPE_MMC) || (pSD->CardType == SDH_TYPE_EMMC))
        {
            if((status = SDH_SDCmdAndRsp(sdh, 3, 0x10000, 0)) != Successful)         // set RCA
                return status;
            pSD->RCA = 0x10000;
        }
        else
        {
            if((status = SDH_SDCmdAndRsp(sdh, 3, 0x00, 0)) != Successful)        // get RCA
                return status;
            else
                pSD->RCA = (sdh->RESP0 << 8) & 0xffff0000;
        }
    }
    if (pSD->CardType == SDH_TYPE_SD_HIGH)
    {
        DEBUG_PRINTF("This is a SDHC card\n");
    }
    else if (pSD->CardType == SDH_TYPE_SD_LOW)
    {
        DEBUG_PRINTF("This is a SD card\n");
    }

    return Successful;
}


int SDH_SwitchToHighSpeed(SDH_T *sdh, SDH_INFO_T *pSD)
{
    int volatile status = 0;
    uint16_t current_comsumption, busy_status0;

    sdh->DMASA = (uint32_t)_SDH_pSDHCBuffer;    // set DMA transfer starting address
    sdh->BLEN = 63;    // 512 bit

    if((status = SDH_SDCmdAndRspDataIn(sdh, 6, 0x00ffff01)) != Successful)
        return Fail;

    current_comsumption = _SDH_pSDHCBuffer[0] << 8 | _SDH_pSDHCBuffer[1];
    if(!current_comsumption)
        return Fail;

    busy_status0 = _SDH_pSDHCBuffer[28] << 8 | _SDH_pSDHCBuffer[29];

    if(!busy_status0)    // function ready
    {
        sdh->DMASA = (uint32_t)_SDH_pSDHCBuffer;        // set DMA transfer starting address
        sdh->BLEN = 63;    // 512 bit

        if((status = SDH_SDCmdAndRspDataIn(sdh, 6, 0x80ffff01)) != Successful)
            return Fail;

        // function change timing: 8 clocks
        sdh->CTL |= SDH_CTL_CLK8OEN_Msk;
        while(sdh->CTL & SDH_CTL_CLK8OEN_Msk);

        current_comsumption = _SDH_pSDHCBuffer[0] << 8 | _SDH_pSDHCBuffer[1];
        if(!current_comsumption)
            return Fail;

        return Successful;
    }
    else
        return Fail;
}


int SDH_SelectCardType(SDH_T *sdh)
{
    int volatile status = 0;
    unsigned int arg;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    if((status = SDH_SDCmdAndRsp(sdh, 7, pSD->RCA, 0)) != Successful)
        return status;

    SDH_CheckRB(sdh);

    // if SD card set 4bit
    if(pSD->CardType == SDH_TYPE_SD_HIGH)
    {
        _SDH_pSDHCBuffer = (uint8_t *)((uint32_t)_SDH_ucSDHCBuffer);
        sdh->DMASA = (uint32_t)_SDH_pSDHCBuffer;    // set DMA transfer starting address
        sdh->BLEN = 0x07;  // 64 bit

        if((status = SDH_SDCmdAndRsp(sdh, 55, pSD->RCA, 0)) != Successful)
            return status;
        if((status = SDH_SDCmdAndRspDataIn(sdh, 51, 0x00)) != Successful)
            return status;

        if((_SDH_ucSDHCBuffer[0] & 0xf) == 0x2)
        {
            status = SDH_SwitchToHighSpeed(sdh, pSD);
            if(status == Successful)
            {
                /* divider */
                SDH_Set_clock(sdh, SDHC_FREQ);
            }
        }

        if((status = SDH_SDCmdAndRsp(sdh, 55, pSD->RCA, 0)) != Successful)
            return status;
        if((status = SDH_SDCmdAndRsp(sdh, 6, 0x02, 0)) != Successful)    // set bus width
            return status;

        sdh->CTL |= SDH_CTL_DBW_Msk;
    }
    else if(pSD->CardType == SDH_TYPE_SD_LOW)
    {
        _SDH_pSDHCBuffer = (uint8_t *)((uint32_t)_SDH_ucSDHCBuffer);
        sdh->DMASA = (uint32_t) _SDH_pSDHCBuffer; // set DMA transfer starting address
        sdh->BLEN = 0x07;  // 64 bit

        if((status = SDH_SDCmdAndRsp(sdh, 55, pSD->RCA, 0)) != Successful)
            return status;
        if((status = SDH_SDCmdAndRspDataIn(sdh, 51, 0x00)) != Successful)
            return status;

        // set data bus width. ACMD6 for SD card, SDCR_DBW for host.
        if((status = SDH_SDCmdAndRsp(sdh, 55, pSD->RCA, 0)) != Successful)
            return status;

        if((status = SDH_SDCmdAndRsp(sdh, 6, 0x02, 0)) != Successful)    // set bus width
            return status;

        sdh->CTL |= SDH_CTL_DBW_Msk;
    }
    else if((pSD->CardType == SDH_TYPE_MMC) || (pSD->CardType == SDH_TYPE_EMMC))
    {

        if(pSD->CardType == SDH_TYPE_MMC)
            sdh->CTL &= ~SDH_CTL_DBW_Msk;

        //--- sent CMD6 to MMC card to set bus width to 4 bits mode
        // set CMD6 argument Access field to 3, Index to 183, Value to 1 (4-bit mode)
        arg = (3 << 24) | (183 << 16) | (1 << 8);
        if((status = SDH_SDCmdAndRsp(sdh, 6, arg, 0)) != Successful)
            return status;
        SDH_CheckRB(sdh);

        sdh->CTL |= SDH_CTL_DBW_Msk;; // set bus width to 4-bit mode for SD host controller

    }

    if((status = SDH_SDCmdAndRsp(sdh, 16, SDH_BLOCK_SIZE, 0)) != Successful)  // set block length
        return status;
    sdh->BLEN = SDH_BLOCK_SIZE - 1;           // set the block size

    SDH_SDCommand(sdh, 7, 0);
    sdh->CTL |= SDH_CTL_CLK8OEN_Msk;
    while(sdh->CTL & SDH_CTL_CLK8OEN_Msk);

    sdh->INTEN |= SDH_INTEN_BLKDIEN_Msk;

    return Successful;
}

void SDH_Get_SD_info(SDH_T *sdh)
{
    unsigned int R_LEN, C_Size, MULT, size;
    unsigned int Buffer[4];
    unsigned char *ptr;
    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    SDH_SDCmdAndRsp2(sdh, 9, pSD->RCA, (uint32_t *)Buffer);

    if((pSD->CardType == SDH_TYPE_MMC) || (pSD->CardType == SDH_TYPE_EMMC))
    {
        // for MMC/eMMC card
        if((Buffer[0] & 0xc0000000) == 0xc0000000)
        {
            // CSD_STRUCTURE [127:126] is 3
            // CSD version depend on EXT_CSD register in eMMC v4.4 for card size > 2GB
            SDH_SDCmdAndRsp(sdh, 7, pSD->RCA, 0);

            ptr = (uint8_t *)((uint32_t)_SDH_ucSDHCBuffer);
            sdh->DMASA = (uint32_t)ptr;  // set DMA transfer starting address
            sdh->BLEN = 511;  // read 512 bytes for EXT_CSD

            if(SDH_SDCmdAndRspDataIn(sdh, 8, 0x00) != Successful)
                return;

            SDH_SDCommand(sdh, 7, 0);
            sdh->CTL |= SDH_CTL_CLK8OEN_Msk;
            while(sdh->CTL & SDH_CTL_CLK8OEN_Msk);

            pSD->totalSectorN = (*(unsigned int *)(ptr + 212));
            pSD->diskSize = pSD->totalSectorN / 2;
        }
        else
        {
            // CSD version v1.0/1.1/1.2 in eMMC v4.4 spec for card size <= 2GB
            R_LEN = (Buffer[1] & 0x000f0000) >> 16;
            C_Size = ((Buffer[1] & 0x000003ff) << 2) | ((Buffer[2] & 0xc0000000) >> 30);
            MULT = (Buffer[2] & 0x00038000) >> 15;
            size = (C_Size + 1) * (1 << (MULT + 2)) * (1 << R_LEN);

            pSD->diskSize = size / 1024;
            pSD->totalSectorN = size / 512;
        }
    }
    else
    {
        if(Buffer[0] & 0xc0000000)
        {
            C_Size = ((Buffer[1] & 0x0000003f) << 16) | ((Buffer[2] & 0xffff0000) >> 16);
            size = (C_Size + 1) * 512;  // Kbytes

            pSD->diskSize = size;
            pSD->totalSectorN = size << 1;
        }
        else
        {
            R_LEN = (Buffer[1] & 0x000f0000) >> 16;
            C_Size = ((Buffer[1] & 0x000003ff) << 2) | ((Buffer[2] & 0xc0000000) >> 30);
            MULT = (Buffer[2] & 0x00038000) >> 15;
            size = (C_Size + 1) * (1 << (MULT + 2)) * (1 << R_LEN);

            pSD->diskSize = size / 1024;
            pSD->totalSectorN = size / 512;
        }
    }
    pSD->sectorSize = 512;
    DEBUG_PRINTF("The size is %d MB\n\n", pSD->diskSize/1024);
}

/// @endcond HIDDEN_SYMBOLS


/**
 *  @brief  This function use to reset SD function and select card detection source and pin.
 *
 *  @param[in]  sdh    The pointer of the specified SDH module.
 *  @param[in]  u32CardDetSrc   Select card detection pin from GPIO or DAT3 pin. ( \ref CardDetect_From_GPIO / \ref CardDetect_From_DAT3)
 *
 *  @return None
 */
void SDH_Open(SDH_T *sdh, uint32_t u32CardDetSrc)
{
    // enable DMAC
    sdh->DMACTL = SDH_DMACTL_DMARST_Msk;
    while(sdh->DMACTL & SDH_DMACTL_DMARST_Msk);

    sdh->DMACTL = SDH_DMACTL_DMAEN_Msk;

    //Reset FMI
    sdh->GCTL = SDH_GCTL_GCTLRST_Msk | SDH_GCTL_SDEN_Msk;        // Start reset FMI controller.
    while(sdh->GCTL & SDH_GCTL_GCTLRST_Msk);

    memset(&SD0, 0, sizeof(SDH_INFO_T));

    // enable SD
    sdh->GCTL = SDH_GCTL_SDEN_Msk;

    if(u32CardDetSrc & CardDetect_From_DAT3)
    {
        DEBUG_PRINTF("CardDetect_From_DAT3\n");
        sdh->INTEN &= ~SDH_INTEN_CDSRC0_Msk;
    }
    else
    {
        DEBUG_PRINTF("CardDetect_From_GPIO\n");
        sdh->INTEN |= SDH_INTEN_CDSRC0_Msk;
    }
    sdh->INTEN |= SDH_INTEN_CDIEN0_Msk;

    sdh->CTL |= SDH_CTL_CTLRST_Msk;     // SD software reset
    while(sdh->CTL & SDH_CTL_CTLRST_Msk);

}

/**
 *  @brief  This function use to initial SD card.
 *
 *  @param[in]    sdh    The pointer of the specified SDH module.
 *
 *  @return None
 *
 *  @details This function is used to initial SD card.
 *           SD initial state needs 400KHz clock output, driver will use HIRC for SD initial clock source.
 *           And then switch back to the user's setting.
 */
uint32_t SDH_Probe(SDH_T *sdh)
{
    uint32_t val;

    // Disable FMI/SD host interrupt
    sdh->GINTEN = 0U;

    sdh->CTL &= ~SDH_CTL_SDNWR_Msk;
    sdh->CTL |=  0x09UL << SDH_CTL_SDNWR_Pos;         // set SDNWR = 9
    sdh->CTL &= ~SDH_CTL_BLKCNT_Msk;
    sdh->CTL |=  0x01UL << SDH_CTL_BLKCNT_Pos;           // set BLKCNT = 1
    sdh->CTL &= ~SDH_CTL_DBW_Msk;               // SD 1-bit data bus

    if(!(SDH_CardDetection(sdh)))
        return SDH_NO_SD_CARD;

    if((val = SDH_Init(sdh)) != 0U)
        return val;

    /* divider */
    if(SD0.CardType == SDH_TYPE_MMC)
        SDH_Set_clock(sdh, MMC_FREQ);
    else
        SDH_Set_clock(sdh, SD_FREQ);
    SDH_Get_SD_info(sdh);

    if((val = SDH_SelectCardType(sdh)) != 0U)
        return val;

    SDH_ok = 1;
    return 0;
}

/**
 *  @brief  This function use to read data from SD card.
 *
 *  @param[in]     sdh           The pointer of the specified SDH module.
 *  @param[out]    pu8BufAddr    The buffer to receive the data from SD card.
 *  @param[in]     u32StartSec   The start read sector address.
 *  @param[in]     u32SecCount   The the read sector number of data
 *
 *  @return None
 */
uint32_t SDH_Read(SDH_T *sdh, uint8_t *pu8BufAddr, uint32_t u32StartSec, uint32_t u32SecCount)
{
    char volatile bIsSendCmd = FALSE, buf;
    unsigned int volatile reg;
    int volatile i, loop, status;
    uint32_t blksize = SDH_BLOCK_SIZE;

    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    //--- check input parameters
    if(u32SecCount == 0U)
    {
        return SDH_SELECT_ERROR;
    }

    if((status = SDH_SDCmdAndRsp(sdh, 7, pSD->RCA, 0)) != Successful)
        return status;
    SDH_CheckRB(sdh);

    sdh->BLEN = blksize - 1U;       // the actual byte count is equal to (SDBLEN+1)

    if((pSD->CardType == SDH_TYPE_SD_HIGH) || (pSD->CardType == SDH_TYPE_EMMC))
        sdh->CMDARG = u32StartSec;
    else
        sdh->CMDARG = u32StartSec * blksize;

    sdh->DMASA = (uint32_t)pu8BufAddr;

    loop = u32SecCount / 255U;
    for(i = 0; i < loop; i++)
    {
        _SDH_SDDataReady = FALSE;
        reg = sdh->CTL & ~SDH_CTL_CMDCODE_Msk;
        reg = reg | 0xff0000UL;   // set BLK_CNT to 255
        if(bIsSendCmd == FALSE)
        {
            sdh->CTL = reg | (18U << 8U) | (SDH_CTL_COEN_Msk | SDH_CTL_RIEN_Msk | SDH_CTL_DIEN_Msk);
            bIsSendCmd = TRUE;
        }
        else
            sdh->CTL = reg | SDH_CTL_DIEN_Msk;

        while(!_SDH_SDDataReady)
        {
            if(_SDH_SDDataReady) break;
            if(pSD->IsCardInsert == FALSE)
                return SDH_NO_SD_CARD;
        }

        if (!(sdh->INTSTS & SDH_INTSTS_CRC7_Msk))
        {    // check CRC7
            //DEBUG_PRINTF("sdioSD_Read_in_blksize(): response error!\n");
            return SDH_CRC7_ERROR;
        }

        if (!(sdh->INTSTS & SDH_INTSTS_CRC16_Msk))
        {   // check CRC16
            //DEBUG_PRINTF("sdioSD_Read_in_blksize() :read data error!\n");
            return SDH_CRC16_ERROR;
        }
    }

    loop = u32SecCount % 255U;
    if(loop != 0)
    {
        _SDH_SDDataReady = FALSE;
        reg = sdh->CTL & (~SDH_CTL_CMDCODE_Msk);
        reg = reg & (~SDH_CTL_BLKCNT_Msk);
        reg |= (loop << 16U);    // setup SDCR_BLKCNT

        if(bIsSendCmd == FALSE)
        {
            sdh->CTL = reg | (18U << 8U) | (SDH_CTL_COEN_Msk | SDH_CTL_RIEN_Msk | SDH_CTL_DIEN_Msk);
            bIsSendCmd = TRUE;
        }
        else
            sdh->CTL = reg | SDH_CTL_DIEN_Msk;

        while(!_SDH_SDDataReady)
        {
            if(pSD->IsCardInsert == FALSE)
                return SDH_NO_SD_CARD;
        }

        if (!(sdh->INTSTS & SDH_INTSTS_CRC7_Msk))
        {
            // check CRC7
            //DEBUG_PRINTF("sdioSD_Read_in_blksize(): response error!\n");
            return SDH_CRC7_ERROR;
        }

        if (!(sdh->INTSTS & SDH_INTSTS_CRC16_Msk))
        {
            // check CRC16
            //DEBUG_PRINTF("sdioSD_Read_in_blksize(): read data error!\n");
            return SDH_CRC16_ERROR;
        }
    }

    if (SDH_SDCmdAndRsp(sdh, 12, 0, 0))
    {
        // stop command
        //DEBUG_PRINTF("stop command fail !!\n");
        return SDH_CRC7_ERROR;
    }
    SDH_CheckRB(sdh);

    SDH_SDCommand(sdh, 7, 0);
    sdh->CTL |= SDH_CTL_CLK8OEN_Msk;
    while(sdh->CTL & SDH_CTL_CLK8OEN_Msk);

    return Successful;
}


/**
 *  @brief  This function use to write data to SD card.
 *
 *  @param[in]    sdh           The pointer of the specified SDH module.
 *  @param[in]    pu8BufAddr    The buffer to send the data to SD card.
 *  @param[in]    u32StartSec   The start write sector address.
 *  @param[in]    u32SecCount   The the write sector number of data.
 *
 *  @return   \ref SDH_SELECT_ERROR : u32SecCount is zero. \n
 *            \ref SDH_NO_SD_CARD : SD card be removed. \n
 *            \ref SDH_CRC_ERROR : CRC error happen. \n
 *            \ref SDH_CRC7_ERROR : CRC7 error happen. \n
 *            \ref Successful : Write data to SD card success.
 */
uint32_t SDH_Write(SDH_T *sdh, uint8_t *pu8BufAddr, uint32_t u32StartSec, uint32_t u32SecCount)
{
    char volatile bIsSendCmd = FALSE;
    unsigned int volatile reg;
    int volatile i, loop, status;

    SDH_INFO_T *pSD;

    /* M2351 is only support SDH0 */
    pSD = &SD0;

    //--- check input parameters
    if(u32SecCount == 0U)
    {
        return SDH_SELECT_ERROR;
    }

    if((status = SDH_SDCmdAndRsp(sdh, 7, pSD->RCA, 0)) != Successful)
        return status;

    SDH_CheckRB(sdh);

    // According to SD Spec v2.0, the write CMD block size MUST be 512, and the start address MUST be 512*n.
    sdh->BLEN = SDH_BLOCK_SIZE - 1U;           // set the block size

    if((pSD->CardType == SDH_TYPE_SD_HIGH) || (pSD->CardType == SDH_TYPE_EMMC))
        sdh->CMDARG = u32StartSec;
    else
        sdh->CMDARG = u32StartSec * SDH_BLOCK_SIZE;  // set start address for SD CMD

    sdh->DMASA = (uint32_t)pu8BufAddr;
    loop = u32SecCount / 255U;   // the maximum block count is 0xFF=255 for register SDCR[BLK_CNT]
    for(i = 0; i < loop; i++)
    {
        _SDH_SDDataReady = FALSE;
        reg = sdh->CTL & 0xff00c080UL;
        reg = reg | 0xff0000UL;   // set BLK_CNT to 0xFF=255
        if(!bIsSendCmd)
        {
            sdh->CTL = reg | (25U << 8U) | (SDH_CTL_COEN_Msk | SDH_CTL_RIEN_Msk | SDH_CTL_DOEN_Msk);
            bIsSendCmd = TRUE;
        }
        else
            sdh->CTL = reg | SDH_CTL_DOEN_Msk;

        while(!_SDH_SDDataReady)
        {
            if(pSD->IsCardInsert == FALSE)
                return SDH_NO_SD_CARD;
        }

        if((sdh->INTSTS & SDH_INTSTS_CRCIF_Msk) != 0U)      // check CRC
        {
            sdh->INTSTS = SDH_INTSTS_CRCIF_Msk;
            return SDH_CRC_ERROR;
        }
    }

    loop = u32SecCount % 255U;
    if(loop != 0U)
    {
        _SDH_SDDataReady = FALSE;
        reg = (sdh->CTL & 0xff00c080UL) | (loop << 16U);
        if(!bIsSendCmd)
        {
            sdh->CTL = reg | (25U << 8U) | (SDH_CTL_COEN_Msk | SDH_CTL_RIEN_Msk | SDH_CTL_DOEN_Msk);
            bIsSendCmd = TRUE;
        }
        else
            sdh->CTL = reg | SDH_CTL_DOEN_Msk;

        while(!_SDH_SDDataReady)
        {
            if(pSD->IsCardInsert == FALSE)
                return SDH_NO_SD_CARD;
        }

        if((sdh->INTSTS & SDH_INTSTS_CRCIF_Msk) != 0U)      // check CRC
        {
            sdh->INTSTS = SDH_INTSTS_CRCIF_Msk;
            return SDH_CRC_ERROR;
        }
    }
    sdh->INTSTS = SDH_INTSTS_CRCIF_Msk;

    if(SDH_SDCmdAndRsp(sdh, 12, 0, 0))       // stop command
    {
        return SDH_CRC7_ERROR;
    }
    SDH_CheckRB(sdh);

    SDH_SDCommand(sdh, 7, 0);
    sdh->CTL |= SDH_CTL_CLK8OEN_Msk;
    while(sdh->CTL & SDH_CTL_CLK8OEN_Msk);

    return Successful;
}


/*@}*/ /* end of group SDH_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group SDH_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/

