/*
 * Copyright (c) 2014-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

#ifndef NET_INTERFACE_H_
#define NET_INTERFACE_H_

#include "ns_types.h"
#include "platform/arm_hal_phy.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mac_api_s;
struct eth_mac_api_s;

/**
 * \file net_interface.h
 * \brief  Network API
*/

/** Network Interface Status */
typedef enum arm_nwk_interface_status_type_e {
    ARM_NWK_BOOTSTRAP_READY = 0, /**< Interface configured Bootstrap is ready.*/
    ARM_NWK_RPL_INSTANCE_FLOODING_READY, /**< RPL instance has been flooded. */
    ARM_NWK_SET_DOWN_COMPLETE, /**< Interface DOWN command completed successfully. */
    ARM_NWK_NWK_SCAN_FAIL,  /**< Interface has not detected any valid network. */
    ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL, /**< IP address allocation failure (ND, DHCPv4 or DHCPv6). */
    ARM_NWK_DUPLICATE_ADDRESS_DETECTED, /**< User-specific GP16 was not valid. */
    ARM_NWK_AUHTENTICATION_START_FAIL, /**< No valid authentication server detected behind the access point. */
    ARM_NWK_AUHTENTICATION_FAIL,    /**< Network authentication failed by handshake. */
    ARM_NWK_NWK_CONNECTION_DOWN, /**< No connection between access point and default router. */
    ARM_NWK_NWK_PARENT_POLL_FAIL, /**< Sleepy host poll failed 3 times. Interface is shut down. */
    ARM_NWK_PHY_CONNECTION_DOWN, /**< Interface PHY cable off or serial port interface not responding anymore. */
} arm_nwk_interface_status_type_e;

/** Event library type. */
typedef enum arm_library_event_type_e {
    ARM_LIB_TASKLET_INIT_EVENT = 0, /**< Tasklet init occurs always when generating a tasklet. */
    ARM_LIB_NWK_INTERFACE_EVENT,    /**< Interface bootstrap or state update event. */
    ARM_LIB_SYSTEM_TIMER_EVENT, /*!*< System timer event. */
    APPLICATION_EVENT, /**< Application-specific event. */
} arm_library_event_type_e;


/*
 *  Socket event description:
 *
 *  8-bit variable where four MSB bits describes the event type and
 *  four LSB bits describes the socket that has received the event.
 *
 *      Type   Socket ID
 *      ----   ----
 *      xxxx   xxxx
 *
 */


/** \name Socket type exceptions.
 * @{
 */
/** Socket event mask. */
#define SOCKET_EVENT_MASK                   0xF0
/** Data received. */
#define SOCKET_DATA                         (0 << 4)
/** TCP connection ready. */
#define SOCKET_CONNECT_DONE                 (1 << 4)
/** TCP connection failure. */
#define SOCKET_CONNECT_FAIL                 (2 << 4)
/** TCP connection authentication failed. */
#define SOCKET_CONNECT_AUTH_FAIL            (3 << 4)
/** TCP incoming connection on listening socket */
#define SOCKET_INCOMING_CONNECTION          (4 << 4)
/** Socket data send failure. */
#define SOCKET_TX_FAIL                      (5 << 4)
/** TCP connection closed (received their FIN and ACK of our FIN). */
#define SOCKET_CONNECT_CLOSED               (6 << 4)
/** TCP connection reset */
#define SOCKET_CONNECTION_RESET             (7 << 4)
/** No route available to the destination. */
#define SOCKET_NO_ROUTE                     (8 << 4)
/** Socket TX done. */
#define SOCKET_TX_DONE                      (9 << 4)
/** Out of memory failure. */
#define SOCKET_NO_RAM                       (10 << 4)
/** TCP connection problem indication (RFC 1122 R1) */
#define SOCKET_CONNECTION_PROBLEM           (11 << 4)

#define SOCKET_BIND_DONE                    SOCKET_CONNECT_DONE      /**< Backward compatibility */
#define SOCKET_BIND_FAIL                    SOCKET_CONNECT_FAIL      /**< Backward compatibility */
#define SOCKET_BIND_AUTH_FAIL               SOCKET_CONNECT_AUTH_FAIL /**< Backward compatibility */
/* @} */

/** Network security levels. */
typedef enum net_security_t {
    NW_NO_SECURITY = 0,                       /**< No Security. */
    NW_SECURITY_LEVEL_MIC32 = 1,              /**< 32-bit MIC verify, no encoding. */
    NW_SECURITY_LEVEL_MIC64 = 2,              /**< 64-bit MIC verify, no encoding. */
    NW_SECURITY_LEVEL_MIC128 = 3,             /**< 128-bit MIC verify, no encoding. */
    NW_SECURITY_LEVEL_ENC = 4,                /**< AES encoding without MIC. */
    NW_SECURITY_LEVEL_ENC_MIC32 = 5,          /**< 32-bit MIC verify with encoding. */
    NW_SECURITY_LEVEL_ENC_MIC64 = 6,          /**< 64-bit MIC verify with encoding. */
    NW_SECURITY_LEVEL_ENC_MIC128 = 7          /**< 128-bit MIC verify with encoding. */
} net_security_t;

/** Ipv6 address type.*/
typedef enum net_address_t {
    ADDR_IPV6_GP,             /**< Node default global address. */
    ADDR_IPV6_GP_SEC,         /**< Node secondary global address. */
    ADDR_IPV6_LL              /**< Node default link local address. */
} net_address_t;

/** MAC address type. */
typedef enum net_mac_address_t {
    ADDR_MAC_SHORT16,             /**< Nodes 16-bit short address. */
    ADDR_MAC_LONG64,              /**< IP layer EUID64 based on MAC layer 64-bit long address after U/I -bit conversion.  */
} net_mac_address_t;

/** TLS cipher type */
typedef enum {
    NET_TLS_PSK_CIPHER,         /**< Network authentication support, only PSK. */
    NET_TLS_ECC_CIPHER,         /**< Network authentication support, only ECC. */
    NET_TLS_PSK_AND_ECC_CIPHER, /**< Network authentication support, PSK & ECC. */
} net_tls_cipher_e;

/** PANA session type. */
typedef enum {
    NET_PANA_SINGLE_SESSION,        /**< Client tracks only one PANA session data, default use case. */
    NET_PANA_MULTI_SESSION,         /**< Client supports many Start network coordinator session data */
} net_pana_session_mode_e;

/** 6LoWPAN network security & authentication modes. */
typedef enum {
    NET_SEC_MODE_NO_LINK_SECURITY,      /**< Security disabled at link layer, DEFAULT. */
    NET_SEC_MODE_PSK_LINK_SECURITY,     /**< Link security by PSK key. */
    NET_SEC_MODE_PANA_LINK_SECURITY, /**< PANA network authentication defined link KEY. */
} net_6lowpan_link_layer_sec_mode_e;


/** Bootstrap modes */
typedef enum {
    NET_6LOWPAN_BORDER_ROUTER,  /**< Root device for 6LoWPAN ND. */
    NET_6LOWPAN_ROUTER,         /**< Router device. */
    NET_6LOWPAN_HOST,           /**< Host device DEFAULT setting. */
    NET_6LOWPAN_SLEEPY_HOST,    /**< Sleepy host device. */
    NET_6LOWPAN_NETWORK_DRIVER, /**< 6LoWPAN radio host device, no bootstrap. */
    NET_6LOWPAN_SNIFFER         /**< Sniffer device, no bootstrap. */
} net_6lowpan_mode_e;

/**  6LoWPAN Extension modes. */
typedef enum {
    NET_6LOWPAN_ND_WITHOUT_MLE,         /**< **UNSUPPORTED** */
    NET_6LOWPAN_ND_WITH_MLE,            /**< 6LoWPAN ND with MLE. */
    NET_6LOWPAN_THREAD,                 /**< 6LoWPAN Thread with MLE attached. */
    NET_6LOWPAN_ZIGBEE_IP               /**< **UNSUPPORTED** */
} net_6lowpan_mode_extension_e;


/** IPv6 bootstrap modes */
typedef enum {
    NET_IPV6_BOOTSTRAP_STATIC,  /**< Application defines the IPv6 prefix. */
    NET_IPV6_BOOTSTRAP_AUTONOMOUS /**< Interface gets IPv6 address automatically from network using ICMP and DHCP. */
} net_ipv6_mode_e;

/** IPv6 accept RA behaviour */
typedef enum {
    NET_IPV6_RA_ACCEPT_IF_AUTONOMOUS, /**<Accept Router Advertisements when using autonomous IPv6 address allocation. Ignore when using a static address. This is the default value for the setting. */
    NET_IPV6_RA_ACCEPT_ALWAYS         /**<Accept Router Advertisements always, even when using static IPv6 address allocation. */
} net_ipv6_accept_ra_e;

/** Network coordinator parameter list.
 * Structure is used to read network parameter for warm start.
 */
typedef struct link_layer_setups_s {
    uint16_t PANId;            /**< Network PAN-ID. */
    uint8_t LogicalChannel;    /**< Network logical channel. */
    net_mac_address_t addr_mode;   /**< Coordinator address mode. */
    uint8_t address[8];        /**< Coordinator address. */
    uint8_t sf;                /**< Network superframe setup. */
} link_layer_setups_s;

/** Network MAC address info. */
typedef struct link_layer_address_s {
    uint16_t PANId;            /**< Network PAN-ID. */
    uint16_t mac_short;        /**< MAC short address, if <0xfffe then is valid. */
    uint8_t mac_long[8];       /**< MAC long address (EUI-48 for Ethernet; EUI-64 for IEEE 802.15.4). */
    uint8_t iid_eui64[8];      /**< IPv6 interface identifier based on EUI-64. */
} link_layer_address_s;

/** Network layer parent address info. */
typedef struct network_layer_address_s {
    uint8_t border_router[16]; /**< ND Border Router Address. */
    uint8_t prefix[8];        /**< Long 64-bit network ID. */
} network_layer_address_s;

/** Different addressing modes for a network interface. */
typedef enum {
    NET_6LOWPAN_GP64_ADDRESS,       /**< Interface registers only GP64 address. */
    NET_6LOWPAN_GP16_ADDRESS,       /**< Interface registers only GP16 address. */
    NET_6LOWPAN_MULTI_GP_ADDRESS,   /**< Interface registers GP16 & GP64 addresses. */
} net_6lowpan_gp_address_mode_e;

/** TLS PSK info */
typedef struct net_tls_psk_info_s {
    uint32_t key_id;    /**< PSK Key ID can be 0x01-0xffff, storage size is intentionally 32 bits. */
    uint8_t key[16];    /**< 128-bit PSK Key. */
} net_tls_psk_info_s;

/** NETWORK PSK link key structure. */
typedef struct {
    uint8_t key_id;             /**< Link layer PSK Key ID, can be 0x01-0xff. */
    uint8_t security_key[16];   /**< Link layer 128-bit PSK Key. */
} net_link_layer_psk_security_info_s;

/** Certificate chain structure. */
typedef struct {
    uint8_t chain_length;           /**< Certificate chain length, indicates the chain length. */
    const uint8_t *cert_chain[4];   /**< Certificate chain pointer list. */
    uint16_t cert_len[4];           /**< Certificate length. */
    const uint8_t *key_chain[4];    /**< Certificate private key. */
} arm_certificate_chain_entry_s;

/** Structure for the network keys used by net_network_key_get */
typedef struct ns_keys_t

{
    uint8_t previous_active_network_key[16];    /**< The key that is currently active when a new key is generated and activated. */
    uint8_t previous_active_key_index;           /**< The index associated to the current_active_network_key. */
    uint8_t current_active_network_key[16];     /**< Last generated and activated key. */
    uint8_t current_active_key_index;           /**< The index associated to the current_active_network_key. */
} ns_keys_t;

/** 6LoWPAN border router information structure. */
typedef struct {
    uint16_t mac_panid;             /**< Link layer PAN-ID, accepts only < 0xfffe.  */
    uint16_t mac_short_adr;         /**< Defines 802.15.4 short address. If the value is <0xfffe it indicates that GP16 is activated. */
    uint8_t beacon_protocol_id;     /**< ZigBeeIP uses always 2. */
    uint8_t network_id[16];         /**< Network ID 16-bytes, will be used at beacon payload. */
    uint8_t lowpan_nd_prefix[8];    /**< Define ND default prefix, ABRO, DODAG ID, GP address. */
    uint16_t ra_life_time;          /**< Define ND router lifetime in seconds, recommend value 180+. */
    uint32_t abro_version_num;      /**< ND ABRO version number (0 when starting a new ND setup). */
} border_router_setup_s;

/** Channel list */
typedef struct channel_list_s
{
    channel_page_e channel_page;    /**< Channel page */
    uint32_t channel_mask[8];       /**< Channel mask. Each bit defining one channel */
} channel_list_s;

/** 6LoWPAN radio interface setup. */
typedef struct {
    uint16_t mac_panid;                 /**< Link layer PAN-ID, accepts only < 0xfffe. */
    uint16_t mac_short_adr;             /**< Defines 802.15.4 short address. If the value is <0xfffe it indicates that GP16 is activated. */
    uint8_t beacon_protocol_id;         /**< ZigBeeIP uses always 2. */
    uint8_t network_id[16];             /**< Network ID 16-bytes, will be used at beacon payload. */
    uint8_t beacon_payload_tlv_length; /**< Optional steering parameter length. */
    uint8_t *beacon_payload_tlv_ptr;  /**< Optional steering parameters. */
} network_driver_setup_s;

/**
  * Init 6LoWPAN library
  *
  * \return 0, Init OK.
  */
extern int8_t net_init_core(void);

/**
 * \brief Create network interface base to IDLE state.
 * \param api Generates interface with ethernet MAC.
 * \param interface_name_ptr String pointer to interface name. Need to end to '\0' character.
 *        Max length 32 characters including NULL at end. Note: the given name is not copied,
 *        so it must remain valid as long as the interface is.
 *
 * \return >=0 Interface ID (0-127). Application needs to save this information.
 * \return -1 api was NULL.
 * \return -2 Ethernet is not supported at this build.
 * \return -3 No memory for the interface.
 */
extern int8_t arm_nwk_interface_ethernet_init(struct eth_mac_api_s *api, const char *interface_name_ptr);

/**
 * \brief Create network interface base to IDLE state.
 * \param api Generates interface with 802.15.4 MAC.
 * \param interface_name_ptr String pointer to interface name. Need to end to '\0' character.
 *        Max length 32 characters including NULL at end. Note: the given name is not copied,
 *        so it must remain valid as long as the interface is.
 *
 * \return >=0 Interface ID (0-127). Application needs to save this information.
 * \return -1 api was NULL.
 * \return -3 No memory for the interface.
 */
extern int8_t arm_nwk_interface_lowpan_init(struct mac_api_s *api, char *interface_name_ptr);

/**
 * \brief Set IPv6 interface setup.
 *
 * \param interface_id Network interface ID.
 * \param bootstrap_mode Selected bootstrap mode:
 *      * NET_IPV6_BOOTSTRAP_STATIC, Application defines the IPv6 prefix.
 * \param ipv6_prefix_pointer Pointer to 64 bit IPv6 prefix. The data is copied, so it can be invalidated after function call.
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 */
extern int8_t arm_nwk_interface_configure_ipv6_bootstrap_set(int8_t interface_id, net_ipv6_mode_e bootstrap_mode, const uint8_t *ipv6_prefix_pointer);

/**
 * \brief Accept Router Advertisements setting.
 *
 * Accept Router Advertisements setting. Setting can be changed after an interface is created.
 * If setting is changed it must be done before the bootstrap is started.
 *
 * \param interface_id The network interface ID.
 * \param accept_ra Router Advertisements handling mode.
 * \return 0 Setting done.
 * \return <0 Failed (for example an invalid interface ID).
 */
extern int8_t arm_nwk_interface_accept_ipv6_ra(int8_t interface_id, net_ipv6_accept_ra_e accept_ra);

/**
 * \brief Set network interface bootstrap setup.
 *
 * \param interface_id Network interface ID.
 * \param bootstrap_mode Selected bootstrap mode:
 *      * NET_6LOWPAN_BORDER_ROUTER, Initialize border router basic setup.
 *      * NET_6LOWPAN_ROUTER, Enable normal 6LoWPAN ND and RPL to bootstrap.
 *      * NET_6LOWPAN_HOST, Enable normal 6LoWPAN ND only to bootstrap.
 *      * NET_6LOWPAN_SLEEPY_HOST, Enable normal 6LoWPAN ND only to bootstrap.
 *      * NET_6LOWPAN_NETWORK_DRIVER, 6LoWPAN radio host device no bootstrap.
 *      * NET_6LOWPAN_SNIFFER, 6LoWPAN sniffer device no bootstrap.
 *
 * \param net_6lowpan_mode_extension Define 6LoWPAN MLE and mode as ZigBeeIP or Thread.
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 * \return -2 Unsupported bootstrap type in this library.
 * \return -3 No memory for 6LoWPAN stack.
 * \return -4 Null pointer parameter.
 */
extern int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension);

/**
 * \brief Set network interface link layer parameters.
 *
 * \param interface_id Network interface ID
 * \param nwk_channel_list Defines network channel page and channel.
 * \param link_setup Link layer parameters for NET_6LOWPAN_NETWORK_DRIVER defines NetworkID, PAN-ID Short Address.
 *
 * \return >=0 Configuration set OK.
 * \return -1 Unknown network ID.
 * \return -2 Interface is active, bootsrap mode not selected or is not NET_6LOWPAN_NETWORK_DRIVER or NET_6LOWPAN_SNIFFER.
 * \return -3 No memory for 6LoWPAN stack.
 * \return -4 Null pointer parameter.
 */
extern int8_t arm_nwk_interface_network_driver_set(int8_t interface_id, const channel_list_s *nwk_channel_list, network_driver_setup_s *link_setup);

/**
 * \brief Set configured network interface global address mode (border router bootstrap mode cannot set this).
 *
 * \param interface_id Network interface ID.
 * \param mode Define 6LoWPAN Global Address register mode:
 *      * NET_6LOWPAN_GP64_ADDRESS, Interface registers only GP64
 *      * NET_6LOWPAN_GP16_ADDRESS, Interface registers only GP16
 *      * NET_6LOWPAN_MULTI_GP_ADDRESS, Interface registers GP16 and GP64 addresses. GP16 is primary address and GP64 is secondary.
 *
 * \param short_address_base Short address base. If the application defines value 0-0xfffd, 6LoWPAN tries to register GP16 address
 * using that address. 0xfffe and 0xffff generate random 16-bit short address.
 *
 * \param define_new_short_address_at_DAD This parameter is only checked when mode is not NET_6LOWPAN_GP64_ADDRESS and
 * short_address_base is 0-0xfffd. Recommended value is 1. It enables automatic new address definition at
 * Duplicate Address Detection (DAD). Value 0 generates a DAD error for the interface bootstrap.
 * Border router device will not check that part.
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 * \return -2 Illegal for border router.
 * \return -3 No memory for 6LoWPAN stack.
 */
extern int8_t arm_nwk_6lowpan_gp_address_mode(int8_t interface_id, net_6lowpan_gp_address_mode_e mode, uint16_t short_address_base, uint8_t define_new_short_address_at_DAD);

/**
 * \brief Set the channel list configuration to be used on the network interface.
 *
 * \param interface_id Network interface ID.
 * \param nwk_channel_list Channel list to be used.
 *
 * \return >=0 Channel configuration OK.
 * \return -1 Unknown network interface ID.
 * \return -2 Empty channel list, no channels enabled.
 * \return -4 If network interface is already active and cannot be re-configured.
 */
extern int8_t arm_nwk_set_channel_list(int8_t interface_id, const channel_list_s *nwk_channel_list);

/**
 * \brief Set the link scan time used on network interface.
 *
 * \param interface_id Network interface ID.
 * \param scan_time Value 0-14, scan duration/channel.
 *
 * \return >=0 Scan configuration OK.
 * \return -1 Unknown network interface ID.
 * \return -4 If network interface is already active and cannot be re-configured.
 * \return -5 Invalid scan time.
 */
extern int8_t arm_nwk_6lowpan_link_scan_parameter_set(int8_t interface_id, uint8_t scan_time);

/**
 * \brief A function to set the PAN ID filter.
 *
 * \param interface_id Network interface ID.
 * \param pan_id_filter Enable filter for specific PAN ID. 0xffff disables the filter.
 *
 * \return 0 Filter set OK.
 * \return -1 Unknown Network interface ID.
 * \return -2 Interface is active.
 *
 */
extern int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(int8_t interface_id, uint16_t pan_id_filter);

/**
  * \brief Get current used channel.
  *
  * \param interface_id Network interface ID.
  *
  * \return Active channel.
  * \return -1 = Radio is closed.
  */
extern int16_t arm_net_get_current_channel(int8_t interface_id);

/**
 * \brief A function to read the PAN ID filter.
 *
 * \param interface_id Network interface ID.
 *
 * \return 16-bit value indicating a PAN ID filter.
 */
extern uint16_t arm_net_get_nwk_pan_id_filter(int8_t interface_id);

/**
  * \brief Enable/Disable network ID filter.
  *
  * \param interface_id Network interface ID.
  * \param nwk_id_filter A pointer to a new network ID filter, NULL disable filter.
  *
  * \return 0 On success.
  * \return -1 Unknown network ID.
  * \return -2 Interface active.
  */
extern int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(int8_t interface_id, const uint8_t *nwk_id_filter);

/**
  * \brief Enable/Disable network protocol ID filter.
  *
  * \param interface_id Network interface ID.
  * \param protocol_ID A value that filters only supported network protocols (0= Zigbee1.x, 2= ZigBeeIP).
  *
  * \return 0 On success.
  * \return -1 Unknown network ID.
  * \return -2 Interface active.
  */
extern int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan(int8_t interface_id, uint8_t protocol_ID);

/**
  * \brief Beacon join priority transmit callback.
  *
  * Callback defines join priority that is transmitted in beacon. Join priority is
  * 8 bit field in beacon that can be set e.g. based on RPL protocol rank data.
  *
  * \param interface_id The network interface ID.
  *
  * \return Join priority to be transmitted in beacon. 0 to 255.
  */
typedef uint8_t beacon_join_priority_tx_cb(int8_t interface_id);

/**
  * \brief Compare received beacon callback.
  *
  * Callback defines how preferred the node that has sent beacon is for connecting
  * to the network.
  *
  * \param interface_id The network interface ID.
  * \param join_priority Join priority that has been received in beacon. 0 to 255.
  * \param link_quality Link quality. 0 to 255. 255 is best quality.
  *
  * \return Connect to preference. 0 to 255. 255 is highest connect to preference.
  */
typedef uint8_t beacon_compare_rx_cb(int8_t interface_id, uint8_t join_priority, uint8_t link_quality);

/**
  * \brief Set callback for beacon join priority transmit
  *
  * Sets callback that defines join priority that is transmitted in beacon.
  * If callback is not set default functionality is used. On default functionality
  * join priority is combined from RPL DAGRank and RPL DODAG preference.
  *
  * \param interface_id The network interface ID.
  * \param beacon_join_priority_tx_cb_ptr Function pointer.
  *
  * \return 0 on success.
  * \return -1 Unknown network ID.
  * \return -2 Other error.
  */
extern int8_t arm_nwk_6lowpan_beacon_join_priority_tx_callback_set(int8_t interface_id, beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr);

/**
  * \brief Set callback for comparing received beacon
  *
  * Sets callback that defines how preferred the node that has sent beacon is for
  * connecting to the network. If callback is not set default functionality is used.
  * On default functionality connecting priority is defined based on join priority
  * received in beacon and link quality.
  *
  * \param interface_id Network interface ID.
  * \param beacon_compare_rx_cb_ptr Function pointer.
  *
  * \return 0 on success.
  * \return -1 Unknown network ID.
  * \return -2 Other error.
  */
extern int8_t arm_nwk_6lowpan_beacon_compare_rx_callback_set(int8_t interface_id, beacon_compare_rx_cb *beacon_compare_rx_cb_ptr);

/**
  * \brief Initialize and configure the interface security mode.
  *
  * \param interface_id Network interface ID.
  * \param mode Defines link layer security mode.
  *  NET_SEC_MODE_NO_LINK_SECURITY, No security.
  *  NET_SEC_MODE_PSK_LINK_SECURITY, Predefined PSK link layer key and ID.
  *  NET_SEC_MODE_PANA_LINK_SECURITY, PANA bootstrap network authentication.
  *
  * \param sec_level Defined security level is checked only when the mode is not NET_SEC_MODE_NO_LINK_SECURITY.
  * \param psk_key_info Pointer for PSK link layer keys. Checked only when the mode is NET_SEC_MODE_PSK_LINK_SECURITY.
  *
  * \return 0 on success.
  */

extern int8_t arm_nwk_link_layer_security_mode(int8_t interface_id, net_6lowpan_link_layer_sec_mode_e mode, uint8_t sec_level, const net_link_layer_psk_security_info_s *psk_key_info);

/**
  * \brief Initialize and configure interface PANA network client.
  *
  * \param interface_id Network interface ID.
  * \param cipher_mode Defines TLS 1.2 Cipher mode PSK, ECC or both.
  * \param psk_key_id PSK KEY id for PSK Setup
  *
  * \return 0 on success.
  * \return -1 Unknown network ID or PANA is not supported at this library.
  * \return -2 Interface active.
  */
extern int8_t arm_pana_client_library_init(int8_t interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id);

/**
  * \brief Initialize and Configure interface PANA network server.
  *
  * \param interface_id Network interface ID.
  * \param cipher_mode Defines TLS 1.2 Cipher mode PSK, ECC or both.
  * \param key_material A pointer to 128-bit key material or NULL when the PANA server generates the random key.
  * \param time_period_before_activate_key Guard period after a succesful key delivery phase before the key will be activated by server.
  *
  * \return 0 On success.
  * \return -1 Unknown network ID.
  * \return -2 Interface active.
  */
extern int8_t arm_pana_server_library_init(int8_t interface_id, net_tls_cipher_e cipher_mode, const uint8_t *key_material, uint32_t time_period_before_activate_key);

/**
  * \brief Manually initiate a PANA client key pull. For test purposes only.
  *
  * \param interface_id Network interface ID.
  *
  * \return 0 On success.
  * \return -1 Unknown network ID.
  */
extern int8_t arm_pana_client_key_pull(int8_t interface_id);

/**
 * \brief Trigger network key update process
 *
 * This function delivers a new network key to all ZigBee routers that have registered a GP address to server.
 * The function call always trig new key-id. Key delivery is started in 300ms interval between nodes.
 * This function does not cause any traffic if the server does not have any router device sessions.
 *
 * \param interface_id Network interface ID.
 * \param network_key_material Pointer to new 128-bit key material, NULL generates a random key.
 *
 * \return 0 Key Update process OK.
 * \return -1 PANA server is not initialized yet.
 * \return -2 Old key update still active.
 * \return -3 Memory allocation fails.
 */
extern int8_t arm_pana_server_key_update(int8_t interface_id, const uint8_t *network_key_material);

/**
 * \brief Activate new key material before standard timeout.
 *
 * This function is only for testing.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0 Key activate process OK.
 * \return -1 No Pending key update.
 * \return -2 PANA server is not initialized or PANA server API is disabled with this library.
 */
extern int8_t arm_pana_activate_new_key(int8_t interface_id);

/**
 * \brief Read PANA server security key material.
 *
 * previous_active_network_key Only valid when current_active_key_index is bigger than 1.
 *
 * \param interface_id Interface
 * \param key Pointer for key material information store.
 *
 * \return 0 Key read OK.
 * \return -1 PANA server key material not available.
 */
extern int8_t arm_network_key_get(int8_t interface_id, ns_keys_t *key);

/**
 * \brief Start network interface bootstrap.
 *
 * \param interface_id Network interface ID.
 *
 *
 * \return >=0 Bootstrap start OK.
 * \return -1 Unknown network ID.
 * \return -2 Not configured.
 * \return -3 Active.
 */
extern int8_t arm_nwk_interface_up(int8_t interface_id);

/**
 * \brief Stop and set interface to idle.
 *
 * \param interface_id Network interface ID
 *
 * \return >=0 Process OK.
 * \return -1 Unknown network ID.
 * \return -3 Not Active.
 */
extern int8_t arm_nwk_interface_down(int8_t interface_id);

/**
 * \brief Define border router MAC and 6LoWPAN ND setup for selected interface.
 *
 * \param interface_id Network interface ID.
 * \param border_router_setup_ptr Pointer to MAC and 6LoWPAN ND setup.
 *
 * \return 0 on success, negative value on error case.
 */
extern int8_t arm_nwk_6lowpan_border_router_init(int8_t interface_id, const border_router_setup_s *border_router_setup_ptr);

/**
 * \brief Add context at 6LoWPAN interface configure state.
 *
 * \param interface_id Network interface ID.
 * \param c_id_flags Bit 4 indicates compress support and bit 0-3 context ID.
 * \param context_len Context length in bits need to be 64-128.
 * \param ttl Context time to live, value in minutes.
 * \param context_ptr Pointer to full 128-bit memory area.
 *
 * \return 0 Context update OK.
 * \return -1 No memory for new context.
 * \return -2 Border router base not allocated.
 * \return -3 Given parameter fails (c_id_flags > 0x1f or contex_len < 64).
 * \
 */
extern int8_t arm_nwk_6lowpan_border_router_context_update(int8_t interface_id, uint8_t c_id_flags, uint8_t context_len, uint16_t ttl, const uint8_t *context_ptr);

/**
 * \brief Update runtime configured context.
 *
 * This function can change the value of context compress state or time to live.
 * It triggers a new ABRO version number, indicating that ND parameters are updated.
 *
 * \param interface_id Network interface ID.
 * \param c_id Context ID stack checks first 4 bits, supported values 0-15.
 * \param compress_mode 0 = Compress disabled, otherwise compress enabled.
 * \param ttl Context time to live value in minutes.
 *
 * \return 0 Update OK.
 * \return -1 Update fail by router state.
 *
 */
extern int8_t arm_nwk_6lowpan_border_router_context_parameter_update(int8_t interface_id, uint8_t c_id, uint8_t compress_mode, uint16_t ttl);

/**
 * \brief Delete allocated context by ID.
 *
 * \param interface_id Network interface ID.
 * \param c_id 4-bit Context ID to be deleted.
 *
 * \return 0 Context delete OK.
 * \return -1 Delete process fails.
 */
extern int8_t arm_nwk_6lowpan_border_router_context_remove_by_id(int8_t interface_id, uint8_t c_id);

/**
 * \brief Update ND ABRO version number.
 *
 * \param interface_id Network interface ID
 *
 * \return 0 ABRO version update OK.
 * \return -1 ABRO update fails (Interface is not up yet or the border router base is not allocated).
 */
extern int8_t arm_nwk_6lowpan_border_router_configure_push(int8_t interface_id);

/**
 * Set timeout for default prefix on cache.
 * Requires arm_nwk_6lowpan_border_router_configure_push() be called to settings be taken into use.
 * \param interface_id mesh interface.
 * \param time seconds
 * \return 0 on success, negative value on failure.
 */
extern int8_t arm_nwk_6lowpan_border_route_nd_default_prefix_timeout_set(int8_t interface_id, uint32_t time);

/**
 * \brief A function to read network layer configurations.
 * \param interface_id Network interface ID.
 * \param network_params A pointer to the structure where the network layer configs are written.
 * \return 0 On success.
 * \return Negative value if interface is not known.
 */
int8_t arm_nwk_param_read(int8_t interface_id, link_layer_setups_s *network_params);

/**
 * \brief A function to read MAC PAN-ID, Short address and EUID64.
 * \param interface_id Network interface ID.
 * \param mac_params A pointer to the structure where the MAC addresses are written.
 * \return 0 On success.
 * \return Negative value if interface is not known.
 */
int8_t arm_nwk_mac_address_read(int8_t interface_id, link_layer_address_s *mac_params);

/**
 * \brief A function to read 6LoWPAN ND border router address and NWK prefix.
 * \param interface_id Network interface ID.
 * \param nd_addr_info Pointer to the structure where the address is written.
 * \return 0 On success.
 * \return Negative value if network interface is not known or if the interface
 *          is not in active or ready state.
 */
int8_t arm_nwk_nd_address_read(int8_t interface_id, network_layer_address_s *nd_addr_info);

/**
 * \brief A function to read the networking address information.
 * \param interface_id Network interface ID.
 * \param addr_id The address information type to be read.
 * \param address A pointer to a structure where the address information is written.
 * \return 0 On success, -1 on failure.
 */
extern int8_t arm_net_address_get(int8_t interface_id, net_address_t addr_id, uint8_t *address);

/**
 * \brief A function to read networking addresses one by one.
 * \param interface_id Network interface ID.
 * \param n A pointer that is incremented every call. Start looping with n=0.
 * \param address_buffer A pointer to buffer where address is copied.
 * \return 0 On success.
 * \return -1 No more addresses available.
 */
extern int8_t arm_net_address_list_get_next(int8_t interface_id, int *n, uint8_t address_buffer[16]);

/**
 * \brief A function to read network interface address count.
 * \param interface_id Network interface ID.
 * \param address_count A pointer to the structure where the address count is saved.
 *
 * \return 0 On success, -1 on errors.
 */
extern int8_t arm_net_interface_address_list_size(int8_t interface_id, uint16_t *address_count);

/**
 * \brief A function to set interface metric.
 * \param interface_id Network interface ID.
 * \param metric Used to rank otherwise-equivalent routes. Lower is preferred and default is 0. The metric value is added to metric provided by the arm_net_route_add() function.
 *
 * \return 0 On success, -1 on errors.
 */
extern int8_t arm_net_interface_set_metric(int8_t interface_id, uint16_t metric);

/**
 * \brief A function to read the interface metric value on an interface.
 * \param interface_id Network interface ID.
 * \param metric A pointer to the variable where the interface metric value is saved.
 *
 * \return 0 On success, -1 on errors.
 */
extern int8_t arm_net_interface_get_metric(int8_t interface_id, uint16_t *metric);

/**
 * \brief A function to read the network interface.
 * \param interface_id Network interface ID.
 * \param address_buf_size Buffer size in bytes, minimum 16 bytes.
 * \param address_buffer A pointer to a structure where the addresses are saved one by one.
 * \param writed_address_count A pointer to the structure where the number of addresses saved is written.
 *
 * \return 0 on success, -1 on errors.
 */
extern int8_t arm_net_address_list_get(int8_t interface_id, uint8_t address_buf_size, uint8_t *address_buffer, int *writed_address_count);

/**
 * \brief A function to add an address to an interface.
 * \param interface_id Network interface ID.
 * \param address The address to be added to the interface.
 * \param prefix_len The length of the address prefix.
 * \param valid_lifetime The time in seconds until the address becomes invalid and is removed from the interface. Value 0xffffffff represents infinity.
 * \param preferred_lifetime The time in seconds until the address becomes deprecated. Value 0xffffffff represents infinity. The preferred lifetime should not be longer than a valid lifetime.
 * \return 0 on success, -1 on errors.
 */
extern int8_t arm_net_address_add_to_interface(int8_t interface_id, const uint8_t address[16], uint8_t prefix_len, uint32_t valid_lifetime, uint32_t preferred_lifetime);

/**
 * \brief A function to remove an address from an interface.
 * \param interface_id Network interface ID.
 * \param address The address to be removed from the interface.
 *
 * \return 0 on success, -1 on errors.
 */
extern int8_t arm_net_address_delete_from_interface(int8_t interface_id, const uint8_t address[16]);

/**
 * \brief A function to add a route to the routing table.
 * \param prefix Destination prefix for the route to be added.
 * \param prefix_len The length of the prefix.
 * \param next_hop Link-local address of the next hop (e.g. router); if NULL the route is marked as on-link.
 * \param lifetime The time in seconds until the route is removed from the routing table. Value 0xffffffff means infinite.
 * \param metric Used to rank otherwise-equivalent routes. Lower is preferred. Normally 128.
 * \param interface_id Network interface ID.
 * \return 0 on success, -1 on add failure, -2 on invalid function parameters.
 */
extern int8_t arm_net_route_add(const uint8_t *prefix, uint8_t prefix_len, const uint8_t *next_hop, uint32_t lifetime, uint8_t metric, int8_t interface_id);

/**
 * \brief A function to remove a route from the routing table.
 * \param prefix The prefix to be removed.
 * \param prefix_len The length of the prefix.
 * \param next_hop Link-local address of the next hop.
 * \param interface_id Network interface ID.
 * \return 0 on success, -1 on delete failure, -2 on invalid function parameters.
 */
extern int8_t arm_net_route_delete(const uint8_t *prefix, uint8_t prefix_len, const uint8_t *next_hop, int8_t interface_id);

/** Border Router ND NVM update types. */
/** ND context update, 20 bytes data behind pointer. */
#define ND_PROXY_CONTEXT_NVM_UPDATE         0
/** ND context update flags update. */
#define ND_PROXY_CONTEXT_FLAGS_NVM_UPDATE   1
/** ND context remove. */
#define ND_PROXY_CONTEXT_NVM_REMOVE         2
/** ND prefix update. */
#define ND_PROXY_PREFIX_NVM_UPDATE          3
/** ND ABRO version update. */
#define ND_PROXY_ABRO_VERSION_NVM_UPDATE    4

/**
 * \brief Load context from NVM at ZigBeeIP interface configure state.
 *
 * \param interface_id Network Interface ID
 * \param contex_data A pointer to properly built 20 bytes update array.
 *
 * \return 0 Context reload OK.
 * \return <0 Load fail.
 */
extern int8_t arm_nwk_6lowpan_border_router_nd_context_load(int8_t interface_id, uint8_t *contex_data);  //NVM

/**
 * Set certificate chain for PANA
 * \param chain_info Certificate chain.
 * \return 0 on success, negative on failure.
 */
extern int8_t arm_network_certificate_chain_set(const arm_certificate_chain_entry_s *chain_info);

/**
 * \brief Add PSK key to TLS library.
 *
 * \param key_ptr A pointer to 16 bytes long key array.
 * \param key_id PSK key ID.
 *
 * \return 0 = success
 * \return -1 = failure
 */
extern int8_t arm_tls_add_psk_key(const uint8_t *key_ptr, uint16_t key_id);

/**
 * \brief Remove PSK key from TLS library.
 *
 * \param key_id PSK key ID.
 *
 * \return 0 = success
 * \return -1 = failure
 */
extern int8_t arm_tls_remove_psk_key(uint16_t key_id);

/**
 * \brief Check if PSK key ID exists.
 *
 * \param key_id PSK key ID
 *
 * \return 0 = success
 * \return -1 = failure
 */
extern int8_t arm_tls_check_key(uint16_t key_id);

/**
 * \brief Print routing table
 *
 * Prints the routing table to the command line
 */
void arm_print_routing_table(void);

/**
 * \brief Print routing table
 *
 * Outputs the routing table using the given printf style function
 *
 * \param print_fn pointer to a printf style output function
 */
void arm_print_routing_table2(void (*print_fn)(const char *fmt, ...));

/**
 * \brief Flush neighbor cache
 *
 * Flushes the neighbor cache
 */
void arm_ncache_flush(void);

/**
 * \brief Print neighbor cache
 *
 * Prints neighbor cache to the command line
 */
void arm_print_neigh_cache(void);

/**
 * \brief Print neighbor cache
 *
 * Outputs the neighbor cache using the given printf style function
 *
 * \param print_fn pointer to a printf style output function
 */
void arm_print_neigh_cache2(void (*print_fn)(const char *fmt, ...));

/**
 * \brief Print PCB list
 *
 * Prints Protocol Control Block list to the command line
 */
void arm_print_protocols(void);

/**
 * \brief Print PCB list
 *
 * Prints Protocol Control Block list using the given printf style function
 *
 * \param print_fn pointer to a printf style output function
 * \param sep column separator character
 */
void arm_print_protocols2(void (*print_fn)(const char *fmt, ...), char sep);

/**
  * \brief Get the library version information.
  *
  * \param *ptr Pointer to data location. Required size is 20 bytes.
  *
  *
  * The array containing the version information has the following structure.
  *
  *  | Platform type | Version | Build ID |
  *  | :-----------: | :----------------: |
  *  | 1 byte        |  1 byte | 4 bytes  |
  *
  */
extern void net_get_version_information(uint8_t *ptr);
#ifdef __cplusplus
}
#endif
#endif /* NET_INTERFACE_H_ */
