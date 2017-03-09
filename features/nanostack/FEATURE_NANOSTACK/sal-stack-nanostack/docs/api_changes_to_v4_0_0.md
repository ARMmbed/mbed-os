# API Changes

This document walks you through the API changes in the Nanostack version 4.0.x in detail and helps you to update your current applications to use the new and changed APIs.

**New APIs:**

- MLE router and host lifetime configuration API
- MLE neighbor limits configuration API
- MLE token bucket configuration API
- API for adding/deleting routes
- FHSS API

**Other changes:**

- Function `arm_nwk_6lowpan_link_scan_paramameter_set()` renamed to `arm_nwk_6lowpan_link_scan_parameter_set()` (due to misspelling)
- Channel mask settings API changed
- The parameters of function `cca_start()` changed


## The Network Layer Control APIs

### MLE router and host lifetime configuration API

**Problem:**

MLE router and host lifetimes were not configurable.

**Solution:**

Added functions `arm_nwk_6lowpan_mle_router_lifetime_set()` and `arm_nwk_6lowpan_mle_host_lifetime_set()`
that can be used to set the MLE neighbor lifetime. The router lifetime controls also the MLE advertisement period that
is the router lifetime in seconds divided by four. The usage of the MLE router and host lifetime configuration is
optional. If the lifetimes are not set, the default router lifetime is 128 seconds and host lifetime 400 seconds.

### MLE neighbor limits configuration API

**Problem:**

The size of the MLE neighbor list was not limited. It would take as much space as there are neighbors.

**Solution:**

Added function `arm_nwk_6lowpan_mle_neighbor_limits_set()` that can be used to set limits for the MLE
neighbor list. With this function, you can set the lower threshold, upper threshold and maximum neighbor count. If the number of neighbors reaches the lower threshold, MLE starts to limit addition of new neighbors. If the number of neighbors reaches
upper threshold, MLE stops adding new neighbors based on multicast MLE messages. The usage of the MLE neighbor list limits is optional. If no limits are set, the number of neighbors is unlimited.

## MLE token bucket configuration API

**Problem:**

The sending rate for MLE messages was not configurable.

**Solution:**

Added function `arm_nwk_6lowpan_mle_token_bucket_settings_set()` that can be used to configure the MLE message
sending rate. With this function, you can set the token bucket size, rate and count. The token bucket size defines the
bucket size. The token bucket rate defines the rate for adding new tokens. The count defines how many
tokens at a time are added to the bucket. The minimum interval of the rate is 0.1 seconds (for example, if the rate is 3 and
the count is 4, then 4 new tokens are added to bucket every 0.3 seconds). The usage of MLE token bucket is optional. If the token bucket is not used, the MLE message sending rate is unlimited.

## Applications

### Node application

**Problem:**

Misspelled function `arm_nwk_6lowpan_link_scan_paramameter_set()`.

**Solution:**

Rename the current `arm_nwk_6lowpan_link_scan_paramameter_set()` function call to `arm_nwk_6lowpan_link_scan_parameter_set()`.

**Problem:**

Parameter `uint32_t channel_mask` is removed from `arm_nwk_6lowpan_link_scan_parameter_set()` function.

**Solution:**

- Remove parameter `channel_mask` from all `arm_nwk_6lowpan_link_scan_parameter_set()` function calls.
- Add a new function call `arm_nwk_set_channel_list();` after `arm_nwk_6lowpan_link_scan_parameter_set()` function call.

		channel_list_s channel_list = {CHANNEL_PAGE_0, 0x07FFF800};
		int main(void)
		{
			.
			.
			.
			net_init_core();
			net_rf_id = arm_nwk_interface_init(NET_INTERFACE_RF_6LOWPAN, rf_phy_device_register_id,
			"6LoWPAN_ROUTER");
			int8_t retval = arm_nwk_6lowpan_link_scan_parameter_set(net_rf_id, 5);
			if (retval) {
				printf(" Scan parameter set failed! Return value = %d\n", retval);
			}
			retval = arm_nwk_set_channel_list(net_rf_id, &channel_list);
			if (retval) {
				printf("Channel set failed! Return value = %d\n", retval);
			}
			.
			.
			.
		}

Function `arm_nwk_set_channel_list(int8_t interface_id, const channel_list_s* nwk_channel_list)` is defined in `net_interface.h`. Instead of using a 32-bit variable, the new API is using structure `channel_list_s`
to define the scanned channel page and channels.

Structure `channel_list_s` is defined in `net_interface.h`:

	typedef struct channel_list_s
	{
		channel_page_e channel_page;
		uint32_t channel_mask[8];
	} channel_list_s;

where enumeration `channel_page_e` from `arm_hal_phy.h` defines the supported channel pages:

	typedef enum
	{
		CHANNEL_PAGE_0 = 0,
		CHANNEL_PAGE_1 = 1,
		CHANNEL_PAGE_2 = 2,
		CHANNEL_PAGE_3 = 3,
		CHANNEL_PAGE_4 = 4,
		CHANNEL_PAGE_5 = 5,
		CHANNEL_PAGE_6 = 6,
		CHANNEL_PAGE_9 = 9,
		CHANNEL_PAGE_10 = 10
	} channel_page_e;

`channel_mask` is an array of 32-bit variables allowing up to 255 channels.

The channel list can be defined using global a structure, for example:

	// IEEE 802.15.4-2006 for 2.4GHz band (channels 11-26)
	channel_list_s channel_list = {CHANNEL_PAGE_0, 0x07FFF800};

or

	// IEEE 802.15.4-2006 for Sub 1GHz band (channels 0-10)
	channel_list_s channel_list = {CHANNEL_PAGE_2, 0x000007ff};

### Border router application

#### Border router without external RF interface

**Problem:**

Structure `border_router_setup_s` content has changed because variable `channel` is removed.

**Solution:**

- Remove all references to `(border_router_setup_s) border_router_setup.channel`.
- Add a new function call `arm_nwk_set_channel_list();` to set the channel.

		void start_6lowpan(void)
		{
    		uint8_t p[16];
    		if (0 == arm_net_address_get(net_backhaul_id, ADDR_IPV6_GP, p)) {
        		memcpy(br_def_prefix, p, 8);
        		uint32_t lifetime = 0xffffffff;
        		uint8_t t_flags = 0;
        		uint8_t prefix_len =0;
        		arm_nwk_interface_configure_6lowpan_bootstrap_set(net_6lowpan_id, operating_mode,
				operating_mode_extension);
    			arm_nwk_set_channel_list(net_6lowpan_id, &channel_list);
			.
			.
			}
		}

Function `arm_nwk_set_channel_list(int8_t interface_id, const channel_list_s *nwk_channel_list)` is defined in `net_interface.h`.

Structure `channel_list_s` is defined in `net_interface.h`:

	typedef struct channel_list_s
	{
		channel_page_e channel_page;
		uint32_t channel_mask[8];
	} channel_list_s;

where enumeration `channel_page_e` from `arm_hal_phy.h` defines the supported channel pages:

	typedef enum
	{
		CHANNEL_PAGE_0 = 0,
		CHANNEL_PAGE_1 = 1,
		CHANNEL_PAGE_2 = 2,
		CHANNEL_PAGE_3 = 3,
		CHANNEL_PAGE_4 = 4,
		CHANNEL_PAGE_5 = 5,
		CHANNEL_PAGE_6 = 6,
		CHANNEL_PAGE_9 = 9,
		CHANNEL_PAGE_10 = 10
	} channel_page_e;

`channel_mask` is an array of 32-bit variables allowing up to 255 channels.

The channel can be defined using a global structure, for example:

	// IEEE 802.15.4-2006 for 2.4GHz band channel 11
	channel_list_s channel_list = {CHANNEL_PAGE_0, 0x00000800};

or

	// IEEE 802.15.4-2006 for Sub 1GHz band channel 0
	channel_list_s channel_list = {CHANNEL_PAGE_2, 0x00000001};

#### Border router (linux) with external RF interface

In addition to what was changed in section [Border router without external RF interface](#border-router-without-external-rf-interface), channel setting needs to be updated also to the NAP interface.

**Problem:**

Instead of the channel number, channel page and channel mask need to be delivered to the RF interface.

**Solution:**

- Define the channel list as in the previous section and send it to the RF interface in configuration structure:

		int app_nap_STATE_MACHINE(void)
		{
    		uint16_t length = 0;
    		libnap_network_parameters_s appNetworkSetup = {
        		.beaconProtocolId = br.beacon_protocol_id,
        		.channel_page = channel_list.channel_page,
        		.network_mode = libNAP_RF_INTERFACE_BOOTSRAP,
        		.panID =  br.mac_panid,
        		.shortAddress = br.mac_short_adr,
        		.networkIdPtr = br.network_id,
        		.steeringTlvLength = 0,
        		.steeringTlvPtr = 0,
    		};
    		memcpy(appNetworkSetup.channel_mask, channel_list.channel_mask, sizeof(uint32_t) * 8);

    		switch (server_state) {
			.
			.
			.
        		case libNAP_SET_CONFIGURATION:
            			length = libNAPBuildMessage(tx_buffer, libNAP_SET_HOST_NETWORK_CONFIGURE_CMD,
						&appNetworkSetup, 0, 0);
            			break;
			.
			.
			.
			}
		}


### Network driver application

**Problem:**

In the new API, channel page and channel list are delivered instead of the channel number. The parameters in the function `arm_nwk_interface_network_driver_set()` have changed.

**Solution:**

- When the network driver application is parsing the `libNAP_SET_HOST_NETWORK_CONFIGURE_CMD` event received from the master device, it needs to read the channel page and channel mask from the received configuration
and use them with the `arm_nwk_interface_network_driver_set()` function call.

		void app_nap_parser_ok(libNAP_command_types_e type, libnap_network_parameters_s *config,
		uint8_t *data_ptr, uint16_t data_length)
		{
			channel_list_s channel_list;
			switch (type) {
				.
				.
				.
				case libNAP_SET_HOST_NETWORK_CONFIGURE_CMD:
					.
					.
					.
					channel_list.channel_page = (channel_page_e) config->channel_page;
					memcpy(channel_list.channel_mask, config->channel_mask, sizeof(uint32_t) * 8);
					arm_nwk_interface_configure_6lowpan_bootstrap_set(net_6lowpan_id, boot_mode,
					(net_6lowpan_mode_extension_e) 0);
					if (arm_nwk_interface_network_driver_set(net_6lowpan_id, tun_driver_id,
					&channel_list, conf_ptr) == 0) {
						host_state = libNAP_CONFIGURATION_READY;
					}
					break;
			}
			.
			.
			.
		}

**Problem:**

Nanostack has introduced a public API for adding/deleting routes. As a consequence, default route ::/0 (on-link) is no longer automatically added by nanostack for tunnel interfaces. Border router needs to set a default route manually by using the route add/delete interface.

**Solution:**

Function `int8_t arm_net_route_add(const uint8_t *prefix, uint8_t prefix_len, const uint8_t *next_hop, uint32_t lifetime, uint8_t metric, int8_t interface_id)` is defined in `net_interface.h`.

For example, to add the same default route `::/0 (on-link)` which was earlier added by nanostack, add the following line to the border router application:

		arm_net_route_add(NULL, 0, NULL, 0xffffffff, 128, interface_id);

where value `0xffffffff` denotes to infinite route lifetime, and value `128` to the default metric value. `NULL` value for `prefix` and `next_hop` parameters means `::/0` prefix and on-link route, respectively. Note that a proper configuration should _always_ add a next-hop value (e.g. link-local address of the neighbouring router) to a default route.

## PHY drivers

### PHY driver registration

**Problem:**

The content in structure `phy_device_driver_s` has changed. The structure `phy_device_channel_info_s` is replaced with `phy_device_channel_page_s` that contains information about supported channel pages,
channel masks, RF configurations etc.

**Solution:**

- Remove the existing `phy_device_channel_info_s` structure and all references to it.
- Create a new structure `phy_device_channel_page_s` (defined in `arm_hal_phy.h`):

		const phy_device_channel_page_s phy_channel_pages[] = {
			{CHANNEL_PAGE_0, &phy_rf_conf},
			{CHANNEL_PAGE_0, NULL}
		};

where

		const phy_rf_channel_configuration_s phy_rf_conf =
		{RF_FREQUENCY, RF_CHANNEL_SPACING, RF_DATARATE, RF_NUMBER_OF_CHANNELS, RF_MODULATION};

Fill the `phy_rf_conf` structure with:

- Channel 0 center frequency (Hz)
- Channel spacing (Hz)
- Datarate (bit/s)
- Number of supported channels
- Modulation (`M_OFDM`, `M_OQPSK`, `M_BPSK`, `M_GFSK`, `M_UNDEFINED`)

For example, if using channel page 0 as in IEEE 802.15.4-2006 for the 2.4GHz band (channels 11-26), create the following structures:

		const phy_rf_channel_configuration_s phy_2_4ghz = {2405000000, 5000000, 250000, 16, M_OQPSK};

		const phy_device_channel_page_s phy_channel_pages[] = {
			{ CHANNEL_PAGE_0, &phy_2_4ghz},
			{ CHANNEL_PAGE_0, NULL}
		};

If using channel page 2 as in IEEE 802.15.4-2006 for Sub 1GHz band (channels 0-10), create the following structures:

		const phy_rf_channel_configuration_s phy_sub1ghz = {868300000, 2000000, 250000, 11, M_OQPSK};

		const phy_device_channel_page_s phy_channel_pages[] = {
			{ CHANNEL_PAGE_2, &phy_sub1ghz},
			{ CHANNEL_PAGE_0, NULL}
		};

**Note**: Channel 0 is on 868MHz band and channels 1-10 above 900MHz and thereby, channel spacing 2MHz is not valid, but is acceptable in this case.

If the PHY driver is supporting several configurations, they can be all defined:

		const phy_device_channel_page_s phy_channel_pages[] = {
			{ CHANNEL_PAGE_0, &phy_2_4ghz},
			{ CHANNEL_PAGE_2, &phy_sub1ghz},
			{ CHANNEL_PAGE_0, NULL}
		};

**Note**: `phy_device_channel_page_s` last entry MUST be `{CHANNEL_PAGE_0, NULL}`.

- Add supported channel pages in the device driver structure:

		static phy_device_driver_s device_driver;
		.
		.
		.
		device_driver.phy_channel_pages = phy_channel_pages;

- Structure `phy_device_driver_s` is registered to NanoStack with `arm_net_phy_register()` function call as earlier:

		rf_driver_id = arm_net_phy_register(&device_driver);

### CCA module

If the PHY driver was using NanoStack internal CCA module, the API call needs to be updated.

**Problem:**

The parameters of function `cca_start()` have changed. The function call now needs to tell the driver id that was given by the NanoStack library when the driver was registered.
Parameters `cca_check_fptr` and `cca_done_fptr` are also now using `channel_status_e` enumeration that is defined in `cca_api.h`.

**Solution:**

- Replace the current `cca_start()` function call:

		cca_start(CCA_RANDOM_IN_RANGE, RF_CCA_MIN, RF_CCA_MAX, rf_check_cca, rf_tx_start);

with:

		cca_start(rf_driver_id, CCA_RANDOM_IN_RANGE, RF_CCA_MIN, RF_CCA_MAX, rf_check_cca, rf_tx_start);

where `rf_driver_id` is the return value from the `arm_net_phy_register()` function call.

- To fix the parameters `cca_check_fptr` and `cca_done_fptr`, replace the current definitions:

		static int rf_check_cca(void); -> static channel_status_e rf_check_cca(void);
		static void rf_tx_start(int channel_status);
		-> static void rf_tx_start(channel_status_e channel_status);

- Fix also the corresponding channel status usage, for example:

		static channel_status_e rf_check_cca(void) {
			if(rf_read_cca_status() == OK)
				return CHANNEL_IDLE;
			return CHANNEL_NOT_IDLE;
		}

		static void rf_tx_start(channel_status_e channel_status) {
			if(channel_status == CHANNEL_NOT_IDLE)	{
			arm_net_phy_tx_done(rf_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 1, 1);
			} else {
				.
				.
				.
			}
		}
