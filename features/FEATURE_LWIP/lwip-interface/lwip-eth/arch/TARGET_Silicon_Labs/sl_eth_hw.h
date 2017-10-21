#ifndef SL_ETH_HW_H
#define SL_ETH_HW_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Sets up hardware pins and configures internal clocks
 */
void sl_eth_hw_init(void);

/**
 * Releases hardware pins and turns off internal clocks
 */
void sl_eth_hw_deinit(void);

#if defined(__cplusplus)
}
#endif

#endif
