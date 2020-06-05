/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP device database.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
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

/* Period in ms to service the database. */
#define SMP_DB_SRV_MS               1000

/* Decrement a timer. */
#define SMP_DB_DEC_TIMER(a)         do { a = a > SMP_DB_SRV_MS ? a - SMP_DB_SRV_MS : 0;} while (0)

/* Device database indicies. */
#define SMP_DB_COMMON_REC           0     /*! Common record used when database is full. */
#define SMP_DB_FIRST_REC            1     /*! Index of first device specific record in database. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  bdAddr_t      peerAddr;                 /*! Peer address. */
  uint8_t       addrType;                 /*! Peer address type. */
  uint8_t       failCount;                /*! Prior attempt failure count. */
  uint16_t      attemptMult;              /*! Attempts timeout multiplier (0 - record not used). */
  uint32_t      lockMs;                   /*! Time remaining until device can attempt pairing. */
  uint32_t      expDecrementMs;           /*! Time remaining until attempt attemptMult decreases. */
  uint32_t      failCountToMs;            /*! Time remaining until failCount is cleared. */
} smpDbDevice_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Control block. */
static struct
{
  smpDbDevice_t db[SMP_DB_MAX_DEVICES];   /*! Device database. */
  wsfTimer_t    serviceTimer;             /*! Timer to service database. */
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
 *  \brief  Check if a database record is in use.
 *
 *  \param  pRec    Pointer to the record.
 *
 *  \return TRUE if record in use, else FALSE.
 */
/*************************************************************************************************/
static bool_t smpDbRecordInUse(smpDbDevice_t *pRec)
{
  /* When failCount, lockMs, and attemptMult are zero, the record can be used for another device. */
  if (pRec->failCount > 0)
  {
    return TRUE;
  }

  if (pRec->lockMs > 0)
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
 *  \brief  Add a device to the database.
 *
 *  \param  pAddr     Peer BD address.
 *  \param  addrType  Peer BD address type.
 *
 *  \return Pointer to DB record or NULL if database full.
 */
/*************************************************************************************************/
static smpDbDevice_t *smpDbAddDevice(uint8_t *pAddr, uint8_t addrType)
{
  smpDbDevice_t *pRec = &smpDbCb.db[SMP_DB_FIRST_REC];
  uint8_t i;

  SMP_TRACE_INFO0("smpDbAddDevice");

  for (i = SMP_DB_FIRST_REC; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec) == FALSE)
    {
      /* Reset record. */
      memset(pRec, 0, sizeof(smpDbDevice_t));

      pRec->addrType = addrType;
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
 *  \return Pointer to the record associated with the connection or the common record.
 */
/*************************************************************************************************/
static smpDbDevice_t *smpDbGetRecord(dmConnId_t connId)
{
  smpDbDevice_t *pRec = &smpDbCb.db[SMP_DB_FIRST_REC];
  uint8_t addrType = DmHostAddrType(DmConnPeerAddrType(connId));
  uint8_t *pAddr = DmConnPeerAddr(connId);
  uint8_t i;

  SMP_TRACE_INFO2("smpDbGetRecord: connId: %d type: %d", connId, addrType);

  for (i = SMP_DB_FIRST_REC; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec) && (pRec->addrType == addrType) && BdaCmp(pRec->peerAddr, pAddr))
    {
      return pRec;
    }
  }

  /* Device is not in the database, add the device. */
  pRec = smpDbAddDevice(pAddr, addrType);

  if (pRec == NULL)
  {
    SMP_TRACE_INFO0("smpDbGetRecord: common record");

    /* Database is full, use the common record. */
    pRec = &smpDbCb.db[SMP_DB_COMMON_REC];
  }

  return pRec;
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
  /* Stop active service timer. */
  if (smpDbCb.serviceTimer.isStarted == TRUE)
  {
    WsfTimerStop(&smpDbCb.serviceTimer);
  }

  /* Reset control block. */
  memset(&smpDbCb, 0, sizeof(smpDbCb));

  /* Setup service timer. */
  smpDbCb.serviceTimer.handlerId = smpCb.handlerId;
  smpDbCb.serviceTimer.msg.event = SMP_DB_SERVICE_IND;
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

  SMP_TRACE_INFO3("SmpDbGetPairingDisabledTime: connId: %d period: %d attemptMult: %d",
                  connId, pRec->lockMs, pRec->attemptMult);

  return pRec->lockMs;
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

  SMP_TRACE_INFO2("SmpDbSetFailureCount: connId: %d count: %d", connId, count);

  pRec->failCount = count;

  if (count != 0)
  {
    pRec->failCountToMs = pSmpCfg->maxAttemptTimeout;
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

  SMP_TRACE_INFO2("SmpDbGetFailureCount: connId: %d count: %d", connId, pRec->failCount);

  return pRec->failCount;
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
  uint16_t multiplier;

  SMP_TRACE_INFO1("SmpDbMaxAttemptReached: connId: %d", connId);

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

  pRec->expDecrementMs = pSmpCfg->attemptDecTimeout;

  /* Ensure the service timer is running. */
  smpDbStartServiceTimer();

  return pRec->lockMs;
}

/*************************************************************************************************/
/*!
 *  \brief  Called to report to the SMP DB that a pairing operation failed
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbPairingFailed(dmConnId_t connId)
{
  smpDbDevice_t *pRec = smpDbGetRecord(connId);

  SMP_TRACE_INFO1("SmpDbPairingFailed: connId: %d", connId);

  /* Reset exponent decrement timer. */
  pRec->expDecrementMs = pSmpCfg->attemptDecTimeout;
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
  uint8_t i;
  smpDbDevice_t *pRec = smpDbCb.db;

  /* Service device specific records. */
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

/*************************************************************************************************/
/*!
 *  \brief  Remove all records from the SMP database.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbRemoveAllDevices(void)
{
  /* Stop active service timer. */
  if (smpDbCb.serviceTimer.isStarted == TRUE)
  {
    WsfTimerStop(&smpDbCb.serviceTimer);
  }

  /* Reset database. */
  memset(&smpDbCb.db, 0, sizeof(smpDbCb.db));
}

/*************************************************************************************************/
/*!
 *  \brief  Remove a device with the given identity from the SMP database.
 *`
 *  \param  pAddr     Pointer to BD Address.
 *  \param  addrType  BD Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbRemoveDevice(uint8_t *pAddr, uint8_t addrType)
{
  smpDbDevice_t *pRec = &smpDbCb.db[SMP_DB_FIRST_REC];
  uint8_t peerAddrType = DmHostAddrType(addrType);
  uint8_t i;

  for (i = SMP_DB_FIRST_REC; i < SMP_DB_MAX_DEVICES; i++, pRec++)
  {
    if (smpDbRecordInUse(pRec) && (pRec->addrType == peerAddrType) && BdaCmp(pRec->peerAddr, pAddr))
    {
      /* Reset record. */
      memset(pRec, 0, sizeof(smpDbDevice_t));

      return;
    }
  }
}
