/**
  ******************************************************************************
  * @file    mx_wifi_slip.c
  * @author  MCD Application Team
  * @brief   Host driver SLIP protocol of MXCHIP Wi-Fi component.
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


/* Includes ------------------------------------------------------------------*/
/*cstat -MISRAC2012-* */
#include "stdio.h"
#include "stdlib.h"
/*cstat +MISRAC2012-* */

#include "mx_wifi_conf.h"
#include "mx_wifi_slip.h"
#include "mx_wifi_ipc.h"

/* debug function */
#ifdef MX_WIFI_SLIP_DEBUG
#define DEBUG_LOG(M, ...)  printf((M), ##__VA_ARGS__)
#else
#define DEBUG_LOG(M, ...)
#endif /* MX_WIFI_SLIP_DEBUG */

#define DEBUG_WARNING(...)
/* slip buffer number */
#define SLIP_BUFFER_SIZE        (MIPC_PKT_MAX_SIZE + 100)


enum
{
  SLIP_STATE_IDLE,       /* NOT START */
  SLIP_STATE_CONTINUE,   /* receive data to buffer */
  SLIP_STATE_GOT_ESCAPE  /* last byte is escape */
};


uint8_t *slip_transfer(uint8_t *data, uint16_t len, uint16_t *outlen)
{
  uint16_t i;
  uint16_t j;
  uint16_t inc = 2;
  uint8_t *buff = NULL;

  for (i = 0; i < len; i++)
  {
    if ((data[i] == SLIP_START) || (data[i] == SLIP_END) || (data[i] == SLIP_ESCAPE))
    {
      inc++;
    }
  }

  buff = (uint8_t *)MX_WIFI_MALLOC(len + inc);
  if (buff == NULL)
  {
    return NULL;
  }

  buff[0] = SLIP_START;
  for (i = 0, j = 1; i < len; i++)
  {
    if (data[i] == SLIP_START)
    {
      buff[j++] = SLIP_ESCAPE;
      buff[j++] = SLIP_ESCAPE_START;
    }
    else if (data[i] == SLIP_END)
    {
      buff[j++] = SLIP_ESCAPE;
      buff[j++] = SLIP_ESCAPE_END;
    }
    else if (data[i] == SLIP_ESCAPE)
    {
      buff[j++] = SLIP_ESCAPE;
      buff[j++] = SLIP_ESCAPE_ES;
    }
    else
    {
      buff[j++] = data[i];
    }
  }
  buff[j++] = SLIP_END;
  *outlen = j;

  return buff;
}


mx_buf_t *slip_input_byte(uint8_t data)
{
  mx_buf_t *outgoing_nbuf = NULL;
  static uint16_t slip_state = SLIP_STATE_IDLE;
  static uint16_t slip_index = 0;
  static uint8_t *slip_buffer = NULL;
  static mx_buf_t *nbuf = NULL;

  if (slip_buffer == NULL)
  {
    bool first_miss = true;
    do
    {
      nbuf = MX_NET_BUFFER_ALLOC(SLIP_BUFFER_SIZE);
      if (nbuf == NULL)
      {
        DELAYms(1);
        if (true == first_miss)
        {
          first_miss = false;
          DEBUG_WARNING("Running Out of buffer for RX\n");
        }
      }
    } while (NULL == nbuf);
    slip_buffer = MX_NET_BUFFER_PAYLOAD(nbuf);
  }

  if (slip_index >= SLIP_BUFFER_SIZE)
  {
    slip_index = 0;
    slip_state = SLIP_STATE_IDLE;
  }

  switch (slip_state)
  {
    case SLIP_STATE_GOT_ESCAPE:
      if (data == SLIP_START)
      {
        slip_index = 0;
      }
      else if (data == SLIP_ESCAPE_START)
      {
        slip_buffer[slip_index++] = SLIP_START;
      }
      else if (data == SLIP_ESCAPE_ES)
      {
        slip_buffer[slip_index++] = SLIP_ESCAPE;
      }
      else if (data == SLIP_ESCAPE_END)
      {
        slip_buffer[slip_index++] = SLIP_END;
      }
      else
      {
        goto RESET;
      }
      slip_state = SLIP_STATE_CONTINUE;
      break;

    case SLIP_STATE_IDLE:
      if (data == SLIP_START)
      {
        slip_index = 0;
        slip_state = SLIP_STATE_CONTINUE;
      }
      break;

    case SLIP_STATE_CONTINUE: /* continue */
      if (data == SLIP_START)
      {
        slip_index = 0;
        slip_state = SLIP_STATE_CONTINUE;
      }
      else if (data == SLIP_END)
      {
        outgoing_nbuf = nbuf;
        slip_buffer = NULL;
        MX_NET_BUFFER_SET_PAYLOAD_SIZE(nbuf, slip_index);
        nbuf = NULL;
        goto RESET;
      }
      else if (data == SLIP_ESCAPE)
      {
        slip_state = SLIP_STATE_GOT_ESCAPE;
      }
      else
      {
        slip_buffer[slip_index++] = data;
      }
      break;

    default:
      break;
  }

  return outgoing_nbuf;

RESET:
  slip_index = 0;
  slip_state = SLIP_STATE_IDLE;
  return outgoing_nbuf;
}

