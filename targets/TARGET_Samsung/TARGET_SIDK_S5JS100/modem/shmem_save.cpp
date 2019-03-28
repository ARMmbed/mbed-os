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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "mbed.h"
#include "shmem_save.h"
#include "modem_io_device.h"
#include "s5js100_pwr.h"
#include "sflash_api.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FLASH_CS_BASE			0x40EAA000
#define FLASH_NV_BASE			0x40E47000
#define FLASH_CAL_BASE			0x40E87000

#define FLASH_CS_SIZE			(1024 * 300)
#define FLASH_NV_SIZE			(1024 * 256)
#define FLASH_CAL_SIZE			(1024 * 128)

#define REQ_CS_MSG			0x0000A20B
#define REQ_NV_MSG			0x0000A21B
#define REQ_CAL_MSG			0x0000A22B

#define BUFFER_SIZE			256
#define SFLASH_SECTOR_SIZE		256

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
static ModemIoDevice *shmem_mio;
static unsigned int cur_save_target;
static unsigned char *save_cached;
/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
extern int nbsleep_req;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#define SHMEM_SECTION_MAGIC		(0x06313184)
typedef struct {
	unsigned int magic0;
	unsigned int version;
	unsigned int start;
	unsigned int magic1;
} shmem_section_data;

static shmem_section_data next_shmem_section[3] = {
	{SHMEM_SECTION_MAGIC, 1, FLASH_CS_BASE + (32 * 1024), SHMEM_SECTION_MAGIC},
	{SHMEM_SECTION_MAGIC, 1, FLASH_NV_BASE + (32 * 1024), SHMEM_SECTION_MAGIC},
	{SHMEM_SECTION_MAGIC, 1, FLASH_CAL_BASE + (32 * 1024), SHMEM_SECTION_MAGIC},
};

void shmem_get_data(unsigned int *cs, unsigned int *nv, unsigned int *cal)
{
	struct {
		unsigned int start;
		unsigned int size;
	} section[] = {
		{FLASH_CS_BASE, FLASH_CS_SIZE},
		{FLASH_NV_BASE, FLASH_NV_SIZE},
		{FLASH_CAL_BASE, FLASH_CAL_SIZE},
	};
	unsigned int i, a, max;
	shmem_section_data *d, *m;

	for (i = 0; i < 3; i++) {
		max = 0;
		for (a = section[i].start; a < section[i].start + section[i].size; a += 0x1000) {
			d = (shmem_section_data *)(a + 0x1000 - sizeof(shmem_section_data));
			if (d->magic0 != 0x06313184 || d->magic1 != 0x06313184)
				continue;
			if (d->version > max) {
				max = d->version;
				m = d;
			}
		}

		switch (i) {
		case 0:
			if (max != 0) {
				*cs = m->start;
				next_shmem_section[i].start = (unsigned int)m + sizeof(shmem_section_data);
				next_shmem_section[i].version = m->version + 1;
			} else {
				*cs = section[i].start;
			}
			break;
		case 1:
			if (max != 0) {
				*nv = m->start;
				next_shmem_section[i].start = (unsigned int)m + sizeof(shmem_section_data);
				next_shmem_section[i].version = m->version + 1;
			} else {
				*nv = section[i].start;
			}
			break;
		case 2:
			if (max != 0) {
				*cal = m->start;
				next_shmem_section[i].start = (unsigned int)m + sizeof(shmem_section_data);
				next_shmem_section[i].version = m->version + 1;
			} else {
				*cal = section[i].start;
			}
			break;
		}
	}
}

int shmem_data_save(unsigned int msg, uint8_t *buffer, uint16_t real_len, uint32_t t_size, uint32_t s_size)
{
	unsigned int flash_start_addr;
	unsigned int offset;
	unsigned int poffset;
	unsigned int psize;
	unsigned char *buf;
	unsigned int cached_size;
	unsigned int wlen;
	unsigned int wsize;
	unsigned int section_idx;

	switch (msg) {
	case REQ_CS_MSG:
		section_idx = 0;
		if (next_shmem_section[0].start + t_size + sizeof(shmem_section_data) < FLASH_CS_BASE + FLASH_CS_SIZE)
			flash_start_addr = next_shmem_section[0].start;
		else
			flash_start_addr = FLASH_CS_BASE;
		next_shmem_section[0].start = flash_start_addr;
		break;
	case REQ_NV_MSG:
		section_idx = 1;
		if (next_shmem_section[1].start + t_size + sizeof(shmem_section_data) < FLASH_NV_BASE + FLASH_NV_SIZE)
			flash_start_addr = next_shmem_section[1].start;
		else
			flash_start_addr = FLASH_NV_BASE;
		next_shmem_section[1].start = flash_start_addr;
		break;
	case REQ_CAL_MSG:
		section_idx = 2;
		if (next_shmem_section[2].start + t_size + sizeof(shmem_section_data) < FLASH_CAL_BASE + FLASH_CAL_SIZE)
			flash_start_addr = next_shmem_section[2].start;
		else
			flash_start_addr = FLASH_CAL_BASE;
		next_shmem_section[2].start = flash_start_addr;
		break;
	default:
		return -1;
	}

	psize = up_progmem_blocksize();
	offset = s_size;
	cached_size = s_size & (psize - 1);
	wlen = 0;

	while (1) {
		wsize = (real_len - wlen  + cached_size > psize) ? psize - cached_size : real_len - wlen;
		//printf("shmem_data_save:%d wsize : 0x%x cached_size : 0x%x real_len : 0x%x wlen : 0x%x psize : 0x%x buffer : 0x%x\n", __LINE__, wsize, cached_size, real_len, wlen, psize, buffer);

		if (wsize != psize) {
			memcpy(save_cached + cached_size, buffer + wlen, wsize);
			buf = save_cached;
		} else {
			buf = buffer + wlen;
		}
		cached_size += wsize;
		wlen += wsize;
		poffset = (offset / psize) * psize;
		offset += wsize;

		//printf("shmem_data_save:%d wsize : 0x%x cached_size : 0x%x real_len : 0x%x wlen : 0x%x psize : 0x%x buf : 0x%x\n", __LINE__, wsize, cached_size, real_len, wlen, psize, buf);

		if (cached_size != psize)
			break;

		//printf("shmem_data_save addr(0x%08X) size(%X) buf(%p)\n", flash_start_addr + poffset, psize, buf);
		cached_size = 0;
		sflash_erase(flash_start_addr + poffset);
		sflash_write(flash_start_addr + poffset, buf, psize);
	}

	if (real_len + s_size >= t_size) {
		poffset = (offset / psize) * psize;
		//printf("shmem_data_save addr(0x%08X) size(%X) buf(%p)\n", flash_start_addr + poffset, cached_size, save_cached);
		if (cached_size + sizeof(shmem_section_data) <= psize) {
			memcpy(save_cached + 0xFF0, (unsigned char *)(&(next_shmem_section[section_idx])), 0x10);
			sflash_erase(flash_start_addr + poffset);
			sflash_write(flash_start_addr + poffset, save_cached, psize);
		} else {
			sflash_erase(flash_start_addr + poffset);
			sflash_write(flash_start_addr + poffset, save_cached, cached_size);
			//printf("shmem_data_save addr(0x%08X) size(%X) buf(%p)\n", flash_start_addr + poffset + psize, cached_size, save_cached);
			memcpy(save_cached + 0xFF0, (unsigned char *)(&(next_shmem_section[section_idx])), 0x10);
			sflash_erase(flash_start_addr + poffset + psize);
			sflash_write(flash_start_addr + poffset, save_cached, psize);
		}
	}

	return 0;
}
void shmem_factory_reset(void)
{
	unsigned int flash_start_addr;
	unsigned int poffset = 0;
	/* erase NV
	 * 0x40E47000 - 0x40E86FFF */
	flash_start_addr = FLASH_NV_BASE;
	for (poffset = 0; poffset < 64; poffset++)
		sflash_erase(flash_start_addr + poffset * 4096);

	/* erase CS
	 * 0x40EAA000 - 0x40EF4FFF */
	flash_start_addr = FLASH_CS_BASE;
	for (poffset = 0; poffset < 75; poffset++)
		sflash_erase(flash_start_addr + poffset * 4096);
}

void shmem_save_read_cb(mio_buf *buf, void *data);
void shmem_initiate_cb(mio_buf *buf, void *data)
{
	save_cached = (unsigned char *)malloc(4096);

	shmem_mio->register_ReadCb(shmem_save_read_cb, NULL);
	shmem_mio->write((char *)(&cur_save_target), 4);
	free_mio_buf(buf);
}

void shmem_save_read_cb(mio_buf *buf, void *data)
{
	uint16_t msg_len;
	uint32_t remain_len, real_len;
	uint8_t *buffer = (uint8_t *)(buf->data);
	uint32_t total_size, sent_size;
	unsigned long long nbsleep_sec;

	msg_len = *(uint16_t *)(buffer + 2);

	if (msg_len != 0) {
		real_len = msg_len - 8;
		total_size = *(uint32_t *)(buffer + 4);
		sent_size = *(uint32_t *)(buffer + 8);
		remain_len = *(uint32_t *)(buffer + 4) - *(uint32_t *)(buffer + 8);
		
		//printf("%s:%d cur_save_target(0x%08X) len(%x)\n", __func__, __LINE__, cur_save_target, buf->len);
		//printf("msg_len : 0x%x	t_size : 0x%x, s_size : 0x%x, real_len : 0x%x, remain_len : 0x%x\n", msg_len, total_size, sent_size, real_len, remain_len);

		shmem_data_save(cur_save_target, buffer + 12, real_len, total_size, sent_size);
	} else {
		//printf("%s:%d cur_save_target(0x%08X) len(%x)\n", __func__, __LINE__, cur_save_target, buf->len);
		remain_len  = 0;
		real_len = 0;
	}

	free_mio_buf(buf);

	if (remain_len == real_len) {
		if (cur_save_target == REQ_CAL_MSG) {
			free(save_cached);
			cur_save_target = REQ_CS_MSG;
			shmem_mio->register_ReadCb(shmem_initiate_cb, NULL);
			nbsleep_req = 1;
			nbsleep_sec = ((((unsigned long long)(getreg32(0x81000000 + 0xE4))) << 32) + getreg32(0x81000000 + 0xE8)) / 32768;
			printf("\nEnter NB sleep!! %lld seconds\n", nbsleep_sec);
		}

		cur_save_target += 0x10;
		if (cur_save_target > REQ_CAL_MSG) {
			cur_save_target = REQ_CS_MSG;
			shmem_mio->register_ReadCb(shmem_initiate_cb, NULL);
		}
	}

	//printf("%s:%d cur_save_target(0x%08X)\n", __func__, __LINE__, cur_save_target);
	shmem_mio->write((char *)(&cur_save_target), 4);
}

extern "C" {
void shmem_save_init(void)
{
	char mio_name[32] = "shmem_save";

	cur_save_target = REQ_CS_MSG;
	shmem_mio = getModemIoDeviceByName(mio_name);
	shmem_mio->register_ReadCb(shmem_initiate_cb, NULL);
}

void shmem_save_stop(void)
{

}
}


