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
 ****************************   CONFIGURATION   ********************************
 ******************************************************************************/
/** Scaling factor applied to all dBm power level inputs and outputs *   */
#define PA_SCALING_FACTOR     10

/** 
 * @brief Selection of the rf power amplifier (PA) to use
 */
typedef enum RADIO_PASel
{
    /** High power PA */
    PA_SEL_2P4_HP,
    /** Low power PA */
    PA_SEL_2P4_LP,
    /** SubGig PA*/
    PA_SEL_SUBGIG,
    /** Invalid PA Selection */
    PA_SEL_INVALID
} RADIO_PASel_t;

/**
 * @brief Selection should match the configuration of the voltage on the vPa pin
 *        of the chip.
 */
typedef enum RADIO_PAVoltMode
{
    /** Vpa = Vbat = 3.3V */
    PA_VOLTMODE_VBAT,
    /** Vpa = DCDC Vout = 1.8V */
    PA_VOLTMODE_DCDC
} RADIO_PAVoltMode_t;

/** 
 * @brief Configuration structure for the rf power amplifier (PA)
 */
typedef struct RADIO_PAInit {
  /** Power Amplifier mode */
  RADIO_PASel_t paSel;
  /** Power Amplifier vPA Voltage mode */
  RADIO_PAVoltMode_t voltMode;
  /** Desired output power in dBm * \ref PA_SCALING_FACTOR */
  int16_t power;
  /** Output power offset in dBm * \ref PA_SCALING_FACTOR */
  int16_t offset;
  /** Desired ramp time in us */
  uint16_t rampTime;
} RADIO_PAInit_t;

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/

/**
 * @brief
 *   Initilize the PA settings based on the settings provided in the paInit
 *   structure.
 *
 * @param[in] paInit
 *   Pointer to a structure containing the desired PA configuration settings.
 *
 * @return
 *   True if the settings were accepted.
 *   False if settings were invalid.
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
*/
bool RADIO_PA_Init(RADIO_PAInit_t * paInit);

/**
 * @brief
 *   Returns the current power level of transmit power
 *
 * @return
 *   Current power level in dBm * \ref PA_SCALING_FACTOR
 */
int32_t PA_OutputPowerGet(void);

/**
 * @brief
 *   Sets the output power of the PA.
 *
 *   Each PA has distinct maximum power, minimum power, and power step sizes.
 *   This API will calculate the best pa output power level setting to acheieve
 *   the desired output power.
 *
 * @note
 *   Board and chip variations will affect the accuracy of this API.  Use
 *   of the RADIO_PAInit_t.offset paramter can help account for this variation.
 *
 * @param[in] power
 *   Power value in dBm * \ref PA_SCALING_FACTOR
 *
 *   Examples with \ref PA_SCALING_FACTOR of 10:
 *     - 10  dBm --> 100
 *     - 5.5 dBm -->  55
 *
 * @return
 *   Returns the actual power that was set in dBm * \ref PA_SCALING_FACTOR
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
int32_t PA_OutputPowerSet(int32_t power);

/**
 * @brief
 *   Set the maximum possible output power for the selected PA.
 *
 * @return
 *   Returns the actual power that was set in dBm * \ref PA_SCALING_FACTOR
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
int32_t PA_MaxOutputPowerSet(void);

/**
 * @brief
 *   Return the current ramp time in microseconds
 *
 * @return
 *   Current ramp time in microseconds
 */
uint32_t PA_RampTimeGet(void);

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
uint32_t PA_RampTimeSet(uint32_t ramptime);

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
 * @note PACTUNE will reset to default values when RADIO_PA_Init() or
 *       RAIL_RadioConfig() are called.
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
void PA_CTuneSet(uint8_t txPaCtuneValue, uint8_t rxPaCtuneValue);

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
 * @param[in]  boostMode
 *   Output boost mode.  Some PA selections have a mode that will increase the
 *   output power for each step if this is enabled.
 *
 * @return
 *   MSB Configured boost mode. \n
 *   LSB Configured power level
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
uint16_t PA_PowerLevelSet(uint8_t pwrLevel, uint8_t boostMode);

/**
 * @brief
 *   Optimize the PA settings based on expected output power level.
 *
 * @details
 *   This API optimizes the current consumption of the radio based on the
 *   provided output power.  This is only necessary when output power is
 *   controlled by PA_PowerLevelSet().
 *
 * @param[in] power
 *   Power value in dBm * \ref PA_SCALING_FACTOR
 *
 * @warning
 *   The radio should not be transmitting when this function is called!
 */
void PA_PowerLevelOptimize(int32_t power);

/** @} (end addtogroup EFR32xG1x_PA_Advanced) */
/** @} (end addtogroup EFR32xG1x_PA) */
/** @} (end addtogroup Chip_Specific) */

#ifdef __cplusplus
}
#endif


#endif /* __RADIO_PA_H */
