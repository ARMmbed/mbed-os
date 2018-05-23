/**
 * @file xmc4_flash.h
 * @date 2016-03-22
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.14 - XMC Peripheral Driver Library 
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
 * 2015-08-17: 
 *     - Added the below API's to the public interface.
 *       1. XMC_FLASH_Reset
 *       2. XMC_FLASH_ErasePhysicalSector
 *       3. XMC_FLASH_EraseUCB
 *       4. XMC_FLASH_ResumeProtection
 *       5. XMC_FLASH_RepairPhysicalSector 
 *     - Added support for XMC4800/4700 devices
 * 2015-12-07:
 *     - Fix XMC_FLASH_READ_ACCESS_TIME for XMC43, 47 and 48 devices
 * 2016-03-18:
 *     - Fix implementation of XMC_PREFETCH_EnableInstructionBuffer and XMC_PREFETCH_DisableInstructionBuffer
 * 2016-03-22:
 *     - Fix implementation of XMC_PREFETCH_InvalidateInstructionBuffer
 * @endcond 
 *
 */

#ifndef XMC4_FLASH_H
#define XMC4_FLASH_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "xmc_common.h"

#if UC_FAMILY == XMC4

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
#define XMC_FLASH_UNCACHED_BASE    (0x0C000000U) /**< Non cached flash starting address of for
                                                      XMC4 family of microcontrollers */
#define XMC_FLASH_WORDS_PER_PAGE   (64UL)        /**< Number of words in a page (256 bytes / 4 bytes = 64 words)*/
#define XMC_FLASH_BYTES_PER_PAGE   (256UL)       /**< Number of bytes in a  page*/

#define XMC_FLASH_UCB0             (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x0000UL) /**< Starting address of User
                                                                                         Configurable Block 0*/
#define XMC_FLASH_UCB1             (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x0400UL) /**< Starting address of User
                                                                                         Configurable Block 1*/
#define XMC_FLASH_UCB2             (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x0800UL) /**< Starting address of User
                                                                                         Configurable Block 2*/
#define XMC_FLASH_BYTES_PER_UCB    (1024UL) /**< Number of bytes in a user configurable block*/

/**< Note : Total number of Sectors depends on the flash size of the controller. So while using these macros for flash
 *          operations ensure that sector is available, other may lead to flash error.
 */
#define XMC_FLASH_SECTOR_0         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x00000UL) /**<Starting address of sector0 */
#define XMC_FLASH_SECTOR_1         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x04000UL) /**<Starting address of sector1 */
#define XMC_FLASH_SECTOR_2         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x08000UL) /**<Starting address of sector2 */
#define XMC_FLASH_SECTOR_3         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x0C000UL) /**<Starting address of sector3 */
#define XMC_FLASH_SECTOR_4         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x10000UL) /**<Starting address of sector4 */
#define XMC_FLASH_SECTOR_5         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x14000UL) /**<Starting address of sector5 */
#define XMC_FLASH_SECTOR_6         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x18000UL) /**<Starting address of sector6 */
#define XMC_FLASH_SECTOR_7         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x1C000UL) /**<Starting address of sector7 */
#define XMC_FLASH_SECTOR_8         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x20000UL) /**<Starting address of sector8 */
#define XMC_FLASH_SECTOR_9         (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x40000UL) /**<Starting address of sector9 */
#define XMC_FLASH_SECTOR_10        (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x80000UL) /**<Starting address of sector10*/
#define XMC_FLASH_SECTOR_11        (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xC0000UL) /**<Starting address of sector11*/

#define XMC_FLASH_SECTOR_12        (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x100000UL) /**<Starting address of sector12*/
#define XMC_FLASH_SECTOR_13        (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x140000UL) /**<Starting address of sector13*/
#define XMC_FLASH_SECTOR_14        (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x180000UL) /**<Starting address of sector14*/
#define XMC_FLASH_SECTOR_15        (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x1C0000UL) /**<Starting address of sector15*/

#define XMC_FLASH_PHY_SECTOR_0     (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x00000UL) /**<Starting address of non cached
                                                                                         physical sector0 */
#define XMC_FLASH_PHY_SECTOR_4     (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x10000UL) /**<Starting address of non cached
                                                                                         physical sector4 */
#define XMC_FLASH_PHY_SECTOR_8     (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x20000UL) /**<Starting address of non cached
                                                                                         physical sector8 */
#define XMC_FLASH_PHY_SECTOR_9     (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x40000UL) /**<Starting address of non cached
                                                                                         physical sector9 */
#define XMC_FLASH_PHY_SECTOR_10    (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x80000UL) /**<Starting address of non cached
                                                                                         physical sector10 */
#define XMC_FLASH_PHY_SECTOR_11    (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xC0000UL) /**<Starting address of non cached
                                                                                         physical sector11 */
#define XMC_FLASH_PHY_SECTOR_12    (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x100000UL) /**<Starting address of non cached
                                                                                         physical sector12 */
#define XMC_FLASH_PHY_SECTOR_13    (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x140000UL) /**<Starting address of non cached
                                                                                         physical sector13 */
#define XMC_FLASH_PHY_SECTOR_14    (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x180000UL) /**<Starting address of non cached
                                                                                         physical sector14 */
#define XMC_FLASH_PHY_SECTOR_15    (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x1C0000UL) /**<Starting address of non cached
                                                                                         physical sector15 */

#if UC_SERIES == XMC45 || UC_SERIES == XMC43 || UC_SERIES == XMC47 || UC_SERIES == XMC48
#define XMC_FLASH_READ_ACCESS_TIME (22E-9F) /* Flash read access time  */
#else
#define XMC_FLASH_READ_ACCESS_TIME (20E-9F)
#endif

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 *  Defines the return status of the API.
 */
typedef enum XMC_FLASH_STATUS
{
  XMC_FLASH_STATUS_OK                               = 0U,                 /**< Operation completed successfully*/
  XMC_FLASH_STATUS_BUSY                             = FLASH_FSR_PBUSY_Msk, /**< API cannot proceed since FLASH is busy*/
  XMC_FLASH_STATUS_PROGRAMMING_STATE                = FLASH_FSR_PROG_Msk, /**< Write page is in progress or finished */
  XMC_FLASH_STATUS_ERASE_STATE                      = FLASH_FSR_ERASE_Msk, /**< Erase page is in progress or finished */ 
  XMC_FLASH_STATUS_PAGE_MODE                        = FLASH_FSR_PFPAGE_Msk, /**< Flash is in page mode. Assembly buffer 
                                                                                 of PFLASH is in use */
  XMC_FLASH_STATUS_OPERATION_ERROR                  = FLASH_FSR_PFOPER_Msk, /**< Flash Operation aborted */
  XMC_FLASH_STATUS_COMMAND_SEQUENCE_ERROR           = FLASH_FSR_SQER_Msk, /**< Improper address or Invalid state machine 
                                                                               operation */ 
  XMC_FLASH_STATUS_PROTECTION_ERROR                 = FLASH_FSR_PROER_Msk, /**< Flash operation addressing the locked 
                                                                                sector */
  XMC_FLASH_STATUS_SINGLE_BIT_ERROR_AND_CORRECTION  = FLASH_FSR_PFSBER_Msk, /**< Single bit error detected and 
                                                                                 corrected */
  XMC_FLASH_STATUS_DOUBLE_BIT_ERROR                 = FLASH_FSR_PFDBER_Msk, /**< Multiple bit error occurred */
  XMC_FLASH_STATUS_PROTECTION_INSTALLED             = FLASH_FSR_PROIN_Msk, /**< Confirmation of the read or/and write 
                                                                                protection is successful */
  XMC_FLASH_STATUS_READ_PROTECTION_INSTALLED        = FLASH_FSR_RPROIN_Msk, /**< Confirmation of read and global write 
                                                                                 protection is successful for user-0 */
  XMC_FLASH_STATUS_READ_PROTECTION_DISABLED_STATE   = FLASH_FSR_RPRODIS_Msk, /**< Read or/and write protection is  
                                                                                  temporarily disabled */
  XMC_FLASH_STATUS_WRITE_PROTECTION_INSTALLED_UCB0  = FLASH_FSR_WPROIN0_Msk, /**< Sector write protection is installed 
                                                                                  for user-0 */
  XMC_FLASH_STATUS_WRITE_PROTECTION_INSTALLED_UCB1  = FLASH_FSR_WPROIN1_Msk, /**< Sector write protection is installed 
                                                                                  for user-1 */
  XMC_FLASH_STATUS_WRITE_PROTECTION_INSTALLED_UCB2  = FLASH_FSR_WPROIN2_Msk, /**< Sector OTP protection is installed 
                                                                                  for user-2 */
  XMC_FLASH_STATUS_WRITE_PROTECTION_DISABLED_UCB0   = FLASH_FSR_WPRODIS0_Msk, /**< Sector write protection is disabled 
                                                                                  for user-0 */
  XMC_FLASH_STATUS_WRITE_PROTECTION_DISABLED_UCB1   = FLASH_FSR_WPRODIS1_Msk, /**< Sector write protection is disabled 
                                                                                  for user-1 */
  XMC_FLASH_STATUS_SLEEP_MODE                       = FLASH_FSR_SLM_Msk, /**< Indicates flash in sleep mode or not  */
  XMC_FLASH_STATUS_VERIFY_ERROR                     = (int32_t)FLASH_FSR_VER_Msk, /**< Write verification and Erase 
                                                                                       verification error occurred  */
} XMC_FLASH_STATUS_t;

/**
 *  Provides the options to select flash margin read settings of the for quality assurance.
 */
typedef enum XMC_FLASH_MARGIN
{
  XMC_FLASH_MARGIN_DEFAULT = 0x0U << FLASH_MARP_MARGIN_Pos, /**< Default margin */ 
  XMC_FLASH_MARGIN_TIGHT0  = 0x1U << FLASH_MARP_MARGIN_Pos, /**< Sub-optimal 0-bits are read as 1s */
  XMC_FLASH_MARGIN_TIGHT1  = 0x4U << FLASH_MARP_MARGIN_Pos  /**< Sub-optimal 1-bits are read as 0s */
} XMC_FLASH_MARGIN_t;

/**
 *  Provides the options to select flash operational events
 */
typedef enum XMC_FLASH_EVENT
{
  XMC_FLASH_EVENT_VERIFY_AND_OPERATION_ERROR = 0x01U << FLASH_FCON_VOPERM_Pos, /**< Flash verify and operation error 
                                                                                    event */
  XMC_FLASH_EVENT_COMMAND_SEQUENCE_ERROR     = 0x02U << FLASH_FCON_VOPERM_Pos, /**< Flash command sequence error event*/
  XMC_FLASH_EVENT_PROTECTION_ERROR           = 0x04U << FLASH_FCON_VOPERM_Pos, /**< Flash protection error event */
  XMC_FLASH_EVENT_SINGLE_BIT_ERROR           = 0x08U << FLASH_FCON_VOPERM_Pos, /**< Flash single bit error event */ 
  XMC_FLASH_EVENT_DOUBLE_BIT_ERROR           = 0x20U << FLASH_FCON_VOPERM_Pos, /**< Flash double bit error event*/
  XMC_FLASH_EVENT_END_OF_BUSY                = (int32_t)(0x80U << FLASH_FCON_VOPERM_Pos) /**< Flash end of busy event*/
} XMC_FLASH_EVENT_t;

/**
 * Provides the options to select sectors for write protection and select global read protection.
 * The members can be combined using 'OR' operator for multiple selection.<br>
 *
 */
typedef enum XMC_FLASH_PROTECTION
{
  XMC_FLASH_PROTECTION_WRITE_SECTOR_0      = 0x0001UL,  /**< Sector 0 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_1      = 0x0002UL,  /**< Sector 1 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_2      = 0x0004UL,  /**< Sector 3 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_3      = 0x0008UL,  /**< Sector 3 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_4      = 0x0010UL,  /**< Sector 4 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_5      = 0x0020UL,  /**< Sector 5 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_6      = 0x0040UL,  /**< Sector 6 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_7      = 0x0080UL,  /**< Sector 7 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_8      = 0x0100UL,  /**< Sector 8 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTOR_9      = 0x0200UL,  /**< Sector 9 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTORS_10_11 = 0x0400UL,  /**< Sector 10 and 11 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTORS_12_13 = 0x0800UL,  /**< Sector 12 and 13 write protection */
  XMC_FLASH_PROTECTION_WRITE_SECTORS_14_15 = 0x1000UL,  /**< Sector 14 and 15 write protection */
  XMC_FLASH_PROTECTION_READ_GLOBAL         = 0x8000UL   /**< Global read protection (Applicable for UserLevel0 alone)*/
} XMC_FLASH_PROTECTION_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 ********************************************************************************************************************/

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
 * Enables the wait state for error correction.process, It enables one additional wait state for ECC by setting WSECPF
 * bit of FCON register.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_DisableWaitStateForECC()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_EnableWaitStateForECC(void)
{
  FLASH0->FCON |= FLASH_FCON_WSECPF_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the wait state for error correction.\n\n Removes additional wait state for ECC by resetting WSECPF bit of
 * FCON register.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnableWaitStateForECC()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_DisableWaitStateForECC(void)
{
  FLASH0->FCON &= (uint32_t)~FLASH_FCON_WSECPF_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables dynamic idle mode feature to save power.\n\n It switches off the PFLASH read path when no read access is
 * pending. Hence power is saved marginally. This slightly reduces the flash read performance because static
 * pre-fetching is disabled.It sets the FCON register IDLE bit to enable this feature.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_DisableDynamicIdle()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_EnableDynamicIdle(void)
{
  FLASH0->FCON |= FLASH_FCON_IDLE_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables dynamic idle mode feature.\n\n It resets the FCON register IDLE bit to disable this feature. Hence normal
 * flash read operation is selected.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnableDynamicIdle()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_DisableDynamicIdle(void)
{
  FLASH0->FCON &= (uint32_t)~FLASH_FCON_IDLE_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables sleep mode of the PFLASH.\n\n Sleep mode is enabled by setting the bit FCON.SLEEP.
 *
 * \par<b>Note:</b><br>
 * fCPU must be equal or above 1 MHz when wake-up request is triggered.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_DisableSleepRequest()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_EnableSleepRequest(void)
{
  FLASH0->FCON |= (uint32_t)FLASH_FCON_SLEEP_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Wake-up the PFLASH from sleep.\n\n Wakes-up from sleep is done by clearing the bit FCON.SLEEP, if selected via this
 * bit, or wake-up is initiated by releasing the external sleep signal from SCU.
 *
 * \par<b>Note:</b><br>
 * fCPU must be equal or above 1 MHz when wake-up request is triggered.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnableSleepRequest()\n\n\n
 *
 */
__STATIC_INLINE void XMC_FLASH_DisableSleepRequest(void)
{
  FLASH0->FCON &= (uint32_t)~FLASH_FCON_SLEEP_Msk;
}

/**
 *
 * @param margin PFLASH margin selection. Use type @ref XMC_FLASH_MARGIN_t.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets the read margin levels for checking the healthiness of flash data.\n\n Configures the margin field of MARP
 * MARP register with the specified \a margin level. It changes the margin levels for read operations to find
 * problematic array bits.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_FLASH_SetMargin(const XMC_FLASH_MARGIN_t margin)
{
  FLASH0->MARP = (FLASH0->MARP & (uint32_t)~FLASH_MARP_MARGIN_Msk) | margin;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables double bit error trap.\n\n. It enables by setting MARP register bit TRAPDIS.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_FLASH_EnableDoubleBitErrorTrap(void)
{
  FLASH0->MARP &= (uint32_t)~FLASH_MARP_TRAPDIS_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the trap generation for double bit error by clearing MARP register bit TRAPDIS.\n\n The double-bit error
 * trap can be disabled for margin checks and also redirected to an error interrupt. Any time during the execution the
 * double bit error trap can be enabled back by calling XMC_FLASH_EnableDoubleBitErrorTrap() API.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_FLASH_DisableDoubleBitErrorTrap(void)
{
  FLASH0->MARP |= FLASH_MARP_TRAPDIS_Msk;
}

/**
 *
 * @param num_wait_states number of wait states for initial read access<BR> Range:  [0 to 15]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the number of wait states for initial flash read access.\n\n Depending on the configured \a
 * num_wait_states value into FCON resister \a WSPFLASH field, the read performance gets optimized . The wait cycles
 * for the flash read access must be configured based on the CPU frequency (fCPU), in relation to the flash access
 * time (\a ta) defined. The access time formula  (\a WSPFLASH x (\a \a \a 1 / fCPU) \a >= \a ta) applies only for
 * the values \a \a \a num_wait_states >0.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_FLASH_SetWaitStates(uint32_t num_wait_states)
{
  FLASH0->FCON = (FLASH0->FCON & (uint32_t)~FLASH_FCON_WSPFLASH_Msk) |
                 (num_wait_states << FLASH_FCON_WSPFLASH_Pos);
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the cacheable accesses to use the instruction buffer by resetting the register bit PREF_PCON.IBYP.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_PREFETCH_EnableInstructionBuffer(void)
{
	PREF->PCON &= (uint32_t)~PREF_PCON_IBYP_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Bypasses the instruction buffer for cacheable accesses, by setting the register bit PREF_PCON.IBYP.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_PREFETCH_DisableInstructionBuffer(void)
{
	PREF->PCON |= PREF_PCON_IBYP_Msk;
}

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Invalidates the instruction buffer by setting PREF_PCON register bit IINV.\n\n After system reset, the instruction
 * buffer is automatically invalidated.
 *
 * \par<b>Note:</b><br>
 * The complete invalidation operation is performed in a single cycle.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void XMC_PREFETCH_InvalidateInstructionBuffer(void)
{
  PREF->PCON |= PREF_PCON_IINV_Msk;
  __DSB();
  __ISB();

  PREF->PCON &= ~PREF_PCON_IINV_Msk;
  __DSB();
  __ISB();

}

/**
 *
 * @param user ID number of the user configuration block (UCB).<BR> Range:  [0 to 2]
 *
 * @param protection_mask ORed values of @ref XMC_FLASH_PROTECTION_t enum type, for which sectors the protection
 *                        has to be enabled.
 * @param password_0 First password for protection.<BR> Range:  [0 to 4294967295]
 * @param password_1 Second password for protection.<BR> Range:  [0 to 4294967295]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Installs the global read and sector write protection.\n\n The installation starts by issuing the page mode entry
 * command followed by the load page command. The load page command mode loads the required sectors intended for
 * protection specified in \a protection_mask. It also loads the specified passwords \a password0 and \a password1
 * respectively. Finally, it issues the write page command for the specified \a user configuration block. Calling
 * XMC_FLASH_ConfirmProtection() after this API completes the protection process by freezing the sectors forever.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_ConfirmProtection()<BR>
 * XMC_FLASH_VerifyReadProtection()<BR>
 */
void XMC_FLASH_InstallProtection(uint8_t user,
                                 uint32_t protection_mask,
                                 uint32_t password_0, 
                                 uint32_t password_1);

/**
 *
 * @param user ID number of the user configuration block (UCB).<BR> Range:  [0 to 2]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Confirms the protection, so that sectors specified under \a user configurable block are locked forever.\n\n The
 * protection shall be installed by calling XMC_FLASH_InstallProtection() before calling this API.
 * The confirmation starts by issuing the page mode entry command followed by the load page command. The load page
 * command issues the confirmation protection command for the sectors on which the protection installation was done.
 * It also loads the specified passwords \a password0 and \a password1 respectively. Finally, it issues the confirm
 * protection command for the specified \a user configuration block so that the sectors will be protected forever.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_InstallProtection()<BR>
 * 
 */
void XMC_FLASH_ConfirmProtection(uint8_t user);

/**
 *
 * @param password_0 First password used for protection.<BR> Range:  [0 to 4294967295]
 * @param password_1 Second password used for protection.<BR> Range:  [0 to 4294967295]
 *
 * @return true if read protection installed properly else returns \a false.
 *
 * \par<b>Description:</b><br>
 * Verifies sector read protection is properly installed or not.\n\n Before entering into verify read protection
 * process, it clears the error status bits inside status register. It temporarily disables the protection with
 * passwords \a password0 and \a password1 respectively. It reads the FSR register and verifies the protection state.
 * Resumption of read protection after disablement is achieved by XMC_FLASH_ResumeProtection or until next reset.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_InstallProtection()<BR> 
 * XMC_FLASH_VerifyWriteProtection()<BR>
 * XMC_FLASH_ResumeProtection()<BR>
 */
bool XMC_FLASH_VerifyReadProtection(uint32_t password_0, uint32_t password_1);

/**
 *
 * @param user ID number of the user configuration block (UCB).<BR> Range:  [0 to 2]
 *
 * @param protection_mask ORed values of @ref XMC_FLASH_PROTECTION_t enum type, for which sectors the protection
 *                        has to be verified.
 * @param password_0 First password used for protection.<BR> Range:  [0 to 4294967295]
 * @param password_1 Second password used for protection.<BR> Range:  [0 to 4294967295]
 *
 * @return true if write protection installed properly else returns \a false.
 *
 * \par<b>Description:</b><br>
 * Verifies sector read protection is properly installed or not.\n\n Before entering into verify read protection
 * process, it clears the error status bits inside status register. It temporarily disables the protection with
 * passwords \a password0 and \a password1 respectively for the intended sectors specified in \a protection_mask.
 * It reads the FSR register and verifies the write protection state.
 * Resumption of write protection after disablement is achieved by XMC_FLASH_ResumeProtection or until next reset.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_InstallProtection()<BR> 
 * XMC_FLASH_VerifyReadProtection()<BR>
 * XMC_FLASH_ResumeProtection()<BR>
 */
bool XMC_FLASH_VerifyWriteProtection(uint32_t user, 
                                     uint32_t protection_mask, 
                                     uint32_t password_0, 
                                     uint32_t password_1);

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Resumes flash protection as it was configured before.\n\n
 * It clears all the disable proection status flags FSR.WPRODISx and FSR.RPRODIS. But FSR.WPRODISx is not 
 * cleared when corresponding UCBx is not in the “confirmed” state. 
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void XMC_FLASH_ResumeProtection(void);

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Repairs the physical sector "PS4".\n\n 
 * For selected devices, Erase Physical Sector can also be used for Sector Soft Repair, depending on the configuration 
 * of PROCON1.PSR. This command sequence is required to run an EEPROM emulation algorithm that cycles the logical  
 * sectors S4..S7 of PS4. This command sequence repairs the corrupted logical sectors inside the physical sector due to 
 * interrupted erase operation.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void XMC_FLASH_RepairPhysicalSector(void);
/**
 *
 * @param sector_start_address Pointer to the starting address of physical sector. Use XMC_FLASH_SECTOR_x MACRO defined
 *                             in xmc4_flash.h file.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Erases the physical sector "PSA".\n\n If "PSA" does not point to base address of a correct sector or an unavailable 
 * sector, it returns SQER. 
 *
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void XMC_FLASH_ErasePhysicalSector(uint32_t *sector_start_address);

/**
 *
 * @param ucb_sector_start_address Pointer to the starting address of physical sector. Use XMC_FLASH_UCBx MACRO 
 *                                 defined in xmc4_flash.h file.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * The addressed user configuration block “UCB” is erased.\n\n
 * Erases UCB whose startting address specified in the input parameter \a ucb_sector_start_address. When the UCB has 
 * an active write protection or the Flash module has an active global read protection the execution fails and 
 * PROER is set. The command fails with SQER when \a ucb_sector_start_address is not the start address of a valid UCB.
 * Call \ref XMC_FLASH_GetStatus API after this API to verify the erase was proper ot not. 
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void XMC_FLASH_EraseUCB(uint32_t *ucb_sector_start_address);

/**
 *
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Resets the command interpreter to its initial state.\n\n
 * Reset to Read can cancel every command sequence before its last command cycle has been received. All error flags 
 * gets cleared by calling this API. 
 * \par<b>Note:</b><br>
 * todo
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void XMC_FLASH_Reset(void);



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

#endif

