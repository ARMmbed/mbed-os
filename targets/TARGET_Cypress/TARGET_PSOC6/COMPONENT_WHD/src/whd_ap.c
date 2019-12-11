/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 *  Provides generic APSTA functionality that chip specific files use
 */
#include "whd_debug.h"
#include "whd_ap.h"
#include "bus_protocols/whd_chip_reg.h"
#include "whd_chip_constants.h"
#include "whd_chip.h"
#include "whd_sdpcm.h"
#include "whd_thread_internal.h"
#include "whd_events_int.h"
#include "whd_int.h"
#include "whd_utils.h"
#include "whd_wifi_api.h"
#include "whd_buffer_api.h"
#include "whd_wlioctl.h"

/******************************************************
** @cond               Constants
*******************************************************/

#define WLC_EVENT_MSG_LINK      (0x01)
#define RATE_SETTING_11_MBPS    (11000000 / 500000)
#define AMPDU_AP_DEFAULT_BA_WSIZE   8
#define AMPDU_STA_DEFAULT_BA_WSIZE  8
#define AMPDU_STA_DEFAULT_MPDU      4   /* STA default num MPDU per AMPDU */
#define WEP40_KEY_LENGTH                     5
#define WEP104_KEY_LENGTH                    13
#define FORMATTED_ASCII_WEP40_KEY_LENGTH     28 /* For 5  bytes key */
#define FORMATTED_ASCII_WEP104_KEY_LENGTH    60 /* For 13 bytes key */

/******************************************************
**                   Enumerations
*******************************************************/
typedef enum
{
    BSS_AP   = 3,
    BSS_STA  = 2,
    BSS_UP   = 1,
    BSS_DOWN = 0
} bss_arg_option_t;

typedef enum
{
    WEP_OPEN_SYSTEM_AUTHENTICATION  = 0,
    WEP_SHARED_KEY_AUTHENTICATION   = 1
} wep_authentication_type_t;

/******************************************************
 *  *               Function Declarations
 *   ******************************************************/
static void *whd_handle_apsta_event(whd_interface_t ifp, const whd_event_header_t *event_header,
                                    const uint8_t *event_data, void *handler_user_data);

/******************************************************
 *        Variables Definitions
 *****************************************************/
static const whd_event_num_t apsta_events[] = { WLC_E_IF, WLC_E_LINK, WLC_E_NONE };
/******************************************************
*               Function Definitions
******************************************************/

void whd_ap_info_init(whd_driver_t whd_driver)
{
    whd_driver->ap_info.ap_is_up = WHD_FALSE;
    whd_driver->ap_info.is_waiting_event = WHD_FALSE;
}

void whd_wifi_set_ap_is_up(whd_driver_t whd_driver, whd_bool_t new_state)
{
    if (whd_driver->ap_info.ap_is_up != new_state)
    {
        whd_driver->ap_info.ap_is_up = new_state;
    }
}

whd_bool_t whd_wifi_get_ap_is_up(whd_driver_t whd_driver)
{
    return whd_driver->ap_info.ap_is_up;
}

whd_result_t whd_wifi_set_block_ack_window_size_common(whd_interface_t ifp, uint16_t ap_win_size, uint16_t sta_win_size)
{
    whd_result_t retval;
    uint16_t block_ack_window_size = ap_win_size;

    /* If the AP interface is already up then don't change the Block Ack window size */
    if (ifp->role == WHD_AP_ROLE)
    {
        return WHD_SUCCESS;
    }

    if (ifp->role == WHD_STA_ROLE)
    {
        block_ack_window_size = sta_win_size;
    }

    retval = whd_wifi_set_iovar_value(ifp, IOVAR_STR_AMPDU_BA_WINDOW_SIZE, ( uint32_t )block_ack_window_size);

    whd_assert("set_block_ack_window_size: Failed to set block ack window size\r\n", retval == WHD_SUCCESS);

    return retval;
}

whd_result_t whd_wifi_set_ampdu_parameters_common(whd_interface_t ifp, uint8_t ba_window_size, int8_t ampdu_mpdu,
                                                  uint8_t rx_factor)
{
    CHECK_RETURN(whd_wifi_set_iovar_value(ifp, IOVAR_STR_AMPDU_BA_WINDOW_SIZE, ba_window_size) );

    /* Set number of MPDUs available for AMPDU */
    CHECK_RETURN(whd_wifi_set_iovar_value(ifp, IOVAR_STR_AMPDU_MPDU, ( uint32_t )ampdu_mpdu) );

    if (rx_factor != AMPDU_RX_FACTOR_INVALID)
    {
        CHECK_RETURN(whd_wifi_set_iovar_value(ifp, IOVAR_STR_AMPDU_RX_FACTOR, rx_factor) );
    }
    return WHD_SUCCESS;
}

/** Sets the chip specific AMPDU parameters for AP and STA
 *  For SDK 3.0, and beyond, each chip will need it's own function for setting AMPDU parameters.
 */
whd_result_t whd_wifi_set_ampdu_parameters(whd_interface_t ifp)
{
    whd_driver_t whd_driver = ifp->whd_driver;
    /* Get the chip number */
    uint16_t wlan_chip_id = whd_chip_get_chip_id(whd_driver);

    if ( (wlan_chip_id == 43012) || (wlan_chip_id == 43455) )
    {
        return whd_wifi_set_ampdu_parameters_common(ifp, AMPDU_STA_DEFAULT_BA_WSIZE, AMPDU_MPDU_AUTO,
                                                    AMPDU_RX_FACTOR_64K);
    }
    else if (wlan_chip_id == 43909)
    {
        return whd_wifi_set_ampdu_parameters_common(ifp, AMPDU_STA_DEFAULT_BA_WSIZE, AMPDU_MPDU_AUTO,
                                                    AMPDU_RX_FACTOR_INVALID);
    }
    else
    {
        return whd_wifi_set_ampdu_parameters_common(ifp, AMPDU_STA_DEFAULT_BA_WSIZE, AMPDU_STA_DEFAULT_MPDU,
                                                    AMPDU_RX_FACTOR_8K);
    }
}

/* All chips */
static void *whd_handle_apsta_event(whd_interface_t ifp, const whd_event_header_t *event_header,
                                    const uint8_t *event_data, void *handler_user_data)
{
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_ap_int_info_t *ap;

    UNUSED_PARAMETER(event_header);
    UNUSED_PARAMETER(event_data);
    UNUSED_PARAMETER(handler_user_data);

    ap = &whd_driver->ap_info;

    if (ap->is_waiting_event == WHD_TRUE)
    {
        if ( (event_header->event_type == (whd_event_num_t)WLC_E_LINK) ||
             (event_header->event_type == WLC_E_IF) )
        {
            whd_result_t result;
            result = cy_rtos_set_semaphore(&ap->whd_wifi_sleep_flag, WHD_FALSE);
            WPRINT_WHD_DEBUG( ("%s failed to post AP link semaphore at %d\n", __func__, __LINE__) );
            REFERENCE_DEBUG_ONLY_VARIABLE(result);
        }
    }
    return handler_user_data;
}

/* All chips */
uint32_t whd_wifi_init_ap(whd_interface_t ifp, whd_ssid_t *ssid, whd_security_t auth_type,
                          const uint8_t *security_key, uint8_t key_length, uint8_t channel)
{
    whd_driver_t whd_driver;
    whd_bool_t wait_for_interface = WHD_FALSE;
    whd_result_t result;
    whd_buffer_t response;
    whd_buffer_t buffer;
    whd_interface_t prim_ifp;
    whd_ap_int_info_t *ap;
    uint32_t *data;
    uint32_t bss_index;
    uint16_t wlan_chip_id;
    uint16_t event_entry = (uint16_t)0xFF;


    CHECK_IFP_NULL(ifp);

    whd_driver = ifp->whd_driver;

    CHECK_DRIVER_NULL(whd_driver);

    ap = &whd_driver->ap_info;

    prim_ifp = whd_get_primary_interface(whd_driver);
    if (prim_ifp == NULL)
    {
        WPRINT_WHD_ERROR( ("%s failed at %d \n", __func__, __LINE__) );
        return WHD_UNKNOWN_INTERFACE;
    }

    /* Turn off APSTA when creating AP mode on primary interface */
    if (ifp == prim_ifp)
    {
        CHECK_RETURN(whd_wifi_set_ioctl_buffer(prim_ifp, WLC_DOWN, NULL, 0) );
        data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)4, IOVAR_STR_APSTA);
        CHECK_IOCTL_BUFFER(data);
        *data = 0;
        result = whd_cdc_send_iovar(ifp, CDC_SET, buffer, 0);
        if ( (result != WHD_SUCCESS) && (result != WHD_WLAN_UNSUPPORTED) )
        {
            WPRINT_WHD_ERROR( ("Could not turn off apsta\n") );
            return result;
        }
        CHECK_RETURN(whd_wifi_set_ioctl_buffer(prim_ifp, WLC_UP, NULL, 0) );
    }

    bss_index = ifp->bsscfgidx;
    /* Get the Chip Number */
    wlan_chip_id = whd_chip_get_chip_id(whd_driver);

    /* Configuration need to come from some structure whd_driver */
#if 0
    uint32_t *auth;
    uint16_t length;
#endif

    ifp->role = WHD_AP_ROLE;

    if (wlan_chip_id == 4334)
    {
        if (auth_type == WHD_SECURITY_WEP_PSK)
        {
            WPRINT_WHD_ERROR( ("WEP auth type is not allowed , %s failed at line %d \n", __func__, __LINE__) );
            return WHD_WEP_NOT_ALLOWED;
        }
    }
    if ( ( (auth_type == WHD_SECURITY_WPA_TKIP_PSK) || (auth_type == WHD_SECURITY_WPA2_AES_PSK) ||
           (auth_type == WHD_SECURITY_WPA2_MIXED_PSK) ) &&
         ( (key_length < (uint8_t)8) || (key_length > (uint8_t)64) ) )
    {
        WPRINT_WHD_INFO( ("Error: WPA security key length must be between 8 and 64\n") );
        return WHD_WPA_KEYLEN_BAD;
    }

    /* Need to decide on the wep configuration */
#if 0
    else if ( ( (auth_type == WHD_SECURITY_WEP_PSK) || (auth_type == WHD_SECURITY_WEP_SHARED) ) &&
              ( (key_length != FORMATTED_ASCII_WEP40_KEY_LENGTH) &&
                (key_length != FORMATTED_ASCII_WEP104_KEY_LENGTH) ) )
    {
        WPRINT_WHD_INFO( ("Error: WEP security Key length must be either 5 / 13 bytes\n") );
        return WHD_WEP_KEYLEN_BAD;
    }
#endif
    if ( (whd_wifi_get_ap_is_up(whd_driver) == WHD_TRUE) )
    {
        WPRINT_WHD_INFO( ("Error: Soft AP or Wi-Fi Direct group owner already up\n") );
        return WHD_AP_ALREADY_UP;
    }

/*    if (wlan_chip_id == 4334 || wlan_chip_id == 43340 || wlan_chip_id == 43342 )
    {
        if ( whd_wifi_set_block_ack_window_size( ifp ) != WHD_SUCCESS )
        {
            return WHD_SET_BLOCK_ACK_WINDOW_FAIL;
        }
    }*/

    /* Query bss state (does it exist? if so is it UP?) */
    data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)4, IOVAR_STR_BSS);
    CHECK_IOCTL_BUFFER(data);

    if ( (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *data = htod32( (uint32_t)CHIP_AP_INTERFACE );
    }
    else
    {
        *data = htod32( (uint32_t)bss_index );
    }

    if ( (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        if (whd_cdc_send_iovar(ifp, CDC_GET, buffer, &response) != WHD_SUCCESS)
        {
            /* Note: We don't need to release the response packet since the iovar failed */
            wait_for_interface = WHD_TRUE;
        }
        else
        {
            /* Check if the BSS is already UP, if so return */
            uint32_t *data2 = (uint32_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, response);
            *data2 = dtoh32 (*data2);
            if (*data2 == (uint32_t)BSS_UP)
            {
                CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
                whd_wifi_set_ap_is_up(whd_driver, WHD_TRUE);
                ap->is_waiting_event = WHD_FALSE;
                return WHD_SUCCESS;
            }
            else
            {
                CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
            }
        }
    }

    if (whd_cdc_send_iovar(prim_ifp, CDC_GET, buffer, &response) != WHD_SUCCESS)
    {
        /* Note: We don't need to release the response packet since the iovar failed */
        wait_for_interface = WHD_TRUE;
    }
    else
    {
        /* Check if the BSS is already UP, if so return */
        uint32_t *data2 = (uint32_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, response);
        *data2 = dtoh32 (*data2);
        if (*data2 == (uint32_t)BSS_UP)
        {
            CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
            whd_wifi_set_ap_is_up(whd_driver, WHD_TRUE);
            ap->is_waiting_event = WHD_FALSE;
            return WHD_SUCCESS;
        }
        else
        {
            CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
        }
    }

    CHECK_RETURN(cy_rtos_init_semaphore(&ap->whd_wifi_sleep_flag, 1, 0) );

    ap->is_waiting_event = WHD_TRUE;
    /* Register for interested events */
    CHECK_RETURN_WITH_SEMAPHORE(whd_management_set_event_handler(ifp, apsta_events, whd_handle_apsta_event,
                                                                 NULL, &event_entry), &ap->whd_wifi_sleep_flag);
    if (event_entry >= WHD_EVENT_HANDLER_LIST_SIZE)
    {
        WPRINT_WHD_DEBUG( ("Event handler registration failed for AP events in function %s and line %d\n",
                           __func__, __LINE__) );
        return WHD_UNFINISHED;
    }
    ifp->event_reg_list[WHD_AP_EVENT_ENTRY] = event_entry;

    if (wait_for_interface == WHD_TRUE)
    {
        CHECK_RETURN_WITH_SEMAPHORE(cy_rtos_get_semaphore(&ap->whd_wifi_sleep_flag, (uint32_t)10000,
                                                          WHD_FALSE), &ap->whd_wifi_sleep_flag);
    }
    ap->is_waiting_event = WHD_FALSE;

    if (prim_ifp == ifp)
    {
        /* Set AP mode */
        data = (uint32_t *)whd_cdc_get_ioctl_buffer(whd_driver, &buffer, (uint16_t)4);
        CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);
        *data = 1; /* Turn on AP */
        CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_AP, buffer, 0),
                                    &ap->whd_wifi_sleep_flag);
    }
#if 0
    if (whd_wifi_set_block_ack_window_size(ifp) != WHD_SUCCESS)
    {
        return WHD_SET_BLOCK_ACK_WINDOW_FAIL;
    }
#endif

    if (NULL_MAC(ifp->mac_addr.octet) )
    {
        /* Change the AP MAC address to be different from STA MAC */
        if ( (result = whd_wifi_get_mac_address(prim_ifp, &ifp->mac_addr) ) != WHD_SUCCESS )
        {
            WPRINT_WHD_INFO ( (" Get STA MAC address failed result=%" PRIu32 "\n", result) );
            return result;
        }
        else
        {
            WPRINT_WHD_INFO ( (" Get STA MAC address success\n") );
        }
    }

    if ( (result = whd_wifi_set_mac_address(ifp, ifp->mac_addr) ) != WHD_SUCCESS )
    {
        WPRINT_WHD_INFO ( (" Set AP MAC address failed result=%" PRIu32 "\n", result) );
        return result;
    }

    /* Set the SSID */
    data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)40, "bsscfg:" IOVAR_STR_SSID);
    CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);
    if (wlan_chip_id == 4334)
    {
        data[0] = htod32( (uint32_t)CHIP_AP_INTERFACE );  /* Set the bsscfg index */
    }
    else
    {
        data[0] = htod32(bss_index); /* Set the bsscfg index */
    }
    data[1] = htod32(ssid->length); /* Set the ssid length */
    memcpy(&data[2], (uint8_t *)ssid->value, ssid->length);
    if ( (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(ifp, CDC_SET, buffer, 0), &ap->whd_wifi_sleep_flag);
    }
    else
    {
        CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0), &ap->whd_wifi_sleep_flag);
    }

    /* Set the channel */
    data = (uint32_t *)whd_cdc_get_ioctl_buffer(whd_driver, &buffer, (uint16_t)4);
    CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);
    *data = htod32(channel);
    CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_CHANNEL, buffer, 0),
                                &ap->whd_wifi_sleep_flag);

    /* Need to decide on the wep configuration */
#if 0
    if ( (auth_type == WHD_SECURITY_WEP_PSK) || (auth_type == WHD_SECURITY_WEP_SHARED) )
    {
        for (length = 0; length < key_length; length = (uint16_t)(length + 2 + security_key[1]) )
        {
            const whd_wep_key_t *in_key = (const whd_wep_key_t *)&security_key[length];
            wl_wsec_key_t *out_key = (wl_wsec_key_t *)whd_cdc_get_ioctl_buffer(&buffer, sizeof(wl_wsec_key_t) );
            CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(out_key, &ap->whd_wifi_sleep_flag);
            memset(out_key, 0, sizeof(wl_wsec_key_t) );
            out_key->index = in_key->index;
            out_key->len = in_key->length;
            memcpy(out_key->data, in_key->data, in_key->length);
            switch (in_key->length)
            {
                case WEP40_KEY_LENGTH:
                    out_key->algo = (uint32_t)CRYPTO_ALGO_WEP1;
                    break;
                case WEP104_KEY_LENGTH:
                    out_key->algo = (uint32_t)CRYPTO_ALGO_WEP128;
                    break;
                default:
                    whd_buffer_release(whd_driver, buffer, WHD_NETWORK_TX);
                    return WHD_INVALID_KEY;
            }
            /* Set the first entry as primary key by default */

            if (length == 0)
            {
                out_key->flags |= WL_PRIMARY_KEY;
            }
            out_key->index = htod32(out_key->index);
            out_key->len = htod32(out_key->len);
            out_key->algo = htod32(out_key->algo);
            out_key->flags = htod32(out_key->flags);
            CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_KEY, buffer,
                                                           NULL), &ap->whd_wifi_sleep_flag);
        }

        /* Set authentication type */
        auth = (uint32_t *)whd_cdc_get_ioctl_buffer(&buffer, (uint16_t)4);
        CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(auth, &ap->whd_wifi_sleep_flag);
        if (auth_type == WHD_SECURITY_WEP_SHARED)
        {
            *auth = WEP_SHARED_KEY_AUTHENTICATION; /* 1 = Shared Key authentication */
        }
        else
        {
            *auth = WEP_OPEN_SYSTEM_AUTHENTICATION; /*  0 = Open System authentication */
        }
        CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_AUTH, buffer,
                                                       0), &ap->whd_wifi_sleep_flag);
    }
#endif

    data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)8, "bsscfg:" IOVAR_STR_WSEC);
    CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);
    if ( (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        data[0] = htod32( (uint32_t)CHIP_AP_INTERFACE );
    }
    else
    {
        data[0] = htod32(bss_index);
    }
    if ( (auth_type & WPS_ENABLED) != 0 )
    {
        data[1] = htod32( (uint32_t)( (auth_type & (~WPS_ENABLED) ) | SES_OW_ENABLED ) );
    }
    else
    {
        data[1] = htod32( (uint32_t)auth_type );
    }
    CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0), &ap->whd_wifi_sleep_flag);
    if (wlan_chip_id == 4334)
    {
        if (auth_type != WHD_SECURITY_OPEN)
        {
            wsec_pmk_t *psk;

            /* Set the wpa auth */
            data =
                (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)8, "bsscfg:" IOVAR_STR_WPA_AUTH);
            CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);
            data[0] = htod32( (uint32_t)CHIP_AP_INTERFACE );
            data[1] = htod32( (uint32_t)(auth_type == WHD_SECURITY_WPA_TKIP_PSK) ?
                              (WPA_AUTH_PSK) : (WPA2_AUTH_PSK | WPA_AUTH_PSK) );
            CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0), &ap->whd_wifi_sleep_flag);

            /* Set the passphrase */
            psk = (wsec_pmk_t *)whd_cdc_get_ioctl_buffer(whd_driver, &buffer, sizeof(wsec_pmk_t) );
            CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(psk, &ap->whd_wifi_sleep_flag);
            memcpy(psk->key, security_key, key_length);
            psk->key_len = htod16(key_length);
            psk->flags = htod16( (uint16_t)WSEC_PASSPHRASE );
            CHECK_RETURN(cy_rtos_delay_milliseconds(1) );
            /* Delay required to allow radio firmware to be ready to receive PMK and avoid intermittent failure */
            CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_WSEC_PMK, buffer, 0),
                                        &ap->whd_wifi_sleep_flag);
        }
    }
    else
    {
        if ( (auth_type != WHD_SECURITY_OPEN) && (auth_type != WHD_SECURITY_WEP_PSK) &&
             (auth_type != WHD_SECURITY_WEP_SHARED) )
        {
            wsec_pmk_t *psk;

            /* Set the wpa auth */
            data =
                (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)8, "bsscfg:" IOVAR_STR_WPA_AUTH);
            CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);
            if ( (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
            {
                data[0] = htod32( (uint32_t)CHIP_AP_INTERFACE );
            }
            else
            {
                data[0] = htod32(bss_index);
            }
            data[1] =
                htod32( (uint32_t)(auth_type ==
                                   WHD_SECURITY_WPA_TKIP_PSK) ? (WPA_AUTH_PSK) : (WPA2_AUTH_PSK | WPA_AUTH_PSK) );
            if ( (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
            {
                CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(ifp, CDC_SET, buffer, 0), &ap->whd_wifi_sleep_flag);
            }
            else
            {
                CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0),
                                            &ap->whd_wifi_sleep_flag);
            }

            /* Set the passphrase */
            psk = (wsec_pmk_t *)whd_cdc_get_ioctl_buffer(whd_driver, &buffer, sizeof(wsec_pmk_t) );
            CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(psk, &ap->whd_wifi_sleep_flag);
            memcpy(psk->key, security_key, key_length);
            psk->key_len = htod16(key_length);
            psk->flags = htod16( (uint16_t)WSEC_PASSPHRASE );
            CHECK_RETURN(cy_rtos_delay_milliseconds(1) );
            /* Delay required to allow radio firmware to be ready to receive PMK and avoid intermittent failure */
            CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_WSEC_PMK, buffer,
                                                           0), &ap->whd_wifi_sleep_flag);
        }
    }

    /* Set the multicast transmission rate to 11 Mbps rather than the default 1 Mbps */
    data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)4, IOVAR_STR_2G_MULTICAST_RATE);
    CHECK_IOCTL_BUFFER(data);
    *data = htod32( (uint32_t)RATE_SETTING_11_MBPS );
    if ( (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        result = whd_cdc_send_iovar(ifp, CDC_SET, buffer, NULL);
        whd_assert("start_ap: Failed to set multicast transmission rate\r\n", result == WHD_SUCCESS);
    }
    else
    {
        CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(ifp, CDC_SET, buffer, NULL), &ap->whd_wifi_sleep_flag);
    }

    return WHD_SUCCESS;
}

uint32_t whd_wifi_start_ap(whd_interface_t ifp)
{
    whd_buffer_t buffer;
    uint32_t *data;
    uint16_t wlan_chip_id;
    whd_ap_int_info_t *ap;
    whd_interface_t prim_ifp;
    whd_driver_t whd_driver;

    CHECK_IFP_NULL(ifp);

    whd_driver = ifp->whd_driver;

    CHECK_DRIVER_NULL(whd_driver);

    prim_ifp = whd_get_primary_interface(whd_driver);

    if (prim_ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    ap = &whd_driver->ap_info;
    /* Get the Chip Number */
    wlan_chip_id = whd_chip_get_chip_id(whd_driver);

    ap->is_waiting_event = WHD_TRUE;
    data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)8, IOVAR_STR_BSS);
    CHECK_IOCTL_BUFFER_WITH_SEMAPHORE(data, &ap->whd_wifi_sleep_flag);

    if ( (wlan_chip_id == 43012) || (wlan_chip_id == 43362) || (wlan_chip_id == 43455) || (wlan_chip_id == 43430) )
    {
        data[0] = htod32(ifp->bsscfgidx);
    }
    else
    {
        data[0] = htod32( (uint32_t)CHIP_AP_INTERFACE );
    }

    data[1] = htod32( (uint32_t)BSS_UP );
    CHECK_RETURN_WITH_SEMAPHORE(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0), &ap->whd_wifi_sleep_flag);

    /* Wait until AP is brought up */
    CHECK_RETURN_WITH_SEMAPHORE(cy_rtos_get_semaphore(&ap->whd_wifi_sleep_flag, (uint32_t)10000,
                                                      WHD_FALSE), &ap->whd_wifi_sleep_flag);
    ap->is_waiting_event = WHD_FALSE;

    whd_wifi_set_ap_is_up(whd_driver, WHD_TRUE);
    return WHD_SUCCESS;
}

uint32_t whd_wifi_stop_ap(whd_interface_t ifp)
{
    uint32_t *data;
    whd_buffer_t buffer;
    whd_buffer_t response;
    whd_result_t result;
    whd_result_t result2;
    whd_interface_t prim_ifp;
    whd_driver_t whd_driver;
    whd_ap_int_info_t *ap;

    CHECK_IFP_NULL(ifp);

    whd_driver = ifp->whd_driver;

    CHECK_DRIVER_NULL(whd_driver);

    ap = &whd_driver->ap_info;

    prim_ifp = whd_get_primary_interface(whd_driver);

    if (prim_ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    /* Get Chip Number */
    uint16_t wlan_chip_id = whd_chip_get_chip_id(whd_driver);
    if ( (wlan_chip_id == 43012) || (wlan_chip_id == 43362) || (wlan_chip_id == 43455) || (wlan_chip_id == 43430) )
    {
        /* Query bss state (does it exist? if so is it UP?) */
        data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)4, IOVAR_STR_BSS);
        CHECK_IOCTL_BUFFER(data);
        *data = ifp->bsscfgidx;
    }
    else
    {
        /* Query bss state (does it exist? if so is it UP?) */
        data = whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)4, IOVAR_STR_BSS);
        CHECK_IOCTL_BUFFER(data);
        *data = htod32( (uint32_t)CHIP_AP_INTERFACE );
    }
    result = whd_cdc_send_iovar(prim_ifp, CDC_GET, buffer, &response);
    if (result == WHD_WLAN_NOTFOUND)
    {
        /* AP interface does not exist - i.e. it is down */
        whd_wifi_set_ap_is_up(whd_driver, WHD_FALSE);
        return WHD_SUCCESS;
    }

    CHECK_RETURN(result);

    *data = dtoh32(*(uint32_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, response) );
    if (data[0] != (uint32_t)BSS_UP)
    {
        /* AP interface indicates it is not up - i.e. it is down */
        CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
        whd_wifi_set_ap_is_up(whd_driver, WHD_FALSE);
        return WHD_SUCCESS;
    }

    CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );

    ap->is_waiting_event = WHD_TRUE;
    /* set BSS down */
    if ( (wlan_chip_id == 43012) || (wlan_chip_id == 43362) || (wlan_chip_id == 43455) || (wlan_chip_id == 43430) )
    {
        data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)8, IOVAR_STR_BSS);
        CHECK_IOCTL_BUFFER(data);
        data[0] = htod32(ifp->bsscfgidx);
        data[1] = htod32( (uint32_t)BSS_DOWN );
        CHECK_RETURN(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0) );
    }
    else
    {
        data = whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)8, IOVAR_STR_BSS);
        CHECK_IOCTL_BUFFER(data);
        data[0] = htod32( (uint32_t)CHIP_AP_INTERFACE );
        data[1] = htod32( (uint32_t)BSS_DOWN );
        CHECK_RETURN(whd_cdc_send_iovar(ifp, CDC_SET, buffer, 0) );
    }

    /* Wait until AP is brought down */
    result = cy_rtos_get_semaphore(&ap->whd_wifi_sleep_flag, (uint32_t)10000, WHD_FALSE);
    ap->is_waiting_event = WHD_FALSE;
    result2 = cy_rtos_deinit_semaphore(&ap->whd_wifi_sleep_flag);
    if (result != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error getting a semaphore, %s failed at %d \n", __func__, __LINE__) );
        return result;
    }
    if (result2 != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error deleting semaphore, %s failed at %d \n", __func__, __LINE__) );
        return result2;
    }

    /* Disable AP mode only if AP is on primary interface */
    if (prim_ifp == ifp)
    {
        data = (uint32_t *)whd_cdc_get_ioctl_buffer(whd_driver, &buffer, (uint16_t)4);
        CHECK_IOCTL_BUFFER(data);
        *data = 0;
        CHECK_RETURN(whd_cdc_send_ioctl(ifp, CDC_SET, WLC_SET_AP, buffer, 0) );
    }

    CHECK_RETURN(whd_wifi_deregister_event_handler(ifp, ifp->event_reg_list[WHD_AP_EVENT_ENTRY]) );
    ifp->event_reg_list[WHD_AP_EVENT_ENTRY] = WHD_EVENT_NOT_REGISTERED;
    whd_wifi_set_ap_is_up(whd_driver, WHD_FALSE);

    ifp->role = WHD_INVALID_ROLE;
    return WHD_SUCCESS;
}

