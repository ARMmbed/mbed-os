/**
  ******************************************************************************
  * @file    mx_wifi_hci.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi_hci.c module
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
#ifndef MX_WIFI_HCI_H
#define MX_WIFI_HCI_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/*cstat -MISRAC2012-* */
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
/*cstat +MISRAC2012-* */

#include "mx_wifi_ipc.h"

/**
  * @brief CONFIGURATIONS
  */
/* #define MX_WIFI_HCI_DEBUG */

/**
  * @brief HCI PACKET
  */
#define MX_WIFI_HCI_DATA_SIZE   (MIPC_PKT_MAX_SIZE)

/**
  * @brief PROTOTYPES
  */
typedef struct _hci_pkt_s
{
  uint16_t len;
  uint8_t *data;
} hci_pkt_t;

typedef uint16_t (*hci_send_func_t)(uint8_t *data, uint16_t size);

/**
  * @brief API
  */
int32_t mx_wifi_hci_init(hci_send_func_t low_level_send);
int32_t mx_wifi_hci_send(uint8_t *payload, uint16_t len);
mx_buf_t *mx_wifi_hci_recv(uint32_t timeout);
void mx_wifi_hci_free(mx_buf_t *nbuf);
int32_t mx_wifi_hci_deinit(void);

/**
  * @brief LOW LEVEL INTERFACE
  */
void mx_wifi_hci_input(mx_buf_t *netbuf);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_HCI_H */
