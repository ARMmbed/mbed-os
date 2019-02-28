/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      MAC system configuration.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef CFG_MAC_H
#define CFG_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Baseband ***/

#ifndef BB_CLK_RATE_HZ
#define BB_CLK_RATE_HZ          1000000 /*!< BB clock rate in hertz. */
#endif

/*** Controller HCI ***/

#ifndef CHCI_BUF_TAILROOM
#define CHCI_BUF_TAILROOM       4       /*!< Extra byte allocation required for buffer (e.g., for MIC). */
#endif

#ifdef __cplusplus
};
#endif

#endif /* CFG_MAC_H */
