/***************************************************************************//**
* \file cy_sysclk.h
* \version 1.0
*
* Provides an API declaration of the sysclk driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \defgroup group_sysclk System Clock (SysClk)
* \{
* The sysclk driver contains an API for configuring system and peripheral clocks.
* Most of the sysclk driver API functions are simple wrappers for writing to registers
* to configure or manage a particular clock.
*
* In the API reference for this driver each clock has its own section, so all the
* information to manage that clock is in the same place.
*
* \section group_sysclk_configuration Configuration Considerations
*
* \warning When the device is in the Ultra-Low Power (ULP) mode, the maximum frequency of all the clocks is limited to
* the 50 MHz with the exception for the slow clock which is limited to 25 MHz.
*
* There are no general sysclk configuration concerns. Some clocks (for example the PLL) require configuration.
* For such a clock the API provides a configuration structure and a Configure() function to set up the clock.
* See notes on individual function definitions.
*
* \section group_sysclk_more_information More Information
*
* See the Clock Component datasheet.
* See also the sysclk chapter of the device technical reference manual (TRM).
*
* \section group_sysclk_MISRA MISRA-C Compliance
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>1.2</td>
*     <td>R</td>
*     <td>No reliance on undefined behavior.</td>
*     <td>Calculation of absolute value in FLL and PLL configure.</td>
*   </tr>
*   <tr>
*     <td>10.1</td>
*     <td>R</td>
*     <td>The value of an expression of integer type shall not be implicitly converted to a different, underlying type if the expression is complex.</td>
*     <td>Using a Cypress defined macro to access memory mapped objects.
*         Using a Cypress defined macro to divide with rounding.</td>
*   </tr>
*   <tr>
*     <td>10.3</td>
*     <td>R</td>
*     <td>A composite integer expression is being cast to a wider type.</td>
*     <td>Using a Cypress defined macro to access memory mapped objects. Calculating clock parameters.</td>
*   </tr>
*   <tr>
*     <td>10.4</td>
*     <td>R</td>
*     <td>A composite floating point expression is being cast to double, or unsigned.</td>
*     <td>Using the C library sqrt() function. Casting a floating point calculation result to an integer.</td>
*   </tr>
*   <tr>
*     <td>10.5</td>
*     <td>R</td>
*     <td>The value of an expression of integer type shall not be implicitly converted to a different, underlying type if the expression is complex.</td>
*     <td>Using a Cypress defined macro to access memory mapped objects.</td>
*   </tr>
*   <tr>
*     <td>12.1</td>
*     <td>A</td>
*     <td>Extra parentheses recommended.</td>
*     <td>Ternary operator uses constants; extra parentheses not needed.</td>
*   </tr>
*   <tr>
*     <td>13.4</td>
*     <td>R</td>
*     <td>The controlling expression of a for statement shall not contain any objects of floating type.</td>
*     <td>Scanning through a list of floating point values.</td>
*   </tr>
*   <tr>
*     <td>18.4</td>
*     <td>R</td>
*     <td>Unions shall not be used.</td>
*     <td>Clock path in \ref Cy_SysClk_PeriphGetFrequency() may use either FLL or PLL.</td>
*   </tr>
*   <tr>
*     <td>19.7</td>
*     <td>A</td>
*     <td>A function shall be used in preference to a function-like macro.</td>
*     <td>Function-like macros are used to achieve more efficient code.</td>
*   </tr>
* </table>
*
* \section group_sysclk_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_sysclk_macro           Macros
* \{
*   \defgroup group_sysclk_macro_return    Function return values
* \}
* \defgroup group_sysclk_imo             Internal Main Oscillator (IMO)
* \defgroup group_sysclk_eco             External Crystal Oscillator (ECO)
* \defgroup group_sysclk_muxes           Input Multiplexers
* \defgroup group_sysclk_fll             Frequency Locked Loop (FLL)
* \defgroup group_sysclk_pll             Phase Locked Loop (PLL)
* \defgroup group_sysclk_ilo             Internal Low-Speed Oscillator (ILO)
* \defgroup group_sysclk_pilo            Precision Internal Low-Speed Oscillator (PILO)
* \defgroup group_sysclk_calclk          Clock Measurement
* \defgroup group_sysclk_trim            Clock Trim
* \defgroup group_sysclk_wco             Watch Crystal Oscillator (WCO)
* \defgroup group_sysclk_clk_hf          High-Frequency Clocks
* \defgroup group_sysclk_clk_fast        Fast Clock
* \defgroup group_sysclk_clk_peripheral  Peripherals Clocks
* \defgroup group_sysclk_clk_slow        Slow Clock
* \defgroup group_sysclk_clk_lf          Low-Frequency Clock
* \defgroup group_sysclk_clk_timer       Timer Clock
* \defgroup group_sysclk_clk_pump        Pump Clock
* \defgroup group_sysclk_clk_bak         Backup Domain Clock
*/

#if !defined(__CY_SYSCLK_H__)
#define __CY_SYSCLK_H__

#include "cy_device_headers.h"
#include "syslib/cy_syslib.h"
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
* \addtogroup group_sysclk_macro
* \{
*/

/** Driver major version */
#define CY_SYSCLK_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_SYSCLK_DRV_VERSION_MINOR       0

/**
* \addtogroup group_sysclk_returns
* \{
* Specifies return values meaning
*/
/** Command completed with no errors */
#define CY_SYSCLK_SUCCESS           0ul
/** Invalid function input parameter */
#define CY_SYSCLK_BAD_PARAM         1ul
/** Status of calibration counters: STARTED */
#define CY_SYSCLK_STARTED           7ul
/** Status of calibration counters: FINISHED */
#define CY_SYSCLK_FINISHED          8ul
/** Timeout occurred */
#define CY_SYSCLK_TIMEOUT        0x10ul
/** Clock is in an invalid state */
#define CY_SYSCLK_INVALID_STATE  0x11ul
/** \} group_sysclk_returns */
/** \} group_sysclk_macro */

/** \cond INTERNAL */
/* Generates shorter code for setting register bitfields */
#define CY_SYSCLK_CLR_SET(reg, field, value) (reg) = _CLR_SET_FLD32U((reg), field, value)
/* Calculate a / b with rounding to the nearest integer. a and b must have the same sign. */
#define CY_SYSCLK_DIV_ROUND(a, b) (((a) + ((b) / 2u)) / (b))
/* Calculate a / b with rounding up if remainder != 0. a and b must both be positive. */
#define CY_SYSCLK_DIV_ROUNDUP(a, b) ((((a) - 1u) / (b)) + 1u)
/** \endcond */

/**
* \addtogroup group_sysclk_imo
* \{
*/

/* ========================================================================== */
/* ===========================    IMO SECTION    ============================ */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_ImoEnable
****************************************************************************//**
*
* Enables the internal main oscillator (IMO).
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ImoEnable(void)
{
    SRSS->CLK_IMO_CONFIG |= _VAL2FLD(SRSS_CLK_IMO_CONFIG_ENABLE, 1u); /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_ImoDisable
****************************************************************************//**
*
* Disables the internal main oscillator (IMO).
*
* \note
* If the IMO is the source of hf_clk[0] then disabling the IMO disables the CPU.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ImoDisable(void)
{
    SRSS->CLK_IMO_CONFIG &= ~_VAL2FLD(SRSS_CLK_IMO_CONFIG_ENABLE, 1u); /* 0 = disable */
}
/** \} group_sysclk_imo */


/**
* \addtogroup group_sysclk_eco
* \{
*/

/* ========================================================================== */
/* ===========================    ECO SECTION    ============================ */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_EcoConfigure
****************************************************************************//**
*
* This function configures the external crystal oscillator (ECO) trim bits based
* on crystal characteristics. This function should be called only when the ECO is
* disabled.
*
* \param freq Operating frequency of the crystal in Hz.
*
* \param cLoad Crystal load capacitance in pF.
*
* \param esr Effective series resistance of the crystal in ohms.
*
* \param driveLevel Crystal drive level in uW.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - ECO configuration completed successfully<br>
* CY_SYSCLK_BAD_PARAM - One or more invalid parameters<br>
* CY_SYSCLK_INVALID_STATE - ECO already enabled
*
* \note
* The following calculations are implemented, generally in floating point:
*   freqMHz = freq / 1,000,000
*   max amplitude Vpp = 1,000 * sqrt(drivelevel / 2 / esr) / 3.14 / freqMHz / cLoad
*   gm_min mA/V = 5 * 4 * 3.14 * 3.14 * freqMhz^2 * cLoad^2 * 4 * esr / 1,000,000,000
*   # amplifier sections = INT(gm_min / 4.5)
*   atrim = if max amplitude < 0.5 then error
*           else 2 * the following:
*                    max amplitude < 0.6: 0
*                    max amplitude < 0.7: 1
*                    max amplitude < 0.8: 2
*                    max amplitude < 0.9: 3
*                    max amplitude < 1.15: 5
*                    max amplitude < 1.275: 6
*                    max amplitude >= 1.275: 7
*   wdtrim = if max amplitude < 0.5 then error
*            else 2 * the following:
*                     max amplitude < 1.2: INT(5 * max amplitude) - 2
*                     max amplitude >= 1.2: 3
*   gtrim = if # amplifier sections > 3 then error
*           else the following:
*                # amplifier sections > 1: # amplifier sections
*                # amplifier sections = 1: 0
*                # amplifier sections < 1: 1
*   rtrim = if gtrim = error then error
*           else the following:
*                freqMHz > 26.8: 0
*                freqMHz > 23.33: 1
*                freqMHz > 16.5: 2
*                freqMHz <= 16.5: 3
*   ftrim = if atrim = error then error
*           else INT(atrim / 2)
*
* \note
* This function returns immediately if the ECO is enabled.
*******************************************************************************/
uint32_t Cy_SysClk_EcoConfigure(uint32_t freq, uint32_t cLoad, uint32_t esr, uint32_t driveLevel);

/*******************************************************************************
* Function Name: Cy_SysClk_EcoEnable
****************************************************************************//**
*
* Enables the external crystal oscillator (ECO). This function should be called
* after \ref Cy_SysClk_EcoConfigure.
*
* \param timeoutus Amount of time in microseconds to wait for the ECO to lock.
* If a lock does not occur, the ECO is stopped. To avoid waiting for a lock, set
* this parameter to 0.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - ECO successfully enabled<br>
* CY_SYSCLK_TIMEOUT - Timeout waiting for ECO lock
*
*******************************************************************************/
uint32_t Cy_SysClk_EcoEnable(uint32_t timeoutus);

/*******************************************************************************
* Function Name: Cy_SysClk_EcoDisable
****************************************************************************//**
*
* Disables the external crystal oscillator (ECO). This function should not be
* called if the ECO is sourcing clk_hf[0].
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_EcoDisable(void)
{
    SRSS->CLK_ECO_CONFIG &= ~_VAL2FLD(SRSS_CLK_ECO_CONFIG_ECO_EN, 1u); /* 0 = disable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_EcoGetStatus
****************************************************************************//**
*
* Reports the current status of the external crystal oscillator (ECO).
*
* \return
* 0 = ECO does not have sufficient amplitude<br>
* 1 = ECO has sufficient amplitude but may not be meeting accuracy and duty cycle specifications<br>
* 2 = ECO has fully stabilized
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_EcoGetStatus(void)
{
    uint32_t retval = (SRSS->CLK_ECO_STATUS & 3ul); /* bit 0 = ECO_OK, bit 1 = ECO_READY */
    /* if ECO is not ready, just report the ECO_OK bit. Otherwise report 2 = ECO ready */
    return (((retval & 2ul) == 0) ? retval : 2ul);
}
/** \} group_sysclk_eco */


/**
* \addtogroup group_sysclk_muxes
* \{
*/

/* ========================================================================== */
/* ====================    INPUT MULTIPLEXER SECTION    ===================== */
/* ========================================================================== */
/**
* Configures two multiplexers: a DSI multiplexer and a path multiplexer. The
* outputs of these multiplexers generally drive the FLL and the PLLs. See
* CLK_DSI_SELECT registers, DSI_MUX bits, and CLK_PATH_SELECT registers,
* PATH_MUX bits. Used with functions \ref Cy_SysClk_ClkPathSetSource and
* \ref Cy_SysClk_ClkPathGetSource.
*
* \note
* Each path multiplexer output is a source for the corresponding FLL/PLL input
* source multiplexer.
*
* \note The PDL driver cannot use the DSI outputs as sources. This must be done
* through DSI configuration using PSoC Creator.
*/
typedef enum
{
    CY_SYSCLK_CLKPATH_IN_IMO    =     0u, /**< Select the IMO as the output of the path mux */
    CY_SYSCLK_CLKPATH_IN_EXT    =     1u, /**< Select the EXT as the output of the path mux */
    CY_SYSCLK_CLKPATH_IN_ECO    =     2u, /**< Select the ECO as the output of the path mux */
    CY_SYSCLK_CLKPATH_IN_ALTHF  =     3u, /**< Select the ALTHF as the output of the path mux */
    CY_SYSCLK_CLKPATH_IN_DSIMUX =     4u, /**< Select the DSI MUX output as the output of the path mux */
    CY_SYSCLK_CLKPATH_IN_DSI    = 0x100u, /**< Select a DSI signal (0 - 15) as the output of the DSI mux and path mux */
    CY_SYSCLK_CLKPATH_IN_ILO    = 0x110u, /**< Select the ILO (16) as the output of the DSI mux and path mux */
    CY_SYSCLK_CLKPATH_IN_WCO    = 0x111u, /**< Select the WCO (17) as the output of the DSI mux and path mux */
    CY_SYSCLK_CLKPATH_IN_ALTLF  = 0x112u, /**< Select the ALTLF (18) as the output of the DSI mux and path mux */
    CY_SYSCLK_CLKPATH_IN_PILO   = 0x113u  /**< Select the PILO (19) as the output of the DSI mux and path mux */
} cy_en_clkpath_in_sources_t;

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPathSetSource
****************************************************************************//**
*
* Configures the source for the specified clock path.
*
* \param clkPath Selects which clock path to configure; 0 is the first clock
* path, which is the FLL.
*
* \param source \ref cy_en_clkpath_in_sources_t
*
*******************************************************************************/
void Cy_SysClk_ClkPathSetSource(uint32_t clkPath, cy_en_clkpath_in_sources_t source);

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPathGetSource
****************************************************************************//**
*
* Reports which source is selected for the path mux.
*
* \param clkPath Selects which clock path to report; 0 is the first clock path,
* which is the FLL.
*
* \return \ref cy_en_clkpath_in_sources_t
*
*******************************************************************************/
cy_en_clkpath_in_sources_t Cy_SysClk_ClkPathGetSource(uint32_t clkPath);
/** \} group_sysclk_muxes */


/**
* \addtogroup group_sysclk_fll
* \{
*/

/* ========================================================================== */
/* ===========================    FLL SECTION    ============================ */
/* ========================================================================== */
/** FLL and PLL output mode.
* See registers CLK_FLL_CONFIG3 and CLK_PLL_CONFIG0, bits BYPASS_SEL.
*/
typedef enum
{
    CY_SYSCLK_FLLPLL_OUTPUT_AUTO   = 0u, /**< Output FLL/PLL input source when not locked, and FLL/PLL output when locked. */
    CY_SYSCLK_FLLPLL_OUTPUT_AUTO1  = 1u, /**< Same as AUTO */
    CY_SYSCLK_FLLPLL_OUTPUT_INPUT  = 2u, /**< Output FLL/PLL input source regardless of lock status. */
    CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT = 3u  /**< Output FLL/PLL output regardless of lock status. This can be dangerous if used to clock hf_clk as FLL/PLL output may be unstable. */
} cy_en_fll_pll_output_mode_t;

/** FLL current-controlled oscillator (CCO) frequency ranges.
* See register CLK_FLL_CONFIG4, bits CCO_RANGE.
*/
typedef enum
{
    CY_SYSCLK_FLL_CCO_RANGE0, /**< Target frequency is in range  48 -  64 MHz. */
    CY_SYSCLK_FLL_CCO_RANGE1, /**< Target frequency is in range  64 -  85 MHz. */
    CY_SYSCLK_FLL_CCO_RANGE2, /**< Target frequency is in range  85 - 113 MHz. */
    CY_SYSCLK_FLL_CCO_RANGE3, /**< Target frequency is in range 113 - 150 MHz. */
    CY_SYSCLK_FLL_CCO_RANGE4  /**< Target frequency is in range 150 - 200 MHz. */
} cy_en_fll_cco_ranges_t;

/** Structure containing information for manual configuration of FLL.
*/
typedef struct
{
    uint32_t                    fllMult;         /**< CLK_FLL_CONFIG  register, FLL_MULT bits */
    uint32_t                    refDiv;          /**< CLK_FLL_CONFIG2 register, FLL_REF_DIV bits */
    cy_en_fll_cco_ranges_t      ccoRange;        /**< CLK_FLL_CONFIG4 register, CCO_RANGE bits */
    bool                        enableOutputDiv; /**< CLK_FLL_CONFIG  register, FLL_OUTPUT_DIV bit */
    uint32_t                    lockTolerance;   /**< CLK_FLL_CONFIG2 register, LOCK_TOL bits */
    uint32_t                    igain;           /**< CLK_FLL_CONFIG3 register, FLL_LF_IGAIN bits */
    uint32_t                    pgain;           /**< CLK_FLL_CONFIG3 register, FLL_LF_PGAIN bits */
    uint32_t                    settlingCount;   /**< CLK_FLL_CONFIG3 register, SETTLING_COUNT bits */
    cy_en_fll_pll_output_mode_t outputMode;      /**< CLK_FLL_CONFIG3 register, BYPASS_SEL bits */
} cy_stc_fll_manual_config_t;

/*******************************************************************************
* Function Name: Cy_SysClk_FllConfigure
****************************************************************************//**
*
* Configures the FLL.
*
* \param inputFreq frequency of input source, in Hz
*
* \param outputFreq Desired FLL output frequency, in Hz. Allowable range is
* 24 MHz to 100 MHz (200 MHz with a relatively poor duty cycle. If the output
* frequency is less than 48 MHz, FLL_OUTPUT_DIV must be set; the output divide by
* 2 option is required.
*
* \param outputMode \ref cy_en_fll_pll_output_mode_t
* If output mode is bypass, then the output frequency equals the input source
* frequency regardless of the frequency parameter values.
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - FLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - FLL not configured because it is enabled<br>
* CY_SYSCLK_BAD_PARAM - desired output frequency is out of valid range
*
* \note
* Do not call this function when the FLL is enabled. If it is, then this function
* returns immediately with an error return value and no register updates.
*******************************************************************************/
uint32_t Cy_SysClk_FllConfigure(uint32_t inputFreq, uint32_t outputFreq, cy_en_fll_pll_output_mode_t outputMode);

/*******************************************************************************
* Function Name: Cy_SysClk_FllManualConfigure
****************************************************************************//**
*
* Manually configures the FLL based on user inputs.
*
* \param config \ref cy_stc_fll_manual_config_t
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - FLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - FLL not configured because it is enabled
*
* \note
* Do not call this function when the FLL is enabled. If it is, then this function
* returns immediately with an error return value and no register updates.
*******************************************************************************/
uint32_t Cy_SysClk_FllManualConfigure(const cy_stc_fll_manual_config_t *config);

/*******************************************************************************
* Function Name: Cy_SysClk_FllGetConfiguration
****************************************************************************//**
*
* Reports the FLL configuration settings.
*
* \param config \ref cy_stc_fll_manual_config_t
*
*******************************************************************************/
void Cy_SysClk_FllGetConfiguration(cy_stc_fll_manual_config_t *config);

/*******************************************************************************
* Function Name: Cy_SysClk_FllEnable
****************************************************************************//**
*
* Enables the FLL. The FLL should be configured before calling this function.
*
* \param timeoutus amount of time in micro seconds to wait for FLL to lock.
* If lock doesn't occur, FLL is stopped. To avoid waiting for lock set this to 0,
* and manually check for lock using \ref Cy_SysClk_FllGetLockStatus.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - FLL successfully enabled<br>
* CY_SYSCLK_TIMEOUT - Timeout waiting for FLL lock
*
*******************************************************************************/
uint32_t Cy_SysClk_FllEnable(uint32_t timeoutus);

/*******************************************************************************
* Function Name: Cy_SysClk_FllGetLockStatus
****************************************************************************//**
*
* Returns lock status of the FLL
*
* \return 0 = not locked, 1 = locked
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_FllGetLockStatus(void)
{
    return (uint32_t)(_FLD2VAL(SRSS_CLK_FLL_STATUS_LOCKED, SRSS->CLK_FLL_STATUS));
}

/*******************************************************************************
* Function Name: Cy_SysClk_FllLostLock
****************************************************************************//**
*
* Reports whether or not the FLL lost its lock since the last time this function
* was called. Clears the lost lock indicator.
*
* \return 0 = didn't lose lock, 1 = lost lock
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_FllLostLock(void)
{
    uint32_t retval = _FLD2VAL(SRSS_CLK_FLL_STATUS_UNLOCK_OCCURRED, SRSS->CLK_FLL_STATUS);
    /* write a 1 to clear the unlock occurred bit */
    SRSS->CLK_FLL_STATUS = _VAL2FLD(SRSS_CLK_FLL_STATUS_UNLOCK_OCCURRED, 1u);
    return (retval);
}

/*******************************************************************************
* Function Name: Cy_SysClk_FllDisable
****************************************************************************//**
*
* Disables the FLL and the CCO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_FllDisable(void)
{
    SRSS->CLK_FLL_CONFIG  &= ~_VAL2FLD(SRSS_CLK_FLL_CONFIG_FLL_ENABLE,  1u); /* 0 = disable */
    SRSS->CLK_FLL_CONFIG4 &= ~_VAL2FLD(SRSS_CLK_FLL_CONFIG4_CCO_ENABLE, 1u); /* 0 = disable */
}
/** \} group_sysclk_fll */


/**
* \addtogroup group_sysclk_pll
* \{
*/

/* ========================================================================== */
/* ===========================    PLL SECTION    ============================ */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_PllConfigure
****************************************************************************//**
*
* Configures a given PLL.
* The configuration formula used is:
*   Fout = pll_clk * (P / Q / div_out), where:
*     Fout is the desired output frequency
*     pll_clk is the frequency of the input source
*     P is the feedback divider. Its value is in bitfield FEEDBACK_DIV.
*     Q is the reference divider. Its value is in bitfield REFERENCE_DIV.
*     div_out is the reference divider. Its value is in bitfield OUTPUT_DIV.
*
* \param clkPath Selects which PLL to configure. 1 is the first PLL; 0 is invalid.
*
* \param inputFreq frequency of input source, in Hz; allowed range is 4 MHz to 64 MHz.
*
* \param outputFreq desired output frequency, in Hz; allowed range 1 MHz to 400 MHz, in 4-MHz steps.
*
* \param outputMode \ref cy_en_fll_pll_output_mode_t. If output mode is bypass, then
* the output frequency equals the input source frequency regardless of the
* frequency parameter values.
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - PLL not configured because it is enabled<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number, or input or desired output frequency is out of valid range
*
* \note
* Do not call this function when the PLL is enabled. If it is, then this function
* returns immediately with an error return value and no register updates.
*******************************************************************************/
uint32_t Cy_SysClk_PllConfigure(uint32_t clkPath, uint32_t inputFreq, uint32_t outputFreq,
                                cy_en_fll_pll_output_mode_t outputMode);

/*******************************************************************************
* Function Name: Cy_SysClk_PllManualConfigure
****************************************************************************//**
*
* Manually configures a PLL based on user inputs.
*
* \param clkPath Selects which PLL to configure. 1 is the first PLL; 0 is invalid.
*
* \param feedbackDiv
* Sets the feedback divider for the selected PLL, valid range is 1 to 50
*
* \param referenceDiv
* Sets reference divider of PLL, valid range is 1 to 31. (Input Frequency /
* reference divider must be in the range of 4 MHz to 8 MHz.
*
* \param outputDiv
* Sets the output divider, valid range is 1 to 255. To use the PLL as the source
* of an hf_clk this value must be 2 or greater.
*
* \param outputMode
* \ref cy_en_fll_pll_output_mode_t
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully configured<br>
* CY_SYSCLK_INVALID_STATE - PLL not configured because it is enabled<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
* \note
* Do not call this function when the PLL is enabled. If it is, then this function
* returns immediately with an error return value and no register updates.
*******************************************************************************/
uint32_t Cy_SysClk_PllManualConfigure(uint32_t clkPath, uint32_t feedbackDiv, uint32_t referenceDiv,
                                      uint32_t outputDiv, cy_en_fll_pll_output_mode_t outputMode);

/*******************************************************************************
* Function Name: Cy_SysClk_PllGetConfiguration
****************************************************************************//**
*
* Reports configuration settings for a PLL.
*
* \param clkPath Selects which PLL to report. 1 is the first PLL; 0 is invalid.
*
* \param feedbackDiv
* Pointer to storage for the feedback divider for the selected PLL.
*
* \param referenceDiv
* Pointer to storage for the reference divider for the selected PLL.
*
* \param outputDiv
* Pointer to storage for the output divider for the selected PLL.
*
* \param outputMode
* \ref cy_en_fll_pll_output_mode_t
*
*
* \return  Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL data successfully reported<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
*******************************************************************************/
uint32_t Cy_SysClk_PllGetConfiguration(uint32_t clkPath, uint32_t *feedbackDiv, uint32_t *referenceDiv,
                                       uint32_t *outputDiv, cy_en_fll_pll_output_mode_t *outputMode);

/*******************************************************************************
* Function Name: Cy_SysClk_PllEnable
****************************************************************************//**
*
* Enables the PLL. The PLL should be configured before calling this function.
*
* \param clkPath Selects which PLL to enable. 1 is the first PLL; 0 is invalid.
*
* \param timeoutus amount of time in microseconds to wait for the PLL to lock.
* If lock doesn't occur, PLL is stopped. To avoid waiting for lock set this to 0,
* and manually check for lock using \ref Cy_SysClk_PllGetLockStatus.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully enabled<br>
* CY_SYSCLK_TIMEOUT - Timeout waiting for PLL lock<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
*******************************************************************************/
uint32_t Cy_SysClk_PllEnable(uint32_t clkPath, uint32_t timeoutus);

/*******************************************************************************
* Function Name: Cy_SysClk_PllGetLockStatus
****************************************************************************//**
*
* Reports the lock status of the selected PLL.
*
* \param clkPath Selects which PLL to check. 1 is the first PLL; 0 is invalid.
*
* \return 0 = not locked, 1 = locked, 0xFF = invalid clock path number
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PllGetLockStatus(uint32_t clkPath)
{
    uint32_t retval = 0xFFul; /* invalid clock path number */
    if ((clkPath != 0ul) && (clkPath <= SRSS_NUM_PLL))
    {
        retval = _FLD2VAL(SRSS_CLK_PLL_STATUS_LOCKED, SRSS->CLK_PLL_STATUS[clkPath - 1ul]);
    }
    return (retval);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PllLostLock
****************************************************************************//**
*
* Reports whether or not the selected PLL lost its lock since the last time this
* function was called. Clears the lost lock indicator.
*
* \param clkPath Selects which PLL to check. 1 is the first PLL; 0 is invalid.
*
* \return 0 = didn't lose lock, 1 = lost lock, 0xFF = invalid clock path number
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PllLostLock(uint32_t clkPath)
{
    uint32_t retval = 0xFFul; /* invalid clock path number */
    if ((clkPath != 0ul) && (clkPath <= SRSS_NUM_PLL))
    {
        retval = _FLD2VAL(SRSS_CLK_PLL_STATUS_UNLOCK_OCCURRED, SRSS->CLK_PLL_STATUS[clkPath - 1ul]);
        /* write a 1 to clear the unlock occurred bit */
        SRSS->CLK_PLL_STATUS[clkPath - 1ul] = _VAL2FLD(SRSS_CLK_PLL_STATUS_UNLOCK_OCCURRED, 1u);
    }
    return (retval);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PllDisable
****************************************************************************//**
*
* Disables the selected PLL.
*
* \param clkPath Selects which PLL to disable. 1 is the first PLL; 0 is invalid.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - PLL successfully disabled<br>
* CY_SYSCLK_BAD_PARAM - invalid clock path number
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PllDisable(uint32_t clkPath)
{
    uint32_t retval = CY_SYSCLK_BAD_PARAM;
    if ((clkPath != 0ul) && (clkPath <= SRSS_NUM_PLL))
    {
        SRSS->CLK_PLL_CONFIG[clkPath - 1ul] &= ~_VAL2FLD(SRSS_CLK_PLL_CONFIG_ENABLE, 1u); /* 0 = disable */
        retval = CY_SYSCLK_SUCCESS;
    }
    return (retval);
}
/** \} group_sysclk_pll */


/**
* \addtogroup group_sysclk_ilo
* \{
*/

/* ========================================================================== */
/* ===========================    ILO SECTION    ============================ */
/* ========================================================================== */
/** ILO trim modes */
typedef enum
{
    CY_SYSCLK_ILO_TRIM_BLOCKING = 0u,   /**< Function doesn't return until trim completes. */
    CY_SYSCLK_ILO_TRIM_NONBLOCKING = 1u /**< Function returns immediately, must be called multiple times until it is finished. */
} cy_en_ilo_trim_modes_t;

/*******************************************************************************
* Function Name: Cy_SysClk_IloEnable
****************************************************************************//**
*
* Enables the ILO.
*
* \note The watchdog timer (WDT) must be unlocked before calling this function.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_IloEnable(void)
{
    SRSS->CLK_ILO_CONFIG |= _VAL2FLD(SRSS_CLK_ILO_CONFIG_ENABLE, 1u); /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_IloDisable
****************************************************************************//**
*
* Disables the ILO. ILO can't be disabled if WDT is enabled.
*
* \note The watchdog timer (WDT) must be unlocked before calling this function.
* Do not call this function if the WDT is enabled, because the WDT is clocked by
* the ILO.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_IloDisable(void)
{
    SRSS->CLK_ILO_CONFIG &= ~_VAL2FLD(SRSS_CLK_ILO_CONFIG_ENABLE, 1u); /* 0 = disable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_IloHibernateOn
****************************************************************************//**
*
* Controls whether the ILO stays on during a hibernate, or through an XRES or
* brown-out detect (BOD) event.
*
* \param on
* 1 = ILO stays on during hibernate or across XRES/BOD. 0 = ILO turns off for
* hibernate or XRES/BOD.
*
* \note Writes to the register/bit are ignored if the watchdog (WDT) is locked.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_IloHibernateOn(bool on)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_ILO_CONFIG, SRSS_CLK_ILO_CONFIG_HVILO_BACKUP, (uint32_t)on);
}
/** \} group_sysclk_ilo */


/**
* \addtogroup group_sysclk_pilo
* \{
*/

/* ========================================================================== */
/* ===========================    PILO SECTION    =========================== */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_PiloEnable
****************************************************************************//**
*
* Enables the PILO.
*
* \note Requires 1 msec, for delay between enabling the PILO and releasing the
* PILO reset.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PiloEnable(void)
{
    SRSS->CLK_PILO_CONFIG |= _VAL2FLD(SRSS_CLK_PILO_CONFIG_PILO_EN, 1u); /* 1 = enable */
    Cy_SysLib_Delay(1/*msec*/);
    /* release the reset and enable clock output */
    SRSS->CLK_PILO_CONFIG |= (_VAL2FLD(SRSS_CLK_PILO_CONFIG_PILO_RESET_N, 1u) |
                              _VAL2FLD(SRSS_CLK_PILO_CONFIG_PILO_CLK_EN, 1u));
}

/*******************************************************************************
* Function Name: Cy_SysClk_PiloDisable
****************************************************************************//**
*
* Disables the PILO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PiloDisable(void)
{
    /* Clear PILO_EN, PILO_RESET_N, and PILO_CLK_EN bitfields. This disables the
       PILO and holds the PILO in a reset state. */
    SRSS->CLK_PILO_CONFIG &= ~(_VAL2FLD(SRSS_CLK_PILO_CONFIG_PILO_EN, 1u)      |
                               _VAL2FLD(SRSS_CLK_PILO_CONFIG_PILO_RESET_N, 1u) |
                               _VAL2FLD(SRSS_CLK_PILO_CONFIG_PILO_CLK_EN, 1u));}
/** \} group_sysclk_pilo */


/**
* \addtogroup group_sysclk_calclk
* \{
*/

/* ========================================================================== */
/* ====================    CLOCK MEASUREMENT SECTION    ===================== */
/* ========================================================================== */
/** Defines all possible clock sources. */
typedef enum
{
    CY_SYSCLK_MEAS_CLK_ILO =           1u,
    CY_SYSCLK_MEAS_CLK_WCO =           2u,
    CY_SYSCLK_MEAS_CLK_ALTLF =         4u,
    CY_SYSCLK_MEAS_CLK_IMO =           6u,
    CY_SYSCLK_MEAS_CLK_PILO =          8u,
    CY_SYSCLK_MEAS_CLK_FAST_CLKS =     0x100u,
    CY_SYSCLK_MEAS_CLK_ECO =           0x101u,
    CY_SYSCLK_MEAS_CLK_EXT =           0x102u,
    CY_SYSCLK_MEAS_CLK_ALTHF =         0x103u,
    CY_SYSCLK_MEAS_CLK_PATH_CLKS =     0x500u,
    CY_SYSCLK_MEAS_CLK_PATH0 =            0x500u,
    CY_SYSCLK_MEAS_CLK_PATH1 =            0x501u,
    CY_SYSCLK_MEAS_CLK_PATH2 =            0x502u,
    CY_SYSCLK_MEAS_CLK_PATH3 =            0x503u,
    CY_SYSCLK_MEAS_CLK_PATH4 =            0x504u,
    CY_SYSCLK_MEAS_CLK_PATH5 =            0x505u,
    CY_SYSCLK_MEAS_CLK_PATH6 =            0x506u,
    CY_SYSCLK_MEAS_CLK_PATH7 =            0x507u,
    CY_SYSCLK_MEAS_CLK_PATH8 =            0x508u,
    CY_SYSCLK_MEAS_CLK_PATH9 =            0x509u,
    CY_SYSCLK_MEAS_CLK_PATH10 =        0x50Au,
    CY_SYSCLK_MEAS_CLK_PATH11 =        0x50Bu,
    CY_SYSCLK_MEAS_CLK_PATH12 =        0x50Cu,
    CY_SYSCLK_MEAS_CLK_PATH13 =        0x50Du,
    CY_SYSCLK_MEAS_CLK_PATH14 =        0x50Eu,
    CY_SYSCLK_MEAS_CLK_PATH15 =        0x50Fu,
    CY_SYSCLK_MEAS_CLK_HF_CLKS =       0x600u,
    CY_SYSCLK_MEAS_CLK_HFCLK0 =        0x600u,
    CY_SYSCLK_MEAS_CLK_HFCLK1 =        0x601u,
    CY_SYSCLK_MEAS_CLK_HFCLK2 =        0x602u,
    CY_SYSCLK_MEAS_CLK_HFCLK3 =        0x603u,
    CY_SYSCLK_MEAS_CLK_HFCLK4 =        0x604u,
    CY_SYSCLK_MEAS_CLK_HFCLK5 =        0x605u,
    CY_SYSCLK_MEAS_CLK_HFCLK6 =        0x606u,
    CY_SYSCLK_MEAS_CLK_HFCLK7 =        0x607u,
    CY_SYSCLK_MEAS_CLK_HFCLK8 =        0x608u,
    CY_SYSCLK_MEAS_CLK_HFCLK9 =        0x609u,
    CY_SYSCLK_MEAS_CLK_HFCLK10 =       0x60Au,
    CY_SYSCLK_MEAS_CLK_HFCLK11 =       0x60Bu,
    CY_SYSCLK_MEAS_CLK_HFCLK12 =       0x60Cu,
    CY_SYSCLK_MEAS_CLK_HFCLK13 =       0x60Du,
    CY_SYSCLK_MEAS_CLK_HFCLK14 =       0x60Eu,
    CY_SYSCLK_MEAS_CLK_HFCLK15 =       0x60Fu,
    CY_SYSCLK_MEAS_CLK_LAST_CLK =      0x610u
} cy_en_meas_clks_t;

/*******************************************************************************
* Function Name: Cy_SysClk_InitClkMeasurementCounters
****************************************************************************//**
*
* Initializes clock measurement counters.
* Measurement counters allow you to measure one clock (clock2) versus another
* clock (clock1). This is done with two hardware counters. One counter (counter1),
* which is clocked off of clock1, is loaded with an initial value and it counts down.
* A second counter (counter2), which is clocked off of clock2, counts up until
* the first counter is done counting down.
* The frequency of clock2 can be determined with the following equation:
* clock2 frequency = (count2/count1) * clock1 frequency
*
* \param clock1 selects which clock is used as the measurement clock
*
* \param clock1FreqHz the frequency of clock1 in Hz
*
* \param count1 sets the initial value for count1
*
* \param clock2 selects the clock that will be measured by clock1
*
* \return Error / status code:<br>
* CY_SYSCLK_INVALID_STATE if already doing a measurement<br>
* CY_SYSCLK_BAD_PARAM if invalid clock input parameter<br>
* else CY_SYSCLK_SUCCESS
*
*******************************************************************************/
uint32_t Cy_SysClk_InitClkMeasurementCounters(cy_en_meas_clks_t clock1, uint32_t clock1FreqHz,
                                           uint32_t count1, cy_en_meas_clks_t clock2);

/*******************************************************************************
* Function Name: Cy_SysClk_StartClkMeasurementCounters
****************************************************************************//**
*
* Starts the calibration counters.
*
* \return Error / status code:<br>
* CY_SYSCLK_INVALID_STATE if already doing a measurement<br>
* else CY_SYSCLK_SUCCESS
*
* \note Cy_SysClk_InitClkMeasurementCounters() must have been called previously.
*******************************************************************************/
uint32_t Cy_SysClk_StartClkMeasurementCounters(void);

/*******************************************************************************
* Function Name: Cy_SysClk_ClkMeasurementCountersDone
****************************************************************************//**
*
* Checks if the calibration counters are done
*
* \return Status of calibration counters:<br>
* CY_SYSCLK_FINISHED if done<br>
* CY_SYSCLK_STARTED if not
*
*******************************************************************************/
__STATIC_INLINE int32_t Cy_SysClk_ClkMeasurementCountersDone(void)
{
    return ((_FLD2VAL(SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE, SRSS->CLK_CAL_CNT1) == 0ul/*0 = not done*/) ?
            CY_SYSCLK_STARTED : CY_SYSCLK_FINISHED);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkMeasurementCountersGetClock2Freq
****************************************************************************//**
*
* Returns the frequency of clock 2 in Hz. The following equation is used:
* clock2 frequency = (count2/count1) * clock1 frequency
*
* \return Frequency of clock 2 in Hz
*
* \note This must only be called if the counters are done.
*******************************************************************************/
uint32_t Cy_SysClk_ClkMeasurementCountersGetClock2Freq(void);
/** \} group_sysclk_calclk */


/**
* \addtogroup group_sysclk_trim
* \{
*/

/* ========================================================================== */
/* ==========================    TRIM SECTION    ============================ */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_IloTrim
****************************************************************************//**
*
* Trims the ILO to be as close to 32,768 Hz as possible. The clock measurement
* counters should be used to obtain the current frequency of the ILO. This
* frequency should then be passed into this function. This function will then
* use this information to trim the ILO as close as possible to 32,768 Hz.
*
* \param iloFreq current ILO frequency as measured by the measurement counters
*
* \return The achieved ILO frequency in Hz
*
* \note The watchdog timer (WDT) must be unlocked before calling this function.
*******************************************************************************/
uint32_t Cy_SysClk_IloTrim(uint32_t iloFreq);

/*******************************************************************************
* Function Name: Cy_SysClk_PiloTrim
****************************************************************************//**
*
* Trims the PILO to be as close to 32,768 Hz as possible. The clock measurement
* counters should be used to obtain the current frequency of the PILO. This
* frequency should then be passed into this function. This function will then
* use this information to trim the PILO as close as possible to 32,768Hz.
*
* \param piloFreq current PILO frequency as measured by the measurement counters
*
* \return The achieved PILO frequency in Hz
*
*******************************************************************************/
uint32_t Cy_SysClk_PiloTrim(uint32_t piloFreq);

/** \} group_sysclk_trim */


/**
* \addtogroup group_sysclk_wco
* \{
*/

/* ========================================================================== */
/* ===========================    WCO SECTION    ============================ */
/* ========================================================================== */
/** WCO bypass modes */
typedef enum
{
    CY_SYSCLK_WCO_NOT_BYPASSED = 0u, /**< WCO is not bypassed crystal is used */
    CY_SYSCLK_WCO_BYPASSED = 1u      /**< WCO is bypassed external clock must be supplied  on XTAL pin */
} cy_en_wco_bypass_modes_t;

/** WCO CSV supervisor clock selections */
typedef enum
{
    CY_SYSCLK_WCO_CSV_SUPERVISOR_ILO,
    CY_SYSCLK_WCO_CSV_SUPERVISOR_ALTLF,
    CY_SYSCLK_WCO_CSV_SUPERVISOR_PILO
} cy_en_wco_csv_supervisor_clock_t;

/**
* Clock supervisor clock loss window. There must be one clock of the supervised
* clock within this many clocks of the supervising clock.
* See registers CLK_CSV_HF_CTL and CLK_CSV_WCO_CTL, bitfield CSV_LOSS_WINDOW.
*/
typedef enum
{
    CY_SYSCLK_CSV_LOSS_4_CYCLES =   0u, /**< 1 clock must be seen within 4 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_8_CYCLES =   1u, /**< 1 clock must be seen within 8 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_16_CYCLES =  2u, /**< 1 clock must be seen within 16 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_32_CYCLES =  3u, /**< 1 clock must be seen within 32 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_64_CYCLES =  4u, /**< 1 clock must be seen within 64 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_128_CYCLES = 5u, /**< 1 clock must be seen within 128 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_256_CYCLES = 6u, /**< 1 clock must be seen within 256 cycles of the supervising clock. */
    CY_SYSCLK_CSV_LOSS_512_CYCLES = 7u  /**< 1 clock must be seen within 512 cycles of the supervising clock. */
} cy_en_csv_loss_window_t;

/**
* Clock supervisor error actions. See register CLK_CSV_HF_CTL[CSV_FREQ_ACTION and CSV_LOSS_ACTION].
*/
typedef enum
{
    CY_SYSCLK_CSV_ERROR_IGNORE = 0u,     /**< Ignore the error reported by the clock supervisor. */
    CY_SYSCLK_CSV_ERROR_FAULT = 1u,      /**< Trigger a fault when an error is reported by the clock supervisor. */
    CY_SYSCLK_CSV_ERROR_RESET = 2u,      /**< Trigger a reset when an error is reported by the clock supervisor. */
    CY_SYSCLK_CSV_ERROR_FAULT_RESET = 3u /**< Trigger a fault then reset when an error is reported by the supervisor. */
} cy_en_csv_error_actions_t;

/**
* This structure is used to configure the clock supervisor for the WCO.
*/
typedef struct
{
    cy_en_wco_csv_supervisor_clock_t SupervisorClock; /**< supervisor clock selection */
    uint32_t enableLossDetection;                  /**< 1= enabled, 0= disabled. Note that if loss detection is enabled, writes to other register bits are ignored. */
    cy_en_csv_loss_window_t LossWindow;               /**< \ref cy_en_csv_loss_window_t */
    cy_en_csv_error_actions_t LossAction;             /**< \ref cy_en_csv_error_actions_t */
} cy_stc_wco_csv_config_t;

/*******************************************************************************
* Function Name: Cy_SysClk_WcoEnable
****************************************************************************//**
*
* Enables the WCO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_WcoEnable(void)
{
    BACKUP->CTL |= _VAL2FLD(BACKUP_CTL_WCO_EN, 1u); /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_WcoDisable
****************************************************************************//**
*
* Disables the WCO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_WcoDisable(void)
{
    BACKUP->CTL &= ~_VAL2FLD(BACKUP_CTL_WCO_EN, 1u); /* 0 = disable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_WcoBypass
****************************************************************************//**
*
* Sets whether the WCO is bypassed or not. If it is bypassed then a 32-kHz clock
* must be provided on the wco_in pin.
*
* \param bypass \ref cy_en_wco_bypass_modes_t
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_WcoBypass(cy_en_wco_bypass_modes_t bypass)
{
    CY_SYSCLK_CLR_SET(BACKUP->CTL, BACKUP_CTL_WCO_BYPASS, bypass);
}

/*******************************************************************************
* Function Name: Cy_SysClk_WcoConfigureCsv
****************************************************************************//**
*
* Configure the WCO clock supervisor (CSV).
*
* \param config \ref cy_stc_wco_csv_config_t
*
* \note
* If loss detection is enabled, writes to other register bits are ignored.
* Therefore loss detection is disabled before writing the config structure
* members to the CTL register. Note that one of the config structure members is
* an enable bit.
*******************************************************************************/
void Cy_SysClk_WcoConfigureCsv(const cy_stc_wco_csv_config_t *config);
/** \} group_sysclk_wco */


/**
* \addtogroup group_sysclk_clk_hf
* \{
*/

/* ========================================================================== */
/* ========================    clk_hf[n] SECTION    ========================= */
/* ========================================================================== */
/**
* Selects which hf_clk input, or root mux, to configure.
* See CLK_ROOT_SELECT registers, bits ROOT_MUX.
* Used with functions \ref Cy_SysClk_HfClockSetSource and \ref Cy_SysClk_HfClockGetSource.
*/
typedef enum
{
    CY_SYSCLK_HFCLK_IN_CLKPATH0  = 0u,  /**< hf_clk input is Clock Path 0 */
    CY_SYSCLK_HFCLK_IN_CLKPATH1  = 1u,  /**< hf_clk input is Clock Path 1 */
    CY_SYSCLK_HFCLK_IN_CLKPATH2  = 2u,  /**< hf_clk input is Clock Path 2 */
    CY_SYSCLK_HFCLK_IN_CLKPATH3  = 3u,  /**< hf_clk input is Clock Path 3 */
    CY_SYSCLK_HFCLK_IN_CLKPATH4  = 4u,  /**< hf_clk input is Clock Path 4 */
    CY_SYSCLK_HFCLK_IN_CLKPATH5  = 5u,  /**< hf_clk input is Clock Path 5 */
    CY_SYSCLK_HFCLK_IN_CLKPATH6  = 6u,  /**< hf_clk input is Clock Path 6 */
    CY_SYSCLK_HFCLK_IN_CLKPATH7  = 7u,  /**< hf_clk input is Clock Path 7 */
    CY_SYSCLK_HFCLK_IN_CLKPATH8  = 8u,  /**< hf_clk input is Clock Path 8 */
    CY_SYSCLK_HFCLK_IN_CLKPATH9  = 9u,  /**< hf_clk input is Clock Path 9 */
    CY_SYSCLK_HFCLK_IN_CLKPATH10 = 10u, /**< hf_clk input is Clock Path 10 */
    CY_SYSCLK_HFCLK_IN_CLKPATH11 = 11u, /**< hf_clk input is Clock Path 11 */
    CY_SYSCLK_HFCLK_IN_CLKPATH12 = 12u, /**< hf_clk input is Clock Path 12 */
    CY_SYSCLK_HFCLK_IN_CLKPATH13 = 13u, /**< hf_clk input is Clock Path 13 */
    CY_SYSCLK_HFCLK_IN_CLKPATH14 = 14u, /**< hf_clk input is Clock Path 14 */
    CY_SYSCLK_HFCLK_IN_CLKPATH15 = 15u, /**< hf_clk input is Clock Path 15 */
} cy_en_hf_clk_sources_t;

/**
* hf_clk divider values. See CLK_ROOT_SELECT registers, bits ROOT_DIV.
* Used with functions \ref Cy_SysClk_HfClockSetDivider and \ref Cy_SysClk_HfClockGetDivider.
*/
typedef enum
{
    CY_SYSCLK_HFCLK_NO_DIVIDE   = 0u,    /**< don't divide hf_clk. */
    CY_SYSCLK_HFCLK_DIVIDE_BY_2 = 1u,    /**< divide hf_clk by 2 */
    CY_SYSCLK_HFCLK_DIVIDE_BY_4 = 2u,    /**< divide hf_clk by 4 */
    CY_SYSCLK_HFCLK_DIVIDE_BY_8 = 3u     /**< divide hf_clk by 8 */
} cy_en_hf_clk_dividers_t;

/**
* hf_clk clock supervisor input sources. See register CLK_CSV_HF_CTL[CSV_MUX].
*/
typedef enum
{
    CY_SYSCLK_HFCLK_CSV_SUPERVISOR_IMO   = 0u,  /**< Supervising clock is the IMO. */
    CY_SYSCLK_HFCLK_CSV_SUPERVISOR_EXT   = 1u,  /**< Supervising clock is the external clock */
    CY_SYSCLK_HFCLK_CSV_SUPERVISOR_ALTHF = 2u   /**< Supervising clock is clk_althf */
} cy_en_hfclk_csv_supervisor_clock_t;

/**
* This structure is used to configure the clock supervisor for hf_clk.
*/
typedef struct
{
    cy_en_hfclk_csv_supervisor_clock_t SupervisorClock; /**< \ref cy_en_hfclk_csv_supervisor_clock_t */
    uint32_t SupervisingWindow;                         /**< Number of supervising clock cycles */
    uint32_t enableFrquencyFaultDetection;              /**< 1= enabled, 0= disabled */
    uint32_t FrequencyLowerLimit;                       /**< Lowest frequency in kHz that supervised clock can go */
    uint32_t FrequencyUpperLimit;                       /**< Highest frequency in kHz that supervised clock can go */
    cy_en_csv_error_actions_t FrequencyAction;          /**< \ref cy_en_csv_error_actions_t */
    uint32_t enableLossDetection;                       /**< 1= enabled, 0= disabled */
    cy_en_csv_loss_window_t LossWindow;                 /**< \ref cy_en_csv_loss_window_t */
    cy_en_csv_error_actions_t LossAction;               /**< \ref cy_en_csv_error_actions_t */
} cy_stc_hf_csv_config_t;

/*******************************************************************************
* Function Name: Cy_SysClk_ClkHfEnable
****************************************************************************//**
*
* Enables the selected ClkHf.
*
* \param clkHf Selects which ClkHf to enable.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkHfEnable(uint32_t clkHf)
{
    SRSS->CLK_ROOT_SELECT[clkHf] |= _VAL2FLD(SRSS_CLK_ROOT_SELECT_ENABLE, 1ul); /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkHfDisable
****************************************************************************//**
*
* Disables the selected ClkHf.
*
* \param clkHf Selects which ClkHf to enable.
*
* \note ClkHf[0] cannot be disabled.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkHfDisable(uint32_t clkHf)
{
    SRSS->CLK_ROOT_SELECT[clkHf] &= ~_VAL2FLD(SRSS_CLK_ROOT_SELECT_ENABLE, 1ul); /* 0 = disable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_HfClockSetSource
****************************************************************************//**
*
* Selects the source of the selected ClkHf.
*
* \param hfClk Which ClkHf mux to configure.
*
* \param source \ref cy_en_hf_clk_sources_t
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_HfClockSetSource(uint32_t hfClk, cy_en_hf_clk_sources_t source)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_ROOT_SELECT[hfClk], SRSS_CLK_ROOT_SELECT_ROOT_MUX, source);
}

/*******************************************************************************
* Function Name: Cy_SysClk_HfClockGetSource
****************************************************************************//**
*
* Reports the source of the selected ClkHf.
*
* \param hfClk selects which ClkHf to get the source of.
*
* \return \ref cy_en_hf_clk_sources_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_hf_clk_sources_t Cy_SysClk_HfClockGetSource(uint32_t hfClk)
{
    return (cy_en_hf_clk_sources_t)(_FLD2VAL(SRSS_CLK_ROOT_SELECT_ROOT_MUX, SRSS->CLK_ROOT_SELECT[hfClk]));
}

/*******************************************************************************
* Function Name: Cy_SysClk_HfClockSetDivider
****************************************************************************//**
*
* Sets the pre-divider for a ClkHf.
*
* \param hfClk selects which ClkHf divider to configure.
*
* \param divider \ref cy_en_hf_clk_dividers_t
*
* \note Also call \ref Cy_SysClk_HfClockSetSource to set the ClkHf source.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_HfClockSetDivider(uint32_t hfClk, cy_en_hf_clk_dividers_t divider)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_ROOT_SELECT[hfClk], SRSS_CLK_ROOT_SELECT_ROOT_DIV, divider);
}

/*******************************************************************************
* Function Name: Cy_SysClk_HfClockGetDivider
****************************************************************************//**
*
* Reports the pre-divider value for a ClkHf.
*
* \param hfClk selects which ClkHf to check divider of.
*
* \return \ref cy_en_hf_clk_dividers_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_hf_clk_dividers_t Cy_SysClk_HfClockGetDivider(uint32_t hfClk)
{
    return (cy_en_hf_clk_dividers_t)(_FLD2VAL(SRSS_CLK_ROOT_SELECT_ROOT_DIV, SRSS->CLK_ROOT_SELECT[hfClk]));
}

/*******************************************************************************
* Function Name: Cy_SysClk_HfClockConfigureCsv
****************************************************************************//**
*
* Configures the ClkHf clock supervisor (CSV).
*
* \param hfClk selects which ClkHf CSV to configure.
*
* \param config \ref cy_stc_hf_csv_config_t
*
* \note
* If loss detection is enabled, writes to other register bits are ignored.
* Therefore loss detection is disabled before writing the config structure
* members to the CTL register. Note that one of the config structure members is
* an enable bit.
*******************************************************************************/
void Cy_SysClk_HfClockConfigureCsv(uint32_t hfClk, const cy_stc_hf_csv_config_t *config);
/** \} group_sysclk_clk_hf */


/**
* \addtogroup group_sysclk_clk_fast
* \{
*/

/* ========================================================================== */
/* =========================    clk_fast SECTION    ========================= */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_FastClkSetDivider
****************************************************************************//**
*
* Sets the clock divider for the fast clock, which sources the main processor.
* The source of this divider is hf_clk[0].
*
* \param divider divider value between 0 and 255.
* Causes integer division of (divider value + 1), or division by 1 to 256.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_FastClkSetDivider(uint32_t divider)
{
    CY_SYSCLK_CLR_SET(CPUSS->CM4_CLOCK_CTL, CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, divider);
}

/*******************************************************************************
* Function Name: Cy_SysClk_FastClkGetDivider
****************************************************************************//**
*
* Returns the clock divider for the fast clock.
*
* \return The divider value for the fast clock.
* The integer division done is by (divider value + 1), or division by 1 to 256.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_FastClkGetDivider(void)
{
    return _FLD2VAL(CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV, CPUSS->CM4_CLOCK_CTL);
}
/** \} group_sysclk_clk_fast */


/**
* \addtogroup group_sysclk_clk_peripheral
* \{
*/

/* ========================================================================== */
/* ========================    clk_peri SECTION    ========================== */
/* ========================================================================== */
/** Programmable clock divider types */
typedef enum
{
    CY_SYSCLK_DIV_8_BIT    = 0u, /**< Divider Type is an 8 bit divider */
    CY_SYSCLK_DIV_16_BIT   = 1u, /**< Divider Type is a 16 bit divider */
    CY_SYSCLK_DIV_16_5_BIT = 2u, /**< Divider Type is a 16.5 bit fractional divider */
    CY_SYSCLK_DIV_24_5_BIT = 3u  /**< Divider Type is a 24.5 bit fractional divider */
} cy_en_divider_types_t;

/*******************************************************************************
* Function Name: Cy_SysClk_PeriClkSetDivider
****************************************************************************//**
*
* Sets the clock divider for the peripheral clock tree. All peripheral clock
* dividers are sourced from this clock. Also the Cortex M0+ clock divider is
* sourced from this clock. The source of this divider is hf_clk[0]
*
* \param divider divider value between 0 and 255
* Causes integer division of (divider value + 1), or division by 1 to 256.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriClkSetDivider(uint32_t divider)
{
    CY_SYSCLK_CLR_SET(CPUSS->CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_PERI_INT_DIV, divider);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriClkGetDivider
****************************************************************************//**
*
* Returns the clock divider of peripheral (peri) clock
*
* \return The divider value.
* The integer division done is by (divider value + 1), or division by 1 to 256.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PeriClkGetDivider(void)
{
    return _FLD2VAL(CPUSS_CM0_CLOCK_CTL_PERI_INT_DIV, CPUSS->CM0_CLOCK_CTL);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphSetDivider
****************************************************************************//**
*
* Sets one of the programmable clock dividers. This is only used for integer
* dividers. Use \ref Cy_SysClk_PeriphSetFracDivider for setting factional dividers.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum the divider number.
*
* \param dividerValue divider value
* Causes integer division of (divider value + 1), or division by 1 to 256
* (8-bit divider) or 1 to 65536 (16-bit divider).
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphSetDivider(cy_en_divider_types_t dividerType,
                                                uint32_t dividerNum, uint32_t dividerValue)
{
    switch(dividerType)
    {
        case CY_SYSCLK_DIV_8_BIT:
            CY_SYSCLK_CLR_SET(PERI->DIV_8_CTL[dividerNum], PERI_DIV_8_CTL_INT8_DIV, dividerValue);
            break;
        case CY_SYSCLK_DIV_16_BIT:
            CY_SYSCLK_CLR_SET(PERI->DIV_16_CTL[dividerNum], PERI_DIV_16_CTL_INT16_DIV, dividerValue);
            break;
        /* the following divider types are invalid for this function, and are ignored */
        case CY_SYSCLK_DIV_16_5_BIT:
        case CY_SYSCLK_DIV_24_5_BIT:
        default:
            break;
    }
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphGetDivider
****************************************************************************//**
*
* Returns the integer divider value for the specified divider. One works for
* integer dividers. Use \ref Cy_SysClk_PeriphGetFracDivider to get the fractional
* divider value
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
* \return The divider value.
* The integer division done is by (divider value + 1), or division by 1 to 256
* (8-bit divider) or 1 to 65536 (16-bit divider).
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PeriphGetDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    uint32_t retval = 0; /* returns 0 if invalid dividerType */

    switch(dividerType)
    {
        case CY_SYSCLK_DIV_8_BIT:
            retval = _FLD2VAL(PERI_DIV_8_CTL_INT8_DIV, PERI->DIV_8_CTL[dividerNum]);
            break;
        case CY_SYSCLK_DIV_16_BIT:
            retval = _FLD2VAL(PERI_DIV_16_CTL_INT16_DIV, PERI->DIV_16_CTL[dividerNum]);
            break;
        /* the following divider types are invalid for this function, and result in an error return */
        case CY_SYSCLK_DIV_16_5_BIT:
        case CY_SYSCLK_DIV_24_5_BIT:
        default:
            break;
    }

    return retval;
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphSetFracDivider
****************************************************************************//**
*
* Sets one of the programmable clock dividers. This function should only be used
* for fractional clock dividers.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
* \param dividerIntValue the integer divider value
* The source of the divider is peri_clk, which is a divided version of hf_clk[0].
* The divider value causes integer division of (divider value + 1), or division
* by 1 to 65536 (16-bit divider) or 1 to 16777216 (24-bit divider).
*
* \param dividerFracValue the fraction part of the divider
* The fractional divider can be 1-32, thus it divides the clock by 1/32 for each
* count. To divide the clock by 11/32nds set this value to 11.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphSetFracDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                    uint32_t dividerIntValue, uint32_t dividerFracValue)
{ /* see CDT 238859 */
    switch(dividerType)
    {
        case CY_SYSCLK_DIV_16_5_BIT:
            CY_SYSCLK_CLR_SET(PERI->DIV_16_5_CTL[dividerNum], PERI_DIV_16_5_CTL_INT16_DIV, dividerIntValue);
            CY_SYSCLK_CLR_SET(PERI->DIV_16_5_CTL[dividerNum], PERI_DIV_16_5_CTL_FRAC5_DIV, dividerFracValue);
            break;
        case CY_SYSCLK_DIV_24_5_BIT:
            CY_SYSCLK_CLR_SET(PERI->DIV_24_5_CTL[dividerNum], PERI_DIV_24_5_CTL_INT24_DIV, dividerIntValue);
            CY_SYSCLK_CLR_SET(PERI->DIV_24_5_CTL[dividerNum], PERI_DIV_24_5_CTL_FRAC5_DIV, dividerFracValue);
            break;
        /* the following divider types are invalid for this function, and are ignored */
        case CY_SYSCLK_DIV_8_BIT:
        case CY_SYSCLK_DIV_16_BIT:
        default:
            break;
    }
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphGetFracDivider
****************************************************************************//**
*
* Reports the integer and fractional parts of the divider
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
* \param *dividerIntValue pointer to return integer divider value
*
* \param *dividerFracValue pointer to return fractional divider value
*
* \return None. Loads pointed-to variables.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphGetFracDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                    uint32_t *dividerIntValue, uint32_t *dividerFracValue)
{
    switch(dividerType)
    {
        case CY_SYSCLK_DIV_16_5_BIT:
            *dividerIntValue  = _FLD2VAL(PERI_DIV_16_5_CTL_INT16_DIV, PERI->DIV_16_5_CTL[dividerNum]);
            *dividerFracValue = _FLD2VAL(PERI_DIV_16_5_CTL_FRAC5_DIV, PERI->DIV_16_5_CTL[dividerNum]);
            break;
        case CY_SYSCLK_DIV_24_5_BIT:
            *dividerIntValue  = _FLD2VAL(PERI_DIV_24_5_CTL_INT24_DIV, PERI->DIV_24_5_CTL[dividerNum]);
            *dividerFracValue = _FLD2VAL(PERI_DIV_24_5_CTL_FRAC5_DIV, PERI->DIV_24_5_CTL[dividerNum]);
            break;
        /* the following divider types are invalid for this function, and are ignored */
        case CY_SYSCLK_DIV_8_BIT:
        case CY_SYSCLK_DIV_16_BIT:
        default:
            break;
    }
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphAssignDivider
****************************************************************************//**
*
* Assigns a programmable divider to a selected IP block, such as a TCPWM or SCB.
*
* \param ipBlock specifies ip block to connect the clock divider to.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphAssignDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType,
                                                   uint32_t dividerNum)
{
    PERI->CLOCK_CTL[ipBlock] = _VAL2FLD(PERI_CLOCK_CTL_TYPE_SEL, dividerType) |
                               _VAL2FLD(PERI_CLOCK_CTL_DIV_SEL,  dividerNum);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphGetAssignedDivider
****************************************************************************//**
*
* Reports which clock divider is assigned to a selected IP block.
*
* \param ipBlock specifies ip block to connect the clock divider to.
*
* \return The divider type and #, where bits [7:6] = type, bits[5:0] = divider
* number within that type
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_PeriphGetAssignedDivider(en_clk_dst_t ipBlock)
{
    return PERI->CLOCK_CTL[ipBlock] & 0xFFul; /* bits [7:6] = TYPE_SEL, bits[5:0] = DIV_SEL */
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphEnableDivider
****************************************************************************//**
*
* Enables the selected divider.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphEnableDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    PERI->DIV_CMD = _CLR_SET_FLD32U(PERI->DIV_CMD, PERI_DIV_CMD_TYPE_SEL, dividerType);
    PERI->DIV_CMD = _CLR_SET_FLD32U(PERI->DIV_CMD, PERI_DIV_CMD_DIV_SEL, dividerNum);
    PERI->DIV_CMD = _CLR_SET_FLD32U(PERI->DIV_CMD, PERI_DIV_CMD_ENABLE, 1ul);  /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphDisableDivider
****************************************************************************//**
*
* Disables the selected divider.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t.
*
* \param dividerNum specifies which divider of the selected type to configure.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphDisableDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    PERI->DIV_CMD = _CLR_SET_FLD32U(PERI->DIV_CMD, PERI_DIV_CMD_TYPE_SEL, dividerType);
    PERI->DIV_CMD = _CLR_SET_FLD32U(PERI->DIV_CMD, PERI_DIV_CMD_DIV_SEL, dividerNum);
    PERI->DIV_CMD = _CLR_SET_FLD32U(PERI->DIV_CMD, PERI_DIV_CMD_DISABLE, 1);  /* 1 = disable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphPhaseDisableDivider
****************************************************************************//**
*
* Disables a selected divider, preparatory to aligning it with another divider;
* see \ref Cy_SysClk_PeriphPhaseAlignDivider.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t.
*
* \param dividerNum specifies which divider of the selected type to configure.
*
* \note
* To phase-align a divider, do the following:
* 1. Call this function.
* 2. Call the appropriate Cy_SysClk_PeriphSet...Divider function to configure the
*    divider.
* 3. Call Cy_SysClk_PeriphPhaseAlignDivider to enable the divider and do the phase
*    alignment.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphPhaseDisableDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    /* specify the divider and disable it */
    PERI->DIV_CMD = _VAL2FLD(PERI_DIV_CMD_DISABLE, 1u) /* 1 = disable */ |
                    _VAL2FLD(PERI_DIV_CMD_TYPE_SEL, dividerType)        |
                    _VAL2FLD(PERI_DIV_CMD_DIV_SEL,  dividerNum);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphPhaseAlignDivider
****************************************************************************//**
*
* First disables a selected divider (\ref Cy_SysClk_PeriphPhaseDisableDivider),
* then aligns that divider to another programmable divider, and enables the
* selected divider.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t.
*
* \param dividerNum specifies which divider of the selected type to configure.
*
* \param dividerTypePA type of divider to phase-align to; \ref cy_en_divider_types_t.
*
* \param dividerNumPA divider number of type specified to phase align to.
*
* \note
* To phase-align a divider to clk_peri, set dividerTypePA to 3 and dividerNumPA
* to 255.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_PeriphPhaseAlignDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                       cy_en_divider_types_t dividerTypePA, uint32_t dividerNumPA)
{
    /* first disable the divider that is to be phase-aligned */
    Cy_SysClk_PeriphPhaseDisableDivider(dividerType, dividerNum);
    /* then specify the reference divider, and the divider, and enable the divider */
    PERI->DIV_CMD = _VAL2FLD(PERI_DIV_CMD_ENABLE, 1u) /* 1 = enable */ |
                    _VAL2FLD(PERI_DIV_CMD_PA_TYPE_SEL, dividerTypePA) |
                    _VAL2FLD(PERI_DIV_CMD_PA_DIV_SEL,  dividerNumPA)  |
                    _VAL2FLD(PERI_DIV_CMD_TYPE_SEL,    dividerType)   |
                    _VAL2FLD(PERI_DIV_CMD_DIV_SEL,     dividerNum);
}

/*******************************************************************************
* Function Name: Cy_SysClk_PeriphGetFrequency
****************************************************************************//**
*
* Reports the frequency of the output of a given peripheral divider.
*
* \param dividerType specifies which type of divider to use; \ref cy_en_divider_types_t
*
* \param dividerNum specifies which divider of the selected type to configure
*
* \return The frequency, in Hz.
*
* \note
* The reported frequency may be zero, which indicates unknown. This happens if
* the source input is clk_ext, ECO, clk_althf, dsi_out, or clk_altlf.
*
*******************************************************************************/
uint32_t Cy_SysClk_PeriphGetFrequency(cy_en_divider_types_t dividerType, uint32_t dividerNum);
/** \} group_sysclk_clk_peripheral */


/**
* \addtogroup group_sysclk_clk_slow
* \{
*/

/* ========================================================================== */
/* =========================    clk_slow SECTION    ========================= */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_SlowClkSetDivider
****************************************************************************//**
*
* Sets the clock divider for the slow clock. The source of this clock is the
* peripheral clock (ClkPeri), which is sourced from hf_clk[0].
*
* \param divider Divider value between 0 and 255.
* Causes integer division of (divider value + 1), or division by 1 to 256.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_SlowClkSetDivider(uint32_t divider)
{
    CY_SYSCLK_CLR_SET(CPUSS->CM0_CLOCK_CTL, CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, divider);
}

/*******************************************************************************
* Function Name: Cy_SysClk_SlowClkGetDivider
****************************************************************************//**
*
* Reports the divider value for the slow clock.
*
* \return The divider value.
* The integer division done is by (divider value + 1), or division by 1 to 256.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_SlowClkGetDivider(void)
{
    return _FLD2VAL(CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV, CPUSS->CM0_CLOCK_CTL);
}
/** \} group_sysclk_clk_slow */


/**
* \addtogroup group_sysclk_clk_lf
* \{
*/

/* ========================================================================== */
/* ==========================    clk_lf SECTION    ========================== */
/* ========================================================================== */
/**
* Low frequency (lf_clk) input sources. See CLK_SELECT register, LFCLK_SEL bits.
* Used with functions \ref Cy_SysClk_LfClkSetSource, and \ref Cy_SysClk_LfClkGetSource.
*/
typedef enum
{
    CY_SYSCLK_LFCLK_IN_ILO   = 0u, /**< lf_clk is sourced by the internal low speed oscillator (ILO) */
    CY_SYSCLK_LFCLK_IN_WCO   = 1u, /**< lf_clk is sourced by the watch crystal oscillator (WCO) */
    CY_SYSCLK_LFCLK_IN_ALTLF = 2u, /**< lf_clk is sourced by the Alternate Low Frequency Clock (ALTLF) */
    CY_SYSCLK_LFCLK_IN_PILO  = 3u  /**< lf_clk is sourced by the precision low speed oscillator (PILO) */
} cy_en_lfclk_input_sources_t;


/*******************************************************************************
* Function Name: Cy_SysClk_LfClkSetSource
****************************************************************************//**
*
* Sets the source for the low frequency clock(lf_clk).
*
* \param source \ref cy_en_lfclk_input_sources_t
*
* \note The watchdog timer (WDT) must be unlocked before calling this function.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_LfClkSetSource(cy_en_lfclk_input_sources_t source)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_SELECT, SRSS_CLK_SELECT_LFCLK_SEL, source);
}

/*******************************************************************************
* Function Name: Cy_SysClk_LfClkGetSource
****************************************************************************//**
*
* Reports the source for the low frequency clock (lf_clk).
*
* \return \ref cy_en_lfclk_input_sources_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_lfclk_input_sources_t Cy_SysClk_LfClkGetSource(void)
{
    return (cy_en_lfclk_input_sources_t)(_FLD2VAL(SRSS_CLK_SELECT_LFCLK_SEL, SRSS->CLK_SELECT));
}
/** \} group_sysclk_clk_lf */


/**
* \addtogroup group_sysclk_clk_timer
* \{
*/

/* ========================================================================== */
/* ========================    clk_timer SECTION    ========================= */
/* ========================================================================== */
/**
* Timer clock (clk_timer) input sources. See CLK_TIMER_CTL register, TIMER_SEL
* and TIMER_HF0_DIV bits. Used with functions \ref Cy_SysClk_ClkTimerSetSource, and
* \ref Cy_SysClk_ClkTimerGetSource.
*/
typedef enum
{
    CY_SYSCLK_CLKTIMER_IN_IMO       =     0u, /**< clk_timer is sourced by the internal main oscillator (IMO) */
    CY_SYSCLK_CLKTIMER_IN_HF0_NODIV =     1u, /**< clk_timer is sourced by clk_hf0 undivided */
    CY_SYSCLK_CLKTIMER_IN_HF0_DIV2  = 0x101u, /**< clk_timer is sourced by clk_hf0 divided by 2 */
    CY_SYSCLK_CLKTIMER_IN_HF0_DIV4  = 0x201u, /**< clk_timer is sourced by clk_hf0 divided by 4 */
    CY_SYSCLK_CLKTIMER_IN_HF0_DIV8  = 0x301u  /**< clk_timer is sourced by clk_hf0 divided by 8 */
} cy_en_clktimer_input_sources_t;


/*******************************************************************************
* Function Name: Cy_SysClk_ClkTimerSetSource
****************************************************************************//**
*
* Sets the source for the timer clock (clk_timer). The timer clock can be used
* as a source for SYSTICK and one or more of the energy profiler counters.
*
* \param source \ref cy_en_clktimer_input_sources_t
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkTimerSetSource(cy_en_clktimer_input_sources_t source)
{
    /* set both fields TIMER_SEL and TIMER_HF0_DIV with the same input value */
    SRSS->CLK_TIMER_CTL =
        (SRSS->CLK_TIMER_CTL & ~(SRSS_CLK_TIMER_CTL_TIMER_SEL_Msk | SRSS_CLK_TIMER_CTL_TIMER_HF0_DIV_Msk)) |
        (uint32_t)source;
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkTimerGetSource
****************************************************************************//**
*
* Reports the source for the timer clock (clk_timer).
*
* \return \ref cy_en_clktimer_input_sources_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_clktimer_input_sources_t Cy_SysClk_ClkTimerGetSource(void)
{
    /* return both fields TIMER_SEL and TIMER_HF0_DIV as a single combined value */
    return (cy_en_clktimer_input_sources_t)
           (SRSS->CLK_TIMER_CTL & (SRSS_CLK_TIMER_CTL_TIMER_SEL_Msk | SRSS_CLK_TIMER_CTL_TIMER_HF0_DIV_Msk));
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkTimerSetDivider
****************************************************************************//**
*
* Sets the divider for the timer clock (clk_timer).
*
* \param divider Divider value; valid range is 0 to 255. Divides the selected
* source (\ref Cy_SysClk_ClkTimerSetSource) by the (value + 1).
*
* \note
* Do not change the divider value while the timer clock is enabled.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkTimerSetDivider(uint32_t divider)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_TIMER_CTL, SRSS_CLK_TIMER_CTL_TIMER_DIV, divider);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkTimerGetDivider
****************************************************************************//**
*
* Reports the divider value for the timer clock (clk_timer).
*
* \return The divider value
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysClk_ClkTimerGetDivider(void)
{
    return (uint32_t)_FLD2VAL(SRSS_CLK_TIMER_CTL_TIMER_DIV, SRSS->CLK_TIMER_CTL);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkTimerEnable
****************************************************************************//**
*
* Enables the timer clock (clk_timer). The timer clock can be used as a source
* for SYSTICK and one or more of the energy profiler counters.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkTimerEnable(void)
{
    SRSS->CLK_TIMER_CTL |= _VAL2FLD(SRSS_CLK_TIMER_CTL_ENABLE, 1ul); /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkTimerDisable
****************************************************************************//**
*
* Disables the timer clock (clk_timer).
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkTimerDisable(void)
{
    SRSS->CLK_TIMER_CTL &= ~_VAL2FLD(SRSS_CLK_TIMER_CTL_ENABLE, 1ul); /* 0 = disable */
}
/** \} group_sysclk_clk_timer */

/**
* \addtogroup group_sysclk_clk_pump
* \{
*/

/* ========================================================================== */
/* =========================    clk_pump SECTION    ========================= */
/* ========================================================================== */
/**
* Pump clock (clk_pump) input sources. See CLK_SELECT register, PUMP_SEL bits.
* Used with functions \ref Cy_SysClk_ClkPumpSetSource, and
* \ref Cy_SysClk_ClkPumpGetSource.
*/
typedef enum
{
    CY_SYSCLK_PUMP_IN_CLKPATH0,  /**< Pump clock input is clock path 0 */
    CY_SYSCLK_PUMP_IN_CLKPATH1,  /**< Pump clock input is clock path 1 */
    CY_SYSCLK_PUMP_IN_CLKPATH2,  /**< Pump clock input is clock path 2 */
    CY_SYSCLK_PUMP_IN_CLKPATH3,  /**< Pump clock input is clock path 3 */
    CY_SYSCLK_PUMP_IN_CLKPATH4,  /**< Pump clock input is clock path 4 */
    CY_SYSCLK_PUMP_IN_CLKPATH5,  /**< Pump clock input is clock path 5 */
    CY_SYSCLK_PUMP_IN_CLKPATH6,  /**< Pump clock input is clock path 6 */
    CY_SYSCLK_PUMP_IN_CLKPATH7,  /**< Pump clock input is clock path 7 */
    CY_SYSCLK_PUMP_IN_CLKPATH8,  /**< Pump clock input is clock path 8 */
    CY_SYSCLK_PUMP_IN_CLKPATH9,  /**< Pump clock input is clock path 9 */
    CY_SYSCLK_PUMP_IN_CLKPATH10, /**< Pump clock input is clock path 10 */
    CY_SYSCLK_PUMP_IN_CLKPATH11, /**< Pump clock input is clock path 11 */
    CY_SYSCLK_PUMP_IN_CLKPATH12, /**< Pump clock input is clock path 12 */
    CY_SYSCLK_PUMP_IN_CLKPATH13, /**< Pump clock input is clock path 13 */
    CY_SYSCLK_PUMP_IN_CLKPATH14, /**< Pump clock input is clock path 14 */
    CY_SYSCLK_PUMP_IN_CLKPATH15  /**< Pump clock input is clock path 15 */
} cy_en_clkpump_input_sources_t;

/**
* Pump clock (clk_pump) divide options. See CLK_SELECT register, PUMP_DIV bits.
* Used with functions \ref Cy_SysClk_ClkPumpSetDivider, and
* \ref Cy_SysClk_ClkPumpGetDivider.
*/
typedef enum
{
    CY_SYSCLK_PUMP_NO_DIV, /**< No division on pump clock */
    CY_SYSCLK_PUMP_DIV_2,  /**< Pump clock divided by 2 */
    CY_SYSCLK_PUMP_DIV_4,  /**< Pump clock divided by 4 */
    CY_SYSCLK_PUMP_DIV_8,  /**< Pump clock divided by 8 */
    CY_SYSCLK_PUMP_DIV_16  /**< Pump clock divided by 16 */
} cy_en_clkpump_divide_t;

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpSetSource
****************************************************************************//**
*
* Sets the source for the pump clock (clk_pump). The pump clock can be used for
* the analog pumps in the CTBm block.
*
* \param source \ref cy_en_clkpump_input_sources_t
*
* \note
* Do not change the source while the pump clock is enabled.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkPumpSetSource(cy_en_clkpump_input_sources_t source)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_SELECT, SRSS_CLK_SELECT_PUMP_SEL, source);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpGetSource
****************************************************************************//**
*
* Reports the source for the pump clock (clk_pump).
*
* \return \ref cy_en_clkpump_input_sources_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_clkpump_input_sources_t Cy_SysClk_ClkPumpGetSource(void)
{
    return (cy_en_clkpump_input_sources_t)_FLD2VAL(SRSS_CLK_SELECT_PUMP_SEL, SRSS->CLK_SELECT);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpSetDivider
****************************************************************************//**
*
* Sets the divider of the pump clock (clk_pump).
*
* \param divider \ref cy_en_clkpump_divide_t
*
* \note
* Do not change the divider value while the pump clock is enabled.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkPumpSetDivider(cy_en_clkpump_divide_t divider)
{
    CY_SYSCLK_CLR_SET(SRSS->CLK_SELECT, SRSS_CLK_SELECT_PUMP_DIV, divider);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpGetDivider
****************************************************************************//**
*
* Reports the divider value for the pump clock (clk_pump).
*
* \return \ref cy_en_clkpump_divide_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_clkpump_divide_t Cy_SysClk_ClkPumpGetDivider(void)
{
    return (cy_en_clkpump_divide_t)_FLD2VAL(SRSS_CLK_SELECT_PUMP_DIV, SRSS->CLK_SELECT);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpEnable
****************************************************************************//**
*
* Enables the pump clock (clk_pump). The pump clock can be used for the analog
* pumps in the CTBm block.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkPumpEnable(void)
{
    SRSS->CLK_SELECT |= _VAL2FLD(SRSS_CLK_SELECT_PUMP_ENABLE, 1ul); /* 1 = enable */
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPumpDisable
****************************************************************************//**
*
* Disables the pump clock (clk_pump).
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkPumpDisable(void)
{
    SRSS->CLK_SELECT &= ~_VAL2FLD(SRSS_CLK_SELECT_PUMP_ENABLE, 1ul); /* 0 = disable */
}
/** \} group_sysclk_clk_pump */

/**
* \addtogroup group_sysclk_clk_bak
* \{
*/

/* ========================================================================== */
/* ==========================    clk_bak SECTION    ========================= */
/* ========================================================================== */
/**
* Backup domain clock (clk_bak) input sources. See BACKUP->CTL register,
* CLK_SEL bits. Used with functions \ref Cy_SysClk_ClkBakSetSource, and
* \ref Cy_SysClk_ClkBakGetSource.
*/
typedef enum
{
    CY_SYSCLK_BAK_IN_WCO,   /**< Backup domain clock input is WCO */
    CY_SYSCLK_BAK_IN_CLKLF  /**< Backup domain clock input is clk_lf */
} cy_en_clkbak_input_sources_t;

/*******************************************************************************
* Function Name: Cy_SysClk_ClkBakSetSource
****************************************************************************//**
*
* Sets the source for the backup domain clock (clk_bak).
*
* \param source \ref cy_en_clkbak_input_sources_t
*
* \note
* Clk_lf is not available in all power modes.  For this reason, WCO is the
* preferred source. If the WCO is routed through the clk_lf multiplexer
* (see \ref Cy_SysClk_LfClkSetSource), select WCO directly - do not select clk_lf.
*******************************************************************************/
__STATIC_INLINE void Cy_SysClk_ClkBakSetSource(cy_en_clkbak_input_sources_t source)
{
    CY_SYSCLK_CLR_SET(BACKUP->CTL, BACKUP_CTL_CLK_SEL, source);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkBakGetSource
****************************************************************************//**
*
* Reports the source for the backup domain clock (clk_bak).
*
* \return \ref cy_en_clkbak_input_sources_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_clkbak_input_sources_t Cy_SysClk_ClkBakGetSource(void)
{
    return (cy_en_clkbak_input_sources_t)_FLD2VAL(BACKUP_CTL_CLK_SEL, BACKUP->CTL);
}

/** \} group_sysclk_clk_bak */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CY_SYSCLK_H__ */

/** \} group_sysclk */


/* [] END OF FILE */
