/***************************************************************************//**
 * @file rail_chip_specific.h
 * @brief This file contains the type definitions for EFR32 chip specific
 *        aspects of RAIL.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_CHIP_SPECIFIC_H_
#define __RAIL_CHIP_SPECIFIC_H_

// Include standard type headers to help define structures
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "em_gpio.h"

#include "rail_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Multiprotocol
// -----------------------------------------------------------------------------
/**
 * @addtogroup Multiprotocol_EFR32 EFR32
 * @{
 * @brief EFR32 Specific multiprotocol support defines
 * @ingroup Multiprotocol
 */

/**
 * Placeholder for a chip specific RAIL handle. Using NULL as a RAIL handle is
 * frowned upon, so we use another value that can't be de-referenced.
 */
#define RAIL_EFR32_HANDLE ((RAIL_Handle_t)0xFFFFFFFFUL)

/** @} */ // end of group Multiprotocol_EFR32

// -----------------------------------------------------------------------------
// Calibration
// -----------------------------------------------------------------------------
/**
 * @addtogroup Calibration_EFR32 EFR32
 * @{
 * @brief EFR32 Specific Calibrations
 * @ingroup Calibration
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
 * frequency to not hit this temperature delta. If the application does not
 * calibrate for temperature, it's possible to miss receive packets due to
 * drift in the carrier frequency.
 */

/** EFR32 specific temperature calibration bit */
#define RAIL_CAL_TEMP_VCO         (0x00000001U)
/** EFR32 specific IR calibration bit */
#define RAIL_CAL_ONETIME_IRCAL    (0x00010000U)

/** Mask to run temperature dependent calibrations */
#define RAIL_CAL_TEMP             (RAIL_CAL_TEMP_VCO)
/** Mask to run one time calibrations */
#define RAIL_CAL_ONETIME          (RAIL_CAL_ONETIME_IRCAL)
/** Mask to run optional performance calibrations */
#define RAIL_CAL_PERF             (0)
/** Mask for calibrations that require the radio to be off */
#define RAIL_CAL_OFFLINE          (RAIL_CAL_ONETIME_IRCAL)
/** Mask to run all possible calibrations for this chip */
#define RAIL_CAL_ALL              (RAIL_CAL_TEMP | RAIL_CAL_ONETIME)
/** Mask to run all pending calibrations */
#define RAIL_CAL_ALL_PENDING      (0x00000000U)
/** Invalid calibration value */
#define RAIL_CAL_INVALID_VALUE    (0xFFFFFFFFU)

/**
 * Applies a given image rejection calibration value.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] imageRejection The image rejection value to apply.
 * @return Status code indicating success of the function call.
 *
 * Take an image rejection calibration value and apply it. This value should be
 * determined from a previous run of \ref RAIL_CalibrateIr on the same
 * physical device with the same radio configuration. The imageRejection value
 * will also be stored to the \ref RAIL_ChannelConfigEntry_t::attr, if possible.
 *
 * If multiple protocols are used, this function will return
 * \ref RAIL_STATUS_INVALID_STATE if it is called and the given railHandle is
 * not active. The caller must attempt to re-call this function later, in that
 * case.
 */
RAIL_Status_t RAIL_ApplyIrCalibration(RAIL_Handle_t railHandle,
                                      uint32_t imageRejection);

/**
 * Run the image rejection calibration
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[out] imageRejection The result of the image rejection calibration.
 * @return Status code indicating success of the function call.
 *
 * Run the image rejection calibration, and apply the resulting value. If the
 * imageRejection parameter is non-NULL, then also store the value at that
 * location. The imageRejection value will also be stored to the
 * \ref RAIL_ChannelConfigEntry_t::attr, if possible. This is a long-running
 * calibration that adds significant code space when run, and can be run with a
 * separate firmware image on each device in order to save code space in the
 * final image.
 *
 * If multiple protocols are used, this function will return
 * \ref RAIL_STATUS_INVALID_STATE if it is called and the given railHandle is
 * not active. The caller must attempt to re-call this function later, in that
 * case.
 */
RAIL_Status_t RAIL_CalibrateIr(RAIL_Handle_t railHandle,
                               uint32_t *imageRejection);

/**
 * Run the temperature calibration
 *
 * @param[in] railHandle A RAIL instance handle.
 * @return Status code indicating success of the function call.
 *
 * Run the temperature calibration, which needs to recalibrate the synth if
 * the temperature crosses 0C or the temperature delta since the last
 * calibration exceeds 70C while sitting in receive. RAIL will run VCO
 * calibration automatically upon entering receive state so the application can
 * omit this calibration if the stack will re-enter receive with enough
 * frequency to not hit this temperature delta. If the application does not
 * calibrate for temperature, it's possible to miss receive packets due to
 * drift in the carrier frequency.
 *
 * If multiple protocols are used, this function will return
 * \ref RAIL_STATUS_INVALID_STATE if it is called and the given railHandle is
 * not active. In that case the calibration will be automatically performed the
 * next time the radio enters receive.
 */
RAIL_Status_t RAIL_CalibrateTemp(RAIL_Handle_t railHandle);

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

/**
 * A define to set all RAIL_CalValues_t values to uninitialized.
 *
 * This define can be used when you have no data to pass to the calibration
 * routines but wish to compute and save all possible calibrations.
 */
#define RAIL_CALVALUES_UNINIT (RAIL_CalValues_t){ \
    RAIL_CAL_INVALID_VALUE,                       \
}

/** @} */ // end of group Calibration_EFR32

// -----------------------------------------------------------------------------
// Diagnostic
// -----------------------------------------------------------------------------
/**
 * @addtogroup Diagnostic_EFR32 EFR32
 * @{
 * @brief Types specific to the EFR32 for the diagnostic routines.
 * @ingroup Diagnostic
 */

/**
 * @typedef RAIL_FrequencyOffset_t
 * @brief Chip-specific type that represents the number of Frequency Offset
 *   units. It is used with \ref RAIL_GetRxFreqOffset() and
 *   \ref RAIL_SetFreqOffset().
 *
 * The units on this chip are radio synthesizer resolution steps (synthTicks).
 * On EFR32 (at least for now), the frequency offset is limited to 15 bits
 * (size of SYNTH_CALOFFSET). A value of \ref RAIL_FREQUENCY_OFFSET_INVALID
 * means that this value is invalid.
 */
typedef int16_t RAIL_FrequencyOffset_t;

/**
 * Specifies an invalid frequency offset value. This will be returned if you
 * call \ref RAIL_GetRxFreqOffset() at an invalid time.
 */
#define RAIL_FREQUENCY_OFFSET_INVALID ((int16_t)0xFFFF)

/** @} */ // end of group Diagnostic_EFR32

// -----------------------------------------------------------------------------
// Radio Configuration
// -----------------------------------------------------------------------------
/**
 * @addtogroup Radio_Configuration_EFR32 EFR32
 * @{
 * @ingroup Radio_Configuration
 * @brief Types specific to the EFR32 for radio configuration.
 */

/**
 * @brief Radio Configuration structure
 *
 * The radio configuration is generated in order to properly configure the
 * radio for operation on a protocol. These configurations should not be
 * created or edited by hand.
 */
typedef const uint32_t *RAIL_RadioConfig_t;

/** @} */ // end of group Radio_Configuration_EFR32

// -----------------------------------------------------------------------------
// Transmit
// -----------------------------------------------------------------------------
/**
 * @addtogroup PA_EFR32 EFR32
 * @{
 * @ingroup PA
 * @brief Types specific to the EFR32 for dealing with the on chip PAs.
 */

/**
 * Raw power levels used directly by the RAIL_Get/SetTxPower API where a higher
 * numerical value corresponds to a higher output power. These are referred to
 * as 'raw (values/units)'. On the EFR32 they can range from one of \ref
 * RAIL_TX_POWER_LEVEL_LP_MIN, \ref RAIL_TX_POWER_LEVEL_HP_MIN, or
 * \ref RAIL_TX_POWER_LEVEL_SUBGIG_MIN to one of \ref
 * RAIL_TX_POWER_LEVEL_LP_MAX, \ref RAIL_TX_POWER_LEVEL_HP_MAX, and \ref
 * RAIL_TX_POWER_LEVEL_SUBGIG_MAX, respectively, depending on the selected \ref
 * RAIL_TxPowerMode_t.
 */
typedef uint8_t RAIL_TxPowerLevel_t;

/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_LP_MAX     7
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_HP_MAX     252
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_MAX 248
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_HP_MIN     0
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_LP_MIN     1
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_MIN 0
/**
 * Invalid RAIL_TxPowerLevel_t value returned when there is an error
 * with RAIL_GetTxPower
 */
#define RAIL_TX_POWER_LEVEL_INVALID 255

/**
 * @enum RAIL_TxPowerMode_t
 * @brief Enumeration of the EFR32 power modes.
 *
 * The power modes on the EFR32 correspond to the different on-chip PA's that
 * are available. For more information about the power and performance
 * characteristics of a given amplifier, please consult the data sheet.
 */
RAIL_ENUM(RAIL_TxPowerMode_t) {
  /** High power amplifier, up to 20dBm, raw values: 0-252 */
  RAIL_TX_POWER_MODE_2P4_HP,
  /** Low power amplifier, up to 0dBm, raw values: 1-7 */
  RAIL_TX_POWER_MODE_2P4_LP,
  /** SubGig amplifier, up to 20dBm, raw values: 0-248 */
  RAIL_TX_POWER_MODE_SUBGIG,
  /** Invalid amplifier Selection */
  RAIL_TX_POWER_MODE_NONE
};

/**
 * @struct RAIL_TxPowerConfig_t
 *
 * @brief Structure containing values used to initialize the power amplifiers.
 */
typedef struct RAIL_TxPowerConfig {
  /** Tx power mode */
  RAIL_TxPowerMode_t mode;
  /** Power amplifier supply voltage in mV, generally:
   *  DCDC supply ~ 1800mV (1.8V)
   *  Battery supply ~ 3300mV (3.3V)
   */
  uint16_t voltage;
  /** The amount of time to spend ramping for Tx in uS. */
  uint16_t rampTime;
} RAIL_TxPowerConfig_t;

/** @} */ // end of group PA_EFR32

// -----------------------------------------------------------------------------
// PTI
// -----------------------------------------------------------------------------
/**
 * @addtogroup PTI_EFR32 EFR32
 * @{
 * @brief EFR32 PTI functionality
 * @ingroup PTI
 *
 * These enums and structures are used with RAIL PTI API. EFR32 supports
 * SPI and UART PTI, and is configurable in terms of baud rates and pin PTI
 * pin locations.
 */

/** Channel type enumeration. */
RAIL_ENUM(RAIL_PtiMode_t) {
  /** Turn PTI off entirely */
  RAIL_PTI_MODE_DISABLED,
  /** SPI mode. */
  RAIL_PTI_MODE_SPI,
  /** UART mode. */
  RAIL_PTI_MODE_UART,
  /** 9bit UART mode. */
  RAIL_PTI_MODE_UART_ONEWIRE
};

/**
 * @struct RAIL_PtiConfig_t
 * @brief Configuration for PTI.
 */
typedef struct RAIL_PtiConfig {
  /** Packet Trace mode (UART or SPI) */
  RAIL_PtiMode_t mode;
  /** Output baudrate for PTI in Hz */
  uint32_t baud;
  /** Data output (DOUT) location for pin/port */
  uint8_t doutLoc;
  /** Data output (DOUT) GPIO port */
  GPIO_Port_TypeDef doutPort;
  /** Data output (DOUT) GPIO pin */
  uint8_t doutPin;
  /** Data clock (DCLK) location for pin/port. Only used in SPI mode */
  uint8_t dclkLoc;
  /** Data clock (DCLK) GPIO port. Only used in SPI mode */
  GPIO_Port_TypeDef dclkPort;
  /** Data clock (DCLK) GPIO pin. Only used in SPI mode */
  uint8_t dclkPin;
  /** Data frame (DFRAME) location for pin/port. Only used for  */
  uint8_t dframeLoc;
  /** Data frame (DFRAME) GPIO port */
  GPIO_Port_TypeDef dframePort;
  /** Data frame (DFRAME) GPIO pin */
  uint8_t dframePin;
} RAIL_PtiConfig_t;

/** @} */ // end of group PTI_EFR32

// -----------------------------------------------------------------------------
// Antenna Control
// -----------------------------------------------------------------------------
/**
 * @addtogroup Antenna_Control_EFR32 EFR32
 * @{
 * @brief EFR32 Antenna Control functionality
 * @ingroup Antenna
 *
 * These enums and structures are used with RAIL Antenna Control API. EFR32 supports
 * up to two antennas and with configurable pin locations.
 */

/**
 * @struct RAIL_AntennaConfig_t
 * @brief Configuration for Antenna switch pins.
 */
typedef struct RAIL_AntennaConfig {
  /** MODEM_ROUTEPEN fields */
  /** Antenna 0 Pin Enable */
  bool ant0PinEn;
  /** Antenna 1 Pin Enable */
  bool ant1PinEn;
  /** MODEM_ROUTELOC1 fields */
  /** Antenna 0 location for pin/port */
  uint8_t ant0Loc;
  /** Antenna 0 output GPIO port */
  GPIO_Port_TypeDef ant0Port;
  /** Antenna 0 output GPIO pin */
  uint8_t ant0Pin;
  /** Antenna 1 location for pin/port */
  uint8_t ant1Loc;
  /** Antenna 1 output GPIO port */
  GPIO_Port_TypeDef ant1Port;
  /** Antenna 1 output GPIO pin */
  uint8_t ant1Pin;
} RAIL_AntennaConfig_t;

/** @} */ // end of group Antenna_Control_EFR32

#ifdef __cplusplus
}
#endif

#endif
