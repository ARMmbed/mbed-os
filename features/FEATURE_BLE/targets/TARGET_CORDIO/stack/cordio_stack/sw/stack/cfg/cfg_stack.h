/*************************************************************************************************/
/*!
 *  \file   cfg_stack.h
 *
 *  \brief  Stack configuration.
 *
 *          $Date: 2017-03-10 14:08:37 -0600 (Fri, 10 Mar 2017) $
 *          $Revision: 11501 $
 *
 *  Copyright (c) 2009-2017 ARM Ltd., all rights reserved.
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
 */
/*************************************************************************************************/
#ifndef CFG_STACK_H
#define CFG_STACK_H


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  HCI
**************************************************************************************************/

/*! Vendor specific targets */
#define HCI_VS_GENERIC        0
#define HCI_VS_EMM            1

/*! Vendor specific target configuration */
#ifndef HCI_VS_TARGET
#define HCI_VS_TARGET HCI_VS_GENERIC
#endif

/* Extra byte allocation required for LL operations (i.e. MIC) in single-chip implementation */
#ifndef HCI_TX_DATA_TAILROOM
#define HCI_TX_DATA_TAILROOM  0
#endif

/**************************************************************************************************
  DM
**************************************************************************************************/

/*! Maximum number of connections */
#ifndef DM_CONN_MAX
#define DM_CONN_MAX           3
#endif

/*! Number of supported advertising sets: must be set to 1 for legacy advertising */
#ifndef DM_NUM_ADV_SETS
#define DM_NUM_ADV_SETS       1
#endif

/*! Number of scanner and initiator PHYs (LE 1M, LE 2M and LE Coded): must be set to 1 for
    legacy scanner and initiator */
#ifndef DM_NUM_PHYS
#define DM_NUM_PHYS           1
#endif

/**************************************************************************************************
  L2C
**************************************************************************************************/

/*! Maximum number of connection oriented channels */
#ifndef L2C_COC_CHAN_MAX
#define L2C_COC_CHAN_MAX      8
#endif

/*! Maximum number of connection oriented channel registered clients */
#ifndef L2C_COC_REG_MAX
#define L2C_COC_REG_MAX       4
#endif

/**************************************************************************************************
  ATT
**************************************************************************************************/

/*! Maximum number of simultanous ATT notifications */
#ifndef ATT_NUM_SIMUL_NTF
#define ATT_NUM_SIMUL_NTF     1
#endif

/**************************************************************************************************
  SMP
**************************************************************************************************/


#ifdef __cplusplus
};
#endif

#endif /* CFG_STACK_H */
