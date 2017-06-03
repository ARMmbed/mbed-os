/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "osdep_service.h"
#include "device_lock.h"

//------------------------------------------------------
#define DEVICE_MUTEX_IS_INIT(device)	(mutex_init & (1<<device))
#define DEVICE_MUTEX_SET_INIT(device)	(mutex_init |= (1<<device))
#define DEVICE_MUTEX_CLR_INIT(device)	(mutex_init &= (~(1<<device)))

static u32 mutex_init = 0;
static _mutex device_mutex[RT_DEV_LOCK_MAX];

//======================================================
static void device_mutex_init(RT_DEV_LOCK_E device)
{
	if(!DEVICE_MUTEX_IS_INIT(device)){
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);
		if(!DEVICE_MUTEX_IS_INIT(device)){
			rtw_mutex_init(&device_mutex[device]);
			DEVICE_MUTEX_SET_INIT(device);
		}
		rtw_exit_critical(&lock, &irqL);
	}
}

//======================================================
static void device_mutex_free(RT_DEV_LOCK_E device)
{
	if(DEVICE_MUTEX_IS_INIT(device)){
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);
		if(!DEVICE_MUTEX_IS_INIT(device)){
			rtw_mutex_free(&device_mutex[device]);
			DEVICE_MUTEX_CLR_INIT(device);
		}
		rtw_exit_critical(&lock, &irqL);
	}
}

//======================================================
void device_mutex_lock(RT_DEV_LOCK_E device)
{
	device_mutex_init(device);
	while(rtw_mutex_get_timeout(&device_mutex[device], 10000)<0)
		printf("device lock timeout: %d\n", device);
}

//======================================================
void device_mutex_unlock(RT_DEV_LOCK_E device)
{
	device_mutex_init(device);
	rtw_mutex_put(&device_mutex[device]);
}
