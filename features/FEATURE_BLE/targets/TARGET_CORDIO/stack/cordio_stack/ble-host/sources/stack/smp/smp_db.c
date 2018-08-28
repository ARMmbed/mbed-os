/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP device database.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
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

#include <string.h>
#include "wsf_types.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "wsf_timer.h"
#include "smp_api.h"
#include "smp_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Period in ms to service the database */
#define SMP_DB_SRV_MS              1000

/* Decrement a timer */
#define SMP_DB_DEC_TIMER(a)        do { a = a > SMP_DB_SRV_MS ? a - SMP_DB_SRV_MS : 0;} while (0)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  bdAddr_t      peerAddr;                 /*! Peer address */
  uint8_t       addrType;                 /*! Peer address type */
  uint8_t       failCount;                /*! Prior attempt failure count */
  uint16_t      attemptMult;              /*! Attempts timeout multiplier (0 - record not used) */
  uint32_t      lockMs;                   /*! Time remaining until device can attempt pairing */
  uint32_t      expDecrementMs;           /*! Time remaining until attempt attemptMult decreases */
  uint32_t      failCountToMs;            /*! Time remaining until failCount is cleared */
} smpDbDevice_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

static struct
{
  smpDbDevice_t db[SMP_DB_MAX_DEVICES];   /*! Device Database */
  wsfTimer_t    serviceTimer;             /*! Timer to service database */
} smpDbCb;

/*************************************************************************************************/
/*!
 *  \brief  Ensure the SMP DB service timer is running.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void smpDbStartServiceTimer(void)
{
  if (smpDbCb.serviceTimer.isStarted == FALSE)
  {
    WsfTimerStartMs(&smpDbCb.serviceTimer, SMP_DB_SRV_MS);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the SMP Database.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbInit(void)
{
  /* Stop active service timer */
  if (smpDbCb.serviceTimer.isStarted == TRUE)
  {
    WsfTimerStop(&smpDbCb.serviceTimer);
  }

  /* Reset control block */
  memset(&smpDbCb, 0, sizeof(smpDbCb));

  /* Setup service timer */
  smpDbCb.serviceTimer.handlerId = smpCb.handlerId;
  smpDbCb.serviceTimer.msg.event = SMP_DB_SERVICE_IND;
}

/*************************************************************************************************/
/*!
 *  \brief  Check if a database record is in use.
 *
 *  \param  pRec    Pointer to the record.
 *
 *  \return TRUE if record in use, else FALSE.
 */
/*************************************************************************************************/
static bool_t smpDbRecordInUse(smpDbDevice_t *pRec)
{
  /* When failCount and attemptMult decrement to zero, the record can be used for another device */
  if (pRec->failCount > 0)
  {
    return TRUE;
  }

  if (pRec->attemptMult > 0)
  {
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Check if the database is full.
 *
 *  \return TRUE if databse is FULL, else FALSE.
 */
/*************************************************************************************************/
static bool_t smpDbFull()
{
  smpDbDevice_t *pRec = smpDbCb.db;
  int8_t i;

  for (i = 0; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec) == FALSE)
    {
      return FALSE;
    }
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Add a device to the database.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return Pointer to DB record.
 */
/*************************************************************************************************/
static smpDbDevice_t *smpDbAddDevice(dmConnId_t connId)
{
  smpDbDevice_t *pRec = smpDbCb.db;
  uint8_t addrType = DmConnPeerAddrType(connId);
  uint8_t *pAddr = DmConnPeerAddr(connId);
  uint8_t peerAddrType = DmHostAddrType(addrType);
  int8_t i;

  for (i = 0; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec) == FALSE)
    {
      pRec->addrType = peerAddrType;
      BdaCpy(pRec->peerAddr, pAddr);
      return pRec;
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Get a record in the database.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return The pointer to the record or NULL.
 */
/*************************************************************************************************/
static smpDbDevice_t *smpDbGetRecord(dmConnId_t connId)
{
  smpDbDevice_t *pRec = smpDbCb.db;
  uint8_t addrType = DmConnPeerAddrType(connId);
  uint8_t *pAddr = DmConnPeerAddr(connId);
  uint8_t peerAddrType = DmHostAddrType(addrType);
  int8_t i;

  SMP_TRACE_INFO2("smpDbGetRecord: connId: %d type: %d", connId, addrType);

  for (i = 0; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec) && (pRec->addrType == peerAddrType) && BdaCmp(pRec->peerAddr, pAddr))
    {
      return pRec;
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the time (msec) that pairing is disabled for a device.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return Time pairing is disabled (msec), or zero if pairing isn't disabled.
 */
/*************************************************************************************************/
uint32_t SmpDbGetPairingDisabledTime(dmConnId_t connId)
{
  smpDbDevice_t *pRec = smpDbGetRecord(connId);

  if (pRec)
  {
    SMP_TRACE_INFO2("SmpDbGetPairingDisabledTime: connId: %d period: %d", connId, pRec->lockMs);
    return pRec->lockMs;
  }

  return 0;
}

/*************************************************************************************************/
/*!
 *  \brief  Set the count of prior failures for a device.
 *
 *  \param  connId    Connection identifier.
 *  \param  count     Failure count.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbSetFailureCount(dmConnId_t connId, uint8_t count)
{
  smpDbDevice_t *pRec = smpDbGetRecord(connId);

  if (pRec == NULL)
  {
    pRec = smpDbAddDevice(connId);
  }

  if (pRec)
  {
    SMP_TRACE_INFO2("SmpDbSetFailureCount: connId: %d count: %d", connId, count);

    pRec->failCount = count;

    if (count != 0)
    {
      pRec->failCountToMs = pSmpCfg->maxAttemptTimeout;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Get the count of prior failures for a device.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return The failure count.
 */
/*************************************************************************************************/
uint8_t SmpDbGetFailureCount(dmConnId_t connId)
{
  smpDbDevice_t *pRec = smpDbGetRecord(connId);

  if (pRec)
  {
    SMP_TRACE_INFO2("SmpDbGetFailureCount: connId: %d count: %d", connId, pRec->failCount);
    return pRec->failCount;
  }

  if (smpDbFull())
  {
    /* The database if full. Return one less than max to prevent disconnect/reconnect exploit. */
    SMP_TRACE_INFO0("SmpDbGetFailureCount: db full");
    return pSmpCfg->maxAttempts - 1;
  }

  return 0;
}

/*************************************************************************************************/
/*!
 *  \brief  Called to report max pairing attempts was reached for a device.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return Time until device can retry pairing.
 */
/*************************************************************************************************/
uint32_t SmpDbMaxAttemptReached(dmConnId_t connId)
{
  smpDbDevice_t *pRec = smpDbGetRecord(connId);

  SMP_TRACE_INFO1("SmpDbMaxAttemptReached: connId: %d", connId);

  if (pRec)
  {
    uint16_t multiplier;

    if (pRec->attemptMult == 0)
    {
      /* Due to a disconnection, a record exists but the attempt multipier hasn't been set. */
      multiplier = 1;
    }
    else
    {
      multiplier = (pRec->attemptMult * pSmpCfg->attemptExp);
    }

    if ((pSmpCfg->attemptTimeout * multiplier) <= pSmpCfg->maxAttemptTimeout)
    {
      pRec->lockMs = pSmpCfg->attemptTimeout * multiplier;
      pRec->attemptMult = multiplier;
    }
    else
    {
      /* Exponential increase is greater than max timeout. */
      pRec->lockMs = pSmpCfg->maxAttemptTimeout;
    }

    pRec->expDecrementMs =  pSmpCfg->attemptDecTimeout;

    /* Ensure the service timer is running. */
    smpDbStartServiceTimer();

    return pRec->lockMs;
  }
  else
  {
    /* Device not in database, try to add the device. */
    pRec = smpDbAddDevice(connId);

    if (pRec)
    {
      pRec->expDecrementMs =  pSmpCfg->attemptDecTimeout;
      pRec->lockMs = pSmpCfg->attemptTimeout;
      pRec->attemptMult = 1;

      /* Ensure the service timer is running. */
      smpDbStartServiceTimer();
    }

    return pSmpCfg->attemptTimeout;
  }

  /* The database if full. Return max time. */
  return pSmpCfg->maxAttemptTimeout;
}

/*************************************************************************************************/
/*!
 *  \brief  Service the device database timers
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbService(void)
{
  int8_t i;
  smpDbDevice_t *pRec = smpDbCb.db;

  for (i = 0; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec))
    {
      /* Decrement all time periods. */
      SMP_DB_DEC_TIMER(pRec->expDecrementMs);
      SMP_DB_DEC_TIMER(pRec->lockMs);
      SMP_DB_DEC_TIMER(pRec->failCountToMs);

      /* Process expDecrementMs timeout. */
      if (pRec->expDecrementMs == 0)
      {
        /* Exponential decrease of multiplier. */
        pRec->attemptMult /= pSmpCfg->attemptExp;

        if (pRec->attemptMult)
        {
          pRec->expDecrementMs =  pSmpCfg->attemptDecTimeout;
        }
      }

      /* Process failCountToMs timeout. */
      if (pRec->failCountToMs == 0)
      {
        pRec->failCount = 0;
      }

      /* If the record is in use, ensure the service timer is running. */
      if (smpDbRecordInUse(pRec))
      {
        smpDbStartServiceTimer();
      }
    }
  }
}

