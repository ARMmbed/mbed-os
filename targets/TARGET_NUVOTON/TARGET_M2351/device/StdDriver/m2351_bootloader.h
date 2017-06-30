/**************************************************************************//**
 * @file     bootloader.h
 * @version  V1.00
 * @brief    Non-Secure Callable library header file of Boot Loader
 *
 * @note
 * Copyright (C) 2017 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#ifdef __cplusplus
extern "C"
{
#endif



/*---------------------------------------------------------*/
/*  Redirect BL API function name to meet API Naming Rule  */
/*---------------------------------------------------------*/
#define BL_GetVersion       BL_Get_Version
#define BL_EnableFMC        BL_FMC_Open
#define BL_DisableFMC       BL_FMC_Close
#define BL_GetISPStatus     BL_FMC_ISP_Status
#define BL_FlashRead        BL_FMC_Read
#define BL_FlashMultiRead   BL_FMC_MultiRead
#define BL_FlashWrite       BL_FMC_Write
#define BL_FlashMultiWrite  BL_FMC_MultiWrite
#define BL_FlashPageErase   BL_FMC_Erase
#define BL_FlashChecksum    BL_FMC_GetChkSum
#define BL_ReadCID          BL_FMC_ReadCID
#define BL_ReadDID          BL_FMC_ReadDID
#define BL_ReadPID          BL_FMC_ReadPID
#define BL_ReadUCID         BL_FMC_ReadUCID
#define BL_ReadUID          BL_FMC_ReadUID
#define BL_CheckFlashAllOne BL_FMC_CheckAllOne
#define BL_CheckOPTStatus   BL_FMC_Is_OTP_Locked
#define BL_EnableOTPLock    BL_FMC_Lock_OTP
#define BL_ReadOTP          BL_FMC_Read_OTP
#define BL_WriteOTP         BL_FMC_Write_OTP
#define BL_GetKPROMStatus   BL_FMC_Read_KPROM_STATUS
#define BL_TrgKPROMCompare  BL_FMC_SKey_Compare
#define BL_GetNSBoundary    BL_FMC_NSCBOUD
#define BL_SetFlashAllLock  BL_FMC_Lock_All_Region
#define BL_SetXOMActive     BL_FMC_Set_XOM_Active
#define BL_GetXOMStatus     BL_FMC_Is_XOM_Actived
#define BL_SetXOMRegion     BL_FMC_Set_XOM_Region
#define BL_ReadXOMRegion    BL_FMC_Read_XOM_Region
#define BL_EraseXOMRegion   BL_FMC_Erase_XOM
#define BL_EraseXOMStatus   BL_FMC_XOM_Erase_Status
#define BL_ResetChip        BL_Chip_Reset



/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup BL_Driver Boot Loader Driver
  @{
*/


/** @addtogroup BL_EXPORTED_FUNCTIONS Boot Loader Exported Functions
  @{
*/



/**
  * @brief      Get Bootloader Version Number
  * @param      None
  * @return     Version number of bootloader
  * @details    Return bootloader version number and it's fixed to be 0x16091414.
  */
uint32_t BL_GetVersion(void);


/**
  * @brief      Enable FMC ISP Function
  * @param      None
  * @return     Maximum APROM size
  * @details    This API will unlock register write-protect, enable relavive settings for access FMC ISP commands
  *             and return maximum APROM by chip package.
  */
uint32_t BL_EnableFMC(void);


/**
  * @brief      Disable FMC ISP Function
  * @param      None
  * @return     None
  * @details    This API will disable relavive settings for disable FMC ISP function and lock register write-protect
  *             until last ISP operation is finished.
  */
void     BL_DisableFMC(void);


/**
  * @brief      Get FMC ISP Busy Status
  * @param      None
  * @retval     0   ISP operation is finished
  * @retval     1   ISP operation is in processing
  * @details    This API indicates ISP operation in in processing or finished.
  */
uint32_t BL_GetISPStatus(void);


/**
  * @brief      Get 32-bit data from Non-Secure Flash Address
  * @param[in]  u32Addr     Flash address should be in non-secure area
  * @retval     0xF0F00001  u32Addr isn't word aligned
  * @retval     0xF0F00002  u32Addr isn't in non-secure area
  * @retval     The data of specified non-secure address
  * @details    To read word data from specified non-secure flash address.
  */
uint32_t BL_FlashRead(uint32_t u32Addr);


/**
  * @brief      Read Multi-Word Data from Non-Secure Flash
  * @param[in]  u32Addr         Starting flash address should be in non-secure area
  * @param[out] pu32NSRamBuf    Starting address to store reading data
  * @param[in]  u32Size         Total read byte counts, it should be word aligned and maximum size is one page size.
  * @retval     0xF0F00001      u32Addr or u32Size is invalid parameter
  * @retval     0xF0F00002      u32Addr isn't in non-secure area
  * @retval     0               Read operation is finished
  * @details    To read multi-words data start from specified non-secure flash address.
  *             And maximum read size is one page size, 20248 bytes.
  */
int32_t  BL_FlashMultiRead(uint32_t u32Addr, uint32_t *pu32NSRamBuf, uint32_t u32Size);


/**
  * @brief      Program 32-bit data into Non-Secure Flash Address
  * @param[in]  u32Addr     Flash address should be in non-secure area
  * @param[in]  u32Data     32-bit Data to program
  * @retval     0xF0F00001  u32Addr isn't word aligned
  * @retval     0xF0F00002  u32Addr isn't in non-secure area
  * @retval     0           Program command is finished
  * @details    To program word data into specified non-secure flash address.
  */
int32_t  BL_FlashWrite(uint32_t u32Addr, uint32_t u32Data);


/**
  * @brief      Program Multi-Word Data into Non-Secure Flash
  * @param[in]  u32Addr         Starting flash address should be in non-secure area
  * @param[in]  pu32NSRamBuf    Starting address to store data to be program
  * @param[in]  u32Size         Total program byte counts, it should be word aligned and maximum size is one page size.
  * @retval     0xF0F00001      u32Addr or u32Size is invalid parameter
  * @retval     0xF0F00002      u32Addr isn't in non-secure area
  * @retval     0               Program operation is finished
  * @details    To program multi-words data start from specified non-secure flash address.
  *             And maximum program size is one page size, 20248 bytes.
  */
int32_t  BL_FlashMultiWrite(uint32_t u32Addr, uint32_t *pu32NSRamBuf, uint32_t u32Size);


/**
  * @brief      Flash Page Erase
  * @param[in]  u32PageAddr Address of the non-secure flash page to be erased. It must be a page size aligned address.
  * @retval     0xF0F00001  u32PageAddr isn't page size aligned
  * @retval     0xF0F00002  u32PageAddr isn't in non-secure area
  * @retval     0           Page erase success
  * @retval     -1          Page erase failed
  * @details    This API is used to perform page erase command on specified non-secure flash address.
  *             And this address must be a 2048 bytes page size aligned address.
  */
int32_t  BL_FlashPageErase(uint32_t u32PageAddr);


/**
  * @brief      Get specified Non-Secure Area CRC32 Checksum
  * @param[in]  u32Addr         Starting non-secure flash address. It must be a page size aligned address.
  * @param[in]  u32ByteCount    Byte count of non-secure flash area to be calculated. It must be multiple of 2048 bytes.
  * @retval     0xFFFFFFFF      u32Addr isn't in non-secure area or u32ByteCount isn't page size aligned
  * @retval     Result of CRC32 checksum
  * @details    This API will calculate the CRC32 checksum result of specified non-secure flash area.
  *             And the starting address and calculated szie must be all 2048 bytes page size aligned.
  */
uint32_t BL_FlashChecksum(uint32_t u32Addr, uint32_t u32ByteCount);


/**
  * @brief      Read Company ID
  * @param      None
  * @return     The company ID (32-bit)
  * @details    The company ID of Nuvoton is fixed to be 0xDA
  */
uint32_t BL_ReadCID(void);


/**
  * @brief      Read Device ID
  * @param      None
  * @return     The device ID (32-bit)
  * @details    This function is used to read device ID.
  */
uint32_t BL_ReadDID(void);


/**
  * @brief      Read Product ID
  * @param      None
  * @return     The product ID (32-bit)
  * @details    This function is used to read product ID.
  */
uint32_t BL_ReadPID(void);


/**
  * @brief      Read UCID
  * @param[in]  u32Index    Index of the UCID to read and u32Index must be 0, 1, 2, or 3.
  * @return     The UCID of specified index
  * @details    This function is used to read unique chip ID (UCID).
  */
uint32_t BL_ReadUCID(uint32_t u32Index);


/**
  * @brief      Read UID
  * @param[in]  u32Index    UID index. 0 = UID[31:0], 1 = UID[63:32], 2 = UID[95:64]
  * @return     The 32-bit unique ID data of specified UID index
  * @details    To read out specified 32-bit unique ID.
  */
uint32_t BL_ReadUID(uint32_t u32Index);


/**
  * @brief      Run Flash All One Verification and Get Result
  * @param[in]  u32Addr         Starting non-secure flash address. It must be a page size aligned address.
  * @param[in]  u32ByteCount    Byte count of non-secure flash area to be calculated. It must be multiple of 2048 bytes.
  * @retval     0xA11FFFFF      The contents of verified non-secure flash area are 0xFFFFFFFF
  * @retval     0xA1100000      Some contents of verified non-secure flash area are not 0xFFFFFFFF
  * @retval     0xF0F00001      u32Addr isn't page size aligned
  * @retval     0xF0F00002      u32Addr or u32ByteCount are not page size aligned
  * @retval     0xFFFFFFFF      Unexpected error occurred
  * @details    This API is used to check specified non-secure flash area are all 0xFFFFFFFF or not.
  */
uint32_t BL_CheckFlashAllOne(uint32_t u32Addr, uint32_t u32ByteCount);


/**
  * @brief      Check the specified OTP is Locked or Unlock
  * @param[in]  u32OTP      The specified OTP number, it must be between 0 ~ 255.
  * @retval     1           Specified OTP is locked
  * @retval     0           Specified OTP is unlocked
  * @retval     -1          Failed to read OTP lock status
  * @retval     -2          Invalid OTP number
  * @details    Use can use this API to check specified OPT is lock or unlock.
  *             If unlocked, user can program specified bit data from 1 to 0 in this OPT area.
  */
int32_t  BL_CheckOPTStatus(uint32_t u32OTP);


/**
  * @brief      Lock the specified OTP
  * @param[in]  u32OTP      The specified OTP number, it must be between 0 ~ 255.
  * @retval     0           Success
  * @retval     -1          Failed to execute write OTP command
  * @retval     -2          Invalid OTP number
  * @details    Use can use this API to lock specified OPT area.
  *             If locked, user can't modify data in this OPT area.
  */
int32_t  BL_EnableOTPLock(uint32_t u32OTP);


/**
  * @brief      Read the 64-bits OTP Data
  * @param[in]  u32OTP      The specified OTP number, it must be between 0 ~ 255.
  * @param[in]  low_word    Low word of the 64-bits data
  * @param[in]  high_word   High word of the 64-bits data
  * @retval     0           Success
  * @retval     -1          Failed to read OTP data
  * @retval     -2          Invalid OTP number
  * @details    Thia API can get the specified 64-bits OTP data.
  */
int32_t  BL_ReadOTP(uint32_t u32OTP, uint32_t *low_word, uint32_t *high_word);


/**
  * @brief      Program a 64-bits OTP Data
  * @param[in]  u32OTP      The specified OTP number, it must be between 0 ~ 255.
  * @param[in]  low_word    Low word of the 64-bits data
  * @param[in]  high_word   High word of the 64-bits data
  * @retval     0           Success
  * @retval     -1          Failed to write OTP data
  * @retval     -2          Invalid OTP number
  * @details    Thia API can program 64-bits data to specified OTP area if current OTP is unlocked.
  */
int32_t  BL_WriteOTP(uint32_t u32OTP, uint32_t low_word, uint32_t high_word);


/**
  * @brief      Read KPROM KPKEYSTS Register Status
  * @param      None
  * @return     KPKEYSTS register status
  * @details    This API can read KPROM KPKEYSTS register status.
  */
uint32_t BL_GetKPROMStatus(void);


/**
  * @brief      Execute Security Key Comparison
  * @param[in]  key[3]      Input key[0]~key[2] to be compared
  * @retval     0           Key matched
  * @retval     -1          Forbidden. Times of key comparison mismatch reach the maximum count.
  * @retval     -2          Key mismatched.
  * @retval     -3          No security key lock. Key comparison is not required.
  * @details    User can use this API to unlock KPROM protection then execute FMC program command well.
  */
int32_t  BL_TrgKPROMCompare(uint32_t key[3]);


/**
  * @brief      Get Non-Scure Boundary
  * @param      None
  * @return     Current non-secure boundary
  * @details    This API can get current non-secure boundary address.
  */
uint32_t BL_GetNSBoundary(void);


/**
  * @brief      Set All Flash Region Lock
  * @param      None
  * @return     Always return 0
  * @details    This API will protect all flash region read/write operate by ICE/TWICP/WRITER interface exclude OPT area.
  */
int32_t  BL_SetFlashAllLock(void);


/**
  * @brief      Set XOM Active
  * @param[in]  u32XOM      Specified XOM region, it must be between 0~3.
  * @retval     0           Set XOM region active success
  * @retval     1           Current XOM region is avtive
  * @retval     -1          Current XOM has not been configured yet
  * @retval     -2          Invalid u32XOM number
  * @retval     -3          XOM region isn't in non-secure area
  * @details    This API will set specified XOM region become active.
  */
int32_t  BL_SetXOMActive(uint32_t u32XOM);


/**
  * @brief      Get XOM Active Status
  * @param[in]  u32XOM      Specified XOM region, it must be between 0~3.
  * @retval     0           Current XOM region isn't avtive yet
  * @retval     1           Current XOM region is avtive
  * @retval     -2          Invalid u32XOM number
  * @details    This API will return specified XOM region is active or not.
  */
int32_t  BL_GetXOMStatus(uint32_t u32XOM);


/**
  * @brief      Set XOM Setting
  * @param[in]  u32XOM          Specified XOM region, it must be between 0~3
  * @param[in]  u32Base         Base address of XOM region
  * @param[in]  u32PageCnt      Page count of XOM region
  * @param[in]  u32SetActive    0: only configure XOM base and page count; 1: set XOM region avtive after XOM setting done.
  * @retval     0               Set specified XOM success
  * @retval     1               Current specified XOM region is active
  * @retval     -1              Current specified XOM region has been configured
  * @retval     -2              Invalid u32XOM number
  * @retval     -3              XOM region isn't in non-secure area
  * @details    This API will set specified XOM settings.
  */
int32_t  BL_SetXOMRegion(uint32_t u32XOM, uint32_t u32Base, uint32_t u32PageCnt, uint32_t u32SetActive);


/**
  * @brief      Read XOM Setting
  * @param[in]  u32XOM      Specified XOM region, it must be between 0~3
  * @param[out] pu32Base    Return specified XOM base address
  * @param[out] pu32PageCnt Return specified XOM page count
  * @retval     0           Read specified XOM setting success
  * @retval     -1          Get specified XOM status failed
  * @retval     -2          Invalid u32XOM number
  * @retval     -3          XOM region isn't in non-secure area
  * @details    This API will read specified XOM relative settings.
  */
int32_t  BL_ReadXOMRegion(uint32_t u32XOM, uint32_t *pu32Base, uint32_t *pu32PageCnt);


/**
  * @brief      Erase XOM Setting
  * @param[in]  u32XOMBase  Specified valid XOM base address to be erase
  * @retval     0           XOM page erase command finished
  * @retval     -1          XOM page erase command failed
  * @retval     -2          invalid u32XOMBase base address
  * @retval     -3          u32XOMBase base mismatched
  * @details    This API will erase specified XOM region data and relavive XOM setting.
  */
int32_t  BL_EraseXOMRegion(uint32_t u32XOMBase);


/**
  * @brief      Get XOM Erased Status
  * @param      None
  * @retval     0       XOM erase operation success
  * @retval     -1      XOM erase operation failed
  * @details    This API will return the XOM erase operation is success or not.
  */
int32_t  BL_EraseXOMStatus(void);


/**
  * @brief      Execute CHIP Reset
  * @param      None
  * @return     None
  * @details    This API will perform reset CHIP command to reset chip.
  */
void     BL_ResetChip(void);


/*@}*/ /* end of group BL_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group BL_Driver */

/*@}*/ /* end of group Standard_Driver */


#ifdef __cplusplus
}
#endif

#endif  /* __BOOTLOADER_H__ */

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
