/*
 *******************************************************************************
 * @brief:   Framework to preform cycle count measurements
 *
 * @details  this is a framework for monitoring the cycle counts
 *           for ISRs and APIs. The framework uses systick.

*******************************************************************************

 Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.

 This software is proprietary and confidential.  By using this software you agree
 to the terms of the associated Analog Devices License Agreement.

 ******************************************************************************/

#ifndef ADI_CYCLECOUNT_H
#define ADI_CYCLECOUNT_H

#include <stdbool.h>
#include <stdint.h>
#include <adi_cycle_counting_config.h>


 /** @addtogroup  cyclecount_logging Cycle Counting Framework
 *   @{
 */

/*!
 * 64-bit integer to record cycle counts.
 * Since UINT32_MAX = 4,294,967,296 cycles
 * at 26 MHz this would allow us to record for 165 seconds
 * before the system would wrap around.
 * By moving to a 64-bit integer we can record for 11,248 years.
 */
typedef uint64_t  adi_cyclecount_t;


/*!
 * The systick timer is a 24-bit count down timer
 * The initial value can, therefore,  be up to 0xFFFFFF
 * The larger the value the fewer interrupts that will be taken
 * and the less impact cycle counting will have on the system
 */
#define ADI_CYCLECOUNT_SYSTICKS        (0xFFFFFFu)

/*!
 * Cycle counting nesting is supported via a cycle counting stack. The initial
 * value of the stack index is one less than the starting stack
 * index (0)
 */
#define ADI_CYCLECOUNT_INITIAL_STACK_INDEX (-1)

/*!
 * Cycle Count API function return values.
 */
typedef enum {

    ADI_CYCLECOUNT_SUCCESS,              /*!< API completed successfully */
    ADI_CYCLECOUNT_ADD_ENTITY_FAILURE,   /*!< There is not enough space in the cycle counting entity array. Consider increasing the size via the #ADI_CYCLECOUNT_NUMBER_USER_DEFINED_APIS static configuration macro   */
    ADI_CYCLECOUNT_INVALID_ID,           /*!< The API/ISR ID is invalid. */
    ADI_CYCLECOUNT_FAILURE               /*!< API did not complete successfully. */
} ADI_CYCLECOUNT_RESULT;


/*!
 * List of cycle counting IDs for the ISRs and APIs that can record cycle counts.
 * Items enumerated here must be aligned with adi_cyclecounting_identifiers
 *
 * Note that the ID numbering starts at 1. ID==0 is not used.
 * Note that the application can extend this list via static configuration (see adi_cycle_counting_config.h) and
 * via the adi_cyclecount_addEntity() API.
 */
#define    ADI_CYCLECOUNT_ISR_EXT_3         1u      /*!< Cycle count ID for EXT3 Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_UART          2u      /*!< Cycle count ID for UART Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_DMA_UART_TX   3u      /*!< Cycle count ID for UART DMA TX Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_DMA_UART_RX   4u      /*!< Cycle count ID for UART DMA RX Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_TMR_COMMON    5u      /*!< Cycle count ID for Timer Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_RTC           6u      /*!< Cycle count ID for RTC Interrupt Handler.*/
#define    ADI_CYCLECOUNT_ISR_SPI           7u      /*!< Cycle count ID for SPI Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_CRC           8u      /*!< Cycle count ID for CRC Interrupt Handler. */
#define    ADI_CYCLECOUNT_ISR_SPORT         9u      /*!< Cycle count ID for SPORT Interrupt Handler. */
#define    ADI_CYCLECOUNT_ID_COUNT         10u      /*!< Number of cycle count ISRs and APIs. Must be one greater than the last ID. */


/*!
 * The following are tracked when cycle counting
 *    Maximum number of cycle counts
 *    Minimum number of cycle counts
 *    Average number of cycle counts
 */
typedef struct
{
  adi_cyclecount_t max_cycles_adjusted;         /*!< Tracks the adjusted max cycle count */
  adi_cyclecount_t min_cycles_adjusted;         /*!< Tracks the adjusted min cycle count */
  adi_cyclecount_t average_cycles_adjusted;     /*!< Tracks the adjusted average cycle count */

  adi_cyclecount_t max_cycles_unadjusted;       /*!< Tracks the unadjusted max cycle count */
  adi_cyclecount_t min_cycles_unadjusted;       /*!< Tracks the unadjusted min cycle count */
  adi_cyclecount_t average_cycles_unadjusted;   /*!< Tracks the unadjusted average cycle count */

  uint32_t         sample_count;                /*!< Number of cycle count samples recorded, used to compute the average */

} ADI_CYCLECOUNT_LOG;

/*!
 * Cycle counting has to be enabled in the cycle counting configuration file
 * If enabled then cycle counting related macros map to the cycle counting APIs.
 * If not enabled, then the macros maps to a NOP
 */
#if defined(ADI_CYCLECOUNT_ENABLED) && (ADI_CYCLECOUNT_ENABLED == 1u)

  #define ADI_CYCLECOUNT_INITIALIZE()        adi_cyclecount_init()      /*!< Initialize the cycle counting data structures */
  #define ADI_CYCLECOUNT_STORE(id)          adi_cyclecount_store(id)    /*!< Record the number of cycles for the specified ISR or API */
  #define ADI_CYCLECOUNT_REPORT()           adi_cyclecount_report()     /*!< Generate a cycle counting report */

#else

  #define ADI_CYCLECOUNT_INITIALIZE()       do{}while(0)      /*!< Initialize the cycle counting data structures */
  #define ADI_CYCLECOUNT_STORE(id)          do{}while(0)      /*!< Record the number of cycles for the specified ISR or API */
  #define ADI_CYCLECOUNT_REPORT()           do{}while(0)      /*!< Generate a cycle counting report */
#endif


/* Forward API declarations */
extern ADI_CYCLECOUNT_RESULT  adi_cyclecount_start(void);
extern ADI_CYCLECOUNT_RESULT  adi_cyclecount_stop(void);
extern adi_cyclecount_t       adi_cyclecount_get(void);
extern ADI_CYCLECOUNT_RESULT  adi_cyclecount_store(uint32_t id);
extern void                   adi_cyclecount_init(void);
extern void                   adi_cyclecount_report(void);
extern ADI_CYCLECOUNT_RESULT  adi_cyclecount_addEntity(const char *EntityName, uint32_t *pid);

extern void SysTick_Handler(void);

/**@}*/

#endif /* ADI_CYCLECOUNT_H */

