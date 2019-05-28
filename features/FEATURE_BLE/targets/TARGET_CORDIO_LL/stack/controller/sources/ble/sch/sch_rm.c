/* Copyright (c) 2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 * \file
 * \brief Operation list maintenance implementation file.
 */
/*************************************************************************************************/

/*************************************************************************************************/
/*
 *  This resource manager consists of the following features:
 *
 *      * Verification of reservation capacity
 *      * Selection of intervals given a range
 *      * Selection of txWindowOffset for adding or updating a reservation
 *
 *  Selection of txWindowOffset is optimized to allow the most medium time between each
 *  reservation's CE. It attempts to place the offset at 1/2 the largest gap among the largest
 *  reservation intervals.
 *
 *  For example, consider adding 3 reservations as illustrated below. Each reservation is added
 *  in order of its handle value. The first reservation added, handle=0, is given the maximum
 *  txWindowOffset to maximize the scan opportunity. Next, handle=1 is placed 1/2 the interval
 *  of handle=0 to maximize the CE duration between both handle=0 and handle=1. And finally,
 *  handle=2 and handle=3 split the remaining gaps.
 *
 *  rsvn[0] = { .handle=3, .offsUsec=30000, .interUsec=120000 }
 *  rsvn[1] = { .handle=1, .offsUsec=60000, .interUsec=120000 }
 *  rsvn[2] = { .handle=2, .offsUsec=90000, .interUsec=120000 }
 *  rsvn[3] = { .handle=0, .offsUsec=120000, .interUsec=120000 }
 *  |--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-
 *  :...........0       :   0     :     0   :       0 :         0         : 0       :   0     :     0   :
 *  :.....1   :       1 :         1         : 1       :   1     :     1   :       1 :         1         :
 *  :........2:         :2        :  2      :    2    :      2  :        2:         :2        :  2      :
 *  :..3      :    3    :      3  :        3:         :3        :  3      :    3    :      3  :        3:
 *  |---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
 *  0         1         2         3         4         5         6         7         8         9        10-ms
 *
 */
/*************************************************************************************************/

#include "sch_int_rm.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include <string.h>

/*! \brief      Assert table doesn't exceed 32 entries addressable by RM 32-bit descriptor masks. */
WSF_CT_ASSERT((SCH_RM_MAX_RSVN <= 32));

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Total number of reservation bins. */
#define SCH_RM_MAX_RSVN_BINS      ((sizeof(schRmRsvnRatio)/sizeof(schRmRsvnRatio[0])) - 1)

/*! \brief      Nominal window widening in microseconds in slave scheduler. */
#define SCH_RM_SLAVE_WW_USECS     100

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Scheduler resource manager control block. */
SchRmCb_t schRmCb;

/*! \brief      Reservation ratio for each reservation bin. One reservation with 10ms takes 50%, one with 20ms takes 25% and so on. */
const uint16_t schRmRsvnRatio[] = {50, 25, 17, 13, 10, 5};

/*************************************************************************************************/
/*!
 *  \brief      Select the preferred interval.
 *
 *  \param      minUsec     Minimum interval in microseconds.
 *  \param      maxUsec     Maximum interval in microseconds.
 *  \param      prefPerUsec Preferred periodicity in microseconds.
 *  \param      pref        Preference for selecting the interval.
 *  \param      exclude     Handle to exclude.
 *
 *  \return     Preferred interval in microseconds.
 */
/*************************************************************************************************/
static uint32_t schRmSelectPreferredIntervalUsec(uint32_t minUsec, uint32_t maxUsec, uint32_t prefPerUsec, uint8_t pref, uint8_t exclude)
{
  uint32_t prefInterUsec;
  uint32_t numInter;

  if (pref == SCH_RM_PREF_CAPACITY)   /* Search from upper bound */
  {
    numInter = (prefPerUsec > SCH_RM_PREF_PER_USEC) ? SCH_RM_DIV_PREF_PER(maxUsec) / SCH_RM_DIV_PREF_PER(prefPerUsec) : SCH_RM_DIV_PREF_PER(maxUsec);
    prefInterUsec = numInter * prefPerUsec;

    /* Use maxUsec if we can not find any interval between min and max with our preferred periodicity. */
    if (prefInterUsec < minUsec)
    {
      prefInterUsec = maxUsec;
    }
  }
  else  /* Search from lower bound */
  {
    numInter = (prefPerUsec > SCH_RM_PREF_PER_USEC) ? SCH_RM_DIV_PREF_PER(minUsec) / SCH_RM_DIV_PREF_PER(prefPerUsec) : SCH_RM_DIV_PREF_PER(minUsec);
    prefInterUsec = numInter * prefPerUsec;

    if (prefInterUsec < minUsec)
    {
      /* Round up to next periodic interval. */
      prefInterUsec = (numInter + 1) * prefPerUsec;
    }
  }

  /*** Prefer existing intervals ***/

  uint32_t intervUsec[SCH_RM_MAX_RSVN];
  uint8_t numIntervUsec = 0;

  for (unsigned int i = 0; i < SCH_RM_MAX_RSVN; i++)
  {
    if ((i != exclude) &&
        schRmCb.rsvnInterUsec[i])
    {
      intervUsec[numIntervUsec++] = schRmCb.rsvnInterUsec[i];
    }
  }

  /* Use larger intervals first to avoid capacity limitation. */
  schRmSortListDescending(intervUsec, numIntervUsec);

  for (unsigned int i = 0; i < numIntervUsec; i++)
  {
    if ((intervUsec[i] >= prefInterUsec) &&
        (intervUsec[i] <= maxUsec))
    {
      return intervUsec[i];
    }
  }

  /*** Limit upper bound ***/

  if (prefInterUsec > maxUsec)
  {
    /* Limit to max value. */
    prefInterUsec = maxUsec;
  }

  return prefInterUsec;
}

/*************************************************************************************************/
/*!
 *  \brief      Check reservation is within capacity.
 *
 *  \param      handle      Reservation handle.
 *  \param      interUsec   Interval in microseconds.
 *
 *  \return     TRUE if new reservation does not exceed capacity, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t schRmCheckRsvnCapacity(uint8_t handle, uint32_t interUsec)
{
  unsigned int maxBinIdx = 0;

  /* Number of reservations by interval size; zero index is SCH_RM_PREF_PER_USEC. */
  uint8_t numRsvnPerInter[SCH_RM_MAX_RSVN_BINS + 1] = { 0 };
  for (unsigned int i = 0; i < SCH_RM_MAX_RSVN; i++)
  {
    unsigned int rsvnIntervUsec;
    if (i != handle)
    {
      rsvnIntervUsec = schRmCb.rsvnInterUsec[i];
    }
    else
    {
      rsvnIntervUsec = interUsec;
    }

    if (!rsvnIntervUsec)
    {
      /* Inactive reservation. */
      continue;
    }

    /* Bin interval value inclusive of max value. */
    unsigned int binIdx = SCH_RM_DIV_PREF_PER(rsvnIntervUsec - 1);

    binIdx = (binIdx > SCH_RM_MAX_RSVN_BINS) ? SCH_RM_MAX_RSVN_BINS : binIdx;
    numRsvnPerInter[binIdx]++;
    maxBinIdx = WSF_MAX(binIdx, maxBinIdx);
  }

  /* Check capacity. */

  uint16_t rsvnPerc = 0;    /* Reservation percentage */
  uint16_t numRsvn;
  bool_t isMaxCap = FALSE;

  for (unsigned int i = 0; i < (maxBinIdx + 1); i++)
  {
    numRsvn = numRsvnPerInter[i];
    while (numRsvn)
    {
      if (!isMaxCap)
      {
        rsvnPerc += schRmRsvnRatio[i];
        numRsvn--;

        if (rsvnPerc >= 100)
        {
          isMaxCap = TRUE;
        }
      }

      if (isMaxCap && (numRsvn > 0))
      {
        /* Capacity exceeded. */
        return FALSE;
      }
    }
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Calculate depth between interval.
 *
 *  \param      intLarge    Larger interval to be compared.
 *  \param      intSmall    Smaller interval to be compared.
 *
 *  \return     Depth between intervals.
 */
/*************************************************************************************************/
static uint8_t schRmIntCalculateDepth(uint32_t intLarge, uint32_t intSmall)
{
  uint8_t x;
  uint8_t depth = 0;
  uint32_t tmpInt = intLarge;

  WSF_ASSERT(intLarge > intSmall);

  for (x = 0; (x < SCH_RM_MAX_SEARCH_DEPTH) && (tmpInt > SCH_RM_MIN_OFFSET_UNIT); x++)
  {
    tmpInt >>= 1;
    if (tmpInt == intSmall)
    {
      depth = x + 1;
      break;
    }
  }

  return depth;
}

/*************************************************************************************************/
/*!
 *  \brief      Add resource manager offset for the specified handle.
 *
 *  \param      handle      Client defined reservation handle.
 *  \param      depth       Offset depth of the interval of the handle compared to maxConInt.
 *
 *  \return     TRUE if update was successful, FALSE otherwise..
 */
/*************************************************************************************************/
static bool_t schRmIntAddRmOffset(uint8_t handle, uint8_t depth)
{
  bool_t updated = FALSE;
  uint8_t x, y, numBitsOn;

  WSF_ASSERT(schRmCb.offsetDepth >= depth);

  for (x = 0; x < (1 << (schRmCb.offsetDepth - depth)); x++)
  {
    /* Depth 0 means that the interval of the handle is same as common interval. Depth 1 means that it is half of common interval. */
    /* When offsetDepth is 3(8 bits) and depth is 1, we will cut rmStatus in half and check if the bits in same location are all zero. */
    numBitsOn = 0;
    for (y = 0; y < (1 << depth); y++)
    {
      if ((schRmCb.rmStatus >> ((1 << (schRmCb.offsetDepth - depth)) * y + x)) & 0x01)
      {
        numBitsOn++;
      }
    }

    /* Found available offset bit(s). */
    if (numBitsOn == 0)
    {
      /* Set all the bits */
      for (y = 0; y < (1 << depth); y++)
      {
        schRmCb.rmStatus |= (0x01 << ((1 << (schRmCb.offsetDepth - depth)) * y + x));
      }

      schRmCb.rsvn[handle].offsetBit = x;
      schRmCb.rsvn[handle].commIntUsed = TRUE;
      updated = TRUE;

      LL_TRACE_WARN2("schRmIntAddRmOffset, handle = %u, offsetDepth = %u", handle, schRmCb.offsetDepth);
      LL_TRACE_WARN1("                     rmStatus = 0x%x", schRmCb.rmStatus);
      LL_TRACE_WARN1("                     commonInt = %u", schRmCb.commonInt);
      break;
    }
  }

  return updated;
}

/*************************************************************************************************/
/*!
 *  \brief      Increase reservation manager offset depth.
 *
 *  \param      depth     Offset depth to be increased.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schRmIntIncOffsetDepth(uint8_t depth)
{
  uint32_t tmpStatus = schRmCb.rmStatus;
  uint8_t x, handle;

  /* b'11 becomes b'00010001, when depth is 2. */
  schRmCb.rmStatus = 0;
  for (x = 0; x < (1 << schRmCb.offsetDepth); x++)
  {
    if ((tmpStatus >> x) & 0x01)
    {
      schRmCb.rmStatus |= (0x01 << (x * (1 << depth)));
    }
  }

  schRmCb.offsetDepth += depth;

  /* Update database according to the new offsetDepth. */
  for (handle = 0; handle < SCH_RM_MAX_RSVN; handle++)
  {
    if (schRmCb.rsvn[handle].commIntUsed)
    {
      /* For example, with depth 2, bit1 will move into bit4. */
      schRmCb.rsvn[handle].offsetBit *= (1 << depth);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Decrease reservation manager offset depth by one if necessary.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schRmIntCheckDecOffsetDepth(void)
{
  uint32_t tmpStatus = schRmCb.rmStatus;
  uint8_t x, evenOdd, other, handle, numBitsOn;
  uint8_t depth = schRmCb.offsetDepth - 1;

  if (schRmCb.offsetDepth < 2)
  {
    /* nothing to do. */
    return;
  }

  /* b'1010 with depth 2 is same as b'11 with depth 1. */
  /* Check if all even or odd bits are zero. */
  for (evenOdd = 0; evenOdd < 2; evenOdd++)
  {
    numBitsOn = 0;
    other = (evenOdd == 0) ? 1 : 0; /* When checking even bits, other is 1. */

    for (x = 0; x < (1 << depth); x++)
    {
      if ((tmpStatus >> (x * 2 + evenOdd)) & 0x01)
      {
        numBitsOn++;
      }
    }

    if (numBitsOn == 0)
    {
      schRmCb.offsetDepth--;
      schRmCb.rmStatus = 0;

      for (x = 0; x < (1 << depth); x++)
      {
        if ((tmpStatus >> (x * 2 + other)) & 0x01)
        {
          schRmCb.rmStatus |= (0x01 << (x));
        }
      }

      /* Update database according to the new offsetDepth. */
      for (handle = 0; handle < SCH_RM_MAX_RSVN; handle++)
      {
        if (schRmCb.rsvn[handle].commIntUsed)
        {
          schRmCb.rsvn[handle].offsetBit >>= 1;

          /* The handle for Bit0 is updated to reference handle. */
          if (schRmCb.rsvn[handle].offsetBit == 0)
          {
            schRmCb.refHandle = handle;
          }
        }
      }

      return;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Remove resource manager offset for the specified handle.
 *
 *  \param      handle      Client defined reservation handle.
 *
 *  \return     TRUE if update was successful, FALSE otherwise..
 */
/*************************************************************************************************/
static void schRmIntRemoveRmOffset(uint8_t handle)
{
  uint8_t x, depth;

  if (schRmCb.rsvn[handle].commIntUsed)
  {
    /* The interval of this handle must be equal to or smaller than the common interval. */
    if (schRmCb.commonInt == schRmCb.rsvn[handle].interUsec)
    {
      schRmCb.rmStatus &= ~(1 << schRmCb.rsvn[handle].offsetBit);
    }
    else
    {
      depth = schRmIntCalculateDepth(schRmCb.commonInt, schRmCb.rsvn[handle].interUsec);

      if (depth)
      {
        for (x = 0; x < (1 << depth); x++)
        {
          schRmCb.rmStatus &= ~(1 << ((1 << (schRmCb.offsetDepth - depth)) * x + schRmCb.rsvn[handle].offsetBit));
        }
      }
    }

    schRmCb.rsvn[handle].commIntUsed = FALSE;

    /* Adjust offsetDepth if necessary. */
    schRmIntCheckDecOffsetDepth();

    /* All the reservations are removed. */
    if (schRmCb.rmStatus == 0)
    {
      schRmCb.commonInt = 0;
      schRmCb.offsetDepth = 0;
    }

    LL_TRACE_WARN2("schRmIntRemoveRmOffset, handle = %u, offsetDepth = %u", handle, schRmCb.offsetDepth);
    LL_TRACE_WARN1("                        rmStatus = 0x%x", schRmCb.rmStatus);
    LL_TRACE_WARN1("                        commonInt = %u", schRmCb.commonInt);
  }
  else
  {
    if (schRmCb.indexUncommon > 0)
    {
      /* Uncommon handle by reservation manager. */
      schRmCb.indexUncommon--;
      LL_TRACE_WARN2("schRmIntRemoveRmOffset, deleted uncommon index %u, Handle = %u", schRmCb.indexUncommon, handle);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Increase common interval of reservation manager.
 *
 *  \param      depth     Depth between current interval and new interval.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schRmIntIncCommInterval(uint8_t depth)
{
  uint8_t x, y;
  uint32_t tmpStatus = schRmCb.rmStatus;

  WSF_ASSERT(depth > 0);

  schRmCb.commonInt = schRmCb.commonInt << depth;

  /* Reconstruct rmStatus as to the new common interval. */
  schRmCb.rmStatus = 0;
  for (x = 0; x < (1 << schRmCb.offsetDepth); x++)
  {
    if ((tmpStatus >> x) & 0x01)
    {
      for (y = 0; y < (1 << depth); y++)
      {
        schRmCb.rmStatus |= (0x01 << (y * (1 << schRmCb.offsetDepth) + x));
      }
    }
  }

  schRmCb.offsetDepth += depth;

  /* Offset bit location(offsetBit) stays same for existing handles. */
}

/*************************************************************************************************/
/*!
 *  \brief      Handle Adding resource manager offset for the specified handle.
 *
 *  \param      handle      Client defined reservation handle.
 *
 *  \return     TRUE if update was successful, FALSE otherwise..
 */
/*************************************************************************************************/
static void schRmIntHandleAddRmOffset(uint8_t handle, uint32_t newInterUsec)
{
  schRmCb.rsvn[handle].handle = handle;
  schRmCb.rsvn[handle].interUsec = newInterUsec;

  /* Let's assume all connections are with common multiples. */
  /* Will add function later to determine if 2 intervals are with common intervals. */
  if (schRmCb.commonInt == 0)
  {
    schRmCb.commonInt = newInterUsec;
    schRmCb.indexUncommon = 0;
    schRmCb.offsetDepth = 0;  /* Offset unit is same as commonInt. */
    schRmCb.rmStatus = 0x01;  /* 100 percent reserved. */

    schRmCb.rsvn[handle].offsetBit = 0;
    schRmCb.rsvn[handle].commIntUsed = TRUE;
    schRmCb.refHandle = handle;
  }
  else if (schRmCb.commonInt == newInterUsec)
  {
    if (!schRmIntAddRmOffset(handle, 0))
    {
      /* Offset depth needs to be increased if RM status is full with the current depth. */
      schRmIntIncOffsetDepth(1);
      (void)schRmIntAddRmOffset(handle, 0);
    }
  }
  else if (schRmCb.commonInt < newInterUsec)   /* new interval is multiple of commonInt. */
  {
    uint8_t depth = schRmIntCalculateDepth(newInterUsec, schRmCb.commonInt);

    if (depth)
    {
      schRmIntIncCommInterval(depth);

      if (!schRmIntAddRmOffset(handle, 0))
      {
        /* Offset depth needs to be increased if RM status is full with the current depth. */
        schRmIntIncOffsetDepth(1);
        (void)schRmIntAddRmOffset(handle, 0);
      }
    }
    else
    {
      /* Uncommon handle by reservation manager. */
      schRmCb.indexUncommon++;
      LL_TRACE_WARN2("Adding uncommon index %u, Handle = %u", schRmCb.indexUncommon, handle);
    }
  }
  else if (schRmCb.commonInt > newInterUsec)
  {
    uint8_t depth = schRmIntCalculateDepth(schRmCb.commonInt, newInterUsec);

    if (depth)
    {
      /* First update offset depth so that offset unit would be same as new interval. */
      if (schRmCb.offsetDepth < depth)
      {
        schRmIntIncOffsetDepth(depth - schRmCb.offsetDepth);
      }

      if (!schRmIntAddRmOffset(handle, depth))
      {
        /* Offset depth needs to be increased if RM status is full with the current depth. */
        schRmIntIncOffsetDepth(1);
        (void)schRmIntAddRmOffset(handle, depth);
      }
    }
    else
    {
      /* Uncommon handle by reservation manager. */
      schRmCb.indexUncommon++;
      LL_TRACE_WARN2("Adding uncommon index %u, Handle = %u", schRmCb.indexUncommon, handle);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      In-place descending sort of a list of numbers.
 *
 *  \param      item        List of numbers.
 *  \param      numItems    Number of numbers in item[].
 * Descending
 *
 *  \return     None.
 */
/*************************************************************************************************/
void schRmSortListDescending(uint32_t item[], uint8_t numItems)
{
  if (numItems <= 1)
  {
    return;
  }

  unsigned int numUnsorted = numItems;

  do
  {
    unsigned int swapIdx = 0;

    for (unsigned int i = 1; i < numUnsorted; i++)
    {
      if (item[i-1] < item[i])
      {
        uint32_t tmp;
        tmp = item[i-1];
        item[i-1] = item[i];
        item[i] = tmp;
        swapIdx = i;
      }
    }

    /* Items after swapIdx are sorted. */
    numUnsorted = swapIdx;

  } while (numUnsorted > 1);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the resource manager.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchRmInit(void)
{
  memset(&schRmCb, 0, sizeof(schRmCb));
}

/*************************************************************************************************/
/*!
 *  \brief      Preferred periodicity.
 *
 *  \return     Preferred periodicity in microseconds.
 */
/*************************************************************************************************/
uint32_t SchRmPreferredPeriodUsec(void)
{
  return SCH_RM_PREF_PER_USEC;
}

/*************************************************************************************************/
/*!
 *  \brief      Calculate the common periodicity.
 *
 *  \param      peerPerUsec Peer's periodicity in microseconds.
 *
 *  \return     Common periodicity in microseconds.
 */
/*************************************************************************************************/
uint32_t SchRmCalcCommonPeriodicityUsec(uint32_t peerPerUsec)
{
  /* Is peer periodicity a multiple of local periodicity? */
  if ((SCH_RM_DIV_PREF_PER(peerPerUsec) * SCH_RM_PREF_PER_USEC) == peerPerUsec)
  {
    /* Accommodate the peer's periodicity. */
    return WSF_MAX(SCH_RM_PREF_PER_USEC, peerPerUsec);
  }

  /* Default to local preference. */
  return SCH_RM_PREF_PER_USEC;
}

/*************************************************************************************************/
/*!
 *  \brief      Validate and commit a new reservation against collision.
 *
 *  \param      handle      Client defined reservation handle.
 *  \param      pref        Preference for selecting the interval.
 *  \param      minUsec     Minimum interval in microseconds.
 *  \param      maxUsec     Maximum interval in microseconds.
 *  \param      durUsec     Duration of the connection in microseconds.
 *  \param      pInterUsec  Actual interval return value in microseconds.
 *
 *  \return     TRUE if reservation available, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t SchRmAdd(uint8_t handle, uint8_t pref, uint32_t minUsec, uint32_t maxUsec, uint32_t durUsec, uint32_t *pInterUsec, GetRefTimeCb_t refTimeCb)
{
  uint32_t perfPerUsec = SCH_RM_PREF_PER_CONN_USEC;

  WSF_ASSERT(handle < SCH_RM_MAX_RSVN);
  WSF_ASSERT(maxUsec >= minUsec);

  if (pref == SCH_RM_PREF_CAPACITY)
  {
    perfPerUsec = SCH_RM_PREF_PER_SYNC_USEC;
  }

  /*** Select reservation's preferred interval. ***/

  uint32_t prefInterUsec = schRmSelectPreferredIntervalUsec(minUsec, maxUsec, perfPerUsec, pref, handle);

  /*** Check reservation capacity. ***/

  if (!schRmCheckRsvnCapacity(handle, prefInterUsec))
  {
    return FALSE;
  }

  /*** Commit reservation. ***/

  schRmCb.rsvnInterUsec[handle] = prefInterUsec;
  schRmCb.numRsvn++;
  if (pInterUsec)
  {
    *pInterUsec = prefInterUsec;
  }

  schRmIntHandleAddRmOffset(handle, prefInterUsec);
  schRmCb.rsvn[handle].refTimeCb = refTimeCb;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Start update to a reservation.
 *
 *  \param      handle      Client defined reservation handle.
 *  \param      minUsec     Minimum interval in microseconds.
 *  \param      maxUsec     Maximum interval in microseconds.
 *  \param      perfPerUsec Preferred periodicity in microseconds.
 *  \param      durUsec     Duration of the connection in microseconds.
 *  \param      pInterUsec  Actual interval return value in microseconds.
 *
 *  \return     TRUE if resource is can be updated, FALSE otherwise.
 *
 *  \note       For BLE, connection belonging to handle must be a master only.
 */
/*************************************************************************************************/
bool_t SchRmStartUpdate(uint8_t handle, uint32_t minUsec, uint32_t maxUsec, uint32_t perfPerUsec, uint32_t durUsec, uint32_t *pInterUsec)
{
  WSF_ASSERT(handle < SCH_RM_MAX_RSVN);
  WSF_ASSERT(maxUsec >= minUsec);

  /*** Select reservation's preferred interval. ***/

  uint32_t prefInterUsec = schRmSelectPreferredIntervalUsec(minUsec, maxUsec, perfPerUsec, SCH_RM_PREF_PERFORMANCE, handle);

  /*** Check reservation capacity. ***/

  if (!schRmCheckRsvnCapacity(handle, prefInterUsec))
  {
    return FALSE;
  }

  /*** Commit reservation. ***/

  schRmCb.rsvnInterUsec[handle] = prefInterUsec;
  *pInterUsec = prefInterUsec;

  schRmIntRemoveRmOffset(handle);
  schRmIntHandleAddRmOffset(handle, prefInterUsec);

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Commit an update to a reservation.
 *
 *  \param      handle      Client defined reservation handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchRmCommitUpdate(uint8_t handle)
{
  /* No action required; already committed in SchRmStartUpdate(). */
}

/*************************************************************************************************/
/*!
 *  \brief      Remove an existing reservation.
 *
 *  \param      handle      Client defined reservation handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchRmRemove(uint8_t handle)
{
  WSF_ASSERT(handle < SCH_RM_MAX_RSVN);
  WSF_ASSERT(schRmCb.rsvnInterUsec[handle]);

  schRmCb.rsvnInterUsec[handle] = 0;
  schRmCb.numRsvn--;

  schRmIntRemoveRmOffset(handle);
}

/*************************************************************************************************/
/*!
 *  \brief      Set the current reference point.
 *
 *  \param      handle      Client defined reservation handle.
 *
 *  \return     None.
 *
 *  The handle is the last completed reservation. Set the reference point to the next reservation
 *  represented by this handle.
 */
/*************************************************************************************************/
void SchRmSetReference(uint8_t handle)
{
  /* No action required. */
}

/*************************************************************************************************/
/*!
 *  \brief      Get the next available offset in microseconds.
 *
 *  \param      defOffsUsec Default offset in microseconds.
 *  \param      handle      Reservation handle.
 *  \param      refTime     Reference time in BB ticks.
 *
 *  \return     Offset in microseconds.
 */
/*************************************************************************************************/
uint32_t SchRmGetOffsetUsec(uint32_t defOffsUsec, uint8_t handle, uint32_t refTime)
{
  uint32_t rmRefTime = refTime;
  uint32_t offsUsec, targetTime;
  uint32_t offsetUnitUs = schRmCb.commonInt >> schRmCb.offsetDepth;

  WSF_ASSERT(schRmCb.numRsvn);

  if (schRmCb.numRsvn <= 1)
  {
    /* No competing reservations. */
    return defOffsUsec;
  }

  /* rmRefTime is the time for offset bit 0. */
  if (schRmCb.rsvn[schRmCb.refHandle].refTimeCb != NULL)
  {
    rmRefTime = schRmCb.rsvn[schRmCb.refHandle].refTimeCb(schRmCb.refHandle, NULL);
  }

  if ((schRmCb.commonInt != 0) && (schRmCb.rsvn[handle].commIntUsed == TRUE))
  {
    /* Time of Offset bit n = rmRefTime + offsetUnit * n. */
    offsUsec = offsetUnitUs * schRmCb.rsvn[handle].offsetBit;
    if (offsUsec == 0)
    {
      offsUsec += schRmCb.rsvn[handle].interUsec;
    }

    targetTime = rmRefTime + BB_US_TO_BB_TICKS(offsUsec);

    LL_TRACE_WARN2("SchRmGetOffsetUsec, handle = %u, refHandle = %u", handle, schRmCb.refHandle);
    LL_TRACE_WARN1("                    refTime = %u", refTime);
    LL_TRACE_WARN1("                    targetTime = %u", targetTime);
  }
  else
  {
    uint8_t offsetBit = 0;
    uint8_t numUncommon = schRmCb.indexUncommon;
    uint32_t uncommonOffsetUs = SCH_RM_OFFSET_UNCOMMON; /* Offset in microseconds for uncommon handle */

    numUncommon--;
    while (numUncommon)
    {
      offsetBit++;
      numUncommon--;
      offsetBit = (offsetBit >= (1 << schRmCb.offsetDepth)) ? 0 : offsetBit;
    }

    /* Find the duration of BOD of the handle which occupies the offset bit. */
    for (uint8_t i = 0; i < SCH_RM_MAX_RSVN; i++)
    {
      if ((schRmCb.rsvn[i].offsetBit == offsetBit) && (schRmCb.rsvn[i].commIntUsed == TRUE))
      {
        if (schRmCb.rsvn[i].refTimeCb != NULL)
        {
          (void)schRmCb.rsvn[i].refTimeCb(i, &uncommonOffsetUs);
          uncommonOffsetUs += BbGetSchSetupDelayUs();
        }
      }
    }

    /* Place the uncommon handle away from each common ones to avoid conflicts. */
    targetTime = rmRefTime + BB_US_TO_BB_TICKS(offsetBit * offsetUnitUs) + BB_US_TO_BB_TICKS(uncommonOffsetUs);

    LL_TRACE_WARN2("SchRmGetOffsetUsec, uncommon handle = %u, refHandle = %u", handle, schRmCb.refHandle);
    LL_TRACE_WARN1("                    refTime = %u", refTime);
    LL_TRACE_WARN1("                    targetTime = %u", targetTime);
  }

  /* Time to return has to be future from refTime. */
  while (targetTime - refTime >= SCH_RM_MAX_SPAN)
  {
    targetTime += BB_US_TO_BB_TICKS(schRmCb.rsvn[handle].interUsec);
  }

  /* 0 < targetTime <= interval */
  while (targetTime - refTime > BB_US_TO_BB_TICKS(schRmCb.rsvn[handle].interUsec))
  {
    targetTime -= BB_US_TO_BB_TICKS(schRmCb.rsvn[handle].interUsec);
  }

  LL_TRACE_WARN1("                    offsUsec = %u", (targetTime - refTime));
  return BB_TICKS_TO_US(targetTime - refTime);
}
