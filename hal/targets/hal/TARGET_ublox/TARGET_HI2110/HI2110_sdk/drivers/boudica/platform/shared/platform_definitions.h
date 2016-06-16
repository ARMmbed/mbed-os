/******************************************************************************
 * @brief    BOUDICA Platform Definitions
 * Copyright (c) 2015 NEUL LIMITED
 ******************************************************************************/

#ifndef PLATFORM_DEFINITIONS_H_
#define PLATFORM_DEFINITIONS_H_

/** @addtogroup PLATFORM
 * @{
 * */

/**
 * Generic enum to specify the required core.
 * Generic enum to specify the required core. By using an enum, type safety can be enforced at compile time.
 */
typedef enum
{
    CORES_SECURITY_CORE   = 0, /*!< Security Core.                               */
    CORES_PROTOCOL_CORE   = 1, /*!< Protocol Core.                               */
    CORES_APPS_CORE       = 2, /*!< Applications Core.                           */
    CORES_MAX_NUMBER      = 3, /*!< Used to size/range arrays where needed.      */
    CORES_NONE = CORES_MAX_NUMBER /*!< Used to return a NONE value where needed. */
} CORES;

/** Interrupt Service Routine Callback Type */
typedef void (*ISR_CALLBACK)(void);

/** Interrupt State Type */
typedef enum
{
    INTERRUPT_STATE_DISABLED = 0, /*!< Interrupt Disabled */
    INTERRUPT_STATE_ENABLED = 1,  /*!< Interrupt Enabled  */
} INTERRUPT_STATE;

/** Boudica PIN numbers */
typedef enum {
    PIN_0 = 0,  /*!< PIN 0 */
    PIN_1 = 1,  /*!< PIN 1 */
    PIN_2 = 2,  /*!< PIN 2 */
    PIN_3 = 3,  /*!< PIN 3 */
    PIN_4 = 4,  /*!< PIN 4 */
    PIN_5 = 5,  /*!< PIN 5 */
    PIN_6 = 6,  /*!< PIN 6 */
    PIN_7 = 7,  /*!< PIN 7 */
    PIN_8 = 8,  /*!< PIN 8 */
    PIN_9 = 9,  /*!< PIN 9 */
    PIN_10 = 10, /*!< PIN 10 */
    PIN_11 = 11, /*!< PIN 11 */
    PIN_12 = 12, /*!< PIN 12 */
    PIN_13 = 13, /*!< PIN 13 */
    PIN_14 = 14, /*!< PIN 14 */
    PIN_15 = 15, /*!< PIN 15 */
    PIN_16 = 16, /*!< PIN 16 */
    PIN_17 = 17, /*!< PIN 17 */
    PIN_18 = 18, /*!< PIN 18 */
    PIN_19 = 19,  /*!< PIN 19 */
    PIN_MAX_NUMBER,  /*!< value USED to iterate in arrays */
	PIN_NONE = PIN_MAX_NUMBER  /*!< value used as invalid/unused PIN number */
} PIN;  //PIN numbers


/** @} */

#endif /* PLATFORM_DEFINITIONS_H_ */
