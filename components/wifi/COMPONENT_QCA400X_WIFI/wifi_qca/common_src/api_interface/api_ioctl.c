//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include "wifi_common.h"

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <custom_wlan_api.h>
#include <hcd_api.h>
#include <common_api.h>
#include <wmi_api.h>
#include <wmi_host.h>
#include <wlan_api.h>
#include <targaddrs.h>
#include <spi_hcd_if.h>
#include "hw20_mbox_host_reg.h"
#include <atheros_wifi_api.h>
#include "hw20_apb_map.h"
#include "hw20_mbox_reg.h"
extern const WMI_SCAN_PARAMS_CMD default_scan_param;
uint8_t countryCode_storge[4];
A_STATUS
Program_CountryCode(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt;
    A_STATUS status = A_ERROR;
    uint32_t param;
    uint8_t pass_cycle = 0;
    A_NETBUF_DECLARE req;
    void *pReq = (void *)&req;
    uint32_t country_code_word;
    uint8_t *ptr_country_code = (uint8_t *)&country_code_word;
    uint32_t i;
    A_NETBUF_CONFIGURE(pReq, &country_code_word, 0, sizeof(uint32_t), sizeof(uint32_t));
    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /* bring the chip down */
        pDCxt->wmiReady = false;
        HW_PowerUpDown(pCxt, false);
        /* bring the chip back up */
        HW_PowerUpDown(pCxt, true);
        pDCxt->htc_creditInit = 0;
        /* re-initialize host controller (sub-)driver = HCD */
        if ((status = Hcd_ReinitTarget(pCxt)) != A_OK)
        {
            A_ASSERT(0);
        }

        if (pass_cycle == 0)
        {
            A_MDELAY(5);
            /* wait for chip to reach point in initialization */
            do
            {
                if (Driver_ReadDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_refclk_hz)), (uint8_t *)&param,
                                        4) != A_OK)
                {
                    A_ASSERT(0);
                }
            } while (param != A_CPU2LE32(EXPECTED_REF_CLK_AR4100) && param != A_CPU2LE32(EXPECTED_REF_CLK_AR400X));
            /* on 1st pass the mac address is loaded into the
            * scratch registers and the parameter is set to
            * cause the firmware to read/program the mac address.
            */
            ptr_country_code[0] = 0;
            // the first byte is used to store the result, the rest three bytes are used to store countrycode
            A_MEMCPY(&ptr_country_code[1], &countryCode_storge[1], 3);
            QCADRV_PRINTF("ptr_country_code is %c, %c.\n", ptr_country_code[1], ptr_country_code[2]);
            // ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, SCRATCH_ADDRESS, true, sizeof(uint32_t));
            ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, SCRATCH_ADDRESS, true, sizeof(uint32_t));

            if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
            {
                A_ASSERT(0);
            }
#if 0
			A_MEMCPY(&ptr_country_code[0], &pDCxt->conn[pDCxt->devId].reqBssid[2], 4);     
			ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, SCRATCH_ADDRESS+4, true, sizeof(uint32_t));

			if(A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq))){
				A_ASSERT(0);
			}
#endif
            A_MDELAY(5);
            param = A_CPU2LE32(AR4XXX_PARAM_COUNTRY_CODE_MODE);

            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_flash_is_present)),
                                     (uint8_t *)&param, 4) != A_OK)
            {
                A_ASSERT(0);
            }
            /* wait some sufficient number of attempts before giving up. */
            for (i = 0; i < 100; i++)
            {
                /* poll scratch register for indication of completion */
                A_MDELAY(1); // delay 1 ms to make target execute the  setting of countrycode
                ATH_SET_PIO_EXTERNAL_READ_OPERATION(pReq, SCRATCH_ADDRESS, true, sizeof(uint32_t));
                if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
                {
                    A_ASSERT(0);
                }
                // result from target is stored in ptr_country_code
                if ((ptr_country_code[0] != 0))
                {
                    QCADRV_PRINTF("ptr_country_code break;.\n");
                    break; /* done - exit loop */
                }
            }
        }
        else
        {
            /* on 2nd pass the chip is brought up normally */
            A_MDELAY(5);
            Driver_BootComm(pCxt);
            pDCxt->chipDown = false;
        }

        pass_cycle++;
    } while (pass_cycle < 2);

    if (pass_cycle >= 2)
    {
        /* store result in reqBssid for use in calling function */
        countryCode_storge[0] = ptr_country_code[0];
    }
    else
    {
        countryCode_storge[0] = 0;
    }
    QCADRV_PRINTF("target result is %c,%d.\n", ptr_country_code[0], ptr_country_code[0]);
    status = A_OK;
    pDCxt->asynchRequest = NULL;
    //	pDCxt->macProgramming = false;
    DRIVER_WAKE_USER(pCxt);

    return status;
}
A_STATUS
Api_ProgramCountryCode(void *pCxt, uint8_t *country_code, uint16_t length, uint8_t *pResult)
{
    A_STATUS status = A_ERROR;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    boolean countryCodeProgram = true;
    /* init pResult to an error code of 0 */
    *pResult = ATH_PROGRAM_COUNTRY_CODE_RESULT_DRIVER_FAILED;

    do
    {
        if (length != sizeof(ATH_PROGRAM_COUNTRY_CODE_PARAM))
        {
            break;
        }

        if (Api_DriverAccessCheck(pCxt, 0, ACCESS_REQUEST_IOCTL) != A_OK)
        {
            break;
        }

        if (pDCxt->asynchRequest != NULL)
        {
            break;
        }
        /* use the reqBssid as storage for the country code.  the Program_CountryCode
         * function will retrieve this value for use in the operation.
         */
        // A_MEMCPY(pDCxt->conn[pDCxt->devId].reqBssid, country_code, 3);
        A_MEMCPY(&countryCode_storge[1], country_code, 3);

        QCADRV_PRINTF("api_ioctl countryCode_storge is %c,%c.\n", countryCode_storge[1], countryCode_storge[2]);
        pDCxt->asynchRequest = Program_CountryCode;
        DRIVER_WAKE_DRIVER(pCxt);
        DRIVER_WAIT_FOR_CONDITION(pCxt, &countryCodeProgram, false, 5000);

        switch (countryCode_storge[0])
        {
            case 1: /*successful result*/
                *pResult = ATH_PROGRAM_COUNTRY_CODE_RESULT_SUCCESS;
                break;
            case 2: /* device failed in the attempt */
                *pResult = ATH_PROGRAM_COUNTRY_CODE_RESULT_DEV_DENIED;
                break;
            case 4: /* the same country is already programmed */
            case 8: /* the device rejected the mac address */
                *pResult = ATH_PROGRAM_COUNTRY_CODE_RESULT_DEV_FAILED;
                break;
            case 0: /* driver failure to communicate with device */
            default:
                break;
        }
        Api_InitFinish(pCxt);
        Api_WMIInitFinish(pCxt);
        status = A_OK;

    } while (0);

    A_MEMZERO(pDCxt->conn[pDCxt->devId].reqBssid, sizeof(pDCxt->conn[pDCxt->devId].reqBssid));

    return status;
}

#if DRIVER_CONFIG_PROGRAM_MAC_ADDR
/*****************************************************************************/
/*  program_mac_addr - Used to program a new mac address into the attached
 *	 wifi device. the mac address is in pDCxt->reqBssid.  Special firmware
 *	 is loaded to perform the operation.  the MAC address is provided to
 * 	 the firmware through the MBOX scratch registers.
 * 		void *pCxt - the driver context.
 *****************************************************************************/
static A_STATUS program_mac_addr(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt;
    A_STATUS status = A_ERROR;
    uint32_t param;
    uint8_t pass_cycle = 0;
    A_NETBUF_DECLARE req;
    void *pReq = (void *)&req;
    uint32_t mac_word;
    uint8_t *ptr_mac_word = (uint8_t *)&mac_word;
    uint32_t i;

    A_NETBUF_CONFIGURE(pReq, &mac_word, 0, sizeof(uint32_t), sizeof(uint32_t));
    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /* bring the chip down */
        pDCxt->wmiReady = false;
        HW_PowerUpDown(pCxt, false);
        /* bring the chip back up */
        HW_PowerUpDown(pCxt, true);
        pDCxt->htc_creditInit = 0;
        /* re-initialize host controller (sub-)driver = HCD */
        if ((status = Hcd_ReinitTarget(pCxt)) != A_OK)
        {
            A_ASSERT(0);
        }

        if (pass_cycle == 0)
        {
            A_MDELAY(5);
            /* wait for chip to reach point in initialization */
            do
            {
                if (Driver_ReadDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_refclk_hz)), (uint8_t *)&param,
                                        4) != A_OK)
                {
                    A_ASSERT(0);
                }
            } while (param != A_CPU2LE32(EXPECTED_REF_CLK_AR4100) && param != A_CPU2LE32(EXPECTED_REF_CLK_AR400X));
            /* on 1st pass the mac address is loaded into the
            * scratch registers and the parameter is set to
            * cause the firmware to read/program the mac address.
            */
            ptr_mac_word[0] = 0;
            ptr_mac_word[1] = 0;
            A_MEMCPY(&ptr_mac_word[2], &pDCxt->conn[pDCxt->devId].reqBssid[0], 2);
            ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, SCRATCH_ADDRESS, true, sizeof(uint32_t));

            if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
            {
                A_ASSERT(0);
            }

            A_MEMCPY(&ptr_mac_word[0], &pDCxt->conn[pDCxt->devId].reqBssid[2], 4);
            ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, SCRATCH_ADDRESS + 4, true, sizeof(uint32_t));

            if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
            {
                A_ASSERT(0);
            }
            A_MDELAY(5);
            param = A_CPU2LE32(AR4XXX_PARAM_MACPROG_MODE);

            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_flash_is_present)),
                                     (uint8_t *)&param, 4) != A_OK)
            {
                A_ASSERT(0);
            }
            /* wait some sufficient number of attempts before giving up. */
            for (i = 0; i < 100; i++)
            {
                /* poll scratch register for indication of completion */
                ATH_SET_PIO_EXTERNAL_READ_OPERATION(pReq, SCRATCH_ADDRESS, true, sizeof(uint32_t));

                if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
                {
                    A_ASSERT(0);
                }
                QCADRV_PRINTF("ptr_mac_word[1] is %c.\n", ptr_mac_word[1]);
                if (ptr_mac_word[1] != 0)
                {
                    break; /* done - exit loop */
                }
                /* delay 1 msec before polling again to give the op time to complete */
                A_MDELAY(1);
            }
        }
        else
        {
            /* on 2nd pass the chip is brought up normally */
            A_MDELAY(5);
            Driver_BootComm(pCxt);
            pDCxt->chipDown = false;
        }

        pass_cycle++;
    } while (pass_cycle < 2);

    if (pass_cycle >= 2)
    {
        /* store result in reqBssid for use in calling function */
        pDCxt->conn[pDCxt->devId].reqBssid[0] = ptr_mac_word[1];
    }
    else
    {
        pDCxt->conn[pDCxt->devId].reqBssid[0] = 0;
    }
    status = A_OK;
    pDCxt->asynchRequest = NULL;
    pDCxt->macProgramming = false;
    DRIVER_WAKE_USER(pCxt);

    return status;
}
#endif /* DRIVER_CONFIG_PROGRAM_MAC_ADDR */

/*****************************************************************************/
/*  Install_static_wep_keys - Used to install WEP keys to the device at the
 *	 appropriate time.
 * 		void *pCxt - the driver context.
 *****************************************************************************/
static void Install_static_wep_keys(void *pCxt)
{
    uint8_t index;
    uint8_t keyUsage;
    A_DRIVER_CONTEXT *pDCxt;
    WMI_ADD_CIPHER_KEY_CMD add_key_param;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    for (index = WMI_MIN_KEY_INDEX; index <= WMI_MAX_KEY_INDEX; index++)
    {
        if (pDCxt->conn[pDCxt->devId].wepKeyList[index].keyLen)
        {
            keyUsage = GROUP_USAGE;
            if (index == pDCxt->conn[pDCxt->devId].wepDefTxKeyIndex)
            {
                keyUsage |= TX_USAGE;
            }

            A_MEMZERO(&add_key_param, sizeof(add_key_param));
            add_key_param.keyIndex = index;
            add_key_param.keyType = WEP_CRYPT;
            add_key_param.keyUsage = keyUsage;
            add_key_param.keyLength = pDCxt->conn[pDCxt->devId].wepKeyList[index].keyLen;
            A_MEMCPY(&add_key_param.key, pDCxt->conn[pDCxt->devId].wepKeyList[index].key, add_key_param.keyLength);
            add_key_param.key_op_ctrl = KEY_OP_INIT_VAL;
            wmi_cmd_start(pDCxt->pWmiCxt, &add_key_param, WMI_ADD_CIPHER_KEY_CMDID, sizeof(WMI_ADD_CIPHER_KEY_CMD));
        }
    }
}

/*****************************************************************************/
/*  Api_DisconnectWiFi - Called by upper layer to disconnect a network
 *   connection. Executes wmi_Disconnnect_cmd
 * 		void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Api_DisconnectWiFi(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    if ((pDCxt->conn[pDCxt->devId].isConnected == true) || (pDCxt->conn[pDCxt->devId].isConnectPending == true))
    {
        wmi_cmd_start(pDCxt->pWmiCxt, NULL, WMI_DISCONNECT_CMDID, 0);
        /*
         * Disconnect cmd is issued, clear connectPending.
         * arConnected will be cleard in disconnect_event notification.
         */
        pDCxt->conn[pDCxt->devId].isConnectPending = false;
        /*
         * clear connect state so that subsequent connect commands start with same
         * initial configuration every time.
         */
        pDCxt->conn[pDCxt->devId].dot11AuthMode = OPEN_AUTH;
        pDCxt->conn[pDCxt->devId].wpaAuthMode = NONE_AUTH;
        pDCxt->conn[pDCxt->devId].wpaPairwiseCrypto = pDCxt->conn[pDCxt->devId].wpaGroupCrypto = NONE_CRYPT;
        pDCxt->conn[pDCxt->devId].wpaPairwiseCryptoLen = pDCxt->conn[pDCxt->devId].wpaGroupCryptoLen = 0;
        A_MEMZERO(pDCxt->conn[pDCxt->devId].reqBssid, sizeof(pDCxt->conn[pDCxt->devId].reqBssid));
        pDCxt->conn[pDCxt->devId].channelHint = 0;
        pDCxt->conn[pDCxt->devId].connectCtrlFlags = 0;
    }

    return A_OK;
}

/*****************************************************************************/
/*  Api_ConnectWiFi - Called by upper layer to start a network connection
 *	 operation.  Executes wmi_connnect_cmd
 * 		void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Api_ConnectWiFi(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt;
    A_STATUS status = A_OK;
    WMI_CONNECT_CMD conn_cmd = {0};
    WMI_SCAN_PARAMS_CMD scan_param_cmd;
    uint8_t devId;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    devId = pDCxt->devId;
    /* The ssid length check prevents second "essid off" from the user,
          to be treated as a connect cmd. The second "essid off" is ignored.
       */
    if ((pDCxt->wmiReady == true) && (pDCxt->conn[devId].ssidLen > 0) && pDCxt->conn[devId].networkType != AP_NETWORK)
    {
        if (/*(ADHOC_NETWORK != ar->arNetworkType) &&*/ // Allow wep key installation in AD-HOC mode
            (NONE_AUTH == pDCxt->conn[pDCxt->devId].wpaAuthMode) &&
            (WEP_CRYPT == pDCxt->conn[pDCxt->devId].wpaPairwiseCrypto))
        {
            Install_static_wep_keys(pCxt);
        }
        /* set scan ctrl flags to default. this will ensure that the firmware will scan for
         * an appropriate AP during connect operation. All params that are zero will remain
         * unchanged from their pre existing value. If WPS had been previously used it would
         * have set the scan ctrl flags in such a way that a subsequent connect might fail.
         * This wmi_scanparams_cmd() fixes that issue.
         */
        do
        {
            A_MEMCPY(&scan_param_cmd, &pDCxt->scan_param, sizeof(WMI_SCAN_PARAMS_CMD));
            scan_param_cmd.bg_period = pDCxt->scan_param.bg_period  = A_CPU2LE16(0xffff);
            if (A_OK !=
                wmi_cmd_start(pDCxt->pWmiCxt, &scan_param_cmd, WMI_SET_SCAN_PARAMS_CMDID, sizeof(WMI_SCAN_PARAMS_CMD)))
            {
                break;
            }
            else
            {
                WAIT_FOR_WMI_RESPONSE(pCxt);
            }

            if (pDCxt->conn[devId].ssidLen)
            {
                A_MEMCPY(conn_cmd.ssid, pDCxt->conn[devId].ssid, pDCxt->conn[devId].ssidLen);
            }

            conn_cmd.ssidLength = pDCxt->conn[devId].ssidLen;
            conn_cmd.networkType = pDCxt->conn[devId].networkType;
            conn_cmd.dot11AuthMode = pDCxt->conn[devId].dot11AuthMode;
            conn_cmd.authMode = pDCxt->conn[devId].wpaAuthMode;
            conn_cmd.pairwiseCryptoType = pDCxt->conn[devId].wpaPairwiseCrypto;
            conn_cmd.pairwiseCryptoLen = pDCxt->conn[devId].wpaPairwiseCryptoLen;
            conn_cmd.groupCryptoType = pDCxt->conn[devId].wpaGroupCrypto;
            conn_cmd.groupCryptoLen = pDCxt->conn[devId].wpaGroupCryptoLen;
            conn_cmd.channel = A_CPU2LE16(pDCxt->conn[devId].channelHint);
            conn_cmd.ctrl_flags = A_CPU2LE32(pDCxt->conn[devId].connectCtrlFlags);

            //NOTE: coverity fix
            A_MEMCPY(conn_cmd.bssid, pDCxt->conn[devId].reqBssid, ATH_MAC_LEN);

            status = wmi_cmd_start(pDCxt->pWmiCxt, (void *)&conn_cmd, WMI_CONNECT_CMDID, sizeof(WMI_CONNECT_CMD));

            if (status != A_OK)
            {
                break;
            }
            else
            {
                WAIT_FOR_WMI_RESPONSE(pCxt);
            }

            pDCxt->conn[pDCxt->devId].isConnectPending = true;
        } while (0);

        return status;
    }
#if ENABLE_AP_MODE
    else if (pDCxt->wmiReady == true && pDCxt->conn[pDCxt->devId].networkType == AP_NETWORK)
    {
        if ((NONE_AUTH == pDCxt->conn[pDCxt->devId].wpaAuthMode) &&
            (WEP_CRYPT == pDCxt->conn[pDCxt->devId].wpaPairwiseCrypto))
        {
            Install_static_wep_keys(pCxt);
        }
        A_MEMCPY(conn_cmd.ssid, (uint8_t *)pDCxt->conn[devId].ssid, sizeof(conn_cmd.ssid));
        conn_cmd.ssidLength = (int)pDCxt->conn[devId].ssidLen;
        conn_cmd.pairwiseCryptoLen = (uint8_t)pDCxt->conn[devId].wpaPairwiseCryptoLen;
        conn_cmd.groupCryptoLen = (uint8_t)pDCxt->conn[devId].wpaGroupCryptoLen;
        conn_cmd.networkType = pDCxt->conn[devId].networkType;
        conn_cmd.dot11AuthMode = pDCxt->conn[devId].dot11AuthMode;
        conn_cmd.authMode = pDCxt->conn[devId].wpaAuthMode;
        conn_cmd.pairwiseCryptoType = pDCxt->conn[devId].wpaPairwiseCrypto;
        conn_cmd.groupCryptoType = pDCxt->conn[devId].wpaGroupCrypto;

        conn_cmd.ctrl_flags = (pDCxt->apmodeWPS) ? A_CPU2LE32(pDCxt->conn[devId].connectCtrlFlags | CONNECT_WPS_FLAG) :
                                                   A_CPU2LE32(pDCxt->conn[devId].connectCtrlFlags);
        conn_cmd.channel = A_CPU2LE16(pDCxt->conn[devId].channelHint);

        status = wmi_cmd_start(pDCxt->pWmiCxt, (void *)&conn_cmd, WMI_AP_CONFIG_COMMIT_CMDID, sizeof(WMI_CONNECT_CMD));
        return status;
    }
#endif /* ENABLE_AP_MODE */

    return A_ERROR;
}

static uint8_t wpa_auth_parse(uint8_t *sel)
{
#define WPA_SEL(x) (((x) << 24) | WPA_OUI)
    uint32_t w = A_LE_READ_4(sel);

    switch (w)
    {
        case WPA_SEL(WPA_ASE_8021X_UNSPEC):
            return WPA_AUTH;
        case WPA_SEL(WPA_ASE_8021X_PSK):
            return WPA_PSK_AUTH;
        case WPA_SEL(WPA_ASE_NONE):
            return NONE_AUTH;
    }

    return 0;
#undef WPA_SEL
}

static uint8_t wpa_cipher_parse(uint8_t *sel, uint8_t *keylen)
{
#define WPA_SEL(x) (((x) << 24) | WPA_OUI)
    uint32_t w = A_LE_READ_4(sel);

    switch (w)
    {
        case WPA_SEL(WPA_CSE_NULL):
            return NONE_CRYPT;
        case WPA_SEL(WPA_CSE_WEP40):
            if (keylen)
                *keylen = 40 >> 3; // 40/bits_per_byte
            return WEP_CRYPT;
        case WPA_SEL(WPA_CSE_WEP104):
            if (keylen)
                *keylen = 104 >> 3; // 104/bits_per_byte
            return WEP_CRYPT;
        case WPA_SEL(WPA_CSE_TKIP):
            return TKIP_CRYPT;
        case WPA_SEL(WPA_CSE_CCMP):
            return AES_CRYPT;
    }

    return 0;
#undef WPA_SEL
}

static CRYPTO_TYPE rsn_cipher_parse(uint8_t *sel, uint8_t *keylen)
{
#define RSN_SEL(x) (((x) << 24) | RSN_OUI)
    uint32_t w = A_LE_READ_4(sel);

    switch (w)
    {
        case RSN_SEL(RSN_CSE_NULL):
            return NONE_CRYPT;
        case RSN_SEL(RSN_CSE_WEP40):
            if (keylen)
                *keylen = 40 >> 3; // 40/bits_per_byte
            return WEP_CRYPT;
        case RSN_SEL(RSN_CSE_WEP104):
            if (keylen)
                *keylen = 104 >> 3; // 104/bits_per_byte
            return WEP_CRYPT;
        case RSN_SEL(RSN_CSE_TKIP):
            return TKIP_CRYPT;
        case RSN_SEL(RSN_CSE_CCMP):
            return AES_CRYPT;
    }
    return NONE_CRYPT;
#undef RSN_SEL
}

static uint8_t rsn_auth_parse(uint8_t *sel)
{
#define RSN_SEL(x) (((x) << 24) | RSN_OUI)
    uint32_t w = A_LE_READ_4(sel);

    switch (w)
    {
        case RSN_SEL(RSN_ASE_8021X_UNSPEC):
            return WPA2_AUTH;
        case RSN_SEL(RSN_ASE_8021X_PSK):
            return WPA2_PSK_AUTH;
        case RSN_SEL(RSN_ASE_NONE):
            return NONE_AUTH;
    }

    return 0;
#undef RSN_SEL
}

static void security_ie_parse(uint8_t *pie, uint8_t ie_len, uint8_t *pResult, uint8_t ie_type)
{
    uint16_t cnt;
    uint16_t i;
    uint8_t wepKeyLen;
    /* skip mcast cipher */
    if (ie_len >= 4)
    {
        ie_len -= 4;
        pie += 4;
    }
    /* examine ucast cipher(s) */
    if (ie_len > 2)
    {
        cnt = A_LE_READ_2(pie);
        ie_len -= 2;
        pie += 2;

        for (i = 0; ((i < cnt) && (ie_len > 0)); i++)
        {
            if (ie_type == IEEE80211_ELEMID_RSN)
            {
                pResult[0] |= rsn_cipher_parse(pie, &wepKeyLen);
            }
            else
            {
                pResult[0] |= wpa_cipher_parse(pie, &wepKeyLen);
            }

            ie_len -= 4;
            pie += 4;
        }
    }
    else
    {
        /* assume default TKIP for wpa */
        pResult[0] |= (ie_type == IEEE80211_ELEMID_RSN) ? AES_CRYPT : TKIP_CRYPT;
    }
    /* parse auth types */
    if (ie_len > 2)
    {
        cnt = A_LE_READ_2(pie);
        ie_len -= 2;
        pie += 2;

        for (i = 0; ((i < cnt) && (ie_len > 0)); i++)
        {
            if (ie_type == IEEE80211_ELEMID_RSN)
            {
                pResult[1] |= rsn_auth_parse(pie);
            }
            else
            {
                pResult[1] |= wpa_auth_parse(pie);
            }

            ie_len -= 4;
            pie += 4;
        }
    }
    else
    {
        pResult[1] |= (ie_type == IEEE80211_ELEMID_RSN) ? WPA2_AUTH : WPA_AUTH;
    }
}

A_STATUS
Api_ParseInfoElem(void *pCxt, WMI_BSS_INFO_HDR *bih, int32_t len, A_SCAN_SUMMARY *pSummary)
{
    uint8_t *buf;
    uint8_t *pie, *pieEnd, *pieTemp;
    uint8_t ie_result[2];
    uint16_t ie_len;

    A_MEMZERO(pSummary, sizeof(A_SCAN_SUMMARY));

    pSummary->channel = (uint8_t)Util_Freq2ieee(bih->channel);
    pSummary->rssi = bih->snr; /* snr is a value from 0 -> 95 where 95 is a strong signal */
    A_MEMCPY(pSummary->bssid, bih->bssid, ATH_MAC_LEN);

    if ((uint32_t)len < sizeof(WMI_BSS_INFO_HDR) + IE_INDEX)
    {
        return A_ERROR;
    }

    buf = (uint8_t *)(bih + 1);
    len -= sizeof(WMI_BSS_INFO_HDR);

    pSummary->caps = (uint16_t)((uint16_t)buf[CAPS_IE_INDEX] | (uint16_t)(buf[CAPS_IE_INDEX + 1] << 8));
    pSummary->beacon_period =
        (uint16_t)((uint16_t)buf[BEACON_PERIOD_IE_INDEX] | (uint16_t)(buf[BEACON_PERIOD_IE_INDEX + 1] << 8));
    /* loop through IE's to collect additional info */
    pie = &buf[IE_INDEX];
    len -= IE_INDEX;
    pieEnd = &pie[len];

    while (pie < pieEnd)
    {
        switch (*pie)
        {
            case IEEE80211_ELEMID_SSID:
                if (pie[1] <= 32)
                {
                    pSummary->ssid_len = pie[1];

                    if (pie[1])
                    {
                        A_MEMCPY(pSummary->ssid, &pie[2], pie[1]);
                    }
                }
                break;
            case IEEE80211_ELEMID_RSN:
                /*******************/
                /* parse RSN IE    */
                /*******************/
                ie_len = pie[1];   /* init ie_len - sizeof wpa_oui */
                pieTemp = &pie[2]; /* init pieTemp beyond wpa_oui */

                if (A_LE_READ_2(pieTemp) != RSN_VERSION)
                {
                    break;
                }
                ie_len -= 2;
                pieTemp += 2;
                ie_result[0] = ie_result[1] = 0;

                security_ie_parse(pieTemp, ie_len, &ie_result[0], IEEE80211_ELEMID_RSN);
                pSummary->rsn_cipher = ie_result[0];
                pSummary->rsn_auth = ie_result[1];
                break;
            case IEEE80211_ELEMID_VENDOR:
                if (pie[1] > 6 && A_LE_READ_4(pie + 2) == ((WPA_OUI_TYPE << 24) | WPA_OUI))
                {
                    /*******************/
                    /* parse WPA IE    */
                    /*******************/
                    ie_len = pie[1] - 4; /* init ie_len - sizeof wpa_oui */
                    pieTemp = &pie[6];   /* init pieTemp beyond wpa_oui */

                    if (A_LE_READ_2(pieTemp) != WPA_VERSION)
                    {
                        break; /* bad version */
                    }

                    ie_len -= 2;
                    pieTemp += 2;
                    ie_result[0] = ie_result[1] = 0;

                    security_ie_parse(pieTemp, ie_len, &ie_result[0], IEEE80211_ELEMID_VENDOR);
                    pSummary->wpa_cipher = ie_result[0];
                    pSummary->wpa_auth = ie_result[1];
                }
                break;
        }

        pie += pie[1] + 2;
    }

    return A_OK;
}

/*****************************************************************************/
/*  Api_DriverAccessCheck - Called by upper layer to confirm that the driver
 *   is in a State to allow IOCTL's and other requests.
 * 		void *pCxt - the driver context.
 *      uint8_t block_allowed - caller can tolerate a task block if the
 *      check is not immediately satisfied.
 *      uint8_t request_reason - callers reason to request driver access.
 *****************************************************************************/
A_STATUS
Api_DriverAccessCheck(void *pCxt, uint8_t block_allowed, uint8_t request_reason)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    A_STATUS status = A_OK;
    uint8_t blocked;

    UNUSED_ARGUMENT(request_reason);

    do
    {
        blocked = 0;
        /* if the chip is powered down when a tx operation is submitted
         * then the policy is to wait for the chip to come back */
        if (true == pDCxt->chipDown)
        {
            blocked = 1;

            if (block_allowed == 0)
            {
                /* caller forbids blocking so return A_ERROR */
                status = A_ERROR;
                break;
            }

            do
            {
                DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->chipDown), false, 5000);
            } while (true == pDCxt->chipDown);
        }
#if DRIVER_CONFIG_ENABLE_STORE_RECALL
        /*Check str-rcl state only if applications requests it*/
        if (request_reason != ACCESS_REQUEST_RX)
        {
            /* if a strrcl command is in process all ioctl's and tx operations
             * must be blocked/deferred until it completes.
             */
            if (true == pDCxt->strrclBlock)
            {
                blocked = 1;

                if (block_allowed == 0)
                {
                    /* caller forbids blocking so return A_ERROR */
                    status = A_ERROR;
                    break;
                }

                do
                {
                    DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->strrclBlock), false, 5000);
                } while (true == pDCxt->strrclBlock);
            }
        }
#endif

    } while (blocked); /* loop until all conditions are met at one pass */

    return status;
}

#if DRIVER_CONFIG_PROGRAM_MAC_ADDR
A_STATUS
Api_ProgramMacAddress(void *pCxt, uint8_t *addr, uint16_t length, uint8_t *pResult)
{
    A_STATUS status = A_ERROR;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    /* init pResult to an error code of 0 */
    *pResult = ATH_PROGRAM_MAC_RESULT_DRIVER_FAILED;

    do
    {
        if (length != sizeof(ATH_PROGRAM_MAC_ADDR_PARAM))
        {
            break;
        }

        if (Api_DriverAccessCheck(pCxt, 0, ACCESS_REQUEST_IOCTL) != A_OK)
        {
            break;
        }

        if (pDCxt->asynchRequest != NULL)
        {
            break;
        }
        /* use the reqBssid as storage for the mac address.  the program_mac_addr
         * function will retrieve this value for use in the operation.
         */
        A_MEMCPY(pDCxt->conn[pDCxt->devId].reqBssid, addr, sizeof(pDCxt->conn[pDCxt->devId].reqBssid));
        pDCxt->macProgramming = true;
        pDCxt->asynchRequest = program_mac_addr;
        DRIVER_WAKE_DRIVER(pCxt);
        DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->macProgramming), false, 5000);

        if (pDCxt->macProgramming == false)
        {
            switch (pDCxt->conn[pDCxt->devId].reqBssid[0])
            {
                case 1: /*successful result*/
                    *pResult = ATH_PROGRAM_MAC_RESULT_SUCCESS;
                    break;
                case 2: /* device failed in the attempt */
                    *pResult = ATH_PROGRAM_MAC_RESULT_DEV_FAILED;
                    break;
                case 4: /* the same mac address is already programmed */
                case 8: /* the device rejected the mac address */
                    *pResult = ATH_PROGRAM_MAC_RESULT_DEV_DENIED;
                    break;
                case 0: /* driver failure to communicate with device */
                default:
                    break;
            }

            Api_InitFinish(pCxt);
            Api_WMIInitFinish(pCxt);
            status = A_OK;
        }

    } while (0);

    A_MEMZERO(pDCxt->conn[pDCxt->devId].reqBssid, sizeof(pDCxt->conn[pDCxt->devId].reqBssid));

    return status;
}
#endif /* DRIVER_CONFIG_PROGRAM_MAC_ADDR */
A_STATUS
Api_SetPowerMode(void *pCxt, POWER_MODE *app_setting)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    if (app_setting->pwr_module < PWR_MAX)
    {
        (app_setting->pwr_mode == REC_POWER) ? (pDCxt->pwrStateSetting &= ~(1 << app_setting->pwr_module)) :
                                               (pDCxt->pwrStateSetting |= (1 << app_setting->pwr_module));
    }
    /* Set MAX Perf */
    if ((app_setting->pwr_mode == MAX_PERF_POWER) && (pDCxt->userPwrMode == REC_POWER))
    {
        pDCxt->userPwrMode = MAX_PERF_POWER;
    }
    /* Set REC Power */
    if ((app_setting->pwr_mode == REC_POWER) && (pDCxt->pwrStateSetting == 0x00))
    {
        pDCxt->userPwrMode = REC_POWER;
    }
    QCADRV_PRINTF("Power Mode : %x,%d,%d \n", pDCxt->pwrStateSetting, app_setting->pwr_mode, app_setting->pwr_module);
    if (A_OK !=
        wmi_cmd_start(pDCxt->pWmiCxt, &pDCxt->userPwrMode, WMI_SET_POWER_MODE_CMDID, sizeof(WMI_POWER_MODE_CMD)))
    {
        return A_ERROR;
    }

    if ((app_setting->pwr_mode == REC_POWER) && pDCxt->userPwrMode == MAX_PERF_POWER)
    {
        QCADRV_PRINTF("Device will remain MAX_PERF \n");
    }
    return A_OK;
}

#if !ENABLE_STACK_OFFLOAD
extern const uint8_t max_performance_power_param;
int32_t power_state_for_module = 0;
/*****************************************************************************/
/*  move_to_maxperf - Changes the power mode to max perf in firmware. There are
 *                    some commands which need to run in MAX_PERF mode like store_recall etc
 *                    This function changes the power modeto MAX_PERF and will not
 *                    intimate the application. Later after the command succeeds,
 *                    use restore_power_state to change to original power state
 *                    Note: Application should not directly use this function
 *  int32_t - module which request the change
 *  Returns- A_OK on success, A_ERROR otherwise
 *****************************************************************************/
A_STATUS move_power_state_to_maxperf(void *pCxt, int32_t module)
{
    A_DRIVER_CONTEXT *pDCxt = (A_DRIVER_CONTEXT *)pCxt;

    /* Already some HIGH module changes state */
    if (power_state_for_module >= module)
    {
        return A_OK;
    }

    /* Change the power mode only when the current power mode is REC_POWER */
    if (pDCxt->userPwrMode == REC_POWER)
    {
        if (A_OK != wmi_cmd_start(pDCxt->pWmiCxt, &max_performance_power_param, WMI_SET_POWER_MODE_CMDID,
                                  sizeof(WMI_POWER_MODE_CMD)))
        {
            return A_ERROR;
        }
    }
    power_state_for_module = module;
    return A_OK;
}
#endif
A_STATUS wait_scan_done(void *pCxt, void *pWmi)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    if (pDCxt->scanDone == false)
    {
        /* block until scan completes */
        DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->scanDone), true, 5000);

        if (pDCxt->scanDone == false)
        {
            wmi_bssfilter_cmd(pWmi, NONE_BSS_FILTER, 0);
        }
        return A_ERROR;
    }
    pDCxt->scanDone = false;
    return A_OK;
}

A_STATUS scan_setup(void *pCxt, void *pWmi, WMI_START_SCAN_CMD *start_scan)
{
    union
    {
        WMI_START_SCAN_CMD scan_cmd;
        WMI_PROBED_SSID_CMD probeParam;
    } stackU;
    A_STATUS error = A_OK;
    uint32_t size;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        if (pDCxt->conn[pDCxt->devId].ssidLen > 0)
        {
            /* apply filter ssid */
            stackU.probeParam.entryIndex = 1;
            stackU.probeParam.flag = SPECIFIC_SSID_FLAG;
            stackU.probeParam.ssidLength = pDCxt->conn[pDCxt->devId].ssidLen;
            A_MEMCPY(stackU.probeParam.ssid, pDCxt->conn[pDCxt->devId].ssid, pDCxt->conn[pDCxt->devId].ssidLen);

            if (A_OK != wmi_cmd_start(pWmi, &stackU.probeParam, WMI_SET_PROBED_SSID_CMDID, sizeof(WMI_PROBED_SSID_CMD)))
            {
                error = A_ERROR;
                break;
            }

            if (A_OK != wmi_bssfilter_cmd(pWmi, PROBED_SSID_FILTER, 0))
            {
                error = A_ERROR;
                break;
            }
        }
        else
        {
            /* clear any pre-existing filter ssid */
            stackU.probeParam.entryIndex = 1;
            stackU.probeParam.flag = DISABLE_SSID_FLAG;
            stackU.probeParam.ssidLength = 0;

            if (A_OK != wmi_cmd_start(pWmi, &stackU.probeParam, WMI_SET_PROBED_SSID_CMDID, sizeof(WMI_PROBED_SSID_CMD)))
            {
                error = A_ERROR;
                break;
            }

            if (A_OK != wmi_bssfilter_cmd(pWmi, ALL_BSS_FILTER, 0))
            {
                error = A_ERROR;
                break;
            }
        }

        GET_DRIVER_COMMON(pCxt)->scanOutSize = ATH_MAX_SCAN_BUFFERS;
        GET_DRIVER_COMMON(pCxt)->scanOutCount = 0;
        pDCxt->scanDone = false;
        /* start the scan */
        if (start_scan == NULL)
        {
            A_MEMZERO(&stackU.scan_cmd, sizeof(WMI_START_SCAN_CMD));
            stackU.scan_cmd.scanType = WMI_LONG_SCAN;
            stackU.scan_cmd.forceFgScan = false;
            stackU.scan_cmd.isLegacy = false;
            stackU.scan_cmd.homeDwellTime = 0;
            stackU.scan_cmd.forceScanInterval = 1;
            stackU.scan_cmd.numChannels = 0;
            if (A_OK != wmi_cmd_start(pWmi, (void *)&stackU.scan_cmd, WMI_START_SCAN_CMDID, sizeof(WMI_START_SCAN_CMD)))
            {
                error = A_ERROR;
            }
        }
        else
        {
            size = sizeof(WMI_START_SCAN_CMD);
            if (start_scan->numChannels > 0)
            {
                size = size + (start_scan->numChannels - 1) * sizeof(uint16_t);
            }

            if (A_OK != wmi_cmd_start(pWmi, (void *)start_scan, WMI_START_SCAN_CMDID, size))
            {
                error = A_ERROR;
            }
        }

    } while (0);

    return error;
}

uint8_t Util_Ascii2Hex(char val)
{
    if ('0' <= val && '9' >= val)
    {
        return (uint8_t)(val - '0');
    }
    else if ('a' <= val && 'f' >= val)
    {
        return (uint8_t)((val - 'a') + 0x0a);
    }
    else if ('A' <= val && 'F' >= val)
    {
        return (uint8_t)((val - 'A') + 0x0a);
    }

    return 0xff; /* error */
}

A_STATUS
TxRawPacket(void *pCxt, void *pReq, ATH_MAC_TX_PARAMS *pParams)
{
    WMI_TX_META_V3 v3_meta;
    HTC_ENDPOINT_ID eid;
    A_DRIVER_CONTEXT *pDCxt;
    A_STATUS status = A_ERROR;

    eid = Util_AC2EndpointID(pCxt, WMM_AC_BE);
    A_NETBUF_SET_ELEM(pReq, A_REQ_EPID, eid);
    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /* translate from ATH_MAC_TX_PARAMS to WMI_TX_META_V3 */
        /* how convenient the values are all the same */
        v3_meta.pktID = pParams->pktID;
        A_MEMCPY(&v3_meta.rateSched.rateSeries, &pParams->rateSched.rateSeries,
                 sizeof(uint8_t) * WMI_TX_MAX_RATE_SERIES);
        A_MEMCPY(&v3_meta.rateSched.trySeries, &pParams->rateSched.trySeries, sizeof(uint8_t) * WMI_TX_MAX_RATE_SERIES);
        v3_meta.rateSched.flags = pParams->rateSched.flags;
        v3_meta.rateSched.accessCategory = pParams->rateSched.accessCategory;

        if (wmi_data_hdr_add(pDCxt->pWmiCxt, pReq, DATA_MSGTYPE, false, WMI_DATA_HDR_DATA_TYPE_802_11,
                             WMI_META_VERSION_3, (void *)&v3_meta) != A_OK)
        {
            break;
        }
        A_MDELAY(2);
        /* HTC interface is asynchronous, if this fails, cleanup will happen in
         * the ar6000_tx_complete callback */
        if (A_OK != Driver_SubmitTxRequest((void *)pCxt, pReq))
        {
            A_NETBUF_FREE(pReq);
            break;
        }

        status = A_OK;
    } while (0);

    return status;
}

/* EOF */
