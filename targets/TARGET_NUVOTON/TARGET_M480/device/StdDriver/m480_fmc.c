/**************************************************************************//**
 * @file     fmc.c
 * @version  V1.00
 * @brief    M480 series FMC driver source file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

//* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "M480.h"


/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_FMC_Driver FMC Driver
  @{
*/


/** @addtogroup M480_FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/


/**
  * @brief Disable FMC ISP function.
  * @return None
  */
void FMC_Close(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk;
}


/**
  * @brief Execute FMC_ISPCMD_PAGE_ERASE command to erase a flash page. The page size is 4096 bytes.
  * @param[in]  u32PageAddr Address of the flash page to be erased.
  *             It must be a 4096 bytes aligned address.
  * @return ISP page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  */
int32_t FMC_Erase(uint32_t u32PageAddr)
{
    if (u32PageAddr == FMC_SPROM_BASE)
        return FMC_Erase_SPROM();

    FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
    FMC->ISPADDR = u32PageAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        return -1;
    }
    return 0;
}


/**
  * @brief Execute FMC_ISPCMD_PAGE_ERASE command to erase SPROM. The page size is 4096 bytes.
  * @return   SPROM page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  */
int32_t FMC_Erase_SPROM(void)
{
    FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
    FMC->ISPADDR = FMC_SPROM_BASE;
    FMC->ISPDAT = 0x0055AA03;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        return -1;
    }
    return 0;
}

/**
  * @brief Execute FMC_ISPCMD_BLOCK_ERASE command to erase a flash block. The block size is 4 pages.
  * @param[in]  u32BlockAddr  Address of the flash block to be erased.
  *             It must be a 4 pages aligned address.
  * @return ISP page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  */
int32_t FMC_Erase_Block(uint32_t u32BlockAddr)
{
    FMC->ISPCMD = FMC_ISPCMD_BLOCK_ERASE;
    FMC->ISPADDR = u32BlockAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        return -1;
    }
    return 0;
}

/**
  * @brief Execute FMC_ISPCMD_BANK_ERASE command to erase a flash block.
  * @param[in]  u32BankAddr Base address of the flash bank to be erased.
  * @return ISP page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  */
int32_t FMC_Erase_Bank(uint32_t u32BankAddr)
{
    FMC->ISPCMD = FMC_ISPCMD_BANK_ERASE;
    FMC->ISPADDR = u32BankAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        return -1;
    }
    return 0;
}

/**
  * @brief Get the current boot source.
  * @return The current boot source.
  * @retval   0  Is boot from APROM.
  * @retval   1  Is boot from LDROM.
  */
int32_t FMC_GetBootSource (void)
{
    if (FMC->ISPCTL & FMC_ISPCTL_BS_Msk)
        return 1;
    else
        return 0;
}


/**
  * @brief Enable FMC ISP function
  * @return None
  */
void FMC_Open(void)
{
    FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk;
}


/**
  * @brief Execute FMC_ISPCMD_READ command to read a word from flash.
  * @param[in]  u32Addr Address of the flash location to be read.
  *             It must be a word aligned address.
  * @return The word data read from specified flash address.
  */
uint32_t FMC_Read(uint32_t u32Addr)
{
    FMC->ISPCMD = FMC_ISPCMD_READ;
    FMC->ISPADDR = u32Addr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    return FMC->ISPDAT;
}


int FMC_Read_64(uint32_t u32addr, uint32_t * u32data0, uint32_t * u32data1)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_64;
    FMC->ISPADDR    = u32addr;
    FMC->ISPDAT = 0x0;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;

    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }

    *u32data0 = FMC->MPDAT0;
    *u32data1 = FMC->MPDAT1;
    return 0;
}


/**
  * @brief    Read company ID.
  * @retval   The company ID.
  */
uint32_t FMC_ReadCID(void)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_CID;
    FMC->ISPADDR = 0x0;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    return FMC->ISPDAT;
}


/**
  * @brief    Read product ID.
  * @retval   The product ID.
  */
uint32_t FMC_ReadPID(void)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_PID;
    FMC->ISPADDR = 0x04;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    return FMC->ISPDAT;
}


/**
  * @brief    This function reads one of the four UCID.
  * @param[in]   u32Index  Index of the UCID to read. u32Index must be 0, 1, 2, or 3.
  * @retval   The UCID.
  */
uint32_t FMC_ReadUCID(uint32_t u32Index)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_UID;
    FMC->ISPADDR = (0x04 * u32Index) + 0x10;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    return FMC->ISPDAT;
}


/**
  * @brief    This function reads one of the three UID.
  * @param[in]  u32Index Index of the UID to read. u32Index must be 0, 1, or 2.
  * @retval   The UID.
  */
uint32_t FMC_ReadUID(uint32_t u32Index)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_UID;
    FMC->ISPADDR = 0x04 * u32Index;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    return FMC->ISPDAT;
}


/**
  * @brief    Get the base address of Data Flash if enabled.
  * @retval   The base address of Data Flash
  */
uint32_t FMC_ReadDataFlashBaseAddr(void)
{
    return FMC->DFBA;
}


/**
  * @brief    This function will force re-map assigned flash page to CPU address 0x0.
  * @param[in]  u32PageAddr Address of the page to be mapped to CPU address 0x0.
  * @return  None
  */
void FMC_SetVectorPageAddr(uint32_t u32PageAddr)
{
    FMC->ISPCMD = FMC_ISPCMD_VECMAP;
    FMC->ISPADDR = u32PageAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;
}


/**
  * @brief Execute ISP FMC_ISPCMD_PROGRAM to program a word to flash.
  * @param[in]  u32Addr Address of the flash location to be programmed.
  *             It must be a word aligned address.
  * @param[out] u32Data The word data to be programmed.
  * @return None
  */
void FMC_Write(uint32_t u32Addr, uint32_t u32Data)
{
    FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADDR = u32Addr;
    FMC->ISPDAT = u32Data;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;
}

int FMC_Write_64(uint32_t u32addr, uint32_t u32data0, uint32_t u32data1)
{
    FMC->ISPCMD = FMC_ISPCMD_PROGRAM_64;
    FMC->ISPADDR    = u32addr;
    FMC->MPDAT0 = u32data0;
    FMC->MPDAT1 = u32data1;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;

    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }
    return 0;
}


/**
  * @brief Program a 64-bits data to the specified OTP.
  * @param[in] otp_num    The OTP number.
  * @param[in] low_word   Low word of the 64-bits data.
  * @param[in] high_word   Low word of the 64-bits data.
  * @retval   0   Success
  * @retval   -1  Program failed.
  * @retval   -2  Invalid OTP number.
  */
int FMC_Write_OTP(int otp_num, uint32_t low_word, uint32_t high_word)
{
    if ((otp_num < 0) || (otp_num > 255))
        return -2;

    FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADDR = FMC_OTP_BASE + otp_num * 8;
    FMC->ISPDAT = low_word;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;
    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }

    FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADDR = FMC_OTP_BASE + otp_num * 8 + 4;
    FMC->ISPDAT = high_word;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;
    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }

    return 0;
}

/**
  * @brief  Read the 64-bits data from the specified OTP.
  * @param[in] otp_num    The OTP number.
  * @param[in] low_word   Low word of the 64-bits data.
  * @param[in] high_word   Low word of the 64-bits data.
  * @retval   0   Success
  * @retval   -1  Read failed.
  * @retval   -2  Invalid OTP number.
  */
int FMC_Read_OTP(int otp_num, uint32_t *low_word, uint32_t *high_word)
{
    if ((otp_num < 0) || (otp_num > 255))
        return -2;

    FMC->ISPCMD = FMC_ISPCMD_READ_64;
    FMC->ISPADDR    = FMC_OTP_BASE + otp_num * 8 ;
    FMC->ISPDAT = 0x0;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;

    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }

    *low_word = FMC->MPDAT0;
    *high_word = FMC->MPDAT1;
    return 0;
}

/**
  * @brief  Lock the specified OTP.
  * @param[in] otp_num    The OTP number.
  * @retval   0   Success
  * @retval   -1  Failed to write OTP lock bits.
  * @retval   -2  Invalid OTP number.
  */
int FMC_Lock_OTP(int otp_num)
{
    if ((otp_num < 0) || (otp_num > 255))
        return -2;

    FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADDR = FMC_OTP_BASE + 0x800 + otp_num * 4;
    FMC->ISPDAT = 0;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }
    return 0;
}

/**
  * @brief  Check the OTP is locked or not.
  * @param[in] otp_num    The OTP number.
  * @retval   1   OTP is locked.
  * @retval   0   OTP is not locked.
  * @retval   -1  Failed to read OTP lock bits.
  * @retval   -2  Invalid OTP number.
  */
int FMC_Is_OTP_Locked(int otp_num)
{
    if ((otp_num < 0) || (otp_num > 255))
        return -2;

    FMC->ISPCMD = FMC_ISPCMD_READ;
    FMC->ISPADDR = FMC_OTP_BASE + 0x800 + otp_num * 4;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) ;

    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        return -1;
    }

    if (FMC->ISPDAT == 0xFFFFFFFF)
        return 0;
    else
        return 1;
}

/**
  * @brief Execute FMC_ISPCMD_READ command to read User Configuration.
  * @param[out]  u32Config A two-word array.
  *              u32Config[0] holds CONFIG0, while u32Config[1] holds CONFIG1.
  * @param[in] u32Count Available word count in u32Config.
  * @return Success or not.
  * @retval   0  Success.
  * @retval   -1  Invalid parameter.
  */
int32_t FMC_ReadConfig(uint32_t *u32Config, uint32_t u32Count)
{
    u32Config[0] = FMC_Read(FMC_CONFIG_BASE);
    if (u32Count < 2)
        return 0;
    u32Config[1] = FMC_Read(FMC_CONFIG_BASE+4);
    return 0;
}


/**
  * @brief Execute ISP commands to erase then write User Configuration.
  * @param[in] u32Config   A two-word array.
  *            u32Config[0] holds CONFIG0, while u32Config[1] holds CONFIG1.
  * @param[in] u32Count  Always be 2 in this BSP.
  * @return Success or not.
  * @retval   0  Success.
  * @retval   -1  Invalid parameter.
  */
int32_t FMC_WriteConfig(uint32_t *u32Config, uint32_t u32Count)
{
    FMC_ENABLE_CFG_UPDATE();
    FMC_Erase(FMC_CONFIG_BASE);
    FMC_Write(FMC_CONFIG_BASE, u32Config[0]);
    FMC_Write(FMC_CONFIG_BASE+4, u32Config[1]);
    FMC_DISABLE_CFG_UPDATE();
    return 0;
}


/**
  * @brief Run CRC32 checksum calculation and get result.
  * @param[in] u32addr   Starting flash address. It must be a page aligned address.
  * @param[in] u32count  Byte count of flash to be calculated. It must be multiple of 512 bytes.
  * @return Success or not.
  * @retval   0           Success.
  * @retval   0xFFFFFFFF  Invalid parameter.
  */
uint32_t  FMC_GetChkSum(uint32_t u32addr, uint32_t u32count)
{
    if ((u32addr % 512) || (u32count % 512))
        return 0xFFFFFFFF;

    FMC->ISPCMD  = FMC_ISPCMD_RUN_CKS;
    FMC->ISPADDR = u32addr;
    FMC->ISPDAT  = u32count;
    FMC->ISPTRG  = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;

    FMC->ISPCMD = FMC_ISPCMD_READ_CKS;
    FMC->ISPADDR    = u32addr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;

    return FMC->ISPDAT;
}


/**
  * @brief Run flash all one verification and get result.
  * @param[in] u32addr   Starting flash address. It must be a page aligned address.
  * @param[in] u32count  Byte count of flash to be calculated. It must be multiple of 512 bytes.
  * @retval   READ_ALLONE_YES      The contents of verified flash area are 0xFFFFFFFF.
  * @retval   READ_ALLONE_NOT  Some contents of verified flash area are not 0xFFFFFFFF.
  * @retval   READ_ALLONE_CMD_FAIL  Unexpected error occurred.
  */
uint32_t  FMC_CheckAllOne(uint32_t u32addr, uint32_t u32count)
{
    FMC->ISPSTS = 0x80;   // clear check all one bit

    FMC->ISPCMD = FMC_ISPCMD_RUN_ALL1;
    FMC->ISPADDR    = u32addr;
    FMC->ISPDAT = u32count;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;

    do {
        FMC->ISPCMD = FMC_ISPCMD_READ_ALL1;
        FMC->ISPADDR    = u32addr;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) ;
    }  while (FMC->ISPDAT == 0);

    if ((FMC->ISPDAT == READ_ALLONE_YES) || (FMC->ISPDAT == READ_ALLONE_NOT))
        return FMC->ISPDAT;

    return READ_ALLONE_CMD_FAIL;
}


/**
  * @brief    Setup security key.
  * @param[in] key      Key 0~2 to be setup.
  * @param[in] kpmax    Maximum unmatched power-on counting number.
  * @param[in] kemax    Maximum unmatched counting number.
  * @param[in] lock_CONFIG   1: Security key lock CONFIG to write-protect. 0: Don't lock CONFIG.
  * @param[in] lock_SPROM    1: Security key lock SPROM to write-protect. 0: Don't lock SPROM.
  * @retval   0     Success.
  * @retval   -1    Key is locked. Cannot overwrite the current key.
  * @retval   -2    Failed to erase flash.
  * @retval   -3    Failed to program key.
  * @retval   -4    Key lock function failed.
  * @retval   -5    CONFIG lock function failed.
  * @retval   -6    SPROM lock function failed.
  * @retval   -7    KPMAX function failed.
  * @retval   -8    KEMAX function failed.
  */
int32_t  FMC_SKey_Setup(uint32_t key[3], uint32_t kpmax, uint32_t kemax, int lock_CONFIG, int lock_SPROM)
{
    uint32_t  lock_ctrl = 0;

    if (FMC->KPKEYSTS != 0x200)
        return -1;

    if (FMC_Erase(FMC_KPROM_BASE))
        return -2;

    if (FMC_Erase(FMC_KPROM_BASE+0x200))
        return -2;

    if (!lock_CONFIG)
        lock_ctrl |= 0x1;

    if (!lock_SPROM)
        lock_ctrl |= 0x2;

    FMC_Write(FMC_KPROM_BASE, key[0]);
    FMC_Write(FMC_KPROM_BASE+0x4, key[1]);
    FMC_Write(FMC_KPROM_BASE+0x8, key[2]);
    FMC_Write(FMC_KPROM_BASE+0xC, kpmax);
    FMC_Write(FMC_KPROM_BASE+0x10, kemax);
    FMC_Write(FMC_KPROM_BASE+0x14, lock_ctrl);

    while (FMC->KPKEYSTS & FMC_KPKEYSTS_KEYBUSY_Msk);

    if (!(FMC->KPKEYSTS & FMC_KPKEYSTS_KEYLOCK_Msk)) {
        printf("Security key lock failed!\n");
        return -4;
    }

    if ((lock_CONFIG && !(FMC->KPKEYSTS & FMC_KPKEYSTS_CFGFLAG_Msk)) ||
            (!lock_CONFIG && (FMC->KPKEYSTS & FMC_KPKEYSTS_CFGFLAG_Msk))) {
        printf("CONFIG lock failed!\n");
        return -5;
    }

    if ((lock_SPROM && !(FMC->KPKEYSTS & FMC_KPKEYSTS_SPFLAG_Msk)) ||
            (!lock_SPROM && (FMC->KPKEYSTS & FMC_KPKEYSTS_SPFLAG_Msk))) {
        printf("CONFIG lock failed!\n");
        return -6;
    }

    if (((FMC->KPCNT & FMC_KPCNT_KPMAX_Msk) >> FMC_KPCNT_KPMAX_Pos) != kpmax) {
        printf("KPMAX failed!\n");
        return -7;
    }

    if (((FMC->KPKEYCNT & FMC_KPKEYCNT_KPKEMAX_Msk) >> FMC_KPKEYCNT_KPKEMAX_Pos) != kemax) {
        printf("KEMAX failed!\n");
        return -8;
    }
    return 0;
}


/**
  * @brief    Execute security key comparison.
  * @param[in] key  Key 0~2 to be compared.
  * @retval   0     Key matched.
  * @retval   -1    Forbidden. Times of key comparison mismatch reach the maximum count.
  * @retval   -2    Key mismatched.
  * @retval   -3    No security key lock. Key comparison is not required.
  */
int32_t  FMC_SKey_Compare(uint32_t key[3])
{
    if (FMC->KPKEYSTS & FMC_KPKEYSTS_FORBID_Msk) {
        printf("FMC_SKey_Compare - FORBID!\n");
        return -1;
    }

    if (!(FMC->KPKEYSTS & FMC_KPKEYSTS_KEYLOCK_Msk)) {
        printf("FMC_SKey_Compare - key is not locked!\n");
        return -3;
    }

    FMC->KPKEY0 = key[0];
    FMC->KPKEY1 = key[1];
    FMC->KPKEY2 = key[2];
    FMC->KPKEYTRG = FMC_KPKEYTRG_KPKEYGO_Msk | FMC_KPKEYTRG_TCEN_Msk;

    while (FMC->KPKEYSTS & FMC_KPKEYSTS_KEYBUSY_Msk);

    if (!(FMC->KPKEYSTS & FMC_KPKEYSTS_KEYMATCH_Msk)) {
        printf("Key mismatched!\n");
        return -2;
    }

    if (FMC->KPKEYSTS & FMC_KPKEYSTS_KEYLOCK_Msk) {
        printf("Key matched, but still be locked!\n");
        return -2;
    }

    printf("FMC_SKey_Compare - OK.\n");
    return 0;
}


/*@}*/ /* end of group M480_FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_FMC_Driver */

/*@}*/ /* end of group M480_Device_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/


