/***************************************************************************//**
 * @file clocking.h
 * @brief Clock selection calculations
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device_peripherals.h"

#if( CORE_CLOCK_SOURCE == HFXO)
# define REFERENCE_FREQUENCY HFXO_FREQUENCY
#elif( CORE_CLOCK_SOURCE == HFRCO)

# if defined _CMU_HFRCOCTRL_BAND_MASK
#  if( HFRCO_FREQUENCY == _CMU_HFRCOCTRL_BAND_1MHZ)
#   define REFERENCE_FREQUENCY 1000000
#  elif(HFRCO_FREQUENCY == _CMU_HFRCOCTRL_BAND_7MHZ)
#   define REFERENCE_FREQUENCY 7000000
#  elif(HFRCO_FREQUENCY == _CMU_HFRCOCTRL_BAND_11MHZ)
#   define REFERENCE_FREQUENCY 7000000
#  elif(HFRCO_FREQUENCY == _CMU_HFRCOCTRL_BAND_14MHZ)
#   define REFERENCE_FREQUENCY 14000000
#  elif(HFRCO_FREQUENCY == _CMU_HFRCOCTRL_BAND_21MHZ)
#   define REFERENCE_FREQUENCY 21000000
#  elif(HFRCO_FREQUENCY == _CMU_HFRCOCTRL_BAND_28MHZ)
#   define REFERENCE_FREQUENCY 28000000
#  else
#   define REFERENCE_FREQUENCY 14000000
#  endif
# elif defined _CMU_HFRCOCTRL_FREQRANGE_MASK
#  define REFERENCE_FREQUENCY HFRCO_FREQUENCY
# else
#  error "HFRCO frequency not defined"
# endif
#endif

#if ( LOW_ENERGY_CLOCK_SOURCE == LFXO )
# define LEUART_USING_LFXO
# if ( (defined(CMU_CTRL_HFLE) || defined(CMU_CTRL_WSHFLE) ) && (REFERENCE_FREQUENCY > 24000000) )
#  define LEUART_HF_REF_FREQ (REFERENCE_FREQUENCY / 4)
# else
#  define LEUART_HF_REF_FREQ (REFERENCE_FREQUENCY / 2)
# endif
# define LEUART_LF_REF_FREQ LFXO_FREQUENCY
#else
# if ( (defined(CMU_CTRL_HFLE) || defined(CMU_CTRL_WSHFLE) ) && (REFERENCE_FREQUENCY > 24000000) )
#  define LEUART_REF_FREQ (REFERENCE_FREQUENCY / 4)
# else
#  define LEUART_REF_FREQ (REFERENCE_FREQUENCY / 2)
# endif
#endif

