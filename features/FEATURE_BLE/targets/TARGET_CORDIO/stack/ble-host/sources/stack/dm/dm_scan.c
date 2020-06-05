/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager scan module.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "dm_api.h"
#include "dm_scan.h"
#include "dm_main.h"

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Control block */
dmScanCb_t dmScanCb;

/*************************************************************************************************/
/*!
 *  \brief  Initialize the scan module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanInit(void)
{
  uint8_t i;

  /* initialize control block */
  for (i = 0; i < DM_NUM_PHYS; i++)
  {
    dmScanCb.scanInterval[i] = DM_GAP_SCAN_FAST_INT_MIN;
    dmScanCb.scanWindow[i] = DM_GAP_SCAN_FAST_WINDOW;
  }

  dmCb.scanFiltPolicy = HCI_FILT_NONE;
  dmScanCb.scanTimer.handlerId = dmCb.handlerId;
  dmScanCb.scanState = DM_SCAN_STATE_IDLE;
  dmCb.scanAddrType = DM_ADDR_PUBLIC;
}

/*************************************************************************************************/
/*!
 *  \brief  Start scanning on the given PHYs.
 *
 *  \param  scanPhys  Scanner PHYs.
 *  \param  mode      Discoverability mode.
 *  \param  pScanType Scan type array.
 *  \param  filterDup Filter duplicates.  Set to TRUE to filter duplicate responses received
 *                    from the same device.  Set to FALSE to receive all responses.
 *  \param  duration  The scan duration, in milliseconds.  If set to zero or both duration and
 *                    period set to non-zero, scanning will continue until DmScanStop() is called.
 *  \param  period    The scan period, in 1.28 sec units (only applicable to AE).  If set to zero,
 *                    periodic scanning is disabled.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanStart(uint8_t scanPhys, uint8_t mode, const uint8_t *pScanType, bool_t filterDup,
                 uint16_t duration, uint16_t period)
{
  uint8_t i;              /* scanPhy bit position */
  uint8_t idx;            /* param array index */
  dmScanApiStart_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmScanApiStart_t))) != NULL)
  {
    pMsg->hdr.event = DM_SCAN_MSG_API_START;
    pMsg->scanPhys = scanPhys;

    for (i = 0, idx = 0; (i < 8) && (idx < DM_NUM_PHYS); i++)
    {
      if (scanPhys & (1 << i))
      {
        /* scan type for this PHY */
        pMsg->scanType[idx] = pScanType[idx];
        idx++;
      }
    }

    pMsg->mode = mode;
    pMsg->duration = duration;
    pMsg->period = period;
    pMsg->filterDup = filterDup;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Stop scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanStop(void)
{
  wsfMsgHdr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(wsfMsgHdr_t))) != NULL)
  {
    pMsg->event = DM_SCAN_MSG_API_STOP;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window.
 *
 *  \param  phyIdx        The scanning PHY.
 *  \param  scanInterval  The scan interval.
 *  \param  scanWindow    The scan window.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmScanSetInterval(uint8_t scanPhy, uint16_t scanInterval, uint16_t scanWindow)
{
  uint8_t phyIdx;

  WSF_ASSERT((scanPhy == HCI_INIT_PHY_LE_1M_BIT) || (scanPhy == HCI_INIT_PHY_LE_CODED_BIT));

  WsfTaskLock();
  phyIdx = DmScanPhyToIdx(scanPhy);
  dmScanCb.scanInterval[phyIdx] = scanInterval;
  dmScanCb.scanWindow[phyIdx] = scanWindow;
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for the specified PHYs.
 *
 *  \param  scanPhys      Scanning PHYs.
 *  \param  pScanInterval Scan interval array.
 *  \param  pScanWindow   Scan window array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanSetInterval(uint8_t scanPhys, uint16_t *pScanInterval, uint16_t *pScanWindow)
{
  uint8_t i;   /* scanPhy bit position */
  uint8_t idx; /* param array index */

  for (i = 0, idx = 0; (i < 8) && (idx < DM_NUM_PHYS); i++)
  {
    if (scanPhys & (1 << i))
    {
      dmScanSetInterval((1 << i), pScanInterval[idx], pScanWindow[idx]);
      idx++;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the local address type used while scanning.  This function can be used to
 *          configure scanning to use a random address.
 *
 *  \param  addrType  Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanSetAddrType(uint8_t addrType)
{
  WsfTaskLock();
  dmCb.scanAddrType = addrType;
  WsfTaskUnlock();
}
