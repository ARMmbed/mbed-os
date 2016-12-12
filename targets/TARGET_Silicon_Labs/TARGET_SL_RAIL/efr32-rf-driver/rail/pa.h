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
 * @addtogroup RF_Library
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup PA
 * @{
 ******************************************************************************/

/*******************************************************************************
 ****************************   CONFIGURATION   ********************************
 ******************************************************************************/
#define PA_SCALING_FACTOR     10

/** 
 * @struct RADIO_PASel_t
 * @brief Selection of the rf power amplifier (PA) to use
 */
typedef enum RADIO_PASel
{
    /** High power PA */
    PA_SEL_2P4_HP,
    /** Low power PA */
    PA_SEL_2P4_LP,
    /** SubGig PA*/
    PA_SEL_SUBGIG
} RADIO_PASel_t;

typedef enum RADIO_PAVoltMode
{
    /** Vpa = Vbat = 3.3V */
    PA_VOLTMODE_VBAT,
    /** Vpa = DCDC Vout = 1.8V */
    PA_VOLTMODE_DCDC
} RADIO_PAVoltMode_t;

/** 
 * @struct RADIO_PAInit_t
 * @brief Configuration structure for the rf power amplifier (PA)
 */
typedef struct RADIO_PAInit {
  /** Power Amplifier mode */
  RADIO_PASel_t paSel;
  /** Power Amplifier vPA Voltage mode */
  RADIO_PAVoltMode_t voltMode;
  /** Desired output power in dBm * 10 */
  int16_t power;
  /** Output power offset in dBm * 10 */
  int16_t offset;
  /** Desired ramp time in us */
  uint16_t rampTime;
} RADIO_PAInit_t;

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/

bool     RADIO_PA_Init(RADIO_PAInit_t * paInit);
int32_t  PA_OutputPowerGet(void);
int32_t  PA_OutputPowerSet(int32_t power);
int32_t  PA_MaxOutputPowerSet(void);
uint32_t PA_RampTimeGet(void);
uint32_t PA_RampTimeSet(uint32_t ramptime);
void     PA_CTuneSet(uint8_t txPaCtuneValue, uint8_t rxPaCtuneValue);

/** @} (end addtogroup PA) */
/** @} (end addtogroup RF_Library) */

#ifdef __cplusplus
}
#endif


#endif /* __RADIO_PA_H */
