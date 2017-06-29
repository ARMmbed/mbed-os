/*!
 *****************************************************************************
 * @file:    system_ADuCM4050.h
 * @brief:   CMSIS Cortex-M4 Device Peripheral Access Layer Header File
 *           for ADuCM4050
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note  Modified September 21 2016 Analog Devices
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
   ---------------------------------------------------------------------------*/


#ifndef SYSTEM_ADUCM4050_H
#define SYSTEM_ADUCM4050_H

#include <stddef.h>     /* for 'NULL' */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern uint32_t SystemCoreClock;     /* System Clock Frequency (Core Clock)  */

#if defined (__ICCARM__)
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): Types which specify sign and size should be used
*   We use bool which is accepted by MISRA but the toolchain does not accept it
*
*/
#pragma diag_suppress=Pm011

#endif

/*! \addtogroup SYS_Driver System Interfaces
 *  @{
 * System global interfaces and CMSIS required variables
 */

/*! System API function return codes */
typedef enum
{
    ADI_SYS_SUCCESS = 0,  /*!< No error detected. */
    ADI_SYS_FAILURE,      /*!< The API call failed. */
} ADI_SYS_RESULT;


/*! \cond PRIVATE */
/* System clock constant */
#define __HFOSC     26000000u

/* System clock constant (may also be 16000000) */
#define __HFXTAL    26000000u

 /*System clock constant (same whether internal osc or external xtal) */
#define __LFCLK        32768u

 /*Selecting HFOSC as input for generating root clock*/
#define HFMUX_INTERNAL_OSC_VAL      (0u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting HFXTAL as input for generating root clock*/
#define HFMUX_EXTERNAL_XTAL_VAL     (1u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting SPLL as input for generating root clock*/
#define HFMUX_SYSTEM_SPLL_VAL       (2u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting GPIO as input for generating root clock*/
#define HFMUX_GPIO_VAL              (3u << BITP_CLKG_CLK_CTL0_CLKMUX)

/*! Cache controller key */
#define CACHE_CONTROLLER_KEY 0xF123F456u
/*! Power  key */
#define PWRKEY_VALUE_KEY 0x4859u

/**
 * Security options
 */
typedef struct {
        const uint32_t ReadProtectKeyHash[4];
        const uint32_t CrcOfReadProtectKeyHash;
        const uint32_t LastCRCPage;
        const uint32_t InCircuitWriteProtectCode;
        const uint32_t FlashBlockWriteProtect;

} ADI_ADUCM4X50_SECURITY_OPTIONS;

/*! \endcond*/

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
ADI_SYS_RESULT adi_system_EnableRetention(ADI_SRAM_BANK eBank, bool bEnable);
void adi_system_EnableISRAM(bool bEnable);

#if defined (__ICCARM__)
#pragma diag_default=Pm011
#endif

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_ADUCM4050_H */

/*@}*/
/*
** EOF
*/
