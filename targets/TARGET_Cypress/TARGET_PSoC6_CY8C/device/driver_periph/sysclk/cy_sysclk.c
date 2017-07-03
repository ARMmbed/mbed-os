/***************************************************************************//**
* \file cy_sysclk.c
* \version 1.0
*
* Provides an API implementation of the sysclk driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_sysclk.h"
#include "syslib/cy_syslib.h"
#include <math.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* # of elements in an array */
#define  CY_SYSCLK_N_ELMTS(a)  (sizeof(a) / sizeof((a)[0]))

/* ========================================================================== */
/* ===========================    ECO SECTION    ============================ */
/* ========================================================================== */
/*******************************************************************************
* Function Name: Cy_SysClk_EcoConfigure
****************************************************************************//**
*
* This function configures the external crystal oscillator (ECO) trim bits based
* on crystal characteristics. This function should only be called when the ECO is
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
uint32_t Cy_SysClk_EcoConfigure(uint32_t freq, uint32_t cLoad, uint32_t esr, uint32_t driveLevel)
{
    /* error if ECO is not disabled - any of the 3 enable bits are set */
    uint32_t rtnval = CY_SYSCLK_INVALID_STATE;
    if ((SRSS->CLK_ECO_CONFIG & 0xE0000000ul) == 0ul)
    {
        /* calculate intemediate values */
        float32_t freqMHz = (float32_t)freq / 1000000.0f;
        float32_t maxAmplitude =
            (1000.0f * ((float32_t)sqrt((float64_t)((float32_t)driveLevel / (2.0f * (float32_t)esr))))) /
            (3.14f * freqMHz * (float32_t)cLoad);
        float32_t gm_min =
            (788.8f /*5 * 4 * 3.14 * 3.14 * 4*/ * freqMHz * freqMHz * (float32_t)cLoad * (float32_t)cLoad) /
            1000000000.0f;
        uint32_t nAmpSections = (uint32_t)(gm_min / 4.5f);

        /* error if input parameters cause erroneous intermediate values */
        rtnval = CY_SYSCLK_BAD_PARAM;
        if ((maxAmplitude >= 0.5f) && (nAmpSections <= 3ul))
        {
            uint32_t atrim, wdtrim, gtrim, rtrim, ftrim, reg;

            atrim = 2ul * (maxAmplitude < 0.6f ? 0ul :
                           (maxAmplitude < 0.7f ? 1ul :
                            (maxAmplitude < 0.8f ? 2ul :
                             (maxAmplitude < 0.9f ? 3ul :
                              (maxAmplitude < 1.15f ? 5ul :
                               (maxAmplitude < 1.275f ? 6ul : 7ul))))));

            wdtrim = 2ul * (maxAmplitude < 1.2f ? (uint32_t)(5.0f * maxAmplitude) - 2ul : 3ul);

            gtrim = (nAmpSections > 1ul ? nAmpSections :
                     (nAmpSections == 1ul ? 0ul : 1ul));

            rtrim = (freqMHz > 26.8f ? 0ul :
                     (freqMHz > 23.33f ? 1ul :
                      (freqMHz > 16.5f ? 2ul : 3ul)));

            ftrim = atrim / 2ul;

            /* update all fields of trim control register with one write, without
               changing the ITRIM field in bits [21:16]:
                 gtrim:  bits [13:12]
                 rtrim:  bits [11:10]
                 ftrim:  bits  [9:8]
                 atrim:  bits  [7:4]
                 wdtrim: bits  [2:0]
            */
            reg = (SRSS->CLK_TRIM_ECO_CTL & ~0x3FFFul);
            reg |= (gtrim  & 3ul)    << 12;
            reg |= (rtrim  & 3ul)    << 10;
            reg |= (ftrim  & 3ul)    << 8;
            reg |= (atrim  & 0x0Ful) << 4;
            reg |= (wdtrim & 7ul);
            SRSS->CLK_TRIM_ECO_CTL = reg;

            rtnval = CY_SYSCLK_SUCCESS;
        } /* if valid parameters */
    } /* if ECO not enabled */

    return (rtnval);
}

/*******************************************************************************
* Function Name: Cy_SysClk_EcoEnable
****************************************************************************//**
*
* Enables the external crystal oscillator (ECO). This function should be called
* after \ref Cy_SysClk_EcoConfigure.
*
* \param timeoutus Amount of time in microseconds to wait for the ECO to lock.
* If a lock does not occur, the ECO is stopped. To avoid waiting for a lock, set
* this paramter to 0.
*
* \return Error / status code:<br>
* CY_SYSCLK_SUCCESS - ECO successfully enabled<br>
* CY_SYSCLK_TIMEOUT - Timeout waiting for ECO lock
*
*******************************************************************************/
uint32_t Cy_SysClk_EcoEnable(uint32_t timeoutus)
{
    uint32_t rtnval;

    /* first set ECO enable */
    SRSS->CLK_ECO_CONFIG |= _VAL2FLD(SRSS_CLK_ECO_CONFIG_ECO_EN, 1ul); /* 1 = enable */

    /* now do the timeout wait for ECO_STATUS, bit ECO_OK */
    for (;
         ((_FLD2VAL(SRSS_CLK_ECO_STATUS_ECO_READY, SRSS->CLK_ECO_STATUS) == 0ul)) &&(timeoutus != 0ul);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1u);
    }

    rtnval = ((timeoutus == 0ul) ? CY_SYSCLK_TIMEOUT : CY_SYSCLK_SUCCESS);
    return rtnval;
}


/* ========================================================================== */
/* ====================    INPUT MULTIPLEXER SECTION    ===================== */
/* ========================================================================== */
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
void Cy_SysClk_ClkPathSetSource(uint32_t clkPath, cy_en_clkpath_in_sources_t source)
{
    if (source >= CY_SYSCLK_CLKPATH_IN_DSI)
    {
        SRSS->CLK_DSI_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_DSI_SELECT_DSI_MUX, (uint32_t)source);
        SRSS->CLK_PATH_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_PATH_SELECT_PATH_MUX, (uint32_t)CY_SYSCLK_CLKPATH_IN_DSIMUX);
    }
    else
    {
        SRSS->CLK_PATH_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_PATH_SELECT_PATH_MUX, (uint32_t)source);
    }
}

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
cy_en_clkpath_in_sources_t Cy_SysClk_ClkPathGetSource(uint32_t clkPath)
{
    cy_en_clkpath_in_sources_t rtnval =
        (cy_en_clkpath_in_sources_t )_FLD2VAL(SRSS_CLK_PATH_SELECT_PATH_MUX, SRSS->CLK_PATH_SELECT[clkPath]);
    if (rtnval == CY_SYSCLK_CLKPATH_IN_DSIMUX)
    {
        rtnval = (cy_en_clkpath_in_sources_t)(CY_SYSCLK_CLKPATH_IN_DSI |
                    (_FLD2VAL(SRSS_CLK_DSI_SELECT_DSI_MUX, SRSS->CLK_DSI_SELECT[clkPath])));
    }
    return rtnval;
}


/* ========================================================================== */
/* ===========================    FLL SECTION    ============================ */
/* ========================================================================== */
/* min and max FLL output frequencies, in Hz */
#define  CY_SYSCLK_MIN_FLL_CCO_OUTPUT_FREQ  48000000ul
#define  CY_SYSCLK_MIN_FLL_OUTPUT_FREQ     (CY_SYSCLK_MIN_FLL_CCO_OUTPUT_FREQ / 2u)
#define  CY_SYSCLK_MAX_FLL_OUTPUT_FREQ     200000000ul
/* FLL CCO settling time, in microseconds */
#define  CY_SYSCLK_FLL_CCO_SETTLING_TIME  1ul

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
uint32_t Cy_SysClk_FllConfigure(uint32_t inputFreq, uint32_t outputFreq, cy_en_fll_pll_output_mode_t outputMode)
{
    uint32_t returnStatus = CY_SYSCLK_SUCCESS;

    /* check for errors */
    if (_FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS->CLK_FLL_CONFIG) != 0u) /* 1 = enabled */
    {
        returnStatus = CY_SYSCLK_INVALID_STATE;
    }
    else if ((outputFreq < CY_SYSCLK_MIN_FLL_OUTPUT_FREQ) || (CY_SYSCLK_MAX_FLL_OUTPUT_FREQ < outputFreq)) /* invalid output frequency */
    {
        returnStatus = CY_SYSCLK_BAD_PARAM;
    }
    else
    { /* return status is OK */
    }

    /* no error */
    if (returnStatus == CY_SYSCLK_SUCCESS) /* no errors */
    {
        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the
           frequency parameters. */
        if (outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            cy_stc_fll_manual_config_t config;
            uint32_t ccoFreq;

            config.outputMode = outputMode;
            /* TRM step 1a,b. Determine if output division by 2 is required. */
            config.enableOutputDiv = (bool)((outputFreq < CY_SYSCLK_MIN_FLL_CCO_OUTPUT_FREQ) ? 1ul : 0ul);
            /* TRM step 1c. Compute the CCO frequency from the output division. */
            ccoFreq = outputFreq * ((uint32_t)(config.enableOutputDiv) + 1ul);
            /* TRM step 1d,e. Compute the CCO range value from the CCO frequency */
            config.ccoRange = (ccoFreq >= 150000000ul ? CY_SYSCLK_FLL_CCO_RANGE4 :
                               (ccoFreq >= 113000000ul ? CY_SYSCLK_FLL_CCO_RANGE3 :
                                (ccoFreq >=  85000000ul ? CY_SYSCLK_FLL_CCO_RANGE2 :
                                 (ccoFreq >=  64000000ul ? CY_SYSCLK_FLL_CCO_RANGE1 : CY_SYSCLK_FLL_CCO_RANGE0))));
            /* TRM step 3. Compute the FLL reference divider value.
                      Formula is REF_DIV = (1 / precision) * (inputFreq / ccoFreq)
                      Precision is assumed to be 0.0553%.
                      Therefore REF_DIV = 1808.019 * inputFreq / ccoFreq
                      Result is rounded up. */
            config.refDiv = (uint32_t)ceilf((1808.019f * (float32_t)inputFreq) / (float32_t)ccoFreq);
            /* TRM step 4. Set the lock tolerance.
                      Output frequency accuracy = precision * lock tolerance.
                      Generally a value of 5 is acceptable. */
            config.lockTolerance = 20ul;
            /* TRM step 5. Compute the FLL MULT value. Note that we can't integer multiply
                  ccoFreq * refDiv because it may overflow a 32-bit integer. */
            config.fllMult = CY_SYSCLK_DIV_ROUND(ccoFreq, CY_SYSCLK_DIV_ROUND(inputFreq, config.refDiv));
            /* TRM step 6. Calculate the settling count, using a 1-usec settling time. */
            config.settlingCount = CY_SYSCLK_DIV_ROUNDUP(inputFreq * CY_SYSCLK_FLL_CCO_SETTLING_TIME, 1000000ul);
            /* TRM step 7. Calculate the CCO igain and pgain. */
            {
                /* CCO gain values based on CCO_RANGE bitfield values */
                const float32_t kCco[] = {46.17112083f, 61.42259619f, 81.71201510f, 108.70353626f, 144.61103156f};

                /* igain and pgain bitfield values correspond to: 1/256, 1/128, ..., 4, 8 */
                const float32_t gains[] = {0.00390625f, 0.0078125f, 0.015625f, 0.03125f, 0.0625f, 0.125f, 0.25f,
                                       0.5f, 1.0f, 2.0f, 4.0f, 8.0f};

                float32_t temp = 0.9f / ((float32_t)(kCco[config.ccoRange] * ((float32_t) config.refDiv / ((float32_t)inputFreq / 1000.0f) )));

                /* find the largest IGAIN value that is less than kCCO * refDiv /inputFreq */
                for(config.igain = CY_SYSCLK_N_ELMTS(gains) - 1ul;
                    (gains[config.igain] >= temp) && (config.igain != 0ul);
                    config.igain--){}
                /* then find the largest PGAIN value such that (IGAIN + PGAIN) is less than
                   kCCO * refDiv /inputFreq */
                for(config.pgain = CY_SYSCLK_N_ELMTS(gains) - 1ul;
                    ((gains[config.igain] + gains[config.pgain]) >= temp) && (config.pgain != 0ul);
                    config.pgain--){}
                /* final check for exceeding limit; adjust IGAIN and PGAIN downward if needed. */
                while(((gains[config.igain] + gains[config.pgain]) >= temp) &&
                      ((config.igain != 0ul) || (config.pgain != 0ul)))
                {
                    if (config.igain != 0ul)
                    {
                        config.igain--;
                    }
                    else if (config.pgain != 0ul)
                    {
                        config.pgain--;
                    }
                    else /* both are zero; exit loop; done the best we can */
                    {
                    }
                }
            }

            /* configure FLL based on calculated values */
            returnStatus = Cy_SysClk_FllManualConfigure(&config);
        } /* if not bypass output mode */

        else
        { /* bypass mode */
            /* update CLK_FLL_CONFIG3 register with divide by 2 parameter */
            CY_SYSCLK_CLR_SET(SRSS->CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, (uint32_t)outputMode);
        }
    } /* if no error */

    return (returnStatus);
}

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
uint32_t Cy_SysClk_FllManualConfigure(const cy_stc_fll_manual_config_t *config)
{
    uint32_t returnStatus = CY_SYSCLK_SUCCESS;

    /* check for errors */
    if (_FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS->CLK_FLL_CONFIG) != 0u) /* 1 = enabled */
    {
        returnStatus = CY_SYSCLK_INVALID_STATE;
    }
    else
    { /* return status is OK */
    }

    /* no error */
    if (returnStatus == CY_SYSCLK_SUCCESS) /* no errors */
    {
        /* update CLK_FLL_CONFIG register with 2 parameters; FLL_ENABLE is already 0 */
        uint32_t reg = _VAL2FLD(SRSS_CLK_FLL_CONFIG_FLL_MULT, config->fllMult);
        SRSS->CLK_FLL_CONFIG = reg | _VAL2FLD(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, (uint32_t)(config->enableOutputDiv));
        /* update CLK_FLL_CONFIG2 register with 2 parameters */
        reg = _VAL2FLD(SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV, config->refDiv);
        SRSS->CLK_FLL_CONFIG2 = reg | _VAL2FLD(SRSS_CLK_FLL_CONFIG2_LOCK_TOL, config->lockTolerance);
        /* update CLK_FLL_CONFIG3 register with 4 parameters */
        reg  = _VAL2FLD(SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN, config->igain);
        reg |= _VAL2FLD(SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN, config->pgain);
        reg |= _VAL2FLD(SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT, config->settlingCount);
        SRSS->CLK_FLL_CONFIG3 = reg | _VAL2FLD(SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, (uint32_t)(config->outputMode));
        /* update CLK_FLL_CONFIG4 register with 1 parameter; preserve other bits */
        CY_SYSCLK_CLR_SET(SRSS->CLK_FLL_CONFIG4, SRSS_CLK_FLL_CONFIG4_CCO_RANGE, (uint32_t)(config->ccoRange));
    } /* if no error */

    return (returnStatus);
}

/*******************************************************************************
* Function Name: Cy_SysClk_FllGetConfiguration
****************************************************************************//**
*
* Reports the FLL configuration settings.
*
* \param config \ref cy_stc_fll_manual_config_t
*
*******************************************************************************/
void Cy_SysClk_FllGetConfiguration(cy_stc_fll_manual_config_t *config)
{
    /* read 2 parameters from CLK_FLL_CONFIG register */
    uint32_t tempReg = SRSS->CLK_FLL_CONFIG;
    config->fllMult         = _FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_MULT, tempReg);
    config->enableOutputDiv = (bool)_FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, tempReg);
    /* read 2 parameters from CLK_FLL_CONFIG2 register */
    tempReg = SRSS->CLK_FLL_CONFIG2;
    config->refDiv          = _FLD2VAL(SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV, tempReg);
    config->lockTolerance   = _FLD2VAL(SRSS_CLK_FLL_CONFIG2_LOCK_TOL, tempReg);
    /* read 4 parameters from CLK_FLL_CONFIG3 register */
    tempReg = SRSS->CLK_FLL_CONFIG3;
    config->igain           = _FLD2VAL(SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN, tempReg);
    config->pgain           = _FLD2VAL(SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN, tempReg);
    config->settlingCount   = _FLD2VAL(SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT, tempReg);
    config->outputMode      = (cy_en_fll_pll_output_mode_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, tempReg);
    /* read 1 parameter from CLK_FLL_CONFIG4 register */
    config->ccoRange        = (cy_en_fll_cco_ranges_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG4_CCO_RANGE, SRSS->CLK_FLL_CONFIG4);
}

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
uint32_t Cy_SysClk_FllEnable(uint32_t timeoutus)
{
    uint32_t rtnval;
    bool nonZeroTimeout = (timeoutus != 0ul);

    /* first set the CCO enable bit */
    SRSS->CLK_FLL_CONFIG4 |= _VAL2FLD(SRSS_CLK_FLL_CONFIG4_CCO_ENABLE, 1ul); /* 1 = enable */

    /* Wait until CCO is ready */
    for (; (_FLD2VAL(SRSS_CLK_FLL_STATUS_CCO_READY, SRSS->CLK_FLL_STATUS) == 0ul) &&
           (timeoutus != 0ul);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1u);
    }

    /* Set the FLL enable bit, if CCO is ready */
    if ((!nonZeroTimeout) || (nonZeroTimeout && (timeoutus != 0ul)))
    {
        SRSS->CLK_FLL_CONFIG |= _VAL2FLD(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, 1ul); /* 1 = enable */
    }

    /* now do the timeout wait for FLL_STATUS, bit LOCKED */
    for (; (_FLD2VAL(SRSS_CLK_FLL_STATUS_LOCKED, SRSS->CLK_FLL_STATUS) == 0ul) &&
           (timeoutus != 0ul);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1u);
    }

    /* If lock doesn't occur, FLL is stopped. */
    if (nonZeroTimeout && (timeoutus == 0ul))
    {
        Cy_SysClk_FllDisable();
    }

    rtnval = ((timeoutus == 0ul) ? CY_SYSCLK_TIMEOUT : CY_SYSCLK_SUCCESS);
    return rtnval;
}


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
                                cy_en_fll_pll_output_mode_t outputMode)
{
    uint32_t returnStatus = CY_SYSCLK_SUCCESS;

    /* check for error */
    if ((inputFreq < 4000000ul) || (64000000ul < inputFreq)) /* invalid input frequency */
    {
        returnStatus = CY_SYSCLK_BAD_PARAM;
    }
    else if ((outputFreq < 1000000ul) || (400000000ul < outputFreq)) /* invalid output frequency */
    {
        returnStatus = CY_SYSCLK_BAD_PARAM;
    }
    else if ((clkPath == 0ul) || (clkPath > SRSS_NUM_PLL)) /* invalid clock path number */
    {
        returnStatus = CY_SYSCLK_BAD_PARAM;
    }
    else if (_FLD2VAL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS->CLK_PLL_CONFIG[clkPath - 1ul]) != 0u) /* 1 = enabled */
    {
        returnStatus = CY_SYSCLK_INVALID_STATE;
    }
    else
    { /* returnStatus is OK */
    }

    /* no errors */
    if (returnStatus == CY_SYSCLK_SUCCESS)
    {
        uint32_t pBest = 0ul, qBest = 0ul, outBest = 0ul;

        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the
           frequency parameters. */
        if (outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            /* for each possible value of OUTPUT_DIV and REFERENCE_DIV (Q), try
               to find a value for FEEDBACK_DIV (P) that gives an output frequency
               as close as possible to the desired output frequency. */
            uint32_t p, q, out;
            uint32_t foutBest = 0ul; /* to ensure at least one pass through the for loops below */

            /* OUTPUT_DIV is 9 bits; range is 2 to 2^9 - 1 */
            for (out = 2ul; (out < 512ul) && (foutBest != outputFreq); out++)
            {
                /* REFERENCE_DIV (Q) is 5 bits; allowed range is 1 to 2^5 - 1 */
                for (q = 1ul; (q < 31ul) && (foutBest != outputFreq); q++)
                {
                    /* calculate a test value for FEEDBACK_DIV (P) */
                    p = CY_SYSCLK_DIV_ROUND(outputFreq * q * out, inputFreq);
                    /* FEEDBACK_DIV is 7 bits; allowed range is 1 to 50 */
                    if ((1u <= p) && (p <= 50u))
                    {
                        /* Calculate what output frequency will actually be produced.
                           If it's closer to the target than what we have so far, then
                           save it. */
                        uint32_t fout = ((p * inputFreq) / q) / out;
                        if ((uint32_t)abs((int32_t)fout - (int32_t)outputFreq) <
                            (uint32_t)abs((int32_t)foutBest - (int32_t)outputFreq))
                        {
                            foutBest = fout;
                            pBest = p; qBest = q; outBest = out;
                        }
                    }
                    /* exit loops if foutBest equals outputFreq */
                }
            }
        } /* if not bypass output mode */

        /* configure PLL based on calculated values */
        returnStatus = Cy_SysClk_PllManualConfigure(clkPath, pBest, qBest, outBest, outputMode);

    } /* if no error */

    return (returnStatus);
}

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
                                      uint32_t outputDiv, cy_en_fll_pll_output_mode_t outputMode)
{
    uint32_t returnStatus = CY_SYSCLK_SUCCESS;

    /* check for error */
    if ((clkPath == 0ul) || (clkPath > SRSS_NUM_PLL)) /* invalid clock path number */
    {
        returnStatus = CY_SYSCLK_BAD_PARAM;
    }
    else if (_FLD2VAL(SRSS_CLK_PLL_CONFIG_ENABLE, SRSS->CLK_PLL_CONFIG[clkPath - 1ul]) != 0u) /* 1 = enabled */
    {
        returnStatus = CY_SYSCLK_INVALID_STATE;
    }
    else
    { /* returnStatus is OK */
    }

    /* no errors */
    if (returnStatus == CY_SYSCLK_SUCCESS)
    {
        clkPath--; /* to correctly access PLL config registers structure */
        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the
           frequency parameters. */
        if (outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            CY_SYSCLK_CLR_SET(SRSS->CLK_PLL_CONFIG[clkPath], SRSS_CLK_PLL_CONFIG_FEEDBACK_DIV,  feedbackDiv);
            CY_SYSCLK_CLR_SET(SRSS->CLK_PLL_CONFIG[clkPath], SRSS_CLK_PLL_CONFIG_REFERENCE_DIV, referenceDiv);
            CY_SYSCLK_CLR_SET(SRSS->CLK_PLL_CONFIG[clkPath], SRSS_CLK_PLL_CONFIG_OUTPUT_DIV,    outputDiv);
        }

        CY_SYSCLK_CLR_SET(SRSS->CLK_PLL_CONFIG[clkPath], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, (uint32_t)outputMode);
    } /* if no error */

    return (returnStatus);
}

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
                                       uint32_t *outputDiv, cy_en_fll_pll_output_mode_t *outputMode)
{
    uint32_t rtnval = CY_SYSCLK_BAD_PARAM;
    if ((clkPath != 0ul) && (clkPath <= SRSS_NUM_PLL))
    {
        uint32_t tempReg = SRSS->CLK_PLL_CONFIG[clkPath - 1ul];
        *feedbackDiv  = _FLD2VAL(SRSS_CLK_PLL_CONFIG_FEEDBACK_DIV, tempReg);
        *referenceDiv = _FLD2VAL(SRSS_CLK_PLL_CONFIG_REFERENCE_DIV, tempReg);
        *outputDiv    = _FLD2VAL(SRSS_CLK_PLL_CONFIG_OUTPUT_DIV, tempReg);
        *outputMode   = (cy_en_fll_pll_output_mode_t)_FLD2VAL(SRSS_CLK_PLL_CONFIG_BYPASS_SEL, tempReg);
        rtnval = CY_SYSCLK_SUCCESS;
    }
    return (rtnval);
}

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
uint32_t Cy_SysClk_PllEnable(uint32_t clkPath, uint32_t timeoutus)
{
    uint32_t rtnval = CY_SYSCLK_BAD_PARAM;
    if ((clkPath != 0ul) && (clkPath <= SRSS_NUM_PLL))
    {
        clkPath--; /* to correctly access PLL config and status registers structures */
        /* first set the PLL enable bit */
        SRSS->CLK_PLL_CONFIG[clkPath] |= _VAL2FLD(SRSS_CLK_PLL_CONFIG_ENABLE, 1ul); /* 1 = enable */

        /* now do the timeout wait for PLL_STATUS, bit LOCKED */
        for (; (_FLD2VAL(SRSS_CLK_PLL_STATUS_LOCKED, SRSS->CLK_PLL_STATUS[clkPath]) == 0ul) &&
               (timeoutus != 0ul);
             timeoutus--)
        {
            Cy_SysLib_DelayUs(1u);
        }
        rtnval = ((timeoutus == 0ul) ? CY_SYSCLK_TIMEOUT : CY_SYSCLK_SUCCESS);
    }
    return (rtnval);
}


/* ========================================================================== */
/* ====================    Clock Measurement section    ==================== */
/* ========================================================================== */

/* Cy_SysClk_InitClkMeasurementCounters() input parameters saved for use later in other functions. */
static uint32_t clk1FregHz;
static uint32_t clk1Count1;

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
                                           uint32_t count1, cy_en_meas_clks_t clock2)
{
    uint32_t rtnval = CY_SYSCLK_INVALID_STATE;
    if (_FLD2VAL(SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE, SRSS->CLK_CAL_CNT1) != 0ul/*1 = done*/)
    {
        /* if clock1 is a slow clock,
             select it in SRSS_CLK_OUTPUT_SLOW.SLOW_SEL0,
             select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL0 = SLOW_SEL0
           else if clock1 is a fast clock,
             select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL0,
           else error, do nothing and return.

           if clock2 is a slow clock,
             select it in SRSS_CLK_OUTPUT_SLOW.SLOW_SEL1,
             select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL1 = SLOW_SEL1
           else if clock2 is a fast clock,
             select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL1,
           else error, do nothing and return.
        */
        rtnval = CY_SYSCLK_BAD_PARAM;
        if ((clock1 < CY_SYSCLK_MEAS_CLK_LAST_CLK) && (clock2 < CY_SYSCLK_MEAS_CLK_LAST_CLK))
        {
            if (clock1 < CY_SYSCLK_MEAS_CLK_FAST_CLKS)
            { /* slow clock */
                SRSS->CLK_OUTPUT_SLOW = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_SLOW, SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0, (uint32_t)clock1);
                SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_FAST_SEL0, 7ul/*slow_sel0 output*/);
            } /* clock1 slow clock */
            else
            { /* fast clock */
                if (clock1 < CY_SYSCLK_MEAS_CLK_PATH_CLKS)
                { /* ECO, EXT, ALTHF */
                    SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_FAST_SEL0, (uint32_t)clock1);
                }
                else
                { /* PATH or HFCLK */
                    SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_FAST_SEL0,
                                                            (((uint32_t)clock1 >> 8) & 0x0Ful) /*use enum bits [11:8]*/);
                    if (clock1 < CY_SYSCLK_MEAS_CLK_HF_CLKS)
                    { /* PATH select */
                        SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_PATH_SEL0,
                                                                ((uint32_t)clock1 & 0x0Ful) /*use enum bits [3:0]*/);
                    }
                    else
                    { /* HFCLK select */
                        SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0,
                                                                ((uint32_t)clock1 & 0x0Ful) /*use enum bits [3:0]*/);
                    }
                }
            } /* clock1 fast clock */

            if (clock2 < CY_SYSCLK_MEAS_CLK_FAST_CLKS)
            { /* slow clock */
                SRSS->CLK_OUTPUT_SLOW = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_SLOW, SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1, (uint32_t)clock2);
                SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_FAST_SEL1, 7ul/*slow_sel1 output*/);
            } /* clock2 slow clock */
            else
            { /* fast clock */
                if (clock2 < CY_SYSCLK_MEAS_CLK_PATH_CLKS)
                { /* ECO, EXT, ALTHF */
                    SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_FAST_SEL1, (uint32_t)clock2);
                }
                else
                { /* PATH or HFCLK */
                    SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_FAST_SEL1,
                                                            (((uint32_t)clock2 >> 8) & 0x0Ful) /*use enum bits [11:8]*/);
                    if (clock2 < CY_SYSCLK_MEAS_CLK_HF_CLKS)
                    { /* PATH select */
                        SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_PATH_SEL1,
                                                                ((uint32_t)clock2 & 0x0Ful) /*use enum bits [3:0]*/);
                    }
                    else
                    { /* HFCLK select */
                        SRSS->CLK_OUTPUT_FAST = _CLR_SET_FLD32U(SRSS->CLK_OUTPUT_FAST, SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1,
                                                                ((uint32_t)clock2 & 0x0Ful) /*use enum bits [3:0]*/);
                    }
                }
            } /* clock2 fast clock */

            /* Save the remaining input parameters for use later, in other functions.
               No error checking is done on these parameters.*/
            clk1FregHz = clock1FreqHz;
            clk1Count1 = count1;

            rtnval = CY_SYSCLK_SUCCESS;
        } /* if (clock1 < CY_SYSCLK_MEAS_CLK_LAST_CLK && clock2 < CY_SYSCLK_MEAS_CLK_LAST_CLK) */
    } /* if (not done) */
    return (rtnval);
}

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
uint32_t Cy_SysClk_StartClkMeasurementCounters(void)
{
    uint32_t rtnval = CY_SYSCLK_INVALID_STATE;
    if (_FLD2VAL(SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE, SRSS->CLK_CAL_CNT1) != 0ul/*1 = done*/)
    {
        /* Counting starts when this register is written with a nonzero value. */
        SRSS->CLK_CAL_CNT1 = clk1Count1;
        rtnval = CY_SYSCLK_SUCCESS;
    }
    return (rtnval);
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
uint32_t Cy_SysClk_ClkMeasurementCountersGetClock2Freq(void)
{
    return (uint32_t)
        CY_SYSCLK_DIV_ROUND((uint64_t)_FLD2VAL(SRSS_CLK_CAL_CNT2_CAL_COUNTER2, SRSS->CLK_CAL_CNT2) * clk1FregHz, clk1Count1);
}

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
/* target frequency */
#define CY_SYSCLK_ILO_TARGET_FREQ  32768u

uint32_t Cy_SysClk_IloTrim(uint32_t iloFreq)
{
    /* Nominal trim step size is 1.5% of "the frequency". Using the target frequency. */
    const uint32_t trimStep = CY_SYSCLK_DIV_ROUND((uint32_t)CY_SYSCLK_ILO_TARGET_FREQ * 15ul, 1000ul);
    /* Get the current trim value, and adjust it up or down the appropriate
       number of steps. Calculate the new frequency. */
    uint32_t newTrim;
    uint32_t newFreq;
    uint32_t curTrim = _FLD2VAL(SRSS_CLK_TRIM_ILO_CTL_ILO_FTRIM, SRSS->CLK_TRIM_ILO_CTL);
    if (iloFreq > CY_SYSCLK_ILO_TARGET_FREQ)
    { /* reduce the trim value; calculate new frequency */
        newTrim = curTrim - CY_SYSCLK_DIV_ROUND(iloFreq - CY_SYSCLK_ILO_TARGET_FREQ, trimStep);
        newFreq = iloFreq - (trimStep * (curTrim - newTrim));
    }
    else
    { /* iloFreq too low. Increase the trim value; calculate new frequency. */
        newTrim = curTrim + CY_SYSCLK_DIV_ROUND(CY_SYSCLK_ILO_TARGET_FREQ - iloFreq, trimStep);
        newFreq = iloFreq + (trimStep * (newTrim - curTrim));
    }

    /* Update the trim value */
    CY_SYSCLK_CLR_SET(SRSS->CLK_TRIM_ILO_CTL, SRSS_CLK_TRIM_ILO_CTL_ILO_FTRIM, newTrim);

    return newFreq;
}

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
/* target frequency */
#define CY_SYSCLK_PILO_TARGET_FREQ  32768u
/* nominal trim step size */
#define CY_SYSCLK_PILO_TRIM_STEP 1000u

uint32_t Cy_SysClk_PiloTrim(uint32_t piloFreq)
{
    /* Get the current trim value, and adjust it up or down the appropriate
       number of steps. Calculate the new frequency. */
    uint32_t newTrim;
    uint32_t newFreq;
    uint32_t curTrim = _FLD2VAL(SRSS_CLK_TRIM_PILO_CTL_PILO_CFREQ, SRSS->CLK_TRIM_PILO_CTL);
    if (piloFreq > CY_SYSCLK_PILO_TARGET_FREQ)
    { /* reduce the trim value; calculate new frequency */
        newTrim = curTrim - CY_SYSCLK_DIV_ROUND(piloFreq - CY_SYSCLK_PILO_TARGET_FREQ, CY_SYSCLK_PILO_TRIM_STEP);
        newFreq = piloFreq - (CY_SYSCLK_PILO_TRIM_STEP * (curTrim - newTrim));
    }
    else
    { /* piloFreq too low. Increase the trim value; calculate new frequency. */
        newTrim = curTrim + CY_SYSCLK_DIV_ROUND(CY_SYSCLK_PILO_TARGET_FREQ - piloFreq, CY_SYSCLK_PILO_TRIM_STEP);
        newFreq = piloFreq + (CY_SYSCLK_PILO_TRIM_STEP * (newTrim - curTrim));
    }

    /* Update the trim value */
    CY_SYSCLK_CLR_SET(SRSS->CLK_TRIM_PILO_CTL, SRSS_CLK_TRIM_PILO_CTL_PILO_CFREQ, newTrim);

    return newFreq;
}

/* ========================================================================== */
/* ===========================    WCO SECTION    ============================ */
/* ========================================================================== */
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
void Cy_SysClk_WcoConfigureCsv(const cy_stc_wco_csv_config_t *config)
{
  #if SRSS_WCOCSV_PRESENT != 0
    /* First clear all bits, including the enable bit; disable loss detection. */
    SRSS->CLK_CSV_WCO_CTL = 0ul;
    /* Then write the structure elements (which include an enable bit) to the register. */
    SRSS->CLK_CSV_WCO_CTL = _VAL2FLD(SRSS_CLK_CSV_WCO_CTL_CSV_MUX, (uint32_t)config->SupervisorClock)      |
                            _VAL2FLD(SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_WINDOW, (uint32_t)(config->LossWindow)) |
                            _VAL2FLD(SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_ACTION, (uint32_t)(config->LossAction)) |
                            _VAL2FLD(SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_EN, config->enableLossDetection);
  #endif /* SRSS_WCOCSV_PRESENT != 0 */
}


/* ========================================================================== */
/* ========================    clk_hf[n] SECTION    ========================= */
/* ========================================================================== */
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
void Cy_SysClk_HfClockConfigureCsv(uint32_t hfClk, const cy_stc_hf_csv_config_t *config)
{
  #if SRSS_MASK_HFCSV != 0
    /* First update the limit bits; this can be done regardless of enable state. */
    SRSS->CLK_CSV[hfClk].HF_LIMIT = _VAL2FLD(CLK_CSV_HF_LIMIT_UPPER_LIMIT, config->FrequencyUpperLimit) |
                                    _VAL2FLD(CLK_CSV_HF_LIMIT_LOWER_LIMIT, config->FrequencyLowerLimit);
    /* Then clear all CTL register bits, including the enable bit; disable loss detection. */
    SRSS->CLK_CSV[hfClk].HF_CTL = 0ul;
    /* Finally, write the structure elements (which include an enable bit) to the CTL register. */
    SRSS->CLK_CSV[hfClk].HF_CTL = _VAL2FLD(CLK_CSV_HF_CTL_CSV_LOSS_EN, config->enableLossDetection)             |
                                  _VAL2FLD(CLK_CSV_HF_CTL_CSV_LOSS_ACTION, (uint32_t)(config->LossAction))      |
                                  _VAL2FLD(CLK_CSV_HF_CTL_CSV_FREQ_EN, config->enableFrquencyFaultDetection)    |
                                  _VAL2FLD(CLK_CSV_HF_CTL_CSV_FREQ_ACTION, (uint32_t)(config->FrequencyAction)) |
                                  _VAL2FLD(CLK_CSV_HF_CTL_CSV_LOSS_WINDOW, (uint32_t)(config->LossWindow))      |
                                  _VAL2FLD(CLK_CSV_HF_CTL_CSV_MUX, (uint32_t)(config->SupervisorClock))         |
                                  _VAL2FLD(CLK_CSV_HF_CTL_CSV_FREQ_WINDOW, config->SupervisingWindow);
  #endif /* SRSS_MASK_HFCSV != 0 */
}

/* ========================================================================== */
/* ========================    clk_peri SECTION    ========================== */
/* ========================================================================== */
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
* \return The frequency, in Hz
*
* \note
* The reported frequency may be zero, which indicates unknown. This happens if
* the source input is clk_ext, ECO, clk_althf, dsi_out, or clk_altlf.
*
*******************************************************************************/
uint32_t Cy_SysClk_PeriphGetFrequency(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    uint32_t rtnval = 0ul; /* 0 = unknown frequency */

    /* FLL or PLL configuration parameters */
    union
    {
        cy_stc_fll_manual_config_t fll;
        struct
        {
            uint32_t feedbackDiv;
            uint32_t referenceDiv;
            uint32_t outputDiv;
        } pll;
    } fllpll = {0ul};

    /* variables holding intermediate clock sources and dividers */
    cy_en_fll_pll_output_mode_t mode = CY_SYSCLK_FLLPLL_OUTPUT_AUTO; /* FLL or PLL mode; n/a for direct */
    uint32_t                 locked = 0ul;    /* FLL or PLL lock status; n/a for direct */
    cy_en_clkpath_in_sources_t  source;          /* source input for path (FLL, PLL, or direct) */
    uint32_t                 source_freq;     /* source clock frequency, in Hz */
    cy_en_hf_clk_sources_t      path;            /* source input for root 0 (clk_hf[0]) */
    uint32_t                 path_freq = 0ul; /* path (FLL, PLL, or direct) frequency, in Hz */
    uint32_t                 root_div;        /* root prescaler (1/2/4/8) */
    uint32_t                 clk_hf0_div;     /* clk_hf[0] predivider to clk_peri */

    /* clk_peri divider to selected peripheral clock */
    struct
    {
        uint32_t integer;
        uint32_t frac;
    } clkdiv = {0ul, 0ul};

    /* Start by finding the source input for root 0 (clk_hf[0]) */
    path = Cy_SysClk_HfClockGetSource(0ul);

    if (path == CY_SYSCLK_HFCLK_IN_CLKPATH0) /* FLL? (always path #0) */
    {
        Cy_SysClk_FllGetConfiguration(&fllpll.fll);
        source = Cy_SysClk_ClkPathGetSource(0ul);
        mode = fllpll.fll.outputMode;
        locked = Cy_SysClk_FllGetLockStatus();
    }
    else if ((uint32_t)path <= (uint32_t)SRSS_NUM_PLL) /* PLL? (always path #1 - N)*/
    {
        (void)Cy_SysClk_PllGetConfiguration((uint32_t)path, &fllpll.pll.feedbackDiv, &fllpll.pll.referenceDiv,
                                            &fllpll.pll.outputDiv, &mode);
        source = Cy_SysClk_ClkPathGetSource((uint32_t)path);
        locked = Cy_SysClk_PllGetLockStatus((uint32_t)path);
    }
    else /* assume clk_path < SRSS_NUM_CLKPATH */
    { /* Direct select path. Use PLL function to get the source. */
        source = Cy_SysClk_ClkPathGetSource((uint32_t)path);
    }

    /* get the frequency of the source, i.e., the path mux input */
    switch(source)
    {
        case CY_SYSCLK_CLKPATH_IN_IMO: /* IMO frequency is fixed at 8 MHz */
            source_freq = 8000000ul; /*Hz*/
            break;
        case CY_SYSCLK_CLKPATH_IN_ILO: /* ILO and WCO frequencies are nominally 32.768 kHz */
        case CY_SYSCLK_CLKPATH_IN_WCO:
            source_freq = 32768ul; /*Hz*/
            break;
        default:
            /* don't know the frequency of clk_ext, ECO, clk_althf, dsi_out, or clk_altlf */
            source_freq = 0ul; /* unknown frequency */
            break;
    }
    if (source_freq != 0ul)
    {
        /* Calculate the path frequency */
        if (path == CY_SYSCLK_HFCLK_IN_CLKPATH0) /* FLL? (always path #0) */
        {
            path_freq = source_freq; /* for bypass mode */
            /* if not bypass mode, apply the dividers calculation */
            if ((mode == CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT) || ((mode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT) && (locked != 0ul)))
            {
                /* Ffll_out = Ffll_clk * FLL_MULT / FLL_REF_DIV / (FLL_OUTPUT_DIV + 1), where:
                 *  FLL_MULT, REFERENCE_DIV, and OUTPUT_DIV are FLL configuration register bitfields
                 * Check for possible divide by 0.
                 */
                if (fllpll.fll.refDiv != 0ul)
                {
                    path_freq = (uint32_t)(((uint64_t)path_freq * (uint64_t)fllpll.fll.fllMult) /
                                           (uint64_t)fllpll.fll.refDiv) /
                                ((uint32_t)(fllpll.fll.enableOutputDiv) + 1ul);
                }
                else
                {
                    path_freq = 0ul; /* error, one of the divisors is 0 */
                }
            }
        }
        else if ((uint32_t)path <= (uint32_t)SRSS_NUM_PLL) /* PLL? (always path #1 - N)*/
        {
            path_freq = source_freq; /* for bypass mode */
            /* if not bypass mode, apply the dividers calculation */
            if ((mode == CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT) || ((mode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT) && (locked != 0ul)))
            {
                /* Fpll_out = Fpll_clk * FEEDBACK_DIV / REFERENCE_DIV / OUTPUT_DIV, where:
                 *  FEEDBACK_DIV, REFERENCE_DIV, and OUTPUT_DIV are PLL configuration register bitfields
                 * Check for possible divide by 0.
                 */
                if ((fllpll.pll.referenceDiv != 0ul) && (fllpll.pll.outputDiv != 0ul))
                {
                    path_freq = (uint32_t)(((uint64_t)source_freq * (uint64_t)fllpll.pll.feedbackDiv) /
                                           (uint64_t)fllpll.pll.referenceDiv) /
                                (uint32_t)fllpll.pll.outputDiv;
                }
                else
                {
                    path_freq = 0ul; /* error, one of the divisors is 0 */
                }
            }
        }
        else /* assume clk_path < SRSS_NUM_CLKPATH */
        { /* direct select path */
            path_freq = source_freq;
        }

        /* get the prescaler value for root 0, or clk_hf[0]: 1/2/4/8 */
        root_div = 1ul << (uint32_t)Cy_SysClk_HfClockGetDivider(0ul);

        /* get the predivider value for clk_hf[0] to clk_peri */
        clk_hf0_div = (uint32_t)Cy_SysClk_PeriClkGetDivider() + 1ul;

        /* get the divider value for clk_peri to the selected peripheral clock */
        switch(dividerType)
        {
            case CY_SYSCLK_DIV_8_BIT:
            case CY_SYSCLK_DIV_16_BIT:
                clkdiv.integer = (uint32_t)Cy_SysClk_PeriphGetDivider(dividerType, dividerNum);
                /* frac = 0 means it's an integer divider */
                break;
            case CY_SYSCLK_DIV_16_5_BIT:
            case CY_SYSCLK_DIV_24_5_BIT:
                (void)Cy_SysClk_PeriphGetFracDivider(dividerType, dividerNum, &clkdiv.integer, &clkdiv.frac);
                break;
            default:
                break;
        }
        /* Divide the path input frequency down, and return the result.
           Stepping through the following code shows the frequency at each stage.
        */
        rtnval = path_freq / root_div; /* clk_hf[0] frequency */
        rtnval /= clk_hf0_div; /* clk_peri frequency */
        /* For fractional dividers, the divider is (int + 1) + frac/32.
         * Use the fractional value to round the divider to the nearest integer.
         */
        rtnval /= (clkdiv.integer + 1ul + ((clkdiv.frac >= 16ul) ? 1ul : 0ul)); /* peripheral divider output frequency */
    }

    return rtnval;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */


/* [] END OF FILE */
