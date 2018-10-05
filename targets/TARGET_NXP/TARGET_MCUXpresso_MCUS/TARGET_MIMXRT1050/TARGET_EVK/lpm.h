/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LPM_H_
#define _LPM_H_

#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern void vPortGPTIsr(void);

#define vPortGptIsr GPT1_IRQHandler

#define CLOCK_SET_MUX(mux, value)                                                                        \
    \
do                                                                                                \
    {                                                                                                    \
        CCM_TUPLE_REG(CCM, mux) = (CCM_TUPLE_REG(CCM, mux) & (~CCM_TUPLE_MASK(mux))) |                   \
                                  (((uint32_t)((value) << CCM_TUPLE_SHIFT(mux))) & CCM_TUPLE_MASK(mux)); \
        while (CCM->CDHIPR != 0)                                                                         \
        {                                                                                                \
        }                                                                                                \
    \
}                                                                                                 \
    while (0)

#define CLOCK_SET_DIV(divider, value)                                                                                \
    \
do                                                                                                            \
    {                                                                                                                \
        CCM_TUPLE_REG(CCM, divider) = (CCM_TUPLE_REG(CCM, divider) & (~CCM_TUPLE_MASK(divider))) |                   \
                                      (((uint32_t)((value) << CCM_TUPLE_SHIFT(divider))) & CCM_TUPLE_MASK(divider)); \
        while (CCM->CDHIPR != 0)                                                                                     \
        {                                                                                                            \
        }                                                                                                            \
    \
}                                                                                                             \
    while (0)

#define CLOCK_CCM_HANDSHAKE_WAIT() \
    \
do                          \
    {                              \
        while (CCM->CDHIPR != 0)   \
        {                          \
        }                          \
    \
}                           \
    while (0)

#define LPM_DELAY(value)                         \
    \
do                                        \
    {                                            \
        for (uint32_t i = 0; i < 5 * value; i++) \
        {                                        \
            __NOP();                             \
        }                                        \
    \
}                                         \
    while (0)

#define ROM_CODE_ENTRY_ADDR (0x200000U)

/*! @name Time sensitive region */
/* @{ */
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#if (defined(__ICCARM__))
#define AT_QUICKACCESS_SECTION_CODE(func) __ramfunc func
#elif(defined(__ARMCC_VERSION))
#define AT_QUICKACCESS_SECTION_CODE(func) __attribute__((section("RamFunction"))) func
#elif defined(__MCUXPRESSO)
#define AT_QUICKACCESS_SECTION_CODE(func) __attribute__((section(".ramfunc.$SRAM_ITC"))) func
#elif(defined(__GNUC__))
#define AT_QUICKACCESS_SECTION_CODE(func) __attribute__((section("RamFunction"))) func
#else
#error Toolchain not supported.
#endif /* defined(__ICCARM__) */
#else
#if (defined(__ICCARM__))
#define AT_QUICKACCESS_SECTION_CODE(func) func
#elif(defined(__ARMCC_VERSION))
#define AT_QUICKACCESS_SECTION_CODE(func) func
#elif(defined(__MCUXPRESSO))
#define AT_QUICKACCESS_SECTION_CODE(func) func
#elif(defined(__GNUC__))
#define AT_QUICKACCESS_SECTION_CODE(func) func
#else
#error Toolchain not supported.
#endif
#endif /* __FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

AT_QUICKACCESS_SECTION_CODE(void LPM_SwitchFlexspiClock(clock_mode_t powermode));
AT_QUICKACCESS_SECTION_CODE(void LPM_RestoreFlexspiClock(void));

/* Initialize the Low Power Management */
bool LPM_Init(void);

/* Deinitialize the Low Power Management */
void LPM_Deinit(void);

/* Enable wakeup source in low power mode */
void LPM_EnableWakeupSource(uint32_t irq);

/* Disable wakeup source in low power mode */
void LPM_DisableWakeupSource(uint32_t irq);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
