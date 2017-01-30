/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
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

#ifndef _FSL_SMARTCARD_PHY_EMVSIM_H_
#define _FSL_SMARTCARD_PHY_EMVSIM_H_

#include "fsl_smartcard.h"

/*!
 * @addtogroup smartcard_phy_emvsim_driver
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Smart card define which specifies the adjustment number of clock cycles during which an ATR string has to be received.
 */
#define SMARTCARD_ATR_DURATION_ADJUSTMENT (360u)

/*! @brief Smart card define which specifies the adjustment number of clock cycles until an initial 'TS' character has to be
 * received. */
#define SMARTCARD_INIT_DELAY_CLOCK_CYCLES_ADJUSTMENT (4200u)

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Fills in the smartcardInterfaceConfig structure with default values.
 *
 * @param config The user configuration structure of type smartcard_interface_config_t.
 * Function fill in members:
 *      clockToResetDelay = 42000,
 *      vcc = kSmartcardVoltageClassB3_3V,
 * with default values.
 */
void SMARTCARD_PHY_EMVSIM_GetDefaultConfig(smartcard_interface_config_t *config);

/*!
 * @brief Configures a Smart card interface.
 *
 * @param base The Smart card peripheral module base address.
 * @param config The user configuration structure of type smartcard_interface_config_t. The user
 *  is responsible to fill out the members of this structure and to pass the pointer of this structure
 *  into this function or call SMARTCARD_PHY_EMVSIMInitUserConfigDefault to fill out structure with default values.
 * @param srcClock_Hz Smart card clock generation module source clock.
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError for an error.
 */
status_t SMARTCARD_PHY_EMVSIM_Init(EMVSIM_Type *base, const smartcard_interface_config_t *config, uint32_t srcClock_Hz);

/*!
 * @brief De-initializes a Smart card interface, stops the Smart card clock, and disables the VCC.
 *
 * @param base Smart card peripheral module base address.
 * @param config Smart card configuration structure.
 */
void SMARTCARD_PHY_EMVSIM_Deinit(EMVSIM_Type *base, const smartcard_interface_config_t *config);

/*!
 * @brief Activates the Smart card IC.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a Smart card driver context structure.
 * @param resetType type of reset to be performed, possible values
 *                       = kSmartcardColdReset, kSmartcardWarmReset
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError for an error.
 */
status_t SMARTCARD_PHY_EMVSIM_Activate(EMVSIM_Type *base,
                                       smartcard_context_t *context,
                                       smartcard_reset_type_t resetType);

/*!
 * @brief De-activates the Smart card IC.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a Smart card driver context structure.
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError for an error.
 */
status_t SMARTCARD_PHY_EMVSIM_Deactivate(EMVSIM_Type *base, smartcard_context_t *context);

/*!
 * @brief Controls the Smart card interface IC.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a Smart card driver context structure.
 * @param control A interface command type.
 * @param param Integer value specific to control type
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError for an error.
 */
status_t SMARTCARD_PHY_EMVSIM_Control(EMVSIM_Type *base,
                                      smartcard_context_t *context,
                                      smartcard_interface_control_t control,
                                      uint32_t param);
/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_SMARTCARD_PHY_EMVSIM_H_*/
