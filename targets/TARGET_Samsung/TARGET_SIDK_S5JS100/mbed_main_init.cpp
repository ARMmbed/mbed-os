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

#include "mbed.h"
#include "s5js100_pwr.h"
#include "modem_link_device_shmem.h"
#include "modem_io_device.h"

struct modem_io_device {
	char name[12];
	int id;
	enum dev_format format;
	enum modem_io type;
} modem_io_list[] = {
	{"ModemProxy",	245,	IPC_FMT,	IODEV_MISC},
	{"rmnet1",	1,	IPC_RAW,	IODEV_NET},
	{"shmem_save",	243,	IPC_RAW,	IODEV_MISC},
	{"dcxo",	244,	IPC_RAW,	IODEV_MISC},
	//{"if5",	242,	IPC_RAW,	IODEV_MISC},
	{"rmnet2",	2,	IPC_RAW,	IODEV_NET},
	{"rmnet3",	3,	IPC_RAW,	IODEV_NET},
	{"rmnet4",	4,	IPC_RAW,	IODEV_NET},
	{"nonip0",	6,	IPC_RAW,	IODEV_MISC},
	{"nonip1",	7,	IPC_RAW,	IODEV_MISC},
	{"nonip2",	8,	IPC_RAW,	IODEV_MISC},
	{"nonip3",	9,	IPC_RAW,	IODEV_MISC},
};


extern "C" {
extern void dcxo_thread_start(void);
extern void dcxo_thread_stop(void);
extern void shmem_save_start(void);
extern void shmem_save_stop(void);

void s5js100_modem_start(void)
{
	unsigned int i;
	char *name;
	int id;

	pShmemLinkDevice = new ShmemLinkDevice();
	ShmemIpcFmtDevice *ipc_fmt = ShmemIpcFmtDevice::getInstance();
	ShmemIpcRawDevice *ipc_raw = ShmemIpcRawDevice::getInstance();
	ShmemIpcDevice *ipc;

	for (i = 0; i < sizeof(modem_io_list) / sizeof(modem_io_list[0]); i++) {
		name = modem_io_list[i].name;
		id = modem_io_list[i].id;
		switch (modem_io_list[i].format) {
		case IPC_FMT:
			ipc = ipc_fmt;
			break;
		case IPC_RAW:
			ipc = ipc_raw;
			break;
		default:
			break;
		}

		switch (modem_io_list[i].type) {
		case IODEV_MISC:
			registerModemIoDevice(new IoDevMisc(name, id, ipc));
			break;
		case IODEV_NET:
			registerModemIoDevice(new IoDevNet(name, id, ipc));
			break;
		default:
			break;
		}
	}
}

void s5js100_modem_stop(void)
{
	delete pShmemLinkDevice;
}

}

