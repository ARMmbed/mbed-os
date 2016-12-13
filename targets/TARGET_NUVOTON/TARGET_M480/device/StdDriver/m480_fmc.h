/**************************************************************************//**
 * @file     fmc.h
 * @version  V1.00
 * @brief    M480 Series Flash Memory Controller Driver Header File
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __FMC_H__
#define __FMC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_FMC_Driver FMC Driver
  @{
*/


/** @addtogroup M480_FMC_EXPORTED_CONSTANTS FMC Exported Constants
  @{
*/


/*---------------------------------------------------------------------------------------------------------*/
/* Define Base Address                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define FMC_APROM_BASE          0x00000000UL    /*!< APROM base address          */
#define FMC_APROM_END           0x00080000UL    /*!< APROM end address           */
#define FMC_APROM_BANK0_END     (FMC_APROM_END/2)    /*!< APROM bank0 end address     */
#define FMC_LDROM_BASE          0x00100000UL    /*!< LDROM base address          */
#define FMC_LDROM_END           0x00101000UL    /*!< LDROM end address           */
#define FMC_SPROM_BASE          0x00200000UL    /*!< SPROM base address          */
#define FMC_SPROM_END           0x00201000UL    /*!< SPROM end address           */
#define FMC_CONFIG_BASE         0x00300000UL    /*!< User Configuration address  */
#define FMC_USER_CONFIG_0       0x00300000UL    /*!< User Config 0 address       */
#define FMC_USER_CONFIG_1       0x00300004UL    /*!< User Config 1 address       */
#define FMC_USER_CONFIG_2       0x00300008UL    /*!< User Config 2 address       */
#define FMC_KPROM_BASE          0x00301000UL    /*!< Security ROM base address   */
#define FMC_OTP_BASE            0x00310000UL    /*!< OTP flash base address      */

#define FMC_FLASH_PAGE_SIZE     0x1000          /*!< Flash Page Size (4K bytes)  */
#define FMC_PAGE_ADDR_MASK      0xFFFFF000UL    /*!< Flash page address mask     */

#define FMC_APROM_SIZE          FMC_APROM_END   /*!< APROM Size                  */
#define FMC_BANK_SIZE           (FMC_APROM_SIZE/2)   /*!< APROM Bank Size             */
#define FMC_LDROM_SIZE          0x1000          /*!< LDROM Size (4 Kbytes)       */
#define FMC_SPROM_SIZE          0x1000          /*!< SPROM Size (4 Kbytes)       */
#define FMC_OTP_ENTRY_CNT       256             /*!< OTP entry number            */


/*---------------------------------------------------------------------------------------------------------*/
/*  ISPCMD constant definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define FMC_ISPCMD_READ         0x00            /*!< ISP Command: Read flash word         */
#define FMC_ISPCMD_READ_UID     0x04            /*!< ISP Command: Read Unique ID          */
#define FMC_ISPCMD_READ_ALL1    0x08            /*!< ISP Command: Read all-one result     */
#define FMC_ISPCMD_READ_CID     0x0B            /*!< ISP Command: Read Company ID         */
#define FMC_ISPCMD_READ_PID     0x0C            /*!< ISP Command: Read Product ID         */
#define FMC_ISPCMD_READ_CKS     0x0D            /*!< ISP Command: Read checksum           */
#define FMC_ISPCMD_PROGRAM      0x21            /*!< ISP Command: Write flash word        */
#define FMC_ISPCMD_PAGE_ERASE   0x22            /*!< ISP Command: Page Erase Flash        */
#define FMC_ISPCMD_BANK_ERASE   0x23            /*!< ISP Command: Erase Flash bank 0 or 1 */
#define FMC_ISPCMD_BLOCK_ERASE  0x25            /*!< ISP Command: Erase 4 pages alignment of APROM in bank 0 or 1  */
#define FMC_ISPCMD_PROGRAM_MUL  0x27            /*!< ISP Command: Multuple word program   */
#define FMC_ISPCMD_RUN_ALL1     0x28            /*!< ISP Command: Run all-one verification*/
#define FMC_ISPCMD_RUN_CKS      0x2D            /*!< ISP Command: Run checksum calculation*/
#define FMC_ISPCMD_VECMAP       0x2E            /*!< ISP Command: Vector Page Remap       */
#define FMC_ISPCMD_READ_64      0x40            /*!< ISP Command: Read double flash word  */
#define FMC_ISPCMD_PROGRAM_64   0x61            /*!< ISP Command: Write double flash word */

#define IS_BOOT_FROM_APROM      0               /*!< Is booting from APROM                */
#define IS_BOOT_FROM_LDROM      1               /*!< Is booting from LDROM                */

#define READ_ALLONE_YES         0xA11FFFFF      /*!< Check-all-one result is all one.     */
#define READ_ALLONE_NOT         0xA1100000      /*!< Check-all-one result is not all one. */
#define READ_ALLONE_CMD_FAIL    0xFFFFFFFF      /*!< Check-all-one command failed.        */


/*@}*/ /* end of group M480_FMC_EXPORTED_CONSTANTS */


/** @addtogroup M480_FMC_EXPORTED_MACROS FMC Exported Macros
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Macros                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

#define FMC_SET_APROM_BOOT()        (FMC->ISPCTL &= ~FMC_ISPCTL_BS_Msk)         /*!< Select booting from APROM  */
#define FMC_SET_LDROM_BOOT()        (FMC->ISPCTL |= FMC_ISPCTL_BS_Msk)          /*!< Select booting from LDROM  */
#define FMC_ENABLE_AP_UPDATE()      (FMC->ISPCTL |=  FMC_ISPCTL_APUEN_Msk)      /*!< Enable APROM update        */
#define FMC_DISABLE_AP_UPDATE()     (FMC->ISPCTL &= ~FMC_ISPCTL_APUEN_Msk)      /*!< Disable APROM update       */
#define FMC_ENABLE_CFG_UPDATE()     (FMC->ISPCTL |=  FMC_ISPCTL_CFGUEN_Msk)     /*!< Enable User Config update  */
#define FMC_DISABLE_CFG_UPDATE()    (FMC->ISPCTL &= ~FMC_ISPCTL_CFGUEN_Msk)     /*!< Disable User Config update */
#define FMC_ENABLE_LD_UPDATE()      (FMC->ISPCTL |=  FMC_ISPCTL_LDUEN_Msk)      /*!< Enable LDROM update        */
#define FMC_DISABLE_LD_UPDATE()     (FMC->ISPCTL &= ~FMC_ISPCTL_LDUEN_Msk)      /*!< Disable LDROM update       */
#define FMC_ENABLE_SP_UPDATE()      (FMC->ISPCTL |=  FMC_ISPCTL_SPUEN_Msk)      /*!< Enable SPROM update        */
#define FMC_DISABLE_SP_UPDATE()     (FMC->ISPCTL &= ~FMC_ISPCTL_SPUEN_Msk)      /*!< Disable SPROM update       */
#define FMC_DISABLE_ISP()           (FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk)      /*!< Disable ISP function       */
#define FMC_ENABLE_ISP()            (FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk)      /*!< Enable ISP function        */
#define FMC_GET_FAIL_FLAG()         ((FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) ? 1 : 0)  /*!< Get ISP fail flag  */
#define FMC_CLR_FAIL_FLAG()         (FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk)       /*!< Clear ISP fail flag        */

/*@}*/ /* end of group M480_FMC_EXPORTED_MACROS */


/** @addtogroup M480_FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Functions                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

extern void FMC_Close(void);
extern int32_t FMC_Erase(uint32_t u32PageAddr);
extern int32_t FMC_Erase_SPROM(void);
extern int32_t FMC_Erase_Block(uint32_t u32BlockAddr);
extern int32_t FMC_Erase_Bank(uint32_t u32BankAddr);
extern int32_t FMC_GetBootSource(void);
extern void FMC_Open(void);
extern uint32_t FMC_Read(uint32_t u32Addr);
extern int FMC_Read_64(uint32_t u32addr, uint32_t * u32data0, uint32_t * u32data1);
extern uint32_t FMC_ReadCID(void);
extern uint32_t FMC_ReadPID(void);
extern uint32_t FMC_ReadUCID(uint32_t u32Index);
extern uint32_t FMC_ReadUID(uint32_t u32Index);
extern uint32_t FMC_ReadDataFlashBaseAddr(void);
extern void FMC_SetVectorPageAddr(uint32_t u32PageAddr);
extern void FMC_Write(uint32_t u32Addr, uint32_t u32Data);
extern int  FMC_Write_64(uint32_t u32addr, uint32_t u32data0, uint32_t u32data1);
extern int  FMC_Write_OTP(int otp_num, uint32_t low_word, uint32_t high_word);
extern int  FMC_Read_OTP(int otp_num, uint32_t *low_word, uint32_t *high_word);
extern int  FMC_Lock_OTP(int otp_num);
extern int  FMC_Is_OTP_Locked(int otp_num);
extern int32_t FMC_ReadConfig(uint32_t *u32Config, uint32_t u32Count);
extern int32_t FMC_WriteConfig(uint32_t *u32Config, uint32_t u32Count);
extern uint32_t FMC_GetChkSum(uint32_t u32addr, uint32_t u32count);
extern uint32_t FMC_CheckAllOne(uint32_t u32addr, uint32_t u32count);
extern int32_t  FMC_SKey_Setup(uint32_t key[3], uint32_t kpmax, uint32_t kemax, int lock_CONFIG, int lock_SPROM);
extern int32_t  FMC_SKey_Compare(uint32_t key[3]);


/*@}*/ /* end of group M480_FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_FMC_Driver */

/*@}*/ /* end of group M480_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif   // __FMC_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
