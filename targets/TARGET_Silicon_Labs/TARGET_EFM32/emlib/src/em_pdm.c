/***************************************************************************//**
 * @file
 * @brief Pulse Density Modulation (PDM) peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_pdm.h"
#if defined(PDM_PRESENT) && (PDM_COUNT == 1)

#include "em_assert.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup PDM
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   De-initialize the PDM peripheral.
 *
 * @details
 *   This function will stop the PDM filter and set PDM control registers to
 *   their reset values.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 ******************************************************************************/
void PDM_DeInit(PDM_TypeDef *pdm)
{
  PDM_Stop(pdm);
  PDM_Clear(pdm);
  PDM_FifoFlush(pdm);
  PDM_Reset(pdm);
}

/***************************************************************************//**
 * @brief
 *   Initialize the PDM peripheral.
 *
 * @details
 *   This function will configure basic settings in PDM according to values
 *   in the initialization data structure.
 *
 *   Notice that enabling of PDM clock, setup of PDM pins and setup of PRS
 *   is not covered by this function.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @param[in] init
 *   A pointer to the initialization structure used to configure the PDM.
 ******************************************************************************/
void PDM_Init(PDM_TypeDef *pdm, const PDM_Init_TypeDef *init)
{
  EFM_ASSERT(init->dsr <= (_PDM_CTRL_DSR_MASK >> _PDM_CTRL_DSR_SHIFT));
  EFM_ASSERT(init->gain <= (_PDM_CTRL_GAIN_MASK >> _PDM_CTRL_GAIN_SHIFT));
  EFM_ASSERT(init->prescaler <= (_PDM_CFG1_PRESC_MASK >> _PDM_CFG1_PRESC_SHIFT));

  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }

  pdm->EN = PDM_EN_EN_DISABLE;

#if defined(PDM_CFG0_NUMCH_THREE)
  pdm->CFG0 = ((uint32_t)init->ch3ClkPolarity       << _PDM_CFG0_CH3CLKPOL_SHIFT)
              | ((uint32_t)init->ch2ClkPolarity     << _PDM_CFG0_CH2CLKPOL_SHIFT)
              | ((uint32_t)init->ch1ClkPolarity     << _PDM_CFG0_CH1CLKPOL_SHIFT)
              | ((uint32_t)init->ch0ClkPolarity     << _PDM_CFG0_CH0CLKPOL_SHIFT)
              | ((uint32_t)init->fifoValidWatermark << _PDM_CFG0_FIFODVL_SHIFT)
              | ((uint32_t)init->dataFormat         << _PDM_CFG0_DATAFORMAT_SHIFT)
              | ((uint32_t)init->numChannels        << _PDM_CFG0_NUMCH_SHIFT)
              | ((uint32_t)init->filterOrder        << _PDM_CFG0_FORDER_SHIFT)
              | (init->enableCh2Ch3Stereo ? PDM_CFG0_STEREOMODECH23_CH23ENABLE : 0U)
              | (init->enableCh0Ch1Stereo ? PDM_CFG0_STEREOMODECH01_CH01ENABLE : 0U);
#else
  pdm->CFG0 = ((uint32_t)init->ch1ClkPolarity       << _PDM_CFG0_CH1CLKPOL_SHIFT)
              | ((uint32_t)init->ch0ClkPolarity     << _PDM_CFG0_CH0CLKPOL_SHIFT)
              | ((uint32_t)init->fifoValidWatermark << _PDM_CFG0_FIFODVL_SHIFT)
              | ((uint32_t)init->dataFormat         << _PDM_CFG0_DATAFORMAT_SHIFT)
              | ((uint32_t)init->numChannels        << _PDM_CFG0_NUMCH_SHIFT)
              | ((uint32_t)init->filterOrder        << _PDM_CFG0_FORDER_SHIFT)
              | (init->enableCh0Ch1Stereo ? PDM_CFG0_STEREOMODECH01_CH01ENABLE : 0U);
#endif

  pdm->CFG1 = init->prescaler << _PDM_CFG1_PRESC_SHIFT;

  pdm->EN = PDM_EN_EN_ENABLE;

  pdm->CTRL = (init->dsr    << _PDM_CTRL_DSR_SHIFT)
#if defined(PDM_CTRL_OUTCLKEN)
              | (init->outClkEn ? PDM_CTRL_OUTCLKEN : 0U)
#endif
              | (init->gain << _PDM_CTRL_GAIN_SHIFT);

  if (init->start) {
    PDM_Clear(pdm);
    PDM_FifoFlush(pdm);
    PDM_Start(pdm);
  }
}

/***************************************************************************//**
 * @brief
 *   Initialize PDM registers with reset values.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 ******************************************************************************/
void PDM_Reset(PDM_TypeDef *pdm)
{
  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }

  if (pdm->EN != 0U) {
    pdm->CMD = PDM_CMD_FIFOFL | PDM_CMD_CLEAR | PDM_CMD_STOP;
    while (pdm->SYNCBUSY != 0U) {
    }
  }

#if defined(PDM_HAS_SET_CLEAR)
  pdm->EN_SET    = PDM_EN_EN;
#endif
  pdm->CTRL      = _PDM_CTRL_RESETVALUE;
  pdm->IEN       = _PDM_IEN_RESETVALUE;
#if defined(PDM_HAS_SET_CLEAR)
  pdm->IF_CLR    = _PDM_IF_MASK;
#else
  pdm->IFC       = _PDM_IFC_MASK;
#endif
#if defined(_PDM_ROUTEPEN_MASK)
  pdm->ROUTEPEN  = _PDM_ROUTEPEN_RESETVALUE;
  pdm->ROUTELOC0 = _PDM_ROUTELOC0_RESETVALUE;
  pdm->ROUTELOC1 = _PDM_ROUTELOC1_RESETVALUE;
#endif
  while (pdm->SYNCBUSY != 0U) {
    /* Must wait for SYNCBUSY before disabling an enabled pdm. */
  }
  pdm->EN        = _PDM_EN_RESETVALUE;
  pdm->CFG0      = _PDM_CFG0_RESETVALUE;
  pdm->CFG1      = _PDM_CFG1_RESETVALUE;

  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }
}

/** @} (end addtogroup PDM) */
/** @} (end addtogroup emlib) */

#endif // defined(PDM_PRESENT) && (PDM_COUNT == 1)
