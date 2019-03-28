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

#include <string.h>
#include "modem_io_device.h"
#include "modem_prj.h"

#include "mbed_trace.h"
#define TRACE_GROUP "IODEV"

#ifndef MODEM_IO_DEVICE_DBG_ON
#define MODEM_IO_DEVICE_DBG_ON		0
#endif
#define MODEM_IO_DEVICE_DBG		if (MODEM_IO_DEVICE_DBG_ON) tr_info

#ifndef MODEM_IO_DEVICE_MAX
#define MODEM_IO_DEVICE_MAX		(24)
#endif

ModemIoDevice *modem_io_deivce_list[MODEM_IO_DEVICE_MAX];
extern ShmemLinkDevice *pShmemLinkDevice;

void registerModemIoDevice(ModemIoDevice *io)
{
	int i;

	for (i = 0; i < MODEM_IO_DEVICE_MAX; i++) {
		if (!modem_io_deivce_list[i]) {
			modem_io_deivce_list[i] = io;
			break;
		}
	}
}

ModemIoDevice *getModemIoDeviceByName(char *name)
{
	int i;

	for (i = 0; i < MODEM_IO_DEVICE_MAX; i++) {
		if (modem_io_deivce_list[i] && !strncmp(modem_io_deivce_list[i]->name, name, 31)) {
			return modem_io_deivce_list[i];
		}
	}
	return NULL;
}

ModemIoDevice *getModemIoDeviceById(int ch)
{
	int i;

	for (i = 0; i < MODEM_IO_DEVICE_MAX; i++) {
		if (modem_io_deivce_list[i] != NULL) {
			if (modem_io_deivce_list[i]->id == ch)
				return modem_io_deivce_list[i];
		}
	}

	return NULL;
}

int ModemIoDevice::ioctl(int cmd, unsigned long arg)
{
	return 0;
}

unsigned short ModemIoDevice::exynos_build_fr_config(unsigned int count)
{
	unsigned short fr_cfg = 0;

	if ((count + EXYNOS_HEADER_SIZE) <= 0xFFFF)
		fr_cfg |= (EXYNOS_SINGLE_MASK << 8);

	return fr_cfg;
}

void ModemIoDevice::exynos_build_header(unsigned char *buff, unsigned short cfg, unsigned char ctl, int count)
{
	unsigned short *exynos_header = (unsigned short *)(buff + EXYNOS_START_OFFSET);
	unsigned short *frame_seq = (unsigned short *)(buff + EXYNOS_FRAME_SEQ_OFFSET);
	unsigned short *frag_cfg = (unsigned short *)(buff + EXYNOS_FRAG_CONFIG_OFFSET);
	unsigned short *size = (unsigned short *)(buff + EXYNOS_LEN_OFFSET);

	*exynos_header = EXYNOS_START_MASK;
	*frame_seq = ++(seq_num.frame_cnt);
	*frag_cfg = cfg;
	*size = (unsigned short)(EXYNOS_HEADER_SIZE + count);
	buff[EXYNOS_CH_ID_OFFSET] = id;

	if (cfg == EXYNOS_SINGLE_MASK)
		*frag_cfg = cfg;

	buff[EXYNOS_CH_SEQ_OFFSET] = ++seq_num.ch_cnt[id];
}

int ModemIoDevice::write(const char *buf, int count)
{
	mio_buf *mio;
	unsigned short fr_cfg;
	size_t tailroom;
	unsigned char header[EXYNOS_HEADER_SIZE];

	MODEM_IO_DEVICE_DBG("%s:%d count(%d)\n", __func__, __LINE__, count);

	fr_cfg = exynos_build_fr_config(count);
	tailroom = exynos_calc_padding_size(EXYNOS_HEADER_SIZE + count);

	mio = (mio_buf *)alloc_mio_buf(0);
	if (!mio)
		return 0;

	exynos_build_header(header, fr_cfg, 0, count);
	mio->header = header;
	mio->data = (unsigned char *)buf;
	mio->len = count;
	mio->ch = id;
	mio->pad = tailroom;

	ipc->send(mio);
	return count;

}

void IoDevMisc::IoReadCallback(mio_buf *buf)
{
	MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);
	if (read_cb != NULL) {
		MODEM_IO_DEVICE_DBG("%s:%d name: %s\n", __func__, __LINE__, name);
		read_cb(buf, cb_data);
		return;
	}
	MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);

	if (rxq.full()) {
		free_mio_buf(buf);
		return;
	}
	MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);

	rxq.put(buf);
}

int IoDevMisc::read(char *buf, int count)
{
	int copied = 0;
	int tmp = 0;
	osEvent evt;
	mio_buf *mio;
	int diff;

	if (remain.len) {
		tmp = remain.len > count ? count : remain.len;
		memcpy(buf, remain.data, tmp);
		remain.len -= tmp;
		if (remain.len == 0)
			free(remain.data);
		count -= tmp;
		if (!count)
			return tmp;
	}

	if (rxq.empty())
		return tmp;

	evt = rxq.get(osWaitForever);

	if (evt.status != (osStatus)osEventMessage)
		return tmp;

	mio = (mio_buf *)(evt.value.p);
	copied += mio->len > count ? count : mio->len;
	memcpy(buf, mio->data, copied);

	diff = mio->len - copied;
	if (diff > 0) {
		remain.data = (unsigned char *)malloc(diff);
		if (remain.data) {
			memcpy(remain.data, mio->data + copied, diff);
			remain.len = diff;
		}
	}

	free(mio->data);
	free(mio);

	return copied + tmp;
}

int IoDevMisc::poll(int setup)
{
	MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);
	r_sem->wait(osWaitForever);

	return 0;
}

void IoDevNet::IoReadCallback(mio_buf *buf)
{
	MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);
	if (read_cb != NULL) {
		MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);
		read_cb(buf, cb_data);
		return;
	}
	MODEM_IO_DEVICE_DBG("%s:%d\n", __func__, __LINE__);
	free_mio_buf(buf);
}

int IoDevNet::read(char *buf, int count)
{
	return 0;
}

