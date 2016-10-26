/*************************************************************************************************/
/*!
 *  \file   wsf_mbed_os.h
 *
 *  \brief  Mbed OS specific callback.
 *
 *  Copyright (c) 2016 ARM, Ltd., all rights reserved.
 *  ARM confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef WSF_MBED_OS_H
#define WSF_MBED_OS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Register MBED OS SignalEvent */
void mbedOSRegisterSignalEventCallback(uint32_t (*fnSignalEventInput)(void));

/*
 * Call Mbed OS SignalEvent
 * This function is called to signal to the user code when a wsf task is ready
 */
void mbedOSSignalEventCallback(void);

#ifdef __cplusplus
};
#endif

#endif /* WSF_MBED_OS_H */
