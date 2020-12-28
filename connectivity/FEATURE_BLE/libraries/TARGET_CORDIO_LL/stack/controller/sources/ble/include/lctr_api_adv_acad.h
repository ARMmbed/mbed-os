/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller ACAD definitions.
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

#ifndef LCTR_API_ADV_ACAD_H
#define LCTR_API_ADV_ACAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ACAD header */
typedef struct
{
  uint8_t       state;              /*!< State of ACAD. */
  uint8_t       opcode;             /*!< Opcode of ACAD. */
  uint8_t       len;                /*!< Length of ACAD data field. */
} LctrAcadHdr_t;

/*! \brief      ACAD data field for channel map update */
typedef struct
{
  LctrAcadHdr_t hdr;                /*!< ACAD header. */
  uint64_t      chanMask;           /*!< Channel mask for the update. */
  uint16_t      instant;            /*!< Instant for the update. */
} LctrAcadChanMapUpd_t;

/*! \brief      ACAD data field for channel map update */
typedef struct
{
  LctrAcadHdr_t hdr;                /*!< ACAD header. */
  /* Reference values. */
  uint32_t      bigAnchorPoint;     /*!< BIG Anchor Point time. */
  /* ACAD fields. */
  uint16_t      bigOffs;            /*!< BIG offset. */
  uint8_t       bigOffsUnits;       /*!< BIG offset units. */
  uint16_t      isoInter;           /*!< ISO interval in units of 1.25ms. */
  uint8_t       numBis;             /*!< Number of BISs. */
  uint8_t       nse;                /*!< Number of subevents. */
  uint8_t       bn;                 /*!< Burst number. */
  uint32_t      subEvtInterUsec;    /*!< Subevent interval in microseconds. */
  uint8_t       pto;                /*!< Pre-transmission offset. */
  uint32_t      bisSpaceUsec;       /*!< BIS spacing in microseconds. */
  uint8_t       irc;                /*!< Immediate repetition count. */
  uint16_t      maxPdu;             /*!< Maximum PDU size. */
  uint32_t      seedAccAddr;        /*!< Seed access address. */
  uint32_t      sduInterUsec;       /*!< SDU interval in microseconds. */
  uint16_t      maxSdu;             /*!< Maximum SDU size. */
  uint16_t      baseCrcInit;        /*!< Base CRC init. */
  uint64_t      chanMap;            /*!< Channel map. */
  uint8_t       phy;                /*!< PHY used by BIG. */
  uint64_t      bisPldCtr;          /*!< BIS payload counter. */
  uint8_t       framing;            /*!< BIG carries framed or unframed data. */
  uint8_t       encrypt;            /*!< Encryption mode of the BISes in the BIG. */
  uint8_t       giv[LL_GIV_LEN];    /*!< GIV. */
  uint8_t       gskd[LL_GSKD_LEN];  /*!< GSKD. */
} LctrAcadBigInfo_t;

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_ACAD_H */
