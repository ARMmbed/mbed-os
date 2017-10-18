#ifndef __SL_ETH_NETIF_H__
#define __SL_ETH_NETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include "sl_eth_phy.h"

// -----------------------------------------------------------------------------
// Config options
// -----------------------------------------------------------------------------
/** Number of descriptors in receive list */
#define SL_ETH_NUM_RX_BD              (16)
/** Number of descriptors in transmit list */
#define SL_ETH_NUM_TX_BD              (8)
/** Size of one buffer in a buffer descriptor (must be multiple of 64) */
#define SL_ETH_RX_BUF_SIZE            (1536)
/** Timeout in milliseconds for polling the PHY link status */
#define SL_ETH_LINK_POLL_PERIOD_MS    (500)

#endif