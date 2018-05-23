/**
 * @file xmc1_flash.h
 * @date 2015-06-20
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2014-12-10:
 *     - Initial <br>
 * 2015-02-20:
 *     - Updated for Documentation related changes<br>
 * 2015-06-20: 
 *     - Removed version macros and declaration of GetDriverVersion API 
 *
 * @endcond 
 *
 */

#ifndef XMC1_FLASH_H
#define XMC1_FLASH_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "xmc_common.h"

#if UC_FAMILY == XMC1

#include <XMC1000_RomFunctionTable.h>

/**
* @addtogroup XMClib XMC Peripheral Library
* @{
*/

/**
* @addtogroup FLASH
* @{
*/

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define XMC_FLASH_PAGES_PER_SECTOR (16U)    /**< Number of pages in a sector. A page consists of 16 blocks.*/
#define XMC_FLASH_BLOCKS_PER_PAGE  (16U)    /**< Number of blocks in a page. A block consists of 4 words(16 bytes).*/

#define XMC_FLASH_BYTES_PER_SECTOR (4096U)  /**< Number of bytes in a sector. (16 pages * 256 bytes = 4096 bytes)*/
#define XMC_FLASH_BYTES_PER_PAGE   (256U)   /**< Number of bytes in a page. (16 blocks * 16 bytes = 256 bytes)*/
#define XMC_FLASH_BYTES_PER_BLOCK  (16U)    /**< Number of bytes in a block. (128 bits = 16 bytes)*/

#define XMC_FLASH_WORDS_PER_SECTOR (1024U)  /**< Number of words in a sector. (16 pages * 64 words = 1024 words)*/
#define XMC_FLASH_WORDS_PER_PAGE   (64U)    /**< Number of words in a page. (16 blocks * 4 words = 64 words) */
#define XMC_FLASH_WORDS_PER_BLOCK  (4U)     /**< Number of words in a block. (128 bit / 32 bit = 4 words) */

#define FLASH_BLOCK_ADDR_MASK      (15U)    /*   Bitwise AND with block address is done to check the address alignment.
                                                 Applicable to XMC_FLASH_WriteBlocks() and XMC_FLASH_VerifyBlocks()
                                                 APIs.*/
#define FLASH_PAGE_ADDR_MASK       (255U)   /*   Bitwise AND with page address is done to check the address alignment.
                                                 Applicable to XMC_FLASH_ErasePages() API.*/
#define FLASH_SECTOR_ADDR_MASK     (4095U)  /*   Bitwise AND with sector address is done to check the address alignment.
                                                 Applicable to XMC_FLASH_EraseSector API.*/

#define XMC_FLASH_BASE             (0x10001000U) /**< Starting address of flash for XMC1 family of microcontrollers*/

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 *  Defines the status of flash, to verify the flash related API calls. Use type \a XMC_FLASH_STATUS_t for this enum.
 *  The members defines the respective masked status bits of \a NVMSTATUS register.
 */
typedef enum XMC_FLASH_STATUS
{
  XMC_FLASH_STATUS_OK                   = 0U,                          /**< Flash related operation was successfully
                                                                            completed*/
  XMC_FLASH_STATUS_BUSY                 = NVM_NVMSTATUS_BUSY_Msk    ,  /**< Cannot execute the flash request because
                                                                            another operation is in progress*/
  XMC_FLASH_STATUS_SLEEP_MODE           = NVM_NVMSTATUS_SLEEP_Msk   ,  /**< Flash is in sleep mode*/
  XMC_FLASH_STATUS_VERIFY_ERROR         = NVM_NVMSTATUS_VERR_Msk    ,  /**< Flash reported a verification failure*/
  XMC_FLASH_STATUS_ECC1_READ_ERROR      = NVM_NVMSTATUS_ECC1READ_Msk,  /**< Flash reports a single bit failure, and it
                                                                            is automatically corrected.*/
  XMC_FLASH_STATUS_ECC2_READ_ERROR      = NVM_NVMSTATUS_ECC2READ_Msk,  /**< Flash reported at least two bit failure*/
  XMC_FLASH_STATUS_WRITE_PROTOCOL_ERROR = NVM_NVMSTATUS_WRPERR_Msk  ,  /**< Write/Verify operation on a block is
                                                                            failed due to protocol violations or write
                                                                            protected sectors*/
} XMC_FLASH_STATUS_t;

/**
 *  Defines NVM ready interrupt event. Use type \a XMC_FLASH_EVENT_t for this enum.
 */
typedef enum XMC_FLASH_EVENT
{
  XMC_FLASH_EVENT_READY = NVM_NVMCONF_INT_ON_Msk  /**< Generates the NVM ready interrupts on flash sequence completion*/
} XMC_FLASH_EVENT_t;

/**
 *  Defines hard read levels for strict data verification. Use type \a XMC_FLASH_HARDREAD_LEVEL_t for this enum.
 *  These \a hardread levels provide some margin to ensure that the data is really programmed with suitably distinct
 *  levels for written and erased bits.
 */
typedef enum XMC_FLASH_HARDREAD_LEVEL
{
  XMC_FLASH_HARDREAD_LEVEL_NORMAL  = (uint16_t)0x0, /**< No \a hardread level verification enabled (Normal read)*/
  XMC_FLASH_HARDREAD_LEVEL_WRITTEN = (uint16_t)0x1, /**< Enables strict margin compare for written data cells*/
  XMC_FLASH_HARDREAD_LEVEL_ERASED  = (uint16_t)0x2  /**< Enables strict margin compare for erased data cells*/
} XMC_FLASH_HARDREAD_LEVEL_t;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @param address   Pointer to the starting address of the flash page from where the erase starts
 * @param num_pages Number of pages to be erased.<BR> Range: [1 to (flash size / 256)]
 *
 * @return None
 *
 * \par<b>Description:</b><BR>
 * Erases a set of flash memory pages.<BR><BR>
 * Erase starts from the  specified \a address.
 * It erases a maximum number of \a num_pages flash pages. The maximum erasable pages are limited to
 * microcontroller flash size. It sets NVMPROG register to continuous page erase mode before erase and resets
 * it action back to normal state on completion. Call XMC_FLASH_GetStatus() after calling this API to verify the erase
 * operation.\n
 *
 * \par<b>Note:</b><BR>
 * Flash will be in busy state during erase operation. Hence no operations on flash are allowed until it completes.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EraseSector(), XMC_FLASH_ErasePage() \n\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 * 
 */
void XMC_FLASH_ErasePages(uint32_t *address, uint32_t num_pages);

/**
 *
 * @param address    Pointer to the starting address of flash block from where the write starts.
 * @param data       Pointer to the source address where targeted data blocks are located.
 * @param num_blocks Maximum number of flash block writes needed.<BR> Range:  [1 to (flash size / 16)]
 * @param verify     If \a true, hardware verification after block write is enabled else disabled.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Writes a set of data blocks into the flash.\n\n Minimum possible writable area is 16 byte block. It sets the NVMPROG
 * register to continuous block write mode before write and resets it action back to normal state on completion.
 * Call XMC_FLASH_GetStatus() API after calling this API to verify the erase operation.
 *
 * \par<b>Note</b><br>
 * Flash will be busy state during write is ongoing, hence no operations allowed until it completes.
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 * 
 */
void XMC_FLASH_WriteBlocks(uint32_t *address, const uint32_t *data, uint32_t num_blocks, bool verify);

/**
 *
 * @param address    Pointer to the starting address of flash block from where the verification starts.
 * @param data       Pointer to the source address where targeted data blocks are located.
 * @param num_blocks Maximum number of flash blocks writes needed.<BR> Range:  [1 to (flash size / 16)]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Performs verification of written data blocks.\n\n After calling XMC_FLASH_WriteBlocks() API, calling this API will
 * verify the correctness of written blocks. It sets the \a NVMPROG register into continuous block write mode before
 * write and resets it action back to normal state on completion. It reads back the written data blocks from the flash
 * and verify the values against the internal buffer values. Calling XMC_FLASH_GetStatus() API after calling this API
 * validates the result of verification.
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 * 
 */
void XMC_FLASH_VerifyBlocks(uint32_t *address, const uint32_t *data, uint32_t num_blocks);


/**
 *
 * @param  address    Pointer to the starting address of flash block from where the read starts.
 * @param  data       Pointer to the destination address, where the read data blocks to be stored.
 * @param  num_blocks Number of blocks to be read.<BR> Range:  [1 to (flash size / 16)]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Reads multiple blocks from flash in one shot, starting from the \a address specified.\n\n The read blocks are stored
 * into the locations starting from the \a data address. Calling XMC_FLASH_GetStatus() API after calling this API
 * verifies the read operation.
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 * 
 */
void XMC_FLASH_ReadBlocks(uint32_t *address, uint32_t *data, uint32_t num_blocks);


/**
 *
 * @param address Pointer to the flash word address from where the read is expected
 *
 * @return <BR>
 *   a 32bit data word stored in the specified \a address.
 *
 * \par<b>Description:</b><br>
 * Reads a single word from the specified flash\a address.\n\n Calling XMC_FLASH_GetStatus() API after calling this
 * API returns the read status.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_ReadBlocks() 
 *
 */
__STATIC_INLINE uint32_t XMC_FLASH_ReadWord(const uint32_t *const address)
{
  return *address;
}

/**
 *
 * @param address Pointer to the starting address of the page to be erased
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Erases a single flash page associated to the specified \a address.\n\n XMC1000 Flash can be erased with granularity
 * of one page = 16 blocks of 16 Bytes = 256 Bytes using this API. It internally calls the Flash Firmware routine
 * \a XMC1000_NvmErasePage(unsigned long pageAddr) to perform the erase operation. Refer XMC1000 reference manual
 * for more details on flash firmware routines (Section 25.3). Call XMC_FLASH_GetStatus() API after calling this API,
 * to verify the erase operation.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_ErasePages() \n\n\n
 */
void XMC_FLASH_ErasePage(uint32_t *address);

/**
 *
 * @param address    Pointer to the starting address of flash page from where the programming starts.
 * @param data       Pointer to the source address where targeted data blocks are located.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Erases, programs and verifies a single flash page starting from the \a address specified.\n\n XMC1000 Flash can be
 * programmed with granularity of one page = 16 blocks of 16 Bytes = 256 Bytes using this API. It internally calls the
 * Flash Firmware routine \a XMC1000_NvmProgVerify(unsigned long pageAddr) to perform the programming. Refer XMC1000
 * reference manual of for more details on flash firmware routines (Section 25.3). Call XMC_FLASH_GetStatus() API after
 * calling this API, to verify the erase operation.
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 * 
 */
void XMC_FLASH_ProgramVerifyPage(uint32_t *address, const uint32_t *data);

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the flash to enter into sleep mode by resetting the NVMCONF register NVM_ON bit.\n\n Flash can wake up from
 * sleep mode on any flash operation completion ready event trigger. To disable the sleep mode any time during execution
 * call the API XMC_FLASH_ExitSleepMode().\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_ExitSleepMode()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_EnterSleepMode(void)
{
  NVM->NVMCONF &= (uint16_t)(~(uint32_t)NVM_NVMCONF_NVM_ON_Msk);
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the flash to exit from sleep mode by setting the NVMCONF register NVM_ON bit.\n\n Calling the API
 * XMC_FLASH_EnterSleepMode() allows the flash to renter into sleep mode.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnterSleepMode()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_ExitSleepMode(void)
{
  NVM->NVMCONF |= (uint16_t)NVM_NVMCONF_NVM_ON_Msk;
}


/**
 *
 * @param sector Flash sector number for which the address extraction is needed<BR> Range:  [0 to 51]
 *
 * @return uint32_t Starting address of the sector specified<BR> Range:  [0x10001000 to 0x10032000]
 *
 * \par<b>Description:</b><br>
 * Finds the starting address of the specified \a sector number.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 *
 */
__STATIC_INLINE uint32_t XMC_FLASH_GetSectorAddress(uint32_t sector)
{
  return (XMC_FLASH_BASE + (XMC_FLASH_BYTES_PER_SECTOR * sector));
}


/**
 * 
 * @param num_sectors    Number of sectors to be protected<BR> Range:  [0 to 51]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Protect the flash sectors starting from 0th sector to the specified \a num_sectors.\n\n It sets the NVMCONF register
 * SECPROT field with the value specified in \a num_sectors. Changing the protection limit can be achieved by calling
 * this API at runtime with a different value of \a num_sectors.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 *
 */
__STATIC_INLINE void XMC_FLASH_SetSectorProtection(uint32_t num_sectors)
{
  NVM->NVMCONF &= (~(uint16_t)NVM_NVMCONF_SECPROT_Msk);
  NVM->NVMCONF |= (uint16_t)((uint16_t)num_sectors << NVM_NVMCONF_SECPROT_Pos);
}


/**
 * 
 * @param level Hard read levels specified in \a XMC_FLASH_HARDREAD_LEVEL_t.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets the hard read level for verification process.\n\n It insists the flash to do a strict margin compare
 * with the written/erased data against the internal buffer. Sets the NVMCONF register HRLEV field with \a level
 * value. This hardread level is used until the end of the verification sequence and, may not be changed in between.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 *
 */
__STATIC_INLINE void XMC_FLASH_SetHardReadLevel(XMC_FLASH_HARDREAD_LEVEL_t level)
{
  NVM->NVMCONF &= (uint16_t)(~(uint16_t)NVM_NVMCONF_HRLEV_Msk);
  NVM->NVMCONF |= (uint16_t)(level<< (uint16_t)NVM_NVMCONF_HRLEV_Pos);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif

#endif /* FLASH_H */
