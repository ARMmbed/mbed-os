/**
 * \file
 *
 * \brief FLEXCOM driver for SAM.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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

#include "flexcom.h"
#include "sysclk.h"
#include "sleepmgr.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond


    /**
     * \brief Enable the FLEXCOM module.
     *
     * \param p_flexcom  Pointer to a FLEXCOM instance.
     *
     */
    void flexcom_enable(Flexcom *p_flexcom)
{
    sleepmgr_lock_mode(SLEEPMGR_ACTIVE);
    /* Enable PMC clock for FLEXCOM */
#ifdef ID_FLEXCOM7
    if (p_flexcom == FLEXCOM7) {
        sysclk_enable_peripheral_clock(ID_FLEXCOM7);
        /* Enable PCK output */
        pmc_disable_pck(PMC_PCK_7);
        pmc_switch_pck_to_mck(PMC_PCK_7, PMC_PCK_PRES_CLK_1);
        pmc_enable_pck(PMC_PCK_7);
    } else
#endif
#ifdef ID_FLEXCOM6
        if (p_flexcom == FLEXCOM6) {
            sysclk_enable_peripheral_clock(ID_FLEXCOM6);
            /* Enable PCK output */
            pmc_disable_pck(PMC_PCK_7);
            pmc_switch_pck_to_mck(PMC_PCK_7, PMC_PCK_PRES_CLK_1);
            pmc_enable_pck(PMC_PCK_7);
        } else
#endif
#ifdef ID_FLEXCOM5
            if (p_flexcom == FLEXCOM5) {
                sysclk_enable_peripheral_clock(ID_FLEXCOM5);
                /* Enable PCK output */
                pmc_disable_pck(PMC_PCK_7);
                pmc_switch_pck_to_mck(PMC_PCK_7, PMC_PCK_PRES_CLK_1);
                pmc_enable_pck(PMC_PCK_7);
            } else
#endif
#ifdef ID_FLEXCOM4
                if (p_flexcom == FLEXCOM4) {
                    sysclk_enable_peripheral_clock(ID_FLEXCOM4);
                    /* Enable PCK output */
                    pmc_disable_pck(PMC_PCK_7);
                    pmc_switch_pck_to_mck(PMC_PCK_7, PMC_PCK_PRES_CLK_1);
                    pmc_enable_pck(PMC_PCK_7);
                } else
#endif
#ifdef ID_FLEXCOM3
                    if (p_flexcom == FLEXCOM3) {
                        sysclk_enable_peripheral_clock(ID_FLEXCOM3);
                        /* Enable PCK output */
                        pmc_disable_pck(PMC_PCK_6);
                        pmc_switch_pck_to_mck(PMC_PCK_6, PMC_PCK_PRES_CLK_1);
                        pmc_enable_pck(PMC_PCK_6);
                    } else
#endif
#ifdef ID_FLEXCOM2
                        if (p_flexcom == FLEXCOM2) {
                            sysclk_enable_peripheral_clock(ID_FLEXCOM2);
                            /* Enable PCK output */
                            pmc_disable_pck(PMC_PCK_6);
                            pmc_switch_pck_to_mck(PMC_PCK_6, PMC_PCK_PRES_CLK_1);
                            pmc_enable_pck(PMC_PCK_6);
                        } else
#endif
#ifdef ID_FLEXCOM1
                            if (p_flexcom == FLEXCOM1) {
                                sysclk_enable_peripheral_clock(ID_FLEXCOM1);
                                /* Enable PCK output */
                                pmc_disable_pck(PMC_PCK_6);
                                pmc_switch_pck_to_mck(PMC_PCK_6, PMC_PCK_PRES_CLK_1);
                                pmc_enable_pck(PMC_PCK_6);
                            } else
#endif
#ifdef ID_FLEXCOM0
                                if (p_flexcom == FLEXCOM0) {
                                    sysclk_enable_peripheral_clock(ID_FLEXCOM0);
                                    /* Enable PCK output */
                                    pmc_disable_pck(PMC_PCK_6);
                                    pmc_switch_pck_to_mck(PMC_PCK_6, PMC_PCK_PRES_CLK_1);
                                    pmc_enable_pck(PMC_PCK_6);
                                } else
#endif
                                {
                                    Assert(false);
                                }
}

/**
 * \brief Disable the FLEXCOM module.
 *
 * \param p_flexcom  Pointer to a FLEXCOM instance.
 *
 */
void flexcom_disable(Flexcom *p_flexcom)
{
    sleepmgr_unlock_mode(SLEEPMGR_ACTIVE);
    /* Enable PMC clock for FLEXCOM */
#ifdef ID_FLEXCOM7
    if (p_flexcom == FLEXCOM7) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM7);
        /* Disable PCK output */
        pmc_disable_pck(PMC_PCK_7);
    } else
#endif
#ifdef ID_FLEXCOM6
        if (p_flexcom == FLEXCOM6) {
            sysclk_disable_peripheral_clock(ID_FLEXCOM6);
            /* Disable PCK output */
            pmc_disable_pck(PMC_PCK_7);
        } else
#endif
#ifdef ID_FLEXCOM5
            if (p_flexcom == FLEXCOM5) {
                sysclk_disable_peripheral_clock(ID_FLEXCOM5);
                /* Disable PCK output */
                pmc_disable_pck(PMC_PCK_7);
            } else
#endif
#ifdef ID_FLEXCOM4
                if (p_flexcom == FLEXCOM4) {
                    sysclk_disable_peripheral_clock(ID_FLEXCOM4);
                    /* Disable PCK output */
                    pmc_disable_pck(PMC_PCK_7);
                } else
#endif
#ifdef ID_FLEXCOM3
                    if (p_flexcom == FLEXCOM3) {
                        sysclk_disable_peripheral_clock(ID_FLEXCOM3);
                        /* Disable PCK output */
                        pmc_disable_pck(PMC_PCK_6);
                    } else
#endif
#ifdef ID_FLEXCOM2
                        if (p_flexcom == FLEXCOM2) {
                            sysclk_disable_peripheral_clock(ID_FLEXCOM2);
                            /* Disable PCK output */
                            pmc_disable_pck(PMC_PCK_6);
                        } else
#endif
#ifdef ID_FLEXCOM1
                            if (p_flexcom == FLEXCOM1) {
                                sysclk_disable_peripheral_clock(ID_FLEXCOM1);
                                /* Disable PCK output */
                                pmc_disable_pck(PMC_PCK_6);
                            } else
#endif
#ifdef ID_FLEXCOM0
                                if (p_flexcom == FLEXCOM0) {
                                    /* Disable PCK output */
                                    pmc_disable_pck(PMC_PCK_6);
                                    sysclk_disable_peripheral_clock(ID_FLEXCOM0);
                                } else
#endif
                                {
                                    Assert(false);
                                }
}

/**
 * \brief Set the FLEXCOM opration mode.
 *
 * \param p_flexcom  Pointer to a FLEXCOM instance.
 * \param opmode  Opration mode.
 *
 */
void flexcom_set_opmode(Flexcom *p_flexcom, enum flexcom_opmode opmode)
{
    p_flexcom->FLEXCOM_MR = opmode;
}

/**
 * \brief Set the FLEXCOM opration mode.
 *
 * \param p_flexcom  Pointer to a FLEXCOM instance.
 * \param opmode  Opration mode.
 *
 */
void flexcom_get_opmode(Flexcom *p_flexcom, enum flexcom_opmode *opmode)
{
    *opmode = (enum flexcom_opmode)(p_flexcom->FLEXCOM_MR & FLEXCOM_MR_OPMODE_Msk);
}

/**
 * \brief Write to the FLEXCOM.
 *
 * \param p_flexcom  Pointer to a FLEXCOM instance.
 * \param data  Data to be tansfer.
 *
 */
void flexcom_write(Flexcom *p_flexcom, uint32_t data)
{
    p_flexcom->FLEXCOM_THR = data;
}

/**
 * \brief Read the FLEXCOM data.
 *
 * \param p_flexcom  Pointer to a FLEXCOM instance.
 * \param data  Data received.
 *
 */
void flexcom_read(Flexcom *p_flexcom, uint32_t *data)
{
    *data = p_flexcom->FLEXCOM_RHR;
}


/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
