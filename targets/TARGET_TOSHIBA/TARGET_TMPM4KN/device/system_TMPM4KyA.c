/**
 *******************************************************************************
 * @file    system_TMPM4KyA.c
 * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer Source File for the
 *          TOSHIBA 'TMPM4ky' Device Series
 * @version V1.0.0.0
 * $Date:: 2020-09-17 #$
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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

/* Semi-Independent Watchdog Timer (SIWDT) Configuration */
#define SIWD_SETUP              (1U)                      /* 1:Disable SIWD, 0:Enable SIWD */
#define SIWDEN_Val              (0x00000000UL)            /* SIWD Disable */
#define SIWDCR_Val              (0x000000B1UL)            /* SIWD Disable code */

/* Clock Generator (CG) Configuration */
#define CLOCK_SETUP             (1U)                      /* 1:External HOSC, 0: Internal HOSC */

#define CG_SYSCR_MCKSEL_SET         (SYSCR_MCKSEL_Val << 6U)
#define CG_SYSCR_MCKSELGST_Val      (SYSCR_MCKSEL_Val << 22U)
#define CG_SYSCR_MCKSELPST_Val      (SYSCR_MCKSEL_Val << 30U)

#define CG_6M_MUL_26_656_FPLL       (0x001C1535UL<<8U)      /* fPLL = 6MHz * 26.656   */
#define CG_8M_MUL_20_FPLL           (0x00245028UL<<8U)      /* fPLL = 8MHz * 20       */
#define CG_10M_MUL_16_FPLL          (0x002E9020UL<<8U)      /* fPLL = 10MHz * 16       */
#define CG_12M_MUL_13_312_FPLL      (0x0036DA1AUL<<8U)      /* fPLL = 12MHz * 13.312   */

#define CG_PLL0SEL_PLL0ON_SET       (0x00000001UL)
#define CG_PLL0SEL_PLL0ON_CLEAR     (0xFFFFFFFEUL)
#define CG_PLL0SEL_PLL0SEL_SET      (0x00000002UL)
#define CG_PLL0SEL_PLL0SEL_CLEAR    (0xFFFFFFFDUL)

#define CG_OSCCR_IHOSC1EN_CLEAR     (0xFFFFFFFEUL)
#define CG_OSCCR_EOSCEN_SET         (0x00000002UL)
#define CG_OSCCR_OSCSEL_SET         (0x00000100UL)

#define SYSCR_GEAR_Val              (0x00000000UL)            /* GEAR = fc */
#define SYSCR_PRCK_Val              (0x00000000UL)            /* phiT0 = fc */
#define SYSCR_MCKSEL_Val            (0x00000001UL)            /* fsysm(phiT0m) = fsysh(phiT0h) / 2 */

#define STBYCR_Val                  (0x00000000UL)

#define CG_WUPHCR_WUON_START_SET    (0x00000001UL)
#define EXT_CG_WUPHCR_WUCLK_SET     (0x00000000UL)      /* WUCLK for External HOSC select the IHOSC1 */

#if (CLOCK_SETUP)
#define CG_WUPHCR_WUCLK_SET     (0x00000100UL)      /* WUCLK for Inital/Lockup time */
#define PLL0SEL_Ready           CG_10M_MUL_16_FPLL
#else
#define CG_WUPHCR_WUCLK_SET     (0x00000000UL)      /* WUCLK for Inital/Lockup time */
#define PLL0SEL_Ready           CG_10M_MUL_16_FPLL
#endif

#define PLL0SEL_Val                 (PLL0SEL_Ready|0x00000003UL)
#define PLL0SEL_MASK                (0xFFFFFF00UL)

/*-------- <<< End of configuration section >>> ------------------------------*/

/*-------- DEFINES -----------------------------------------------------------*/
/* Define clocks */
#define IOSC_10M_DIV4_PLLON     (80000000UL)    /* 10.00MHz * 32.0000 / 4 */
#define IOSC_10M_DIV8_PLLON     (40000000UL)    /* 10.00MHz * 32.0000 / 8 */
#define IOSC_10M_DIV2_PLLON     (160000000UL)   /* 10.00MHz * 32.0000 / 2 */

#define EOSC_6M                 (6000000UL)
#define EOSC_8M                 (8000000UL)
#define EOSC_10M                (10000000UL)
#define EOSC_12M                (12000000UL)
#define IOSC_10M                (10000000UL)
#define EXTALH                  EOSC_10M        /* External high-speed oscillator freq */
#define IXTALH                  IOSC_10M        /* Internal high-speed oscillator freq */
#define EOSC_6M_DIV4_PLLON      (79970000UL)    /* 6.00MHz  * 53.3125 / 4 */
#define EOSC_8M_DIV4_PLLON      (80000000UL)    /* 8.00MHz  * 40.0000 / 4 */
#define EOSC_10M_DIV4_PLLON     (80000000UL)    /* 10.00MHz * 32.0000 / 4 */
#define EOSC_12M_DIV4_PLLON     (79880000UL)    /* 12.00MHz * 26.6250 / 4 */
#define EOSC_6M_DIV8_PLLON      (39980000UL)    /* 6.00MHz  * 53.3125 / 8 */
#define EOSC_8M_DIV8_PLLON      (40000000UL)    /* 8.00MHz  * 40.0000 / 8 */
#define EOSC_10M_DIV8_PLLON     (40000000UL)    /* 10.00MHz * 32.0000 / 8 */
#define EOSC_12M_DIV8_PLLON     (39940000UL)    /* 12.00MHz * 26.6250 / 8 */
#define EOSC_6M_DIV2_PLLON      (160000000UL)   /* 6.00MHz  * 53.3125 / 2 */
#define EOSC_8M_DIV2_PLLON      (160000000UL)   /* 8.00MHz  * 40.0000 / 2 */
#define EOSC_10M_DIV2_PLLON     (160000000UL)   /* 10.00MHz * 32.0000 / 2 */
#define EOSC_12M_DIV2_PLLON     (160000000UL)   /* 12.00MHz * 26.6250 / 2 */

/* Configure Warm-up time */
#define HZ_1M                       (1000000UL)
#define WU_TIME_EXT                 (5000UL)            /* warm-up time for EXT is 5ms   */
#define INIT_TIME_PLL               (100UL)             /* Initial time for PLL is 100us */
#define LOCKUP_TIME_PLL             (400UL)             /* Lockup time for PLL is 400us  */

#define WUPHCR_WUPT_EXT        ((uint32_t)(((((uint64_t)WU_TIME_EXT * IXTALH / HZ_1M) - 16UL) /16UL) << 20U)) /* OSCCR<WUPT11:0> = (warm-up time(us) * IXTALH - 16) / 16 */
#if (CLOCK_SETUP)
#define WUPHCR_INIT_PLL      ((uint32_t)(((((uint64_t)INIT_TIME_PLL * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#define WUPHCR_LUPT_PLL      ((uint32_t)(((((uint64_t)LOCKUP_TIME_PLL * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#else
#define WUPHCR_INIT_PLL      ((uint32_t)(((((uint64_t)INIT_TIME_PLL * IXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#define WUPHCR_LUPT_PLL      ((uint32_t)(((((uint64_t)LOCKUP_TIME_PLL * IXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#endif
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

#if   ((SYSCR_GEAR_Val & 7U) == 0U)     /* Gear -> fc                         */
#define __CORE_SYS   (__CORE_CLK)
#elif ((SYSCR_GEAR_Val & 7U) == 1U)     /* Gear -> fc/2                       */
#define __CORE_SYS   (__CORE_CLK / 2U)
#elif ((SYSCR_GEAR_Val & 7U) == 2U)     /* Gear -> fc/4                       */
#define __CORE_SYS   (__CORE_CLK / 4U )
#elif ((SYSCR_GEAR_Val & 7U) == 3U)     /* Gear -> fc/8                       */
#define __CORE_SYS   (__CORE_CLK / 8U)
#elif ((SYSCR_GEAR_Val & 7U) == 4U)     /* Gear -> fc/16                      */
#define __CORE_SYS   (__CORE_CLK / 16U)
#else                                   /* Gear -> reserved                   */
#define __CORE_SYS   (0U)
#endif


/* Clock Variable definitions */
uint32_t SystemCoreClock = __CORE_SYS;  /*!< System Clock Frequency (Core Clock) */

/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Private_define Flash Private Define
 *  @{
 */
#define FC_KCR_KEYCODE      (0xA74A9D23UL)              /*!< The specific code that writes the FCKCR register. */
#define FC_BRANK_VALUE      (uint32_t)(0xFFFFFFFFUL)    /*!< Brank value */
#define FC_MAX_PAGES        (uint8_t)(0x80)             /*!< Maxmum pages */
#define FC_MAX_BLOCKS       (uint8_t)(0x16)             /*!< Maxmum blocks */
#define FC_MAX_AREAS        (uint8_t)(0x1)              /*!< Maxmum areas */
#define FC_MAX_DATA_PAGES   (uint8_t)(0x21)             /*!< Maxmum pages */
#define FC_MAX_DATA_BLOCKS  (uint8_t)(0x8)              /*!< Maxmum blocks */
#define FC_CMD_ADDRESS_MASK (uint32_t)(0xFFFF0000UL)    /*!< Upper address mask of the upper address */
#define FC_CMD_BC1_ADDR     (0x00000550UL)              /*!< The lower address of the first bus cycle when uses commans */
#define FC_CMD_BC2_ADDR     (0x00000AA0UL)              /*!< The lower address of the second bus cycle when uses commans */
#define FC_BANK_USER_INFO   (0x00000007UL)              /*!< Bank Change User Information Area */
#define FC_BANK_CODE_FLASH  (0x00000000UL)              /*!< Bank Change Code Flash */
#define FC_BUFFER_DISABLE   (0x00000007UL)              /*!< Flash Buffer Disable nad Clear */
#define FC_BUFFER_ENABLE    (0x00000000UL)              /*!< Flash Buffer Enable */

#define FC_ACCR_FDLC_4      (0x00000300UL)              /*!< Data Flash read clock 4clock  */
#define FC_ACCR_FDLC_5      (0x00000400UL)              /*!< Data Flash read clock 5clock  */
#define FC_ACCR_FDLC_6      (0x00000500UL)              /*!< Data Flash read clock 6clock  */
#define FC_ACCR_FCLC_1      (0x00000000UL)              /*!< Code Flash read clock 1clock  */
#define FC_ACCR_FCLC_2      (0x00000001UL)              /*!< Code Flash read clock 2clock  */
#define FC_ACCR_FCLC_3      (0x00000002UL)              /*!< Code Flash read clock 3clock  */
#define FC_ACCR_FCLC_4      (0x00000003UL)              /*!< Code Flash read clock 4clock  */
#define FC_ACCR_FCLC_5      (0x00000004UL)              /*!< Code Flash read clock 5clock  */
#define FC_ACCR_FCLC_6      (0x00000005UL)              /*!< Code Flash read clock 6clock  */
#define SYSCORECLOCK_80M    (80000000UL)                /*!< 80MHz                         */


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
    oscsel = regval & CG_OSCCR_OSCSEL_SET;
    if (oscsel) {  /* If system clock is External high-speed oscillator freq */
        CoreClock = EXTALH;
    } else {                    /* If system clock is Internal high-speed oscillator freq */
        CoreClock = IXTALH;
    }
    regval = TSB_CG->PLL0SEL;
    pll0sel = regval & CG_PLL0SEL_PLL0SEL_SET;
    pll0on = regval & CG_PLL0SEL_PLL0ON_SET;
    if (pll0sel && pll0on) {                        /* If PLL enabled  */
        if (CoreClock == EOSC_6M) {      /* If input is 6MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_6M_MUL_26_656_FPLL) {
                CoreClockInput = EOSC_6M_DIV2_PLLON;                /* output clock is 159.938MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == EOSC_8M) {      /* If input is 8MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_8M_MUL_20_FPLL) {
                CoreClockInput = EOSC_8M_DIV2_PLLON;                /* output clock is 160MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == EOSC_10M) {      /* If input is 10MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_10M_MUL_16_FPLL) {
                CoreClockInput = EOSC_10M_DIV2_PLLON;               /* output clock is 160MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == EOSC_12M) {      /* If input is 12MHz */
            if ((TSB_CG->PLL0SEL & PLL0SEL_MASK) == CG_12M_MUL_13_312_FPLL) {
                CoreClockInput = EOSC_12M_DIV2_PLLON;               /* output clock is 159.75MHz */
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
            if (CoreClockInput >= EOSC_12M) {
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

#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->SYSCR = (SYSCR_GEAR_Val | (SYSCR_PRCK_Val << 8)); /* set <GEAR> <PRCK> */

    TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | CG_WUPHCR_WUCLK_SET);
    TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_SET;
    TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
    while (TSB_CG_WUPHCR_WUEF) {
        ;
    }                           /* Warm-up */

    TSB_CG->OSCCR |= CG_OSCCR_OSCSEL_SET;
    while (!TSB_CG_OSCCR_OSCF) {
        ;
    }                         /* Confirm CGOSCCR<OSCF>="1" */
    TSB_CG->OSCCR &= CG_OSCCR_IHOSC1EN_CLEAR ;
#else
    /* Internal HOSC Enable (Setting after a Reset) */
#endif

    /* PLL Setup */
    TSB_CG->SYSCR = (SYSCR_GEAR_Val | CG_SYSCR_MCKSEL_SET);   /* set <MCKSEL> */
    while ((TSB_CG->SYSCR & (CG_SYSCR_MCKSELGST_Val | CG_SYSCR_MCKSELPST_Val))
            != ((CG_SYSCR_MCKSELGST_Val | CG_SYSCR_MCKSELPST_Val))) {
        ;
    }

    TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUCLK_SET);
    TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0SEL_CLEAR;    /* PLL-->fOsc */
    TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0ON_CLEAR;
    TSB_CG->PLL0SEL = PLL0SEL_Ready;
    TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
    while (TSB_CG_WUPHCR_WUEF) {
        ;
    }                           /* Warm-up */

    TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUCLK_SET);
    TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0ON_SET;    /* PLL enabled */
    TSB_CG->STBYCR = STBYCR_Val;
    TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
    while (TSB_CG_WUPHCR_WUEF) {
        ;
    }                           /* Lockup */
    TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0SEL_SET;
    while (!TSB_CG_PLL0SEL_PLL0ST) {
        ;
    }                        /*Confirm CGPLL0SEL<PLL0ST> = "1" */

}


