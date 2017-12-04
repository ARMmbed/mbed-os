/*!
 *****************************************************************************
 * @file:    system_ADuCM3029.h
 * @brief:   CMSIS Cortex-M3 Device Peripheral Access Layer Header File
 *           for the ADI ADuCxxx Device Series
 * @version: $Revision: 36134 $
 * @date:    $Date: 2017-01-12 05:13:23 -0500 (Thu, 12 Jan 2017) $
 *-----------------------------------------------------------------------------
 *
 * Copyright (C) 2009-2013 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *****************************************************************************/


/*! \addtogroup SYS_Driver System Interfaces
 *  @{
 * add result types to doxygen
 */

#ifndef SYSTEM_ADUCM3029_H
#define SYSTEM_ADUCM3029_H

#include <stddef.h>     /* for 'NULL' */
#include <stdint.h>
#include <stdbool.h>
#include <adi_processor.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! \cond PRIVATE */
#define SUCCESS 0u

#define FAILURE 1u

/* System clock constant */
#define __HFOSC     26000000u  

/* System clock constant (may also be 16000000) */
#define __HFXTAL    26000000u  

/*System clock constant (same whether internal osc or external xtal) */
#define __LFCLK        32768u 

/* Selecting HFOSC as input for generating root clock*/
#define HFMUX_INTERNAL_OSC_VAL      (0u << BITP_CLKG_CLK_CTL0_CLKMUX)

/* Selecting HFXTL as input for generating root clock*/
#define HFMUX_EXTERNAL_XTAL_VAL     (1u << BITP_CLKG_CLK_CTL0_CLKMUX)

/* Selecting SPLL as input for generating root clock*/
#define HFMUX_SYSTEM_SPLL_VAL       (2u << BITP_CLKG_CLK_CTL0_CLKMUX)

/* Selecting GPIO as input for generating root clock*/
#define HFMUX_GPIO_VAL              (3u << BITP_CLKG_CLK_CTL0_CLKMUX)

/*
 * Security options
 */
typedef struct {
        const uint32_t ReadProtectKeyHash[4];    
        const uint32_t CrcOfReadProtectKeyHash;  
        const uint32_t LastCRCPage;              
        const uint32_t InCircuitWriteProtectCode;
        const uint32_t FlashBlockWriteProtect;   

} ADI_ADUCM302X_SECURITY_OPTIONS;

/*! \endcond  */

/*! Cache controller key */
#define CACHE_CONTROLLER_KEY 0xF123F456u
/*! Power  key */
#define PWRKEY_VALUE_KEY 0x4859u

  
/**
 *  SRAM banks
 */
typedef uint32_t ADI_SRAM_BANK;

/*! SRAM_BANK_0 */
#define ADI_SRAM_BANK_0  (1u << 0)
/*! SRAM_BANK_1 */
#define ADI_SRAM_BANK_1  (1u << 1)
/*! SRAM_BANK_2 */
#define ADI_SRAM_BANK_2  (1u << 2)
/*! SRAM_BANK_3 */
#define ADI_SRAM_BANK_3  (1u << 3)
/*! SRAM_BANK_4 */
#define ADI_SRAM_BANK_4  (1u << 4)
/*! SRAM_BANK_5 */
#define ADI_SRAM_BANK_5  (1u << 5)
/*! SRAM_BANK_6 */
#define ADI_SRAM_BANK_6  (1u << 6)
/*! SRAM_BANK_7 */
#define ADI_SRAM_BANK_7  (1u << 7)

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
void adi_system_EnableCache(bool bEnable);
uint32_t  adi_system_EnableRetention(ADI_SRAM_BANK eBank, bool bEnable);
void adi_system_EnableISRAM(bool bEnable);
extern uint32_t SystemCoreClock;

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_ADUCM3029_H */

/**@}*/

/*
** EOF
*/
