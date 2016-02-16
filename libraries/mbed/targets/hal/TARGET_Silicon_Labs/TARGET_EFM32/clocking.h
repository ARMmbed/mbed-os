/***************************************************************************//**
 * @file clocking.h
 * @brief Clock selection calculations
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
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

