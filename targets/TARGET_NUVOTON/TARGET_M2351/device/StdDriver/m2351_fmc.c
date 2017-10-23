/**************************************************************************//**
 * @file     fmc.c
 * @version  V3.00
 * $Revision: 1 $
 * $Date: 16/07/07 7:50p $
 * @brief    M2351 Series Flash Memory Controller(FMC) driver source file
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include <stdio.h>
#include "M2351.h"

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup FMC_Driver FMC Driver
  @{
*/

/** @addtogroup FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/

/**
  * @brief      Set boot source from LDROM or APROM after next software reset
  *
  * @param[in]  i32BootSrc
  *                1: Boot from LDROM
  *                0: Boot from APROM
  *
  * @return    None
  *
  * @details   This function is used to switch APROM boot or LDROM boot. User need to call
  *            FMC_SetBootSource to select boot source first, then use CPU reset or
  *            System Reset Request to reset system.
  *
  */
void FMC_SetBootSource(int32_t i32BootSrc)
{
    if(i32BootSrc)
        FMC->ISPCTL |= FMC_ISPCTL_BS_Msk; /* Boot from LDROM */
    else
        FMC->ISPCTL &= ~FMC_ISPCTL_BS_Msk;/* Boot from APROM */
}


/**
  * @brief    Disable ISP Functions
  *
  * @param    None
  *
  * @return   None
  *
  * @details  This function will clear ISPEN bit of ISPCON to disable ISP function
  *
  */
void FMC_Close(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk;
}


/**
  * @brief    Disable APROM update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Disable APROM update function will forbid APROM programming when boot form APROM.
  *           APROM update is default to be disable.
  *
  */
void FMC_DisableAPUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_APUEN_Msk;
}


/**
  * @brief    Disable User Configuration update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Disable User Configuration update function will forbid User Configuration programming.
  *           User Configuration update is default to be disable.
  */
void FMC_DisableConfigUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_CFGUEN_Msk;
}


/**
  * @brief    Disable LDROM update function
  *
  * @param    None
  *
  * @return   None

  * @details  Disable LDROM update function will forbid LDROM programming.
  *           LDROM update is default to be disable.
  */
void FMC_DisableLDUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_LDUEN_Msk;
}


/**
  * @brief    Enable APROM update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Enable APROM to be able to program when boot from APROM.
  *
  */
void FMC_EnableAPUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_APUEN_Msk;
}


/**
  * @brief    Enable User Configuration update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Enable User Configuration to be able to program.
  *
  */
void FMC_EnableConfigUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_CFGUEN_Msk;
}


/**
  * @brief    Enable LDROM update function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  Enable LDROM to be able to program.
  *
  */
void FMC_EnableLDUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_LDUEN_Msk;
}


/**
  * @brief    Get the current boot source
  *
  * @param    None
  *
  * @retval   0 This chip is currently booting from APROM
  * @retval   1 This chip is currently booting from LDROM
  *
  * @note     This function only show the boot source.
  *           User need to read ISPSTA register to know if IAP mode supported or not in relative boot.
  */
int32_t FMC_GetBootSource(void)
{
    if(FMC->ISPCTL & FMC_ISPCTL_BS_Msk)
        return 1;
    else
        return 0;
}


/**
  * @brief    Enable FMC ISP function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  ISPEN bit of ISPCON must be set before we can use ISP commands.
  *           Therefore, To use all FMC function APIs, user needs to call FMC_Open() first to enable ISP functions.
  *
  * @note     ISP functions are write-protected. user also needs to unlock it by calling SYS_UnlockReg() before using all ISP functions.
  *
  */
void FMC_Open(void)
{
    FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk;
}

/**
  * @brief       Read the User Configuration words.
  *
  * @param[out]  u32Config  The word buffer to store the User Configuration data.
  * @param[in]   u32Count   The word count to be read.
  *
  * @retval       0 Success
  * @retval      -1 Failed
  *
  * @details     This function is used to read the settings of user configuration.
  *              if u32Count = 1, Only CONFIG0 will be returned to the buffer specified by u32Config.
  *              if u32Count = 2, Both CONFIG0 and CONFIG1 will be returned.
  */
int32_t FMC_ReadConfig(uint32_t *u32Config, uint32_t u32Count)
{
    int32_t i;

    for(i = 0; i < u32Count; i++)
        u32Config[i] = FMC_Read(FMC_CONFIG_BASE + i * 4);

    return 0;
}


/**
  * @brief    Write User Configuration
  *
  * @param[in]  u32Config The word buffer to store the User Configuration data.
  * @param[in]  u32Count The word count to program to User Configuration.
  *
  * @retval     0 Success
  * @retval    -1 Failed
  *
  * @details  User must enable User Configuration update before writing it.
  *           User must erase User Configuration before writing it.
  *           User Configuration is also be page erase. User needs to backup necessary data
  *           before erase User Configuration.
  */
int32_t FMC_WriteConfig(uint32_t *u32Config, uint32_t u32Count)
{
    int32_t i;

    for(i = 0; i < u32Count; i++)
    {
        FMC_Write(FMC_CONFIG_BASE + i * 4, u32Config[i]);
        if(FMC_Read(FMC_CONFIG_BASE + i * 4) != u32Config[i])
            return -1;
    }

    return 0;
}

/**
  * @brief      Execute Flash Block erase
  *
  * @param[in]  u32BlockAddr  Address of the flash block to be erased.
  *             It must be a 4 pages aligned address.
  *
  * @return ISP page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  *
  * @details Execute FMC_ISPCMD_BLOCK_ERASE command to erase a flash block. The block size is 4 pages.
  */
int32_t FMC_Erase_Block(uint32_t u32BlockAddr)
{
    int32_t  ret = 0;

    FMC->ISPCMD = FMC_ISPCMD_BLOCK_ERASE;
    FMC->ISPADDR = u32BlockAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        ret = -1;
    }
    return ret;
}

/**
  * @brief      Execute Flash Bank erase
  *
  * @param[in]  u32BankAddr Base address of the flash bank to be erased.
  *
  * @return ISP page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  *
  * @details  Execute FMC_ISPCMD_BANK_ERASE command to erase a flash block.
  */
int32_t FMC_Erase_Bank(uint32_t u32BankAddr)
{
    int32_t  ret = 0;

    FMC->ISPCMD = FMC_ISPCMD_BANK_ERASE;
    FMC->ISPADDR = u32BankAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        ret = -1;
    }
    return ret;
}

/**
  * @brief     Write data to OTP
  * @param[in] otp_num    The OTP number.
  * @param[in] low_word   Low word of the 64-bits data.
  * @param[in] high_word   Low word of the 64-bits data.
  *
  * @retval   0   Success
  * @retval   -1  Program failed.
  * @retval   -2  Invalid OTP number.
  *
  * @details  Program a 64-bits data to the specified OTP. 
  */
int32_t FMC_Write_OTP(uint32_t otp_num, uint32_t low_word, uint32_t high_word)
{
    int32_t  ret = 0;

    if (otp_num > 255UL) {
        ret = -2;
    }

    if (ret == 0) {
        FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
        FMC->ISPADDR = FMC_OTP_BASE + otp_num * 8UL;
        FMC->ISPDAT = low_word;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        }
    }

    if (ret == 0) {
        FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
        FMC->ISPADDR = FMC_OTP_BASE + otp_num * 8UL + 4UL;
        FMC->ISPDAT = high_word;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        }
    }

    return ret;
}

/**
  * @brief  Read data from OTP
  *
  * @param[in] otp_num    The OTP number.
  * @param[in] low_word   Low word of the 64-bits data.
  * @param[in] high_word   Low word of the 64-bits data.
  *
  * @retval   0   Success
  * @retval   -1  Read failed.
  * @retval   -2  Invalid OTP number.
  *
  * @details  Read the 64-bits data from the specified OTP.
  */
int32_t FMC_Read_OTP(uint32_t otp_num, uint32_t *low_word, uint32_t *high_word)
{
    int32_t  ret = 0;

    if (otp_num > 255UL) {
        ret = -2;
    }

    if (ret == 0) {
        FMC->ISPCMD = FMC_ISPCMD_READE_8;
        FMC->ISPADDR    = FMC_OTP_BASE + otp_num * 8UL ;
        FMC->ISPDAT = 0x0UL;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        } else {
            *low_word = FMC->MPDAT0;
            *high_word = FMC->MPDAT1;
        }
    }
    return ret;
}

/**
  * @brief  Lock the specified OTP.
  *
  * @param[in] otp_num    The OTP number.
  *
  * @retval   0   Success
  * @retval   -1  Failed to write OTP lock bits.
  * @retval   -2  Invalid OTP number.
  *
  * @details  To lock specified OTP number 
  */
int32_t FMC_Lock_OTP(uint32_t otp_num)
{
    int32_t  ret = 0;

    if (otp_num > 255UL) {
        ret = -2;
    }

    if (ret == 0) {
        FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
        FMC->ISPADDR = FMC_OTP_BASE + 0x800UL + otp_num * 4UL;
        FMC->ISPDAT = 0UL;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        }
    }
    return ret;
}

/**
  * @brief  Check the OTP is locked or not.
  *
  * @param[in] otp_num    The OTP number.
  *
  * @retval   1   OTP is locked.
  * @retval   0   OTP is not locked.
  * @retval   -1  Failed to read OTP lock bits.
  * @retval   -2  Invalid OTP number.
  *
  * @details To get specify OPT lock status
  */
int32_t FMC_Is_OTP_Locked(uint32_t otp_num)
{
    int32_t  ret = 0;

    if (otp_num > 255UL) {
        ret = -2;
    }

    if (ret == 0) {
        FMC->ISPCMD = FMC_ISPCMD_READ;
        FMC->ISPADDR = FMC_OTP_BASE + 0x800UL + otp_num * 4UL;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        } else {
            if (FMC->ISPDAT != 0xFFFFFFFFUL) {
                ret = 1;   /* Lock work was progrmmed. OTP was locked. */
            }
        }
    }
    return ret;
}

/**
  * @brief Run CRC32 checksum calculation and get result.
  *
  * @param[in] u32addr   Starting flash address. It must be a page aligned address.
  * @param[in] u32count  Byte count of flash to be calculated. It must be multiple of 512 bytes.
  *
  * @return Success or not.
  * @retval   0           Success.
  * @retval   0xFFFFFFFF  Invalid parameter.
  *
  * details   Run ISP checksum command to calculate specify area
  */
uint32_t  FMC_GetChkSum(uint32_t u32addr, uint32_t u32count)
{
    uint32_t   ret;

    if ((u32addr % 512UL) || (u32count % 512UL)) {
        ret = 0xFFFFFFFF;
    } else {
        FMC->ISPCMD  = FMC_ISPCMD_CAL_CHECKSUM;
        FMC->ISPADDR = u32addr;
        FMC->ISPDAT  = u32count;
        FMC->ISPTRG  = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

        FMC->ISPCMD = FMC_ISPCMD_CHECKSUM;
        FMC->ISPADDR    = u32addr;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

        ret = FMC->ISPDAT;
    }

    return ret;
}


/**
  * @brief Run flash all one verification and get result.
  *
  * @param[in] u32addr   Starting flash address. It must be a page aligned address.
  * @param[in] u32count  Byte count of flash to be calculated. It must be multiple of 512 bytes.
  *
  * @retval   READ_ALLONE_YES      The contents of verified flash area are 0xFFFFFFFF.
  * @retval   READ_ALLONE_NOT  Some contents of verified flash area are not 0xFFFFFFFF.
  * @retval   READ_ALLONE_CMD_FAIL  Unexpected error occurred.
  *
  * @details  Run ISP check all one command to check specify area is all one or not.
  */
uint32_t  FMC_CheckAllOne(uint32_t u32addr, uint32_t u32count)
{
    uint32_t  ret = READ_ALLONE_CMD_FAIL;

    FMC->ISPSTS = 0x80UL;   /* clear check all one bit */

    FMC->ISPCMD   = FMC_ISPCMD_RUN_ALL1;
    FMC->ISPADDR  = u32addr;
    FMC->ISPDAT   = u32count;
    FMC->ISPTRG   = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

    do {
        FMC->ISPCMD = FMC_ISPCMD_READ_ALL1;
        FMC->ISPADDR    = u32addr;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }
    }  while (FMC->ISPDAT == 0UL);

    if (FMC->ISPDAT == READ_ALLONE_YES) {
        ret = FMC->ISPDAT;
    }

    if (FMC->ISPDAT == READ_ALLONE_NOT) {
        ret = FMC->ISPDAT;
    }

    return ret;
}


/**
  * @brief    Setup security key.
  *
  * @param[in] key      Key 0~2 to be setup.
  * @param[in] kpmax    Maximum unmatched power-on counting number.
  * @param[in] kemax    Maximum unmatched counting number.
  * @param[in] lock_CONFIG   1: Security key lock CONFIG to write-protect. 0: Don't lock CONFIG.
  * @param[in] lock_SPROM    1: Security key lock SPROM to write-protect. 0: Don't lock SPROM. (This status is not supported on M2351)
  *
  * @retval   0     Success.
  * @retval   -1    Key is locked. Cannot overwrite the current key.
  * @retval   -2    Failed to erase flash.
  * @retval   -3    Failed to program key.
  * @retval   -4    Key lock function failed.
  * @retval   -5    CONFIG lock function failed.
  * @retval   -5    CONFIG lock function failed.
  * @retval   -6    SPROM lock function failed. (This status is not supported on M2351)
  * @retval   -7    KPMAX function failed.
  * @retval   -8    KEMAX function failed.
  *
  * @details  Set security keys and setup key compare count. The Security key also can protect user config.
  */
int32_t  FMC_SKey_Setup(uint32_t key[3], uint32_t kpmax, uint32_t kemax,
                        const int32_t lock_CONFIG, const int32_t lock_SPROM)
{
    uint32_t  lock_ctrl = 0UL;
    uint32_t  u32KeySts;
    int32_t   ret = 0;

    if (FMC->KPKEYSTS != 0x200UL) {
        ret = -1;
    }

    if (FMC_Erase(FMC_KPROM_BASE)) {
        ret = -2;
    }

    if (FMC_Erase(FMC_KPROM_BASE+0x200UL)) {
        ret = -3;
    }

    if (!lock_CONFIG) {
        lock_ctrl |= 0x1UL;
    }

    if (!lock_SPROM) {
        lock_ctrl |= 0x2UL;
    }

    if (ret == 0) {
        FMC_Write(FMC_KPROM_BASE, key[0]);
        FMC_Write(FMC_KPROM_BASE+0x4UL, key[1]);
        FMC_Write(FMC_KPROM_BASE+0x8UL, key[2]);
        FMC_Write(FMC_KPROM_BASE+0xCUL, kpmax);
        FMC_Write(FMC_KPROM_BASE+0x10UL, kemax);
        FMC_Write(FMC_KPROM_BASE+0x14UL, lock_ctrl);

        while (FMC->KPKEYSTS & FMC_KPKEYSTS_KEYBUSY_Msk) { }

        u32KeySts = FMC->KPKEYSTS;

        if (!(u32KeySts & FMC_KPKEYSTS_KEYLOCK_Msk)) {
            /* Security key lock failed! */
            ret = -4;
        } else if ((lock_CONFIG && (!(u32KeySts & FMC_KPKEYSTS_CFGFLAG_Msk))) ||
                   ((!lock_CONFIG) && (u32KeySts & FMC_KPKEYSTS_CFGFLAG_Msk))) {
            /* CONFIG lock failed! */
            ret = -5;
        } else if (((FMC->KPCNT & FMC_KPCNT_KPMAX_Msk) >> FMC_KPCNT_KPMAX_Pos) != kpmax) {
            /* KPMAX failed! */
            ret = -7;     
        } else if (((FMC->KPKEYCNT & FMC_KPKEYCNT_KPKEMAX_Msk) >> FMC_KPKEYCNT_KPKEMAX_Pos) != kemax) {
            /* KEMAX failed! */
            ret = -8;
        }
    }
    return ret;
}


/**
  * @brief    Execute security key comparison.
  *
  * @param[in] key  Key 0~2 to be compared.
  *
  * @retval   0     Key matched.
  * @retval   -1    Forbidden. Times of key comparison mismatch reach the maximum count.
  * @retval   -2    Key mismatched.
  * @retval   -3    No security key lock. Key comparison is not required.
  *
  @ details   Inpue a keys to compare with security keys
  */
int32_t  FMC_SKey_Compare(uint32_t key[3])
{
    uint32_t  u32KeySts;
    int32_t   ret = 0;

    if (FMC->KPKEYSTS & FMC_KPKEYSTS_FORBID_Msk) {
        /* FMC_SKey_Compare - FORBID!  */
        ret = -1;
    }

    if (!(FMC->KPKEYSTS & FMC_KPKEYSTS_KEYLOCK_Msk)) {
        /* FMC_SKey_Compare - key is not locked!  */
        ret = -3;
    }

    if (ret == 0) {
        FMC->KPKEY0 = key[0];
        FMC->KPKEY1 = key[1];
        FMC->KPKEY2 = key[2];
        FMC->KPKEYTRG = FMC_KPKEYTRG_KPKEYGO_Msk | FMC_KPKEYTRG_TCEN_Msk;
                        
        while (FMC->KPKEYSTS & FMC_KPKEYSTS_KEYBUSY_Msk) { }

        u32KeySts = FMC->KPKEYSTS;

        if (!(u32KeySts & FMC_KPKEYSTS_KEYMATCH_Msk)) {
            /* Key mismatched! */
            ret = -2;
        } else if (u32KeySts & FMC_KPKEYSTS_KEYLOCK_Msk) {
            /* Key matched, but still be locked! */
            ret = -2;
        }
    }
    return ret;
}


/**
  * @brief  Check the XOM is actived or not.
  *
  * @param[in] xom_num    The xom number(0~3).
  *
  * @retval   1   XOM is actived.
  * @retval   0   XOM is not actived.
  * @retval   -2  Invalid XOM number.
  *
  * @details To get specify XOMRn(n=0~3) active status
  */
int32_t FMC_Is_XOM_Actived(uint32_t xom_num)
{
    int32_t  ret = 0, STS = 0;

    if (xom_num >= 4UL)
		{
        return -2;
    }

		STS =  (((FMC->XOMSTS) & 0xf) & (1 << xom_num)) >> xom_num;
		
		return STS;
}



/**
  * @brief     Config XOM Region
  * @param[in] otp_num    The OTP number.
  * @param[in] low_word   Low word of the 64-bits data.
  * @param[in] high_word   Low word of the 64-bits data.
  *
  * @retval   0   Success
  * @retval   1   XOM is has already actived.
  * @retval   -1  Program failed.
  * @retval   -2  Invalid XOM number.
  *
  * @details  Program XOM base address and XOM size(page)
  */
int32_t FMC_Config_XOM(uint32_t xom_num, uint32_t xom_base, uint8_t xom_page)
{
    int32_t  ret = 0;

    if (xom_num >= 4UL)
		{
        ret = -2;
    }
		
		if(ret==0)
		{ 
		    ret = FMC_Is_XOM_Actived(xom_num);
    }	
		
		if(ret==0)
		{
        FMC_Write(FMC_XOM_BASE + xom_num*0x10, xom_base);
        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk){
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
				}
    }
				
		if(ret==0)	
		{			
		    FMC_Write(FMC_XOM_BASE + xom_num*0x10 + +0x04, xom_page);    //[2k~512k, 0~255]
        if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        }
	  }	
		
		if(ret==0)	
		{			
		    FMC_Write(FMC_XOM_BASE + xom_num*0x10 + 0x08, 0);    //Active
				if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk) {
            FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
            ret = -1;
        }
		}

    return ret;
}


/**
  * @brief  Execute Erase XOM Region
  *
  * @param[in]  u32Addr  XOMRn(n=0~3) region base address
  *
  * @return   XOM erase success or not.
  * @retval    0  Success
  * @retval   -1  Erase failed
  * @retval   -2  Invalid XOM number.
  *
  * @details Execute FMC_ISPCMD_PAGE_ERASE command to erase XOM. 
  */
int32_t FMC_Erase_XOM(uint32_t xom_num)
{
    uint32_t u32Addr, u32Active;
	
    if (xom_num >= 4UL)
		{
        return -2;
    }
    	
		u32Active = FMC_Is_XOM_Actived(xom_num);
		
		
		if(u32Active)
		{   
			  //u32Addr = inpw(((&(FMC->XOMR0STS) + xom_num*4) & 0xFFFFFF00) >> 8);
		    u32Addr = (inpw(0x4000C0D0 + xom_num*4) & 0xFFFFFF00) >> 8;			
        FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
        FMC->ISPADDR = u32Addr;
        FMC->ISPDAT = 0x55aa03;
        FMC->ISPTRG = 0x1;
#if ISBEN
        __ISB();
#endif
        while(FMC->ISPTRG);

         /* Check ISPFF flag to know whether erase OK or fail. */
        if(FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
        {
            FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
            return -1;
        }
	  }
		else 
		{
			  return -1;
		}
		
		return 0;
}

/*@}*/ /* end of group FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group FMC_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/


