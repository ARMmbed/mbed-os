/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
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
 *
 * Component   : RTSL
 * File        : cb_status.h
 *
 * Description : Common RTSL status codes 
 *-------------------------------------------------------------------------*/
#ifndef _CB_STATUS_H_
#define _CB_STATUS_H_

/*===========================================================================
 * DEFINES
 *=========================================================================*/

#define OK(status) (status == cbSTATUS_OK)
#define BUSY(status) (status == cbSTATUS_BUSY)
#define ERR(status) (status == cbSTATUS_ERROR)

/*===========================================================================
 * TYPES
 *=========================================================================*/

 typedef enum
 {
    cbSTATUS_OK,
    cbSTATUS_ERROR,
    cbSTATUS_BUSY,
    cbSTATUS_RECEIVE_DATA_MODE,
    cbSTATUS_TIMEOUT
 
 } cbRTSL_Status;

#endif /* _CB_STATUS_H_ */

