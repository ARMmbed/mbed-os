/**
 * @file xmc_flash.h
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
 * @endcond 
 *
 */


#ifndef XMC_FLASH_H
#define XMC_FLASH_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "xmc_common.h"

#if UC_FAMILY == XMC1
 #include "xmc1_flash.h"
#endif
 
#if UC_FAMILY == XMC4
 #include "xmc4_flash.h"
#endif

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup FLASH
 * @brief Flash driver for XMC microcontroller family.
 *
 * Flash is a non volatile memory module used to store instruction code or constant data.
 * The flash low level driver provides support to the following functionalities of flash memory.<BR>
 * <OL>
 * \if XMC4
 * <LI>Provides function to program a page. ( XMC_FLASH_ProgramPage() )</LI><BR>
 * <LI>Provides functions to support read and write protection. ( XMC_FLASH_InstallProtection(), 
 * XMC_FLASH_ConfirmProtection(), XMC_FLASH_VerifyReadProtection(), XMC_FLASH_VerifyWriteProtection() ) </LI><BR> 
 * <LI>Provides function to erase sector. ( XMC_FLASH_EraseSector() ) </LI><BR> 
 * \endif
 * \if XMC1
 * <LI>Provides functions to program and verify pages. ( XMC_FLASH_ProgramPage(), XMC_FLASH_ProgramPages() 
 *  XMC_FLASH_ProgramVerifyPage() )</LI><BR>
 * <LI>Provides functions to write and verify blocks. ( XMC_FLASH_WriteBlocks(), XMC_FLASH_VerifyBlocks() )</LI><BR>
 * <LI>Provides functions to read data in terms of word and blocks. ( XMC_FLASH_ReadBlocks(), XMC_FLASH_ReadWord() )
 * </LI><BR> 
 * <LI>Provides function to erase page. ( XMC_FLASH_ErasePage() ) </LI><BR> 
 * \endif
 * </OL>
 * @{
 */

/*******************************************************************************
 * API PROTOTYPE
 *******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC1
 * Clears the previous error status by reseting the ECC and VERR error status bits of NVMSTATUS register.\n\n
 * Call this API before starting any flash programming / erase related APIs to ensure all previous errors are cleared.
 * \endif
 * \if XMC4
 * Clears the previous error status by reseting the FSR status register.\n\n  Call this API before starting any flash
 * programming / erase related APIs to ensure all previous errors are cleared.
 * \endif
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 *
 */
void XMC_FLASH_ClearStatus(void);

/**
 *
 * @param None
 *
 * @return uint32_t Status of the previous flash operation.
 *
 * \par<b>Description:</b><br>
 * \if XMC1
 * Informs the status of flash by reading the NVMSTATUS register.\n\n  It indicates the ECC, VERR(verification error),
 * WRPERR (Write protocol error) errors as well as the current flash state. After calling the flash read/write/erase
 * operation related APIs, call this API to get the verification status. The return value of this API shall be checked
 * against the members of @ref XMC_FLASH_STATUS_t enumeration to get the relevant status.
 * \endif
 * \if XMC4
 * Informs the status of flash by reading the FSR register.\n\n  It indicates the error status such as PFOPER, SQER,
 * PROER, PFDBER, ORIER, VER errors as well as the current flash state. After calling the flash read/write/erase
 * operation related APIs, call this API to verify flash status. The return value of this API shall be checked against
 * the members of @ref XMC_FLASH_STATUS_t enumeration to get the relevant status.
 * \endif
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
uint32_t XMC_FLASH_GetStatus(void);

/**
 *
 * @param event_msk ORed values of @ref XMC_FLASH_EVENT_t enumeration
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the particular flash events as specified in the input parameter.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_DisableEvent()\n\n\n
 *
 */
void XMC_FLASH_EnableEvent(const uint32_t event_msk);

/**
 *
 * @param event_msk ORed values of @ref XMC_FLASH_EVENT_t enumeration
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the particular flash events as specified in the input parameter.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnableEvent()\n\n\n
 *
 */
void XMC_FLASH_DisableEvent(const uint32_t event_msk);

/**
 *
 * @param address    Pointer to the starting address of flash page from where the programming starts.
 * @param data       Pointer to the source address where targeted data is located.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC1
 * Programs a single flash page associated with the specified \a address.\n\n XMC1000 Flash can be programmed with one
 * page (256 bytes) using this API. It calls the Flash Firmware routine \a XMC1000_NvmProgVerify(unsigned long pageAddr)
 * to perform the programming. Refer XMC1000 reference manual of for more details on flash firmware routines
 * (Section 25.3). Call XMC_FLASH_GetStatus() API after calling this API, to verify the programming operation.
 * \endif
 * \if XMC4
 * Programs a single flash page associated with the specified \a address.\n\n  XMC4000 flash can be programmed with a
 * granularity of 256 bytes page using this API. Before entering into page write process, it clears the error status
 * bits inside status register. It starts the write process by issuing the page mode command followed by the load page
 * command which loads the targeted \a data blocks into internal assembly buffer. Finally, it issues the write page
 * command which programs the \a data into flash. Call XMC_FLASH_GetStatus() API after calling this API, to verify the
 * programming operation.\n
 * \endif
 *
 * \par<b>Note:</b><br>
 * Flash will be busy state during write is ongoing, hence no operations allowed until it completes.
 *
 * \par<b>Related APIs:</b><BR>
 * None 
 *
 */
void XMC_FLASH_ProgramPage(uint32_t *address, const uint32_t *data);

/**
 *
 * @param address Pointer to the starting address of the page to be erased.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC1
 * Erases a complete sector starting from the \a address specified.\n\n  XMC1000 Flash can be erased with granularity
 * of one page = 16 blocks of 16 Bytes = 256 Bytes using this API. It internally calls XMC_FLASH_ErasePages API 16
 * times starting from the first page of the sector.. Call XMC_FLASH_GetStatus() API after calling this API,
 * to verify the erase operation.\n
 * \endif
 *
 * \if XMC4
 * Erases a sector associated with the specified \a address.\n\n  Before erase, it clears the error status bits inside
 * FSR status register. Issues the erase sector command sequence with the specified starting \a address to start flash
 * erase process. Call XMC_FLASH_GetStatus() API after calling this API, to verify the erase operation.\n
 * \endif
 * \if XMC1
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_ErasePages() \n\n\n
 * \endif
 * \if XMC4
 * \par<b>Related APIs:</b><BR>
 * None
 * \endif
 */
void XMC_FLASH_EraseSector(uint32_t *address);

/**
 *
 * @param None
 *
 * @return true if flash is in busy state else returns \a false.
 *
 * \par<b>Description:</b><br>
 * Checks whether flash is in busy state or not.\n\n  It is checked by calling the XMC_FLASH_GetStatus() API internally.
 * Refer XMC_FLASH_GetStatus() for more details.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_GetStatus()\n\n\n
 *
 */
__STATIC_INLINE bool XMC_FLASH_IsBusy(void)
{
  return (bool)(XMC_FLASH_GetStatus() & XMC_FLASH_STATUS_BUSY);
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

