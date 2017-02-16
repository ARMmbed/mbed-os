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
 *******************************************************************************/
#define _GSPI_IO_C_

#include <drv_types.h>

#ifdef CONFIG_SDIO_HCI
#include "wifi_io.h" //from sdio_host driver
//#include <FreeRTOS.h>
//#include <task.h>
//#define SDIO_CMD52_IO

//SDIO host local register space mapping.
#define SDIO_LOCAL_MSK				0x0FFF
#define WLAN_IOREG_MSK 	             	0x7FFF
#define WLAN_FIFO_MSK			      	0x1FFF	// Aggregation Length[12:0]
#define WLAN_RX0FF_MSK			      	0x0003

#define SDIO_WITHOUT_REF_DEVICE_ID		0	// Without reference to the SDIO Device ID
#define SDIO_LOCAL_DEVICE_ID			0	// 0b[16], 000b[15:13]
#define WLAN_TX_HIQ_DEVICE_ID			4	// 0b[16], 100b[15:13]
#define WLAN_TX_MIQ_DEVICE_ID			5	// 0b[16], 101b[15:13]
#define WLAN_TX_LOQ_DEVICE_ID			6	// 0b[16], 110b[15:13]
#define WLAN_RX0FF_DEVICE_ID			7	// 0b[16], 111b[15:13]
#define WLAN_IOREG_DEVICE_ID			8	// 1b[16]

//
//	Description:
//		Query SDIO Local register to query current the number of Free TxPacketBuffer page.
//
//	Assumption:
//		1. Running at PASSIVE_LEVEL
//		2. RT_TX_SPINLOCK is NOT acquired.
//
//	Created by Roger, 2011.01.28.
//
#ifdef CONFIG_RTL8188F
u8 spi_query_status_info(struct dvobj_priv *pdvobj)
{

	ADAPTER *padapter = pdvobj->if1;
	pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX] = rtw_read8(padapter, LOCAL_REG_FREE_TXPG);
	pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX] = rtw_read8(padapter, LOCAL_REG_FREE_TXPG+2);
	pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX] = rtw_read8(padapter, LOCAL_REG_FREE_TXPG+4);
	pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] = rtw_read8(padapter, LOCAL_REG_FREE_TXPG+6);
	RT_TRACE(_module_hci_ops_c_, _drv_notice_,
			("%s: Free page for HIQ(%x),MIDQ(%x),LOWQ(%x),PUBQ(%x)\n",
			__FUNCTION__,
			pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX]));
	//_exit_critical_bh(&phal->SdioTxFIFOFreePageLock, &irql);

	return _TRUE;
}
#else
u8 spi_query_status_info(struct dvobj_priv *pdvobj)
{
	u32 NumOfFreePage;
	ADAPTER *padapter = pdvobj->if1;

	NumOfFreePage = rtw_read32(padapter, LOCAL_REG_FREE_TXPG);

//	_enter_critical_bh(&pdvobj->SdioTxFIFOFreePageLock, &irql);
	rtw_memcpy(pdvobj->SdioTxFIFOFreePage, &NumOfFreePage, 4);

	RT_TRACE(_module_hci_ops_c_, _drv_notice_,
			("%s: Free page for HIQ(%x),MIDQ(%x),LOWQ(%x),PUBQ(%x)\n",
			__FUNCTION__,
			pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX]));
//	_exit_critical_bh(&pdvobj->SdioTxFIFOFreePageLock, &irql);

	return _TRUE;
}
#endif
static unsigned char get_deviceid(unsigned int addr)
{
	unsigned char devideId;
	unsigned short pseudoId;


	pseudoId = (unsigned short)(addr >> 16);
	switch (pseudoId)
	{
		case 0x1025:
			devideId = SDIO_LOCAL_DEVICE_ID;
			break;

		case 0x1026:
			devideId = WLAN_IOREG_DEVICE_ID;
			break;

//		case 0x1027:
//			devideId = SDIO_FIRMWARE_FIFO;
//			break;

		case 0x1031:
			devideId = WLAN_TX_HIQ_DEVICE_ID;
			break;

		case 0x1032:
			devideId = WLAN_TX_MIQ_DEVICE_ID;
			break;

		case 0x1033:
			devideId = WLAN_TX_LOQ_DEVICE_ID;
			break;

		case 0x1034:
			devideId = WLAN_RX0FF_DEVICE_ID;
			break;

		default:
//			devideId = (u8)((addr >> 13) & 0xF);
			devideId = WLAN_IOREG_DEVICE_ID;
			break;
	}

	return devideId;
}

static unsigned int _cvrt2ftaddr(const unsigned int addr, unsigned char *pdeviceId, unsigned short *poffset)
{
	unsigned char deviceId;
	unsigned short offset;
	unsigned int ftaddr;


	deviceId = get_deviceid(addr);
	offset = 0;

	switch (deviceId)
	{
		case SDIO_LOCAL_DEVICE_ID:
			offset = addr & SDIO_LOCAL_MSK;
			break;

		case WLAN_TX_HIQ_DEVICE_ID:
		case WLAN_TX_MIQ_DEVICE_ID:
		case WLAN_TX_LOQ_DEVICE_ID:
			offset = addr & WLAN_FIFO_MSK;
			break;

		case WLAN_RX0FF_DEVICE_ID:
			offset = addr & WLAN_RX0FF_MSK;
			break;

		case WLAN_IOREG_DEVICE_ID:
		default:
			deviceId = WLAN_IOREG_DEVICE_ID;
			offset = addr & WLAN_IOREG_MSK;
			break;
	}
	ftaddr = (deviceId << 13) | offset;

	if (pdeviceId) *pdeviceId = deviceId;
	if (poffset) *poffset = offset;

	return ftaddr;
}

unsigned char sdio_read8(ADAPTER *Adapter, unsigned int addr, int *err)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned char val;
	
_func_enter_;

	psdiodev = adapter_to_dvobj(Adapter);
	ftaddr = _cvrt2ftaddr(addr, NULL, NULL);

	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	val = rtw_sdio_bus_ops.readb(psdiodev->intf_data.func, ftaddr,  err);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);

	if(err && *err)
		DBG_871X( "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);

_func_exit_;

	return val;

}

unsigned short sdio_read16(ADAPTER *Adapter, unsigned int addr, int *err)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned short val;
	
_func_enter_;

	psdiodev = adapter_to_dvobj(Adapter);

	if (addr & 1)
		DBG_871X( "sdio_read16 addr is wrong addr:0x%08x\n", addr);

	ftaddr = _cvrt2ftaddr(addr, NULL, NULL);
	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	val = rtw_sdio_bus_ops.readw(psdiodev->intf_data.func, ftaddr,  err);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);
	
	if(err && *err)
		DBG_871X( "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);
	
_func_exit_;

	return val;
}

unsigned int sdio_read32(ADAPTER *Adapter, unsigned int addr, int *err)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned int val;
	
_func_enter_;

	psdiodev = adapter_to_dvobj(Adapter);

	if (addr & 3)
		DBG_871X( "sdio_read32 addr is wrong addr:0x%08x\n", addr);

	ftaddr = _cvrt2ftaddr(addr, NULL, NULL);
	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	val = rtw_sdio_bus_ops.readl(psdiodev->intf_data.func, ftaddr, err);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);
	if(err && *err)
		DBG_871X( "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);
	
_func_exit_;

	return val;

}

 
unsigned int sdio_write8(ADAPTER *Adapter, unsigned int addr, unsigned int buf, int*err)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned int val = 0;
	
_func_enter_;

	psdiodev = adapter_to_dvobj(Adapter);
	
	ftaddr = _cvrt2ftaddr(addr, NULL, NULL);

	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	rtw_sdio_bus_ops.writeb(psdiodev->intf_data.func, buf&0xFF,ftaddr, err);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);
	
	if(err && *err)
		DBG_871X( "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);
	
_func_exit_;

	return val;
}

unsigned int  sdio_write16(ADAPTER *Adapter, unsigned int addr,unsigned int buf, int *err)
{    
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned int val = 0;
	
_func_enter_;

	psdiodev = adapter_to_dvobj(Adapter);

	if (addr & 1)
		DBG_871X( "sdio_write16 addr is wrong addr:0x%08x\n", addr);
	
	ftaddr = _cvrt2ftaddr(addr, NULL, NULL);
	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	rtw_sdio_bus_ops.writew(psdiodev->intf_data.func, buf&0xFFFF,ftaddr, err);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);
	if(err && *err)
		DBG_871X( "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);
	
_func_exit_;

	return val;
}

unsigned int  sdio_write32(ADAPTER *Adapter, unsigned int addr, unsigned int buf, int *err)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned int val = 0;
	
_func_enter_;

	psdiodev = adapter_to_dvobj(Adapter);

	if (addr & 3)
		DBG_871X( "sdio_write32 addr is wrong addr:0x%08x\n", addr);

	ftaddr = _cvrt2ftaddr(addr, NULL, NULL);
	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	rtw_sdio_bus_ops.writel(psdiodev->intf_data.func, buf&0xFFFFFFFF,ftaddr, err);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);

	if(err && *err)
		DBG_871X( "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);
	
_func_exit_;

	return val;
}

unsigned int _fifoqueue2ftaddr(unsigned int fifo, unsigned int addr)
{
	unsigned int cmdaddr = TX_HIQ_DOMAIN;

	switch(fifo) {
		case TX_LOQ_DOMAIN:
			cmdaddr = ((WLAN_TX_LOQ_DEVICE_ID << 13) | (addr & WLAN_FIFO_MSK));
		 	break;
		case TX_HIQ_DOMAIN:
			cmdaddr = ((WLAN_TX_HIQ_DEVICE_ID << 13) | (addr & WLAN_FIFO_MSK));
			break;				
		case TX_MIQ_DOMAIN:
			cmdaddr = ((WLAN_TX_MIQ_DEVICE_ID << 13) | (addr & WLAN_FIFO_MSK));
			break;
		case RX_RXFIFO_DOMAIN:
			cmdaddr = ((WLAN_RX0FF_DEVICE_ID << 13) | (addr & WLAN_RX0FF_MSK));
			break;
		default:
			cmdaddr = ((WLAN_TX_MIQ_DEVICE_ID << 13) | (addr & WLAN_FIFO_MSK));
			break;		
			
	}
	return cmdaddr;
}

void sdio_write_tx_fifo(ADAPTER *Adapter, unsigned char *buf, int reallen, unsigned int fifo)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned char *mem = buf;
	int free_mem = 0;
	int status;
	unsigned int cnt = (unsigned int)reallen;
	unsigned int page_num = 0;
	unsigned int wait_num = 100;
	unsigned int use_page = 0;

_func_enter_;
	use_page = (cnt + TX_FIFO_PAGE_SZ - 1) / TX_FIFO_PAGE_SZ;
	
	if (cnt > 512)
		cnt = _RND(cnt, 512);
	else
		cnt = _RND(cnt, 4);

	if (((u32)buf) % 4) {
		mem = rtw_zmalloc(cnt);
		while(!mem) {
			DBG_871X("rtw_zmalloc fail, cannot write tx fifo now\n");
			rtw_yield_os();
			mem = rtw_zmalloc(cnt);
		}

		free_mem = 1;
		//DBG_871X("sdio_write_tx_fifo tem_buf:%p ", mem);
		rtw_memcpy(mem, buf, reallen);

	} else {
		mem = buf;
	}

	if (((u32)mem) % 4) {
		DBG_871X("sdio_write_tx_fifo: Oops mem %p not 4 byte Alignment this will cause DMA wrong \n", mem);
	}

	psdiodev = adapter_to_dvobj(Adapter);

	
	if (fifo == TX_HIQ_DOMAIN)
		page_num = psdiodev->SdioTxFIFOFreePage[HI_QUEUE_IDX];
	else if (fifo == TX_LOQ_DOMAIN)
		page_num = psdiodev->SdioTxFIFOFreePage[LOW_QUEUE_IDX];
	else
		page_num = psdiodev->SdioTxFIFOFreePage[MID_QUEUE_IDX];

	if (page_num + psdiodev->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] < use_page) {
		spi_query_status_info(Adapter->dvobj);
	}

	while (page_num + psdiodev->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] < use_page) {
		DBG_871X("Oops: spi_write_tx_fifo(): page_num is %d, padapter->pub_page is %d, wait_num is %d",
			page_num, psdiodev->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX], wait_num);
		
		rtw_msleep_os(1);
		//rtw_udelay_os(20);
		spi_query_status_info(Adapter->dvobj);
		
		if (fifo == TX_HIQ_DOMAIN)
			page_num = psdiodev->SdioTxFIFOFreePage[HI_QUEUE_IDX];
		else if (fifo == TX_LOQ_DOMAIN)
			page_num = psdiodev->SdioTxFIFOFreePage[LOW_QUEUE_IDX];
		else
			page_num = psdiodev->SdioTxFIFOFreePage[MID_QUEUE_IDX];

		if (wait_num <= 2) {
			DBG_871X("%s(): wait_num is <= 2 drop", __FUNCTION__);
			return;
		}
		wait_num --;
	}

	if (fifo == TX_HIQ_DOMAIN) {
		if (use_page <= page_num) {
			psdiodev->SdioTxFIFOFreePage[HI_QUEUE_IDX] -= page_num;
		} else {
			psdiodev->SdioTxFIFOFreePage[HI_QUEUE_IDX] = 0;
			psdiodev->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] -= use_page - psdiodev->SdioTxFIFOFreePage[HI_QUEUE_IDX];
		}
	} else if (fifo == TX_LOQ_DOMAIN) {
		if (use_page <= page_num) {
			psdiodev->SdioTxFIFOFreePage[LOW_QUEUE_IDX] -= page_num;
		} else {
			psdiodev->SdioTxFIFOFreePage[LOW_QUEUE_IDX] = 0;
			psdiodev->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] -= use_page - psdiodev->SdioTxFIFOFreePage[LOW_QUEUE_IDX];
		}
	} else {
		if (use_page <= page_num) {
			psdiodev->SdioTxFIFOFreePage[MID_QUEUE_IDX] -= page_num;
		} else {
			psdiodev->SdioTxFIFOFreePage[MID_QUEUE_IDX] = 0;
			psdiodev->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] -= use_page - psdiodev->SdioTxFIFOFreePage[MID_QUEUE_IDX];
		}
	}

	//must reallen here or tx will wrong when RND(512)
	ftaddr = _fifoqueue2ftaddr(fifo, reallen >> 2);

	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	status = rtw_sdio_bus_ops.memcpy_toio(psdiodev->intf_data.func, ftaddr, mem, cnt);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);

	if (free_mem) {
		rtw_mfree(mem, cnt);
	}

	if (status) {
		DBG_871X("sdio_write_tx_fifo:status:%x ftaddr:%x Length:%d fifo:%x ", status, ftaddr, cnt, fifo);
	} 
	
_func_exit_;

	return;
}

void sdio_read_rx_fifo(ADAPTER *Adapter, unsigned char *buf, int reallen)
{
	struct dvobj_priv *psdiodev;
	unsigned int ftaddr;
	unsigned char *mem = buf;
	int free_mem = 0;
	int status;
	unsigned int cnt = (unsigned int)reallen;
	static unsigned int sdio_rxfifo_cnt = 0;
	unsigned int fifo = RX_RXFIFO_DOMAIN;

_func_enter_;

	if (cnt > 512)
		cnt = _RND(cnt, 512);
	else
		cnt = _RND(cnt, 4);

	mem = rtw_zmalloc(cnt);
	if (mem) {
		free_mem = 1;
		//DBG_871X("sdio_read_rx_fifo tem_buf:%p ", mem);		
	} else {
		//DBG_871X("sdio_read_rx_fifo tem_buf:Oops %p ", mem);
		mem = buf;
	}

	if (mem == NULL) {
		DBG_871X("sdio_read_rx_fifo: Oops mem is NULL \n");
		return;
	}
		
	if (((u32)mem) % 4) {
		DBG_871X("sdio_read_rx_fifo: Oops mem %p not 4 byte Alignment this will cause DMA wrong \n", mem);
	}

	psdiodev = adapter_to_dvobj(Adapter);
	
	ftaddr = _fifoqueue2ftaddr(fifo, sdio_rxfifo_cnt++);
	
	rtw_sdio_bus_ops.claim_host(psdiodev->intf_data.func);
	status = rtw_sdio_bus_ops.memcpy_fromio(psdiodev->intf_data.func, mem, ftaddr, cnt);
	rtw_sdio_bus_ops.release_host(psdiodev->intf_data.func);

	if (free_mem) {
		if (buf)
			rtw_memcpy(buf, mem, reallen);
		rtw_mfree(mem, cnt);
	}

	if (status) {
		//error
		DBG_871X("rtw_sdio_read_rx_fifo error 0x%x\n"
			  "***** Addr = %x *****\n"
			  "***** Length = %d *****\n", status, ftaddr, cnt);
	} 


_func_exit_;

	return;
}


void sdio_cmd52_read(ADAPTER *Adapter, u32 addr, u32 cnt, u8 *pdata, int *err)
{
	int i = 0;

	for (i = 0; i < cnt; i++) {
		pdata[i] = sdio_read8(Adapter, addr + i, err);
		if (err && *err)
			break;
	}
}

void sdio_cmd52_write(ADAPTER *Adapter, u32 addr, u32 cnt, u8 *pdata, int *err)
{
	int i = 0;

	for (i = 0; i < cnt; i++) {
		sdio_write8(Adapter, addr + i, pdata[i], err);
		if (err && *err)
			break;
	}
}

u8 _sdio_read8(struct dvobj_priv *pdvobj, u32 addr, s32 *err)
{
	u8 val;
	ADAPTER *Adapter = pdvobj->if1;

_func_enter_;

	val = sdio_read8(Adapter, addr, err);

_func_exit_;

	return val;
}

u16 _sdio_read16(struct dvobj_priv *pdvobj, u32 addr, s32 *err)
{
	u8 bMacPwrCtrlOn = _FALSE;
	u16 val;
	u8 cmd52_io = 0;
	ADAPTER *Adapter = pdvobj->if1;

_func_enter_;
	/* we should use CMD 52 before bMacPwrCtrlOn */
	rtw_hal_get_hwreg(Adapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	cmd52_io = !bMacPwrCtrlOn;
#ifdef SDIO_CMD52_IO
	cmd52_io = 1;
#endif
	if (cmd52_io) {
		sdio_cmd52_read(Adapter, addr, 2, (u8*)&val, err);
		val = le16_to_cpu(val);
		return val;
	}

	val = sdio_read16(Adapter, addr, err);

_func_exit_;

	return val;
}

u32 _sdio_read32(struct dvobj_priv *pdvobj, u32 addr, s32 *err)
{
	u8 bMacPwrCtrlOn = _FALSE;
	u32 val;
	u8 cmd52_io = 0;
	ADAPTER *Adapter = pdvobj->if1;

_func_enter_;

	/* we should use CMD 52 before bMacPwrCtrlOn */
	rtw_hal_get_hwreg(Adapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	cmd52_io = !bMacPwrCtrlOn;
#ifdef SDIO_CMD52_IO
	cmd52_io = 1;
#endif
	if (cmd52_io) {
		sdio_cmd52_read(Adapter, addr, 4, (u8*)&val, err);
		val = le32_to_cpu(val);
		return val;
	}

	val = sdio_read32(Adapter, addr, err);

_func_exit_;

	return val;
}

s32 _sdio_write8(struct dvobj_priv *pdvobj, u32 addr, u8 val, s32 *err)
{
	ADAPTER *Adapter = pdvobj->if1;

_func_enter_;

	sdio_write8(Adapter, addr, (u32)val, err);

_func_exit_;

	return _SUCCESS;
}

s32 _sdio_write16(struct dvobj_priv *pdvobj, u32 addr, u16 val, s32 *err)
{
	u8 bMacPwrCtrlOn = _FALSE;
	u8 cmd52_io = 0;
	ADAPTER *Adapter = pdvobj->if1;

_func_enter_;
	/* we should use CMD 52 before bMacPwrCtrlOn */
	rtw_hal_get_hwreg(Adapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	cmd52_io = !bMacPwrCtrlOn;
#ifdef SDIO_CMD52_IO
	cmd52_io = 1;
#endif
	if (cmd52_io) {
		val = cpu_to_le16(val);
		sdio_cmd52_write(Adapter, addr, 2, (u8*)&val, err);
		return _SUCCESS;
	}

	sdio_write16(Adapter, addr, (u32)val, err);

_func_exit_;

	return _SUCCESS;
}

s32 _sdio_write32(struct dvobj_priv *pdvobj, u32 addr, u32 val, s32 *err)
{
	u8 bMacPwrCtrlOn = _FALSE;
	u8 cmd52_io = 0;
	ADAPTER *Adapter = pdvobj->if1;

_func_enter_;
	/* we should use CMD 52 before bMacPwrCtrlOn */
	rtw_hal_get_hwreg(Adapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	cmd52_io = !bMacPwrCtrlOn;
#ifdef SDIO_CMD52_IO
	cmd52_io = 1;
#endif
	if (cmd52_io) {
		val = cpu_to_le32(val);
		sdio_cmd52_write(Adapter, addr, 4, (u8*)&val, err);
		return _SUCCESS;
	}

	sdio_write32(Adapter, addr, val, err);

_func_exit_;

	return _SUCCESS;
}

/*
 * Description:
 *	Read from RX FIFO
 *	Round read size to block size,
 *	and make sure data transfer will be done in one command.
 *
 * Parameters:
 *	pintfhdl	a pointer of intf_hdl
 *	addr		port ID
 *	cnt			size to read
 *	rmem		address to put data
 *
 * Return:
 *	_SUCCESS(1)		Success
 *	_FAIL(0)		Fail
 */
static int _sdio_read_rx_fifo(struct dvobj_priv *pdvobj, u32 addr, u8 *mem, u32 cnt, struct fifo_more_data *pmore_data)
{
	ADAPTER *Adapter = pdvobj->if1;

	//struct spi_more_data more_data = {0};

	//DBG_8192C("%s \n", __func__);
	rtw_memset(pmore_data, 0, sizeof(struct fifo_more_data));

	sdio_read_rx_fifo(Adapter, mem, cnt);

	return _SUCCESS;
}
	
/*
 *	Description:
 *		Translate sdio fifo address to Domain ID in each WLAN FIFO
 */
static u32 hwaddr2txfifo(u32 addr)
{
	u32 fifo_domain_id;
	switch (addr)
	{
		case WLAN_TX_HIQ_DEVICE_ID:
				fifo_domain_id = TX_HIQ_DOMAIN;
				break;

		case WLAN_TX_MIQ_DEVICE_ID:
				fifo_domain_id = TX_MIQ_DOMAIN;
				break;

		case WLAN_TX_LOQ_DEVICE_ID:
				fifo_domain_id = TX_LOQ_DOMAIN;
				break;
		default:
				fifo_domain_id = TX_LOQ_DOMAIN;
				break;
	}

	return fifo_domain_id;
}


static int _sdio_write_tx_fifo(struct dvobj_priv *pdvobj, u32 addr, u8 *mem, u32 cnt)
{
	u8 remain_len = 0;
	u32 w_sz = cnt;
	ADAPTER *Adapter = pdvobj->if1;

	remain_len = w_sz%4;
	if (remain_len != 0)
		w_sz += 4 -remain_len;

#if 0//ndef LZM_TEST
		if (1) {
			int i = 0;
			for(i = 0; i < w_sz; i += 4) {
				DBG_871X("_sdio_write_port[%d]: 0x%08x ", i,  *(u32*)(mem + i));
			}
		}
#endif

#if 0
	{
		static u32 write_test = 0;
		u32 now_time = 0;
		write_test++;
		if(write_test==1000) {
			now_time = xTaskGetTickCount() * portTICK_RATE_MS;
			DBG_8192C("%s fifo:%d cnt:%d w_sz:%d mem:%p, now time:%d\n", __func__, addr, cnt, w_sz, mem, now_time);
			write_test = 0;
		}
	}
#endif
	sdio_write_tx_fifo(Adapter, mem, w_sz, hwaddr2txfifo(addr));

	return _SUCCESS;
}

static int sdio_io_priv(struct dvobj_priv *pdvobj)
{	
	ADAPTER *Adapter = pdvobj->if1;
	//struct dvobj_priv *pdvobj = &Adapter->dvobjpriv;

	return _SUCCESS;
}

void sdio_set_intf_ops(struct _io_ops	*pops)
{
	pops->init_io_priv = &sdio_io_priv;
	pops->write8_endian = NULL;
	
	pops->_read8 = &_sdio_read8;
	pops->_read16 = &_sdio_read16;
	pops->_read32 = &_sdio_read32;

	pops->_write8 = &_sdio_write8;
	pops->_write16 = &_sdio_write16;
	pops->_write32 = &_sdio_write32;

	pops->read_rx_fifo = &_sdio_read_rx_fifo;
	pops->write_tx_fifo = &_sdio_write_tx_fifo;
}

#endif
