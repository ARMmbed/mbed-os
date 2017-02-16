/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#ifndef __OSDEP_INTF_H_
#define __OSDEP_INTF_H_

typedef	struct	net_device *	_nic_hdl;
struct iw_request_info { 
    u16           cmd;            /* Wireless Extension command */
    u16           flags;          /* More to come ;-) */ 
};
typedef int (*iw_handler)(struct net_device *dev, struct iw_request_info *info,
			  union iwreq_data *wrqu, char *extra);

struct pkt_buff {
	_list list;
	u32 len;
	unsigned char *data;
};

#if defined(PLATFORM_FREERTOS) || defined (PLATFORM_CMSIS_RTOS)

#include "freertos/wrapper.h"
#include "freertos/freertos_intfs.h"
#include "freertos/freertos_xmit.h"
#include "freertos/freertos_recv.h"

#elif defined(PLATFORM_ECOS)

#include "ecos/ecos_xmit.h"
#include "ecos/ecos_recv.h"
#include "ecos/ecos_mlme.h"
#include "ecos/ecos_intfs.h"

#ifdef CONFIG_PROC_DEBUG  //need move to ecos/ecos_intfs.h
void rtw_proc_init_one(struct net_device *dev);
void rtw_proc_remove_one(struct net_device *dev);
#else
static void rtw_proc_init_one(struct net_device *dev){}
static void rtw_proc_remove_one(struct net_device *dev){}
#endif //CONFIG_PROC_DEBUG

#elif defined(PLATFORM_LINUX)
int rtw_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);

int rtw_init_netdev_name(struct net_device *pnetdev, const char *ifname);
struct net_device *rtw_init_netdev(_adapter *padapter);

#ifdef CONFIG_PROC_DEBUG
void rtw_proc_init_one(struct net_device *dev);
void rtw_proc_remove_one(struct net_device *dev);
#else
static void rtw_proc_init_one(struct net_device *dev){}
static void rtw_proc_remove_one(struct net_device *dev){}
#endif //CONFIG_PROC_DEBUG
#endif

#ifdef CONFIG_CONCURRENT_MODE
struct _io_ops;
_adapter *rtw_drv_if2_init(_adapter *primary_padapter, char *name, void (*set_intf_ops)(struct _io_ops *pops));
void rtw_drv_if2_free(_adapter *pbuddy_padapter);
#endif

struct net_device *rtw_alloc_etherdev_with_old_priv(int sizeof_priv, void *old_priv);
struct net_device * rtw_alloc_etherdev(int sizeof_priv);
void rtw_free_netdev(struct net_device * netdev);

int	rtw_netif_queue_stopped(struct net_device *pnetdev);
void	rtw_netif_wake_queue(struct net_device *pnetdev);
void	rtw_netif_start_queue(struct net_device *pnetdev);
void	rtw_netif_stop_queue(struct net_device *pnetdev);

struct pkt_buff *rtw_alloc_pktbuf(unsigned int size);
void rtw_free_pktbuf(struct pkt_buff *skb);

#if 0
int RTW_STATUS_CODE(int error_code);

//flags used for rtw_update_mem_stat()
enum {
	MEM_STAT_VIR_ALLOC_SUCCESS,
	MEM_STAT_VIR_ALLOC_FAIL,
	MEM_STAT_VIR_FREE,
	MEM_STAT_PHY_ALLOC_SUCCESS,
	MEM_STAT_PHY_ALLOC_FAIL,
	MEM_STAT_PHY_FREE,
	MEM_STAT_TX, //used to distinguish TX/RX, asigned from caller
	MEM_STAT_TX_ALLOC_SUCCESS,
	MEM_STAT_TX_ALLOC_FAIL,
	MEM_STAT_TX_FREE,
	MEM_STAT_RX, //used to distinguish TX/RX, asigned from caller
	MEM_STAT_RX_ALLOC_SUCCESS,
	MEM_STAT_RX_ALLOC_FAIL,
	MEM_STAT_RX_FREE
};
#endif

#endif	//_OSDEP_INTF_H_

