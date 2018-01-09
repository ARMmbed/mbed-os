#ifndef MBED_DRV_CFG_H
#define MBED_DRV_CFG_H

/* can_api.c */
#define CAN_TEST_GLOBAL_CH  1

/* gpio_api.c */
#define GPIO_GROUP_MAX      9

/* pwmout_api.c */
#define FUNC_MOTOR_CTL_PWM
#define FUMC_MTU2_PWM

/* rtc_api.c */
#define USE_RTCX1_CLK
//#define USE_EXTAL_CLK
//#define USE_RTCX3_CLK

#endif
