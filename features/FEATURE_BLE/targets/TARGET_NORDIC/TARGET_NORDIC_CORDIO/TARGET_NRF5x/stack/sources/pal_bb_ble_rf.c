/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE RF path compensation implementation file.
 *
 *  Copyright (c) 2016-2019 Arm Ltd. All Rights Reserved.
 *  Arm Ltd. confidential and proprietary.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "stack/platform/include/pal_types.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Minimum RF path compensation (in 0.1dBm units). */
#define BB_BLE_MIN_PATH_COMP        -1280

/*! \brief      Minimum RF path compensation (in 1dBm units). */
#define BB_BLE_MIN_PATH_COMP_DBM    -128

/*! \brief      Maximum RF path compensation (in 0.1dBm units). */
#define BB_BLE_MAX_PATH_COMP        1280

/*! \brief      Binary divide with 10 divisor (n[max]=0xFFFFFFFF). */
#define BB_BLE_MATH_DIV_10(n)       ((uint32_t)(((uint64_t)(n) * UINT64_C(419431)) >> 22))

/**************************************************************************************************
  Constants
**************************************************************************************************/

#if defined(NRF52840_XXAA) || defined(NRF52832_XXAA)
/* \brief   Minimum Tx power level (expressed in 1dBm units). */
static const int16_t bbBleMinTxPwr = -40;  /* -40dBm */
#else
/* \brief   Minimum Tx power level (expressed in 1dBm units). */
static const int16_t bbBleMinTxPwr = -30;  /* -30dBm */
#endif

#if defined(NRF52840_XXAA)
/* \brief   Maximum Tx power level (expressed in 1dBm units). */
static const int16_t bbBleMaxTxPwr = 9;    /*  +9dBm */
#else
/* \brief   Maximum Tx power level (expressed in 1dBm units). */
static const int16_t bbBleMaxTxPwr = 4;    /*  +4dBm */
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Current Tx path compensation value. */
int16_t bbBleTxPathComp = -1280;

/*! \brief      Current Rx path compensation value. */
int16_t bbBleRxPathComp = -1280;

/*************************************************************************************************/
/*!
 *  \brief      Get transmit RF path compensation.
 *
 *  \return     Transmit RF path compensation (in 1-dBm units).
 */
/*************************************************************************************************/
int8_t palBbBleRfGetTxRfPathComp(void)
{
  uint16_t pathCompUnsigned = (uint16_t)(bbBleTxPathComp - BB_BLE_MIN_PATH_COMP);

  return (int16_t)BB_BLE_MATH_DIV_10(pathCompUnsigned) + BB_BLE_MIN_PATH_COMP_DBM;
}

/*************************************************************************************************/
/*!
 *  \brief      Get receive RF path compensation.
 *
 *  \return     Transmit RF path compensation (in 1-dBm units).
 */
/*************************************************************************************************/
int8_t PalRadioGetRxRfPathComp(void)
{
  uint16_t pathCompUnsigned = (uint16_t)(bbBleRxPathComp - BB_BLE_MIN_PATH_COMP);

  return (int16_t)BB_BLE_MATH_DIV_10(pathCompUnsigned) + BB_BLE_MIN_PATH_COMP_DBM;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize RF path compensation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalRadioInitPathComp(void)
{
  bbBleTxPathComp = 0;
  bbBleRxPathComp = 0;
}

/*************************************************************************************************/
/*!
 *  \brief      Get supported transmit power.
 *
 *  \param      pMinTxPwr   Return buffer for minimum transmit power (expressed in 1dBm units).
 *  \param      pMaxTxPwr   Return buffer for maximum transmit power (expressed in 1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalRadioGetSupTxPower(int8_t *pMinTxPwr, int8_t *pMaxTxPwr)
{
  *pMinTxPwr = bbBleMinTxPwr;
  *pMaxTxPwr = bbBleMaxTxPwr;
}

/*************************************************************************************************/
/*!
 *  \brief      Get RF path compensation.
 *
 *  \param      pTxPathComp Return buffer for RF transmit path compensation value (expressed in 0.1dBm units).
 *  \param      pRxPathComp Return buffer for RF receive path compensation value (expressed in 0.1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalRadioReadRfPathComp(int16_t *pTxPathComp, int16_t *pRxPathComp)
{
  *pTxPathComp = bbBleTxPathComp;
  *pRxPathComp = bbBleRxPathComp;
}

/*************************************************************************************************/
/*!
 *  \brief      Set RF path compensation.
 *
 *  \param      txPathComp      RF transmit path compensation value (expressed in 0.1dBm units).
 *  \param      rxPathComp      RF receive path compensation value (expressed in 0.1dBm units).
 *
 *  \return     TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t PalRadioWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp)
{
  bbBleTxPathComp = txPathComp;
  bbBleRxPathComp = rxPathComp;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  \param      txPwr           Tx power provided by the host (expressed in 1dBm units).
 *  \param      compFlag        Flag to apply Tx path compensation or not.
 *
 *  \return     Actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  Tx path compensation is only used for extended ADV header.
 *  Compensation is not considered when filling in HCI events.
 */
/*************************************************************************************************/
int8_t PalRadioGetActualTxPower(int8_t txPwr, bool_t compFlag)
{
#if defined(NRF52840_XXAA)
  if      (txPwr >=   9)  {  txPwr =   9;  }
  else if (txPwr >=   8)  {  txPwr =   8;  }
  else if (txPwr >=   7)  {  txPwr =   7;  }
  else if (txPwr >=   6)  {  txPwr =   6;  }
  else if (txPwr >=   5)  {  txPwr =   5;  }
  else if (txPwr >=   4)  {  txPwr =   4;  }
  else if (txPwr >=   3)  {  txPwr =   3;  }
  else if (txPwr >=   2)  {  txPwr =   2;  }
  else if (txPwr >=   0)  {  txPwr =   0;  }
  else if (txPwr >=  -4)  {  txPwr =  -4;  }
  else if (txPwr >=  -8)  {  txPwr =  -8;  }
  else if (txPwr >= -12)  {  txPwr = -12;  }
  else if (txPwr >= -16)  {  txPwr = -16;  }
  else if (txPwr >= -20)  {  txPwr = -20;  }
  else                    {  txPwr = -40;  }
#elif defined(NRF52832_XXAA)
  if      (txPwr >=   4)  {  txPwr =   4;  }
  else if (txPwr >=   3)  {  txPwr =   3;  }
  else if (txPwr >=   0)  {  txPwr =   0;  }
  else if (txPwr >=  -4)  {  txPwr =  -4;  }
  else if (txPwr >=  -8)  {  txPwr =  -8;  }
  else if (txPwr >= -12)  {  txPwr = -12;  }
  else if (txPwr >= -16)  {  txPwr = -16;  }
  else if (txPwr >= -20)  {  txPwr = -20;  }
  else                    {  txPwr = -40;  }
#else
  if      (txPwr >=   4)  {  txPwr =   4;  }
  else if (txPwr >=   0)  {  txPwr =   0;  }
  else if (txPwr >=  -4)  {  txPwr =  -4;  }
  else if (txPwr >=  -8)  {  txPwr =  -8;  }
  else if (txPwr >= -12)  {  txPwr = -12;  }
  else if (txPwr >= -16)  {  txPwr = -16;  }
  else if (txPwr >= -20)  {  txPwr = -20;  }
  else                    {  txPwr = -30;  }
#endif

  if (compFlag)
  {
    txPwr += palBbBleRfGetTxRfPathComp();
  }

  return txPwr;
}
