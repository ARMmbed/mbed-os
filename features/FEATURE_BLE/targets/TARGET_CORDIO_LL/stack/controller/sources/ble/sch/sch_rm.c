/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Operation list maintenance implementation file.
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
#define SCH_RM_MAX_RSVN_BINS     ((SCH_RM_MAX_RSVN + (SCH_RM_MAX_RSVN_PREF_PER - 1)) / SCH_RM_MAX_RSVN_PREF_PER)

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Scheduler resource manager control block. */
SchRmCb_t schRmCb;

/*************************************************************************************************/
/*!
 *  \brief      Select the preferred interval.
 *
 *  \param      minUsec     Minimum interval in microseconds.
 *  \param      maxUsec     Maximum interval in microseconds.
 *  \param      exclude     Handle to exclude.
 *
 *  \return     Preferred interval in microseconds.
 */
/*************************************************************************************************/
static uint32_t schRmSelectPreferredIntervalUsec(uint32_t minUsec, uint32_t maxUsec, uint8_t exclude)
{
  /*** Select lower bound rounded to preferred interval ***/

  uint32_t numMinInter = SCH_RM_DIV_PREF_PER(minUsec);
  uint32_t prefMinInterUsec = numMinInter * SCH_RM_PREF_PER_USEC;

  if (prefMinInterUsec < minUsec)
  {
    /* Round up to next periodic interval. */
    prefMinInterUsec = (numMinInter + 1) * SCH_RM_PREF_PER_USEC;
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
    if ((intervUsec[i] >= prefMinInterUsec) &&
        (intervUsec[i] <= maxUsec))
    {
      return intervUsec[i];
    }
  }

  /*** Limit upper bound ***/

  if (prefMinInterUsec > maxUsec)
  {
    /* Limit to max value. */
    prefMinInterUsec = maxUsec;
  }

  return prefMinInterUsec;
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
  uint8_t numRsvnPerInter[SCH_RM_MAX_RSVN_BINS] = { 0 };
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
    if (binIdx < SCH_RM_MAX_RSVN_BINS)
    {
      numRsvnPerInter[binIdx]++;
      maxBinIdx = WSF_MAX(binIdx, maxBinIdx);
    }
    /* Else interval is too large to be significant. */
  }

  /* Check capacity. */
  unsigned int numRsvnLimit = 0;
  unsigned int numRsvnSum = 0;
  bool_t isMaxCap = FALSE;
  for (unsigned int i = 0; i < (maxBinIdx + 1); i++)
  {
    if (!isMaxCap)
    {
      numRsvnLimit += SCH_RM_MAX_RSVN_PREF_PER;
      numRsvnSum += numRsvnPerInter[i];

      if (numRsvnSum > numRsvnLimit)
      {
        /* Capacity exceeded. */
        return FALSE;
      }

      if (numRsvnSum == numRsvnLimit)
      {
        isMaxCap = TRUE;
      }
    }
    else
    {
      if (numRsvnPerInter[i])
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
 *  \brief      Find reservations with equal intervals.
 *
 *  \param      rsvn            Sorted reservation sequence table.
 *  \param      numRsvn         Number of reservations in sequence table.
 *  \param      handle          Target reservation handle.
 *  \param      pSelectMask     Bitmask of selected reservations.
 *  \param      pNumSelected    Number of selected reservations.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schRmFindEqualIntervals(schRmRsvn_t rsvn[], uint8_t numRsvn, uint8_t handle,
                                    uint32_t *pSelectMask, uint8_t *pNumSelected)
{
  WSF_ASSERT(numRsvn > 0);

  const uint32_t inter = schRmCb.rsvnInterUsec[handle];

  uint32_t selectMask = 0;
  uint8_t numSelected = 0;

  for (unsigned int i = 0; i < numRsvn; i++)
  {
    if (rsvn[i].interUsec == inter)
    {
      selectMask |= 1 << i;
      numSelected++;
    }
  }

  /* Return values. */
  *pSelectMask = selectMask;
  *pNumSelected = numSelected;
}

/*************************************************************************************************/
/*!
 *  \brief      Find reservations with the largest intervals.
 *
 *  \param      rsvn        Sorted reservation sequence table.
 *  \param      numRsvn     Number of reservations in sequence table.
 *  \param      pSelectMask Bitmask of selected reservations.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schRmFindLargestIntervals(schRmRsvn_t rsvn[], uint8_t numRsvn, uint32_t *pSelectMask)
{
  WSF_ASSERT(numRsvn > 0);

  /* Initially the largest interval is first reservation. */
  uint32_t selectMask = 1 << 0;
  uint32_t lgInterUsec = rsvn[0].interUsec;

  /* Start comparison with the second reservation. */
  for (unsigned int i = 1; i < numRsvn; i++)
  {
    if (rsvn[i].interUsec < lgInterUsec)
    {
      /* Skip this reservation. */
      continue;
    }

    if (rsvn[i].interUsec > lgInterUsec)
    {
      /* New largest interval found. */
      selectMask = 0;
      lgInterUsec = rsvn[i].interUsec;
    }

    selectMask |= 1 << i;
  }

  /* Return values. */
  *pSelectMask = selectMask;
}

/*************************************************************************************************/
/*!
 *  \brief      Find largest gap between start times of reservation with equal intervals.
 *
 *  \param      rsvn        Sorted reservation sequence table.
 *  \param      numRsvn     Number of reservations in sequence table.
 *  \param      selectMask  Bitmask of equal interval reservations.
 *
 *  \return     Offset in microseconds of the center of the largest gap.
 */
/*************************************************************************************************/
static uint32_t schRmFindLargestGapOffsetUsec(schRmRsvn_t rsvn[], uint8_t numRsvn, uint32_t selectMask)
{
  WSF_ASSERT(numRsvn > 0);
  WSF_ASSERT(selectMask != 0);

  schRmRsvn_t *pFirstRsvn = NULL;
  schRmRsvn_t *pLastRsvn = NULL;
  schRmRsvn_t *pLgGapEnd = NULL;
  uint32_t lgGap = 0;
  uint32_t gapStartOffsUsec = 0;

  for (unsigned int i = 0; i < numRsvn; i++)
  {
    if ((selectMask & (1 << i)) == 0)
    {
      continue;
    }

    if (!pFirstRsvn)
    {
      pFirstRsvn = &rsvn[i];
    }

    uint32_t gap = pLastRsvn ? (rsvn[i].offsUsec - pLastRsvn->offsUsec) : rsvn[i].offsUsec;

    /* Favor latest offset. */
    if (lgGap <= gap)
    {
      lgGap = gap;
      if (pLgGapEnd)
      {
        gapStartOffsUsec = pLgGapEnd->offsUsec;
      }
      pLgGapEnd = &rsvn[i];
    }

    pLastRsvn = &rsvn[i];
  }

  /* Check end of sequence gap. */
  if (pFirstRsvn)       /* pLastRsvn valid if pFirstRsvn valid */
  {
    uint32_t firstNextOffsUsec = pFirstRsvn->offsUsec + pFirstRsvn->interUsec;
    WSF_ASSERT(firstNextOffsUsec > pLastRsvn->offsUsec);

    if (firstNextOffsUsec > pLastRsvn->offsUsec)
    {
      uint32_t gap = firstNextOffsUsec - pLastRsvn->offsUsec;

      /* Favor earliest offset. */
      if (lgGap < gap)
      {
        lgGap = gap;
        gapStartOffsUsec = pLastRsvn->offsUsec;
      }
    }
  }

  return gapStartOffsUsec + (lgGap >> 1);
}

/*************************************************************************************************/
/*!
 *  \brief      Select initial offset considering existing reservations.
 *
 *  \param      rsvn        Sorted reservation sequence table.
 *  \param      numRsvn     Number of reservations in sequence table.
 *  \param      selectMask  Bitmask of selected reservations in rsvn[].
 *  \param      numSelected Number of selected reservations in rsvn[].
 *
 *  \return     Suggested offset in microseconds.
 */
/*************************************************************************************************/
static uint32_t schRmSelectInitOffset(schRmRsvn_t rsvn[], uint8_t numRsvn, uint32_t selectMask, uint8_t numSelected)
{
  uint32_t interUsec = 0;

  for (unsigned int i = 0; i < numRsvn; i++)
  {
    if ((selectMask & (1 << i)) == 0)
    {
      /* Reservation not selected. */
      continue;
    }

    interUsec = rsvn[i].interUsec;
    break;
  }

  WSF_ASSERT(interUsec != 0);

  uint32_t offsetUsec = schRmFindLargestGapOffsetUsec(rsvn, numRsvn, selectMask);

  if (offsetUsec > interUsec)
  {
    /* Ensure offset is within bounds. */
    offsetUsec -= interUsec;
  }

  return offsetUsec;
}

/*************************************************************************************************/
/*!
 *  \brief      Build reservation table.
 *
 *  \param      rsvn        Target reservation sequence table.
 *  \param      rsvnOffs    List of reservation offsets indexed by handle.
 *  \param      exclude     Handle of reservation to exclude.
 *
 *  \return     Number of valid reservation entries.
 */
/*************************************************************************************************/
uint8_t schRmBuildReservationTable(schRmRsvn_t rsvn[], uint32_t rsvnOffs[], uint8_t exclude)
{
  uint8_t numRsvn = 0;

  for (unsigned int i = 0; i < SCH_RM_MAX_RSVN; i++)
  {
    if ((schRmCb.rsvnInterUsec[i]) &&
        (i != exclude))
    {
      rsvn[numRsvn].handle = i;
      rsvn[numRsvn].offsUsec = rsvnOffs[i];
      rsvn[numRsvn].interUsec = schRmCb.rsvnInterUsec[i];

      numRsvn++;
    }
  }

  schRmSortRsvnList(rsvn, numRsvn);

  return numRsvn;
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
 *  \brief      In-place sorting of a reservation description list.
 *
 *  \param      rsvn        Reservation descriptor table.
 *  \param      numRsvn     Number of reservation descriptors in rsvn[].
 *
 *  \return     None.
 */
/*************************************************************************************************/
void schRmSortRsvnList(schRmRsvn_t rsvn[], uint8_t numRsvn)
{
  if (numRsvn <= 1)
  {
    return;
  }

  unsigned int numUnsorted = numRsvn;

  do
  {
    unsigned int swapIdx = 0;

    for (unsigned int i = 1; i < numUnsorted; i++)
    {
      if (rsvn[i-1].offsUsec > rsvn[i].offsUsec)
      {
        schRmRsvn_t tmp;
        memcpy(&tmp, &rsvn[i-1], sizeof(schRmRsvn_t));
        memcpy(&rsvn[i-1], &rsvn[i], sizeof(schRmRsvn_t));
        memcpy(&rsvn[i], &tmp, sizeof(schRmRsvn_t));
        swapIdx = i;
      }
    }

    /* Reservations after swapIdx are sorted. */
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
 *  \brief      Validate and commit a new reservation against collision.
 *
 *  \param      handle      Client defined reservation handle.
 *  \param      minUsec     Minimum interval in microseconds.
 *  \param      maxUsec     Maximum interval in microseconds.
 *  \param      durUsec     Duration of the connection in microseconds.
 *  \param      pInterUsec  Actual interval return value in microseconds.
 *
 *  \return     TRUE if reservation available, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t SchRmAdd(uint8_t handle, uint32_t minUsec, uint32_t maxUsec, uint32_t durUsec, uint32_t *pInterUsec)
{
  WSF_ASSERT(handle < SCH_RM_MAX_RSVN);
  WSF_ASSERT(maxUsec >= minUsec);
  WSF_ASSERT(durUsec <= (SCH_RM_PREF_PER_USEC / SCH_RM_MAX_RSVN_PREF_PER));

  /*** Select reservation's preferred interval. ***/

  uint32_t prefInterUsec = schRmSelectPreferredIntervalUsec(minUsec, maxUsec, handle);

  /*** Check reservation capacity. ***/

  if (!schRmCheckRsvnCapacity(handle, prefInterUsec))
  {
    return FALSE;
  }

  /*** Commit reservation. ***/

  schRmCb.rsvnInterUsec[handle] = prefInterUsec;
  schRmCb.numRsvn++;
  *pInterUsec = prefInterUsec;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Start update to a reservation.
 *
 *  \param      handle      Client defined reservation handle.
 *  \param      minUsec     Minimum interval in microseconds.
 *  \param      maxUsec     Maximum interval in microseconds.
 *  \param      durUsec     Duration of the connection in microseconds.
 *  \param      pInterUsec  Actual interval return value in microseconds.
 *
 *  \return     TRUE if resource is can be updated, FALSE otherwise.
 *
 *  \note       For BLE, connection belonging to handle must be a master only.
 */
/*************************************************************************************************/
bool_t SchRmStartUpdate(uint8_t handle, uint32_t minUsec, uint32_t maxUsec, uint32_t durUsec, uint32_t *pInterUsec)
{
  WSF_ASSERT(handle < SCH_RM_MAX_RSVN);
  WSF_ASSERT(maxUsec >= minUsec);
  WSF_ASSERT(durUsec <= (SCH_RM_PREF_PER_USEC / SCH_RM_MAX_RSVN_PREF_PER));

  /*** Select reservation's preferred interval. ***/

  uint32_t prefInterUsec = schRmSelectPreferredIntervalUsec(minUsec, maxUsec, handle);

  /*** Check reservation capacity. ***/

  if (!schRmCheckRsvnCapacity(handle, prefInterUsec))
  {
    return FALSE;
  }

  /*** Commit reservation. ***/

  schRmCb.rsvnInterUsec[handle] = prefInterUsec;
  *pInterUsec = prefInterUsec;

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
 *  \param      rsvnOffs    List of reservation offsets indexed by handle.
 *  \param      defOffsUsec Default offset in microseconds.
 *  \param      handle      Reservation handle.
 *
 *  \return     Offset in microseconds.
 */
/*************************************************************************************************/
uint32_t SchRmGetOffsetUsec(uint32_t rsvnOffs[], uint32_t defOffsUsec, uint8_t handle)
{
  WSF_ASSERT(schRmCb.numRsvn);

  if (schRmCb.numRsvn <= 1)
  {
    /* No competing reservations. */
    return defOffsUsec;
  }

  schRmRsvn_t rsvn[SCH_RM_MAX_RSVN];
  uint8_t numRsvn;
  uint8_t numSelected;
  uint32_t selectMask;
  numRsvn = schRmBuildReservationTable(rsvn, rsvnOffs, handle);

  /* Attempt alignment with existing reservations at equal intervals. */
  schRmFindEqualIntervals(rsvn, numRsvn, handle, &selectMask, &numSelected);
  if (numSelected)
  {
    return schRmSelectInitOffset(rsvn, numRsvn, selectMask, numSelected);
  }

  /* Else align with largest interval reservations. */
  schRmFindLargestIntervals(rsvn, numRsvn, &selectMask);
  uint32_t lgGapOffsUsec = schRmFindLargestGapOffsetUsec(rsvn, numRsvn, selectMask);

  /* Ensure offset is within bounds. */
  while (lgGapOffsUsec > schRmCb.rsvnInterUsec[handle])
  {
    lgGapOffsUsec = lgGapOffsUsec >> 1;
  }

  return lgGapOffsUsec;
}
