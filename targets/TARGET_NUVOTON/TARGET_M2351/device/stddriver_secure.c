/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arm_cmse.h>
#include <stdbool.h>
#include "cmsis.h"
#include "partition_M2351.h"
#include "stddriver_secure.h"
#include "mbed_error.h"

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

typedef struct nu_modidx_ns_s {
    uint32_t    sys_modidx;     // Module index defined in SYS
    uint32_t    clk_modidx;     // Module index defined in CLK
    uint32_t    ns;             // 0: secure or undefined, 1: non-secure
} nu_modidx_ns_t;

/* Placeholder for module index not defined
 *
 * Not all modules have corresponding SYS/CLK module index.
 * Check sys.h/clk.h under StdDriver for non-used value to mean 'NONE'.
 */
#define NU_SYS_MODIDX_UNDEF     0xFFFFFFFFUL
#define NU_CLK_MODIDX_UNDEF     0xFFFFFFFFUL

/* Table which records security state of module based on partition file.
 *
 * Some secure modules are not listed here for saving memory:
 * 1. Modules are hard-wired to secure.
 * 2. Modules belonging to the same PNSSET register are all secure.
 *
 * check_mod_ns must filter out 'NONE' module index to avoid security issue.
 */
static const nu_modidx_ns_t modidx_ns_tab[] = {

#if defined(SCU_INIT_PNSSET0_VAL) && SCU_INIT_PNSSET0_VAL
    {USBH_RST,              USBH_MODULE,            SCU_INIT_PNSSET0_VAL & (1 << 9)},
    {SDH0_RST,              SDH0_MODULE,            SCU_INIT_PNSSET0_VAL & (1 << 13)},
    {NU_SYS_MODIDX_UNDEF,   NU_CLK_MODIDX_UNDEF,    SCU_INIT_PNSSET0_VAL & (1 << 14)},
    {EBI_RST,               EBI_MODULE,             SCU_INIT_PNSSET0_VAL & (1 << 16)},
    {PDMA1_RST,             PDMA1_MODULE,           SCU_INIT_PNSSET0_VAL & (1 << 24)},
#endif

#if defined(SCU_INIT_PNSSET1_VAL) && SCU_INIT_PNSSET1_VAL
    {CRC_RST,               CRC_MODULE,             SCU_INIT_PNSSET1_VAL & (1 << 17)},
    {CRPT_RST,              CRPT_MODULE,            SCU_INIT_PNSSET1_VAL & (1 << 18)},
#endif

#if defined(SCU_INIT_PNSSET2_VAL) && SCU_INIT_PNSSET2_VAL
    {NU_SYS_MODIDX_UNDEF,   RTC_MODULE,             SCU_INIT_PNSSET2_VAL & (1 << 1)},
    {EADC_RST,              EADC_MODULE,            SCU_INIT_PNSSET2_VAL & (1 << 3)},
    {ACMP01_RST,            ACMP01_MODULE,          SCU_INIT_PNSSET2_VAL & (1 << 5)},
    {NU_SYS_MODIDX_UNDEF,   NU_CLK_MODIDX_UNDEF,    SCU_INIT_PNSSET2_VAL & (1 << 6)},
    {DAC_RST,               DAC_MODULE,             SCU_INIT_PNSSET2_VAL & (1 << 7)},
    {I2S0_RST,              I2S0_MODULE,            SCU_INIT_PNSSET2_VAL & (1 << 8)},
    {OTG_RST,               OTG_MODULE,             SCU_INIT_PNSSET2_VAL & (1 << 13)},
    {TMR2_RST,              TMR2_MODULE,            SCU_INIT_PNSSET2_VAL & (1 << 17)},
    {TMR3_RST,              TMR3_MODULE,            SCU_INIT_PNSSET2_VAL & (1 << 17)},
    {EPWM0_RST,             EPWM0_MODULE,           SCU_INIT_PNSSET2_VAL & (1 << 24)},
    {EPWM1_RST,             EPWM1_MODULE,           SCU_INIT_PNSSET2_VAL & (1 << 25)},
    {BPWM0_RST,             BPWM0_MODULE,           SCU_INIT_PNSSET2_VAL & (1 << 26)},
    {BPWM1_RST,             BPWM1_MODULE,           SCU_INIT_PNSSET2_VAL & (1 << 27)},
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && SCU_INIT_PNSSET3_VAL
    {SPI0_RST,              SPI0_MODULE,            SCU_INIT_PNSSET3_VAL & (1 << 0)},
    {SPI1_RST,              SPI1_MODULE,            SCU_INIT_PNSSET3_VAL & (1 << 1)},
    {SPI2_RST,              SPI2_MODULE,            SCU_INIT_PNSSET3_VAL & (1 << 2)},
    {SPI3_RST,              SPI3_MODULE,            SCU_INIT_PNSSET3_VAL & (1 << 3)},
    {NU_SYS_MODIDX_UNDEF,   NU_CLK_MODIDX_UNDEF,    SCU_INIT_PNSSET3_VAL & (1 << 4)},
    {SPI5_RST,              SPI5_MODULE,            SCU_INIT_PNSSET3_VAL & (1 << 5)},
    {UART0_RST,             UART0_MODULE,           SCU_INIT_PNSSET3_VAL & (1 << 16)},
    {UART1_RST,             UART1_MODULE,           SCU_INIT_PNSSET3_VAL & (1 << 17)},
    {UART2_RST,             UART2_MODULE,           SCU_INIT_PNSSET3_VAL & (1 << 18)},
    {UART3_RST,             UART3_MODULE,           SCU_INIT_PNSSET3_VAL & (1 << 19)},
    {UART4_RST,             UART4_MODULE,           SCU_INIT_PNSSET3_VAL & (1 << 20)},
    {UART5_RST,             UART5_MODULE,           SCU_INIT_PNSSET3_VAL & (1 << 21)},
#endif

#if defined(SCU_INIT_PNSSET4_VAL) && SCU_INIT_PNSSET4_VAL
    {I2C0_RST,              I2C0_MODULE,            SCU_INIT_PNSSET4_VAL & (1 << 0)},
    {I2C1_RST,              I2C1_MODULE,            SCU_INIT_PNSSET4_VAL & (1 << 1)},
    {I2C2_RST,              I2C2_MODULE,            SCU_INIT_PNSSET4_VAL & (1 << 2)},
    {SC0_RST,               SC0_MODULE,             SCU_INIT_PNSSET4_VAL & (1 << 16)},
    {SC1_RST,               SC1_MODULE,             SCU_INIT_PNSSET4_VAL & (1 << 17)},
    {SC2_RST,               SC2_MODULE,             SCU_INIT_PNSSET4_VAL & (1 << 18)},
#endif

#if defined(SCU_INIT_PNSSET5_VAL) && SCU_INIT_PNSSET5_VAL
    {CAN0_RST,              CAN0_MODULE,            SCU_INIT_PNSSET5_VAL & (1 << 0)},
    {QEI0_RST,              QEI0_MODULE,            SCU_INIT_PNSSET5_VAL & (1 << 16)},
    {QEI1_RST,              QEI1_MODULE,            SCU_INIT_PNSSET5_VAL & (1 << 17)},
    {ECAP0_RST,             ECAP0_MODULE,           SCU_INIT_PNSSET5_VAL & (1 << 20)},
    {ECAP1_RST,             ECAP1_MODULE,           SCU_INIT_PNSSET5_VAL & (1 << 21)},
    {DSRC_RST,              DSRC_MODULE,            SCU_INIT_PNSSET5_VAL & (1 << 23)},
    {NU_SYS_MODIDX_UNDEF,   NU_CLK_MODIDX_UNDEF,    SCU_INIT_PNSSET5_VAL & (1 << 24)},
    {TRNG_RST,              TRNG_MODULE,            SCU_INIT_PNSSET5_VAL & (1 << 25)},
#endif

#if defined(SCU_INIT_PNSSET6_VAL) && SCU_INIT_PNSSET6_VAL
    {USBD_RST,              USBD_MODULE,            SCU_INIT_PNSSET6_VAL & (1 << 0)},
    {USCI0_RST,             USCI0_MODULE,           SCU_INIT_PNSSET6_VAL & (1 << 16)},
    {USCI1_RST,             USCI1_MODULE,           SCU_INIT_PNSSET6_VAL & (1 << 17)},
#endif

};

#define NU_MODCLASS_SYS     0
#define NU_MODCLASS_CLK     1

/* Check if specified module is non-secure
 *
 * \param modclass          Module class (NU_MODCLASS_SYS/NU_MODCLASS_CLK)
 * \param modidx            Module index
 *
 * \return                  0 if not non-secure (secure or undefined), or 1 if non-secure
 *
 * \note                    Undefined module index is treated as not non-secure.
 */
static bool check_mod_ns(int modclass, uint32_t modidx);

__NONSECURE_ENTRY
void SYS_ResetModule_S(uint32_t u32ModuleIndex)
{
    /* Guard access to secure module from non-secure domain */
    if (cmse_nonsecure_caller() &&
       (! check_mod_ns(NU_MODCLASS_SYS, u32ModuleIndex))) {
        error("Non-secure domain tries to control secure or undefined module.");
    }
    
    SYS_ResetModule(u32ModuleIndex);
}

__NONSECURE_ENTRY
void CLK_SetModuleClock_S(uint32_t u32ModuleIndex, uint32_t u32ClkSrc, uint32_t u32ClkDiv)
{
    /* Guard access to secure module from non-secure domain */
    if (cmse_nonsecure_caller() &&
       (! check_mod_ns(NU_MODCLASS_CLK, u32ModuleIndex))) {
        error("Non-secure domain tries to control secure or undefined module.");
    }
    
    CLK_SetModuleClock(u32ModuleIndex, u32ClkSrc, u32ClkDiv);
}

__NONSECURE_ENTRY
void CLK_EnableModuleClock_S(uint32_t u32ModuleIndex)
{
    /* Guard access to secure module from non-secure domain */
    if (cmse_nonsecure_caller() &&
       (! check_mod_ns(NU_MODCLASS_CLK, u32ModuleIndex))) {
        error("Non-secure domain tries to control secure or undefined module.");
    }
    
    CLK_EnableModuleClock(u32ModuleIndex);
}

__NONSECURE_ENTRY
void CLK_DisableModuleClock_S(uint32_t u32ModuleIndex)
{
    /* Guard access to secure module from non-secure domain */
    if (cmse_nonsecure_caller() &&
       (! check_mod_ns(NU_MODCLASS_CLK, u32ModuleIndex))) {
        error("Non-secure domain tries to control secure or undefined module.");
    }
    
    CLK_DisableModuleClock(u32ModuleIndex);
}

__NONSECURE_ENTRY
void SYS_LockReg_S(void)
{
    /* Allow non-secure domain to lock/unlock locked registers without check.
     * Guard access to locked registers is done through other related secure functions. */
    SYS_LockReg();
}

__NONSECURE_ENTRY
void SYS_UnlockReg_S(void)
{
    /* Allow non-secure domain to lock/unlock locked registers without check.
     * Guard access to locked registers is done through other related secure functions. */
    SYS_UnlockReg();
}

__NONSECURE_ENTRY
void CLK_Idle_S(void)
{
    CLK_Idle();
}

__NONSECURE_ENTRY
void CLK_PowerDown_S(void)
{
    CLK_PowerDown();
}

static bool check_mod_ns(int modclass, uint32_t modidx)
{
    const nu_modidx_ns_t *modidx_ns = modidx_ns_tab;
    const nu_modidx_ns_t *modidx_ns_end = modidx_ns_tab + sizeof (modidx_ns_tab) / sizeof (modidx_ns_tab[0]);
   
    if (modclass == NU_MODCLASS_SYS) {
        /* The modidx_ns_tab table has 'NONE' module index.
         * We must filter it out to avoid security issue. */
        if (modidx == NU_SYS_MODIDX_UNDEF) {
            return false;
        }

        for (; modidx_ns != modidx_ns_end; modidx_ns ++) {
            if (modidx == modidx_ns->sys_modidx) {
                if (modidx_ns->ns) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    } else if (modclass == NU_MODCLASS_CLK) {
        /* The modidx_ns_tab table has 'NONE' module index. 
         * We must filter it out to avoid security issue. */
        if (modidx == NU_CLK_MODIDX_UNDEF) {
            return false;
        }
        
        for (; modidx_ns != modidx_ns_end; modidx_ns ++) {
            if (modidx == modidx_ns->clk_modidx) {
                if (modidx_ns->ns) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
        
    return false;
}

#endif
