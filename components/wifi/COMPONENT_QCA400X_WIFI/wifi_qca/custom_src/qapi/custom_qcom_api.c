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

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wmi_api.h>
#include <bmi.h>
#include <htc.h>
#include <wmi_host.h>
#if ENABLE_P2P_MODE
#include <wmi.h>
#include "p2p.h"
#endif
#include "atheros_wifi.h"
#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"
#include "atheros_stack_offload.h"
#include "dset_api.h"
#include "common_stack_offload.h"
#include "qcom_api.h"
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------

extern uint32_t Custom_Api_Mediactl(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t command_id, void *inout_param);

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name  : custom_qapi
* Returned Value : A_ERROR on error else A_OK
* Comments       : Custom part of QAPI layer. This function will implement the
*                  custom portion of different QAPIs (OS specific).
*                  PORTING NOTE: Engineer should rewrite this function based on
*                  the OS framework.
*END------------------------------------------------------------------*/
A_STATUS custom_qapi(uint8_t device_id, uint32_t cmd, void *param)
{
    uint32_t result;
    //TODO: check param against NULL !
    switch (cmd)
    {
        case ATH_SET_SCAN:
            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_SET_MEDIACTL_SCAN, NULL);
            return QCA_OK == result ? A_OK : A_ERROR;
        case ATH_GET_SCAN_RESULTS:
        {
            QCA_SCAN_LIST scan_param;
            if (Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_GET_MEDIACTL_SCAN, &scan_param) != A_OK)
            {
                return A_ERROR;
            }
            ((ATH_SCAN_LIST *)param)->scan_list = scan_param.scan_info_list;
            ((ATH_SCAN_LIST *)param)->num_scan_entries = scan_param.num_scan_entries;
            return A_OK;
        }
        case ATH_SET_SSID:
        {
            QCA_ESSID ssid_param;
            char *pssid = ((ATH_IOCTL_PARAM_STRUCT *)param)->data;

            //NOTE: lower layer does not use 'ssid_param.flags' at all. The decision actually depends on 'ssid_param.length'.
            //TODO: remove ssid_param.flags ??
            if (NULL == pssid)
            {
                ssid_param.flags = 0;
                ssid_param.essid = pssid;
                ssid_param.length = 0;
            }
            else
            {
                ssid_param.flags = 1;
                ssid_param.essid = pssid;
                ssid_param.length = strlen(pssid);
            }

            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_SET_MEDIACTL_ESSID, &ssid_param);
            return QCA_OK == result ? A_OK : A_ERROR;
        }
        case ATH_GET_ESSID:
        {
            QCA_MEDIACTL_PARAM ssid_param;
            ssid_param.data = ((ATH_IOCTL_PARAM_STRUCT *)param)->data;
            if (Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_GET_MEDIACTL_ESSID, &ssid_param) != A_OK)
                return A_ERROR;

            ((ATH_IOCTL_PARAM_STRUCT *)param)->length = ssid_param.length;
        }
        break;
        case ATH_SET_COMMIT:
            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_SET_MEDIACTL_COMMIT, param);
            return QCA_OK == result ? A_OK : A_ERROR;
        case ATH_GET_POWER:
        {
            QCA_MEDIACTL_PARAM inout_param;
            if (Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_GET_MEDIACTL_POWER, &inout_param) != A_OK)
            {
                return A_ERROR;
            }
            *(uint32_t *)param = inout_param.value;
            return A_OK;
        }
        case ATH_SET_SEC_TYPE:
            switch (*(uint32_t *)param)
            {
                case WLAN_AUTH_NONE:
                    *(uint32_t *)param = QCA_MEDIACTL_SECURITY_TYPE_NONE;
                    break;
                case WLAN_AUTH_WPA_PSK:
                    *(uint32_t *)param = QCA_MEDIACTL_SECURITY_TYPE_WPA;
                    break;
                case WLAN_AUTH_WPA2_PSK:
                    *(uint32_t *)param = QCA_MEDIACTL_SECURITY_TYPE_WPA2;
                    break;
                case WLAN_AUTH_WEP:
                    *(uint32_t *)param = QCA_MEDIACTL_SECURITY_TYPE_WEP;
                    break;
                default:
                    return (A_ERROR);
            }
            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_SET_MEDIACTL_SEC_TYPE, param);
            return QCA_OK == result ? A_OK : A_ERROR;

        case ATH_SET_OP_MODE:
            if (*(uint32_t *)param == QCOM_WLAN_DEV_MODE_STATION)
            {
                *(uint32_t *)param = QCA_MEDIACTL_MODE_INFRA;
            }
            else if (*(uint32_t *)param == QCOM_WLAN_DEV_MODE_ADHOC)
            {
                *(uint32_t *)param = QCA_MEDIACTL_MODE_ADHOC;
            }
            else if (*(uint32_t *)param == QCOM_WLAN_DEV_MODE_AP)
            {
                *(uint32_t *)param = QCA_MEDIACTL_MODE_MASTER;
            }
            else
            {
                return A_ERROR;
            }
            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_SET_MEDIACTL_MODE, param);
            return QCA_OK == result ? A_OK : A_ERROR;

        case ATH_GET_OP_MODE:
            if (Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_GET_MEDIACTL_MODE, param) != A_OK)
            {
                return A_ERROR;
            }

            switch (*(uint32_t *)param)
            {
                case QCA_MEDIACTL_MODE_INFRA:
                    *(uint32_t *)param = QCOM_WLAN_DEV_MODE_STATION;
                    break;
                case QCA_MEDIACTL_MODE_ADHOC:
                    *(uint32_t *)param = QCOM_WLAN_DEV_MODE_ADHOC;
                    break;
                case QCA_MEDIACTL_MODE_MASTER:
                    *(uint32_t *)param = QCOM_WLAN_DEV_MODE_AP;
                    break;
                default:
                    break;
            }
            break;
        case ATH_SET_PASSPHRASE:
        {
            QCA_MEDIACTL_PARAM enet_param;
            enet_param.data = ((ATH_IOCTL_PARAM_STRUCT *)param)->data;
            enet_param.length = ((ATH_IOCTL_PARAM_STRUCT *)param)->length;
            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_SET_MEDIACTL_PASSPHRASE, &enet_param);
            return QCA_OK == result ? A_OK : A_ERROR;
        }
        default:
            result = Custom_Api_Mediactl(Custom_Api_GetDriverCxt(device_id), QCA_MEDIACTL_VENDOR_SPECIFIC, param);
            return QCA_OK == result ? A_OK : A_ERROR;
    }
    return A_OK;
}
