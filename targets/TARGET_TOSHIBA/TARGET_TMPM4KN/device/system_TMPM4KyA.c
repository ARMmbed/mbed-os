/**
 *******************************************************************************
 * @file    system_TMPM4KyA.c
 * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer Source File for the
 *          TOSHIBA 'TMPM4Ky' Device Series
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2022
 * SPDX-License-Identifier: Apache-2.0
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
 *******************************************************************************
 */

#include "TMPM4KNA.h"

/*-------- <<< Start of configuration section >>> ----------------------------*/
#define FC_KCR_KEYCODE      (0xA74A9D23UL)              /*!< The specific code that writes the FCKCR register. */
#define FC_ACCR_FDLC_4      (0x00000300UL)              /*!< Data Flash read clock 4clock  */
#define FC_ACCR_FDLC_5      (0x00000400UL)              /*!< Data Flash read clock 5clock  */
#define FC_ACCR_FDLC_6      (0x00000500UL)              /*!< Data Flash read clock 6clock  */
#define FC_ACCR_FCLC_1      (0x00000000UL)              /*!< Code Flash read clock 1clock  */
#define FC_ACCR_FCLC_2      (0x00000001UL)              /*!< Code Flash read clock 2clock  */
#define FC_ACCR_FCLC_3      (0x00000002UL)              /*!< Code Flash read clock 3clock  */
#define FC_ACCR_FCLC_4      (0x00000003UL)              /*!< Code Flash read clock 4clock  */
#define FC_ACCR_FCLC_5      (0x00000004UL)              /*!< Code Flash read clock 5clock  */
#define FC_ACCR_FCLC_6      (0x00000005UL)              /*!< Code Flash read clock 6clock  */
#define SYSCORECLOCK_80M    (80000000UL)                /*!< 80MHz */

/* Clock Generator (CG) Configuration */
#define CLOCK_SETUP             (1U)                      /* 1:External HOSC, 0: Internal HOSC */

#define CG_GEAR_FC_1            (0x00000000UL)      /*!< CG fc register value */
#define CG_GEAR_FC_2            (0x00000001UL)      /*!< CG fc/2 register value */
#define CG_GEAR_FC_4            (0x00000002UL)      /*!< CG fc/4 register value */
#define CG_GEAR_FC_8            (0x00000003UL)      /*!< CG fc/8 register value */
#define CG_GEAR_FC_16           (0x00000004UL)      /*!< CG fc/16 register value */
#define CG_PRCK_FC_1            (0x00000000UL)      /*!< CG PhiT0 fc register value */
#define CG_PRCK_FC_2            (0x00000100UL)      /*!< CG PhiT0 fc/2 register value */
#define CG_PRCK_FC_4            (0x00000200UL)      /*!< CG PhiT0 fc/4 register value */
#define CG_PRCK_FC_8            (0x00000300UL)      /*!< CG PhiT0 fc/8 register value */
#define CG_PRCK_FC_16           (0x00000400UL)      /*!< CG PhiT0 fc/16 register value */
#define CG_PRCK_FC_32           (0x00000500UL)      /*!< CG PhiT0 fc/32 register value */
#define CG_PRCK_FC_64           (0x00000600UL)      /*!< CG PhiT0 fc/64 register value */
#define CG_PRCK_FC_128          (0x00000700UL)      /*!< CG PhiT0 fc/128 register value*/
#define CG_PRCK_FC_256          (0x00000800UL)      /*!< CG PhiT0 fc/256 register value*/
#define CG_PRCK_FC_512          (0x00000900UL)      /*!< CG PhiT0 fc/512 register value*/

#define CG_MCKSEL_1             (0x00000000UL)      /*!< CG fsysm, mPhiT0 fc value */
#define CG_MCKSEL_2             (0x00000040UL)      /*!< CG fsysm, mPhiT0 fc/2 value */
#define CG_MCKSEL_4             (0x00000080UL)      /*!< CG fsysm, mPhiT0 fc/4 value */

#define CG_GEAR_MASK            ((uint32_t)0x00000007)    /*!< CG GEAR mask */
#define CG_PRCK_MASK            ((uint32_t)0x00000F00)    /*!< CG PRCK mask */
#define CG_MCKSEL_MASK          ((uint32_t)0x000000C0)    /*!< CG MCKSEL mask */
#define CG_PRCKST_MASK          ((uint32_t)0x0F000000)    /*!< CG PRCKST mask */
#define CG_MCKSELPST_MASK       ((uint32_t)0xC0000000)    /*!< CG MCKSELPST mask */
#define CG_MCKSELGST_MASK       ((uint32_t)0x00C00000)    /*!< CG MCKSELGST mask */

#define SYSCR_Val               (CG_GEAR_FC_1 | CG_PRCK_FC_1 | CG_MCKSEL_2)

#define STBYCR_Val              (0x00000000UL)
/* Define clocks */
#define IOSC_10M_DIV2_PLLON     (160000000UL)   /* 10.00MHz * 32.0000 / 2 */

#define EOSC_6M                 (6000000UL)
#define EOSC_8M                 (8000000UL)
#define EOSC_10M                (10000000UL)
#define EOSC_12M                (12000000UL)
#define EXTALH                  EOSC_10M        /* External high-speed oscillator freq */
#define EOSC_6M_DIV2_PLLON      (159900000UL)   /* 6.00MHz  * 53.3125 / 2 */
#define EOSC_8M_DIV2_PLLON      (160000000UL)   /* 8.00MHz  * 40.0000 / 2 */
#define EOSC_10M_DIV2_PLLON     (160000000UL)   /* 10.00MHz * 32.0000 / 2 */
#define EOSC_12M_DIV2_PLLON     (159800000UL)   /* 12.00MHz * 26.6250 / 2 */

#define FIXED_6MHz              (6000000UL)     /* 6.00MHz */
#define FIXED_8MHz              (8000000UL)     /* 8.00MHz */
#define FIXED_10MHz             (10000000UL)    /* 10.00MHz */
#define FIXED_12MHz             (12000000UL)    /* 12.00MHz */
#define FIXED_160MHz            (160000000UL)

#define IOSC_10M                (10000000UL)
#define IXTALH                  IOSC_10M                    /* Internal high-speed oscillator freq */

#define CG_6M_MUL_26_656_FPLL       (0x001C1535UL<<8U)      /* fPLL = 6MHz * 26.656   */
#define CG_8M_MUL_20_FPLL           (0x00245028UL<<8U)      /* fPLL = 8MHz * 20       */
#define CG_10M_MUL_16_FPLL          (0x002E9020UL<<8U)      /* fPLL = 10MHz * 16       */
#define CG_12M_MUL_13_312_FPLL      (0x0036DA1AUL<<8U)      /* fPLL = 12MHz * 13.312   */

#define CG_PLL0SEL_FPLL         ((uint32_t)0x00000002)      /*!< CG PLL0SEL fPLL */
#define CG_PLL0SEL_FOSC         ((uint32_t)0x00000000)      /*!< CG PLL0SEL fosc */

#define PLL0SEL_MASK            (0xFFFFFF00UL)              /*!< CG PLL mask */

#define CG_PLL0SEL_PLL0ON_SET                  ((uint32_t)0x00000001)
#define CG_PLL0SEL_PLL0ON_CLEAR                ((uint32_t)0xFFFFFFFE)
#define CG_PLL0SEL_PLL0SEL_SET                 ((uint32_t)0x00000002)
#define CG_PLL0SEL_PLL0SEL_CLEAR               ((uint32_t)0xFFFFFFFD)

#define CG_OSCSEL_EHOSC         ((uint32_t)0x00000100)      /*!< CG OSCSEL EHOSC */

#define CG_OSCCR_IHOSC1EN_CLEAR              ((uint32_t)0xFFFFFFFE)
#define CG_OSCCR_EOSCEN_SET                  ((uint32_t)0x00000002)

#define CG_OSCCR_OSCSEL_SET                  ((uint32_t)0x00000100)

/* Configure Warm-up time */
#define HZ_1M                   (1000000UL)
#define WU_TIME_EXT             (5000UL)                    /* warm-up time for EXT is 5ms   */
#define WU_TIME_INT             (1634UL)                    /* warm-up time for INT is 163.4us x 10 */
#define WU_TIME_INT_MIN         (0x03C << 20U)              /* minimum warm-up time for INT */
#define CGWUPHCR_WUEF_R_DONE    ((uint32_t)0x00000000)      /*!< WUEF    :[R]    :Done    */

/* Warm-up setting value */
/* EOSC OSCCR<WUPT11:0> = (warm-up time(us) * EXTALH - 16) / 16 */
#define WUPHCR_WUPT_EXT         ((uint32_t)(((((uint64_t)WU_TIME_EXT * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
/* IOSC OSCCR<WUPT11:0> = ((warm-up time(us) - 63.3us) * IXTALH - 41) / 16 */
#define WUPHCR_WUPT_INT         ((uint32_t)(((((uint64_t)(WU_TIME_INT - 633UL) * (IXTALH / 10) / HZ_1M) - 41UL) / 16UL) << 20U))
#define CG_WUPHCR_WUON_START_SET             ((uint32_t)0x00000001)
#if (CLOCK_SETUP)
#define CG_WUPHCR_WUCLK_SET                ((uint32_t)0x00000100)
#define WUPHCR_INIT_PLL     ((uint32_t)(((((uint64_t)INIT_TIME_PLL * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#define WUPHCR_LUPT_PLL     ((uint32_t)(((((uint64_t)LOCKUP_TIME_PLL * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#define PLL0SEL_Ready          CG_10M_MUL_16_FPLL
#else
#define CG_WUPHCR_WUCLK_SET                ((uint32_t)0x00000000)
#define WUPHCR_INIT_PLL     ((uint32_t)(((((uint64_t)((INIT_TIME_PLL * 10) - 633UL) * (IXTALH / 10) / HZ_1M) - 41UL) /16UL) << 20U))
#define WUPHCR_LUPT_PLL     ((uint32_t)(((((uint64_t)((LOCKUP_TIME_PLL * 10) - 633UL) * (IXTALH /10) / HZ_1M) - 41UL) /16UL) << 20U))
#define PLL0SEL_Ready          CG_10M_MUL_12_FPLL
#endif

/* Semi-Independent Watchdog Timer (SIWDT) Configuration */
#define SIWD_SETUP              (1U)                      /* 1:Disable SIWD, 0:Enable SIWD */
#define SIWDEN_Val              (0x00000000UL)            /* SIWD Disable */
#define SIWDCR_Val              (0x000000B1UL)            /* SIWD Disable code */

#define PLL0SEL_Val              (PLL0SEL_Ready|0x00000003UL)

#if defined ( __CC_ARM   ) || defined(__ARMCC_VERSION)      /* RealView Compiler */
extern uint32_t Load$$FLASH_CODE_RAM$$Base;
extern uint32_t Image$$FLASH_CODE_RAM$$Base;
extern uint32_t Load$$FLASH_CODE_RAM$$Length;

#elif defined ( __ICCARM__ )    /* IAR Compiler */
#pragma section = "FLASH_CODE_RAM"
#pragma section = "FLASH_CODE_ROM"
#endif

#if defined ( __CC_ARM   ) || defined(__ARMCC_VERSION)      /* RealView Compiler */
#define FLASH_API_ROM           (uint32_t *)&Load$$FLASH_CODE_RAM$$Base
#define FLASH_API_RAM           (uint32_t *)&Image$$FLASH_CODE_RAM$$Base
#define SIZE_FLASH_API          (uint32_t)&Load$$FLASH_CODE_RAM$$Length



#elif defined ( __ICCARM__ )    /* IAR Compiler */
#define FLASH_API_ROM           ((uint32_t *)__section_begin("FLASH_CODE_ROM"))
#define FLASH_API_RAM           ((uint32_t *)__section_begin("FLASH_CODE_RAM"))
#define SIZE_FLASH_API          ((uint32_t)__section_size("FLASH_CODE_ROM"))

#endif

/*-------- <<< End of configuration section >>> ------------------------------*/

/*-------- DEFINES -----------------------------------------------------------*/
/* Configure Warm-up time */
#define WU_TIME_EXT             (5000UL)         /* warm-up time for EXT is 5ms   */
#define INIT_TIME_PLL           (100UL)          /* Initial time for PLL is 100us */
#define LOCKUP_TIME_PLL         (400UL)          /* Lockup time for PLL is 400us  */
/* Determine core clock frequency according to settings */
/* System clock is high-speed clock*/
#if (CLOCK_SETUP)
#define CORE_TALH (EXTALH)
#else
#define CORE_TALH (IXTALH)
#endif

#if ((PLL0SEL_Val & (1U<<1U)) && (PLL0SEL_Val & (1U<<0U)))  /* If PLL selected and enabled */
#if   (CORE_TALH == EOSC_6M)        /* If input is 6MHz */
#if ((PLL0SEL_Val & PLL0SEL_MASK) == (CG_6M_MUL_26_656_FPLL))
#define __CORE_CLK   EOSC_6M_DIV2_PLLON                               /* output clock is 159.938MHz */
#else                                      /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 6MHz */
#elif (CORE_TALH == EOSC_8M)        /* If input is 8MHz */
#if ((PLL0SEL_Val & PLL0SEL_MASK) == (CG_8M_MUL_20_FPLL))
#define __CORE_CLK   EOSC_8M_DIV2_PLLON                               /* output clock is 160MHz */
#else                                     /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 8MHz */
#elif (CORE_TALH == EOSC_10M)       /* If input is 10MHz */
#if ((PLL0SEL_Val & PLL0SEL_MASK) == CG_10M_MUL_16_FPLL)
#define __CORE_CLK   EOSC_10M_DIV2_PLLON                              /* output clock is 160MHz */
#else                                     /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 10MHz */
#elif (CORE_TALH == EOSC_12M)       /* If input is 12MHz */
#if ((PLL0SEL_Val & PLL0SEL_MASK) == CG_12M_MUL_13_312_FPLL)
#define __CORE_CLK   EOSC_12M_DIV2_PLLON                              /* output clock is 159.75MHz */
#else                                    /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 12MHz    */
#elif (CORE_TALH == IOSC_10M)       /* If input is 10MHz */
#if ((PLL0SEL_Val & PLL0SEL_MASK) == CG_10M_MUL_16_FPLL)
#define __CORE_CLK   IOSC_10M_DIV2_PLLON                              /* output clock is 160MHz */
#else                                    /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 12MHz    */
#else                                                   /* input clock not known */
#define __CORE_CLK   (0U)
#error "Core Oscillator Frequency invalid!"
#endif                                                  /* End switch input clock */
#else                                                       /* If PLL not used */
#define __CORE_CLK   (CORE_TALH)
#endif

#if   ((SYSCR_Val & 7U) == 0U)      /* Gear -> fc                         */
#define __CORE_SYS   (__CORE_CLK)
#elif ((SYSCR_Val & 7U) == 1U)      /* Gear -> fc/2                       */
#define __CORE_SYS   (__CORE_CLK / 2U)
#elif ((SYSCR_Val & 7U) == 2U)      /* Gear -> fc/4                       */
#define __CORE_SYS   (__CORE_CLK / 4U )
#elif ((SYSCR_Val & 7U) == 3U)      /* Gear -> fc/8                       */
#define __CORE_SYS   (__CORE_CLK / 8U)
#elif ((SYSCR_Val & 7U) == 4U)      /* Gear -> fc/16                      */
#define __CORE_SYS   (__CORE_CLK / 16U)
#else                               /* Gear -> reserved                   */
#define __CORE_SYS   (0U)
#endif


/* Clock Variable definitions */
uint32_t SystemCoreClock = __CORE_SYS;  /*!< System Clock Frequency (Core Clock) */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Update SystemCoreClock according register values.
 */
void SystemCoreClockUpdate(void)
{
    /* Get Core Clock Frequency */
    uint32_t CoreClock = 0U;
    uint32_t CoreClockInput = 0U;
    uint32_t regval = 0U;
    uint32_t oscsel = 0U;
    uint32_t pll0sel = 0U;
    uint32_t pll0on = 0U;
    /* Determine clock frequency according to clock register values */
    /* System clock is high-speed clock */
    regval = TSB_CG->OSCCR;
    oscsel = regval & CG_OSCSEL_EHOSC;
    if (oscsel) {  /* If system clock is External high-speed oscillator freq */
        CoreClock = EXTALH;
    } else {                    /* If system clock is Internal high-speed oscillator freq */
        CoreClock = IXTALH;
    }
    regval = TSB_CG->PLL0SEL;
    pll0sel = regval & CG_PLL0SEL_FPLL;
    pll0on = regval & CG_PLL0SEL_PLL0ON_SET;
    if (pll0sel && pll0on) {                        /* If PLL enabled  */
        if (CoreClock == EOSC_6M) {      /* If input is 6MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_6M_MUL_26_656_FPLL) {
                CoreClockInput = EOSC_6M_DIV2_PLLON;           /* output clock is 159.938MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == EOSC_8M) {      /* If input is 8MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_8M_MUL_20_FPLL) {
                CoreClockInput = EOSC_8M_DIV2_PLLON;           /* output clock is 160MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == EOSC_10M) {      /* If input is 10MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_10M_MUL_16_FPLL) {
                CoreClockInput = EOSC_10M_DIV2_PLLON;          /* output clock is 160MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == EOSC_12M) {      /* If input is 12MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_12M_MUL_13_312_FPLL) {
                CoreClockInput = EOSC_12M_DIV2_PLLON;          /* output clock is 159.75MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == IOSC_10M) {      /* If input is 10MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_10M_MUL_16_FPLL) {
                CoreClockInput = EOSC_10M_DIV2_PLLON;          /* output clock is 160MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else {
            CoreClockInput = 0U;
        }
    } else {                                       /* If PLL not used */
        CoreClockInput = CoreClock;
    }

    switch (TSB_CG->SYSCR & 7U) {
        case 0U:                                 /* Gear -> fc */
            SystemCoreClock = CoreClockInput;
            break;
        case 1U:                                 /* Gear -> fc/2 */
            SystemCoreClock = CoreClockInput / 2U;
            break;
        case 2U:                                 /* Gear -> fc/4 */
            SystemCoreClock = CoreClockInput / 4U;
            break;
        case 3U:                                 /* Gear -> fc/8 */
            if (CoreClockInput >= EOSC_8M) {
                SystemCoreClock = CoreClockInput / 8U;
            } else {
                SystemCoreClock = 0U;
            }
            break;
        case 4U:                                 /* Gear -> fc/16 */
            if (CoreClockInput > EOSC_12M) {
                SystemCoreClock = CoreClockInput / 16U;
            } else {
                SystemCoreClock = 0U;
            }
            break;
        case 5U:
        case 6U:
        case 7U:
            SystemCoreClock = 0U;
            break;
        default:
            SystemCoreClock = 0U;
            break;
    }
}

#if defined ( __CC_ARM   ) || defined ( __ICCARM__ ) || defined(__ARMCC_VERSION)
static void CopyRoutine(uint32_t *dest, uint32_t *source, uint32_t size);
#endif
static void FlashReadClockSet(uint32_t sysclock);

/*--------------------------------------------------*/
/**
  * @brief  copy 32-bit data from source to dest
  * @param  the address of source and dast, the data size
  * @retval none.
  * @note   -
  */
/*--------------------------------------------------*/
static void CopyRoutine(uint32_t *dest, uint32_t *source, uint32_t size)
{
    uint32_t *dest_addr, *source_addr, tmpsize;
    uint32_t i, tmps, tmpd, mask;

    dest_addr = dest;
    source_addr = source;

    tmpsize = size >> 2U;
    for (i = 0U; i < tmpsize; i++) {    /* 32bits copy */
        *dest_addr = *source_addr;
        dest_addr++;
        source_addr++;
    }
    if (size & 0x00000003U) {   /* if the last data size is not 0(maybe 1,2 or 3), copy the last data */
        mask = 0xFFFFFF00U;
        i = size & 0x00000003U;
        tmps = *source_addr;
        tmpd = *dest_addr;
        while (i - 1U) {
            mask = mask << 8U;
            i--;
        }
        tmps = tmps & (~mask);
        tmpd = tmpd & (mask);
        *dest_addr = tmps + tmpd;       /* 32bits copy, but only change the bytes need to be changed */
    } else {
        /* Do nothing */
    }
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
#if (SIWD_SETUP)                  /* Watchdog Setup */
    /* SIWD Disable */
    TSB_SIWD0->EN = SIWDEN_Val;
    TSB_SIWD0->CR = SIWDCR_Val;
#else
    /* SIWD Enable (Setting after a Reset) */
#endif

#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)   /* FPU setting */
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

#if defined ( __CC_ARM   ) || defined ( __ICCARM__ ) || defined(__ARMCC_VERSION)
    CopyRoutine(FLASH_API_RAM, FLASH_API_ROM, SIZE_FLASH_API);     /* copy flash API to RAM */
#endif
    FlashReadClockSet(FIXED_160MHz);

#if (CLOCK_SETUP)               /* Clock(external) Setup */

    TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_SET;
    TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | CG_WUPHCR_WUCLK_SET);
    TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
    while (TSB_CG_WUPHCR_WUEF) {
        ;
    }                           /* Warm-up */
    TSB_CG->OSCCR |= CG_OSCCR_OSCSEL_SET;
    while (!TSB_CG_OSCCR_OSCF) {
        ;
    }                         /* Confirm CGOSCCR<OSCF>="1" */
#else
    /* Internal HOSC Enable (Setting after a Reset) */
#endif

    TSB_CG->SYSCR = SYSCR_Val;
    while ((TSB_CG->SYSCR & CG_MCKSELGST_MASK) != ((SYSCR_Val & CG_MCKSEL_MASK) << 16U)) {
        ;
    }                         /* Confirm CGSYSCCR<MCKSELGST>="01" */
    while ((TSB_CG->SYSCR & CG_MCKSELPST_MASK) != ((SYSCR_Val & CG_MCKSEL_MASK) << 24U)) {
        ;
    }                         /* Confirm CGSYSCR<MCKSELPST>="01" */
    while ((TSB_CG->SYSCR & CG_PRCKST_MASK) != ((SYSCR_Val & CG_PRCK_MASK) << 16U)) {
        ;
    }                         /* Confirm CGSYSCR<PRCKST>="0000" */

#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUCLK_SET);
#else
    TSB_CG->WUPHCR = (WUPHCR_INIT_PLL);
#endif
    TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0SEL_CLEAR;    /* PLL-->fOsc */
    TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0ON_CLEAR;
    TSB_CG->PLL0SEL = PLL0SEL_Ready;
#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
#else
    TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUON_START_SET);
#endif

    while (TSB_CG_WUPHCR_WUEF) {
        ;
    }                           /* Warm-up */

#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUCLK_SET);
#else
    TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL);
#endif
    TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0ON_SET;    /* PLL enabled */
    TSB_CG->STBYCR = STBYCR_Val;

#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
#else
    TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUON_START_SET);
#endif
    while (TSB_CG_WUPHCR_WUEF) {
        ;
    }                           /* Warm-up */

    TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0SEL_SET;
    while (!TSB_CG_PLL0SEL_PLL0ST) {
        ;
    }                        /*Confirm CGPLL0SEL<PLL0ST> = "1" */

#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->OSCCR &= CG_OSCCR_IHOSC1EN_CLEAR;
#endif
}

/*--------------------------------------------------*/
/**
  * @brief  Raed clock set.
  * @param  none.
  * @retval none.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __CC_ARM   )   /* RealView Compiler */
#pragma arm section code="FLASH_ROM"
#elif defined ( __ICCARM__ )    /* IAR Compiler */
#pragma location = "FLASH_ROM"
#elif defined(__ARMCC_VERSION)
__attribute__((section("FLASH_ROM")))
#else    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void FlashReadClockSet(uint32_t sysclock)
{
    uint32_t regval = 0;

    if (sysclock > SYSCORECLOCK_80M) {
        regval = (uint32_t)(FC_ACCR_FDLC_5 | FC_ACCR_FCLC_5);
        TSB_FC->KCR = FC_KCR_KEYCODE;
        TSB_FC->FCACCR = regval;
        while (TSB_FC->FCACCR != (uint32_t)(FC_ACCR_FDLC_5 | FC_ACCR_FCLC_5)) {
            /* no processing */
        }
    } else {
        regval = (uint32_t)(FC_ACCR_FDLC_4 | FC_ACCR_FCLC_4);
        TSB_FC->KCR = FC_KCR_KEYCODE;
        TSB_FC->FCACCR = regval;
        while (TSB_FC->FCACCR != (uint32_t)(FC_ACCR_FDLC_4 | FC_ACCR_FCLC_4)) {
            /* no processing */
        }
    }
}
