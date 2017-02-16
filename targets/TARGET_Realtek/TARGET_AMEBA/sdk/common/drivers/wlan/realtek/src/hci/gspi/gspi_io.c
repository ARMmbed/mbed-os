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

#ifdef CONFIG_GSPI_HCI

u8 spi_read8(struct dvobj_priv *pdvobj, u32 addr, s32 *err);
u16 spi_read16(struct dvobj_priv *pdvobj, u32 addr, s32 *err);
u32 spi_read32(struct dvobj_priv *pdvobj, u32 addr, s32 *err);
s32 spi_write8(struct dvobj_priv *pdvobj, u32 addr, u8 buf, s32 *err);
s32 spi_write16(struct dvobj_priv *pdvobj, u32 addr, u16 buf, s32 *err);
s32 spi_write32(struct dvobj_priv *pdvobj, u32 addr, u32 buf, s32 *err);

static u32 rtw_spi_transfer(
	struct dvobj_priv *pdvobj,
	bool			pool,
	u8* 			buf, 	
	u32	 		buf_len)
{
	_mutex		*spi_mutex;
	u32			ret_value = _SUCCESS;

	spi_mutex = &(pdvobj->intf_data.spi_mutex);
	
	rtw_enter_critical_mutex(spi_mutex, NULL);
	if(!WLAN_BSP_Transfer(buf, buf_len))
		ret_value = _FAIL;
	rtw_exit_critical_mutex(spi_mutex, NULL);

	return ret_value;
}
static int addr_convert(u32 addr)
{
	u32 domain_id = 0 ;
	u32 temp_addr = addr&0xffff0000;

	if (temp_addr == 0 ) {
		domain_id = WLAN_IOREG_DOMAIN;
		return domain_id;
	}

	switch (temp_addr) {
	case WLAN_LOCAL_OFFSET:
		domain_id = SPI_LOCAL_DOMAIN;
		break;
	case WLAN_IOREG_OFFSET:
		domain_id = WLAN_IOREG_DOMAIN;
		break;
	case FW_FIFO_OFFSET:
		domain_id = FW_FIFO_DOMAIN;
		break;
	case TX_HIQ_OFFSET:
		domain_id = TX_HIQ_DOMAIN;
		break;
	case TX_MIQ_OFFSET:
		domain_id = TX_MIQ_DOMAIN;
		break;
	case TX_LOQ_OFFSET:
		domain_id = TX_LOQ_DOMAIN;
		break;
	case RX_RXOFF_OFFSET:
		domain_id = RX_RXFIFO_DOMAIN;
		break;
	default:
		break;
	}

	return domain_id;
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

static u32 buf_endian_reverse(u32 src)
{
	return (((src&0x000000ff)<<24)|((src&0x0000ff00)<<8)|
		((src&0x00ff0000)>>8)|((src&0xff000000)>>24));
}

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
	pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX] = spi_read8(pdvobj, LOCAL_REG_FREE_TXPG, NULL);
	pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX] =spi_read8(pdvobj, LOCAL_REG_FREE_TXPG+2, NULL);
	pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX] = spi_read8(pdvobj, LOCAL_REG_FREE_TXPG+4, NULL);
	pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] = spi_read8(pdvobj, LOCAL_REG_FREE_TXPG+6, NULL);
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

	NumOfFreePage = spi_read32(pdvobj, LOCAL_REG_FREE_TXPG, NULL);

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
static void spi_get_status_info(struct dvobj_priv *pdvobj, unsigned char *status)
{
#ifdef CONFIG_MEMORY_ACCESS_ALIGNED
	u32 local_status[2];
	u8 *pstatus = (u8*)(&local_status[0]);

	memcpy(pstatus, status, GSPI_STATUS_LEN); 
#else
	u8 *pstatus = status;
#endif

#ifdef CONFIG_RTL8188F
	pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] = GET_STATUS_PUB_PAGE_NUM(pstatus)*2;
#else
	pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] = GET_STATUS_PUB_PAGE_NUM(pstatus);
#endif

	pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX] = GET_STATUS_HI_PAGE_NUM(pstatus);
	pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX] = GET_STATUS_MID_PAGE_NUM(pstatus);
	pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX] = GET_STATUS_LOW_PAGE_NUM(pstatus);

	RT_TRACE(_module_hci_ops_c_, _drv_dump_,
			("%s: Free page for HIQ(%x),MIDQ(%x),LOWQ(%x),PUBQ(%x)\n",
			__FUNCTION__,
			pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX],
			pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX]));
}

static int spi_read_write_reg(struct dvobj_priv *pdvobj, int  write_flag, u32 addr, char * buf, int len, u32 eddien)
{
	int  fun = 1, domain_id = 0x0; //LOCAL
	unsigned int cmd = 0 ;
	int byte_en = 0 ;//,i = 0 ;
	int ret = 1;
	unsigned char status[8] = {0};
	unsigned int data_tmp = 0;
	u32 force_bigendian = eddien;

	u32 spi_buf[4] = {0};
	bool polled = TRUE;

	if (len!=1 && len!=2 && len != 4) {
		return -1;
	}

	domain_id = addr_convert(addr);

	addr &= 0x7fff;
	len &= 0xff;
	if (write_flag) //write register
	{
		int remainder = addr % 4;
		u32 val32 = *(u32 *)buf;
		switch(len) {
		case 1:
			byte_en = (0x1 << remainder);
			data_tmp = (val32& 0xff)<< (remainder*8);
			break;
		case 2:
			byte_en = (0x3 << remainder);
			data_tmp = (val32 & 0xffff)<< (remainder*8);
			break;
		case 4:
			byte_en = 0xf;
			data_tmp = val32 & 0xffffffff;
			break;
		default:
			byte_en = 0xf;
			data_tmp = val32 & 0xffffffff;
			break;
		}
	}
	else //read register
	{
		switch(len) {
		case 1:
		    	byte_en = 0x1;
		 	break;
		case 2:
		    	byte_en = 0x3;
		   	break;
		case 4:
		 	byte_en = 0xf;
		    	break;
		default:
		 	byte_en = 0xf;
			break;
		}

		if(domain_id == SPI_LOCAL_DOMAIN)
			byte_en = 0;
	}

	//addr = 0xF0 4byte: 0x2800f00f
	REG_LEN_FORMAT(&cmd, byte_en);
	REG_ADDR_FORMAT(&cmd, (addr&0xfffffffc));
	REG_DOMAIN_ID_FORMAT(&cmd, domain_id);
	REG_FUN_FORMAT(&cmd, fun);
	REG_RW_FORMAT(&cmd, write_flag);

	if (force_bigendian) {
		cmd = buf_endian_reverse(cmd);
	}

	if (!write_flag && (domain_id!= RX_RXFIFO_DOMAIN)) {
		u32 read_data = 0;

		rtw_memset(spi_buf, 0x00, sizeof(spi_buf));

		spi_buf[0] = cmd;
		spi_buf[1] = 0;
		spi_buf[2] = 0;
		spi_buf[3] = 0;	

		rtw_spi_transfer(pdvobj, polled, (u8*)spi_buf, sizeof(spi_buf));

		rtw_memcpy(status, (u8 *) &spi_buf[1], sizeof(status));
		read_data = EF4Byte(spi_buf[3]);

		//add for 8810
#ifdef CONFIG_BIG_ENDIAN
		if (!force_bigendian)
			read_data = buf_endian_reverse(read_data);
#else
		if (force_bigendian)
			read_data = buf_endian_reverse(read_data);
#endif
		*(u32*)buf = read_data;
	} else if (write_flag ) {

#ifdef CONFIG_BIG_ENDIAN
		if (!force_bigendian)
			data_tmp = buf_endian_reverse(data_tmp);
#else
		if (force_bigendian)
			data_tmp = buf_endian_reverse(data_tmp);
#endif

		spi_buf[0] = cmd;
		spi_buf[1] = data_tmp;
		spi_buf[2] = 0;
		spi_buf[3] = 0;
		
		rtw_spi_transfer(pdvobj, polled, (u8*)spi_buf, sizeof(spi_buf));

		rtw_memcpy(status, (u8 *) &spi_buf[2], sizeof(status));
	}

	spi_get_status_info(pdvobj, (unsigned char*)status);

	return ret;
}

static int spi_io_priv(struct dvobj_priv *pdvobj)
{	
	//struct dvobj_priv *pdvobj = &Adapter->dvobjpriv;

	return _SUCCESS;
}

static int spi_write8_endian(struct dvobj_priv *pdvobj, u32 addr, u32 buf, u32 big)
{
	return spi_read_write_reg(pdvobj,1,addr,(char *)&buf,1, big);
}

u8 spi_read8(struct dvobj_priv *pdvobj, u32 addr, s32 *err)
{
	u32 ret = 0;
	int val32 = 0 , remainder = 0 ;
	s32 _err = 0;

	 _err = spi_read_write_reg(pdvobj,0,addr&0xFFFFFFFC,(char *)&ret,4,0);
	remainder = addr % 4;
	val32 = ret;
	val32 = (val32& (0xff<< (remainder<<3)))>>(remainder<<3);

	if (err)
		*err = _err;

	return (u8)val32;

}

u16 spi_read16(struct dvobj_priv *pdvobj, u32 addr, s32 *err)
{
   	u32 ret = 0;
	int val32 = 0 , remainder = 0 ;
	s32 _err = 0;

	_err = spi_read_write_reg(pdvobj,0,addr&0xFFFFFFFC,(char *)&ret,4,0);
	remainder = addr % 4;
	val32 = ret;
	val32 = (val32& (0xffff<< (remainder<<3)))>>(remainder<<3);

	if (err)
		*err = _err;

	return (u16)val32;
}

u32 spi_read32(struct dvobj_priv *pdvobj, u32 addr, s32 *err)
{
	u32 ret = 0;
	s32 _err = 0;

	_err = spi_read_write_reg(pdvobj,0,addr&0xFFFFFFFC,(char *)&ret,4,0);
	if (err)
		*err = _err;

	return  ret;
}

s32 spi_write8(struct dvobj_priv *pdvobj, u32 addr, u8 buf, s32 *err)
{
	int ret = 0;

	ret = spi_read_write_reg(pdvobj,1,addr,(char *)&buf,1,0);
	if (err)
		*err = ret;
	return ret;
}

s32 spi_write16(struct dvobj_priv *pdvobj, u32 addr, u16 buf, s32 *err)
{
	int ret = 0;

	ret = spi_read_write_reg(pdvobj,1,addr,(char *)&buf,2,0);
	if (err)
		*err = ret;
	return ret;
}

s32 spi_write32(struct dvobj_priv *pdvobj, u32 addr, u32 buf, s32 *err)
{
	int	ret = 0;

	ret = spi_read_write_reg(pdvobj, 1,addr,(char *)&buf,4,0);
	if (err)
		*err = ret;
	return ret;
}

static int spi_read_rx_fifo(struct dvobj_priv *pdvobj, u32 addr, u8 *buf, u32 len, struct fifo_more_data *pmore_data)
{
	int fun = 1, domain_id = RX_RXFIFO_DOMAIN;
	unsigned int cmd = 0;
	unsigned char *status = buf + len;
	u8 *spi_buf = (u8 *) (buf - GSPI_CMD_LEN);
	int spi_buf_len = 0;
	bool polled = TRUE;
	bool use_alloc = FALSE;
	u32 max_skb_len = 0;

#ifndef CONFIG_DONT_CARE_TP
	max_skb_len = MAX_SKB_BUF_SIZE;
#else
	max_skb_len = MAX_RX_SKB_BUF_SIZE;
#endif

	if(((GSPI_CMD_LEN + len + GSPI_STATUS_LEN) > max_skb_len) || (!buf)) {
	  #if !defined(CONFIG_MP_INCLUDED) || !defined(CONFIG_MP_IWPRIV_SUPPORT) // Cloud 2013/09/06
		DBG_871X("data len=%d, MAX_SKB_BUF_SIZE(%d) is not enough, change to dynamic alloc\n", len, max_skb_len);
	  #endif
		use_alloc = TRUE;
		spi_buf_len = GSPI_CMD_LEN + len + GSPI_STATUS_LEN;
		spi_buf = rtw_malloc(spi_buf_len);

		if(spi_buf == NULL) {
			DBG_871X("Failed to alloc %d bytes\n", len);
			return _FAIL;
		}
		else {
			buf = spi_buf + GSPI_CMD_LEN;
			status = spi_buf + GSPI_CMD_LEN + len;
		}
	}

	FIFO_LEN_FORMAT(&cmd, len); //TX Agg len
	FIFO_DOMAIN_ID_FORMAT(&cmd, domain_id);
	FIFO_FUN_FORMAT(&cmd, fun);
	FIFO_RW_FORMAT(&cmd, 0); //read

	rtw_memset(status, 0x00, GSPI_STATUS_LEN);
	rtw_memset(buf, 0x0, len);

#ifdef CONFIG_MEMORY_ACCESS_ALIGNED
	memcpy(spi_buf, (u8 *)&cmd, sizeof(int));
#else
	*((u32 *) spi_buf) = cmd;
#endif

	rtw_spi_transfer(pdvobj, polled, (u8 *) spi_buf, GSPI_CMD_LEN + len + GSPI_STATUS_LEN);

	spi_get_status_info(pdvobj, status);
	pmore_data->more_data = GET_STATUS_HISR_LOW8BIT(status) & BIT(0);
	pmore_data->len = GET_STATUS_RX_LENGTH(status);

	if(use_alloc) {
		//Drop the data
		rtw_mfree(spi_buf, spi_buf_len);
		return _FAIL;
	}

	return _SUCCESS;
}

static int spi_write_tx_fifo(struct dvobj_priv *pdvobj, u32 addr, u8 *buf, u32 len)
{
	int fun = 1; //TX_HIQ_FIFO
	unsigned int cmd = 0;
	unsigned char *status = buf + len;
	u8 *spi_buf = (u8 *) (buf - GSPI_CMD_LEN);
	u32 page_num = 0;
	u32 wait_num = 100;
	bool polled = TRUE;
	u32 fifo = 0;

_func_enter_;
	fifo = hwaddr2txfifo(addr);

	spi_query_status_info(pdvobj);
	if (fifo == TX_HIQ_DOMAIN)
		page_num = pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX];
	else if (fifo == TX_LOQ_DOMAIN)
		page_num = pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX];
	else
		page_num = pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX];

	while (page_num + pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX] < 15) {
		DBG_871X("Oops: spi_write_tx_fifo(): page_num is %d, padapter->pub_page is %d, wait_num is %d",
			page_num, pdvobj->SdioTxFIFOFreePage[PUBLIC_QUEUE_IDX], wait_num);
		
		rtw_msleep_os(1);
		//rtw_udelay_os(20);
		spi_read32(pdvobj, 0x608, NULL);
		
		if (fifo == TX_HIQ_DOMAIN)
			page_num = pdvobj->SdioTxFIFOFreePage[HI_QUEUE_IDX];
		else if (fifo == TX_LOQ_DOMAIN)
			page_num = pdvobj->SdioTxFIFOFreePage[LOW_QUEUE_IDX];
		else
			page_num = pdvobj->SdioTxFIFOFreePage[MID_QUEUE_IDX];

		if (wait_num <= 2) {
			DBG_871X("%s(): wait_num is <= 2 drop", __FUNCTION__);
			return _FAIL;
		}
		wait_num --;
	}

	FIFO_LEN_FORMAT(&cmd, len); //TX Agg len
	FIFO_DOMAIN_ID_FORMAT(&cmd, fifo);
	FIFO_FUN_FORMAT(&cmd, fun);
	FIFO_RW_FORMAT(&cmd, (unsigned int) 1);	//write

	//DBG_871X("%s(): len = %d\n", __FUNCTION__, len);
	//RT_PRINT_DATA(_module_hal_xmit_c_, _drv_always_, "Tx:\n", buf, GSPI_CMD_LEN + len);
	rtw_memset(status, 0x00, GSPI_STATUS_LEN);

#ifdef CONFIG_MEMORY_ACCESS_ALIGNED
	memcpy(spi_buf, (u8 *)&cmd, sizeof(int));
#else
	*((u32 *) spi_buf) = cmd;
#endif

	rtw_spi_transfer(pdvobj, polled, (u8 *) spi_buf, GSPI_CMD_LEN + len + GSPI_STATUS_LEN);

	spi_get_status_info(pdvobj, status);

_func_exit_;

	return _SUCCESS;
}

void spi_set_intf_ops(struct _io_ops	*pops)
{
	pops->init_io_priv = &spi_io_priv;
	pops->write8_endian = &spi_write8_endian;
	
	pops->_read8 = &spi_read8;
	pops->_read16 = &spi_read16;
	pops->_read32 = &spi_read32;

	pops->_write8 = &spi_write8;
	pops->_write16 = &spi_write16;
	pops->_write32 = &spi_write32;

	pops->read_rx_fifo = &spi_read_rx_fifo;
	pops->write_tx_fifo = &spi_write_tx_fifo;
}
#endif
