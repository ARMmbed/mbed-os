#ifndef RESET_H_INCLUDED
#define RESET_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup asfdoc_sam0_system_group
 * @{
 */

/**
 * \brief Reset causes of the system.
 *
 * List of possible reset causes of the system.
 */
enum system_reset_cause {
    /** The system was last reset by a software reset. */
    SYSTEM_RESET_CAUSE_SOFTWARE       = PM_RCAUSE_SYST,
    /** The system was last reset by the watchdog timer. */
    SYSTEM_RESET_CAUSE_WDT            = PM_RCAUSE_WDT,
    /** The system was last reset because the external reset line was pulled low. */
    SYSTEM_RESET_CAUSE_EXTERNAL_RESET = PM_RCAUSE_EXT,
    /** The system was last reset by the BOD33. */
    SYSTEM_RESET_CAUSE_BOD33          = PM_RCAUSE_BOD33,
    /** The system was last reset by the BOD12. */
    SYSTEM_RESET_CAUSE_BOD12          = PM_RCAUSE_BOD12,
    /** The system was last reset by the POR (Power on reset). */
    SYSTEM_RESET_CAUSE_POR            = PM_RCAUSE_POR,
};


/**
 * \name Reset Control
 * @{
 */

/**
 * \brief Reset the MCU.
 *
 * Resets the MCU and all associated peripherals and registers, except RTC, all 32kHz sources,
 * WDT (if ALWAYSON is set) and GCLK (if WRTLOCK is set).
 *
 */
static inline void system_reset(void)
{
    NVIC_SystemReset();
}

/**
 * \brief Return the reset cause.
 *
 * Retrieves the cause of the last system reset.
 *
 * \return An enum value indicating the cause of the last system reset.
 */
static inline enum system_reset_cause system_get_reset_cause(void)
{
    return (enum system_reset_cause)PM->RCAUSE.reg;
}

/**
 * @}
 */

/** @} */
#ifdef __cplusplus
}
#endif

#endif /* RESET_H_INCLUDED */
