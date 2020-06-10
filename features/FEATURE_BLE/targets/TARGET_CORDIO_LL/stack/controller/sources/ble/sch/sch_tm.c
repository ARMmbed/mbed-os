/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE topology manager implementation file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#include "sch_int_tm.h"
#include "sch_int_rm.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include <string.h>

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Topology link node descriptor. */
typedef struct
{
  uint32_t tBegin;      /*!< Begin time of topology link node. */
  uint32_t tEnd;        /*!< End time of topology link node. */
} schTmNode_t;

/*! \brief      Topology link node control block. */
typedef struct
{
  schTmNode_t tnode[SCH_TM_MAX_LINK + 1];   /*!< Information for each topology node. */
} schTmNodeCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Topology manager control block. */
SchTmCb_t schTmCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize the topology manager.
 */
/*************************************************************************************************/
void SchTmInit(void)
{
  memset(&schTmCb, 0, sizeof(schTmCb));
}

/*************************************************************************************************/
/*!
 *  \brief      Add a new topology link.
 *
 *  \param      handle      Client defined topology handle.
 *  \param      interUsec   Interval in microseconds.
 *  \param      durUsec     Duration of the topology link in microseconds.
 *  \param      movable     Movable.
 *  \param      refTimeCb   Callback function to get reference time.
 */
/*************************************************************************************************/
void SchTmAdd(uint8_t handle, uint32_t interUsec, uint32_t durUsec, bool_t movable, GetTopRefTimeCb_t refTimeCb)
{
  WSF_ASSERT(handle < SCH_TM_MAX_LINK);

  schTmCb.tlink[handle].enabled = TRUE;
  schTmCb.tlink[handle].movable = movable;
  schTmCb.tlink[handle].interUsec = interUsec;
  schTmCb.tlink[handle].durUsec = durUsec;
  schTmCb.tlink[handle].refTimeCb = refTimeCb;

  LL_TRACE_INFO2("Topology Add, handle=%u, interval_ms=%u", handle, LL_MATH_DIV_10E3(interUsec));
  LL_TRACE_INFO1("Topology Add,            durUsec=%u", durUsec);
}

/*************************************************************************************************/
/*!
 *  \brief      Remove an existing topology link.
 *
 *  \param      handle      Client defined topology handle.
 */
/*************************************************************************************************/
void SchTmRemove(uint8_t handle)
{
  WSF_ASSERT(handle < SCH_TM_MAX_LINK);

  schTmCb.tlink[handle].enabled = FALSE;
  LL_TRACE_INFO1("Topology Remove, handle=%u", handle);
}

/*************************************************************************************************/
/*!
 *  \brief      Check if given timing parameters have conflict with any topology links.
 *
 *  \param      refBegin      Reference time in microseconds.
 *  \param      interUsec     Interval in microseconds.
 *  \param      durUsec       Duration in microseconds.
 *
 *  \return     TRUE if conflict is found with any of topology links.
 */
/*************************************************************************************************/
bool_t SchTmCheckConflict(uint32_t refBegin, uint32_t interUsec, uint32_t durUsec)
{
  uint32_t refEnd = refBegin + durUsec + BbGetSchSetupDelayUs();    /* End time of the link to be compared. */
  uint32_t linkBegin, linkEnd;  /* Begin and end time of topology link. */
  uint8_t i;

  for (i = 0; i < SCH_TM_MAX_LINK; i++)
  {
    if ((schTmCb.tlink[i].enabled == TRUE) && schTmCb.tlink[i].refTimeCb)
    {
      /* First check if the links have common interval. */
      if (((interUsec > schTmCb.tlink[i].interUsec) && (schRmIntCalculateDepth(interUsec, schTmCb.tlink[i].interUsec) == 0)) ||
          ((interUsec < schTmCb.tlink[i].interUsec) && (schRmIntCalculateDepth(schTmCb.tlink[i].interUsec, interUsec) == 0)))
      {
        continue;
      }

      linkBegin = schTmCb.tlink[i].refTimeCb(i);

      /* Start from the time when the TM link is in the past from the reference time. */
      while (SCH_TM_IS_IN_FUTURE(linkBegin, refBegin))
      {
        linkBegin -= schTmCb.tlink[i].interUsec;
      }

      linkEnd = linkBegin + schTmCb.tlink[i].durUsec + BbGetSchSetupDelayUs();

      /* Now, check conflicts between TM link and the reference. */
      while (!SCH_TM_IS_IN_FUTURE(linkBegin, refEnd))
      {
        /* Found conflict. */
        if (!SCH_TM_IS_IN_PAST(linkEnd, refBegin))
        {
          LL_TRACE_WARN1("SchTmCheckConflict, ======>>> Found conflict with TM link %u ", i);
          return TRUE;
        }

        /* Use the smaller interval to check conflicts. */
        linkBegin += (interUsec < schTmCb.tlink[i].interUsec) ? interUsec : schTmCb.tlink[i].interUsec;
        linkEnd = linkBegin + schTmCb.tlink[i].durUsec + BbGetSchSetupDelayUs();
      }
    }
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Find the best location to place the first anchor point.
 *
 *  \param      refTime       Reference time in microseconds.
 *  \param      defOffsUsec   Default offset in microseconds.
 *  \param      interUsec     Interval in microseconds.
 *  \param      durUsec       Duration in microseconds.
 *
 *  \return     Anchor point offset from the reference time.
 */
/*************************************************************************************************/
uint32_t SchTmGetFirstAnchor(uint32_t refTime, uint32_t defOffsUsec, uint32_t interUsec, uint32_t durUsec)
{
  schTmNodeCb_t schTmNodeCb;
  uint8_t i, numNode, refLink, curLink, bestIndex;
  uint32_t lastEnd, curBegin, rangeBegin, rangeEnd;
  uint32_t nextBegin, nextEnd;  /* Begin and end time of the next node to add. */
  uint32_t gap, biggestGap, commInterUsec;
  bool_t done[SCH_TM_MAX_LINK];

  numNode = 0;
  commInterUsec = interUsec;
  memset(done, FALSE, SCH_TM_MAX_LINK);

  /* Construct schTmNodeCb from schTmCb. */
  /* When there are 3 links(A,B,C), the order in schTmNodeCb will be A0 - B0 - C0 - A1.*/
  for (i = 0; i < SCH_TM_MAX_LINK; i++)
  {
    /* Find the first link which will be used as a reference time. */
    if ((schTmCb.tlink[i].enabled == TRUE) && schTmCb.tlink[i].refTimeCb)
    {
      /* Check if the links have common interval. */
      if (((interUsec > schTmCb.tlink[i].interUsec) && (schRmIntCalculateDepth(interUsec, schTmCb.tlink[i].interUsec) == 0)) ||
          ((interUsec < schTmCb.tlink[i].interUsec) && (schRmIntCalculateDepth(schTmCb.tlink[i].interUsec, interUsec) == 0)))
      {
        done[i] = TRUE;
      }
      else
      {
        /* Use smaller common interval. */
        commInterUsec = WSF_MIN(commInterUsec, schTmCb.tlink[i].interUsec);

        if (numNode == 0)
        {
          refLink = i;
          schTmNodeCb.tnode[numNode].tBegin = schTmCb.tlink[i].refTimeCb(i);
          schTmNodeCb.tnode[numNode].tEnd = schTmNodeCb.tnode[numNode].tBegin + schTmCb.tlink[i].durUsec;

          rangeBegin = schTmNodeCb.tnode[numNode].tBegin;
          done[i] = TRUE;
          numNode++;
        }
      }
    }
    else
    {
      done[i] = TRUE;
    }
  }

  if (numNode == 0)
  {
    /* Nothing to do. */
    return defOffsUsec;
  }

  rangeEnd = rangeBegin + commInterUsec;

  /* Find next ones. */
  while (TRUE)
  {
    lastEnd = schTmNodeCb.tnode[numNode - 1].tEnd;
    curLink = 0xFF;   /* invalid */
    nextBegin = 0;    /* invalid */

    /* Find node which is closest from nextBegin. */
    for (i = 0; i < SCH_TM_MAX_LINK; i++)
    {
      if (done[i] == FALSE)
      {
        /* curBegin should be between rangeBegin and rangeEnd. */
        curBegin = schTmCb.tlink[i].refTimeCb(i);
        while (SCH_TM_IS_IN_PAST(curBegin, rangeBegin))
        {
          curBegin += commInterUsec;
        }

        while (SCH_TM_IS_IN_FUTURE(curBegin, rangeEnd))
        {
          curBegin -= commInterUsec;
        }

        if (curLink == 0xFF)
        {
          curLink = i;
          nextBegin = curBegin;
        }
        else
        {
          if (SCH_TM_IS_IN_PAST(curBegin, nextBegin))
          {
            curLink = i;
            nextBegin = curBegin;
          }
        }
      }
    }

    if (curLink != 0xFF)
    {
      done[curLink] = TRUE;
      nextEnd = nextBegin + schTmCb.tlink[curLink].durUsec;

      /* Check if we can merge two nodes when they overlap. */
      if (SCH_TM_IS_IN_FUTURE(nextBegin, lastEnd))
      {
        schTmNodeCb.tnode[numNode].tBegin = nextBegin;
        schTmNodeCb.tnode[numNode].tEnd = nextBegin + schTmCb.tlink[curLink].durUsec;
        numNode++;
      }
      else
      {
        /* Overlap. Merge nodes. */
        schTmNodeCb.tnode[numNode - 1].tEnd = SCH_TM_IS_IN_FUTURE(nextEnd, lastEnd) ? nextEnd : lastEnd;
      }
    }
    else
    {
      /* Finished sorting. */
      break;
    }
  }

  /* Add A1(next instance of link A) at the end. */
  if (numNode > 0)
  {
    lastEnd = schTmNodeCb.tnode[numNode - 1].tEnd;
    nextBegin = schTmNodeCb.tnode[0].tBegin + commInterUsec;
    nextEnd = nextBegin + schTmCb.tlink[refLink].durUsec;

    /* Check if we can merge two nodes when they overlap. */
    if (SCH_TM_IS_IN_FUTURE(nextBegin, lastEnd))
    {
      schTmNodeCb.tnode[numNode].tBegin = nextBegin;
      schTmNodeCb.tnode[numNode].tEnd = nextBegin + schTmCb.tlink[refLink].durUsec;
      numNode++;
    }
    else
    {
      /* Overlap. Merge nodes. */
      schTmNodeCb.tnode[numNode - 1].tEnd = SCH_TM_IS_IN_FUTURE(nextEnd, lastEnd) ? nextEnd : lastEnd;
    }
  }

  WSF_ASSERT(numNode >= 2);

  biggestGap = 0;
  bestIndex = 0;

  /* Find biggest gap between nodes. */
  for (i = 0; i < (numNode - 1); i++)
  {
    gap = schTmNodeCb.tnode[i + 1].tBegin - schTmNodeCb.tnode[i].tEnd;

    if (gap > biggestGap)
    {
      biggestGap = gap;
      bestIndex = i;
    }
  }

  uint32_t anchorTime = schTmNodeCb.tnode[bestIndex].tEnd + (biggestGap >> 1) - (durUsec >> 1);

  while (SCH_TM_IS_IN_PAST(anchorTime, refTime))
  {
    anchorTime += commInterUsec;
  }

  while (SCH_TM_IS_IN_FUTURE(anchorTime, (refTime + interUsec)))
  {
    anchorTime -= commInterUsec;
  }

  LL_TRACE_INFO1("Topology SchTmGetFirstAnchor, refTime=%u", refTime);
  LL_TRACE_INFO1("Topology SchTmGetFirstAnchor, anchorTime=%u", anchorTime);

  /* Return value should be 0 ~ defOffsecUsec. */
  return (anchorTime - refTime);
}
