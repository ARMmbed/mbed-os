/*************************************************************************************************/
/*!
 *  \file   hci_drv.h
 *
 *  \brief  HCI vendor specific functions for EM Microelectronic.
 *
 *          $Date: 2013-01-02 22:19:17 -0800 (Wed, 02 Jan 2013) $
 *          $Revision: 405 $
 *
 *  Copyright (c) 2012-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_VS_H
#define HCI_VS_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/
/*!
 *  \fn     HciVsSetPublicAddr
 *        
 *  \brief  Vendor-specific set public address function.
 *
 *  \param  param    public address
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsSetPublicAddr(uint8_t *bdAddr);

/*************************************************************************************************/
/*!
 *  \fn     HciVsSetTxPower
 *        
 *  \brief  Vendor-specific set RF output power function
 *
 *  \param  param    output power in dB
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsSetTxPower(int txPower);

#ifdef __cplusplus
};
#endif

#endif /* HCI_VS_H */
