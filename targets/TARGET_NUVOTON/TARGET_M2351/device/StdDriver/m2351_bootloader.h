/**************************************************************************//**
 * @file     bootloader.h
 * @version  V1.00
 * @brief    Secure Bootloader library header file
 *
 * @copyright (C) 2017 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup BL_Driver Bootloader Driver
  @{
*/

/** @addtogroup BL_EXPORTED_CONSTANTS Bootloader Exported Constants
  @{
*/
/*--------------------------------------------------------------------------------------------------*/
/*  Status and Erroe Code Constant Definitions                                                      */
/*--------------------------------------------------------------------------------------------------*/
#define BL_ERR_TT_CHECK         0xF0F00000UL    /*!< Not a non-secure parameter         */
#define BL_ERR_PARAMETER        0xF0F00001UL    /*!< Invalid parameter                  */
#define BL_PARAM_ALIGN          0xF0F00002UL    /*!< Parameter alignment error          */
#define BL_NOT_FLASH_ADDR       0xF0F00003UL    /*!< Invalid flash address              */
#define BL_NOT_SRAM_ADDR        0xF0F00004UL    /*!< Invalid sram address               */
#define BL_XOM_NOT_CONFIG       0xF0F00005UL    /*!< XOM is not configure yet           */
#define BL_XOM_HAS_CONFIG       0xF0F00006UL    /*!< XOM has beeen configured           */
#define BL_XOM_HAS_ACTIVE       0xF0F00007UL    /*!< XOM is actived                     */
#define BL_XOM_BASE_ERROR       0xF0F00008UL    /*!< Invalid XOM base address           */
#define BL_KPROM_NOT_ENABLE     0xF0F00009UL    /*!< KPROM is not enabled yet           */
#define BL_KPROM_KEY_FORBID     0xF0F0000AUL    /*!< KPROM comparison is forbidden      */
#define BL_KPROM_KEY_UNMATCH    0xF0F0000BUL    /*!< KPROM comparison is unmatched      */
#define BL_KPROM_KEY_LOCKED     0xF0F0000CUL    /*!< KPROM write-protect is enabled     */
#define BL_KPROM_SET_FAIL       0xF0F0000EUL    /*!< Set KPROM key fail                 */
#define BL_ISP_CMD_FAIL         (-1)            /*!< FMC command fail                   */
#define BL_FLASH_ALLONE         0xA11FFFFFUL    /*!< Check-all-one result is all one    */
#define BL_FLASH_NOT_ALLONE     0xA1100000UL    /*!< Check-all-one result is not all one*/

/*--------------------------------------------------------------------------------------------------*/
/*  Random Number Generator Constant Definitions                                                    */
/*--------------------------------------------------------------------------------------------------*/
#define BL_RNG_PRNG             (0UL)   /*!<Use H/W random number generator */
#define BL_RNG_SWRNG            (1UL)   /*!<Use S/W random number generator */
#define BL_RNG_LIRC32K          (0UL)   /*!<Use LIRC32 for random number generator */
#define BL_RNG_LXT              (2UL)   /*!<Use LXT for random number generator */

/*@}*/ /* end of group BL_EXPORTED_CONSTANTS */

/** @addtogroup BLC_EXPORTED_STRUCTS Bootloader Exported Structs
  @{
*/
/**
  * @details    Random Number Generator Struct
  */
typedef struct
{
    uint32_t opt;       /*!< Operation mode */
    int32_t data_len;   /*!< Internal use for random number generator */
    uint8_t buf[32];    /*!< Internal use for random number generator */
    uint8_t buf2[20];   /*!< Internal use for random number generator */
} BL_RNG_T;

/*@}*/ /* end of group BL_EXPORTED_STRUCTS */


/** @addtogroup BL_EXPORTED_FUNCTIONS Bootlader Exported Functions
  @{
*/
/**
  * @brief      Get Bootloader Version Number
  * @param      None
  * @return     Version number of bootloader
  * @details    Return bootloader version number.
  */
uint32_t BL_GetVersion(void);


/**
  * @brief      Enable FMC ISP Function and return maximum APROM size
  * @param      None
  * @return     Maximum APROM size
  * @details    This API will unlock register write-protect, enable relative settings for access FMC ISP commands
  *             and return maximum APROM by chip package.
  */
uint32_t BL_EnableFMC(void);


/**
  * @brief      Disable FMC ISP Function
  * @param      None
  * @return     None
  * @details    This API will disable relative settings for disable FMC ISP function and lock register write-protect
  *             until last ISP operation is finished.
  */
void BL_DisableFMC(void);


/**
  * @brief      Get FMC ISP Busy Status
  * @param      None
  * @retval     0   ISP operation is finished
  * @retval     1   ISP operation is in processing
  * @details    This API indicates ISP operation in in processing or finished.
  */
uint32_t BL_GetISPStatus(void);


/**
  * @brief      Get Non-Secure Boundary
  * @param      None
  * @return     Current Non-Secure boundary
  * @details    This API can get current Non-Secure boundary address.
  */
uint32_t BL_GetNSBoundary(void);


/**
  * @brief      Set All Flash Region Lock
  * @param      None
  * @retval     -1      Set flash all lock failed
  * @retval     0       Set flash all lock operation is success
  * @details    This API will protect all flash region read/write operate by ICE/TWICP/WRITER interface exclude OPT area.
  */
int32_t BL_SetFlashAllLock(void);


/**
  * @brief      Read Non-Secure Flash Address Data (for Non-Secure region)
  * @param[in]  u32NSAddr   Non-Secure flash address
  * @retval     0xF0F00000  u32NSAddr isn't in Non-Secure area
  * @retval     0xF0F00001  u32NSAddr isn't word aligned
  * @retval     0xF0F00003  u32NSAddr isn't valid flash address
  * @retval     -1          Flash read failed
  * @retval     The data of specified Non-Secure address
  * @details    To read word data from specified Non-Secure flash address.
  */
uint32_t BL_FlashRead(uint32_t u32NSAddr);


/**
  * @brief      Read Multi-Word Data from Non-Secure Flash Address (for Non-Secure region)
  * @param[in]  u32NSAddr       Starting Non-Secure flash address
  * @param[out] pu32NSRamBuf    Non-Secure sram address to store reading data
  * @param[in]  u32Size         Total read byte counts, it should be word aligned and maximum size is one page size.
  * @retval     0xF0F00000      u32NSAddr or pu32NSRamBuf region isn't in Non-Secure area
  * @retval     0xF0F00001      Wrong u32NSAddr, pu32NSRamBuf or u32Size parameter
  * @retval     0xF0F00003      u32NSAddr isn't valid flash address
  * @retval     0xF0F00004      pu32NSRamBuf isn't valid sram address
  * @retval     -1              Multi-words read failed
  * @retval     0               Read operation is success
  * @details    To read multi-words data start from specified Non-Secure flash address.
  *             And maximum read size is one page size, 2048 bytes.
  */
int32_t BL_FlashMultiRead(uint32_t u32NSAddr, uint32_t *pu32NSRamBuf, uint32_t u32Size);


/**
  * @brief      Program Data into Non-Secure Flash Address (for Non-Secure region)
  * @param[in]  u32NSAddr   Non-Secure flash address
  * @param[in]  u32Data     32-bit Data to program
  * @retval     0xF0F00000  u32NSAddr isn't in Non-Secure area
  * @retval     0xF0F00001  u32NSAddr isn't word aligned
  * @retval     0xF0F00003  u32NSAddr isn't valid flash address
  * @retval     -1          Flash write failed
  * @retval     0           Program command is finished
  * @details    To program word data into specified Non-Secure flash address.
  */
int32_t BL_FlashWrite(uint32_t u32NSAddr, uint32_t u32Data);


/**
  * @brief      Program Multi-Word Data into Non-Secure Flash Address (for Non-Secure region)
  * @param[in]  u32NSAddr       Starting Non-Secure flash address
  * @param[in]  pu32NSRamBuf    Non-Secure sram buffer address to store program data
  * @param[in]  u32Size         Total program byte counts, it should be word aligned and maximum size is one page size.
  * @retval     0xF0F00000      u32NSAddr or pu32NSRamBuf region isn't in Non-Secure area
  * @retval     0xF0F00001      Wrong u32NSAddr, pu32NSRamBuf or u32Size parameter
  * @retval     0xF0F00003      u32NSAddr isn't valid flash address
  * @retval     0xF0F00004      pu32NSRamBuf isn't valid sram address
  * @retval     -1              Multi-words write failed
  * @retval     0               Program operation is finished
  * @details    To program multi-words data start from specified Non-Secure flash address.
  *             And maximum program size is one page size, 2048 bytes.
  */
int32_t BL_FlashMultiWrite(uint32_t u32NSAddr, uint32_t *pu32NSRamBuf, uint32_t u32Size);


/**
  * @brief      Non-Secure Flash Page Erase (for Non-Secure region)
  * @param[in]  u32NSAddr   Non-Secure flash region to be erased and must be a page size aligned address.
  * @retval     0xF0F00000  u32NSAddr region isn't in Non-Secure area
  * @retval     0xF0F00001  u32NSAddr isn't page size aligned
  * @retval     0xF0F00003  u32NSAddr isn't valid flash address
  * @retval     -1          Page erase failed
  * @retval     0           Page erase success
  * @details    This API is used to perform page erase command on specified Non-Secure flash address.
  *             And this address must be a page size aligned address.
  */
int32_t BL_FlashPageErase(uint32_t u32NSAddr);


/**
  * @brief      Get Non-Secure Flash Area CRC32 Checksum (for Non-Secure region)
  * @param[in]  u32NSAddr       Non-Secure flash region to be calculated. u32NSAddr must be a page size aligned address.
  * @param[in]  u32ByteCount    Byte counts of Non-Secure flash area to be calculated. It must be multiple of 2048 bytes.
  * @retval     0xF0F00000      u32NSAddr region isn't in Non-Secure area
  * @retval     0xF0F00001      Wrong u32NSAddr or u32ByteCount parameter
  * @retval     0xF0F00003      u32NSAddr isn't valid flash address
  * @retval     -1              Execute CRC32 operation failed
  * @retval     Result of CRC32 checksum
  * @details    This API will calculate the CRC32 checksum result of specified Non-Secure flash area.
  *             And the starting address and calculated szie must be all 2048 bytes page size aligned.
  */
uint32_t BL_FlashChecksum(uint32_t u32NSAddr, uint32_t u32ByteCount);


/**
  * @brief      Check Non-Secure Flash Area Data are all ONE or not (for Non-Secure region)
  * @param[in]  u32NSAddr       Non-Secure flash region to be calculated. u32NSAddr must be a page size aligned address.
  * @param[in]  u32ByteCount    Byte counts of Non-Secure flash area to be calculated. It must be multiple of 2048 bytes.
  * @retval     0xF0F00000      u32NSAddr region isn't in Non-Secure area
  * @retval     0xF0F00001      Wrong u32NSAddr or u32ByteCount parameter
  * @retval     0xF0F00003      u32NSAddr isn't valid flash address
  * @retval     -1              Execute Check Flash All One operation failed
  * @retval     0xA11FFFFF      The contents of verified Non-Secure flash area are 0xFFFFFFFF
  * @retval     0xA1100000      Some contents of verified Non-Secure flash area are not 0xFFFFFFFF
  * @details    This API is used to check specified Non-Secure flash area are all 0xFFFFFFFF or not.
  */
uint32_t BL_CheckFlashAllOne(uint32_t u32NSAddr, uint32_t u32ByteCount);


/**
  * @brief      Read Company ID
  * @param      None
  * @return     The company ID (32-bit)
  * @details    The company ID of Nuvoton is fixed to be 0xDA.
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
  * @brief      Read the 64-bits OTP Data
  * @param[in]  u32OTP      The specified OTP number, it must be between 0 ~ 255.
  * @param[out] pu32DataOut Buffer to store 64-bit OTP data
  * @retval     0           OPT unlock
  * @retval     1           OPT lock
  * @retval     0xF0F00001  Wrong u32OTP or pu32DataOut parameter
  * @retval     -1          Execute BL_ReadOTP failed
  * @details    Thia API can get the specified 64-bits OTP data.
  */
int32_t BL_ReadOTP(uint32_t u32OTP, uint32_t *pu32DataOut);


/**
  * @brief      Get XOM Active Status
  * @param[in]  u32XOM      Specified XOM region, it must be between 0~3.
  * @retval     0xF0F00001  Invalid u32XOM number
  * @retval     0           Current XOM region isn't active yet
  * @retval     1           Current XOM region is active
  * @details    This API will return specified XOM region active status.
  */
uint32_t BL_GetXOMActiveStatus(uint32_t u32XOM);


/**
  * @brief      Read XOM Setting (for Non-Secure region)
  * @param[in]  u32XOM          Specified XOM region, it must be between 0~3
  * @param[out] pu32Base        Return specified XOM base address
  * @param[out] pu32PageCnt     Return specified XOM page count
  * @retval     0xF0F00000      pu32Base, pu32PageCnt or XOM base address isn't in Non-Secure area
  * @retval     0xF0F00001      Wrong u32XOM, pu32Base or pu32PageCnt parameter
  * @retval     0xF0F00003      XOM base address isn't valid flash address
  * @retval     0xF0F00004      pu32Base or pu32PageCnt isn't valid sram address
  * @retval     0xF0F00005      XOM region isn't configured
  * @retval     0               Read specified XOM setting success
  * @details    This API will read specified XOM relative settings.
  */
int32_t BL_ReadXOMRegion(uint32_t u32XOM, uint32_t *pu32Base, uint32_t *pu32PageCnt);


/**
  * @brief      Set XOM Region and Active (for Non-Secure region)
  * @param[in]  u32XOM          Specified XOM region, it must be between 0~3
  * @param[in]  u32Base         Base address of XOM region
  * @param[in]  u32PageCnt      Page count of XOM region
  * @param[in]  u32IsDebugMode  1: Enable XOM debug mode; others will disable XOM debug mode.
  * @retval     0xF0F00000      XOM region isn't in Non-Secure area
  * @retval     0xF0F00001      Wrong u32XOM, u32Base or u32PageCnt parameter
  * @retval     0xF0F00003      u32Base isn't valid flash address
  * @retval     0xF0F00006      XOM region has configured
  * @retval     0xF0F00007      XOM region has active
  * @retval     -1              Set XOM failed
  * @retval     0               Set specified XOM success
  * @details    This API will set specified XOM active.
  */
int32_t BL_SetXOMRegion(uint32_t u32XOM, uint32_t u32Base, uint32_t u32PageCnt, uint32_t u32IsDebugMode);


/**
  * @brief      Erase XOM Setting (for Non-Secure region)
  * @param[in]  u32XOMBase  Specified XOM region to be erase
  * @retval     0xF0F00000  u32XOMBase or erase XOM region isn't in Non-Secure area
  * @retval     0xF0F00001  u32XOMBase isn't page size aligned
  * @retval     0xF0F00003  u32XOMBase isn't valid flash address
  * @retval     0xF0F00008  Invalid u32XOMBase address
  * @retval     -1          Erase XOM region failed
  * @retval     0           Erase XOM region success
  * @details    This API will erase specified XOM region data and relavive XOM setting.
  */
int32_t BL_EraseXOMRegion(uint32_t u32XOMBase);


/**
  * @brief      Get XOM Erased Status
  * @param      None
  * @retval     -1      Erase XOM operation failed
  * @retval     0       Erase XOM operation success
  * @details    This API will return the XOM erase operation is success or not.
  */
int32_t BL_GetXOMEraseStatus(void);


/**
  * @brief      Read KPKEYSTS Status
  * @param      None
  * @return     KPKEYSTS register status
  * @details    This API can read KPROM KPKEYSTS register status.
  */
uint32_t BL_GetKPROMStatus(void);


/**
  * @brief      Read KPKEYCNT Status
  * @param      None
  * @return     KPKEYCNT register status
  * @details    This API can read KPROM KPKEYCNT register status.
  */
uint32_t BL_GetKPROMCounter(void);


/**
  * @brief      Read KPCNT Status
  * @param      None
  * @return     KPCNT register status
  * @details    This API can read KPROM KPCNT register status.
  */

uint32_t BL_GetKPROMPowerOnCounter(void);


/**
  * @brief      Execute KPROM Key Comparison
  * @param[in]  key0        KPROM key0
  * @param[in]  key1        KPROM key1
  * @param[in]  key2        KPROM key2
  * @retval     0xF0F00009  KPROM function isn't enabled
  * @retval     0xF0F0000A  Trigger KPROM key comparison is FORBID
  * @retval     0xF0F0000B  KPROM Key is mismatch
  * @retval     0xF0F0000C  KPROM key still locked
  * @retval     0           KPROM Key are matched
  * @details    User can use this API to unlock KPROM write-protection then execute FMC program command well.
  */
int32_t BL_TrgKPROMCompare(uint32_t key0, uint32_t key1, uint32_t key2);


/**
  * @brief      Enable KPROM Write-Protect
  * @param[in]  key0        KPROM key0
  * @param[in]  key1        KPROM key1
  * @param[in]  key2        KPROM key2
  * @param[in]  u32attr     [23:16] means Power-on Maximum Number of Error Key Entry
  *                         [15:8] means Maximum Number of Error Key Entry for Each Power-on
  *                         [7:0], 0xA5: KPROM and flash region are write-protected;
  *                                0xA4: KPROM, flash and CONFIG region are write-protected.
  * @retval     0xF0F00001  Error u32attr parameter
  * @retval     0xF0F0000C  KPROM still locked
  * @retval     0xF0F0000E  Set KPROM fail
  * @retval     -1          FMC ISP command fail
  * @retval     0           Enable KPROM function is success
  * @details    User can use this API to lock FMC program command in flash or CONFIG region.
  */
int32_t BL_SetKPROM(uint32_t key0, uint32_t key1, uint32_t key2, uint32_t u32attr);


/**
  * @brief      Execute CHIP Reset
  * @param      None
  * @return     None
  * @details    This API will perform reset CHIP command to reset chip.
  */
void BL_ResetChip(void);


/**
  * @brief      Get ID ECC R, S digital signature (for Secure code)
  * @param[out] R           R of the (R,S) pair digital signature
  * @param[out] S           S of the (R,S) pair digital signature
  * @retval     -1          Get R, S digital signature fail
  * @retval     0           Success
  * @details    This API will return ECC R, S digital signature of chip ID, include PDID, UID0~2 and UCID0~3.
  */
int32_t BL_GetIDECCSignature(uint32_t *R, uint32_t *S);


/**
  * @brief      Initial UART1 Command mode (for Secure code)
  * @param      None
  * @retval     -1      Execute API in Non-Secure code
  * @details    Execute this API will initial UART1 command mode.
  *             User can use command tool (Secure ISP Tool) to conmunicate with target chip.
  * @note       User should configure relate UART1 functional setting before calling this API.
  */
int32_t BL_UART1UpgradeFlowInit(void);


/**
  * @brief      Process UART1 Interrupt (for Secure code)
  * @param      None
  * @retval     -1      Execute API in Non-Secure code
  * @retval     0       Process UART1 interrupt event success
  * @details    This API is used to process UART1 command data and should be called in UART1_IRQHandler().
  */
int32_t BL_ProcessUART1Interrupt(void);


/**
  * @brief      Initial USBD Command mode (for Secure code)
  * @param      None
  * @retval     -1      Execute API in Non-Secure code
  * @details    Execute this API will initial USBD command mode.
  *             User can use command tool (Secure ISP Tool) to conmunicate with target chip.
  * @note       User should configure relate USBD functional setting before calling this API.
  */
int32_t BL_USBDUpgradeFlowInit(void);


/**
  * @brief      Process USBD Interrupt (for Secure code)
  * @param      None
  * @retval     -1      Execute API in Non-Secure code
  * @retval     0       Process USBD interrupt event success
  * @details    This API is used to process USBD command data and should be called in USBD_IRQHandler().
  */
int32_t BL_ProcessUSBDInterrupt(void);


/**
  * @brief      Initial Random Number Generator (for Secure code)
  *
  * @param[in]  rng     The structure of random number generator
  * @param[in]  opt     Operation mode. Possible options are,
  *                         (BL_RNG_PRNG | BL_RNG_LIRC32K),
  *                         (BL_RNG_PRNG | BL_RNG_LXT),
  *                         (BL_RNG_SWRNG | BL_RNG_LIRC32K),
  *                         (BL_RNG_SWRNG | BL_RNG_LXT)
  * @retval     -1      Fail
  * @retval     0       Success
  *
  * @details    This API is used to initial random number generator for generate random number after calling BL_Random API.
  */
int32_t BL_RandomInit(BL_RNG_T *rng, uint32_t opt);


/**
  * @brief      Generate Random Number (for Secure code)
  *
  * @param[in]  rng     The structure of random number generator
  * @param[out] p       Starting buffer address to store random number
  * @param[in]  size    Total byte counts of random number
  * @retval     -1      Fail
  * @retval     0       Success
  * @details    This API is used to generate random number.
  */
int32_t BL_Random(BL_RNG_T *rng, uint8_t *p, uint32_t size);


/**
  * @brief      This function makes USBD module to be ready to use (for Secure code)
  * @param[in]  param           The structure of USBD information
  * @param[in]  pfnClassReq     USB Class request callback function
  * @param[in]  pfnSetInterface USB Set Interface request callback function
  * @retval     -1      Execute API in Non-Secure code
  * @retval     0       Success
  * @details    This function will enable USB controller, USB PHY transceiver and pull-up resistor of USB_D+ pin. USB PHY will drive SE0 to bus.
  */
int32_t BL_USBDOpen(const S_USBD_INFO_T *param, CLASS_REQ pfnClassReq, SET_INTERFACE_REQ pfnSetInterface);


/**
  * @brief      This function makes USB host to recognize the device (for Secure code)
  * @param      None
  * @retval     -1      Execute API in Non-Secure code
  * @retval     0       Success
  * @details    Enable WAKEUP, FLDET, USB and BUS interrupts. Disable software-disconnect function after 100ms delay with SysTick timer.
  */
int32_t BL_USBDStart(void);


/**
  * @brief      Install EP Callback Function (for Secure code)
  * @param[in]  ep              EP number
  * @param[in]  pfnEPHandler    EP callback function
  * @retval     -1      Fail
  * @retval     0       Success
  * @details    This function is used to set specific EP callback function
  */
int32_t BL_USBDInstallEPHandler(uint32_t ep, SET_INTERFACE_REQ pfnEPHandler);

/*@}*/ /* end of group BL_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group BL_Driver */

/*@}*/ /* end of group Standard_Driver */


#ifdef __cplusplus
}
#endif

#endif  /* __BOOTLOADER_H__ */

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
