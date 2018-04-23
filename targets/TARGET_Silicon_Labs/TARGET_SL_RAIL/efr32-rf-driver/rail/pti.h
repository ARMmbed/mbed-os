/***************************************************************************//**
 * @file pti.h
 * @brief This header file contains information for working with the packet
 * trace APIs.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#ifndef __RADIO_PTI_H
#define __RADIO_PTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "em_gpio.h"

#include "rail_chip_specific.h"

// Once this is a RAIL API this code can be removed as rail-types.h does this
#ifndef RAIL_ENUM
#ifdef DOXYGEN_SHOULD_SKIP_THIS
/// The RAIL library does not use actual enums because the ARM EABI leaves their
/// size ambiguous. This ambiguity causes problems if the application is built
/// with different flags than the library. To work around this we use uint8_t
/// typedefs in compiled code for all enums. For documentation purposes this is
/// converted to an actual enum since it's much easier to read in Doxygen.
#define RAIL_ENUM(name) enum name
#else
/// Define used for the actual RAIL library which sets each enum to a uint8_t
/// typedef and creates a named enum structure for the enumeration values.
#define RAIL_ENUM(name) typedef uint8_t name; enum name##_enum
// For debugging use the following define to turn this back into a proper enum
// #define RAIL_ENUM(name) typedef enum name##_enum name; enum name##_enum
#endif
#endif

/***************************************************************************//**
 * @addtogroup Chip_Specific
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EFR32xG1x_PTI
 * @{
 * @brief EFR32 Packet Trace Interface (PTI) setup and configuration
 ******************************************************************************/

/*************************  FUNCTION PROTOTYPES   *****************************/

/**
 * Initialize the PTI interface
 *
 * @param ptiInit The structure that defines what pins and modes to use for
 * packet trace.
 *
 * This API will initialize the packet trace interface. It allows you to
 * configure what mode and pins to use for packet trace output. You must call
 * this API either before RAIL initialization or before an explicit call to
 * \ref PTI_Enable() to properly initialize PTI.
 */
RAIL_Status_t PTI_Config(const RAIL_PtiConfig_t *config);

/**
 * Enable or disable the PTI interface
 *
 * This API will turn on or off the packet trace interface (PTI). By default
 * this is turned on already during init time. Note that you must call \ref
 * RADIO_PTI_Init() with a valid initialization structure before calling this
 * API or PTI will not actually turn on.
 */
RAIL_Status_t PTI_Enable(bool enable);

/**
 * Get the current state of the PTI
 *
 * This function will return a pointer to a copy of the PTI state. If you
 * actually want to change the settings, the referenced structure must be
 * updated and then passed back to \ref RADIO_PTI_Config
 */
RAIL_Status_t PTI_GetConfig(RAIL_PtiConfig_t *ptiConfig);

/** @} (end addtogroup EFR32xG1x_PTI) */
/** @} (end addtogroup Chip_Specific) */

#ifdef __cplusplus
}
#endif

#endif //__RADIO_PTI_H
