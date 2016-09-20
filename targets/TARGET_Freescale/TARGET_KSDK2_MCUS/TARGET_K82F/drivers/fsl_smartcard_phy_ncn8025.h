/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#ifndef _FSL_SMARTCARD_PHY_NCN8025_H_
#define _FSL_SMARTCARD_PHY_NCN8025_H_

#include "fsl_smartcard.h"

/*!
 * @addtogroup smartcard_phy_ncn8025_driver
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief SMARTCARD define which specify adjustment number of clock cycles during which ATR string has to be received
 */
#define SMARTCARD_ATR_DURATION_ADJUSTMENT (360u)

/*! @brief SMARTCARD define which specify adjustment number of clock cycles until initial 'TS' character has to be
 * received */
#define SMARTCARD_INIT_DELAY_CLOCK_CYCLES_ADJUSTMENT (4200u)

/*! @brief Masks for NCN8025 status register */
#define SMARTCARD_NCN8025_STATUS_PRES (0x01u)             /*!< SMARTCARD phy NCN8025 smartcard present status */
#define SMARTCARD_NCN8025_STATUS_ACTIVE (0x02u)           /*!< SMARTCARD phy NCN8025 smartcard active status */
#define SMARTCARD_NCN8025_STATUS_FAULTY (0x04u)           /*!< SMARTCARD phy NCN8025 smartcard faulty status */
#define SMARTCARD_NCN8025_STATUS_CARD_REMOVED (0x08u)     /*!< SMARTCARD phy NCN8025 smartcard removed status */
#define SMARTCARD_NCN8025_STATUS_CARD_DEACTIVATED (0x10u) /*!< SMARTCARD phy NCN8025 smartcard deactivated status */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Fill in config structure with default values.
 *
 * @param config The smartcard user configuration structure which contains configuration structure of type
 * smartcard_interface_config_t.
 * Function fill in members:
 *      clockToResetDelay = 42000,
 *      vcc = kSmartcardVoltageClassB3_3V,
 * with default values.
 */
void SMARTCARD_PHY_NCN8025_GetDefaultConfig(smartcard_interface_config_t *config);

/*!
 * @brief Initializes an SMARTCARD interface instance for operation.
 *
 * @param base The SMARTCARD peripheral base address.
 * @param config The user configuration structure of type smartcard_interface_config_t. The user
 *  can call to fill out configuration structure function SMARTCARD_PHY_NCN8025_GetDefaultConfig().
 * @param srcClock_Hz Smartcard clock generation module source clock.
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError in case of error.
 */
status_t SMARTCARD_PHY_NCN8025_Init(void *base, smartcard_interface_config_t const *config, uint32_t srcClock_Hz);

/*!
 * @brief De-initializes an SMARTCARD interface. Stops smartcard clock and disable VCC.
 *
 * @param base The SMARTCARD peripheral module base address.
 * @param config The user configuration structure of type smartcard_interface_config_t.
 */
void SMARTCARD_PHY_NCN8025_Deinit(void *base, smartcard_interface_config_t *config);

/*!
 * @brief Activates the smart card IC.
 *
 * @param base The SMARTCARD peripheral module base address.
 * @param context A pointer to a smartcard driver context structure.
 * @param resetType type of reset to be performed, possible values
 *                       = kSmartcardColdReset, kSmartcardWarmReset
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError in case of error.
 */
status_t SMARTCARD_PHY_NCN8025_Activate(void *base, smartcard_context_t *context, smartcard_reset_type_t resetType);

/*!
 * @brief De-activates the smart card IC.
 *
 * @param base The SMARTCARD peripheral module base address.
 * @param context A pointer to a smartcard driver context structure.
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError in case of error.
 */
status_t SMARTCARD_PHY_NCN8025_Deactivate(void *base, smartcard_context_t *context);

/*!
 * @brief Controls SMARTCARD interface IC.
 *
 * @param base The SMARTCARD peripheral module base address.
 * @param context A pointer to a smartcard driver context structure.
 * @param control A interface command type.
 * @param param Integer value specific to control type
 *
 * @retval kStatus_SMARTCARD_Success or kStatus_SMARTCARD_OtherError in case of error.
 */
status_t SMARTCARD_PHY_NCN8025_Control(void *base,
                                       smartcard_context_t *context,
                                       smartcard_interface_control_t control,
                                       uint32_t param);

/*!
 * @brief SMARTCARD interface IC IRQ ISR.
 *
 * @param base The SMARTCARD peripheral module base address.
 * @param context The smartcard context pointer.
 */
void SMARTCARD_PHY_NCN8025_IRQHandler(void *base, smartcard_context_t *context);
/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_SMARTCARD_NCN8025_H_*/
