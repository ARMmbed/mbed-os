/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
 */
#ifndef PPP_LWIP_H_
#define PPP_LWIP_H_

#ifdef __cplusplus
extern "C" {
#endif
nsapi_error_t ppp_lwip_if_init(struct netif *netif);
err_t ppp_lwip_connect(void);
#ifdef __cplusplus
}
#endif

#endif /* PPP_LWIP_H_ */
