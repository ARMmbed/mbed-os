/**
 * @file xmc1_flash.c
 * @date 2015-10-14
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
 * 2015-10-14: 
 *     - Fixed defect in API XMC_FLASH_ErasePages, related to the errata NVM_CM.001
 *     - NVM ROM user routine XMC1000_NvmErasePage(address) used for erase page. 
 *
 * @endcond 
 *
 */

#include "xmc_flash.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#if UC_FAMILY == XMC1

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/* FLASH programming / erase options */
typedef enum FLASH_ACTION
{
  FLASH_ACTION_IDLE                        =  (uint32_t)0x00,
  FLASH_ACTION_ONESHOT_WRITE_VERIFY        = ((uint32_t)0x51 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_ONESHOT_WRITE               = ((uint32_t)0x91 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_WRITE_VERIFY     = ((uint32_t)0x61 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_WRITE            = ((uint32_t)0xa1 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_ONESHOT_PAGE_ERASE          = ((uint32_t)0x92 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_PAGE_ERASE       = ((uint32_t)0xa2 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_ONESHOT_VERIFY_ONLY         = ((uint32_t)0xd0 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_VERIFY_ONLY      = ((uint32_t)0xe0 << NVM_NVMPROG_ACTION_Pos)
} FLASH_ACTION_t;


/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/*
 * This API shall clear the ECC and VERIFICATION error status.
 */
void XMC_FLASH_ClearStatus(void)
{
  NVM->NVMPROG |= (uint16_t)((uint16_t)NVM_NVMPROG_RSTVERR_Msk | (uint16_t)NVM_NVMPROG_RSTECC_Msk);
}

/*
 * This API shall return the status of NVM.
 */
uint32_t XMC_FLASH_GetStatus(void)
{
  return NVM->NVMSTATUS;
}

/*
 * This API shall enable the the flash interrupt event.
 */
void XMC_FLASH_EnableEvent(const uint32_t event_msk)
{
  NVM->NVMCONF |= (uint16_t)event_msk;
}

/*
 * This API shall disable the the flash interrupt event.
 */
void XMC_FLASH_DisableEvent(const uint32_t event_msk)
{
  NVM->NVMCONF &= (uint16_t)(~(uint16_t)event_msk);
}

void XMC_FLASH_ErasePage(uint32_t *address)
{
  (void)XMC1000_NvmErasePage(address);
}

void XMC_FLASH_ProgramVerifyPage(uint32_t *address, const uint32_t *data)
{
  (void)XMC1000_NvmProgVerify(data, address);
}

/* Write blocks of data into flash*/
void XMC_FLASH_WriteBlocks(uint32_t *address, const uint32_t *data, uint32_t num_blocks, bool verify)
{
  uint32_t word;
  uint32_t block;

  XMC_ASSERT("XMC_FLASH_WriteBlocks: Starting address not aligned to Block",
                                                                   ((uint32_t)address & FLASH_BLOCK_ADDR_MASK) == 0U)

  /* Configure the continuous Write option command and reset the NVM error / verification status*/
  NVM->NVMPROG &= (uint16_t)(~(uint16_t)NVM_NVMPROG_ACTION_Msk);
  NVM->NVMPROG |= (uint16_t)(NVM_NVMPROG_RSTVERR_Msk | NVM_NVMPROG_RSTECC_Msk);

  if (verify == true)
  {
    NVM->NVMPROG |= (uint16_t)FLASH_ACTION_CONTINUOUS_WRITE_VERIFY;
  }
  else
  {
    NVM->NVMPROG |= (uint16_t)FLASH_ACTION_CONTINUOUS_WRITE;
  }

  for (block = 0U; block < num_blocks; ++block)
  {
    for (word = 0U; word < XMC_FLASH_WORDS_PER_BLOCK; ++word)
    {
      *address = *data;
      data++;
      address++;
    }
      
    while (XMC_FLASH_IsBusy() == true)
    {
    }
  }

  /* Stop continuous write operation */
  NVM->NVMPROG &= (uint16_t)(~(uint16_t)NVM_NVMPROG_ACTION_Msk);
}

/* Erase flash pages */
void XMC_FLASH_ErasePages(uint32_t *address, uint32_t num_pages)
{
  uint32_t page;

  XMC_ASSERT("XMC_FLASH_ErasePages: Starting address not aligned to Page",
                                                                    ((uint32_t)address & FLASH_PAGE_ADDR_MASK) == 0U)

  for (page = 0U; page < num_pages; ++page)
  {
    (void)XMC1000_NvmErasePage(address);

    while (XMC_FLASH_IsBusy() == true)
    {
    }

    /* Increment the page address for the next erase */
    address += XMC_FLASH_WORDS_PER_PAGE;

  }

}

/* Write multiple data blocks and verify the written data */
void XMC_FLASH_VerifyBlocks(uint32_t *address, const uint32_t *data, uint32_t num_blocks)
{
  uint32_t word;
  uint32_t block;

  XMC_ASSERT("XMC_FLASH_VerifyBlocks: Starting address not aligned to Block",
                                                                   ((uint32_t)address & FLASH_BLOCK_ADDR_MASK) == 0U)

  /* Configure the Continuous write with verify option command and reset the NVM error / verification status*/
  NVM->NVMPROG &= (uint16_t)~NVM_NVMPROG_ACTION_Msk;
  NVM->NVMPROG |= (uint16_t)((uint16_t)NVM_NVMPROG_RSTVERR_Msk | 
                             (uint16_t)NVM_NVMPROG_RSTECC_Msk | 
                             (uint16_t)FLASH_ACTION_CONTINUOUS_VERIFY_ONLY);

  for (block = 0U; block < num_blocks; ++block)
  {
    for (word = 0U; word < XMC_FLASH_WORDS_PER_BLOCK; ++word)
    {
      *address = *data;
      data++;
      address++;
    }

    while (XMC_FLASH_IsBusy() == true)
    {
    }
  }

  /* Stop continuous verify operation */
  NVM->NVMPROG &= (uint16_t)(~(uint16_t)NVM_NVMPROG_ACTION_Msk);
}

/* Read data blocks from flash */
void XMC_FLASH_ReadBlocks(uint32_t *address, uint32_t *data, uint32_t num_blocks)
{
  uint32_t word;
  uint32_t block;

  XMC_ASSERT("XMC_FLASH_ReadBlocks: Starting address not aligned to Block",
                                                                  ((uint32_t)address & FLASH_BLOCK_ADDR_MASK) == 0U)

  for (block = 0U; block < num_blocks; ++block)
  {
    for (word = 0U; word < XMC_FLASH_WORDS_PER_BLOCK; ++word)
    {
      *data = *address;
      data++;
      address++;
    }
  }
}

/* Erase single sector */
void XMC_FLASH_EraseSector(uint32_t *address)
{
  XMC_ASSERT("XMC_FLASH_EraseSector: Starting address not aligned to Sector",
                                                                 ((uint32_t)address & FLASH_SECTOR_ADDR_MASK) == 0U)
  XMC_FLASH_ErasePages(address, XMC_FLASH_PAGES_PER_SECTOR);
}

/* Program single page */
void XMC_FLASH_ProgramPage(uint32_t *address, const uint32_t *data)
{
  XMC_FLASH_ProgramVerifyPage(address, data);
}

#endif
