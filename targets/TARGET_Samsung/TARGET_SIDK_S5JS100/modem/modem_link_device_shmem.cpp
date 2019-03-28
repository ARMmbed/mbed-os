/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "modem_link_device_shmem.h"
#include "modem_io_device.h"
#include "modem_util.h"
#include "modem_prj.h"
#include "s5js100_error.h"
#include "s5js100_type.h"
#include <string.h>

#ifndef IPCRAM_PADDR
#define IPCRAM_PADDR 0x10000000
#endif
#ifndef IPC_REGION_SIZE
#define IPC_REGION_SIZE SZ_64K
#endif
#if IPC_REGION_SIZE != SZ_64K
#error "Check IPC_REGION_SIZE! Refer to struct shmem_64KB_phys_map"
#endif

#include "mbed_trace.h"
#define TRACE_GROUP "SMEM"

#ifndef MODEM_LINK_DEVICE_SHMEM_DBG_ON
#define MODEM_LINK_DEVICE_SHMEM_DBG_ON		0
#endif
#define MODEM_LINK_DEVICE_SHMEM_DBG		if (MODEM_LINK_DEVICE_SHMEM_DBG_ON) tr_info

static void shmem_irq_handler(void *data);
static void shmem_save_irq_handler(void *data);
static void shmem_stop_req_irq_handler(void *data);
int modem_link_device_thread_start(void);

ShmemLinkDevice *pShmemLinkDevice;

EventQueue *modem_link_device_wqueue;
Thread *msgrxwork;
Thread *irqwork;
Semaphore modem_link_device_sem(0, 512);

static bool circ_valid(unsigned int qsize, unsigned int in, unsigned int out)
{
	if (in >= qsize)
		return false;

	if (out >= qsize)
		return false;

	return true;
}

static unsigned int circ_get_space(unsigned int qsize, unsigned int in, unsigned int out)
{
	return (in < out) ? (out - in - 1) : (qsize + out - in - 1);
}

static unsigned int circ_get_usage(unsigned int qsize, unsigned int in, unsigned int out)
{
	return (in >= out) ? (in - out) : (qsize - out + in);
}

static unsigned int circ_new_pointer(unsigned int qsize, unsigned int p, unsigned int len)
{
	p += len;
	return (p < qsize) ? p : (p - qsize);
}


/**
 * circ_read
 * @dst: start address of the destination buffer
 * @src: start address of the buffer in a circular queue
 * @qsize: size of the circular queue
 * @out: offset to read
 * @len: length of data to be read
 *
 * Should be invoked after checking data length
 */
static void circ_read(unsigned char *dst, unsigned char *src, unsigned int qsize, unsigned int out, unsigned int len)
{
	uint32_t len1;

	if (out >= qsize)
		out -= qsize;

	if ((out + len) <= qsize) {
		/* ----- (out)         (in) ----- */
		/* -----   7f 00 00 7e      ----- */
		memcpy(dst, (src + out), len);
	} else {
		/*       (in) ----------- (out)   */
		/* 00 7e      -----------   7f 00 */

		/* 1) data start (out) ~ buffer end */
		len1 = qsize - out;
		memcpy(dst, (src + out), len1);

		/* 2) buffer start ~ data end (in?) */
		memcpy((dst + len1), src, (len - len1));
	}
}

/**
 * circ_write
 * @dst: pointer to the start of the circular queue
 * @src: pointer to the source
 * @qsize: size of the circular queue
 * @in: offset to write
 * @len: length of data to be written
 *
 * Should be invoked after checking free space
 */
static void circ_write(unsigned char *dst, unsigned char *src, unsigned int qsize, unsigned int in, unsigned int len)
{
	unsigned int space;

	if ((in + len) < qsize) {
		/*       (in) ----------- (out)   */
		/* 00 7e      -----------   7f 00 */
		memcpy((dst + in), src, len);
	} else {
		/* ----- (out)         (in) ----- */
		/* -----   7f 00 00 7e      ----- */

		/* 1) space start (in) ~ buffer end */
		space = qsize - in;
		memcpy((dst + in), src, ((len > space) ? space : len));

		/* 2) buffer start ~ data end */
		if (len > space)
			memcpy(dst, (src + space), (len - space));
	}
}



unsigned int ShmemIpcDevice::GetTxqHead(void)
{
	return getreg32(txq.head);
}

unsigned int ShmemIpcDevice::GetTxqTail(void)
{
	return getreg32(txq.tail);
}

unsigned int ShmemIpcDevice::GetRxqHead(void)
{
	return getreg32(rxq.head);
}

unsigned int ShmemIpcDevice::GetRxqTail(void)
{
	return getreg32(rxq.tail);
}

int ShmemIpcDevice::get_txq_space(shmem_circ_status *circ)
{
	int cnt = 0;
	unsigned int qsize;
	unsigned int head;
	unsigned int tail;
	int space;

	while (1) {
		qsize = txq.size;
		head = getreg32(txq.head);
		tail = getreg32(txq.tail);
		space = circ_get_space(qsize, head, tail);

		if (circ_valid(qsize, head, tail))
			break;

		cnt++;
		if (cnt >= 1) {
			space = -5;
			break;
		}

		osDelay(1);
	}

	circ->buff = txq.buff;
	circ->qsize = qsize;
	circ->in = head;
	circ->out = tail;
	circ->size = space;

	return space;
}

void ShmemIpcDevice::write_ipc_to_txq(shmem_circ_status *circ, mio_buf *mio)
{
	unsigned int qsize = circ->qsize;
	unsigned int in = circ->in;
	unsigned char *buff = circ->buff;
	unsigned char *header = (unsigned char *)(mio->header);
	unsigned char *src = (unsigned char *)(mio->data);
	unsigned int len = mio->len;
	unsigned int pad = mio->pad;

	/* Write data to the TXQ */
	circ_write(buff, header, qsize, in, EXYNOS_HEADER_SIZE);
	in += EXYNOS_HEADER_SIZE;
	if (in >= qsize) in -= qsize;
	circ_write(buff, src, qsize, in, len + pad);

	/* Update new head (in) pointer */
	putreg32(circ_new_pointer(qsize, circ->in, EXYNOS_HEADER_SIZE + len + pad), txq.head);
}

int ShmemIpcDevice::xmit_ipc_msg(mio_buf *msg)
{
	shmem_circ_status circ;
	int space;
	int copied = 0;
	bool chk_nospc = false;
#if MODEM_LINK_DEVICE_SHMEM_DBG_ON
	int i;
	char mm[96] = "";
#endif

	MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d", __func__, __LINE__);
#if MODEM_LINK_DEVICE_SHMEM_DBG_ON
	MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d ch[%d] len[%d]", __func__, __LINE__, msg->ch, msg->len);
	for (i = 0; i < msg->len; i++) {
		sprintf(mm + (i % 16) * 3, "%02X ", msg->data[i]);
		//MODEM_LINK_DEVICE_SHMEM_DBG("%02X ", msg->data[i]);
		if (i % 16 == 15 && i != 0)
			MODEM_LINK_DEVICE_SHMEM_DBG("%s", mm);
	}
	if ((i - 1) % 16 != 15)
		MODEM_LINK_DEVICE_SHMEM_DBG("%s", mm);
#endif


	/* Acquire the spin lock for a TXQ */
	tx_lock->lock();

	/* Get the size of free space in the TXQ */
	space = get_txq_space(&circ);
	if (space < 0) {
		/* Empty out the TXQ */
		copied = -EIO;
		return copied;//break;
	}

	/* Check the free space size,
	  - FMT : comparing with mxb->len
	  - RAW : check used buffer size  */
	chk_nospc = (space < msg->len) ? true : false;

	if (chk_nospc) {
		printf("ERROR Not implement %s:%d\n", __func__, __LINE__);
		while(1);
		/* Set res_required flag for the "dev" */
		res_required = 1;

		/* Take the mxb back to the mxb_txq */
		//mbuf_queue_head(txq, mxb);

		copied = -ENOSPC;
		return copied;//break;
	}

	/* TX only when there is enough space in the TXQ */
	write_ipc_to_txq(&circ, msg);
	copied += msg->len;
	free_mio_buf(msg);

	/* Release the spin lock */
	tx_lock->unlock();

	return copied;
}

int ShmemIpcDevice::send(mio_buf *buf)
{
	int ret;

	MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d", __func__, __LINE__);
	ret = xmit_ipc_msg(buf);
	if (ret > 0) {
		MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d", __func__, __LINE__);
		pShmemLinkDevice->send_int2cp(INT_NON_CMD(mask_send));
	}

	return 0;
}

void ShmemIpcDevice::send_cmd(unsigned short cmd)
{
	ShmemLinkDevice *sld = pShmemLinkDevice;

	sld->send_int2cp(cmd);
}

void ShmemIpcDevice::save_release(void)
{
	ShmemLinkDevice *sld = pShmemLinkDevice;

	sld->save_sem_release();
}

int ShmemIpcDevice::ioctl(unsigned int cmd, unsigned long arg)
{
	return 0;
}

int ShmemIpcDevice::check_security(unsigned long arg)
{
	return 0;
}

int ShmemIpcDevice::sec_init(unsigned long arg)
{
	return 0;
}


unsigned short ShmemLinkDevice::get_mbx_cp2ap_msg(void)
{
	return mbox_get_value(mbox.mbx_cp2ap_msg);
}

unsigned short ShmemLinkDevice::get_mbx_ap2cp_msg(void)
{
	return mbox_get_value(mbox.mbx_ap2cp_msg);
}

unsigned int ShmemLinkDevice::get_mbx_magic(void)
{
	unsigned int ret = getreg32(shmem_magic);
	return ret;
}

unsigned int ShmemLinkDevice::get_mbx_access(void)
{
	unsigned int ret = getreg32(shmem_access);
	return ret;
}

void ShmemLinkDevice::send_int2cp(unsigned short mask)
{
	mbox_set_value(mbox.mbx_ap2cp_msg, mask);
	mbox_set_interrupt(mbox.int_ap2cp_msg);
}

void ShmemLinkDevice::save_sem_release()
{
	save_sem->release();
}

int ShmemLinkDevice::init(void)
{
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	if (get_mbx_magic() == SHM_IPC_MAGIC && get_mbx_access() == 1 && state == STATE_ONLINE)
		return 0;	// Already initialized

	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	putreg32(0, shmem_access);
	putreg32(SHM_IPC_MAGIC, shmem_magic);
	putreg32(1, shmem_access);
	state = STATE_ONLINE;

	return 0;
}


ShmemLinkDevice::ShmemLinkDevice()
{
	ipc_device[IPC_FMT] = ShmemIpcFmtDevice::getInstance();
	ipc_device[IPC_RAW] = ShmemIpcRawDevice::getInstance();

	shmem_size = IPC_REGION_SIZE;
	shmem_base = (void *)IPCRAM_PADDR;
	shmem_magic = (unsigned int *) & (((struct shmem_64KB_phys_map *)IPCRAM_PADDR)->magic);
	shmem_access = (unsigned int *) & (((struct shmem_64KB_phys_map *)IPCRAM_PADDR)->access);
	memset(shmem_base, 0, shmem_size);

	/* Initialize locks, completions, and bottom halves */
	save_sem = new Semaphore(0, 1);
	stop_sem = new Semaphore(0, 1);
	ipc_rx_sem = new Semaphore(0, 512);

	mbox_ipc_register_handler(mbox.irq_cp2ap_msg, shmem_irq_handler, NULL);
	mbox_ipc_register_handler(mbox.save_irq, shmem_save_irq_handler, NULL);
	mbox_ipc_register_handler(mbox.stop_irq, shmem_stop_req_irq_handler, NULL);

	modem_link_device_thread_start();
}
ShmemLinkDevice::~ShmemLinkDevice()
{
	printf("need to add code here ,%s\n\r", __func__);
}

int ShmemLinkDevice::init_shmem_save(void)
{
	int i;

	if (get_mbx_magic() == SHM_SAVE_MAGIC && get_mbx_access() == 1)
		return 0;

	putreg32(0, shmem_access);

	for (i = 0; i < MAX_IPC_DEV; i++) {
		putreg32(0, ipc_device[i]->txq.head);
		putreg32(0, ipc_device[i]->txq.tail);
		putreg32(0, ipc_device[i]->rxq.head);
		putreg32(0, ipc_device[i]->rxq.tail);
		ipc_device[i]->res_required = 0;
	}

	putreg32(SHM_SAVE_MAGIC, shmem_magic);
	putreg32(1, shmem_access);

	if (get_mbx_magic() != SHM_SAVE_MAGIC || get_mbx_access() != 1)
		return -13;

	state = STATE_NV_CS_SAVE;

	return 0;
}

/**
 * cmd_phone_start_handler
 *
 * Handles the PHONE_START command from a CP.
 */

void rx_cmd_phone_start(void)
{
	ShmemLinkDevice *ld = pShmemLinkDevice;
	int i;

	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);
	ld->init();
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	for (i = 0; i < MAX_IPC_DEV; i++)
		ld->ipc_device[i]->init();
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	ld->send_int2cp(INT_CMD(INT_CMD_INIT_END));
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);
}

void rx_cmd_cp_crash(void)
{
	printf("\n!!!!!CP Crash!!!!!\n");
	MBED_ASSERT(0);
}

static void cmd_handler(unsigned short cmd)
{
	switch (INT_CMD_MASK(cmd)) {

	case INT_CMD_PHONE_START:
		rx_cmd_phone_start();
		break;
	case INT_CMD_CRASH_EXIT:
		rx_cmd_cp_crash();
		break;
	default:
		//mifdbg("SHMEM: unknown command 0x%04X\n", cmd);
		break;
	}
}
void ShmemIpcDevice::init(void)
{
	putreg32(0, txq.head);
	putreg32(0, txq.tail);
	putreg32(0, rxq.head);
	putreg32(0, rxq.tail);

	res_required = 0;
}


void ShmemIpcDevice::done_req_ack(void)
{
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	if (req_ack_rcvd < 0)
		req_ack_rcvd = 0;

	if (req_ack_rcvd == 0)
		return;

	pShmemLinkDevice->send_int2cp(INT_NON_CMD(mask_res_ack));
	req_ack_rcvd--;

}

void ShmemIpcDevice::recv_res_ack(unsigned short intr)
{
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	if (intr & mask_res_ack)
		req_ack_cmpl->release();
}

void ShmemIpcDevice::recv_req_ack(unsigned short intr)
{
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d\n", __func__, __LINE__);

	if (intr & mask_req_ack)
		req_ack_rcvd++;
}

int ShmemIpcDevice::get_rxq_rcvd(unsigned int head, unsigned int tail, shmem_circ_status *circ)
{
	circ->buff = rxq.buff;
	circ->qsize = rxq.size;
	circ->in = head;
	circ->out = tail;
	circ->size = circ_get_usage(circ->qsize, circ->in, circ->out);

	if (circ_valid(circ->qsize, circ->in, circ->out)) {
//		mifwdbg("SHMEM: %s_RXQ qsize[%u] in[%u] out[%u] rcvd[%u]\n",
//			get_dev_name(dev), circ->qsize, circ->in,
//			circ->out, circ->size);
		return 0;
	} else {
//		mifdbg("SHMEM: ERR! %s_RXQ invalid (qsize[%d] in[%d] out[%d])\n",
//			get_dev_name(dev), circ->qsize, circ->in,
//			circ->out);
		return -5;
	}
}

ShmemIpcFmtDevice::ShmemIpcFmtDevice()
{
	struct shmem_64KB_phys_map *map;

	map = (struct shmem_64KB_phys_map *)IPCRAM_PADDR;

	strncpy(name, "FMT", 15);

	txq.head = (unsigned int *)(&(map->fmt_tx_head));
	txq.tail = (unsigned int *)(&(map->fmt_tx_tail));
	txq.buff = (unsigned char *)(map->fmt_tx_buff);
	txq.size = SHM_64K_FMT_TX_BUFF_SZ;

	rxq.head = (unsigned int *)(&(map->fmt_rx_head));
	rxq.tail = (unsigned int *)(&(map->fmt_rx_tail));
	rxq.buff = (unsigned char *)(map->fmt_rx_buff);
	rxq.size = SHM_64K_FMT_RX_BUFF_SZ;

	mask_req_ack = INT_MASK_REQ_ACK_F;
	mask_res_ack = INT_MASK_RES_ACK_F;
	mask_send = INT_MASK_SEND_F;

	res_required = 0;

	tx_lock = new Mutex("IPC_FMT");
	req_ack_cmpl = new Semaphore(1, 1);
}

ShmemIpcFmtDevice *ShmemIpcFmtDevice::getInstance(void)
{
	static ShmemIpcFmtDevice *s_ipcfmtdevice;

	if (s_ipcfmtdevice == NULL) {
		s_ipcfmtdevice = new ShmemIpcFmtDevice();
	}

	return s_ipcfmtdevice;
}

int ShmemIpcFmtDevice::rx_ipc_frames(shmem_circ_status *circ)
{
	ShmemLinkDevice *ld = pShmemLinkDevice;

	/**
	 * variables for the status of the circular queue
	 */
	unsigned char *src;
	unsigned char hdr[EXYNOS_HEADER_SIZE];
	/**
	 * variables for RX processing
	 */
	int qsize;	/* size of the queue			*/
	int rcvd;	/* size of data in the RXQ or error	*/
	int rest;	/* size of the rest data		*/
	int out;	/* index to the start of current frame	*/
	int tot;	/* total length including padding data	*/

	src = circ->buff;
	qsize = circ->qsize;
	out = circ->out;
	rcvd = circ->size;

	rest = circ->size;
	tot = 0;

	while (rest > 0) {
		mio_buf *msg;

		/* Copy the header in the frame to the header buffer */
		circ_read(hdr, src, qsize, out, EXYNOS_HEADER_SIZE);

		/* Check the config field in the header */
		if (!exynos_start_valid(hdr)) {
//			mifdbg("SHMEM: ERR! %s INVALID config 0x%02X (rcvd %d, rest %d)\n",
//				get_dev_name(dev), hdr[0],
//				rcvd, rest);
			goto bad_msg;
		}

		/* Verify the total length of the frame (data + padding) */
		tot = exynos_get_total_len(hdr);
		if (tot > rest) {
//			mifdbg("SHMEM: ERR! %s tot %d > rest %d (rcvd %d)\n",
//				get_dev_name(dev), tot, rest, rcvd);
			goto bad_msg;
		}
		msg = alloc_mio_buf(tot - EXYNOS_HEADER_SIZE);
		circ_read((unsigned char *)(msg->data), src, qsize, out + EXYNOS_HEADER_SIZE, tot - EXYNOS_HEADER_SIZE);
		msg->ch = exynos_get_ch(hdr);
		msg->len = tot - EXYNOS_HEADER_SIZE;
		ld->ipc_rxq.put(msg);
		ld->ipc_rx_sem->release();

		/* Calculate new out value */
		rest -= tot;
		out += tot;
		if (out >= qsize)
			out -= qsize;
	}

	/* Update tail (out) pointer to empty out the RXQ */
	putreg32(out, rxq.tail);

	return rcvd;

bad_msg:
#ifdef CONFIG_DEBUG_MODEM_IF
	//mifdbg("SHMEM: ERR! rcvd:%d tot:%d rest:%d\n", rcvd, tot, rest);
#endif
	return -74;
}

ShmemIpcRawDevice::ShmemIpcRawDevice()
{
	struct shmem_64KB_phys_map *map;

	map = (struct shmem_64KB_phys_map *)IPCRAM_PADDR;

	memmove(name, "RAW", strlen("RAW"));

	txq.head = (unsigned int *)(&(map->raw_tx_head));
	txq.tail = (unsigned int *)(&(map->raw_tx_tail));
	txq.buff = (unsigned char *)(map->raw_tx_buff);
	txq.size = SHM_64K_RAW_TX_BUFF_SZ;

	rxq.head = (unsigned int *)(&(map->raw_rx_head));
	rxq.tail = (unsigned int *)(&(map->raw_rx_tail));
	rxq.buff = (unsigned char *)(map->raw_rx_buff);
	rxq.size = SHM_64K_RAW_RX_BUFF_SZ;

	mask_req_ack = INT_MASK_REQ_ACK_R;
	mask_res_ack = INT_MASK_RES_ACK_R;
	mask_send = INT_MASK_SEND_R;

	res_required = 0;

	req_ack_cmpl = new Semaphore(1, 1);
	tx_lock = new Mutex("IPC_RAW");
}

ShmemIpcRawDevice *ShmemIpcRawDevice::getInstance(void)
{
	static ShmemIpcRawDevice *s_ipclawdevice;

	if (s_ipclawdevice == NULL) {
		s_ipclawdevice = new ShmemIpcRawDevice();
	}

	return s_ipclawdevice;
}

int ShmemIpcRawDevice::rx_ipc_frames(shmem_circ_status *circ)
{
	ShmemLinkDevice *ld = pShmemLinkDevice;

	/**
	 * variables for the status of the circular queue
	 */
	unsigned char *src;
	unsigned char hdr[EXYNOS_HEADER_SIZE];
	/**
	 * variables for RX processing
	 */
	int qsize;	/* size of the queue			*/
	int rcvd;	/* size of data in the RXQ or error	*/
	int rest;	/* size of the rest data		*/
	int out;	/* index to the start of current frame	*/
	int tot;	/* total length including padding data	*/

	src = circ->buff;
	qsize = circ->qsize;
	out = circ->out;
	rcvd = circ->size;

	rest = circ->size;
	tot = 0;

	while (rest > 0) {
		mio_buf *msg;

		/* Copy the header in the frame to the header buffer */
		circ_read(hdr, src, qsize, out, EXYNOS_HEADER_SIZE);

		/* Check the config field in the header */
		if (!exynos_start_valid(hdr)) {
			tr_info("SHMEM: ERR! IPC_RAW INVALID config 0x%02X (rcvd %d, rest %d)\n",
				hdr[0], rcvd, rest);
			goto bad_msg;
		}

		/* Verify the total length of the frame (data + padding) */
		tot = exynos_get_total_len(hdr);
		if (tot > rest) {
			tr_info("SHMEM: ERR! IPC_RAW tot %d > rest %d (rcvd %d)\n",
				tot, rest, rcvd);
			goto bad_msg;
		}
		msg = alloc_mio_buf(tot - EXYNOS_HEADER_SIZE);
		circ_read((unsigned char *)(msg->data), src, qsize, out + EXYNOS_HEADER_SIZE, tot - EXYNOS_HEADER_SIZE);
		msg->ch = exynos_get_ch(hdr);
		msg->len = tot - EXYNOS_HEADER_SIZE;
		ld->ipc_rxq.put(msg);
		ld->ipc_rx_sem->release();

		/* Calculate new out value */
		rest -= tot;
		out += tot;
		if (out >= qsize)
			out -= qsize;
	}

	/* Update tail (out) pointer to empty out the RXQ */
	putreg32(out, rxq.tail);

	return rcvd;

bad_msg:
#ifdef CONFIG_DEBUG_MODEM_IF
	//mifdbg("SHMEM: ERR! rcvd:%d tot:%d rest:%d\n", rcvd, tot, rest);
#endif
	return -74;
}

void ipc_rx_task(void);
static void shmem_irq_handler(void *data)
{
	modem_link_device_wqueue->call(ipc_rx_task);
}

void ipc_shmem_save_task(void)
{
	ShmemLinkDevice *ld = pShmemLinkDevice;
	mio_buf *msg;

	msg = alloc_mio_buf(12);
	if (msg == NULL)
		return;

	ld->init_shmem_save();

	msg->ch = 243;
	strcpy((char *)(msg->data), "shmem_start");

	ld->ipc_rxq.put(msg);
	ld->ipc_rx_sem->release();
}

static void shmem_save_irq_handler(void *data)
{
	modem_link_device_wqueue->call(ipc_shmem_save_task);
}

static void shmem_stop_req_irq_handler(void *data)
{
	/* DO SOMETHING here */
}

void msg_handler(unsigned int head[], unsigned int tail[])
{
	ShmemLinkDevice *ld = pShmemLinkDevice;
	ShmemIpcDevice *ipc;
	int i;
	int ret;
	unsigned int magic;
	shmem_circ_status circ;

	//MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d  head[] = {%d, %d}, tail[] = {%d, %d}\n", __func__, __LINE__, head[0], head[1], tail[0], tail[1]);

	/* Is IPC active ? */
	if (ld->get_mbx_access() != 1)
		system_halt();
	magic = ld->get_mbx_magic();
	if (magic != SHM_IPC_MAGIC && magic != SHM_SAVE_MAGIC)
		system_halt();


	/* Skip RX processing if there is no data in the RXQ */
	/* Read data in the RXQ */
	/* Process REQ_ACK (At this point, the RXQ may be empty.) */
	for (i = 0; i < MAX_IPC_DEV; i++) {
		ipc = ld->ipc_device[i];
		if (head[i] == tail[i]) {
			ipc->done_req_ack();
			continue;
		}

		ret = ipc->get_rxq_rcvd(head[i], tail[i], &circ);
		if (ret < 0)
			return;

		ret = ipc->rx_ipc_frames(&circ);
		if (ret  < 0)
			return;

		if (ret < (int)(circ.size))
			break;

		ipc->done_req_ack();
	}

}

void ipc_rx_task(void)
{
	ShmemLinkDevice *ld = pShmemLinkDevice;
	unsigned int head[MAX_IPC_DEV];
	unsigned int tail[MAX_IPC_DEV];
	unsigned short int2ap;
	int i;

	/* get_shmem_status */
	int2ap = ld->get_mbx_cp2ap_msg();
	for (i = 0; i < MAX_IPC_DEV; i++) {
		head[i] = ld->ipc_device[i]->GetRxqHead();
		tail[i] = ld->ipc_device[i]->GetRxqTail();
	}
	//MODEM_LINK_DEVICE_SHMEM_DBG("%s int2ap[0x%08X]\n", __func__, int2ap);

	/* Process a SHMEM command */
	if (INT_CMD_VALID(int2ap)) {
		cmd_handler(int2ap);
		return;
	}

	/* Update tail variables with the current tail pointers */
	for (i = 0; i < MAX_IPC_DEV; i++)
		tail[i] = ld->ipc_device[i]->GetRxqTail();

	/* Check and receive RES_ACK from CP */
	/* Check and receive REQ_ACK from CP */
	for (i = 0; i < MAX_IPC_DEV; i++) {
		switch (int2ap) {
		case INT_MASK_RES_ACK_F:
		case INT_MASK_RES_ACK_R:
		case INT_MASK_RES_ACK_RFS:
			ld->ipc_device[i]->recv_res_ack(int2ap);
			break;
		case INT_MASK_REQ_ACK_F:
		case INT_MASK_REQ_ACK_R:
		case INT_MASK_REQ_ACK_RFS:
			ld->ipc_device[i]->recv_req_ack(int2ap);
			break;
		default:
			break;
		}
	}

	msg_handler(head, tail);
}

void msg_rx_work(void)
{
	ShmemLinkDevice *ld;
	ModemIoDevice *iodev;
	mio_buf *msg;
	osEvent evt;
#if MODEM_LINK_DEVICE_SHMEM_DBG_ON
	int i;
	char mm[96] = "";
#endif

	while (1) {
		ld = pShmemLinkDevice;
		ld->ipc_rx_sem->wait();

		while (!(ld->ipc_rxq.empty())) {
			MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d", __func__, __LINE__);
			evt = ld->ipc_rxq.get(osWaitForever);

			if (evt.status != (osStatus)osEventMessage)
				continue;

			msg = (mio_buf *)(evt.value.p);
#if MODEM_LINK_DEVICE_SHMEM_DBG_ON
			MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d ch[%d] len[%d]", __func__, __LINE__, msg->ch, msg->len);
			for (i = 0; i < msg->len; i++) {
				sprintf(mm + (i % 16) * 3, "%02X ", msg->data[i]);
				//MODEM_LINK_DEVICE_SHMEM_DBG("%02X ", msg->data[i]);
				if (i % 16 == 15 && i != 0)
					MODEM_LINK_DEVICE_SHMEM_DBG("%s", mm);
			}
			if ((i - 1) % 16 != 15)
					MODEM_LINK_DEVICE_SHMEM_DBG("%s", mm);
			MODEM_LINK_DEVICE_SHMEM_DBG("\n");
#endif
			MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d ch:%d", __func__, __LINE__, msg->ch);
			iodev = getModemIoDeviceById(msg->ch);
			if (iodev != NULL) {
				MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d [%s]", __func__, __LINE__, iodev->name);
				iodev->IoReadCallback(msg);
			} else {
				free_mio_buf(msg);
			}
			MODEM_LINK_DEVICE_SHMEM_DBG("%s:%d", __func__, __LINE__);
		}
	}
}

int modem_link_device_thread_start(void)
{
	MODEM_LINK_DEVICE_SHMEM_DBG("%s\n", __func__);
	msgrxwork = new rtos::Thread(osPriorityNormal, 2048, NULL, "msgrxwork");
	msgrxwork->start(msg_rx_work);

	modem_link_device_wqueue = new EventQueue();
	irqwork = new rtos::Thread(osPriorityNormal, 2048, NULL, "ipc_rx_task");
	irqwork->start(callback(modem_link_device_wqueue, &events::EventQueue::dispatch_forever));

	return 0;
}


