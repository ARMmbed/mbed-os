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
#define _GSPI_INTF_C_

#include <drv_types.h>

#ifdef CONFIG_GSPI_HCI

struct dvobj_priv *gspi_dvobj_init(void)
{
//	int status = _FAIL;
	struct dvobj_priv *dvobj = NULL;
	PGSPI_DATA pgspi_data;

_func_enter_;

	dvobj = (struct dvobj_priv*)rtw_zmalloc(sizeof(*dvobj));
	if (NULL == dvobj) {
		goto exit;
	}

	pgspi_data = &dvobj->intf_data;
	
	rtw_mutex_init(&pgspi_data->spi_mutex);	
	//pgspi_data->block_transfer_len = 512; //512 blocks r/w is not required for GSPI interface
	//pgspi_data->tx_block_mode = 0;
	//pgspi_data->rx_block_mode = 0;

//	status = _SUCCESS;

#if 0	
free_dvobj:
	if (status != _SUCCESS && dvobj) {
		rtw_mfree((u8*)dvobj, sizeof(*dvobj));
		dvobj = NULL;
	}
#endif
exit:
_func_exit_;

	return dvobj;
}

void gspi_dvobj_deinit(struct dvobj_priv *dvobj)
{
//TODO
//	struct dvobj_priv *dvobj = spi_get_drvdata(spi);

_func_enter_;
//TODO
//	spi_set_drvdata(spi, NULL);
	if (dvobj) {
//TODO
//		gspi_deinit(dvobj);
		rtw_mutex_free(&dvobj->intf_data.spi_mutex);	
		rtw_mfree((u8*)dvobj, sizeof(*dvobj));
	}

_func_exit_;
}

s32 gspi_dvobj_xmit_mgnt(_adapter * padapter, struct xmit_frame *pmgntframe)
{
	s32 ret = _SUCCESS;
	struct pkt_attrib	*pattrib;
	struct xmit_buf	*pxmitbuf;
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct	mlme_priv	*pmlmepriv = &(padapter->mlmepriv);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	u8	*pframe = NULL;
	
	RT_TRACE(_module_rtl871x_xmit_c_, _drv_info_, ("+rtw_xmit_mgnt()\n"));

	pattrib = &pmgntframe->attrib;
	pxmitbuf = pmgntframe->pxmitbuf;
	pframe = (u8 *)(pmgntframe->buf_addr) + TXDESC_OFFSET;

	rtw_hal_update_txdesc(padapter, pmgntframe, pmgntframe->buf_addr);

	pxmitbuf->len = TXDESC_SIZE + pattrib->last_txcmdsz;
	//pxmitbuf->pg_num = (pxmitbuf->len + 127)/128; // 128 is tx page size
	//pxmitbuf->ptail = pmgntframe->buf_addr + pxmitbuf->len;
	pxmitbuf->ff_hwaddr = rtw_get_ff_hwaddr(pmgntframe);

	rtw_count_tx_stats(padapter, pmgntframe, pattrib->last_txcmdsz);

	//RT_TRACE(_module_rtl871x_xmit_c_, _drv_always_, ("+rtw_xmit_mgnt(): type=%d\n", GetFrameSubType(pframe)));
	if(GetFrameSubType(pframe)==WIFI_BEACON) //dump beacon directly
	{
//When using dedicated xmit frame for issue bcn on ap mode
//free xmit frame for bcn reserved page on station mode - Alex Fang
#if USE_DEDICATED_BCN_TX
		if(check_fwstate(pmlmepriv, WIFI_AP_STATE) != _TRUE) {
			rtw_free_xmitframe(pxmitpriv, pmgntframe);
			pxmitbuf->priv_data = NULL;
		}
		rtw_write_port(padapter, ffaddr2deviceId(pdvobjpriv, pxmitbuf->ff_hwaddr), pxmitbuf->len, pxmitbuf->pbuf);

		if(check_fwstate(pmlmepriv, WIFI_AP_STATE) != _TRUE)
			rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
#else
		rtw_free_xmitframe(pxmitpriv, pmgntframe);
		pxmitbuf->priv_data = NULL;
		rtw_xmit_xmitbuf(padapter, pxmitbuf);
#endif
	}		
	else
	{
		rtw_free_xmitframe(pxmitpriv, pmgntframe);
		pxmitbuf->priv_data = NULL;
		rtw_xmit_xmitbuf(padapter, pxmitbuf);
	}

	if  (ret != _SUCCESS)
		rtw_sctx_done_err(&pxmitbuf->sctx, RTW_SCTX_DONE_UNKNOWN);

	RT_TRACE(_module_rtl871x_xmit_c_, _drv_info_, ("-rtw_xmit_mgnt\n"));
	return ret;
}

//#include <skbuff.h>
s32 gspi_dvobj_xmit_data(_adapter *padapter, struct xmit_frame *pxmitframe)
{
	struct xmit_priv 	*pxmitpriv = &padapter->xmitpriv;
	_irqL irql;
	s32 err;

#ifdef CONFIG_80211N_HT
	if ((pxmitframe->frame_tag == DATA_FRAMETAG) &&
		(pxmitframe->attrib.ether_type != 0x0806) &&
		(pxmitframe->attrib.ether_type != 0x888e) &&
		(pxmitframe->attrib.dhcp_pkt != 1))
	{
		if (padapter->mlmepriv.LinkDetectInfo.bBusyTraffic == _TRUE)
			rtw_issue_addbareq_cmd(padapter, pxmitframe);
	}
#endif

#if USE_SKB_AS_XMITBUF
	rtw_xmitframe_coalesce(padapter, pxmitframe->pkt, pxmitframe);
#endif

	rtw_enter_critical_bh(&pxmitpriv->lock, &irql);
#if 1 //FIX_XMITFRAME_FAULT, move from rtw_xmit(). 
#ifdef CONFIG_AP_MODE
	if(xmitframe_enqueue_for_sleeping_sta(padapter, pxmitframe) == _TRUE)
	{
		rtw_exit_critical_bh(&pxmitpriv->lock, &irql);
		return 1;		
	}
#endif
#endif
	err = rtw_xmitframe_enqueue(padapter, pxmitframe);
	rtw_exit_critical_bh(&pxmitpriv->lock, &irql);
	
	if (err != _SUCCESS) {
		RT_TRACE(_module_rtl871x_xmit_c_, _drv_err_, ("rtw_xmit_data(): enqueue xmitframe fail\n"));
		rtw_free_xmitframe(pxmitpriv, pxmitframe);

		// Trick, make the statistics correct
		pxmitpriv->tx_pkts--;
		pxmitpriv->tx_drop++;
		return _TRUE;
	}

	rtw_count_tx_stats(padapter, pxmitframe, pxmitframe->attrib.last_txcmdsz);
#ifdef CONFIG_CONCURRENT_MODE
	if (padapter->adapter_type > PRIMARY_ADAPTER){
		padapter = padapter->pbuddy_adapter;
	}
#endif

	rtw_wakeup_task(&padapter->xmitThread);

	return _FALSE;
}

const struct host_ctrl_intf_ops hci_ops = {
	gspi_dvobj_init,
	gspi_dvobj_deinit,
	NULL,
	NULL
};


//TODO
#if 0

unsigned int oob_irq;
static irqreturn_t spi_interrupt_thread(int irq, void *data)
{
	struct dvobj_priv *dvobj;
	PGSPI_DATA pgspi_data;


	dvobj = (struct dvobj_priv*)data;
	pgspi_data = &dvobj->intf_data;

	//spi_int_hdl(padapter);
	if (pgspi_data->priv_wq)
		queue_delayed_work(pgspi_data->priv_wq, &pgspi_data->irq_work, 0);

	return IRQ_HANDLED;
}

static u8 gspi_alloc_irq(struct dvobj_priv *dvobj)
{
	PGSPI_DATA pgspi_data;
	struct spi_device *spi;
	int err;


	pgspi_data = &dvobj->intf_data;
	spi = pgspi_data->func;

	err = request_irq(oob_irq, spi_interrupt_thread,
			IRQF_TRIGGER_FALLING,//IRQF_TRIGGER_HIGH;//|IRQF_ONESHOT,
		       	DRV_NAME, dvobj);
	//err = request_threaded_irq(oob_irq, NULL, spi_interrupt_thread,
	//		IRQF_TRIGGER_FALLING,
	//		DRV_NAME, dvobj);
	if (err < 0) {
		DBG_871X("Oops: can't allocate irq %d err:%d\n", oob_irq, err);
		goto exit;
	}
	enable_irq_wake(oob_irq);
	disable_irq(oob_irq);

exit:
	return err?_FAIL:_SUCCESS;
}

#endif	//#if 0
//TODO
#if 0

static void spi_irq_work(void *data)
{
	struct delayed_work *dwork;
	PGSPI_DATA pgspi;
	struct dvobj_priv *dvobj;


	dwork = container_of(data, struct delayed_work, work);
	pgspi = container_of(dwork, GSPI_DATA, irq_work);

	dvobj = spi_get_drvdata(pgspi->func);
	if (!dvobj->if1) {
		DBG_871X("%s if1 == NULL !!\n", __FUNCTION__);
		return;
	}
	spi_int_hdl(dvobj->if1);
}

#endif	//#if 0


//TODO
#if 0

static int rtw_gspi_suspend(struct spi_device *spi, pm_message_t mesg)
{
	struct dvobj_priv *dvobj = spi_get_drvdata(spi);
	PADAPTER padapter = (_adapter *)dvobj->if1;
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;
	struct net_device *pnetdev = padapter->pnetdev;
	int ret = 0;

	u32 start_time = rtw_get_current_time();

	_func_enter_;

	DBG_871X("==> %s (%s:%d)\n",__FUNCTION__, current->comm, current->pid);

	pwrpriv->bInSuspend = _TRUE;

	while (pwrpriv->bips_processing == _TRUE)
		rtw_msleep_os(1);

	if((!padapter->bup) || (padapter->bDriverStopped)||(padapter->bSurpriseRemoved))
	{
		DBG_871X("%s bup=%d bDriverStopped=%d bSurpriseRemoved = %d\n", __FUNCTION__
			,padapter->bup, padapter->bDriverStopped,padapter->bSurpriseRemoved);
		goto exit;
	}

	rtw_cancel_all_timer(padapter);
	LeaveAllPowerSaveMode(padapter);

	//padapter->net_closed = _TRUE;
	//s1.
	if(pnetdev)
	{
		netif_carrier_off(pnetdev);
		rtw_netif_stop_queue(pnetdev);
	}
#ifdef CONFIG_WOWLAN
	padapter->pwrctrlpriv.bSupportWakeOnWlan=_TRUE;
#else
	//s2.
	//s2-1.  issue rtw_disassoc_cmd to fw
	disconnect_hdl(padapter, NULL);
	//rtw_disassoc_cmd(padapter);
#endif

#ifdef CONFIG_LAYER2_ROAMING_RESUME
	if(check_fwstate(pmlmepriv, WIFI_STATION_STATE) && check_fwstate(pmlmepriv, _FW_LINKED) )
	{
		DBG_871X("%s %s(" MAC_FMT "), length:%d assoc_ssid.length:%d\n",__FUNCTION__,
				pmlmepriv->cur_network.network.Ssid.Ssid,
				MAC_ARG(pmlmepriv->cur_network.network.MacAddress),
				pmlmepriv->cur_network.network.Ssid.SsidLength,
				pmlmepriv->assoc_ssid.SsidLength);

		pmlmepriv->to_roaming = 1;
	}
#endif

	//s2-2.  indicate disconnect to os
	rtw_indicate_disconnect(padapter);
	//s2-3.
	rtw_free_assoc_resources(padapter, 1);

	//s2-4.
	rtw_free_network_queue(padapter, _TRUE);

	rtw_led_control(padapter, LED_CTL_POWER_OFF);

	rtw_dev_unload(padapter);

	if(check_fwstate(pmlmepriv, _FW_UNDER_SURVEY))
		rtw_indicate_scan_done(padapter, 1);

	if(check_fwstate(pmlmepriv, _FW_UNDER_LINKING))
		rtw_indicate_disconnect(padapter);

	// interface deinit
	gspi_deinit(dvobj);
	RT_TRACE(_module_hci_intfs_c_, _drv_notice_, ("%s: deinit GSPI complete!\n", __FUNCTION__));

	rtw_wifi_gpio_wlan_ctrl(WLAN_PWDN_OFF);
	rtw_mdelay_os(1);
exit:
	DBG_871X("<===  %s return %d.............. in %dms\n", __FUNCTION__
		, ret, rtw_get_passing_time_ms(start_time));

	_func_exit_;
	return ret;
}

extern int pm_netdev_open(struct net_device *pnetdev,u8 bnormal);
int rtw_resume_process(_adapter *padapter)
{
	struct net_device *pnetdev;
	struct pwrctrl_priv *pwrpriv;
	u8 is_pwrlock_hold_by_caller;
	u8 is_directly_called_by_auto_resume;
	int ret = 0;
	u32 start_time = rtw_get_current_time();

	_func_enter_;

	DBG_871X("==> %s (%s:%d)\n",__FUNCTION__, current->comm, current->pid);

	rtw_wifi_gpio_wlan_ctrl(WLAN_PWDN_ON);
	rtw_mdelay_os(1);

	rtw_set_chip_endian(adapter);

	if (padapter) {
		pnetdev = padapter->pnetdev;
		pwrpriv = &padapter->pwrctrlpriv;
	} else {
		ret = -1;
		goto exit;
	}

	// interface init
	if (gspi_init(adapter_to_dvobj(padapter)) != _SUCCESS)
	{
		ret = -1;
		RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("%s: initialize SDIO Failed!!\n", __FUNCTION__));
		goto exit;
	}
	rtw_hal_disable_interrupt(padapter);
	if (gspi_alloc_irq(adapter_to_dvobj(padapter)) != _SUCCESS)
	{
		ret = -1;
		RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("%s: gspi_alloc_irq Failed!!\n", __FUNCTION__));
		goto exit;
	}

	rtw_reset_drv_sw(padapter);
	pwrpriv->bkeepfwalive = _FALSE;

	DBG_871X("bkeepfwalive(%x)\n",pwrpriv->bkeepfwalive);
	if(pm_netdev_open(pnetdev,_TRUE) != 0) {
		ret = -1;
		goto exit;
	}

	netif_device_attach(pnetdev);
	netif_carrier_on(pnetdev);

	if( padapter->pid[1]!=0) {
		DBG_871X("pid[1]:%d\n",padapter->pid[1]);
		rtw_signal_process(padapter->pid[1], SIGUSR2);
	}

	#ifdef CONFIG_LAYER2_ROAMING_RESUME
	rtw_roaming(padapter, NULL);
	#endif

	#ifdef CONFIG_RESUME_IN_WORKQUEUE
	rtw_unlock_suspend();
	#endif //CONFIG_RESUME_IN_WORKQUEUE

	pwrpriv->bInSuspend = _FALSE;
exit:
	DBG_871X("<===  %s return %d.............. in %dms\n", __FUNCTION__
		, ret, rtw_get_passing_time_ms(start_time));

	_func_exit_;

	return ret;
}

static int rtw_gspi_resume(struct spi_device *spi)
{
	struct dvobj_priv *dvobj = spi_get_drvdata(spi);
	PADAPTER padapter = (_adapter *)dvobj->if1;
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	 int ret = 0;


	DBG_871X("==> %s (%s:%d)\n",__FUNCTION__, current->comm, current->pid);

	if(pwrpriv->bInternalAutoSuspend ){
 		ret = rtw_resume_process(padapter);
	} else {
#ifdef CONFIG_RESUME_IN_WORKQUEUE
		rtw_resume_in_workqueue(pwrpriv);
#elif defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_ANDROID_POWER)
		if(rtw_is_earlysuspend_registered(pwrpriv)) {
			//jeff: bypass resume here, do in late_resume
			pwrpriv->do_late_resume = _TRUE;
		} else {
			ret = rtw_resume_process(padapter);
		}
#else // Normal resume process
		ret = rtw_resume_process(padapter);
#endif //CONFIG_RESUME_IN_WORKQUEUE
	}

	DBG_871X("<========  %s return %d\n", __FUNCTION__, ret);
	return ret;

}


static struct spi_driver rtw_spi_drv = {
	.probe = rtw_drv_probe,
	.remove = rtw_dev_remove,
	.suspend = rtw_gspi_suspend,
	.resume = rtw_gspi_resume,
	.driver = {
		.name = "wlan_spi",
		.bus = &spi_bus_type,
		.owner = THIS_MODULE,
	}

};

#endif	//#if 0
#endif
