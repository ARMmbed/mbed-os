/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Baseband driver interface file.
 */
/*************************************************************************************************/

#ifndef BB_DRV_H
#define BB_DRV_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup BB_DRV_INIT
 *  \{
 *  This section contains driver routines which initialize as well as enable the sleep mode
 *  of the BB hardware.
 */

/*************************************************************************************************/
/*!
 *  \brief      Initialize the baseband driver.
 *
 *  \return     None.
 *
 *  One-time initialization of baseband resources. This routine can be used to setup baseband
 *  resources, load RF trim parameters and execute RF calibrations and seed the random number
 *  generator.
 *
 *  This routine should block until the BB hardware is completely initialized.
 */
/*************************************************************************************************/
void BbDrvInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Enable the BB hardware.
 *
 *  \return     None.
 *
 *  This routine brings the BB hardware out of low power (enable power and clocks) just before a
 *  first BB operation is executed.
 */
/*************************************************************************************************/
void BbDrvEnable(void);

/*************************************************************************************************/
/*!
 *  \brief      Disable the BB hardware.
 *
 *  \return     None.
 *
 *  This routine signals the BB hardware to go into low power (disable power and clocks) after all
 *  BB operations have been disabled.
 */
/*************************************************************************************************/
void BbDrvDisable(void);

/*************************************************************************************************/
/*!
 *  \brief      Get version codes.
 *
 *  \param      pBbVer      If non-NULL, return BB hardware version.
 *  \param      pPhyVer     If non-NULL, return PHY hardware version.
 *
 *  \return     None.
 *
 *  Interrogate the HW for version codes.
 */
/*************************************************************************************************/
void BbDrvGetVersions(uint32_t *pBbVer, uint32_t *pPhyVer);

/*! \} */    /* BB_DRV_INIT */

/*! \addtogroup BB_DRV_CLOCK
 *  \{
 *  This section contains driver routines related to the BB clock.
 */

/*************************************************************************************************/
/*!
 *  \brief      Get the current BB clock value.
 *
 *  \return     Current BB clock value.
 *
 *  This routine reads the current value from the BB clock and returns its value.  The clock should
 *  increment at the rate BB_CLK_RATE_HZ (wrapping as appropriate) whenever the BB is enabled.
 */
/*************************************************************************************************/
uint32_t BbDrvGetCurrentTime(void);

/*************************************************************************************************/
/*!
 *  \brief      Get the current FRC time.
 *
 *  \param      pTime   Pointer to return the current time.
 *
 *  \return     TRUE if time is valid, FALSE otherwise.
 *
 *  Get the current FRC time.
 *
 *  \note       FRC is limited to the same bit-width as the BB clock. Return value is available
 *              only when the BB is active.
 */
/*************************************************************************************************/
bool_t BbDrvGetTimestamp(uint32_t *pTime);

/*! \} */    /* BB_DRV_CLOCK */

#ifdef __cplusplus
};
#endif

#endif /* BB_DRV_H */
