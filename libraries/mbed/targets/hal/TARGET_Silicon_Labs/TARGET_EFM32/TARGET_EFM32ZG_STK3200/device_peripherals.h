/* Using TIMER0 for lower timer */
#define US_TIMER_L	TIMER0
#define US_TIMER_L_IRQn	TIMER0_IRQn
#define US_TIMER_L_CLOCK cmuClock_TIMER0

/* Using TIMER1 for upper timer */
#define US_TIMER_H	TIMER1
#define US_TIMER_H_IRQn	TIMER1_IRQn
#define US_TIMER_H_CLOCK cmuClock_TIMER1


#define SERIAL_NUM_UARTS (5)


#define PWM_TIMER TIMER0
#define PWM_TIMER_CLOCK cmuClock_TIMER0
#define PWM_ROUTE TIMER_ROUTE_LOCATION_LOC1
