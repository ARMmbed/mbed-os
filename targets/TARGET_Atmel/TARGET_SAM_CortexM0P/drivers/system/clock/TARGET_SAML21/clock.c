/**
 * \file
 *
 * \brief SAM L21 Clock Driver
 *
 * Copyright (C) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <clock.h>
#include <conf_clocks.h>
#include <system.h>


/**
 * \internal
 * \brief DFLL-specific data container.
 */
struct _system_clock_dfll_config {
    uint32_t control;
    uint32_t val;
    uint32_t mul;
};

/**
 * \internal
 * \brief DPLL-specific data container.
 */
struct _system_clock_dpll_config {
    uint32_t frequency;
};


/**
 * \internal
 * \brief XOSC-specific data container.
 */
struct _system_clock_xosc_config {
    uint32_t frequency;
};

/**
 * \internal
 * \brief System clock module data container.
 */
struct _system_clock_module {
    volatile struct _system_clock_dfll_config dfll;
    volatile struct _system_clock_dpll_config dpll;

    volatile struct _system_clock_xosc_config xosc;
    volatile struct _system_clock_xosc_config xosc32k;
};

/**
 * \internal
 * \brief Internal module instance to cache configuration values.
 */
static struct _system_clock_module _system_clock_inst = {
    .dfll = {
        .control     = 0,
        .val     = 0,
        .mul     = 0,
    },
    .dpll = {
        .frequency   = 0,
    },
    .xosc = {
        .frequency   = 0,
    },
    .xosc32k = {
        .frequency   = 0,
    },
};

/**
 * \internal
 * \brief Wait for sync to the DFLL control registers.
 */
static inline void _system_dfll_wait_for_sync(void)
{
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY)) {
        /* Wait for DFLL sync */
    }
}

/**
 * \internal
 * \brief Wait for sync to the OSC32K control registers.
 */
static inline void _system_osc32k_wait_for_sync(void)
{
    while (!(OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_OSC32KRDY)) {
        /* Wait for OSC32K sync */
    }
}

/**
 * \internal
 * \brief OSC16M frequency selection.
 *  Frequency selection can be done only when OSC16M is disabled,thus,
 *  OSCULP32K is temporarily used as a new clocksource for mainclock .
 *
 */
static inline void _system_clock_source_osc16m_freq_sel(void)
{
    struct system_gclk_gen_config gclk_conf;
    struct system_clock_source_osc16m_config osc16m_conf;

    /* Select OSCULP32K as new clock source for mainclock temporarily */
    system_gclk_gen_get_config_defaults(&gclk_conf);
    gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_ULP32K;
    system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);

    /* GCLK0 is enabled after POR */

    /* Disable OSC16M clock*/
    system_clock_source_disable(SYSTEM_CLOCK_SOURCE_OSC16M);

    /* Switch to new frequency selection and enable OSC16M */
    system_clock_source_osc16m_get_config_defaults(&osc16m_conf);
    osc16m_conf.fsel      		= CONF_CLOCK_OSC16M_FREQ_SEL;
    osc16m_conf.on_demand       = 0;
    osc16m_conf.run_in_standby  = CONF_CLOCK_OSC16M_RUN_IN_STANDBY;
    system_clock_source_osc16m_set_config(&osc16m_conf);
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC16M);
    while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_OSC16M));

    /* Select OSC16M for mainclock again */
    system_gclk_gen_get_config_defaults(&gclk_conf);
    gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_OSC16M;
    system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);
    if (CONF_CLOCK_OSC16M_ON_DEMAND) {
        OSCCTRL->OSC16MCTRL.reg |= OSCCTRL_OSC16MCTRL_ONDEMAND;
    }
}

static inline void _system_clock_source_dfll_set_config_errata_9905(void)
{

    /* Disable ONDEMAND mode while writing configurations */
    OSCCTRL->DFLLCTRL.reg = _system_clock_inst.dfll.control & ~OSCCTRL_DFLLCTRL_ONDEMAND;
    _system_dfll_wait_for_sync();

    OSCCTRL->DFLLMUL.reg = _system_clock_inst.dfll.mul;
    OSCCTRL->DFLLVAL.reg = _system_clock_inst.dfll.val;

    /* Write full configuration to DFLL control register */
    OSCCTRL->DFLLCTRL.reg = _system_clock_inst.dfll.control;
}

/**
 * \brief Retrieve the frequency of a clock source.
 *
 * Determines the current operating frequency of a given clock source.
 *
 * \param[in] clock_source  Clock source to get the frequency of
 *
 * \returns Frequency of the given clock source, in Hz.
 */
uint32_t system_clock_source_get_hz(
    const enum system_clock_source clock_source)
{
    switch (clock_source) {
        case SYSTEM_CLOCK_SOURCE_XOSC:
            return _system_clock_inst.xosc.frequency;

        case SYSTEM_CLOCK_SOURCE_OSC16M:
            return (OSCCTRL->OSC16MCTRL.bit.FSEL+1)*4000000UL;

        case SYSTEM_CLOCK_SOURCE_OSC32K:
            return 32768UL;

        case SYSTEM_CLOCK_SOURCE_ULP32K:
            return 32768UL;

        case SYSTEM_CLOCK_SOURCE_XOSC32K:
            return _system_clock_inst.xosc32k.frequency;

        case SYSTEM_CLOCK_SOURCE_DFLL:

            /* Check if the DFLL has been configured */
            if (!(_system_clock_inst.dfll.control & OSCCTRL_DFLLCTRL_ENABLE))
                return 0;

            /* Make sure that the DFLL module is ready */
            _system_dfll_wait_for_sync();

            /* Check if operating in closed loop mode */
            if (_system_clock_inst.dfll.control & OSCCTRL_DFLLCTRL_MODE) {
                return system_gclk_chan_get_hz(OSCCTRL_GCLK_ID_DFLL48) *
                       (_system_clock_inst.dfll.mul & 0xffff);
            }

            return 48000000UL;

        case SYSTEM_CLOCK_SOURCE_DPLL:
            if (!(OSCCTRL->DPLLCTRLA.reg & OSCCTRL_DPLLCTRLA_ENABLE)) {
                return 0;
            }

            return _system_clock_inst.dpll.frequency;

        default:
            return 0;
    }
}

/**
 * \brief Configure the internal OSC16M oscillator clock source.
 *
 * Configures the 16MHz (nominal) internal RC oscillator with the given
 * configuration settings.
 *
 * \note Frequency selection can be done only when OSC16M is disabled.
 *
 * \param[in] config  OSC16M configuration structure containing the new config
 */
void system_clock_source_osc16m_set_config(
    struct system_clock_source_osc16m_config *const config)
{
    OSCCTRL_OSC16MCTRL_Type temp = OSCCTRL->OSC16MCTRL;

    /* Use temporary struct to reduce register access */
    temp.bit.FSEL    = config->fsel;
    temp.bit.ONDEMAND = config->on_demand;
    temp.bit.RUNSTDBY = config->run_in_standby;

    OSCCTRL->OSC16MCTRL = temp;
}

/**
 * \brief Configure the internal OSC32K oscillator clock source.
 *
 * Configures the 32KHz (nominal) internal RC oscillator with the given
 * configuration settings.
 *
 * \param[in] config  OSC32K configuration structure containing the new config
 */
void system_clock_source_osc32k_set_config(
    struct system_clock_source_osc32k_config *const config)
{
    OSC32KCTRL_OSC32K_Type temp = OSC32KCTRL->OSC32K;


    /* Update settings via a temporary struct to reduce register access */
    temp.bit.EN1K     = config->enable_1khz_output;
    temp.bit.EN32K    = config->enable_32khz_output;
    temp.bit.STARTUP  = config->startup_time;
    temp.bit.ONDEMAND = config->on_demand;
    temp.bit.RUNSTDBY = config->run_in_standby;
    temp.bit.WRTLOCK  = config->write_once;

    OSC32KCTRL->OSC32K  = temp;
}

/**
 * \brief Configure the internal OSCULP32K oscillator clock source.
 *
 * Configures the Ultra Low Power 32KHz internal RC oscillator with the given
 * configuration settings.
 *
 * \note The OSCULP32K is enabled by default after a Power On Reset (POR) and
 *       will always run except during POR.
 *
 * \param[in] config  OSCULP32K configuration structure containing the new config
 */
void system_clock_source_osculp32k_set_config(
    struct system_clock_source_osculp32k_config *const config)
{
    OSC32KCTRL_OSCULP32K_Type temp = OSC32KCTRL->OSCULP32K;
    /* Update settings via a temporary struct to reduce register access */
    temp.bit.WRTLOCK  = config->write_once;
    OSC32KCTRL->OSCULP32K  = temp;
}

/**
 * \brief Configure the external oscillator clock source.
 *
 * Configures the external oscillator clock source with the given configuration
 * settings.
 *
 * \param[in] config  External oscillator configuration structure containing
 *                    the new config
 */
void system_clock_source_xosc_set_config(
    struct system_clock_source_xosc_config *const config)
{
    OSCCTRL_XOSCCTRL_Type temp = OSCCTRL->XOSCCTRL;

    temp.bit.STARTUP = config->startup_time;

    if (config->external_clock == SYSTEM_CLOCK_EXTERNAL_CRYSTAL) {
        temp.bit.XTALEN = 1;
    } else {
        temp.bit.XTALEN = 0;
    }

    temp.bit.AMPGC = config->auto_gain_control;

    /* Set gain if automatic gain control is not selected */
    if (!config->auto_gain_control) {
        if (config->frequency <= 2000000) {
            temp.bit.GAIN = 0;
        } else if (config->frequency <= 4000000) {
            temp.bit.GAIN = 1;
        } else if (config->frequency <= 8000000) {
            temp.bit.GAIN = 2;
        } else if (config->frequency <= 16000000) {
            temp.bit.GAIN = 3;
        } else if (config->frequency <= 30000000) {
            temp.bit.GAIN = 4;
        }

    }

    temp.bit.ONDEMAND = config->on_demand;
    temp.bit.RUNSTDBY = config->run_in_standby;

    /* Store XOSC frequency for internal use */
    _system_clock_inst.xosc.frequency = config->frequency;

    OSCCTRL->XOSCCTRL = temp;
}

/**
 * \brief Configure the XOSC32K external 32KHz oscillator clock source.
 *
 * Configures the external 32KHz oscillator clock source with the given
 * configuration settings.
 *
 * \param[in] config  XOSC32K configuration structure containing the new config
 */
void system_clock_source_xosc32k_set_config(
    struct system_clock_source_xosc32k_config *const config)
{
    OSC32KCTRL_XOSC32K_Type temp = OSC32KCTRL->XOSC32K;

    temp.bit.STARTUP = config->startup_time;

    if (config->external_clock == SYSTEM_CLOCK_EXTERNAL_CRYSTAL) {
        temp.bit.XTALEN = 1;
    } else {
        temp.bit.XTALEN = 0;
    }

    temp.bit.EN1K = config->enable_1khz_output;
    temp.bit.EN32K = config->enable_32khz_output;

    temp.bit.ONDEMAND = config->on_demand;
    temp.bit.RUNSTDBY = config->run_in_standby;
    temp.bit.WRTLOCK  = config->write_once;

    /* Cache the new frequency in case the user needs to check the current
     * operating frequency later */
    _system_clock_inst.xosc32k.frequency = config->frequency;

    OSC32KCTRL->XOSC32K = temp;
}

/**
 * \brief Configure the DFLL clock source.
 *
 * Configures the Digital Frequency Locked Loop clock source with the given
 * configuration settings.
 *
 * \note The DFLL will be running when this function returns, as the DFLL module
 *       needs to be enabled in order to perform the module configuration.
 *
 * \param[in] config  DFLL configuration structure containing the new config
 */
void system_clock_source_dfll_set_config(
    struct system_clock_source_dfll_config *const config)
{
    _system_clock_inst.dfll.val =
        OSCCTRL_DFLLVAL_COARSE(config->coarse_value) |
        OSCCTRL_DFLLVAL_FINE(config->fine_value);

    _system_clock_inst.dfll.control =
        (uint32_t)config->wakeup_lock     |
        (uint32_t)config->stable_tracking |
        (uint32_t)config->quick_lock      |
        (uint32_t)config->chill_cycle     |
        ((uint32_t)config->on_demand << OSCCTRL_DFLLCTRL_ONDEMAND_Pos) |
        ((uint32_t)config->run_in_stanby << OSCCTRL_DFLLCTRL_RUNSTDBY_Pos);

    if (config->loop_mode == SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED) {

        _system_clock_inst.dfll.mul =
            OSCCTRL_DFLLMUL_CSTEP(config->coarse_max_step) |
            OSCCTRL_DFLLMUL_FSTEP(config->fine_max_step)   |
            OSCCTRL_DFLLMUL_MUL(config->multiply_factor);

        /* Enable the closed loop mode */
        _system_clock_inst.dfll.control |= config->loop_mode;
    }
    if (config->loop_mode == SYSTEM_CLOCK_DFLL_LOOP_MODE_USB_RECOVERY) {

        _system_clock_inst.dfll.mul =
            OSCCTRL_DFLLMUL_MUL(config->multiply_factor);

        /* Enable the USB recovery mode */
        _system_clock_inst.dfll.control |= config->loop_mode |
                                           OSCCTRL_DFLLCTRL_BPLCKC;
    }
}

/**
 * \brief Configure the DPLL clock source.
 *
 * Configures the Digital Phase-Locked Loop clock source with the given
 * configuration settings.
 *
 * \note The DPLL will be running when this function returns, as the DPLL module
 *       needs to be enabled in order to perform the module configuration.
 *
 * \param[in] config  DPLL configuration structure containing the new config
 */
void system_clock_source_dpll_set_config(
    struct system_clock_source_dpll_config *const config)
{

    uint32_t tmpldr;
    uint8_t  tmpldrfrac;
    uint32_t refclk;

    refclk = config->reference_frequency;

    /* Only reference clock REF1 can be divided */
    if (config->reference_clock == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_XOSC) {
        refclk = refclk / (2 * (config->reference_divider + 1));
    }

    /* Calculate LDRFRAC and LDR */
    tmpldr = (config->output_frequency << 4) / refclk;
    tmpldrfrac = tmpldr & 0x0f;
    tmpldr = (tmpldr >> 4) - 1;

    OSCCTRL->DPLLCTRLA.reg =
        ((uint32_t)config->on_demand << OSCCTRL_DPLLCTRLA_ONDEMAND_Pos) |
        ((uint32_t)config->run_in_standby << OSCCTRL_DPLLCTRLA_RUNSTDBY_Pos);

    OSCCTRL->DPLLRATIO.reg =
        OSCCTRL_DPLLRATIO_LDRFRAC(tmpldrfrac) |
        OSCCTRL_DPLLRATIO_LDR(tmpldr);

    while(OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO) {
    }

    OSCCTRL->DPLLCTRLB.reg =
        OSCCTRL_DPLLCTRLB_DIV(config->reference_divider) |
        ((uint32_t)config->lock_bypass << OSCCTRL_DPLLCTRLB_LBYPASS_Pos) |
        OSCCTRL_DPLLCTRLB_LTIME(config->lock_time) |
        OSCCTRL_DPLLCTRLB_REFCLK(config->reference_clock) |
        ((uint32_t)config->wake_up_fast << OSCCTRL_DPLLCTRLB_WUF_Pos) |
        ((uint32_t)config->low_power_enable << OSCCTRL_DPLLCTRLB_LPEN_Pos) |
        OSCCTRL_DPLLCTRLB_FILTER(config->filter);

    OSCCTRL->DPLLPRESC.reg  = OSCCTRL_DPLLPRESC_PRESC(config->prescaler);
    while(OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_DPLLPRESC) {
    }
    /*
     * Fck = Fckrx * (LDR + 1 + LDRFRAC / 16)
     */
    _system_clock_inst.dpll.frequency =
        (refclk * (((tmpldr + 1) << 4) + tmpldrfrac)) >> 4;
}

/**
 * \brief Writes the calibration values for a given oscillator clock source.
 *
 * Writes an oscillator calibration value to the given oscillator control
 * registers. The acceptable ranges are:
 *
 * For OSC32K:
 *  - 7 bits (max value 128)
 * For OSC16MHZ:
 *  - 8 bits (Max value 255)
 * For OSCULP:
 *  - 5 bits (Max value 32)
 *
 * \note The frequency range parameter applies only when configuring the 8MHz
 *       oscillator and will be ignored for the other oscillators.
 *
 * \param[in] clock_source       Clock source to calibrate
 * \param[in] calibration_value  Calibration value to write
 * \param[in] freq_range         Frequency range (8MHz oscillator only)
 *
 * \retval STATUS_OK               The calibration value was written
 *                                 successfully.
 * \retval STATUS_ERR_INVALID_ARG  The setting is not valid for selected clock
 *                                 source.
 */
enum status_code system_clock_source_write_calibration(
    const enum system_clock_source clock_source,
    const uint16_t calibration_value,
    const uint8_t freq_select)
{
    switch (clock_source) {
        case SYSTEM_CLOCK_SOURCE_OSC16M:
                    //to enable DSU test mode and add calibration value
                    return STATUS_OK;

            case SYSTEM_CLOCK_SOURCE_OSC32K:

                if (calibration_value > 128) {
                return STATUS_ERR_INVALID_ARG;
            }

            _system_osc32k_wait_for_sync();
            OSC32KCTRL->OSC32K.bit.CALIB = calibration_value;
            break;

        case SYSTEM_CLOCK_SOURCE_ULP32K:

            if (calibration_value > 32) {
                return STATUS_ERR_INVALID_ARG;
            }

            OSC32KCTRL->OSCULP32K.bit.CALIB = calibration_value;
            break;

        default:
            Assert(false);
            return STATUS_ERR_INVALID_ARG;
            break;
    }

    return STATUS_OK;
}

/**
 * \brief Enables a clock source.
 *
 * Enables a clock source which has been previously configured.
 *
 * \param[in] clock_source       Clock source to enable
 *
 * \retval STATUS_OK               Clock source was enabled successfully and
 *                                 is ready
 * \retval STATUS_ERR_INVALID_ARG  The clock source is not available on this
 *                                 device
 */
enum status_code system_clock_source_enable(
    const enum system_clock_source clock_source)
{
    switch (clock_source) {
        case SYSTEM_CLOCK_SOURCE_OSC16M:
                    OSCCTRL->OSC16MCTRL.reg |= OSCCTRL_OSC16MCTRL_ENABLE;
                return STATUS_OK;

            case SYSTEM_CLOCK_SOURCE_OSC32K:
                OSC32KCTRL->OSC32K.reg |= OSC32KCTRL_OSC32K_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_XOSC:
                OSCCTRL->XOSCCTRL.reg |= OSCCTRL_XOSCCTRL_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_XOSC32K:
                OSC32KCTRL->XOSC32K.reg |= OSC32KCTRL_XOSC32K_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_DFLL:
                _system_clock_inst.dfll.control |= OSCCTRL_DFLLCTRL_ENABLE;
                _system_clock_source_dfll_set_config_errata_9905();
                break;

            case SYSTEM_CLOCK_SOURCE_DPLL:
                OSCCTRL->DPLLCTRLA.reg |= OSCCTRL_DPLLCTRLA_ENABLE;
                while(OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_ENABLE) {
            }
            break;

        case SYSTEM_CLOCK_SOURCE_ULP32K:
            /* Always enabled */
            return STATUS_OK;

        default:
            Assert(false);
            return STATUS_ERR_INVALID_ARG;
    }

    return STATUS_OK;
}

/**
 * \brief Disables a clock source.
 *
 * Disables a clock source that was previously enabled.
 *
 * \param[in] clock_source  Clock source to disable
 *
 * \retval STATUS_OK               Clock source was disabled successfully
 * \retval STATUS_ERR_INVALID_ARG  An invalid or unavailable clock source was
 *                                 given
 */
enum status_code system_clock_source_disable(
    const enum system_clock_source clock_source)
{
    switch (clock_source) {
        case SYSTEM_CLOCK_SOURCE_OSC16M:
                    OSCCTRL->OSC16MCTRL.reg &= ~OSCCTRL_OSC16MCTRL_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_OSC32K:
                OSC32KCTRL->OSC32K.reg &= ~OSC32KCTRL_OSC32K_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_XOSC:
                OSCCTRL->XOSCCTRL.reg &= ~OSCCTRL_XOSCCTRL_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_XOSC32K:
                OSC32KCTRL->XOSC32K.reg &= ~OSC32KCTRL_XOSC32K_ENABLE;
                break;

            case SYSTEM_CLOCK_SOURCE_DFLL:
                _system_clock_inst.dfll.control &= ~OSCCTRL_DFLLCTRL_ENABLE;
                OSCCTRL->DFLLCTRL.reg = _system_clock_inst.dfll.control;
                break;
            case SYSTEM_CLOCK_SOURCE_DPLL:
                OSCCTRL->DPLLCTRLA.reg &= ~OSCCTRL_DPLLCTRLA_ENABLE;
                break;
            case SYSTEM_CLOCK_SOURCE_ULP32K:
            /* Not possible to disable */

            default:
                Assert(false);
                return STATUS_ERR_INVALID_ARG;

        }

        return STATUS_OK;
    }

    /**
     * \brief Checks if a clock source is ready.
     *
     * Checks if a given clock source is ready to be used.
     *
     * \param[in] clock_source  Clock source to check if ready
     *
     * \returns Ready state of the given clock source.
     *
     * \retval true   Clock source is enabled and ready
     * \retval false  Clock source is disabled or not yet ready
     */
    bool system_clock_source_is_ready(
        const enum system_clock_source clock_source)
{
    uint32_t mask = 0;

    switch (clock_source) {
        case SYSTEM_CLOCK_SOURCE_OSC16M:
            mask = OSCCTRL_STATUS_OSC16MRDY;
            return ((OSCCTRL->STATUS.reg & mask) == mask);

        case SYSTEM_CLOCK_SOURCE_OSC32K:
            mask = OSC32KCTRL_STATUS_OSC32KRDY;
            return ((OSC32KCTRL->STATUS.reg & mask) == mask);

        case SYSTEM_CLOCK_SOURCE_XOSC:
            mask = OSCCTRL_STATUS_XOSCRDY;
            return ((OSCCTRL->STATUS.reg & mask) == mask);

        case SYSTEM_CLOCK_SOURCE_XOSC32K:
            mask = OSC32KCTRL_STATUS_XOSC32KRDY;
            return ((OSC32KCTRL->STATUS.reg & mask) == mask);

        case SYSTEM_CLOCK_SOURCE_DFLL:
            if (CONF_CLOCK_DFLL_LOOP_MODE == SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED) {
                mask = (OSCCTRL_STATUS_DFLLRDY |
                        OSCCTRL_STATUS_DFLLLCKF | OSCCTRL_STATUS_DFLLLCKC);
            } else {
                mask = OSCCTRL_STATUS_DFLLRDY;
            }
            return ((OSCCTRL->STATUS.reg & mask) == mask);

        case SYSTEM_CLOCK_SOURCE_DPLL:
            return ((OSCCTRL->DPLLSTATUS.reg &
                     (OSCCTRL_DPLLSTATUS_CLKRDY | OSCCTRL_DPLLSTATUS_LOCK)) ==
                    (OSCCTRL_DPLLSTATUS_CLKRDY | OSCCTRL_DPLLSTATUS_LOCK));
        case SYSTEM_CLOCK_SOURCE_ULP32K:
            /* Not possible to disable */
            return true;

        default:
            return false;
    }
}

/* Include some checks for conf_clocks.h validation */
#include "clock_config_check.h"

#if !defined(__DOXYGEN__)
/** \internal
 *
 * Configures a Generic Clock Generator with the configuration from \c conf_clocks.h.
 */
#  define _CONF_CLOCK_GCLK_CONFIG(n, unused) \
	if (CONF_CLOCK_GCLK_##n##_ENABLE == true) { \
		struct system_gclk_gen_config gclk_conf;                          \
		system_gclk_gen_get_config_defaults(&gclk_conf);                  \
		gclk_conf.source_clock    = CONF_CLOCK_GCLK_##n##_CLOCK_SOURCE;   \
		gclk_conf.division_factor = CONF_CLOCK_GCLK_##n##_PRESCALER;      \
		gclk_conf.run_in_standby  = CONF_CLOCK_GCLK_##n##_RUN_IN_STANDBY; \
		gclk_conf.output_enable   = CONF_CLOCK_GCLK_##n##_OUTPUT_ENABLE;  \
		system_gclk_gen_set_config(GCLK_GENERATOR_##n, &gclk_conf);       \
		system_gclk_gen_enable(GCLK_GENERATOR_##n);                       \
	}

/** \internal
 *
 * Configures a Generic Clock Generator with the configuration from \c conf_clocks.h,
 * provided that it is not the main Generic Clock Generator channel.
 */
#  define _CONF_CLOCK_GCLK_CONFIG_NONMAIN(n, unused) \
		if (n > 0) { _CONF_CLOCK_GCLK_CONFIG(n, unused); }
#endif

/**
 * \brief Initialize clock system based on the configuration in conf_clocks.h.
 *
 * This function will apply the settings in conf_clocks.h when run from the user
 * application. All clock sources and GCLK generators are running when this function
 * returns.
 *
 * \note OSC16M is always enabled and if user selects other clocks for GCLK generators,
 * the OSC16M default enable can be disabled after system_clock_init. Make sure the
 * clock switches successfully before disabling OSC8M.
 */
void system_clock_init(void)
{
    /* Various bits in the INTFLAG register can be set to one at startup.
       This will ensure that these bits are cleared */
    OSCCTRL->INTFLAG.reg = OSCCTRL_INTFLAG_DFLLRDY;
    SUPC->INTFLAG.reg = SUPC_INTFLAG_BOD33RDY | SUPC_INTFLAG_BOD33DET;

    system_flash_set_waitstates(CONF_CLOCK_FLASH_WAIT_STATES);

    /*  Switch to PL2 to be sure configuration of GCLK0 is safe */
    system_switch_performance_level(SYSTEM_PERFORMANCE_LEVEL_2);

    /* XOSC */
#if CONF_CLOCK_XOSC_ENABLE == true
    struct system_clock_source_xosc_config xosc_conf;
    system_clock_source_xosc_get_config_defaults(&xosc_conf);

    xosc_conf.external_clock    = CONF_CLOCK_XOSC_EXTERNAL_CRYSTAL;
    xosc_conf.startup_time      = CONF_CLOCK_XOSC_STARTUP_TIME;
    xosc_conf.auto_gain_control = CONF_CLOCK_XOSC_AUTO_GAIN_CONTROL;
    xosc_conf.frequency         = CONF_CLOCK_XOSC_EXTERNAL_FREQUENCY;
    xosc_conf.on_demand         = CONF_CLOCK_XOSC_ON_DEMAND;
    xosc_conf.run_in_standby    = CONF_CLOCK_XOSC_RUN_IN_STANDBY;

    system_clock_source_xosc_set_config(&xosc_conf);
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC);
#endif

    /* XOSC32K */
#if CONF_CLOCK_XOSC32K_ENABLE == true
    struct system_clock_source_xosc32k_config xosc32k_conf;
    system_clock_source_xosc32k_get_config_defaults(&xosc32k_conf);

    xosc32k_conf.frequency           = 32768UL;
    xosc32k_conf.external_clock      = CONF_CLOCK_XOSC32K_EXTERNAL_CRYSTAL;
    xosc32k_conf.startup_time        = CONF_CLOCK_XOSC32K_STARTUP_TIME;
    xosc32k_conf.enable_1khz_output  = CONF_CLOCK_XOSC32K_ENABLE_1KHZ_OUPUT;
    xosc32k_conf.enable_32khz_output = CONF_CLOCK_XOSC32K_ENABLE_32KHZ_OUTPUT;
    xosc32k_conf.on_demand           = false;
    xosc32k_conf.run_in_standby      = CONF_CLOCK_XOSC32K_RUN_IN_STANDBY;

    system_clock_source_xosc32k_set_config(&xosc32k_conf);
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K);
    while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_XOSC32K));
    if (CONF_CLOCK_XOSC32K_ON_DEMAND) {
        OSC32KCTRL->XOSC32K.bit.ONDEMAND = 1;
    }
#endif

    /* OSCK32K */
#if CONF_CLOCK_OSC32K_ENABLE == true

    struct system_clock_source_osc32k_config osc32k_conf;
    system_clock_source_osc32k_get_config_defaults(&osc32k_conf);

    osc32k_conf.startup_time        = CONF_CLOCK_OSC32K_STARTUP_TIME;
    osc32k_conf.enable_1khz_output  = CONF_CLOCK_OSC32K_ENABLE_1KHZ_OUTPUT;
    osc32k_conf.enable_32khz_output = CONF_CLOCK_OSC32K_ENABLE_32KHZ_OUTPUT;
    osc32k_conf.on_demand           = CONF_CLOCK_OSC32K_ON_DEMAND;
    osc32k_conf.run_in_standby      = CONF_CLOCK_OSC32K_RUN_IN_STANDBY;

    system_clock_source_osc32k_set_config(&osc32k_conf);
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC32K);
#endif

    /* OSC16M */
    if (CONF_CLOCK_OSC16M_FREQ_SEL == SYSTEM_OSC16M_4M) {
        OSCCTRL->OSC16MCTRL.bit.ONDEMAND = CONF_CLOCK_OSC16M_ON_DEMAND ;
        OSCCTRL->OSC16MCTRL.bit.RUNSTDBY = CONF_CLOCK_OSC16M_RUN_IN_STANDBY;
    } else {
        _system_clock_source_osc16m_freq_sel();
    }

    /* DFLL Config (Open and Closed Loop) */
#if CONF_CLOCK_DFLL_ENABLE == true
    struct system_clock_source_dfll_config dfll_conf;
    system_clock_source_dfll_get_config_defaults(&dfll_conf);

    dfll_conf.loop_mode      = CONF_CLOCK_DFLL_LOOP_MODE;
    dfll_conf.on_demand      = false;
    dfll_conf.run_in_stanby  = CONF_CLOCK_DFLL_RUN_IN_STANDBY;

    /* Using DFLL48M COARSE CAL value from NVM Software Calibration Area Mapping
       in DFLL.COARSE helps to output a frequency close to 48 MHz.*/
#define NVM_DFLL_COARSE_POS    26 /* DFLL48M Coarse calibration value bit position.*/
#define NVM_DFLL_COARSE_SIZE   6  /* DFLL48M Coarse calibration value bit size.*/

    uint32_t coarse =( *((uint32_t *)(NVMCTRL_OTP5)
                         + (NVM_DFLL_COARSE_POS / 32))
                       >> (NVM_DFLL_COARSE_POS % 32))
                     & ((1 << NVM_DFLL_COARSE_SIZE) - 1);
    /* In some revision chip, the Calibration value is not correct */
    if (coarse == 0x3f) {
        coarse = 0x1f;
    }

    dfll_conf.coarse_value = coarse;

    if (CONF_CLOCK_DFLL_LOOP_MODE == SYSTEM_CLOCK_DFLL_LOOP_MODE_OPEN) {
        dfll_conf.fine_value   = CONF_CLOCK_DFLL_FINE_VALUE;
    }

#  if CONF_CLOCK_DFLL_QUICK_LOCK == true
    dfll_conf.quick_lock = SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE;
#  else
    dfll_conf.quick_lock = SYSTEM_CLOCK_DFLL_QUICK_LOCK_DISABLE;
#  endif

#  if CONF_CLOCK_DFLL_TRACK_AFTER_FINE_LOCK == true
    dfll_conf.stable_tracking = SYSTEM_CLOCK_DFLL_STABLE_TRACKING_TRACK_AFTER_LOCK;
#  else
    dfll_conf.stable_tracking = SYSTEM_CLOCK_DFLL_STABLE_TRACKING_FIX_AFTER_LOCK;
#  endif

#  if CONF_CLOCK_DFLL_KEEP_LOCK_ON_WAKEUP == true
    dfll_conf.wakeup_lock = SYSTEM_CLOCK_DFLL_WAKEUP_LOCK_KEEP;
#  else
    dfll_conf.wakeup_lock = SYSTEM_CLOCK_DFLL_WAKEUP_LOCK_LOSE;
#  endif

#  if CONF_CLOCK_DFLL_ENABLE_CHILL_CYCLE == true
    dfll_conf.chill_cycle = SYSTEM_CLOCK_DFLL_CHILL_CYCLE_ENABLE;
#  else
    dfll_conf.chill_cycle = SYSTEM_CLOCK_DFLL_CHILL_CYCLE_DISABLE;
#  endif

    if (CONF_CLOCK_DFLL_LOOP_MODE == SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED) {
        dfll_conf.multiply_factor = CONF_CLOCK_DFLL_MULTIPLY_FACTOR;
    }

    dfll_conf.coarse_max_step = CONF_CLOCK_DFLL_MAX_COARSE_STEP_SIZE;
    dfll_conf.fine_max_step   = CONF_CLOCK_DFLL_MAX_FINE_STEP_SIZE;

    if (CONF_CLOCK_DFLL_LOOP_MODE == SYSTEM_CLOCK_DFLL_LOOP_MODE_USB_RECOVERY) {
        dfll_conf.fine_value   = 0x1ff;
        dfll_conf.quick_lock = SYSTEM_CLOCK_DFLL_QUICK_LOCK_ENABLE;
        dfll_conf.stable_tracking = SYSTEM_CLOCK_DFLL_STABLE_TRACKING_FIX_AFTER_LOCK;
        dfll_conf.wakeup_lock = SYSTEM_CLOCK_DFLL_WAKEUP_LOCK_KEEP;
        dfll_conf.chill_cycle = SYSTEM_CLOCK_DFLL_CHILL_CYCLE_DISABLE;

        dfll_conf.multiply_factor = 48000;
    }

    system_clock_source_dfll_set_config(&dfll_conf);
#endif

    /* GCLK */
#if CONF_CLOCK_CONFIGURE_GCLK == true
    system_gclk_init();

    /* Configure all GCLK generators except for the main generator, which
     * is configured later after all other clock systems are set up */
    MREPEAT(GCLK_GEN_NUM, _CONF_CLOCK_GCLK_CONFIG_NONMAIN, ~);
#  if CONF_CLOCK_DFLL_ENABLE == true
    /* Enable DFLL reference clock if in closed loop mode */
    if (CONF_CLOCK_DFLL_LOOP_MODE == SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED) {
        struct system_gclk_chan_config dfll_gclk_chan_conf;

        system_gclk_chan_get_config_defaults(&dfll_gclk_chan_conf);
        dfll_gclk_chan_conf.source_generator = CONF_CLOCK_DFLL_SOURCE_GCLK_GENERATOR;
        system_gclk_chan_set_config(OSCCTRL_GCLK_ID_DFLL48, &dfll_gclk_chan_conf);
        system_gclk_chan_enable(OSCCTRL_GCLK_ID_DFLL48);
    }
#  endif

#  if CONF_CLOCK_DPLL_ENABLE == true
    /* Enable DPLL internal lock timer and reference clock */
    struct system_gclk_chan_config dpll_gclk_chan_conf;
    system_gclk_chan_get_config_defaults(&dpll_gclk_chan_conf);
    if (CONF_CLOCK_DPLL_LOCK_TIME != SYSTEM_CLOCK_SOURCE_DPLL_LOCK_TIME_DEFAULT) {
        dpll_gclk_chan_conf.source_generator = CONF_CLOCK_DPLL_LOCK_GCLK_GENERATOR;
        system_gclk_chan_set_config(OSCCTRL_GCLK_ID_FDPLL32K, &dpll_gclk_chan_conf);
        system_gclk_chan_enable(OSCCTRL_GCLK_ID_FDPLL32K);
    }

    if (CONF_CLOCK_DPLL_REFERENCE_CLOCK == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_GCLK) {
        dpll_gclk_chan_conf.source_generator = CONF_CLOCK_DPLL_REFERENCE_GCLK_GENERATOR;
        system_gclk_chan_set_config(OSCCTRL_GCLK_ID_FDPLL, &dpll_gclk_chan_conf);
        system_gclk_chan_enable(OSCCTRL_GCLK_ID_FDPLL);
    }
#  endif
#endif

    /* DFLL Enable (Open and Closed Loop) */
#if CONF_CLOCK_DFLL_ENABLE == true
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DFLL);
    while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_DFLL));
    if (CONF_CLOCK_DFLL_ON_DEMAND) {
        OSCCTRL->DFLLCTRL.bit.ONDEMAND = 1;
    }
#endif

    /* DPLL */
#  if (CONF_CLOCK_DPLL_ENABLE == true)

    /* Enable DPLL reference clock */
    if (CONF_CLOCK_DPLL_REFERENCE_CLOCK == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_XOSC32K) {
        /* XOSC32K should have been enabled for GCLK_XOSC32 */
        Assert(CONF_CLOCK_XOSC32K_ENABLE);
    } else if (CONF_CLOCK_DPLL_REFERENCE_CLOCK == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_XOSC) {
        /* XOSC should have been enabled for GCLK_XOSC */
        Assert(CONF_CLOCK_XOSC_ENABLE);
    } else if (CONF_CLOCK_DPLL_REFERENCE_CLOCK == SYSTEM_CLOCK_SOURCE_DPLL_REFERENCE_CLOCK_GCLK) {
        /* GCLK should have been enabled */
        Assert(CONF_CLOCK_CONFIGURE_GCLK);
    } else {
        Assert(false);
    }

    struct system_clock_source_dpll_config dpll_config;
    system_clock_source_dpll_get_config_defaults(&dpll_config);

    dpll_config.on_demand        = false;
    dpll_config.run_in_standby   = CONF_CLOCK_DPLL_RUN_IN_STANDBY;
    dpll_config.lock_bypass      = CONF_CLOCK_DPLL_LOCK_BYPASS;
    dpll_config.wake_up_fast     = CONF_CLOCK_DPLL_WAKE_UP_FAST;
    dpll_config.low_power_enable = CONF_CLOCK_DPLL_LOW_POWER_ENABLE;

    dpll_config.filter           = CONF_CLOCK_DPLL_FILTER;
    dpll_config.lock_time        = CONF_CLOCK_DPLL_LOCK_TIME;

    dpll_config.reference_clock     = CONF_CLOCK_DPLL_REFERENCE_CLOCK;
    dpll_config.reference_frequency = CONF_CLOCK_DPLL_REFERENCE_FREQUENCY;
    dpll_config.reference_divider   = CONF_CLOCK_DPLL_REFERENCE_DIVIDER;
    dpll_config.output_frequency    = CONF_CLOCK_DPLL_OUTPUT_FREQUENCY;
    dpll_config.prescaler           = CONF_CLOCK_DPLL_PRESCALER;

    system_clock_source_dpll_set_config(&dpll_config);
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DPLL);
    while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_DPLL));
    if (CONF_CLOCK_DPLL_ON_DEMAND) {
        OSCCTRL->DPLLCTRLA.bit.ONDEMAND = 1;
    }

#  endif

    /* CPU and BUS clocks */
    system_cpu_clock_set_divider(CONF_CLOCK_CPU_DIVIDER);
    system_main_clock_set_failure_detect(CONF_CLOCK_CPU_CLOCK_FAILURE_DETECT);
    system_low_power_clock_set_divider(CONF_CLOCK_LOW_POWER_DIVIDER);
    system_backup_clock_set_divider(CONF_CLOCK_BACKUP_DIVIDER);

    /* GCLK 0 */
#if CONF_CLOCK_CONFIGURE_GCLK == true
    /* Configure the main GCLK last as it might depend on other generators */
    _CONF_CLOCK_GCLK_CONFIG(0, ~);
#endif

    /* If CPU frequency is less than 12MHz, scale down performance level to PL0 */
    uint32_t cpu_freq = system_cpu_clock_get_hz();
    if (cpu_freq <= 12000000) {
        system_switch_performance_level(SYSTEM_PERFORMANCE_LEVEL_0);
    }
}
