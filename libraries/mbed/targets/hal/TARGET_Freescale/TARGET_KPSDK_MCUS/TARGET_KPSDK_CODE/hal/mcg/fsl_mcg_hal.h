/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
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

#if !defined(__FSL_MCG_HAL_H__)
#define __FSL_MCG_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_mcg_features.h"

/*! @addtogroup mcg_hal*/
/*! @{*/

/*! @file fsl_mcg_hal.h */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief MCG constant definitions*/
enum _mcg_constant
{
   kMcgConstant1 =        (1u),
   kMcgConstant5 =        (5u),
   
   kMcgConstant640 =      (640u),
   kMcgConstant1280 =    (1280u),
   kMcgConstant1920 =    (1920u),
   kMcgConstant2560 =    (2560u),
   kMcgConstant732  =    (732u),
   kMcgConstant1464 =    (1464u),
   kMcgConstant2197 =    (2197u),
   kMcgConstant2929 =    (2929u),

   kMcgConstantHex20 =       (0x20u),
   kMcgConstantHex40 =       (0x40u),
   kMcgConstantHex60 =       (0x60u),
   kMcgConstantHex80 =       (0x80u),
   kMcgConstantHexA0 =       (0xA0u),
   kMcgConstantHexC0 =       (0xC0u),
   kMcgConstantHexE0 =       (0xE0u)
};

/*! @brief MCG clock source select */
typedef enum _mcg_clock_select
{
    kMcgClockSelectOut,             /* Output of FLL or PLLCS is selected(depends on PLLS bit) */
    kMcgClockSelectIn,              /* Internal reference clock is selected */
    kMcgClockSelectExt,             /* External reference clock is selected */
    kMcgClockSelectReserved
} mcg_clock_select_t;

/*! @brief MCG internal reference clock source select */
typedef enum _mcg_iref_clock_source
{
    kMcgIrefClockSourceExt,         /* External reference clock is selected */
    kMcgIrefClockSourceSlow         /* The slow internal reference clock is selected */
} mcg_iref_clock_source_t;

/*! @brief MCG frequency range select */
typedef enum _mcg_freq_range_select
{
    kMcgFreqRangeSelectLow,         /* Low frequency range selected for the crystal OSC */
    kMcgFreqRangeSelectHigh,        /* High frequency range selected for the crystal OSC */
    kMcgFreqRangeSelectVeryHigh,    /* Very High frequency range selected for the crystal OSC */
    kMcgFreqRangeSelectVeryHigh1    /* Very High frequency range selected for the crystal OSC */
} mcg_freq_range_select_t;

/*! @brief MCG high gain oscillator select */
typedef enum _mcg_hgo_select
{
    kMcgHgoSelectLow,               /* Configure crystal oscillator for low-power operation */
    kMcgHgoSelectHigh               /* Configure crystal oscillator for high-gain operation */
} mcg_hgo_select_t;

/*! @brief MCG high gain oscillator select */
typedef enum _mcg_eref_clock_select
{
    kMcgErefClockSelectExt,         /* External reference clock requested */
    kMcgErefClockSelectOsc          /* Oscillator requested */
} mcg_eref_clock_select_t;

/*! @brief MCG low power select */
typedef enum _mcg_lp_select
{
    kMcgLpSelectNormal,             /* FLL (or PLL) is not disabled in bypass modes */
    kMcgLpSelectLowPower            /* FLL (or PLL) is disabled in bypass modes (lower power) */
} mcg_lp_select_t;

/*! @brief MCG internal reference clock select */
typedef enum _mcg_iref_clock_select
{
    kMcgIrefClockSelectSlow,        /* Slow internal reference clock selected */
    kMcgIrefClockSelectFast         /* Fast internal reference clock selected */
} mcg_iref_clock_select_t;

/*! @brief MCG DCO Maximum Frequency with 32.768 kHz Reference */
typedef enum _mcg_dmx32_select
{
    kMcgDmx32Default,               /* DCO has a default range of 25% */
    kMcgDmx32Fine                   /* DCO is fine-tuned for maximum frequency with 32.768 kHz reference */
} mcg_dmx32_select_t;

/*! @brief MCG DCO range select */
typedef enum _mcg_dco_range_select
{
    kMcgDcoRangeSelectLow,          /* Low frequency range */
    kMcgDcoRangeSelectMid,          /* Mid frequency range*/
    kMcgDcoRangeSelectMidHigh,      /* Mid-High frequency range */
    kMcgDcoRangeSelectHigh          /* High frequency range */
} mcg_dco_range_select_t;

/*! @brief MCG PLL external reference clock select */
typedef enum _mcg_pll_eref_clock_select
{
    kMcgPllErefClockSelectOsc0,    /* Selects OSC0 clock source as its external reference clock */
    kMcgPllErefClockSelectOsc1     /* Selects OSC1 clock source as its external reference clock */
} mcg_pll_eref_clock_select_t;

/*! @brief MCG PLL select */
typedef enum _mcg_pll_select
{
    kMcgPllSelectFll,               /* FLL is selected */
    kMcgPllSelectPllcs              /* PLLCS output clock is selected */
} mcg_pll_select_t;

/*! @brief MCG loss of lock status */
typedef enum _mcg_lols_status
{
    kMcgLolsNotLostLock,           /* PLL has not lost lock since LOLS 0 was last cleared */
    kMcgLolsLostLock               /* PLL has lost lock since LOLS 0 was last cleared */
} mcg_lols_status_t;

/*! @brief MCG lock status */
typedef enum _mcg_lock_status
{
    kMcgLockUnlocked,              /* PLL is currently unlocked */
    kMcgLockLocked                 /* PLL is currently locked */
} mcg_lock_status_t;

/*! @brief MCG clock status */
typedef enum _mcg_pllst_status
{
    kMcgPllstFll,                   /* Source of PLLS clock is FLL clock */
    kMcgPllstPllcs                  /* Source of PLLS clock is PLLCS output clock */
} mcg_pllst_status_t;

/*! @brief MCG iref status */
typedef enum _mcg_irefst_status
{
    kMcgIrefstExt,                  /* FLL reference clock is the external reference clock */
    kMcgIrefstInt                   /* FLL reference clock is the internal reference clock */
} mcg_irefst_status_t;

/*! @brief MCG clock mode status */
typedef enum _mcg_clkst_status
{
    kMcgClkstFll,                   /* Output of the FLL is selected (reset default) */
    kMcgClkstIref,                  /* Internal reference clock is selected */
    kMcgClkstEref,                  /* External reference clock is selected */
    kMcgClkstPll                    /* Output of the PLL is selected */
} mcg_clkst_status_t;

/*! @brief MCG ircst status */
typedef enum _mcg_ircst_status
{
    kMcgIrcstSlow,                  /* internal reference clock is the slow clock (32 kHz IRC) */
    kMcgIrcstFast                   /* internal reference clock is the fast clock (2 MHz IRC) */
} mcg_ircst_status_t;

/*! @brief MCG auto trim fail status */
typedef enum _mcg_atmf_status
{
    kMcgAtmfNormal,                 /* Automatic Trim Machine completed normally */
    kMcgAtmfFail                    /* Automatic Trim Machine failed */
} mcg_atmf_status_t;

/*! @brief MCG loss of clock status */
typedef enum _mcg_locs0_status
{
    kMcgLocs0NotOccured,            /* Loss of OSC0 has not occurred */
    kMcgLocs0Occured                /* Loss of OSC0 has occurred */
} mcg_locs0_status_t;

/*! @brief MCG Automatic Trim Machine Select */
typedef enum _mcg_atms_select
{
    kMcgAtmsSelect32k,              /* 32 kHz Internal Reference Clock selected */
    kMcgAtmsSelect4m                /* 4 MHz Internal Reference Clock selected */
} mcg_atms_select_t;

/*! @brief MCG OSC Clock Select */
typedef enum _mcg_oscsel_select
{
    kMcgOscselOsc,                  /* Selects System Oscillator (OSCCLK) */
    kMcgOscselRtc,                  /* Selects 32 kHz RTC Oscillator */
    kMcgOscselIrc                   /* Selects 48 MkHz IRC Oscillator */
} mcg_oscsel_select_t;

/*! @brief MCG loss of clock status */
typedef enum _mcg_locs1_status
{
    kMcgLocs1NotOccured,            /* Loss of RTC has not occurred */
    kMcgLocs1Occured                /* Loss of RTC has occurred */
} mcg_locs1_status_t;

/*! @brief MCG PLLCS select */
typedef enum _mcg_pllcs_select
{
    kMcgPllcsSelectPll0,            /* PLL0 output clock is selected */
    kMcgPllcsSelectPll1,            /* PLL1 output clock is selected */
} mcg_pllcs_select_t;

/*! @brief MCG loss of clock status */
typedef enum _mcg_locs2_status
{
    kMcgLocs2NotOccured,            /* Loss of OSC1 has not occurred */
    kMcgLocs2Occured                /* Loss of OSC1 has occurred */
} mcg_locs2_status_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name MCG out clock access API*/
/*@{*/

/*!
 * @brief Gets the current MCG FLL clock.
 *
 * This function  returns the mcgfllclk value in frequency(Hertz) based on the
 * current MCG configurations and settings. FLL should be properly configured
 * in order to get the valid value.
 *
 * @param none
 * @return value Frequency value in Hertz of the mcgpllclk.
 */
uint32_t clock_hal_get_fllclk(void);

/*!
 * @brief Gets the current MCG PLL/PLL0 clock.
 *
 * This function  returns the mcgpllclk/mcgpll0 value in frequency(Hertz) based
 * on the current MCG configurations and settings. PLL/PLL0 should be properly 
 * configured in order to get the valid value.
 *
 * @param none
 * @return value Frequency value in Hertz of the mcgpllclk or the mcgpll0clk.
 */
uint32_t clock_hal_get_pll0clk(void);

#if FSL_FEATURE_MCG_HAS_PLL1
/*!
 * @brief Gets the current MCG PLL1 clock.
 *
 * This function  returns the mcgpll1clk value in frequency (Hertz) based
 * on the current MCG configurations and settings. PLL1 should be properly configured
 * in order to get the valid value.
 *
 * @param none
 * @return value Frequency value in Hertz of mcgpll1clk.
 */
uint32_t clock_hal_get_pll1clk(void);
#endif

/*!
 * @brief Gets the current MCG IR clock.
 *
 * This function  returns the mcgirclk value in frequency (Hertz) based
 * on the current MCG configurations and settings. It does not check if the 
 * mcgirclk is enabled or not, just calculate and return the value.
 *
 * @param none
 * @return value Frequency value in Hertz of the mcgirclk. 
 */
uint32_t clock_hal_get_irclk(void);

/*!
 * @brief Gets the current MCG out clock.
 *
 * This function  returns the mcgoutclk value in frequency (Hertz) based on the
 * current MCG configurations and settings. The configuration should be 
 * properly done in order to get the valid value.
 *
 * @param none
 * @return value Frequency value in Hertz of mcgoutclk.
 */
uint32_t clock_hal_get_outclk(void);

/*@}*/

/*! @name MCG control register access API*/
/*@{*/

/*!
 * @brief Sets the Clock Source Select 
 *
 * This function  selects the clock source for the MCGOUTCLK.
 *
 * @param select    Clock source selection
 *                  - 00: Output of FLL or PLLCS is selected(depends on PLLS control bit)
 *                  - 01: Internal reference clock is selected.
 *                  - 10: External reference clock is selected.
 *                  - 11: Reserved.
 */
static inline void clock_set_clks(mcg_clock_select_t select)
{
    BW_MCG_C1_CLKS(select);
}

/*!
 * @brief Gets the Clock Source Select.
 *
 * This function  gets the select of the clock source for the MCGOUTCLK.
 *
 * @return select    Clock source selection
 */
static inline mcg_clock_select_t clock_get_clks(void)
{
    return (mcg_clock_select_t)BR_MCG_C1_CLKS;
}

/*!
 * @brief Sets the FLL External Reference Divider.
 *
 * This function  sets the FLL External Reference Divider.
 *
 * @param setting   Divider setting
 */
static inline void clock_set_frdiv(uint8_t setting)
{
    BW_MCG_C1_FRDIV(setting);
}

/*!
 * @brief Gets the FLL External Reference Divider.
 *
 * This function  gets the FLL External Reference Divider.
 *
 * @return setting  Divider setting
 */
static inline uint8_t clock_get_frdiv(void)
{
    return BR_MCG_C1_FRDIV;
}

/*!
 * @brief Sets the Internal Reference Select. 
 *
 * This function  selects the reference clock source for the FLL.
 *
 * @param select   Clock source select
 *                 - 0: External reference clock is selected
 *                 - 1: The slow internal reference clock is selected
 */
static inline void clock_set_irefs(mcg_iref_clock_source_t select)
{
    BW_MCG_C1_IREFS(select);
}

/*!
 * @brief Gets the Internal Reference Select  
 *
 * This function  gets the reference clock source for the FLL.
 *
 * @return select  Clock source select
 */
static inline mcg_iref_clock_source_t clock_get_irefs(void)
{
    return (mcg_iref_clock_source_t)BR_MCG_C1_IREFS;
}

/*!
 * @brief Sets the CLKS, FRDIV and IREFS at the same time.
 *
 * This function  sets the CLKS, FRDIV, and IREFS settings at the same time
 * in order keep the integrity of the clock switching.
 *
 * @param clks    Clock source select
 * @param frdiv   FLL external reference divider select
 * @param irefs   Internal reference select
 */
static inline void clock_set_clks_frdiv_irefs(mcg_clock_select_t clks,
                                              uint8_t frdiv,
                                              mcg_iref_clock_source_t irefs)
{
    /* Set the required CLKS , FRDIV  and IREFS values */
    HW_MCG_C1_WR((HW_MCG_C1_RD() & ~(BM_MCG_C1_CLKS | BM_MCG_C1_FRDIV | BM_MCG_C1_IREFS)) 
                  | (BF_MCG_C1_CLKS(clks) | BF_MCG_C1_FRDIV(frdiv) | BF_MCG_C1_IREFS(irefs)));
}

/*!
 * @brief Sets the Enable Internal Reference Clock setting.
 *
 * This function  enables/disables the internal reference clock to use as the MCGIRCLK.
 *
 * @params enable Enable or disable internal reference clock.
 *                 - true: MCGIRCLK active
 *                 - false: MCGIRCLK inactive
 */
static inline void clock_set_irclken(bool enable)
{
    BW_MCG_C1_IRCLKEN(enable ? 1 : 0);
}

/*!
 * @brief Gets the enable Internal Reference Clock setting.
 *
 * This function  gets the reference clock enable setting.
 *
 * @return enabled  True if the internal reference clock is enabled.
 */
static inline bool clock_get_irclken(void)
{
    return BR_MCG_C1_IRCLKEN;
}

/*!
 * @brief Sets the Internal Reference Clock Stop Enable setting.
 *
 * This function  controls whether or not the internal reference clock remains 
 * enabled when the MCG enters Stop mode.
 *
 * @params enable Enable or disable the internal reference clock stop setting.
 *                 - true: Internal reference clock is enabled in Stop mode if IRCLKEN is set
                        or if MCG is in FEI, FBI, or BLPI modes before entering Stop mode.
 *                 - false: Internal reference clock is disabled in Stop mode
 */
static inline void clock_set_irefsten(bool enable)
{
    BW_MCG_C1_IREFSTEN(enable ? 1 : 0);
}

/*!
 * @brief Gets the Enable Internal Reference Clock setting.
 *
 * This function  gets the Internal Reference Clock Stop Enable setting.
 *
 * @return enabled  True if internal reference clock stop is enabled.
 */
static inline bool clock_get_irefsten(void)
{
    return BR_MCG_C1_IREFSTEN;
}

/*!
 * @brief Sets the Loss of Clock Reset Enable setting.
 *
 * This function  determines whether an interrupt or a reset request is made following a loss
 *  of the OSC0 external reference clock. The LOCRE0 only has an affect when CME0 is set.
 *
 * @params enable Loss of Clock Reset Enable setting
 *                 - true: Generate a reset request on a loss of OSC0 external reference clock
 *                 - false: Interrupt request is generated on a loss of OSC0 external reference clock
 */
static inline void clock_set_locre0(bool enable)
{
    BW_MCG_C2_LOCRE0(enable ? 1 : 0);
}

/*!
 * @brief Gets the Loss of Clock Reset Enable setting.
 *
 * This function  gets the Loss of Clock Reset Enable setting.
 *
 * @return enabled  True if Loss of Clock Reset is enabled.
 */
static inline bool clock_get_locre0(void)
{
    return BR_MCG_C2_LOCRE0;
}

#if FSL_FEATURE_MCG_HAS_FCFTRIM
/*!
 * @brief Sets the Fast Internal Reference Clock Fine Trim setting.
 *
 * This function  sets the Fast Internal Reference Clock Fine Trim setting. FCFTRIM
 * controls the smallest adjustment of the fast internal reference clock frequency. 
 * Setting the FCFTRIM increases the period and clearing FCFTRIM decreases the period 
 * by the smallest amount possible. If an FCFTRIM value is stored and non-volatile 
 * memory is to be used, it is the user's responsibility to copy that value from the 
 * non-volatile memory location to this bit.
 *
 * @params setting Fast Internal Reference Clock Fine Trim setting
 */
static inline void clock_set_fcftrim(uint8_t setting)
{
    BW_MCG_C2_FCFTRIM(setting);
}

/*!
 * @brief Gets the Fast Internal Reference Clock Fine Trim setting.
 *
 * This function  gets the Fast Internal Reference Clock Fine Trim setting.
 *
 * @return setting  Fast Internal Reference Clock Fine Trim setting
 */
static inline uint8_t clock_get_fcftrim(void)
{
    return BR_MCG_C2_FCFTRIM;
}
#endif /* FSL_FEATURE_MCG_HAS_FCFTRIM */

/*!
 * @brief Sets the Frequency Range Select.
 *
 * This function  selects the frequency range for the crystal oscillator or an external
 * clock source. See the Oscillator (OSC) chapter for more details and the device 
 * data sheet for the frequency ranges used.
 *
 * @params select  Frequency Range Select
 *                 - 00: Low frequency range selected for the crystal oscillator
 *                 - 01: High frequency range selected for the crystal oscillator
 *                 - 1X: Very high frequency range selected for the crystal oscillator
 */
static inline void clock_set_range0(mcg_freq_range_select_t select)
{
    BW_MCG_C2_RANGE(select);
}

/*!
 * @brief Gets the Frequency Range Select.
 *
 * This function  gets the Frequency Range Select.
 *
 * @return select  Frequency Range Select
 */
static inline mcg_freq_range_select_t clock_get_range0(void)
{
    return (mcg_freq_range_select_t)BR_MCG_C2_RANGE;
}

/*!
 * @brief Sets the High Gain Oscillator Select.
 *
 * This function  controls the crystal oscillator mode of operation. See the
 * Oscillator (OSC) chapter for more details.
 *
 * @params select  High Gain Oscillator Select.
 *                 - 0: Configure crystal oscillator for low-power operation
 *                 - 1: Configure crystal oscillator for high-gain operation
 */
static inline void clock_set_hgo0(mcg_hgo_select_t select)
{
    BW_MCG_C2_HGO(select);
}

/*!
 * @brief Gets the High Gain Oscillator Select.
 *
 * This function  gets the High Gain Oscillator Select.
 *
 * @return select  High Gain Oscillator Select
 */
static inline mcg_hgo_select_t clock_get_hgo0(void)
{
    return (mcg_hgo_select_t)BR_MCG_C2_HGO;
}

/*!
 * @brief Sets the External Reference Select.
 *
 * This function  selects the source for the external reference clock. 
 * See the Oscillator (OSC) chapter for more details.
 *
 * @params select  External Reference Select
 *                 - 0: External reference clock requested
 *                 - 1: Oscillator requested
 */
static inline void clock_set_erefs0(mcg_eref_clock_select_t select)
{
    BW_MCG_C2_EREFS(select);
}

/*!
 * @brief Gets the External Reference Select.
 *
 * This function  gets the External Reference Select.
 *
 * @return select  External Reference Select
 */
static inline mcg_eref_clock_select_t clock_get_erefs0(void)
{
    return (mcg_eref_clock_select_t)BR_MCG_C2_EREFS;
}

/*!
 * @brief Sets the Low Power Select.
 *
 * This function  controls whether the FLL (or PLL) is disabled in the BLPI and the 
 * BLPE modes. In the FBE or the PBE modes, setting this bit to 1  transitions the MCG
 * into the BLPE mode; in the FBI mode, setting this bit to 1  transitions the MCG into
 * the BLPI mode. In any other MCG mode, the LP bit has no affect..
 *
 * @params select  Low Power Select
 *                 - 0: FLL (or PLL) is not disabled in bypass modes
 *                 - 1: FLL (or PLL) is disabled in bypass modes (lower power)
 */
static inline void clock_set_lp(mcg_lp_select_t select)
{
    BW_MCG_C2_LP(select);
}

/*!
 * @brief Gets the Low Power Select.
 *
 * This function  gets the Low Power Select.
 *
 * @return select  Low Power Select
 */
static inline mcg_lp_select_t clock_get_lp(void)
{
    return (mcg_lp_select_t)BR_MCG_C2_LP;
}

/*!
 * @brief Sets the Internal Reference Clock Select.
 *
 * This function  selects between the fast or slow internal reference clock source.
 *
 * @params select  Low Power Select
 *                 - 0: Slow internal reference clock selected.
 *                 - 1: Fast internal reference clock selected.
 */
static inline void clock_set_ircs(mcg_iref_clock_select_t select)
{
    BW_MCG_C2_IRCS(select);
}

/*!
 * @brief Gets the Internal Reference Clock Select.
 *
 * This function  gets the Internal Reference Clock Select.
 *
 * @return select  Internal Reference Clock Select
 */
static inline mcg_iref_clock_select_t clock_get_ircs(void)
{
    return (mcg_iref_clock_select_t)BR_MCG_C2_IRCS;
}

/*!
 * @brief Sets the Slow Internal Reference Clock Trim Setting.
 *
 * This function  controls the slow internal reference clock frequency by 
 * controlling the slow internal reference clock period. The SCTRIM bits are
 * binary weighted (that is, bit 1 adjusts twice as much as bit 0).
 * Increasing the binary value increases the period, and decreasing the value
 * decreases the period.
 * An additional fine trim bit is available in the C4 register as the SCFTRIM bit.
 * Upon reset, this value is loaded with a factory trim value.
 * If an SCTRIM value stored in non-volatile memory is to be used, it is the user's 
 * responsibility to copy that value from the non-volatile memory location to 
 * this register.
 *
 * @params setting  Slow Internal Reference Clock Trim Setting
 */
static inline void clock_set_sctrim(uint8_t setting)
{
    BW_MCG_C3_SCTRIM(setting);
}

/*!
 * @brief Gets the Slow Internal Reference Clock Trim Setting.
 *
 * This function  gets the Slow Internal Reference Clock Trim Setting.
 *
 * @return setting  Slow Internal Reference Clock Trim Setting
 */
static inline uint8_t clock_get_sctrim(void)
{
    return BR_MCG_C3_SCTRIM;
}

/*!
 * @brief Sets the DCO Maximum Frequency with 32.768 kHz Reference.
 *
 * This function  controls whether or not the DCO frequency range 
 * is narrowed to its maximum frequency with a 32.768 kHz reference.
 *
 * @params setting  DCO Maximum Frequency with 32.768 kHz Reference Setting
 *                  - 0: DCO has a default range of 25%.
 *                  - 1: DCO is fine-tuned for maximum frequency with 32.768 kHz reference.
 */
static inline void clock_set_dmx32(mcg_dmx32_select_t setting)
{
    BW_MCG_C4_DMX32(setting);
}

/*!
 * @brief Gets the DCO Maximum Frequency with the 32.768 kHz Reference Setting.
 *
 * This function  gets the DCO Maximum Frequency with 32.768 kHz Reference Setting.
 *
 * @return setting  DCO Maximum Frequency with 32.768 kHz Reference Setting
 */
static inline mcg_dmx32_select_t clock_get_dmx32(void)
{
    return (mcg_dmx32_select_t)BR_MCG_C4_DMX32;
}

/*!
 * @brief Sets the DCO Range Select.
 *
 * This function  selects the frequency range for the FLL output, DCOOUT.
 * When the LP bit is set, the writes to the DRS bits are ignored. The DRST read
 * field indicates the current frequency range for the DCOOUT. The DRST field does
 * not update immediately after a write to the DRS field due to internal 
 * synchronization between the clock domains. See the DCO Frequency Range table
 *  for more details.
 *
 * @params setting  DCO Range Select Setting
 *                  - 00: Low range (reset default).
 *                  - 01: Mid range.
 *                  - 10: Mid-high range.
 *                  - 11: High range.
 */
static inline void clock_set_drst_drs(mcg_dco_range_select_t setting)
{
    BW_MCG_C4_DRST_DRS(setting);
}

/*!
 * @brief Gets the DCO Range Select Setting.
 *
 * This function  gets the DCO Range Select Setting.
 *
 * @return setting  DCO Range Select Setting
 */
static inline mcg_dco_range_select_t clock_get_drst_drs(void)
{
    return (mcg_dco_range_select_t)BR_MCG_C4_DRST_DRS;
}

/*!
 * @brief Sets the Fast Internal Reference Clock Trim Setting.
 *
 * This function  controls the fast internal reference clock frequency
 * by controlling the fast internal reference clock period. The FCTRIM 
 * bits are binary weighted (that is, bit 1 adjusts twice as much as bit 0).
 * Increasing the binary value increases the period, and decreasing the 
 * value decreases the period. 
 * If an FCTRIM[3:0] value stored in non-volatile memory is to be used, it is
 * the user's responsibility to copy that value from the non-volatile memory location
 * to this register.
 *
 * @params setting  Fast Internal Reference Clock Trim Setting.
 */
static inline void clock_set_fctrim(uint8_t setting)
{
    BW_MCG_C4_FCTRIM(setting);
}

/*!
 * @brief Gets the Fast Internal Reference Clock Trim Setting.
 *
 * This function  gets the Fast Internal Reference Clock Trim Setting.
 *
 * @return setting  Fast Internal Reference Clock Trim Setting
 */
static inline uint8_t clock_get_fctrim(void)
{
    return BR_MCG_C4_FCTRIM;
}

/*!
 * @brief Sets the Slow Internal Reference Clock Fine Trim Setting.
 *
 * This function  controls the smallest adjustment of the slow internal
 * reference clock frequency. Setting the SCFTRIM increases the period and 
 * clearing the SCFTRIM decreases the period by the smallest amount possible.
 * If an SCFTRIM value, stored in non-volatile memory, is to be used, it is 
 * the user's responsibility to copy that value from the non-volatile memory 
 * location to this bit.
 *
 * @params setting  Slow Internal Reference Clock Fine Trim Setting
 */
static inline void clock_set_scftrim(uint8_t setting)
{
    BW_MCG_C4_SCFTRIM(setting);
}

/*!
 * @brief Gets the Slow Internal Reference Clock Fine Trim Setting.
 *
 * This function  gets the Slow Internal Reference Clock Fine Trim Setting.
 *
 * @return setting  Slow Internal Reference Clock Fine Trim Setting
 */
static inline uint8_t clock_get_scftrim(void)
{
    return BR_MCG_C4_SCFTRIM;
}

#if FSL_FEATURE_MCG_USE_PLLREFSEL
/*!
 * @brief Sets the PLL0 External Reference Select Setting.
 *
 * This function  selects the PLL0 external reference clock source.
 *
 * @params setting  PLL0 External Reference Select Setting
 *                  - 0: Selects OSC0 clock source as its external reference clock
 *                  - 1: Selects OSC1 clock source as its external reference clock
 */
static inline void clock_set_pllrefsel0(mcg_pll_eref_clock_select_t setting)
{
    BW_MCG_C5_PLLREFSEL0(setting);
}

/*!
 * @brief Gets the PLL0 External Reference Select Setting.
 *
 * This function  gets the PLL0 External Reference Select Setting.
 *
 * @return setting  PLL0 External Reference Select Setting
 */
static inline mcg_pll_eref_clock_select_t clock_get_pllrefsel0(void)
{
    return (mcg_pll_eref_clock_select_t)BR_MCG_C5_PLLREFSEL0;
}
#endif /* FSL_FEATURE_MCG_USE_PLLREFSEL */

/*!
 * @brief Sets the PLL Clock Enable Setting.
 *
 * This function  enables/disables the PLL0 independent of the PLLS and enables the PLL0
 * clock to use as the MCGPLL0CLK and the MCGPLL0CLK2X. (PRDIV0 needs to be programmed to
 * the correct divider to generate a PLL1 reference clock in a valid reference range
 * prior to setting the PLLCLKEN0 bit). Setting PLLCLKEN0  enables the external 
 * oscillator selected by REFSEL if not already enabled. Whenever the PLL0 is being
 * enabled with the PLLCLKEN0 bit, and the external oscillator is being used
 * as the reference clock, the OSCINIT 0 bit should be checked to make sure it is set.
 *
 * @params enable  PLL Clock Enable Setting
 *                 - true: MCGPLL0CLK and MCGPLL0CLK2X are active
 *                 - false: MCGPLL0CLK and MCGPLL0CLK2X are inactive
 */
static inline void clock_set_pllclken0(bool enable)
{
    BW_MCG_C5_PLLCLKEN0(enable ? 1 : 0);
}

/*!
 * @brief Gets the PLL Clock Enable Setting.
 *
 * This function  gets the PLL Clock Enable Setting.
 *
 * @return enabled True if PLL0 PLL Clock is enabled.
 */
static inline bool clock_get_pllclken0(void)
{
    return BR_MCG_C5_PLLCLKEN0;
}

/*!
 * @brief Sets the PLL0 Stop Enable Setting.
 *
 * This function  enables/disables the PLL0 Clock during a Normal Stop (In Low
 * Power Stop mode, the PLL0 clock gets disabled even if PLLSTEN0=1). In all other
 * power modes, the PLLSTEN0 bit has no affect and does not enable the PLL0 Clock 
 * to run if it is written to 1.
 *
 * @params enable   PLL0 Stop Enable Setting
 *                  - true: MCGPLL0CLK and MCGPLL0CLK2X are enabled if system is in
 *                       Normal Stop mode.
 *                  - false: MCGPLL0CLK and MCGPLL0CLK2X are disabled in any of the 
 *                       Stop modes.
 */
static inline void clock_set_pllsten0(bool enable)
{
    BW_MCG_C5_PLLSTEN0(enable ? 1 : 0);
}

/*!
 * @brief Gets the PLL0 Stop Enable Setting.
 *
 * This function  gets the PLL0 Stop Enable Setting.
 *
 * @return enabled  True if the PLL0 Stop is enabled.
 */
static inline bool clock_get_pllsten0(void)
{
    return BR_MCG_C5_PLLSTEN0;
}

/*!
 * @brief Sets the PLL0 External Reference Divider Setting.
 *
 * This function  selects the amount to divide down the external reference
 * clock for the PLL0. The resulting frequency must be in a valid reference 
 * range. After the PLL0 is enabled, (by setting either PLLCLKEN0 or PLLS), the
 * PRDIV0 value must not be changed when LOCK0 is zero.
 *
 * @params setting  PLL0 External Reference Divider Setting
 */
static inline void clock_set_prdiv0(uint8_t setting)
{
    BW_MCG_C5_PRDIV0(setting);
}

/*!
 * @brief Gets the PLL0 External Reference Divider Setting.
 *
 * This function  gets the PLL0 External Reference Divider Setting.
 *
 * @return setting  PLL0 External Reference Divider Setting
 */
static inline uint8_t clock_get_prdiv0(void)
{
    return BR_MCG_C5_PRDIV0;
}

/*!
 * @brief Sets the Loss of Lock Interrupt Enable Setting.
 *
 * This function  determine whether an interrupt request is made following a loss
 * of lock indication. This bit only has an effect when LOLS 0 is set.
 *
 * @params enable  Loss of Lock Interrupt Enable Setting
 *                 - true: Generate an interrupt request on loss of lock.
 *                 - false: No interrupt request is generated on loss of lock.
 */
static inline void clock_set_lolie0(bool enable)
{
    BW_MCG_C6_LOLIE0(enable ? 1 : 0);
}

/*!
 * @brief Gets the Loss of the Lock Interrupt Enable Setting.
 *
 * This function  gets the Loss of the Lock Interrupt Enable Setting.
 *
 * @return enabled  True if the Loss of Lock Interrupt is enabled.
 */
static inline bool clock_get_lolie0(void)
{
    return BR_MCG_C6_LOLIE0;
}

/*!
 * @brief Sets the PLL Select Setting.
 *
 * This function  controls whether the PLLCS or FLL output is selected as the
 * MCG source when CLKS[1:0]=00. If the PLLS bit is cleared and PLLCLKEN0 and 
 * PLLCLKEN1 is not set, the PLLCS output clock is disabled in all modes. If the
 * PLLS is set, the FLL is disabled in all modes.
 *
 * @params setting  PLL Select Setting
 *                  - 0: FLL is selected.
 *                  - 1: PLLCS output clock is selected (PRDIV0 bits of PLL in 
 *                       control need to be programmed to the correct divider to
 *                       generate a PLL reference clock in the range of 1 - 32 MHz 
 *                       prior to setting the PLLS bit).
 */
static inline void clock_set_plls(mcg_pll_select_t setting)
{
    BW_MCG_C6_PLLS(setting);
}

/*!
 * @brief Gets the PLL Select Setting.
 *
 * This function  gets the PLL Select Setting.
 *
 * @return setting  PLL Select Setting
 */
static inline mcg_pll_select_t clock_get_plls(void)
{
    return (mcg_pll_select_t)BR_MCG_C6_PLLS;
}

/*!
 * @brief Sets the Clock Monitor Enable Setting.
 *
 * This function  enables/disables the loss of clock monitoring circuit for 
 * the OSC0 external reference mux select. The LOCRE0 bit  determines whether an 
 * interrupt or a reset request is generated following a loss of the OSC0 indication.
 * The CME0 bit should only be set to a logic 1 when the MCG is in an operational
 * mode that uses the external clock (FEE, FBE, PEE, PBE, or BLPE). Whenever the
 * CME0 bit is set to a logic 1, the value of the RANGE0 bits in the C2 register
 * should not be changed. CME0 bit should be set to a logic 0 before the MCG 
 * enters any Stop mode. Otherwise, a reset request may occur while in Stop mode. 
 * CME0 should also be set to a logic 0 before entering VLPR or VLPW power modes 
 * if the MCG is in BLPE mode.
 *
 * @params enable  Clock Monitor Enable Setting
 *                 - true: External clock monitor is enabled for OSC0.
 *                 - false: External clock monitor is disabled for OSC0.
 */
static inline void clock_set_cme0(bool enable)
{
    BW_MCG_C6_CME0(enable ? 1 : 0);
}

/*!
 * @brief Gets the Clock Monitor Enable Setting.
 *
 * This function  gets the Clock Monitor Enable Setting.
 *
 * @return enabled  True if Clock Monitor is enabled
 */
static inline bool clock_get_cme0(void)
{
    return BR_MCG_C6_CME0;
}

/*!
 * @brief Sets the VCO0 Divider Setting.
 *
 * This function  selects the amount to divide the VCO output of the PLL0. 
 * The VDIV0 bits establish the multiplication factor (M) applied to the 
 * reference clock frequency. After the PLL0 is enabled (by setting either
 * PLLCLKEN0 or PLLS), the VDIV0 value must not be changed when LOCK0 is zero.
 *
 * @params setting  VCO0 Divider Setting
 */
static inline void clock_set_vdiv0(uint8_t setting)
{
    BW_MCG_C6_VDIV0(setting);
}

/*!
 * @brief Gets the VCO0 Divider Setting.
 *
 * This function  gets the VCO0 Divider Setting.
 *
 * @return setting  VCO0 Divider Setting
 */
static inline uint8_t clock_get_vdiv0(void)
{
    return BR_MCG_C6_VDIV0;
}

/*!
 * @brief Gets the Loss of the Lock Status.
 *
 * This function  gets the Loss of Lock Status. This bit is a sticky bit indicating
 * the lock status for the PLL. LOLS 0 is set if after acquiring lock, the PLL 
 * output frequency has fallen outside the lock exit frequency tolerance, D unl . 
 * LOLIE 0 determines whether an interrupt request is made when LOLS 0 is set. 
 * This bit is cleared by reset or by writing a logic 1 to it when set. Writing a
 * logic 0 to this bit has no effect.
 *
 * @return status  Loss of Lock Status
 *                 - 0: PLL has not lost lock since LOLS 0 was last cleared
 *                 - 1: PLL has lost lock since LOLS 0 was last cleared
 */
static inline mcg_lols_status_t clock_get_lols0(void)
{
    return (mcg_lols_status_t)BR_MCG_S_LOLS0;
}

/*!
 * @brief Gets the Lock Status.
 *
 * This function  gets the Lock Status. This bit indicates whether the PLL0 has 
 * acquired the lock. Lock detection is disabled when not operating in either the PBE or the
 * PEE mode unless PLLCLKEN0=1 and the MCG is not configured in the BLPI or the BLPE mode.
 * While the PLL0 clock is locking to the desired frequency, MCGPLL0CLK and 
 * MCGPLL0CLK2X are  gated off until the LOCK0 bit gets asserted. If the lock
 * status bit is set, changing the value of the PRDIV0[2:0] bits in the C5 register
 * or the VDIV0[4:0] bits in the C6 register causes the lock status bit to clear 
 * and stay cleared until the PLL0 has reacquired the lock. The loss of the PLL0 reference 
 * clock  also causes the LOCK0 bit to clear until the PLL0 has an entry into the LLS, 
 * VLPS, or a regular Stop with PLLSTEN0=0 also causes the lock status bit to clear
 * and stay cleared until the stop mode is exited and the PLL0 has reacquired the lock.
 * Any time the PLL0 is enabled and the LOCK0 bit is cleared, the MCGPLL0CLK and
 * MCGPLL0CLK2X are  gated off until the LOCK0 bit is reasserted.
 *
 * @return status  Lock Status
 *                 - 0: PLL is currently unlocked
 *                 - 1: PLL is currently locked
 */
static inline mcg_lock_status_t clock_get_lock0(void)
{
    return (mcg_lock_status_t)BR_MCG_S_LOCK0;
}

/*!
 * @brief Gets the PLL Select Status.
 *
 * This function  gets the PLL Select Status. This bit indicates the clock source
 * selected by PLLS . The PLLST bit does not update immediately after a write to
 * the PLLS bit due to the internal synchronization between the clock domains.
 *
 * @return status  PLL Select Status
 *                 - 0: Source of PLLS clock is FLL clock.
 *                 - 1: Source of PLLS clock is PLLCS output clock.
 */
static inline mcg_pllst_status_t clock_get_pllst(void)
{
    return (mcg_pllst_status_t)BR_MCG_S_PLLST;
}

/*!
 * @brief Gets the Internal Reference Status.
 *
 * This function  gets the Internal Reference Status. This bit indicates the current
 * source for the FLL reference clock. The IREFST bit does not update immediately 
 * after a write to the IREFS bit due to internal synchronization between the clock 
 * domains.
 *
 * @return status  Internal Reference Status
 *                 - 0: Source of FLL reference clock is the external reference clock.
 *                 - 1: Source of FLL reference clock is the internal reference clock.
 */
static inline mcg_irefst_status_t clock_get_irefst(void)
{
    return (mcg_irefst_status_t)BR_MCG_S_IREFST;
}

/*!
 * @brief Gets the Clock Mode Status.
 *
 * This function  gets the Clock Mode Status. These bits indicate the current clock mode.
 * The CLKST bits do not update immediately after a write to the CLKS bits due to 
 * internal synchronization between clock domains.
 *
 * @return status  Clock Mode Status
 *                 - 00: Output of the FLL is selected (reset default).
 *                 - 01: Internal reference clock is selected.
 *                 - 10: External reference clock is selected.
 *                 - 11: Output of the PLL is selected.
 */
static inline mcg_clkst_status_t clock_get_clkst(void)
{
    return (mcg_clkst_status_t)BR_MCG_S_CLKST;
}

/*!
 * @brief Gets the OSC Initialization Status.
 *
 * This function  gets the OSC Initialization Status. This bit, which resets to 0, is set
 * to 1 after the initialization cycles of the crystal oscillator clock have completed. 
 * After being set, the bit is cleared to 0 if the OSC is subsequently disabled. See the
 * OSC module's detailed description for more information.
 *
 * @return status  OSC Initialization Status
 */
static inline uint8_t clock_get_oscinit0(void)
{
    return BR_MCG_S_OSCINIT0;
}

/*!
 * @brief Gets the Internal Reference Clock Status.
 *
 * This function  gets the Internal Reference Clock Status. The IRCST bit indicates the
 * current source for the internal reference clock select clock (IRCSCLK). The IRCST bit
 * does not update immediately after a write to the IRCS bit due to the internal 
 * synchronization between clock domains. The IRCST bit is only  updated if the 
 * internal reference clock is enabled, either by the MCG being in a mode that uses the
 * IRC or by setting the C1[IRCLKEN] bit.
 *
 * @return status  Internal Reference Clock Status
 *                 - 0: Source of internal reference clock is the slow clock (32 kHz IRC).
 *                 - 1: Source of internal reference clock is the fast clock (2 MHz IRC).
 */
static inline mcg_ircst_status_t clock_get_ircst(void)
{
    return (mcg_ircst_status_t)BR_MCG_S_IRCST;
}

/*!
 * @brief Gets the Automatic Trim machine Fail Flag.
 *
 * This function  gets the Automatic Trim machine Fail Flag. This Fail flag for the 
 * Automatic Trim Machine (ATM). This bit asserts when the Automatic Trim Machine is
 * enabled (ATME=1) and a write to the C1, C3, C4, and SC registers is detected or the MCG
 * enters into any Stop mode. A write to ATMF clears the flag.
 *
 * @return flag  Automatic Trim machine Fail Flag
 *                 - 0: Automatic Trim Machine completed normally.
 *                 - 1: Automatic Trim Machine failed.
 */
static inline mcg_atmf_status_t clock_get_atmf(void)
{
    return (mcg_atmf_status_t)BR_MCG_SC_ATMF;
}

/*!
 * @brief Sets the Automatic Trim machine Fail Flag.
 *
 * This function  clears the ATMF flag.
 */
static inline void clock_set_atmf(void)
{
    BW_MCG_SC_ATMF(1);
}

/*!
 * @brief Gets the OSC0 Loss of Clock Status.
 *
 * This function  gets the OSC0 Loss of Clock Status. The LOCS0 indicates when a loss of 
 * OSC0 reference clock has occurred. The LOCS0 bit only has an effect when CME0 is set. 
 * This bit is cleared by writing a logic 1 to it when set.
 *
 * @return status  OSC0 Loss of Clock Status
 *                 - 0: Loss of OSC0 has not occurred.
 *                 - 1: Loss of OSC0 has occurred.
 */
static inline mcg_locs0_status_t clock_get_locs0(void)
{
    return (mcg_locs0_status_t)BR_MCG_SC_LOCS0;
}

/*!
 * @brief Sets the Automatic Trim Machine Enable Setting.
 *
 * This function  enables/disables the Auto Trim Machine to start automatically
 * trimming the selected Internal Reference Clock.
 * ATME de-asserts after the Auto Trim Machine has completed trimming all trim bits
 * of the IRCS clock selected by the ATMS bit.
 * Writing to C1, C3, C4, and SC registers or entering Stop mode aborts the auto 
 * trim operation and clears this bit.
 *
 * @params enable  Automatic Trim Machine Enable Setting
 *                 - true: Auto Trim Machine enabled
 *                 - false: Auto Trim Machine disabled
 */
static inline void clock_set_atme(bool enable)
{
    BW_MCG_SC_ATME(enable ? 1 : 0);
}

/*!
 * @brief Gets the Automatic Trim Machine Enable Setting.
 *
 * This function  gets the Automatic Trim Machine Enable Setting.
 *
 * @return enabled  True if Automatic Trim Machine is enabled
 */
static inline bool clock_get_atme(void)
{
    return BR_MCG_SC_ATME;
}

/*!
 * @brief Sets the Automatic Trim Machine Select Setting.
 *
 * This function  selects the IRCS clock for Auto Trim Test.
 *
 * @params setting  Automatic Trim Machine Select Setting
 *                  - 0: 32 kHz Internal Reference Clock selected
 *                  - 1: 4 MHz Internal Reference Clock selected
 */
static inline void clock_set_atms(mcg_atms_select_t setting)
{
    BW_MCG_SC_ATMS(setting);
}

/*!
 * @brief Gets the Automatic Trim Machine Select Setting.
 *
 * This function  gets the Automatic Trim Machine Select Setting.
 *
 * @return setting  Automatic Trim Machine Select Setting
 */
static inline mcg_atms_select_t clock_get_atms(void)
{
    return (mcg_atms_select_t)BR_MCG_SC_ATMS;
}

/*!
 * @brief Sets the FLL Filter Preserve Enable Setting.
 *
 * This function  sets the FLL Filter Preserve Enable. This bit  prevents the
 * FLL filter values from resetting allowing the FLL output frequency to remain the
 * same during the clock mode changes where the FLL/DCO output is still valid. 
 * (Note: This requires that the FLL reference frequency  remain the same as 
 *  the value prior to the new clock mode switch. Otherwise, the FLL filter and the frequency 
 * values  change.)
 *
 * @params enable  FLL Filter Preserve Enable Setting
 *                 - true: FLL filter and FLL frequency retain their previous values 
 *                       during new clock mode change
 *                 - false: FLL filter and FLL frequency will reset on changes to correct 
 *                       clock mode
 */
static inline void clock_set_fltprsrv(bool enable)
{
    BW_MCG_SC_FLTPRSRV(enable ? 1 : 0);
}

/*!
 * @brief Gets the FLL Filter Preserve Enable Setting.
 *
 * This function  gets the FLL Filter Preserve Enable Setting.
 *
 * @return enabled  True if FLL Filter Preserve is enabled.
 */
static inline bool clock_get_fltprsrv(void)
{
    return BR_MCG_SC_FLTPRSRV;
}

/*!
 * @brief Sets the Fast Clock Internal Reference Divider Setting.
 *
 * This function  selects the amount to divide down the fast internal reference
 * clock. The resulting frequency is  in the range 31.25 kHz to 4 MHz.
 * (Note: Changing the divider when the Fast IRC is enabled is not supported).
 *
 * @params setting  Fast Clock Internal Reference Divider Setting
 */
static inline void clock_set_fcrdiv(uint8_t setting)
{
    BW_MCG_SC_FCRDIV(setting);
}

/*!
 * @brief Gets the Fast Clock Internal Reference Divider Setting.
 *
 * This function  gets the Fast Clock Internal Reference Divider Setting.
 *
 * @return setting  Fast Clock Internal Reference Divider Setting
 */
static inline uint8_t clock_get_fcrdiv(void)
{
    return BR_MCG_SC_FCRDIV;
}

/*!
 * @brief Sets the ATM Compare Value High Setting.
 *
 * This function  sets the ATM compare value high setting. The values are used by the 
 * Auto Trim Machine to compare and adjust the Internal Reference trim values during the ATM
 * SAR conversion.
 *
 * @params setting  ATM Compare Value High Setting
 */
static inline void clock_set_atcvh(uint8_t setting)
{
    BW_MCG_ATCVH_ATCVH(setting);
}

/*!
 * @brief Gets the ATM Compare Value High Setting.
 *
 * This function  gets the ATM Compare Value High Setting.
 *
 * @return setting  ATM Compare Value High Setting
 */
static inline uint8_t clock_get_atcvh(void)
{
    return BR_MCG_ATCVH_ATCVH;
}

/*!
 * @brief Sets the ATM Compare Value Low Setting.
 *
 * This function  sets the ATM compare value low setting. The values are used by the 
 * Auto Trim Machine to compare and adjust Internal Reference trim values during the ATM
 * SAR conversion.
 *
 * @params setting  ATM Compare Value Low Setting
 */
static inline void clock_set_atcvl(uint8_t setting)
{
    BW_MCG_ATCVL_ATCVL(setting);
}

/*!
 * @brief Gets the ATM Compare Value Low Setting.
 *
 * This function  gets the ATM Compare Value Low Setting.
 *
 * @return setting  ATM Compare Value Low Setting
 */
static inline uint8_t clock_get_atcvl(void)
{
    return BR_MCG_ATCVL_ATCVL;
}

#if FSL_FEATURE_MCG_USE_OSCSEL
/*!
 * @brief Sets the MCG OSC Clock Select Setting.
 *
 * This function  selects the MCG FLL external reference clock.
 *
 * @params setting  MCG OSC Clock Select Setting
 *                  - 0: Selects System Oscillator (OSCCLK).
 *                  - 1: Selects 32 kHz RTC Oscillator.
 */
static inline void clock_set_oscsel(mcg_oscsel_select_t setting)
{
    BW_MCG_C7_OSCSEL(setting);
}

/*!
 * @brief Gets the MCG OSC Clock Select Setting.
 *
 * This function  gets the MCG OSC Clock Select Setting.
 *
 * @return setting  MCG OSC Clock Select Setting
 */
static inline mcg_oscsel_select_t clock_get_oscsel(void)
{
    return (mcg_oscsel_select_t)BR_MCG_C7_OSCSEL;
}
#endif /* FSL_FEATURE_MCG_USE_OSCSEL */

#if FSL_FEATURE_MCG_HAS_LOLRE
/*!
 * @brief Sets the PLL Loss of Lock Reset Enable Setting.
 *
 * This function  determines whether an interrupt or a reset request is made
 * following a PLL loss of lock.
 *
 * @params enable   PLL Loss of Lock Reset Enable Setting
 *                  - true: Generate a reset request on a PLL loss of lock indication.
 *                  - false: Interrupt request is generated on a PLL loss of lock
 *                       indication. The PLL loss of lock interrupt enable bit
 *                       must also be set to generate the interrupt request.
 */
static inline void clock_set_lolre(bool enable)
{
    BW_MCG_C8_LOLRE(enable ? 1 : 0);
}

/*!
 * @brief Gets the PLL Loss of Lock Reset Enable Setting.
 *
 * This function  gets the PLL Loss of Lock Reset Enable Setting.
 *
 * @return enabled  True if the PLL Loss of Lock Reset is enabled.
 */
static inline bool clock_get_lolre(void)
{
    return BR_MCG_C8_LOLRE;
}
#endif /* FSL_FEATURE_MCG_HAS_LOLRE */


#if FSL_FEATURE_MCG_HAS_RTC_32K
/*!
 * @brief Sets the Loss of Clock Reset Enable Setting.
 *
 * This function  determines whether an interrupt or a reset request is made following
 * a loss of the RTC external reference clock. The LOCRE1 only has an affect when CME1 
 * is set.
 *
 * @params enable   Loss of Clock Reset Enable Setting
 *                  - true: Generate a reset request on a loss of RTC external reference clock.
 *                  - false: Interrupt request is generated on a loss of RTC external 
 *                       reference clock.
 */
static inline void clock_set_locre1(bool enable)
{
    BW_MCG_C8_LOCRE1(enable ? 1 : 0);
}

/*!
 * @brief Gets the Loss of Clock Reset Enable Setting.
 *
 * This function  gets the Loss of Clock Reset Enable Setting.
 *
 * @return enabled  True if Loss of Clock Reset is enabled.
 */
static inline bool clock_get_locre1(void)
{
    return BR_MCG_C8_LOCRE1;
}

/*!
 * @brief Sets the Clock Monitor Enable1 Setting.
 *
 * This function  enables/disables the loss of the clock monitoring circuit for the
 * output of the RTC external reference clock. The LOCRE1 bit  determines whether an
 * interrupt or a reset request is generated following a loss of the RTC clock indication.
 * The CME1 bit should only be set to a logic 1 when the MCG is in an operational mode
 * that uses the external clock (FEE, FBE, PEE, PBE, or BLPE). CME1 bit must be set to
 * a logic 0 before the MCG enters any Stop mode. Otherwise, a reset request may occur 
 * while in Stop mode. CME1 should also be set to a logic 0 before entering VLPR or 
 * VLPW power modes if the MCG is in BLPE mode.
 *
 * @params enable   Clock Monitor Enable1 Setting
 *                  - true: External clock monitor is enabled for RTC clock.
 *                  - false: External clock monitor is disabled for RTC clock.
 */
static inline void clock_set_cme1(bool enable)
{
    BW_MCG_C8_CME1(enable ? 1 : 0);
}

/*!
 * @brief Gets the Clock Monitor Enable1 Setting.
 *
 * This function  gets the Clock Monitor Enable1 Setting.
 *
 * @return enabled True if Clock Monitor Enable1 is enabled
 */
static inline bool clock_get_cme1(void)
{
    return BR_MCG_C8_CME1;
}

/*!
 * @brief Gets the RTC Loss of Clock Status.
 *
 * This function  gets the RTC Loss of Clock Status. This bit indicates when a loss
 * of clock has occurred. This bit is cleared by writing a logic 1 to it when set.
 *
 * @return status  RTC Loss of Clock Status
 *                 - 0: Loss of RTC has not occurred
 *                 - 1: Loss of RTC has occurred
 */
static inline mcg_locs1_status_t clock_get_locs1(void)
{
    return (mcg_locs1_status_t)BR_MCG_C8_LOCS1;
}
#endif /* FSL_FEATURE_MCG_HAS_RTC_32K */

#if FSL_FEATURE_MCG_USE_PLLREFSEL
/*!
 * @brief Sets the OSC1 Loss of Clock Reset Enable Setting.
 *
 * This function  determines whether an interrupt or reset request is made following
 * a loss of OSC1 external reference clock. The LOCRE2 only has an affect when 
 * LOCS2 is set.
 *
 * @params enable   OSC1 Loss of Clock Reset Enable Setting
 *                  - true: Reset request is generated on a loss of OSC1 external 
 *                       reference clock..
 *                  - false: Interrupt request is generated on a loss of OSC1 external 
 *                       reference clock.
 */
static inline void clock_set_locre2(bool enable)
{
    BW_MCG_C10_LOCRE2(enable ? 1 : 0);
}

/*!
 * @brief Gets the OSC1 Loss of the Clock Reset Enable Setting.
 *
 * This function  gets the OSC1 Loss of Clock Reset Enable Setting.
 *
 * @return enabled  True if OSC1 Loss of Clock Reset is enabled.
 */
static inline bool clock_get_locre2(void)
{
    return BR_MCG_C10_LOCRE2;
}

/*!
 * @brief Sets the Frequency Range1 Select Setting.
 *
 * This function  selects the frequency range for the OSC1 crystal oscillator
 * or an external clock source. See the Oscillator chapter for more details and
 * the device data sheet for the frequency ranges used.
 *
 * @params setting  Frequency Range1 Select Setting
 *                  - 00: Low frequency range selected for the crystal oscillator.
 *                  - 01: High frequency range selected for the crystal oscillator.
 *                  - 1X: Very high frequency range selected for the crystal oscillator.
 */
static inline void clock_set_range1(mcg_freq_range_select_t setting)
{
    BW_MCG_C10_RANGE1(setting);
}

/*!
 * @brief Gets the Frequency Range1 Select Setting.
 *
 * This function  gets the Frequency Range1 Select Setting.
 *
 * @return setting  Frequency Range1 Select Setting
 */
static inline mcg_freq_range_select_t clock_get_range1(void)
{
    return (mcg_freq_range_select_t)BR_MCG_C10_RANGE1;
}

/*!
 * @brief Sets the High Gain Oscillator1 Select Setting.
 *
 * This function  controls the OSC1 crystal oscillator mode of operation.
 * See the Oscillator chapter for more details.
 *
 * @params setting  High Gain Oscillator1 Select Setting
 *                  - 0: Configure crystal oscillator for low-power operation.
 *                  - 1: Configure crystal oscillator for high-gain operation.
 */
static inline void clock_set_hgo1(mcg_hgo_select_t setting)
{
    BW_MCG_C10_HGO1(setting);
}

/*!
 * @brief Gets the High Gain Oscillator1 Select Setting.
 *
 * This function  gets the High Gain Oscillator1 Select Setting.
 *
 * @return setting  High Gain Oscillator1 Select Setting
 */
static inline mcg_hgo_select_t clock_get_hgo1(void)
{
    return (mcg_hgo_select_t)BR_MCG_C10_HGO1;
}

/*!
 * @brief Sets the External Reference Select Setting.
 *
 * This function  selects the source for the OSC1 external reference clock. 
 * See the Oscillator chapter for more details.
 *
 * @params setting  External Reference Select Setting
 *                  - 0: External reference clock requested.
 *                  - 1: Oscillator requested.
 */
static inline void clock_set_erefs1(mcg_eref_clock_select_t setting)
{
    BW_MCG_C10_EREFS1(setting);
}

/*!
 * @brief Gets the External Reference Select Setting.
 *
 * This function  gets the External Reference Select Setting.
 *
 * @return setting  External Reference Select Setting
 */
static inline mcg_eref_clock_select_t clock_get_erefs1(void)
{
    return (mcg_eref_clock_select_t)BR_MCG_C10_EREFS1;
}

/*!
 * @brief Sets the PLL1 External Reference Select Setting.
 *
 * This function  selects the PLL1 external reference clock source.
 *
 * @params setting  PLL1 External Reference Select Setting
 *                  - 0: Selects OSC0 clock source as its external reference clock.
 *                  - 1: Selects OSC1 clock source as its external reference clock.
 */
static inline void clock_set_pllrefsel1(mcg_pll_eref_clock_select_t setting)
{
    BW_MCG_C11_PLLREFSEL1(setting);
}

/*!
 * @brief Gets the PLL1 External Reference Select Setting.
 *
 * This function  gets the PLL1 External Reference Select Setting.
 *
 * @return setting  PLL1 External Reference Select Setting
 */
static inline mcg_pll_eref_clock_select_t clock_get_pllrefsel1(void)
{
    return (mcg_pll_eref_clock_select_t)BR_MCG_C11_PLLREFSEL1;
}

/*!
 * @brief Sets the PLL1 Clock Enable Setting.
 *
 * This function  enables/disables the PLL1 independent of PLLS and enables the
 * PLL clocks for use as MCGPLL1CLK, MCGPLL1CLK2X, and MCGDDRCLK2X. (PRDIV1 needs 
 * to be programmed to the correct divider to generate a PLL1 reference clock in a
 * valid reference range prior to setting the PLLCLKEN1 bit.) Setting PLLCLKEN1 
 *  enables the PLL1 selected external oscillator if not already enabled. 
 * Whenever the PLL1 is  enabled with the PLLCLKEN1 bit, and the 
 * external oscillator is  used as the reference clock, the OSCINIT1 bit should
 * be checked to make sure it is set.
 *
 * @params enable   PLL1 Clock Enable Setting
 *                  - true: MCGPLL1CLK, MCGPLL1CLK2X, and MCGDDRCLK2X are active unless
 *                       MCG is in a bypass mode with LP=1 (BLPI or BLPE).
 *                  - false: MCGPLL1CLK, MCGPLL1CLK2X, and MCGDDRCLK2X are inactive.
 */
static inline void clock_set_pllclken1(bool enable)
{
    BW_MCG_C11_PLLCLKEN1(enable ? 1 : 0);
}

/*!
 * @brief Gets the PLL1 Clock Enable Setting.
 *
 * This function  gets the PLL1 Clock Enable Setting.
 *
 * @return enabled  True if the PLL1 Clock is enabled.
 */
static inline bool clock_get_pllclken1(void)
{
    return BR_MCG_C11_PLLCLKEN1;
}

/*!
 * @brief Sets the PLL1 Stop Enable Setting.
 *
 * This function  enables/disables the PLL1 Clock during the Normal Stop (In Low
 * Power Stop modes, the PLL1 clock gets disabled even if PLLSTEN1=1. In all other 
 * power modes, PLLSTEN1 bit has no affect and does not enable the PLL1 Clock to
 * run if it is written to 1.
 *
 * @params enable   PLL1 Stop Enable Setting
 *                  - true: PLL1 and its clocks (MCGPLL1CLK, MCGPLL1CLK2X, and 
 *                       MCGDDRCLK2X) are enabled if system is in Normal Stop mode.
 *                  - false: PLL1 clocks (MCGPLL1CLK, MCGPLL1CLK2X, and MCGDDRCLK2X) 
 *                       are disabled in any of the Stop modes.
 */
static inline void clock_set_pllsten1(bool enable)
{
    BW_MCG_C11_PLLSTEN1(enable ? 1 : 0);
}

/*!
 * @brief Gets the PLL1 Stop Enable Setting.
 *
 * This function  gets the PLL1 Stop Enable Setting.
 *
 * @return enabled True if PLL1 Stop is enabled.
 */
static inline bool clock_get_pllsten1(void)
{
    return BR_MCG_C11_PLLSTEN1;
}

/*!
 * @brief Sets the PLL Clock Select Setting.
 *
 * This function  controls  whether the PLL0 or PLL1 output is selected as the
 * MCG source when CLKS are programmed in PLL Engaged External (PEE) mode 
 * (CLKS[1:0]=00 and IREFS=0 and PLLS=1).
 *
 * @params setting  PLL Clock Select Setting
 *                  - 0: PLL0 output clock is selected.
 *                  - 1: PLL1 output clock is selected.
 */
static inline void clock_set_pllcs(mcg_pllcs_select_t setting)
{
    BW_MCG_C11_PLLCS(setting);
}

/*!
 * @brief Gets the PLL Clock Select Setting.
 *
 * This function  gets the PLL Clock Select Setting.
 *
 * @return setting  PLL Clock Select Setting
 */
static inline mcg_pllcs_select_t clock_get_pllcs(void)
{
    return (mcg_pllcs_select_t)BR_MCG_C11_PLLCS;
}

/*!
 * @brief Sets the PLL1 External Reference Divider Setting.
 *
 * This function  selects the amount to divide down the external reference 
 * clock selected by REFSEL2 for PLL1. The resulting frequency must be in a valid
 * reference range. After the PLL1 is enabled (by setting either PLLCLKEN1 or PLLS),
 * the PRDIV1 value must not be changed when LOCK1 is zero.
 *
 * @params setting  PLL1 External Reference Divider Setting
 */
static inline void clock_set_prdiv1(uint8_t setting)
{
    BW_MCG_C11_PRDIV1(setting);
}

/*!
 * @brief Gets the PLL1 External Reference Divider Setting.
 *
 * This function  gets the PLL1 External Reference Divider Setting.
 *
 * @return setting  PLL1 External Reference Divider Setting
 */
static inline uint8_t clock_get_prdiv1(void)
{
    return BR_MCG_C11_PRDIV1;
}

/*!
 * @brief Sets the PLL1 Loss of Lock Interrupt Enable Setting.
 *
 * This function  determines whether an interrupt request is made following a 
 * loss of lock indication for PLL1. This bit only has an affect when LOLS1 is set.
 *
 * @params enable   PLL1 Loss of Lock Interrupt Enable Setting
 *                  - true: Generate an interrupt request on loss of lock on PLL1.
 *                  - false: No interrupt request is generated on loss of lock on PLL1.
 */
static inline void clock_set_lolie1(bool enable)
{
    BW_MCG_C12_LOLIE1(enable ? 1 : 0);
}

/*!
 * @brief Gets the PLL1 Loss of Lock Interrupt Enable Setting.
 *
 * This function  gets the PLL1 Loss of Lock Interrupt Enable Setting.
 *
 * @return enabled true if PLL1 Loss of Lock Interrupt is enabled.
 */
static inline bool clock_get_lolie1(void)
{
    return BR_MCG_C12_LOLIE1;
}

/*!
 * @brief Sets the Clock Monitor Enable2 Setting
 *
 * This function  enables/disables the loss of the clock monitor for the OSC1 external
 * reference clock. LOCRE2  determines whether a reset or interrupt request is generated
 * following a loss of OSC1 external reference clock. The CME2 bit should only be set
 * to a logic 1 when the MCG is in an operational mode that uses the external clock 
 * (PEE or PBE) . Whenever the CME2 bit is set to a logic 1, the value of the RANGE1
 * bits in the C10 register should not be changed. CME2 bit should be set to a logic 0
 * before the MCG enters any Stop mode. Otherwise, a reset request may occur while in 
 * Stop mode.
 *
 * @params enable  Clock Monitor Enable2 Setting
 *                  - true: Generate a reset request on loss of external clock on OSC1.
 *                  - false: External clock monitor for OSC1 is disabled.
 */
static inline void clock_set_cme2(bool enable)
{
    BW_MCG_C12_CME2(enable ? 1 : 0);
}

/*!
 * @brief Gets the Clock Monitor Enable2 Setting.
 *
 * This function  gets the Clock Monitor Enable2 Setting.
 *
 * @return enabled True if Clock Monitor Enable2 is enabled.
 */
static inline bool clock_get_cme2(void)
{
    return BR_MCG_C12_CME2;
}

/*!
 * @brief Sets the VCO1 Divider Setting.
 *
 * This function  selects the amount to divide the VCO output of the PLL1. 
 * The VDIV1 bits establishes the multiplication factor (M) applied to the reference
 * clock frequency. After the PLL1 is enabled (by setting either PLLCLKEN1 or 
 * PLLS), the VDIV1 value must not be changed when LOCK1 is zero.
 *
 * @params setting  VCO1 Divider Setting
 */
static inline void clock_set_vdiv1(uint8_t setting)
{
    BW_MCG_C12_VDIV1(setting);
}

/*!
 * @brief Gets the VCO1 Divider Setting.
 *
 * This function  gets the VCO1 Divider Setting.
 *
 * @return setting  VCO1 Divider Setting
 */
static inline uint8_t clock_get_vdiv1(void)
{
    return BR_MCG_C12_VDIV1;
}

/*!
 * @brief Gets the Loss of the Lock2 Status.
 *
 * This function  gets the Loss of the Lock2 Status. This bit is a sticky bit indicating
 * the lock status for the PLL1. LOLS1 is set if after acquiring lock, the PLL1 
 * output frequency has fallen outside the lock exit frequency tolerance, D unl. 
 * LOLIE1 determines whether an interrupt request is made when LOLS1 is set. This
 * bit is cleared by reset or by writing a logic 1 to it when set. Writing a logic 0
 * to this bit has no effect.
 *
 * @return status  Loss of Lock2 Status
 *                 - 0: PLL1 has not lost lock since LOLS1 was last cleared.
 *                 - 1: PLL1 has lost lock since LOLS1 was last cleared.
 */
static inline mcg_lols_status_t clock_get_lols1(void)
{
    return (mcg_lols_status_t)BR_MCG_S2_LOLS1;
}

/*!
 * @brief Gets the Lock1 Status.
 *
 * This function  gets the Lock1 Status. This bit indicates whether PLL1 has 
 * acquired the lock. PLL1 Lock detection is disabled when not operating in either
 * PBE or PEE mode unless the PLLCLKEN1=1 and the the MCG is not configured in the BLPI or the
 * BLPE mode. While the PLL1 clock is locking to the desired frequency, MCGPLL1CLK,
 * MCGPLL1CLK2X, and MCGDDRCLK2X  are gated off until the LOCK1 bit gets 
 * asserted. If the lock status bit is set, changing the value of the PRDIV1[2:0] 
 * bits in the C8 register or the VDIV2[4:0] bits in the C9 register causes the
 * lock status bit to clear and stay cleared until the PLL1 has reacquired lock. 
 * Loss of PLL1 reference clock will also causes the LOCK1 bit to clear until the PLL1 
 * has reacquired lock. Entry into the LLS, VLPS, or a regular Stop with the PLLSTEN1=0 also
 * causes the lock status bit to clear and stay cleared until the Stop mode is exited
 * and the PLL1 has reacquired the lock. Any time the PLL1 is enabled and the LOCK1 bit
 * is cleared, the MCGPLL1CLK, MCGPLL1CLK2X, and MCGDDRCLK2X  are gated off 
 * until the LOCK1 bit is asserted again.
 *
 * @return status  Lock1 Status
 *                 - 0: PLL1 is currently unlocked.
 *                 - 1: PLL1 is currently locked.
 */
static inline mcg_lock_status_t clock_get_lock1(void)
{
    return (mcg_lock_status_t)BR_MCG_S2_LOCK1;
}

/*!
 * @brief Gets the PLL Clock Select Status.
 *
 * This function  gets the PLL Clock Select Status. The PLLCST indicates the PLL
 * clock selected by PLLCS. The PLLCST bit is not updated immediately after a
 * write to the PLLCS bit due internal synchronization between clock domains.
 *
 * @return status  PLL Clock Select Status
 *                 - 0: Source of PLLCS is PLL0 clock.
 *                 - 1: Source of PLLCS is PLL1 clock.
 */
static inline mcg_pllcs_select_t clock_get_pllcst(void)
{
    return (mcg_pllcs_select_t)BR_MCG_S2_PLLCST;
}

/*!
 * @brief Gets the OSC1 Initialization Status.
 *
 * This function  gets the OSC1 Initialization Status. This bit is set after the 
 * initialization cycles of the 2nd crystal oscillator clock have completed. See
 * the Oscillator block guide for more details.
 *
 * @return status  OSC1 Initialization Status
 */
static inline uint8_t clock_get_oscinit1(void)
{
    return BR_MCG_S2_OSCINIT1;
}

/*!
 * @brief Gets the OSC1 Loss of Clock Status.
 *
 * This function  gets the OSC1 Loss of Clock Status. This bit indicates when a loss
 * of the OSC1 external reference clock has occurred. LOCRE2 determines if a reset or 
 * interrupt is generated when LOCS2 is set. This bit is cleared by writing a 
 * logic 1 to it when set.
 *
 * @return status  OSC1 Loss of Clock Status
 *                 - 0: No loss of OSC1 external reference clock has occurred.
 *                 - 1: Loss of OSC1 external reference clock has occurred.
 */
static inline mcg_locs2_status_t clock_get_locs2(void)
{
    return (mcg_locs2_status_t)BR_MCG_S2_LOCS2;
}
#endif /* FSL_FEATURE_MCG_USE_PLLREFSEL */

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_MCG_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

