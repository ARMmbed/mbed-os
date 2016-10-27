/*************************************************************************************************/
/*!
 *  \file   cfg_stack.h
 *
 *  \brief  Stack configuration.
 *
 *          $Date: 2015-06-12 04:19:18 -0700 (Fri, 12 Jun 2015) $
 *          $Revision: 3061 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
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

/**************************************************************************************************
  DM
**************************************************************************************************/

/*! Maximum number of connections */
#ifndef DM_CONN_MAX
#define DM_CONN_MAX 3
#endif

/**************************************************************************************************
  L2C
**************************************************************************************************/

/*! Maximum number of connection oriented channels */
#ifndef L2C_COC_CHAN_MAX
#define L2C_COC_CHAN_MAX 8
#endif

/*! Maximum number of connection oriented channel registered clients */
#ifndef L2C_COC_REG_MAX
#define L2C_COC_REG_MAX 4
#endif

/**************************************************************************************************
  ATT
**************************************************************************************************/

/**************************************************************************************************
  SMP
**************************************************************************************************/



#ifdef __cplusplus
};
#endif

#endif /* CFG_STACK_H */
