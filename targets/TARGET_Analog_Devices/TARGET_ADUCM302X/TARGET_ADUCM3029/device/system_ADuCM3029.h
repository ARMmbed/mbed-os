/**************************************************************************//**
 * @file     system_ADuCM3029.h
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Header File for
 *           Device ADuCM3029
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note     Modified 19. September 2016 Analog Devices
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

   Portions Copyright (c) 2016 Analog Devices, Inc.
   ---------------------------------------------------------------------------*/

/*! \addtogroup SYS_Driver System Interfaces
 *  @{
 * add result types to doxygen
 */

#ifndef SYSTEM_ADUCM3029_H
#define SYSTEM_ADUCM3029_H

#include <stddef.h>     /* for 'NULL' */
#include <adi_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SUCCESS 0u

#define FAILURE 1u

/* System clock constant */
#define __HFOSC     26000000u  

/* System clock constant (may also be 16000000) */
#define __HFXTAL    26000000u  

 /* System clock constant (same whether internal osc or external xtal) */
#define __LFCLK        32768u 

 /* Selecting HFOSC as input for generating root clock*/
#define HFMUX_INTERNAL_OSC_VAL      (0u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /* Selecting HFXTL as input for generating root clock*/
#define HFMUX_EXTERNAL_XTAL_VAL     (1u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /* Selecting SPLL as input for generating root clock*/
#define HFMUX_SYSTEM_SPLL_VAL       (2u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /* Selecting GPIO as input for generating root clock*/
#define HFMUX_GPIO_VAL              (3u << BITP_CLKG_CLK_CTL0_CLKMUX)

/*! Cache controller key */
#define CACHE_CONTROLLER_KEY 0xF123F456u

/*! Power  key */
#define PWRKEY_VALUE_KEY 0x4859u

/**
 * Enumeration of different channel of the SPORT
 */
typedef enum
{
    /*! SRAM_BANK_0 */
    ADI_SRAM_BANK_0      = 1,
    /*! SRAM_BANK_1 */
    ADI_SRAM_BANK_1      = 2,
    /*! SRAM_BANK_2 */
    ADI_SRAM_BANK_2      = 4,
    /*! SRAM_BANK_3 */
    ADI_SRAM_BANK_3      = 8,
    /*! SRAM_BANK_4 */
    ADI_SRAM_BANK_4      = 16,
    /*! SRAM_BANK_5 */
    ADI_SRAM_BANK_5      = 32

} ADI_SRAM_BANK;

/**
 * Security options
 */
typedef struct {
        const uint32_t ReadProtectKeyHash[4];
        const uint32_t CrcOfReadProtectKeyHash;
        const uint32_t LastCRCPage;
        const uint32_t InCircuitWriteProtectCode;
        const uint32_t FlashBlockWriteProtect;

} ADI_ADUCM302X_SECURITY_OPTIONS;

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

/**
 * @brief Enables or disables the cache
 *
 * @param  bEnable : To specify whether to enable/disable cache.
 *               true : To enable cache.
 *               false : To disable cache.
 *
 * @return none
 *
 */
void adi_system_EnableCache (bool_t bEnable);

/**
 * @brief  Enables or disables instruction SRAM
 *
 * @param bEnable: To enable/disable the instruction SRAM.
 *              true : To enable cache.
 *              false : To disable cache.
 *
 * @return none
 * @note:  Please note that respective linker file needs to support the configuration.
 */
void adi_system_EnableISRAM (bool_t bEnable);

/**
 * @brief  Enables/disable SRAM retention during the hibernation.
 *
 * @param eBank:   Specify which SRAM bank. Only BANK1 and BANK2 are valid.
 * @param bEnable: To enable/disable the  retention for specified  SRAM bank.
 *              true : To enable retention during the hibernation.
 *              false :To disable retention during the hibernation.
 *
 * @return : SUCCESS : Configured successfully.
 *           FAILURE :  For invalid bank.
 *
 * @note: Please note that respective linker file needs to support the configuration. Only BANK-1 and
 *        BANK-2 of SRAM is valid.
 */
uint32_t adi_system_EnableRetention (ADI_SRAM_BANK eBank, bool_t bEnable);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_ADUCM3029_H */

/**@}*/
