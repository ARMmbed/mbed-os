
/***************************************************************************//**
 * @file ieee802154_gb868_efr32xg1_configurator_out.h
 * @brief IEEE802154 GB868_Configuration
 * @copyright Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __IEEE802154_GB868_EFR32XG1_CONFIGURATOR_OUT_H__
#define __IEEE802154_GB868_EFR32XG1_CONFIGURATOR_OUT_H__

#include <stdint.h>

#define IEEE802154_863_RADIO_CONFIG_BASE_FREQUENCY 868300000UL
#define IEEE802154_863_RADIO_CONFIG_XTAL_FREQUENCY 38400000UL
#define IEEE802154_863_RADIO_CONFIG_BITRATE "100kbps"
#define IEEE802154_863_RADIO_CONFIG_MODULATION_TYPE "OQPSK"
#define IEEE802154_863_RADIO_CONFIG_DEVIATION "333.3kHz"

extern const uint32_t ieee802154_config_863[];
extern const uint32_t ieee802154_config_863_min[];

#define IEEE802154_915_RADIO_CONFIG_BASE_FREQUENCY 906000000UL
#define IEEE802154_915_RADIO_CONFIG_XTAL_FREQUENCY 38400000UL
#define IEEE802154_915_RADIO_CONFIG_BITRATE "250kbps"
#define IEEE802154_915_RADIO_CONFIG_MODULATION_TYPE "OQPSK"
#define IEEE802154_915_RADIO_CONFIG_DEVIATION "333.3kHz"

extern const uint32_t ieee802154_config_915[];
extern const uint32_t ieee802154_config_915_min[];

#endif // __IEEE802154_GB868_EFR32XG1_CONFIGURATOR_OUT_H__
