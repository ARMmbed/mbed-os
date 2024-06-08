/*
 * Copyright (c) 2022 Macronix International Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BCH_H
#define _BCH_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define DIV_ROUND_UP(n, d)  (((n) + (d) - 1) / (d))

struct bch_code {
    unsigned int    m;
    unsigned int    n;
    unsigned int    t;
    unsigned int    ecc_bits;
    unsigned int    ecc_words;
    unsigned int    len;
    unsigned int   *a_pow;
    unsigned int   *a_log;
    unsigned int   *mod_tab;
    unsigned int   *ecc;
    unsigned int   *syn;
    unsigned int   *elp;
    unsigned int   *buf;
    unsigned int   *buf2;
    unsigned char   *input_data;
    unsigned int   endian;
};

struct bch_code *bch_init(unsigned int m, unsigned int t);
void bch_free(struct bch_code *bch);
void bch_encode(struct bch_code *bch, unsigned char *data, unsigned int *ecc);
int bch_decode(struct bch_code *bch, unsigned char *data, unsigned int *ecc);
int fls(int x);

#ifdef __cplusplus
}
#endif
#endif
