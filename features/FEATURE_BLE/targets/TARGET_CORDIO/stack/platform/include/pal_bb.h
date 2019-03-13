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
 *  \brief Baseband interface file.
 */
/*************************************************************************************************/

#ifndef PAL_BB_H
#define PAL_BB_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Protocol types */
enum
{
  BB_PROT_NONE,                         /*!< Non-protocol specific operation. */
  BB_PROT_BLE,                          /*!< Bluetooth Low Energy normal mode. */
  BB_PROT_BLE_DTM,                      /*!< Bluetooth Low Energy direct test mode. */
  BB_PROT_PRBS15,                       /*!< Enable the continuous PRBS15 transmit sequence. */
  BB_PROT_15P4,                         /*!< 802.15.4. */
  BB_PROT_NUM                           /*!< Number of protocols. */
};

/*! \brief      Status codes */
enum
{
  BB_STATUS_SUCCESS,                    /*!< Operation successful. */
  BB_STATUS_FAILED,                     /*!< General failure. */
  BB_STATUS_CANCELED,                   /*!< Receive canceled. */
  BB_STATUS_RX_TIMEOUT,                 /*!< Receive packet timeout. */
  BB_STATUS_CRC_FAILED,                 /*!< Receive packet with CRC verification failed. */
  BB_STATUS_FRAME_FAILED,               /*!< Receive packet with frame verification failed. */
  BB_STATUS_ACK_FAILED,                 /*!< ACK packet failure. */
  BB_STATUS_ACK_TIMEOUT,                /*!< ACK packet timeout. */
  BB_STATUS_TX_CCA_FAILED,              /*!< Transmit CCA failure. */
  BB_STATUS_TX_FAILED                   /*!< Transmit failure. */
};

/*! \brief      PHY types. */
enum
{
  BB_PHY_BLE_1M    = 1,                 /*!< Bluetooth Low Energy 1Mbps PHY. */
  BB_PHY_BLE_2M    = 2,                 /*!< Bluetooth Low Energy 2Mbps PHY. */
  BB_PHY_BLE_CODED = 3,                 /*!< Bluetooth Low Energy Coded PHY (data coding unspecified). */
  BB_PHY_15P4      = 4,                 /*!< 802.15.4 PHY. */
};

/*! \brief      PHY options. */
enum
{
  BB_PHY_OPTIONS_DEFAULT          = 0,  /*!< BB defined PHY Options behavior. */
  BB_PHY_OPTIONS_BLE_S2           = 1,  /*!< Always use S=2 coding when transmitting on LE Coded PHY. */
  BB_PHY_OPTIONS_BLE_S8           = 2   /*!< Always use S=8 coding when transmitting on LE Coded PHY. */
};

#ifndef BB_CLK_RATE_HZ
/*! \brief      BB clock rate in hertz. */
#define BB_CLK_RATE_HZ          1000000
#endif

/*! \brief      Binary divide with 1,000,000 divisor (n[max]=0xFFFFFFFF). */
#define BB_MATH_DIV_10E6(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(4295)) >> 32))

#if (BB_CLK_RATE_HZ == 1000000)
/*! \brief      Return microseconds (no conversion required). */
#define BB_US_TO_BB_TICKS(us)       (us)
#elif (BB_CLK_RATE_HZ == 8000000)
/*! \brief      Compute BB ticks from given time in microseconds (max time is interval=1,996s). */
#define BB_US_TO_BB_TICKS(us)       ((uint32_t)((us) << 3))
#elif (BB_CLK_RATE_HZ == 32768)
/*! \brief      Compute BB ticks from given time in microseconds (max time is interval=1,996s). */
#define BB_US_TO_BB_TICKS(us)       ((uint32_t)(((uint64_t)(us) * UINT64_C(549755)) >> 24))   /* calculated value may be one tick low */
#else
/*! \brief      Compute BB ticks from given time in microseconds (max time is interval=1,996s). */
#define BB_US_TO_BB_TICKS(us)       BB_MATH_DIV_10E6((uint64_t)(us) * (uint64_t)(BB_CLK_RATE_HZ))
#endif

#define RTC_CLOCK_RATE              32768
#define USE_RTC_BB_CLK              (BB_CLK_RATE_HZ == RTC_CLOCK_RATE)
#define HFCLK_OSC_SETTLE_TICKS      (RTC_CLOCK_RATE / 1000)

#if (BB_CLK_RATE_HZ == 1000000)
/*! \brief      BB ticks to microseconds (no conversion required). */
#define BB_TICKS_TO_US(n)           (n)
#elif (BB_CLK_RATE_HZ == 8000000)
/*! \brief      BB ticks to microseconds (8MHz). */
#define BB_TICKS_TO_US(n)           ((n) >> 3)
#elif (BB_CLK_RATE_HZ == 32768)
/*! \brief      BB ticks to microseconds (32768 Hz). */
#define BB_TICKS_TO_US(n)           (uint32_t)(((uint64_t)(n) * 15625) >> 9)
#else
/*! \brief      BB ticks to microseconds. */
#define BB_TICKS_TO_US(n)           (uint32_t)((uint64_t)(n) * 1000000 / BB_CLK_RATE_HZ)
#endif

/*! \brief      Typical maximum duration to scan in a scan interval (BbRtCfg_t::maxScanPeriodMs). */
#define BB_MAX_SCAN_PERIOD_MS       1000

/*! \brief      Typical RF setup delay (BbRtCfg_t::rfSetupDelayUs). */
#define BB_RF_SETUP_DELAY_US        150

/*! \brief      Typical operation setup delay in microseconds (BbRtCfg_t::schSetupDelayUs). */
#define BB_SCH_SETUP_DELAY_US       500

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  IRQ callback datatypes. */
typedef void (*bbDrvIrqCback_t)(void);

/*! \brief  BB configuration. */
typedef struct
{
  uint16_t clkPpm;                  /*!< Clock accuracy in PPM. */
  uint8_t  rfSetupDelayUsec;        /*!< RF setup delay in microseconds. */
  uint16_t maxScanPeriodMsec;       /*!< Maximum scan period in milliseconds. */
  uint16_t schSetupDelayUsec;       /*!< Schedule setup delay in microseconds. */
} PalBbCfg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup PAL_BB_INIT
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
void PalBbInit(void);

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
void PalBbEnable(void);

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
void PalBbDisable(void);

/*************************************************************************************************/
/*!
 *  \brief      Load BB timing configuration.
 *
 *  \param      pCfg                Return configuration values.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalBbLoadCfg(PalBbCfg_t *pCfg);

/*! \} */    /* PAL_BB_INIT */

/*! \addtogroup PAL_BB_CLOCK
 *  \{
 *  This section contains driver routines related to the BB clock.
 */

/*************************************************************************************************/
/*!
 *  \brief      Get the current BB clock value.
 *
 *  \param      useRtcBBClk   Use RTC BB clock.
 *
 *  \return     Current BB clock value, units are microseconds.
 *
 *  This routine reads the current value from the BB clock and returns its value.  The clock should
 *  increment at the rate BB_CLK_RATE_HZ (wrapping as appropriate) whenever the BB is enabled.
 */
/*************************************************************************************************/
uint32_t PalBbGetCurrentTime(bool_t useRtcBBClk);

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
bool_t PalBbGetTimestamp(uint32_t *pTime);

/*************************************************************************************************/
/*!
 *  \brief      Called to register a protocol's Radio and Timer IRQ callback functions.
 *
 *  \param      protId      Protocol ID.
 *  \param      timerCback  Timer IRQ callback.
 *  \param      radioCback  Timer IRQ callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalBbRegisterProtIrq(uint8_t protId, bbDrvIrqCback_t timerCback, bbDrvIrqCback_t radioCback);

/*************************************************************************************************/
/*!
 *  \brief      Set protocol ID.
 *
 *  \param      protId      Protocol ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalBbSetProtId(uint8_t protId);

/*! \} */    /* PAL_BB_CLOCK */

#ifdef __cplusplus
};
#endif

#endif /* PAL_BB_H */
