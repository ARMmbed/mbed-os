#ifndef _NORDIC_GLOBAL_H_
#define _NORDIC_GLOBAL_H_

/* There are no global defines in mbed, so we need to define */
/* mandatory conditional compilation flags here              */
//#define NRF51
#ifndef	DEBUG_NRF_USER
#define DEBUG_NRF_USER
#endif
#ifndef	BLE_STACK_SUPPORT_REQD
#define BLE_STACK_SUPPORT_REQD
#endif
#ifndef	BOARD_PCA10001
#define BOARD_PCA10001
#endif

#endif
