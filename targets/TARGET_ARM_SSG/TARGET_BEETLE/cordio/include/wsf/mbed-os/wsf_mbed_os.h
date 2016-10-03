/*************************************************************************************************/
/*!
 *  \file   wsf_mbed_os.h
 *
 *  \brief  Mbed OS specific callback.
 *
 *          $Date: 2016-06-24 12:54:16 -0400 (Mon, 05 Oct 2015) $
 *          $Revision: 1 $
 *
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
