#ifndef _ROM_RTW_MESSAGE_
#define _ROM_RTW_MESSAGE_

#include <platform_stdlib.h>

typedef enum {
#define ROM_E_RTW_MSGPOOL(name,str)  ROM_E_RTW_MSGP_##name,
#include "rom_rtw_message_e.h"
  ROM_E_RTW_MSGP_MAX
} rom_e_rtw_msgp_t;

#if ROM_E_RTW_MSG
extern const char *rom_e_rtw_msgp_str_[];
#define rom_e_rtw_msg_printf(name, fmt, args...) printf((char*)rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_##name], ## args)
#define rom_e_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		printf("\n\r");\
		printf((char*)rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_##name], ## args);\
	}while(0)
#else
#define rom_e_rtw_msg_printf(name, fmt, args...) printf(fmt, ## args)
#define rom_e_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		printf("\n\r");\
		printf(DRIVER_PREFIX ##fmt, ## args);\
	}while(0)
#endif //ROM_E_RTW_MSG

#endif //_ROM_RTW_MESSAGE_
