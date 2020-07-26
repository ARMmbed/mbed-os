/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Hardware audio codec interface file.
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

#ifndef PAL_CODEC_H
#define PAL_CODEC_H

#include "wsf_types.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup PAL_CODEC
 *  \{
 */

/**************************************************************************************************'
  Data Types
**************************************************************************************************/

/*! \brief      Audio data path direction. */
typedef enum
{
  PAL_CODEC_DIR_INPUT   = 0,    /*!< Input data path. */
  PAL_CODEC_DIR_OUTPUT  = 1     /*!< Output data path. */
} PalAudioDir_t;

/*! \brief      Audio Channel. */
typedef enum
{
  PAL_CODEC_CH_LEFT     = 0,    /*!< Left channel. */
  PAL_CODEC_CH_RIGHT    = 1,    /*!< Right channel. */
  AUDIO_NUM_CH                  /*!< Right channel. */
} PalAudioChan_t;

/*! \brief      Audio Channel mask. */
enum
{
  PAL_CODEC_CH_LEFT_BIT  = (1 << PAL_CODEC_CH_LEFT),
  PAL_CODEC_CH_RIGHT_BIT = (1 << PAL_CODEC_CH_RIGHT)
};

/*! \brief      Standard codec info block. */
typedef struct
{
  uint8_t codecId;              /*!< Codec ID. */
} AudioStdCodecInfo_t;

/*! \brief      VS codec info block. */
typedef struct
{
  uint16_t compId;              /*!< Company ID. */
  uint16_t codecId;             /*!< Codec ID. */
} AudioVsCodecInfo_t;

/*! \brief      Buffer available call signature. */
typedef void (*PalCodecDataReady_t)(uint16_t id);

/*! \brief      Codec. */
typedef struct
{
  PalAudioDir_t dir;            /*!< Stream data direction. */
  uint16_t      chMask;         /*!< Audio channel mask. */
  uint32_t      intervalUsec;   /*!< SDU interval in microseconds. */
  uint32_t      pktCtr;         /*!< Initial packet counter value. */
  PalCodecDataReady_t rdyCback; /*!< Data ready callback. */
} PalCodecSreamParam_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Codec Information */
void PalCodecReadLocalSupportedCodecs(uint8_t *pNumStd, AudioStdCodecInfo_t stdCodecs[],
                                      uint8_t *pNumVs, AudioVsCodecInfo_t vsCodecs[]);
bool_t PalCodecReadLocalSupportedCodecCapabilities(uint8_t codingFmt, uint16_t compId, uint16_t vsCodecId, PalAudioDir_t dir);
bool_t PalCodecReadLocalSupportedControllerDelay(uint8_t codingFmt, uint16_t compId, uint16_t vsCodecId, PalAudioDir_t dir,
                                                 uint32_t *pMinDly, uint32_t *pMaxDly);
bool_t PalCodecConfigureDataPath(PalAudioDir_t dir, uint8_t dataPathId);

/* Audio Amplifier */
void PalCodecAmpInit(void);
uint8_t PalCodecAmpGetVol(void);
void PalCodecAmpVolumeUp(void);
void PalCodecAmpVolumeDown(void);
void PalCodecAmpMute(void);
void PalCodecAmpUnmute(void);

/* Data Path */
void PalCodecDataInit(void);
bool_t PalCodecDataStartStream(uint16_t id, PalCodecSreamParam_t *pParam);
void PalCodecDataStopStream(uint16_t id);
uint16_t PalCodecDataStreamIn(uint16_t id, uint8_t *pBuf, uint16_t len, uint32_t *pPktCtr);
void PalCodecDataStreamOut(uint16_t id, const uint8_t *pBuf, uint16_t len, uint32_t pktCtr);

/*! \} */    /* PAL_CODEC */

#ifdef __cplusplus
};
#endif

#endif /* PAL_CODEC_H */
