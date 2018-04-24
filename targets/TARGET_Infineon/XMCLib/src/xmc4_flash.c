/**
 * @file xmc4_flash.c
 * @date 2016-01-08
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
 * 2015-02-10:
 *     - Initial <br>
 *
 * 2015-06-20: 
 *     - Removed definition of GetDriverVersion API 
 *
 * 2015-08-17: 
 *     - Added the below API's to the public interface.
 *       1. XMC_FLASH_Reset
 *       2. XMC_FLASH_ErasePhysicalSector
 *       3. XMC_FLASH_EraseUCB
 *       4. XMC_FLASH_ResumeProtection
 *       5. XMC_FLASH_RepairPhysicalSector
 *
 * 2016-01-08: 
 *     - Wait until operation is finished for the next functions:
 *       1. XMC_FLASH_InstallProtection
 *       2. XMC_FLASH_ConfirmProtection
 *       3. XMC_FLASH_ProgramPage
 *       4. XMC_FLASH_EraseSector
 *       5. XMC_FLASH_ErasePhysicalSector
 *       6. XMC_FLASH_EraseUCB
 *     - Fix XMC_FLASH_VerifyReadProtection and XMC_FLASH_VerifyWriteProtection
 *
 * @endcond 
 *
 */

#include "xmc_flash.h"

#if UC_FAMILY == XMC4

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define XMC_FLASH_PROTECTION_CONFIGURATION_WORDS (8UL) /* Used to upadte the assembly buffer during protection 
                                                          configuration */
#define XMC_FLASH_PROT_CONFIRM_OFFSET (512UL) /* Offset address for UCB page */
#define XMC_FLASH_PROT_CONFIRM_WORDS  (4UL)
#define XMC_FLASH_PROT_CONFIRM_CODE   (0x8AFE15C3UL)

/*********************************************************************************************************************
 * LOCAL FUNCTIONS
 ********************************************************************************************************************/
void XMC_FLASH_lEnterPageModeCommand(void);
void XMC_FLASH_lLoadPageCommand(uint32_t low_word, uint32_t high_word);
void XMC_FLASH_lWritePageCommand(uint32_t *page_start_address);
void XMC_FLASH_lWriteUCBPageCommand(uint32_t *page_start_address);
void XMC_FLASH_lEraseSectorCommand(uint32_t *sector_start_address);
void XMC_FLASH_lDisableSectorWriteProtectionCommand(uint32_t user, uint32_t password_0, uint32_t password_1);
void XMC_FLASH_lDisableReadProtectionCommand(uint32_t password_0, uint32_t password_1);
void XMC_FLASH_lRepairPhysicalSectorCommand(void);
void XMC_FLASH_lErasePhysicalSectorCommand(uint32_t *sector_start_address);
void XMC_FLASH_lClearStatusCommand(void);

/*
 * Command to program the PFLASH in to page mode, so that assembly buffer is used 
 */
void XMC_FLASH_lEnterPageModeCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = (uint32_t)0x50U;
}

/*
 * Command to load the data into the page assembly buffer 
 */
void XMC_FLASH_lLoadPageCommand(uint32_t low_word, uint32_t high_word)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x55f0U);
  *address = low_word;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x55f4U);
  *address = high_word;
}

/*
 * Command to start the programming of one page with data from the assembly buffer
 */
void XMC_FLASH_lWritePageCommand(uint32_t *page_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xa0U;
  address = page_start_address;
  *address = 0xaaU;
}

/*
 * Command to start the programming of UCB page with data from the assembly buffer
 */
void XMC_FLASH_lWriteUCBPageCommand(uint32_t *page_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xc0U;
  address = page_start_address;
  *address = 0xaaU;
}

/*
 * Command to erase sector which is starting with the specified address
 */
void XMC_FLASH_lEraseSectorCommand(uint32_t *sector_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = sector_start_address;
  *address = 0x30U;
}


/*
 * Command to temporarily disables the write protection belonging to the the USER specified, when passwords match with their
 * configured values
 */
void XMC_FLASH_lDisableSectorWriteProtectionCommand(uint32_t user, uint32_t password_0, uint32_t password_1)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x553cU);
  *address = user;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_0;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_1;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5558U);
  *address = 0x05U;
}

/*
 * Command to temporarily disables the read protection along with write protection, when passwords match with their
 * configured values
 */
void XMC_FLASH_lDisableReadProtectionCommand(uint32_t password_0, uint32_t password_1)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x553cU);
  *address = 0x00U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_0;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_1;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5558U);
  *address = 0x08U;
}

/*
 * Command to clear FSR.PROG and FSR.ERASE and the error flags in FSR such as PFOPER, SQER, PROER, PFDBER, ORIER, VER
 */
void XMC_FLASH_lClearStatusCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xf5U;
}

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

 /*
 * This API shall clear Program, erase and error flags(PFOPER, SQER, PROER, PFDBER, ORIER, VER) of FSR register.
 */
void XMC_FLASH_ClearStatus(void)
{
  XMC_FLASH_lClearStatusCommand();
}

/*
 * This API returns the FSR register value
 */
uint32_t XMC_FLASH_GetStatus(void)
{
  return FLASH0->FSR;
}

/*
 * This API enables the events which required to trigger the ISR 
 */
void XMC_FLASH_EnableEvent(const uint32_t event_msk)
{
  FLASH0->FCON |= event_msk;
}

/*
 * This API disables the event generation 
 */
void XMC_FLASH_DisableEvent(const uint32_t event_msk)
{
  FLASH0->FCON &= ~event_msk;
}

/*
 * This API write the PFLASH page
 */
void XMC_FLASH_ProgramPage(uint32_t *address, const uint32_t *data)
{
  uint32_t idx;

  XMC_FLASH_lClearStatusCommand();  
  XMC_FLASH_lEnterPageModeCommand();

  for (idx = 0U; idx < XMC_FLASH_WORDS_PER_PAGE; idx += 2U)
  {
    XMC_FLASH_lLoadPageCommand(data[idx], data[idx + 1U]);
  }

  XMC_FLASH_lWritePageCommand(address);    

  /* wait until the operation is completed */
  while ((FLASH0->FSR & (uint32_t)FLASH_FSR_PBUSY_Msk) != 0U){}
}

/*
 * This API erase the logical sector
 */
void XMC_FLASH_EraseSector(uint32_t *address)
{
  XMC_FLASH_lClearStatusCommand();
  XMC_FLASH_lEraseSectorCommand(address);

  /* wait until the operation is completed */
  while ((FLASH0->FSR & (uint32_t)FLASH_FSR_PBUSY_Msk) != 0U){}
}

/*
 * Command to erase physical sector which is starting with the specified address
 */
void XMC_FLASH_lErasePhysicalSectorCommand(uint32_t *sector_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = sector_start_address;
  *address = 0x40U;
}

/*
 * Command to erase physical sector-4 which is starting with the specified address
 * This command is only available if PROCON1.PRS = 1.
 */
void XMC_FLASH_lRepairPhysicalSectorCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = XMC_FLASH_PHY_SECTOR_4;
  *address = 0x40U;
}

 /* 
  * This API erase the physical sector
  */
void XMC_FLASH_ErasePhysicalSector(uint32_t *address)
{
  XMC_FLASH_lClearStatusCommand();
  XMC_FLASH_lErasePhysicalSectorCommand(address);

  /* wait until the operation is completed */
  while ((FLASH0->FSR & (uint32_t)FLASH_FSR_PBUSY_Msk) != 0U){}
}

/*
 * This API repair the physical sector
 */
void XMC_FLASH_RepairPhysicalSector(void)
{
  XMC_FLASH_lClearStatusCommand();
  XMC_FLASH_lRepairPhysicalSectorCommand();
}

/*
 * Command to erase UCB sector which is starting with the specified address
 */
void XMC_FLASH_EraseUCB(uint32_t *ucb_sector_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = ucb_sector_start_address;
  *address = 0xc0U;

  /* wait until the operation is completed */
  while ((FLASH0->FSR & (uint32_t)FLASH_FSR_PBUSY_Msk) != 0U){}
}

/*
 * Command to reset the status of the PFLASH
 */
void XMC_FLASH_Reset(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xf0U;
}

/*
 * This API install the global read and sector write protection for the specified user
 */
void XMC_FLASH_InstallProtection(uint8_t user,
                                 uint32_t protection_mask,
	                             uint32_t password_0,
                                 uint32_t password_1)
{
  uint32_t idx;
  
  XMC_ASSERT(" XMC_FLASH_ConfigureProtection: User level out of range", (user < 3U))

  XMC_FLASH_lEnterPageModeCommand();

  XMC_FLASH_lLoadPageCommand(protection_mask, 0UL);
  XMC_FLASH_lLoadPageCommand(protection_mask, 0UL);
  XMC_FLASH_lLoadPageCommand(password_0, password_1);
  XMC_FLASH_lLoadPageCommand(password_0, password_1);
  
  for (idx = 0U; idx < (XMC_FLASH_WORDS_PER_PAGE - XMC_FLASH_PROTECTION_CONFIGURATION_WORDS); idx += 2U)
  {
    XMC_FLASH_lLoadPageCommand(0UL, 0UL);
  }

  XMC_FLASH_lWriteUCBPageCommand((uint32_t *)((uint32_t)XMC_FLASH_UCB0 + (user * XMC_FLASH_BYTES_PER_UCB)));

  /* wait until the operation is completed */
  while ((FLASH0->FSR & (uint32_t)FLASH_FSR_PBUSY_Msk) != 0U){}
}

/*
 * This API confirm the protection. So that This sectors are locked with the specified protection.
 */
void XMC_FLASH_ConfirmProtection(uint8_t user)
{
  uint32_t idx;

  XMC_ASSERT(" XMC_FLASH_ConfirmProtection: User level out of range", (user < 3U))

  XMC_FLASH_lEnterPageModeCommand();

  XMC_FLASH_lLoadPageCommand(XMC_FLASH_PROT_CONFIRM_CODE, 0U);
  XMC_FLASH_lLoadPageCommand(XMC_FLASH_PROT_CONFIRM_CODE, 0U);

  /* Fill the rest of page buffer with zeros*/
  for (idx = 0UL; idx < (XMC_FLASH_WORDS_PER_PAGE - XMC_FLASH_PROT_CONFIRM_WORDS); idx += 2U)
  {
    XMC_FLASH_lLoadPageCommand(0UL, 0UL);
  }

  XMC_FLASH_lWriteUCBPageCommand((uint32_t *)((uint32_t)XMC_FLASH_UCB0 + 
                                 (user * XMC_FLASH_BYTES_PER_UCB) + XMC_FLASH_PROT_CONFIRM_OFFSET));

  /* wait until the operation is completed */
  while ((FLASH0->FSR & (uint32_t)FLASH_FSR_PBUSY_Msk) != 0U){}
}

/*
 * This API verify read protection configuration. And returns true if passwords are matching.
 */
bool XMC_FLASH_VerifyReadProtection(uint32_t password_0, uint32_t password_1)
{
  bool status = false;
  
  /* Check if read protection is installed */
  if ((XMC_FLASH_GetStatus() & (uint32_t)XMC_FLASH_STATUS_READ_PROTECTION_INSTALLED) != 0U)
  {  
    XMC_FLASH_lClearStatusCommand();
    XMC_FLASH_lDisableReadProtectionCommand(password_0, password_1);

    status = (bool)(XMC_FLASH_GetStatus() & (uint32_t)XMC_FLASH_STATUS_READ_PROTECTION_DISABLED_STATE);
  }

  return status;
}

/*
 * This API verify sector write protection configuration. And returns true if passwords are matching for the 
 * specified user.
 */
bool XMC_FLASH_VerifyWriteProtection(uint32_t user,
                                     uint32_t protection_mask, 
                                     uint32_t password_0, 
                                     uint32_t password_1)
{
  bool status = false;
  uint32_t *flash_procon_ptr = (uint32_t *)(void*)(&(FLASH0->PROCON0) + user);

  XMC_ASSERT(" XMC_FLASH_VerifyWriteProtection: User level out of range", (user < 2U))

  /* Check if write protection for selected user is installed */
  if ((XMC_FLASH_GetStatus() & (uint32_t)((uint32_t)1U << (uint32_t)((uint32_t)FLASH_FSR_WPROIN0_Pos + user))) != 0U)
  {  
    XMC_FLASH_lClearStatusCommand();
    XMC_FLASH_lDisableSectorWriteProtectionCommand(user, password_0, password_1);
    
    status = (bool)((XMC_FLASH_GetStatus() & (uint32_t)((uint32_t)1U << (uint32_t)((uint32_t)FLASH_FSR_WPRODIS0_Pos + user)))) &&
             (*flash_procon_ptr == (protection_mask & (uint32_t)(~(uint32_t)XMC_FLASH_PROTECTION_READ_GLOBAL)));
  }

  return status;
}

/*
 * Command to enables the protection as it was configured
 */
void XMC_FLASH_ResumeProtection(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x5eU;
}

#endif
