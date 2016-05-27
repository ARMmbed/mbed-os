/**
 * \file
 *
 * \brief Supply Controller (SUPC) driver for SAM.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
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

#include "supc.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

    /**
     * \defgroup sam_drivers_supc_group Supply Controller (SUPC)
     *
     * Driver for the SUPC (Supply Controller). This driver provides access to the main
     * features of the Supply Controller.
     *
     * @{
     */

#if (!SAMG)
    /**
     * \brief Switch off the voltage regulator to put the device in backup mode.
     *
     * \param p_supc Pointer to a SUPC instance.
     */
    void supc_enable_backup_mode(Supc *p_supc)
{
    p_supc->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF;
    __WFE();
    __WFI();
}

/**
 * \brief Enable the internal voltage regulator.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_voltage_regulator(Supc *p_supc)
{
#if (SAM3U || SAM3XA)
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_VDDIORDYONREG));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_VDDIORDYONREG;
#else
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_ONREG));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_ONREG;
#endif
}

/**
 * \brief Disable the internal voltage regulator to supply VDDCORE with an external supply.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_voltage_regulator(Supc *p_supc)
{
#if (SAM3U || SAM3XA)
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_VDDIORDYONREG));
#else
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_ONREG));
#endif
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}
#endif

/**
 * \brief Switch slow clock source selection to external 32k (Xtal or Bypass) oscillator.
 * This function disables the PLLs.
 *
 * \note Switching sclk back to 32krc is only possible by shutting down the VDDIO power supply.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void supc_switch_sclk_to_32kxtal(Supc *p_supc, uint32_t ul_bypass)
{
    /* Set Bypass mode if required */
    if (ul_bypass == 1) {
        p_supc->SUPC_MR |= SUPC_MR_KEY_PASSWD | SUPC_MR_OSCBYPASS;
    } else {
        p_supc->SUPC_MR &= ~(SUPC_MR_KEY_PASSWD | SUPC_MR_OSCBYPASS);
    }

    p_supc->SUPC_CR |= SUPC_CR_KEY_PASSWD | SUPC_CR_XTALSEL;
}

/**
 * \brief Enable the core brownout detector.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_brownout_detector(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_BODDIS));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}

/**
 * \brief Disable the core brownout detector.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_brownout_detector(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_BODDIS));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_BODDIS;
}

/**
 * \brief Enable the assertion of core reset signal when a brownout detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_brownout_reset(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_BODRSTEN));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_BODRSTEN;
}

/**
 * \brief Disable the assertion of core reset signal when a brownout detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_brownout_reset(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_BODRSTEN));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}

/**
 * \brief Set Supply monitor threshold.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_threshold Supply monitor threshold (between 1.9V and 3.4V).
 */
void supc_set_monitor_threshold(Supc *p_supc, uint32_t ul_threshold)
{
    uint32_t ul_smmr = p_supc->SUPC_SMMR & (~SUPC_SMMR_SMTH_Msk);
    p_supc->SUPC_SMMR = ul_smmr | (SUPC_SMMR_SMTH_Msk & ul_threshold);
}

/**
 * \brief Set Supply monitor sampling period.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_period Supply monitor sampling period.
 */
void supc_set_monitor_sampling_period(Supc *p_supc, uint32_t ul_period)
{
    uint32_t ul_smmr = p_supc->SUPC_SMMR & (~SUPC_SMMR_SMSMPL_Msk);
    p_supc->SUPC_SMMR = ul_smmr | (SUPC_SMMR_SMSMPL_Msk & ul_period);
}

/**
 * \brief Enable the assertion of the core reset signal when a supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_monitor_reset(Supc *p_supc)
{
    p_supc->SUPC_SMMR |= SUPC_SMMR_SMRSTEN;
}

/**
 * \brief Disable the assertion of core reset signal when a supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_monitor_reset(Supc *p_supc)
{
    p_supc->SUPC_SMMR &= ~SUPC_SMMR_SMRSTEN;
}

/**
 * \brief Enable the assertion of SUPC interrupt signal when a supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_monitor_interrupt(Supc *p_supc)
{
    p_supc->SUPC_SMMR |= SUPC_SMMR_SMIEN;
}

/**
 * \brief Disable the assertion of SUPC interrupt signal when a supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_monitor_interrupt(Supc *p_supc)
{
    p_supc->SUPC_SMMR &= ~SUPC_SMMR_SMIEN;
}

#if (!(SAMG51 || SAMG53 || SAMG54))
/**
 * \brief Set system controller wake up mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_mode Bitmask of wake up mode (please refer to datasheet for more details).
 */
void supc_set_wakeup_mode(Supc *p_supc, uint32_t ul_mode)
{
    p_supc->SUPC_WUMR = ul_mode;
}

/**
 * \brief Set system controller wake up inputs.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_inputs Bitmask of wake-up inputs that can force wake up of
 * the core power supply.
 * \param ul_transition Bitmask of level transition of the wake-up inputs.
 * 1 means a high-to-low level transition forces the wake up of core power supply.
 * 0 means a low-to-high level transition forces the wake up of core power supply.
 */
void supc_set_wakeup_inputs(Supc *p_supc, uint32_t ul_inputs,
                            uint32_t ul_transition)
{
    p_supc->SUPC_WUIR = ul_inputs | ul_transition;
}
#endif

/**
 * \brief Get supply controller status.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return The status of supply controller.
 */
uint32_t supc_get_status(Supc *p_supc)
{
    return p_supc->SUPC_SR;
}

#if (SAM4C || SAM4CP || SAM4CM)
/**
 * \brief Enable Backup Area Power-On Reset.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_backup_power_on_reset(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_BUPPOREN));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_BUPPOREN;
}

/**
 * \brief Disable Backup Area Power-On Reset.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_backup_power_on_reset(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_BUPPOREN));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}
/**
 * \brief Get SLCD power mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return The mode of SLCDC.
 */
enum slcdc_power_mode supc_get_slcd_power_mode(Supc *p_supc)
{
    return (enum slcdc_power_mode)(p_supc->SUPC_MR & SUPC_MR_LCDMODE_Msk);
}

/**
 * \brief Set SLCD power mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param mode The mode of SLCDC.
 */
void supc_set_slcd_power_mode(Supc *p_supc, enum slcdc_power_mode mode)
{
    enum slcdc_power_mode pre_mode;
    uint32_t tmp;

    pre_mode = supc_get_slcd_power_mode(p_supc);

    if ((pre_mode == SLCDC_POWER_MODE_LCDON_EXTVR) &&
            (mode == SLCDC_POWER_MODE_LCDON_INVR)) {
        return;
    } else if ((pre_mode == SLCDC_POWER_MODE_LCDON_INVR) &&
               (mode == SLCDC_POWER_MODE_LCDON_EXTVR)) {
        return;
    }
    tmp = p_supc->SUPC_MR;
    tmp &= ~SUPC_MR_LCDMODE_Msk;
    tmp |=  SUPC_MR_KEY_PASSWD | mode;
    p_supc->SUPC_MR = tmp;

    if (mode == SLCDC_POWER_MODE_LCDOFF) {
        while(supc_get_status(p_supc) & SUPC_SR_LCDS_ENABLED);
    } else {
        while(!(supc_get_status(p_supc) & SUPC_SR_LCDS_ENABLED));
    }
}

/**
 * \brief Set LCD Voltage Regulator Output.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param vol  The voltage of Regulator Output.
 */
void supc_set_slcd_vol(Supc *p_supc, uint32_t vol)
{
    uint32_t tmp= p_supc->SUPC_MR;
    tmp &= ~SUPC_MR_LCDVROUT_Msk;
    tmp |=  SUPC_MR_KEY_PASSWD |  SUPC_MR_LCDVROUT(vol);
    p_supc->SUPC_MR = tmp;

}
#endif

#if SAMG54
/**
 * \brief Set the internal voltage regulator to use factory trim value.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_set_regulator_trim_factory(Supc *p_supc)
{
    uint32_t ul_mr = p_supc->SUPC_MR &
                     (~(SUPC_MR_VRVDD_Msk | SUPC_MR_VDDSEL_USER_VRVDD));
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}

/**
 * \brief Set the internal voltage regulator trim value.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param value the trim value.
 *
 * \note For the trim value in 96M PLL, please read the value in flash unique identifier area.
 */
void supc_set_regulator_trim_user(Supc *p_supc, uint32_t value)
{
    uint32_t ul_mr = p_supc->SUPC_MR & (~SUPC_MR_VRVDD_Msk);
    p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_VDDSEL_USER_VRVDD
                      | SUPC_MR_VRVDD(value);
}

#endif

#if (SAMV70 || SAMV71 || SAME70 || SAMS70)
/**
 * \brief SRAM On In Backup Mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 */
void supc_backup_sram_on(Supc *p_supc)
{
    p_supc->SUPC_MR |= (SUPC_MR_KEY_PASSWD | SUPC_MR_BKUPRETON);
}

/**
 * \brief SRAM Off In Backup Mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 */
void supc_backup_sram_off(Supc *p_supc)
{
    p_supc->SUPC_MR &= (~(SUPC_MR_KEY_PASSWD | SUPC_MR_BKUPRETON));
}
#endif

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
