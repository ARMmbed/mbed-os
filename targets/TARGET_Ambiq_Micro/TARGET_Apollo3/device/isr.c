#include "am_mcu_apollo.h"
#include "lp_ticker_defines.h"
volatile bool someFlagThatGetSetinISR = false;

void am_ctimer_isr(void)
{
  uint32_t ui32Status;

  //
  // Check and clear any active CTIMER interrupts.
  //
  ui32Status = am_hal_ctimer_int_status_get(true);
  am_hal_ctimer_int_clear(ui32Status);
  // //
  // // Run handlers for the various possible timer events.
  // //
  am_hal_ctimer_int_service(ui32Status);

  //am_hal_ctimer_int_service(am_hal_ctimer_int_status_get(true)); // get enabled interrupt status and then service those only
}