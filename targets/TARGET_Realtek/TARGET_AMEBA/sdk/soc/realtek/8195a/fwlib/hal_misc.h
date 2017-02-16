/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _MISC_H_
#define _MISC_H_

#include <basic_types.h>

#define CHIP_ID_8711AM      0xFF
#define CHIP_ID_8195AM      0xFE
#define CHIP_ID_8711AF      0xFD
#define CHIP_ID_8710AF      0xFC
#define CHIP_ID_8711AN      0xFB
#define CHIP_ID_8710AM      0xFA
#define CHIP_ID_SIP         0xF9  
#define CHIP_ID_COMBO_SIP   0xF8

#ifdef CONFIG_TIMER_MODULE
extern _LONG_CALL_ u32 HalDelayUs(u32 us);
#endif

extern _LONG_CALL_ u32 HalGetCpuClk(VOID);
extern _LONG_CALL_ u8 HalGetRomInfo(VOID);

extern _LONG_CALL_ROM_ void *_memset( void *s, int c, SIZE_T n );
extern _LONG_CALL_ROM_ void *_memcpy( void *s1, const void *s2, SIZE_T n );
#if defined(CONFIG_RELEASE_BUILD_LIBRARIES) && (!defined(E_CUT_ROM_DOMAIN))
// we built A/B/C cut ROM Lib with this wrong declaration, we need to keep the same for compatible
extern _LONG_CALL_ROM_ int *_memcmp( const void *av, const void *bv, SIZE_T len );
#else
extern _LONG_CALL_ROM_ int _memcmp( const void *av, const void *bv, SIZE_T len );
#endif
extern _LONG_CALL_ROM_ SIZE_T _strlen(const char *s);
extern _LONG_CALL_ROM_ int _strcmp(const char *cs, const char *ct);


#endif  //_MISC_H_
