/**
  ******************************************************************************
  * @file    mx_wifi_slip.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi_slip.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_WIFI_SLIP_H
#define MX_WIFI_SLIP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_wifi_conf.h"

/*
 * CONFIGURATIONS
 */
#define MX_WIFI_SLIP_DEBUG


/* SLIP_PACKET
 * |--------+---------+--------|
 * | START  | PAYLOAD | END    |
 * |--------+---------+--------|
 * | 1Bytes | nBytes  | 1Bytes |
 * |--------+---------+--------|
 */

/*
 * API
 */

/* transfer HCI data to SLIP data,
 * return the SLIP buffer */
uint8_t *slip_transfer(uint8_t *data, uint16_t len, uint16_t *outlen);

/* PHY receive one serial byte to SLIP,
 * return SLIP frame
 * NOTE: use slip_buf_free to free slip buffer if data process finished. */
mx_buf_t *slip_input_byte(uint8_t data);

/* free slip frame buffer returned by slip_input_byte */
void slip_buf_free(uint8_t *buf);

enum
{
  SLIP_START = 0xC0,
  SLIP_END = 0xD0,
  SLIP_ESCAPE = 0xDB,
  SLIP_ESCAPE_START = 0xDC,
  SLIP_ESCAPE_ES = 0xDD,
  SLIP_ESCAPE_END = 0xDE
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_SLIP_H */
