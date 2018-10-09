/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _DEVICE_LOCK_H_
#define _DEVICE_LOCK_H_

enum _RT_DEV_LOCK_E
{
	RT_DEV_LOCK_EFUSE = 0,
	RT_DEV_LOCK_FLASH = 1,
	RT_DEV_LOCK_CRYPTO = 2,
	RT_DEV_LOCK_PTA = 3,
	RT_DEV_LOCK_WLAN = 4,
	RT_DEV_LOCK_MAX = 5
};
typedef uint32_t RT_DEV_LOCK_E;

void device_mutex_lock(RT_DEV_LOCK_E device);
void device_mutex_unlock(RT_DEV_LOCK_E device);

#endif //_DEVICE_LOCK_H_
