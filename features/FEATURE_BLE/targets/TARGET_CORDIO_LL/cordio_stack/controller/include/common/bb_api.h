/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Baseband interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  \addtogroup BB_API              Baseband (BB) API
 *  \{
 *
 *  The baseband porting layer is a protocol independent BB + radio abstraction layer. It allows
 *  the simultaneous operation of protocol specific schedulers to transport packets across each
 *  medium via a single multi-protocol baseband. This interface describes operations for the
 *  following protocols:
 *
 *    - Bluetooth low energy: advertising and connections
 *    - ZigBee/802.15.4 (TBD)
 *    - BAN/802.15.6 (TBD)
 *
 *  \dot
 *  digraph
 *  {
 *    node [shape=record, width=1.0, height=0.5, fontname=Helvetica, fontsize=8];
 *
 *    ble    [label="BLE\nlink layer", style=filled, fillcolor=lightgray];
 *    zigbee [label="802.15.4\nMAC", style=filled, fillcolor=lightgray];
 *    ban    [label="802.15.6\nMAC", style=filled, fillcolor=lightgray];
 *    sch    [label="Multi-protocol\nscheduler", style=filled, fillcolor=lightgray];
 *    bb_drv [label="{ BB porting layer | BB specific driver }"];
 *    bb_hw  [label="BB hardware", style=filled, fillcolor=dimgray];
 *
 *    ble -> sch;
 *    ble -> bb_drv;
 *    zigbee -> sch;
 *    zigbee -> bb_drv;
 *    ban -> sch;
 *    ban -> bb_drv;
 *    sch -> bb_drv;
 *    bb_drv -> bb_hw [style=dashed];
 *  }
 *  \enddot
 *
 *  \defgroup BB_API_INIT           Initialization
 *  \defgroup BB_API_BOD            Baseband Operation Descriptors (BOD)
 *  \defgroup BB_API_CTRL           Control interface
 *  \defgroup BB_API_BLE            Bluetooth LE Protocol
 *
 *  \}
 */
/*************************************************************************************************/

#ifndef BB_API_H
#define BB_API_H

#include "wsf_types.h"
#include "cfg_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \addtogroup BB_API_BOD
 *  \{ */

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

/*! \} */    /* BB_API_BOD */

/**************************************************************************************************
  Macros
**************************************************************************************************/

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

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \addtogroup BB_API_INIT
 *  \{ */

/*! \brief      Typical maximum duration to scan in a scan interval (BbRtCfg_t::maxScanPeriodMs). */
#define BB_MAX_SCAN_PERIOD_MS       1000

/*! \brief      Typical RF setup delay (BbRtCfg_t::rfSetupDelayUs). */
#define BB_RF_SETUP_DELAY_US        150

/*! \brief      Typical operation setup delay in microseconds (BbRtCfg_t::schSetupDelayUs). */
#define BB_SCH_SETUP_DELAY_US       500


/*! \brief      BB runtime configuration parameters. */
typedef struct
{
  /* Common */
  uint16_t  clkPpm;                 /*!< Clock accuracy in PPM. */
  uint8_t   rfSetupDelayUs;         /*!< RF setup delay in microseconds. */

  /* BLE */
  uint16_t  maxScanPeriodMs;        /*!< Maximum duration to scan in a scan interval. */

  /* Scheduler */
  uint16_t  schSetupDelayUs;        /*!< Operation setup delay in microseconds. */
} BbRtCfg_t;

/*! \} */    /* BB_API_INIT */

/*! \addtogroup BB_API_BOD
 *  \{ */

/* Forward definition of BOD. */
struct BbOpDesc_tag;

/*! \brief      BOD completion event callback signature. */
typedef void (*BbBodCompCback_t)(void);

/*! \brief      BOD event callback signature. */
typedef void (*BbBodCback_t)(struct BbOpDesc_tag *pBod);

/*! \brief     Protocol event callback signature. */
typedef void (*BbProtCback_t)(void);

/*! \brief      BOD rescheduling policy (listed in priority order). */
typedef enum
{
  BB_RESCH_FIXED_PREFERRED,         /*!< BOD is fixed (rescheduling has limited recoverable consequences). */
  BB_RESCH_FIXED,                   /*!< BOD is fixed (rescheduling has recoverable consequences). */
  BB_RESCH_PERIODIC,                /*!< BOD is periodic (rescheduling has consequences). */
  BB_RESCH_MOVEABLE_PREFERRED,      /*!< BOD is movable (rescheduling has minor consequences). */
  BB_RESCH_MOVEABLE,                /*!< BOD is movable (rescheduling has no consequences). */
  BB_RESCH_BACKGROUND               /*!< BOD is single background task. */
} BbReschPol_t;

/*! \} */    /* BB_API_BOD */

/*! \addtogroup BB_API_BOD
 *  \{ */

/* Forward protocol data definitions. */
struct BbBleData_tag;
struct Bb154Data_tag;

/*! \brief      Baseband operation descriptor (BOD). */
typedef struct BbOpDesc_tag
{
  struct BbOpDesc_tag *pPrev;       /*!< Previous BOD. */
  struct BbOpDesc_tag *pNext;       /*!< Next BOD. */

  uint32_t      due;                /*!< Absolute clock due time. */
  uint32_t      minDurUsec;         /*!< Minimum required duration in microseconds. */
  uint32_t      maxDurUsec;         /*!< Maximum required duration in microseconds. */
  uint16_t      dueOffsetUsec;      /*!< Due time offset in microseconds. */

  uint8_t       reschPolicy;        /*!< Rescheduling policy. */

  uint8_t       protId;             /*!< Protocol type. */

  BbBodCback_t  endCback;           /*!< End of BOD callback (when BOD ends). */
  BbBodCback_t  abortCback;         /*!< Abort BOD callback (when BOD is removed before beginning). */

  void          *pCtx;              /*!< Client defined context. */

  union
  {
    struct BbBleData_tag *pBle;     /*!< BLE operation data. */
    struct Bb154Data_tag *p154;     /*!< 802.15.4 operation data. */
  } prot;                           /*!< Protocol specific data. */
} BbOpDesc_t;

/*! \} */    /* BB_API_BOD */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup BB_API_INIT
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Initialize runtime configuration.
 *
 *  \param      pCfg        Pointer to runtime configuration parameters (data must be static).
 *
 *  \return     None.
 *
 *  This function initializes the BB subsystem's runtime configuration.
 *
 *  \note       This routine must be called only once before any other initialization routine.
 */
/*************************************************************************************************/
void BbInitRunTimeCfg(const BbRtCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BB.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Register operation completion handler.
 *
 *  \param      eventCback  Event callback.
 *
 *  \return     None.
 *
 *  Register operation completion handler.
 */
/*************************************************************************************************/
void BbRegister(BbBodCompCback_t eventCback);

/*************************************************************************************************/
/*!
 *  \brief      Register protocol handlers.
 *
 *  \param      protId          Protocol ID.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 *  \param      startProtCback  Start protocol callback.
 *  \param      stopProtCback   Stop protocol callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbRegisterProt(uint8_t protId, BbBodCback_t execOpCback, BbBodCback_t cancelOpCback,
    BbProtCback_t startProtCback, BbProtCback_t stopProtCback);

/*! \} */    /* BB_API_INIT */

/*! \addtogroup BB_API_CTRL
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 *
 *  Enable BB and start processing the list of BODs.  This routine may be called several times, if
 *  a protocol layers has several simultaneously-enabled operations.  However, \ref BbStop() must
 *  be called an equal number of time to disable the baseband.
 */
/*************************************************************************************************/
void BbStart(uint8_t protId);

/*************************************************************************************************/
/*!
 *  \brief      Stop BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  Disable BB processing of BODs.
 *
 *  \note       For any particular protocol, calls to \ref BbStart() and \ref BbStop() must be
 *              balanced to ensure that the hardware is disabled if and only if appropriate.
 */
/*************************************************************************************************/
void BbStop(uint8_t protId);

/*************************************************************************************************/
/*!
 *  \brief      Execute BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *
 *  \return     None.
 *
 *  Execute the protocol specific BOD handler.
 */
/*************************************************************************************************/
void BbExecuteBod(BbOpDesc_t *pBod);

/*************************************************************************************************/
/*!
 *  \brief      Cancel current executing BOD.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbCancelBod(void);

/*************************************************************************************************/
/*!
 *  \brief      Get the currently-executing BOD.
 *
 *  \return     Currently-executing BOD.
 */
/*************************************************************************************************/
BbOpDesc_t *BbGetCurrentBod(void);

/*************************************************************************************************/
/*!
 *  \brief      Cancel current executing BOD.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD, typically in the related ISRs.
 */
/*************************************************************************************************/
void BbSetBodTerminateFlag(void);

/*************************************************************************************************/
/*!
 *  \brief      Get termination state of current executing BOD.
 *
 *  \return     TRUE if termination flag set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbGetBodTerminateFlag(void);

/*************************************************************************************************/
/*!
 *  \brief      Terminate a BOD immediately.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD, typically in the related ISRs.
 */
/*************************************************************************************************/
void BbTerminateBod(void);

/*************************************************************************************************/
/*!
 *  \brief      Get BB clock accuracy.
 *
 *  \return     Clock accuracy in part per million.
 *
 *  Returns the current BB clock accuracy.
 */
/*************************************************************************************************/
uint16_t BbGetClockAccuracy(void);

/*************************************************************************************************/
/*!
 *  \brief      Get scheduler setup delay.
 *
 *  \return     Scheduler setup delay in microseconds.
 *
 *  Returns the scheduler setup delay.
 */
/*************************************************************************************************/
uint16_t BbGetSchSetupDelayUs(void);

/*************************************************************************************************/
/*!
 *  \brief      Returns the ID of the active protocol.
 *
 *  \return     Protocol operation in progress.
 */
/*************************************************************************************************/
uint8_t BbGetActiveProtocol(void);

/*! \} */    /* BB_API_CTRL */

#ifdef __cplusplus
};
#endif

#endif /* BB_API_H */
