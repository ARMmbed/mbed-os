/***************************************************************************//**
 * @file pa.h
 * @brief RADIO PA API
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef __RADIO_PA_H
#define __RADIO_PA_H

#include <stdint.h>
#include <stdbool.h>

#include "timing_state.h"
#include "rail_chip_specific.h"

// Once this is a RAIL API this code can be removed as rail-types.h does this
#ifndef RAIL_ENUM
#ifdef DOXYGEN_SHOULD_SKIP_THIS
/// The RAIL library does not use actual enums because the ARM EABI leaves their
/// size ambiguous. This ambiguity causes problems if the application is built
/// with different flags than the library. To work around this we use uint8_t
/// typedefs in compiled code for all enums. For documentation purposes this is
/// converted to an actual enum since it's much easier to read in Doxygen.
#define RAIL_ENUM(name) enum name
#else
/// Define used for the actual RAIL library which sets each enum to a uint8_t
/// typedef and creates a named enum structure for the enumeration values.
#define RAIL_ENUM(name) typedef uint8_t name; enum name##_enum
// For debugging use the following define to turn this back into a proper enum
// #define RAIL_ENUM(name) typedef enum name##_enum name; enum name##_enum
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup Chip_Specific
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EFR32xG1x_PA
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/

/**
 * @brief
 *   Initilize the PA settings based on the settings provided in the paInit
 *   structure.
 *
 * @param[in] paConfig
 *   Pointer to a structure containing the desired PA configuration settings.
 *
 * @return
 *   RAIL_Status_t indicating success
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
RAIL_Status_t PA_Config(const RAIL_TxPowerConfig_t *paConfig);

/**
 * @brief
 *   Get the current PA settings in use
 *
 * @param[out] config
 *   Pointer to memory location into which the configuration will be
 *   copied
 *
 * @return
 *   RAIL_Status_t indicating success
 */
RAIL_Status_t PA_GetTxPowerConfig(RAIL_TxPowerConfig_t *config);

/**
 * @brief
 *   Returns the current power level of transmit power
 *
 * @return
 *   Current power level in deci-dBm
 */
RAIL_TxPowerLevel_t PA_GetPowerLevel(void);

/**
 * @brief
 *   Return the current ramp time in microseconds
 *
 * @return
 *   Current ramp time in microseconds
 */
uint32_t PA_GetRampTime(void);

/**
 * @brief
 *   Sets up the ramp configuration so that it best matches the given ramp time
 *
 * @details
 *   Each PA has a distinct ramp level and ramp rate that can be used to
 *   achieve various ramp times.  This API will pick the ramp rate that closest
 *   approximates the desired ramp time.
 *
 * @param[in] ramptime
 *   Desired ramp time in microseconds
 *
 * @return
 *   The actual ramp time that was set in microseconds.
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
uint32_t PA_SetRampTime(uint32_t ramptime, StateTimings_t *timings);

/**
 * Enable/Disable PA calibration
 *
 * @param[in] enable Enables/Disables PA calibration
 * @return void
 *
 * Enabling this will ensure that the PA power remains constant chip to chip.
 * By default this feature is disabled after reset.
 */
void PA_EnableCal(bool enable);

/***************************************************************************//**
 * @addtogroup EFR32xG1x_PA_Advanced
 * @{
 ******************************************************************************/

/**
 * @brief
 *   Set PACTUNE value for TX and RX mode.
 *
 *   This value can vary depending on band and match and board design.
 *
 * @param[in] txPaCtuneValue
 *   Transmit value for pa ctune
 * @param[in] rxPaCtuneValue
 *   Receive value for pa ctune
 *
 * @note PACTUNE will reset to default values when PA_Config() or
 *       RAIL_ConfigRadio() are called.
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
void PA_SetCTune(uint8_t txPaCtuneValue, uint8_t rxPaCtuneValue);

/**
 * @brief
 *   Set the output power level based on power steps available in the chosen PA.
 *
 * @details
 *   Each PA has distinct maximum power, minimum power, and power step sizes.
 *   This API allows direct access to these power steps to tune between the
 *   maximum and minimum output power the selected PA is capable of.
 *
 * @param[in] pwrLevel
 *   Output power level.  Note that the maximum power level will change
 *   depending on PA selection.
 *
 * @return
 *   Power level set in the current PA.
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
RAIL_TxPowerLevel_t PA_SetPowerLevel(RAIL_TxPowerLevel_t pwrLevel);

/** @} (end addtogroup EFR32xG1x_PA_Advanced) */
/** @} (end addtogroup EFR32xG1x_PA) */
/** @} (end addtogroup Chip_Specific) */

/**
 * Non RAIL functions.
 *
 * The following functions can only be safely used by customers not yet on the
 * RAIL platform yet. For those on RAIL, please see RAIL_ConvertDbmToRaw and
 * RAIL_ConvertRawToDbm.
 */

/**
 * Function used to convert deci-dBm values to raw values that can be used in
 * SetTxPower.
 *
 * @param[in] power deci-dBm value that should be converted to the appropriate
 * raw power for the current PA
 * @return equivalent raw power for the deci-dBm value supplied, for the active
 * PA.
 */
RAIL_TxPowerLevel_t PA_ConvertDbmToRaw(RAIL_TxPower_t power);

/**
 * Function to convert the raw power levels returned from GetTxPower to
 * the equivalent deci-dBm value for the current PA.
 *
 * @param[in] powerLevel Raw PA power level that should be converted into
 * the equivalent deci-dBm value for the active PA
 * @return equivalent deci-dBm value for the raw power level passed in.
 */
RAIL_TxPower_t PA_ConvertRawToDbm(RAIL_TxPowerLevel_t powerLevel);

#ifdef __cplusplus
}
#endif

#endif /* __RADIO_PA_H */
