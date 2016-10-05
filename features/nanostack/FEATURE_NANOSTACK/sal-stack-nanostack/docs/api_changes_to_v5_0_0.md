# API Changes

This document walks you through the API changes in the Nanostack version 5.0.x in detail and helps you to update your current applications to use the new and changed APIs.

**New APIs:**

- MAC API
- Ethernet MAC API
- Serial MAC API
- Virtual RF API
- SW MAC API

**Deleted APIs:**
- `br_list_nvm_api.h`
- `br_mle_lib.h`
- `socket_security.h`
- `cca_api.h`

**Disabled APIs:**
- `net_fhss.h`


**Other changes:**

- `net_interface.h`: Separated `arm_nwk_interface_init()` to MAC-specific initialisers.
  * `arm_nwk_interface_ethernet_init()`
  * `arm_nwk_interface_lowpan_init()`

- `arm_hal_phy.h`: Added rx and tx_done callbacks to `phy_device_driver_s` to be settable instead of the function.
  * `arm_net_phy_rx()` changed to `phy_device_driver_s.phy_rx_cb`
  * `arm_net_phy_tx_done()` changed to `phy_device_driver_s.phy_tx_done_cb`

- `arm_hal_phy.h`: Added rx and tx virtual callbacks to `phy_device_driver_s` to be settable by the upper layer.
  * `phy_device_driver_s.arm_net_virtual_rx_cb`
  * `phy_device_driver_s.arm_net_virtual_tx_cb`

- `mac_common_defines.h` added

- `nanostack/mac_mcps.h` added

- `mlme.h` published

- `net_nwk_scan.h` minor changes

- Local socket "support" removed. Serial MAC should be used instead.



