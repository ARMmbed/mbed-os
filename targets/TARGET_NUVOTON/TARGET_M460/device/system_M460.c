/**************************************************************************//**
 * @file     system_M460.c
 * @version  V3.000
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Source File for M460
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "NuMicro.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __SYSTEM_CLOCK;    /*!< System Clock Frequency (Core Clock)*/
uint32_t CyclesPerUs      = (__HSI / 1000000UL); /* Cycles per micro second */
uint32_t PllClock         = __HSI;             /*!< PLL Output Clock Frequency         */
uint32_t gau32ClkSrcTbl[] = {__HXT, __LXT, 0UL, __LIRC, 0UL, 0UL, 0UL, __HIRC};

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    uint32_t u32Freq, u32ClkSrc;
    uint32_t u32HclkDiv;

    /* Update PLL Clock */
    PllClock = CLK_GetPLLClockFreq();

    u32ClkSrc = CLK->CLKSEL0 & CLK_CLKSEL0_HCLKSEL_Msk;

    if(u32ClkSrc == CLK_CLKSEL0_HCLKSEL_PLL)
    {
        /* Use PLL clock */
        u32Freq = PllClock;
    }
    else
    {
        /* Use the clock sources directly */
        u32Freq = gau32ClkSrcTbl[u32ClkSrc];
    }

    u32HclkDiv = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLKDIV_Msk) + 1UL;

    /* Update System Core Clock */
    SystemCoreClock = u32Freq / u32HclkDiv;


    //if(SystemCoreClock == 0)
    //    __BKPT(0);

    CyclesPerUs = (SystemCoreClock + 500000UL) / 1000000UL;
}


/**
 * @brief  Initialize the System
 *
 * @param  none
 * @return none
 */
void SystemInit (void)
{

    /* Add your system initialize code here.
       Do not use global variables because this function is called before
       reaching pre-main. RW section maybe overwritten afterwards.          */

    /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1U) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */
#endif

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Set HCLK switch to be reset by HRESET reset sources */
    outpw(0x40000014, inpw(0x40000014)|BIT7);

#if MBED_CONF_TARGET_HXT_PRESENT
    /* Set HXT crystal as INV type */
    CLK->PWRCTL &= ~CLK_PWRCTL_HXTSELTYP_Msk;
#endif

    /* Lock protected registers */
    SYS_LockReg();

#if defined(MBED_CONF_TARGET_HBI_ENABLE) && MBED_CONF_TARGET_HBI_ENABLE
    /* Initialize HBI for HyperRAM */
    void nu_hbi_init(void);
    nu_hbi_init();
#else
    int32_t nu_hyperram_used(void);
    if (nu_hyperram_used()) {
        /* TODO: Report error: HyperRAM used but HBI not enabled */
    }
#endif
}

/* Detect whether or not HyperRAM is used
 *
 * NOTE: For Arm Compiler, Image$$region_name doesn't include ZI. To avoid
 *       confusion, use Image$$region_name$$RO/Image$$region_name$$RW/
 *       Image$$region_name$$ZI instead.
 * NOTE: Compiler e.g. Arm Compiler can optimize assuming (&region_name$$Limit != &region_name$$Base) being true.
 *       Change to (&region_name$$Limit - &region_name$$Base) instead.
 * NOTE: Compiler e.g. GCC can optimize assuming &region_name$$Length being non-zero.
 *       Change to (&region_name$$Limit - &region_name$$Base) instead.
 */
#if defined(__ARMCC_VERSION) || defined(__GNUC__)
extern uint32_t Image$$NU_HYPERRAM$$RO$$Base;
extern uint32_t Image$$NU_HYPERRAM$$RO$$Limit;
extern uint32_t Image$$NU_HYPERRAM$$RW$$Base;
extern uint32_t Image$$NU_HYPERRAM$$RW$$Limit;
extern uint32_t Image$$NU_HYPERRAM$$ZI$$Base;
extern uint32_t Image$$NU_HYPERRAM$$ZI$$Limit;
#define NU_HYPERRAM_USED                                                                            \
    ((((uint32_t) &Image$$NU_HYPERRAM$$RO$$Limit) - ((uint32_t) &Image$$NU_HYPERRAM$$RO$$Base)) || \
    (((uint32_t) &Image$$NU_HYPERRAM$$RW$$Limit) - ((uint32_t) &Image$$NU_HYPERRAM$$RW$$Base)) ||  \
    (((uint32_t) &Image$$NU_HYPERRAM$$ZI$$Limit) - ((uint32_t) &Image$$NU_HYPERRAM$$ZI$$Base)))
#elif defined(__ICCARM__)
extern uint32_t NU_HYPERRAM$$Base;
extern uint32_t NU_HYPERRAM$$Limit;
#define NU_HYPERRAM_USED                                                                            \
    (!!(((uint32_t) &NU_HYPERRAM$$Limit) - ((uint32_t) &NU_HYPERRAM$$Base)))
#endif

int32_t nu_hyperram_used(void)
{
    return NU_HYPERRAM_USED;
}

#if defined(MBED_CONF_TARGET_HBI_ENABLE) && MBED_CONF_TARGET_HBI_ENABLE

/* Simple array size macro without type check */
#define _NU_ARRAY_SIZE(arr)     (sizeof(arr)/sizeof(arr[0]))

void nu_hbi_init(void)
{
    /* Configurable HBI multi-function pin
     *
     * NOTE: C runtime not initialized yet, locate at ROM region.
     */
    static const uint32_t hbi_mfp_reg_arr[] = {
        MBED_CONF_TARGET_HBI_MFP_REG_LIST
    };
    static const uint32_t hbi_mfp_reg_msk_arr[] = {
        MBED_CONF_TARGET_HBI_MFP_REG_MSK_LIST
    };
    static const uint32_t hbi_mfp_reg_val_arr[] = {
        MBED_CONF_TARGET_HBI_MFP_REG_VAL_LIST
    };

    /* Make sure consistent HBI multi-function pin configurations */
    _Static_assert(_NU_ARRAY_SIZE(hbi_mfp_reg_arr) == _NU_ARRAY_SIZE(hbi_mfp_reg_msk_arr),
                   "Inconsistent HBI MFP register and mask list length");
    _Static_assert(_NU_ARRAY_SIZE(hbi_mfp_reg_arr) == _NU_ARRAY_SIZE(hbi_mfp_reg_val_arr),
                   "Inconsistent HBI MFP register and value list length");

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Initialize HBI module */
    SYS_ResetModule(HBI_RST);

    /* Enable HBI module clock */
    CLK_EnableModuleClock(HBI_MODULE);

    /* Set HBI multi-function pins */
    const uint32_t *reg_pos = hbi_mfp_reg_arr;
    const uint32_t *reg_end = hbi_mfp_reg_arr + _NU_ARRAY_SIZE(hbi_mfp_reg_arr);
    const uint32_t *msk_pos = hbi_mfp_reg_msk_arr;
    const uint32_t *val_pos = hbi_mfp_reg_val_arr;
    for (; reg_pos != reg_end; reg_pos ++, msk_pos ++, val_pos ++) {
        M32(*reg_pos) = (M32(*reg_pos) & ~*msk_pos) | *val_pos;
    }

    /* Lock protected registers */
    SYS_LockReg();
}

#endif  /* #if defined(MBED_CONF_TARGET_HBI_ENABLE) && MBED_CONF_TARGET_HBI_ENABLE */
