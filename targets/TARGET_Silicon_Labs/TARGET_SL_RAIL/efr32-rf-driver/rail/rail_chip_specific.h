/***************************************************************************//**
 * @file rail_chip_specific.h
 * @brief This file contains the type definitions for EFR32 chip specific
 *        aspects of RAIL.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_CHIP_SPECIFIC_H_
#define __RAIL_CHIP_SPECIFIC_H_

// Include standard type headers to help define structures
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// -----------------------------------------------------------------------------
// Calibration
// -----------------------------------------------------------------------------
/**
 * @addtogroup Calibration
 * @{
 */

/**
 * @addtogroup EFR32
 * @{
 * @brief EFR32 Specific Calibrations
 *
 * The EFR32 has two supported calibrations. There is the Image Rejection (IR)
 * calibration and a temperature dependent calibration. The IR calibration is
 * something that can be computed once and stored off or computed each time at
 * startup. It is PHY specific and provides sensitivity improvements so we
 * highly recommend using it. The IR calibration should only be run when the
 * radio is IDLE.
 *
 * The temperature dependent calibrations are used to recalibrate the synth if
 * the temperature crosses 0C or the temperature delta since the last
 * calibration exceeds 70C while sitting in receive. RAIL will run VCO
 * calibration automatically upon entering receive state so the application can
 * omit this calibration if the stack will re-enter receive with enough
 * frequency to not hit this temperature delta.  If the application does not
 * calibrate for temperature, it's possible to miss receive packets due to
 * drift in the carrier frequency.
 */

/**
 * @struct RAIL_CalValues_t
 * @brief Calibration value structure
 *
 * This structure contains the set of persistent calibration values for the
 * EFR32. You can set these before hand and apply them at startup to save the
 * time required to compute them. Any of these values may be set to
 * RAIL_CAL_INVALID_VALUE to force the code to compute that calibration value.
 */
typedef struct RAIL_CalValues {
  uint32_t imageRejection; /**< Image Rejection (IR) calibration value */
} RAIL_CalValues_t;

/** Invalid calibration value */
#define RAIL_CAL_INVALID_VALUE  (0xFFFFFFFF)

/**
 * A define to set all RAIL_CalValues_t values to uninitialized.
 *
 * This define can be used when you have no data to pass to the calibration
 * routines but wish to compute and save all possible calibrations.
 */
#define RAIL_CALVALUES_UNINIT {                         \
  RAIL_CAL_INVALID_VALUE, \
}

/** EFR32 specific temperature calibration bit */
#define RAIL_CAL_TEMP_VCO         (0x00000001)
/** EFR32 specific IR calibration bit */
#define RAIL_CAL_ONETIME_IRCAL    (0x00010000)

/** Mask to run temperature dependent calibrations */
#define RAIL_CAL_TEMP         (RAIL_CAL_TEMP_VCO)
/** Mask to run one time calibrations */
#define RAIL_CAL_ONETIME      (RAIL_CAL_ONETIME_IRCAL)
/** Mask to run optional performance calibrations */
#define RAIL_CAL_PERF         ()
/** Mask for calibrations that require the radio to be off */
#define RAIL_CAL_OFFLINE      (RAIL_CAL_ONETIME_IRCAL)
/** Mask to run all possible calibrations for this chip */
#define RAIL_CAL_ALL          (RAIL_CAL_TEMP | RAIL_CAL_ONETIME)
/** Mask to run all pending calibrations */
#define RAIL_CAL_ALL_PENDING  (0x00000000)

/**
 * @}
 * @}
 */

#endif
