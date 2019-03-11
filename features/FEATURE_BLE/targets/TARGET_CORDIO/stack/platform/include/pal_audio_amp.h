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
 *  \brief Audio amplifier driver definition.
 */
/*************************************************************************************************/

#ifndef PAL_AUDIO_AMP_H
#define PAL_AUDIO_AMP_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Operational states. */
typedef enum
{
  PAL_AUDIO_AMP_STATE_UNINIT = 0,   /*!< Uninitialized state. */
  PAL_AUDIO_AMP_STATE_ERROR  = 0,   /*!< Error state. */
  PAL_AUDIO_AMP_STATE_READY,        /*!< Ready state. */
  PAL_AUDIO_AMP_STATE_BUSY          /*!< BUSY state. */
} PalAudioAmpState_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalAudioAmpInit(void);
void PalAudioAmpDeInit(void);

/* Control and Status */
PalAudioAmpState_t PalAudioAmpGetState(void);
uint8_t PalAudioAmpGetVol(void);
void PalAudioAmpSetVolUp(void);
void PalAudioAmpSetVolDown(void);
void PalAudioAmpMute(void);
void PalAudioAmpUnmute(void);

#ifdef __cplusplus
};
#endif

#endif /* PAL_AUDIO_AMP_H */

