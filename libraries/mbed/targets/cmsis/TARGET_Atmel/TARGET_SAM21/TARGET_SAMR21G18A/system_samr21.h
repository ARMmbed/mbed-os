#ifndef _SYSTEM_SAMR21_H_INCLUDED_
#define _SYSTEM_SAMR21_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;   /*!< System Clock Frequency (Core Clock)  */

void SystemInit(void);
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_SAMR21_H_INCLUDED */
