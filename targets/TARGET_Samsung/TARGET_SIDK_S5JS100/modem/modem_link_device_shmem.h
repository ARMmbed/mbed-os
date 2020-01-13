/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef __MODEM_LINK_DEVICE_SHMEM_H__
#define __MODEM_LINK_DEVICE_SHMEM_H__

#include "mbed.h"
#include "exynos_ipc.h"
#include "s5js100_mbox_ipc.h"

typedef enum {
    STATE_OFFLINE = 0,
    STATE_ONLINE,
    STATE_NV_CS_SAVE,
} modem_state;

enum dev_format {
    IPC_FMT = 0,
    IPC_RAW,
    MAX_IPC_DEV,
};

enum modem_io {
    IODEV_MISC = 0,
    IODEV_NET,
    IODEV_DUMMY,
};

typedef struct {
    unsigned int *head;
    unsigned int *tail;
    unsigned char *buff;
    unsigned int size;
} shmem_circ;

typedef struct {
    unsigned char *buff;
    unsigned int qsize; /* the size of a circular buffer */
    unsigned int in;
    unsigned int out;
    unsigned int size;  /* the size of free space or received data */
} shmem_circ_status;


class ShmemIpcDevice {
public:
    char name[16];

    shmem_circ txq;
    shmem_circ rxq;

    unsigned short mask_req_ack;
    unsigned short mask_res_ack;
    unsigned short mask_send;

    int req_ack_rcvd;
    int res_required;
    Mutex *tx_lock;
    Semaphore *req_ack_cmpl;
    unsigned short header_cnt;
public:

    void done_req_ack(void);
    void recv_req_ack(unsigned short intr);
    void recv_res_ack(unsigned short intr);
    void init(void);

    int init_comm(void)
    {
        return 0;
    }
    int release_shmem_mb(void)
    {
        return 0;
    }
    int resume_shmem_mb(void)
    {
        return 0;
    }
    void terminate_comm(void) { }

    int get_txq_space(void);
    void write_ipc_to_txq(mio_buf *mxb);
    int xmit_ipc_msg(mio_buf *mxb);
    int send(mio_buf *mxb);
    void send_cmd(unsigned short cmd);
    void save_release();
    int ioctl(unsigned int cmd, unsigned long arg);
    int check_security(unsigned long arg);
    int sec_init(unsigned long arg);

    int rx_ipc_frames(void);
};

class ShmemIpcFmtDevice : public ShmemIpcDevice {
public:
    ShmemIpcFmtDevice();
    ~ShmemIpcFmtDevice();
//    void TxWork(unsigned long arg) { }
    static ShmemIpcFmtDevice *getInstance(void);
//    int rx_ipc_frames(shmem_circ_status *circ);
};

class ShmemIpcRawDevice : public ShmemIpcDevice {
public:
    ShmemIpcRawDevice();
    ~ShmemIpcRawDevice();
//    void TxWork(unsigned long arg) { }
    static ShmemIpcRawDevice *getInstance(void);
//    int rx_ipc_frames(shmem_circ_status *circ);
};


class ShmemLinkDevice : public ExynosMboxIpc {
private:
    modem_state state;

public:
    ShmemIpcDevice *ipc_device[MAX_IPC_DEV];
    int shmem_size;
    void *shmem_base;

    /* Pointers (aliases) to IPC device map */
    unsigned int *shmem_magic;
    unsigned int *shmem_access;

    Semaphore *save_sem;
    Semaphore *stop_sem;

    Semaphore *ipc_rx_sem;
    Queue<mio_buf, 32> ipc_rxq;

public:
    ShmemLinkDevice();
    ~ShmemLinkDevice();
    int ShmemLinkDevice_start(void);
    unsigned short get_mbx_cp2ap_msg(void);
    unsigned short get_mbx_ap2cp_msg(void);
    unsigned int get_mbx_magic(void);
    unsigned int get_mbx_access(void);
    void send_int2cp(unsigned short mask);
    void save_sem_release(void);
    int init(void);
    modem_state getstate(void)
    {
        return state;
    }
    void setstate(modem_state st)
    {
        state = st;
    }

    int IsCpOffline(void)
    {
        return (state == STATE_OFFLINE);
    }

    int IsCpOnline(void)
    {
        return (state == STATE_ONLINE);
    }

    int IsCpNvCsSave(void)
    {
        return (state == STATE_NV_CS_SAVE);
    }
    int init_shmem_save(void);
};

extern ShmemLinkDevice *pShmemLinkDevice;

/* shmem message */
#define INT_MASK_REQ_ACK_F  0x0020
#define INT_MASK_REQ_ACK_R  0x0010
#define INT_MASK_RES_ACK_F  0x0008
#define INT_MASK_RES_ACK_R  0x0004
#define INT_MASK_SEND_F     0x0002
#define INT_MASK_SEND_R     0x0001

#define INT_MASK_REQ_ACK_RFS    0x0400 /* Request RES_ACK_RFS       */
#define INT_MASK_RES_ACK_RFS    0x0200 /* Response of REQ_ACK_RFS   */
#define INT_MASK_SEND_RFS   0x0100 /* Indicate sending RFS data */

#define INT_MASK_REQ_ACK_SET \
    (INT_MASK_REQ_ACK_F | INT_MASK_REQ_ACK_R | INT_MASK_REQ_ACK_RFS)

#define INT_MASK_RES_ACK_SET \
    (INT_MASK_RES_ACK_F | INT_MASK_RES_ACK_R | INT_MASK_RES_ACK_RFS)



/* shmem physical memory */
#define SHM_IPC_MAGIC       0x000000AA
#define SHM_SAVE_MAGIC      0x76617300

#define SHM_64K_RESERVED_SZ (8 + 16)
#define SHM_64K_FMT_TX_BUFF_SZ  0x3FE0
#define SHM_64K_FMT_RX_BUFF_SZ  0x3FE0
#define SHM_64K_RAW_TX_BUFF_SZ  0x4000
#define SHM_64K_RAW_RX_BUFF_SZ  0x4000

struct shmem_64KB_phys_map {
    unsigned int magic;
    unsigned int access;

    unsigned int fmt_tx_head;
    unsigned int fmt_tx_tail;

    unsigned int fmt_rx_head;
    unsigned int fmt_rx_tail;

    unsigned int raw_tx_head;
    unsigned int raw_tx_tail;

    unsigned int raw_rx_head;
    unsigned int raw_rx_tail;

    unsigned char reserved[SHM_64K_RESERVED_SZ];

    unsigned char fmt_tx_buff[SHM_64K_FMT_TX_BUFF_SZ];
    unsigned char fmt_rx_buff[SHM_64K_FMT_RX_BUFF_SZ];

    unsigned char raw_tx_buff[SHM_64K_RAW_TX_BUFF_SZ];
    unsigned char raw_rx_buff[SHM_64K_RAW_RX_BUFF_SZ];
} __attribute__((packed));
#endif
