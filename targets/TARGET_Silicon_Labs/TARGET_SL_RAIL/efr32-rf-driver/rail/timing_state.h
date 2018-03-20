/***************************************************************************//**
 * @file timing_state.h
 * @brief RADIO API Abstraction layer for setting state timing information
 * @version INTERNAL
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://silabs.com</b>
 ******************************************************************************/
#ifndef __TIMING_STATE_H
#define __TIMING_STATE_H

#include <stdint.h>
#include "em_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

// All the timing information for state transitions
typedef struct StateTimings {
  int32_t rxChainDelayNs;
#if (_SILICON_LABS_32B_SERIES_1_CONFIG == 1) \
  || (_SILICON_LABS_32B_SERIES_1_CONFIG == 4)
  // On Dumbo the RX2TX time is based on the RXDONE time, which is stable enough
  // for our purposes. However, the timestamp is still based on FRAMEDET, so we
  // need two delays, one for the chain delay (FRAMEDET delay), and the second
  // for the RXDONE delay, which we use for RX2TX transitions only.

  // On Nixi the RX2TX time and the timestamp are based on RXDONE. The chain
  // delay for RX only calculates how long we need to stay in RX to get SYNC,
  // which does not need to be precise. (And hard to measure.)
  int32_t rxDoneDelayNs;
#endif
  int32_t txChainDelayNs;
  uint16_t rxSearch;
  uint16_t txToRxSearch;
  uint16_t rxWarm;
  uint16_t rxToTx;
  uint16_t rxFrameToTx;
  uint16_t txWarm;
  uint16_t txToRx;
  uint16_t txToTx;
} StateTimings_t;

/** @} (end addtogroup TIMING) */
/** @} (end addtogroup RF_Library) */

#ifdef __cplusplus
}
#endif

#endif /* __TIMING_STATE_H */
