#include <system.h>

/**
 * \internal
 * Dummy initialization function, used as a weak alias target for the various
 * init functions called by \ref system_init().
 */
void _system_dummy_init(void);
void _system_dummy_init(void)
{
    return;
}

#if !defined(__DOXYGEN__)
#  if defined(__GNUC__)
void system_clock_init(void) WEAK __attribute__((alias("_system_dummy_init")));
void system_board_init(void) WEAK __attribute__((alias("_system_dummy_init")));
void _system_events_init(void) WEAK __attribute__((alias("_system_dummy_init")));
void _system_extint_init(void) WEAK __attribute__((alias("_system_dummy_init")));
#  elif defined(__ICCARM__)
void system_clock_init(void);
void system_board_init(void);
void _system_events_init(void);
void _system_extint_init(void);
#    pragma weak system_clock_init=_system_dummy_init
#    pragma weak system_board_init=_system_dummy_init
#    pragma weak _system_events_init=_system_dummy_init
#    pragma weak _system_extint_init=_system_dummy_init
#  endif
#endif

/**
 * \brief Initialize system.
 *
 * This function will call the various initialization functions within the
 * system namespace. If a given optional system module is not available, the
 * associated call will effectively be a NOP (No Operation).
 *
 * Currently the following initialization functions are supported:
 *  - System clock initialization (via the SYSTEM CLOCK sub-module)
 *  - Board hardware initialization (via the Board module)
 *  - Event system driver initialization (via the EVSYS module)
 *  - External Interrupt driver initialization (via the EXTINT module)
 */
void system_init(void)
{
    /* Configure GCLK and clock sources according to conf_clocks.h */
    system_clock_init();

    /* Initialize board hardware */
    system_board_init();

    /* Initialize EVSYS hardware */
    _system_events_init();

    /* Initialize External hardware */
    _system_extint_init();
}

