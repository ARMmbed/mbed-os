/**
 *******************************************************************************
 * @file    sleep_api.c
 * @brief Implementation of a sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  $
 * $Date: $
 ******************************************************************************
 * @copyright (c) 2015 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup sleep
 *
 * @details
 * Sleep implementation TBD - Dummy function is fine for first release
 *
 */

#if DEVICE_SLEEP
#include "sleep.h"
#include "sleep_api.h"
#include "cmsis_nvic.h"

void mbed_enter_sleep(sleep_t *obj)
{
#if 0

#if 1 /* TODO Temporary code will be removed once mbed OS paases this details */
	obj->timeToSleep = False;
	obj->SleepType 	 = SLEEP_TYPE_DEEPSLEEP;
#endif

	CLOCK_ENABLE(CLOCK_PMU);

	/* Select sleep mode */
    if(obj->timeToSleep == False)
	{/* Use SleepType to select sleep mode*/
		if(obj->SleepType == SLEEP_TYPE_SLEEP)
		{
			sleep();
		}
		else if(obj->SleepType == SLEEP_TYPE_DEEPSLEEP)
		{
			deepsleep();
		}
		else if(obj->SleepType == SLEEP_TYPE_COMA)
		{
			coma();
		}
		else
		{
			/* Do nothing */
		}
	}
	else
	{/* Select sleep mode using TimetoSleep */
		if((obj->timeToSleep >= SLEEP_DURATION_SLEEP_MIN) && (obj->timeToSleep <= SLEEP_DURATION_SLEEP_MAX))
		{
			sleep();
		}
		else if((obj->timeToSleep > SLEEP_DURATION_SLEEP_MAX) && (obj->timeToSleep <= SLEEP_DURATION_DEEPSLEEP_MAX))
		{
			deepsleep();
		}
		else if((obj->timeToSleep > SLEEP_DURATION_DEEPSLEEP_MAX) && (obj->timeToSleep <= SLEEP_DURATION_COMA_MAX))
		{
			coma();
		}
		else
		{
			/* Do nothing */
		}
	}

    CLOCK_DISABLE(CLOCK_PMU);
#endif
}

void mbed_exit_sleep(sleep_t *obj)
{
    (void)obj;
}
#endif /* DEVICE_SLEEP */