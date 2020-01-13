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

#ifndef __EXYNOS_IPC_H__
#define __EXYNOS_IPC_H__

#define EXYNOS_SINGLE_MASK          (0b11000000)
#define EXYNOS_MULTI_START_MASK (0b10000000)
#define EXYNOS_MULTI_LAST_MASK      (0b01000000)

#define EXYNOS_START_MASK           0xABCD
#define EXYNOS_START_OFFSET     0
#define EXYNOS_START_SIZE           2

#define EXYNOS_FRAME_SEQ_OFFSET 2
#define EXYNOS_FRAME_SIZE           2

#define EXYNOS_FRAG_CONFIG_OFFSET   4
#define EXYNOS_FRAG_CONFIG_SIZE 2

#define EXYNOS_LEN_OFFSET           6
#define EXYNOS_LEN_SIZE         2

#define EXYNOS_CH_ID_OFFSET     8
#define EXYNOS_CH_SIZE              1

#define EXYNOS_CH_SEQ_OFFSET        9
#define EXYNOS_CH_SEQ_SIZE          1

#define EXYNOS_HEADER_SIZE      12

#define EXYNOS_DATA_LOOPBACK_CHANNEL    82

#define EXYNOS_FMT_NUM      1
#define EXYNOS_RFS_NUM      10

enum exynos_ch_id {
    EXYNOS_CH_ID_MULTIPDP = 0,

    EXYNOS_CH_ID_PDP_0 = 1,    /*rmnet0*/
    EXYNOS_CH_ID_PDP_1,
    EXYNOS_CH_ID_PDP_2,
    EXYNOS_CH_ID_PDP_3,
    EXYNOS_CH_ID_PDP_4,

    EXYNOS_CH_ID_FMT_0 = 245,   /*umts_ipc0*/
    EXYNOS_CH_ID_MAX = 255,
    EXYNOS_CH_ID_FLOW_CTRL = 255
};

struct __attribute__((packed)) frag_config {
    unsigned char frame_first: 1,
             frame_last: 1,
             packet_index: 6;
    unsigned char frame_index;
};

/* EXYNOS link-layer header */
struct  __attribute__((packed)) exynos_link_header {
    unsigned short seq;
    struct frag_config cfg;
    unsigned short len;
    unsigned short reserved_1;
    unsigned char ch_id;
    unsigned char ch_seq;
    unsigned short reserved_2;
};

struct exynos_frame_data {
    /* Frame length calculated from the length fields */
    unsigned int len;

    /* The length of link layer header */
    unsigned int hdr_len;

    /* The length of received header */
    unsigned int hdr_rcvd;

    /* The length of link layer payload */
    unsigned int pay_len;

    /* The length of received data */
    unsigned int pay_rcvd;

    /* The length of link layer padding */
    unsigned int pad_len;

    /* The length of received padding */
    unsigned int pad_rcvd;

    /* Header buffer */
    unsigned char hdr[EXYNOS_HEADER_SIZE];
};

static inline bool exynos_start_valid(unsigned char *frm)
{
    unsigned short cfg = *(unsigned short *)(frm + EXYNOS_START_OFFSET);
    return cfg == EXYNOS_START_MASK ? true : false;
}

static inline bool exynos_multi_start_valid(unsigned char *frm)
{
    unsigned short cfg = *(unsigned short *)(frm + EXYNOS_FRAG_CONFIG_OFFSET);
    return ((cfg >> 8) & EXYNOS_MULTI_START_MASK) == EXYNOS_MULTI_START_MASK;
}

static inline bool exynos_multi_last_valid(unsigned char *frm)
{
    unsigned short cfg = *(unsigned short *)(frm + EXYNOS_FRAG_CONFIG_OFFSET);
    return ((cfg >> 8) & EXYNOS_MULTI_LAST_MASK) == EXYNOS_MULTI_LAST_MASK;
}

static inline bool exynos_single_frame(unsigned char *frm)
{
    unsigned short cfg = *(unsigned short *)(frm + EXYNOS_FRAG_CONFIG_OFFSET);
    return ((cfg >> 8) & EXYNOS_SINGLE_MASK) == EXYNOS_SINGLE_MASK;
}

static inline unsigned char exynos_get_ch(unsigned char *frm)
{
    return frm[EXYNOS_CH_ID_OFFSET];
}

static inline unsigned int exynos_get_frame_seq(unsigned char *frm)
{
    unsigned short cfg = *(unsigned short *)(frm + EXYNOS_FRAME_SEQ_OFFSET);
    return cfg;
}

static inline unsigned int exynos_get_ch_seq(unsigned char *frm)
{
    return frm[EXYNOS_CH_SEQ_OFFSET];
}

static inline unsigned int exynos_calc_padding_size(unsigned int len)
{
    unsigned int residue = len & 0x3;
    return residue ? (4 - residue) : 0;
}

static inline unsigned int exynos_get_frame_len(unsigned char *frm)
{
    return (unsigned int) * (unsigned short *)(frm + EXYNOS_LEN_OFFSET);
}

static inline bool exynos_fmt_ch(unsigned char ch)
{
    return (ch == EXYNOS_CH_ID_FMT_0) ? true : false;
}

static inline bool exynos_ps_ch(unsigned char ch)
{
    return (ch >= EXYNOS_CH_ID_PDP_0 && ch <= EXYNOS_CH_ID_PDP_4) ?
           true : false;
}

static inline unsigned int exynos_get_total_len(unsigned char *frm)
{
    unsigned int len;
    unsigned int pad;

    len = exynos_get_frame_len(frm);
    pad = exynos_calc_padding_size(len) ? exynos_calc_padding_size(len) : 0;
    return len + pad;
}

static inline bool exynos_padding_exist(unsigned char *frm)
{
    return exynos_calc_padding_size(exynos_get_frame_len(frm)) ? true : false;
}

typedef struct {
    int ch;
    int len;
    unsigned char *data;
    unsigned char *header;
    short pad;
    short dalloc;
} mio_buf;

inline mio_buf *alloc_mio_buf(int len)
{
    mio_buf *mio;

    mio = (mio_buf *)malloc(sizeof(mio_buf));
    if (mio == NULL) {
        return NULL;
    }

    if (len != 0) {
        mio->data = (unsigned char *)malloc(len);
        if (mio->data == NULL) {
            free(mio);
            return NULL;
        }
        mio->dalloc = 1;
    } else {
        mio->dalloc = 0;
    }

    return mio;
}

inline void free_mio_buf(mio_buf *mio)
{
    if (mio->dalloc != 0) {
        free(mio->data);
    }
    free(mio);
}

#endif
