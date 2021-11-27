/**
  ******************************************************************************
  * @file    mx_wifi_hci.c
  * @author  MCD Application Team
  * @brief   Host driver HCI protocol of MXCHIP Wi-Fi component.
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
#include "stdlib.h"
/*cstat +MISRAC2012-* */
#include <inttypes.h>

#include "mx_wifi_conf.h"
#include "mx_wifi_hci.h"
#include "mx_wifi_slip.h"
#include "checksumutils.h"

#ifdef MX_WIFI_HCI_DEBUG
#define DEBUG_LOG(M, ...)       printf((M), ##__VA_ARGS__)
#else
#define DEBUG_LOG(M, ...)
#endif /* MX_WIFI_HCI_DEBUG */

#define DEBUG_ERROR(M, ...)     printf((M), ##__VA_ARGS__)

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* HCI low level function */
static hci_send_func_t tcl_output = NULL;
/* HCI recv data queue */
static FIFO_DECLARE(hci_pkt_fifo);

static bool mx_wifi_hci_pkt_verify(uint8_t *data, uint16_t len);


/* Private functions ---------------------------------------------------------*/
static bool mx_wifi_hci_pkt_verify(uint8_t *data, uint16_t len)
{
  /* here each uart slip frame or spi frame as a hci packet */
  return true;
}


/* Global functions ----------------------------------------------------------*/
int32_t mx_wifi_hci_init(hci_send_func_t low_level_send)
{
  int32_t ret = 0;

  tcl_output = low_level_send;

  if (0 == ret)
  {
    FIFO_INIT(hci_pkt_fifo, MX_WIFI_MAX_RX_BUFFER_COUNT);
  }

  return ret;
}

int32_t mx_wifi_hci_deinit(void)
{
  FIFO_DEINIT(hci_pkt_fifo);
  return 0;
}

int32_t mx_wifi_hci_send(uint8_t *payload, uint16_t len)
{
  int32_t ret = 0;
  uint16_t sent = 0;

#if MX_WIFI_USE_SPI
  sent = tcl_output(payload, len);
  if (len != sent)
  {
    DEBUG_ERROR("tcl_output(spi) error sent=%d !\n", sent);
    ret = -1;
  }
  else
  {
    ret = 0;
  }
#else
  uint8_t *slip_frame = NULL;
  uint16_t slip_len = 0;

  slip_frame = slip_transfer(payload, len, &slip_len);
  if ((NULL != slip_frame) && (slip_len > 0))
  {
    sent = tcl_output(slip_frame, slip_len);
    if (slip_len == sent)
    {
      ret = 0;
    }
    else
    {
      DEBUG_ERROR("tcl_output(uart) error sent=%d !\r\n", sent);
      ret = -1;
    }

    MX_WIFI_FREE(slip_frame);
    slip_frame = NULL;
  }
  else
  {
    DEBUG_ERROR("create slip frame error!\r\n");
    ret = -2;
  }
#endif /* MX_WIFI_USE_SPI */

  return ret;
}

void process_txrx_poll(uint32_t timeout);

mx_buf_t *mx_wifi_hci_recv(uint32_t timeout)
{
  mx_buf_t *nbuf;
  nbuf = (mx_buf_t*) FIFO_POP(hci_pkt_fifo, timeout, process_txrx_poll);
  if (nbuf != NULL)
  {
    MX_STAT(out_fifo);
  }
  return nbuf;
}

void mx_wifi_hci_free(mx_buf_t *nbuf)
{
  if (NULL != nbuf)
  {
    (void) MX_NET_BUFFER_FREE(nbuf);
  }
}


/**
  * @brief LOW LEVEL INTERFACE
  */
void mx_wifi_hci_input(mx_buf_t *netbuf)
{
  if (NULL != netbuf)
  {
    uint8_t  *data = MX_NET_BUFFER_PAYLOAD(netbuf);
    uint32_t  len  = MX_NET_BUFFER_GET_PAYLOAD_SIZE(netbuf);

    if ((NULL != data) && (len > 0))
    {
      if (mx_wifi_hci_pkt_verify(data, len))
      {
        if (FIFO_OK != FIFO_PUSH(hci_pkt_fifo, netbuf, WAIT_FOREVER, NULL))
        {
          DEBUG_ERROR("push tcl input queue err!\n");
          MX_NET_BUFFER_FREE(netbuf);
        }
        else
        {
          DEBUG_LOG("\nhci input len %"PRIu32"  nbuf : %p\n", len, netbuf);
          MX_STAT(in_fifo);
        }
      }
      else
      {
        DEBUG_LOG("input bad tcl pkt!\n");
        MX_NET_BUFFER_FREE(netbuf);
      }
    }
  }
}

