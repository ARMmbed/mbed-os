#include "W7500x_gpio.h"
#include "W7500x_exti.h"
#include "W7500x_pwm.h"
#include "W7500x_uart.h"
#include "W7500x_i2c.h"
#include "W7500x_adc.h"
#include "W7500x_dualtimer.h"
#include "system_W7500x.h"


#ifdef USE_FULL_ASSERT
    #define assert_param(expr)  ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__,__LINE__))
#else
    #define assert_param(expr)   ((void)0)
#endif /* USE_FULL_ASSERT */

