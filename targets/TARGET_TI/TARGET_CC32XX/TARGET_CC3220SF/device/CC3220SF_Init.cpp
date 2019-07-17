/* CC3220SF_Init.cpp
 * Copyright (c) 2018 ARM Limited
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
#include <assert.h>
#include "nsapi_types.h"
#include "ti/drivers/net/wifi/netcfg.h"
#include "ti/drivers/ti_SPI.h"
#include "ti/drivers/dma/UDMACC32XX.h"
#include "ti/drivers/net/wifi/sl_socket.h"
#include "ti/drivers/net/wifi/netapp.h"
#include "ti/drivers/net/wifi/wlan.h"
#include "mbed.h"
#include "cc3200_simplelink.h"
#include "CC3220SF_Init.h"

#define CHANNEL_MASK_ALL        (0x1FFF)
#define RSSI_TH_MAX             (-95)
#define TASK_STACK_SIZE         (2048)

/* The SimpleLink host driver architecture mandate spawn thread to be created prior to calling Sl_start (turning the NWP on). */
/* The purpose of this thread is to handle asynchronous events sent from the NWP.
 * Every event is classified and later handled by the Host driver event handlers. */
Thread sl_Task_thread(osPriorityNormal, TASK_STACK_SIZE, NULL, "sl_task_thread");
Mutex sl_init_mutex;

void sl_task_entry()
{
    sl_Task(NULL);
}

int CC3220SF_initSimplelink(void)
{
    uint8_t                              ucConfigOpt;
    uint8_t                              ucPower;
    int32_t                              RetVal = -1;
    int32_t                              Mode = -1;
    uint32_t                             IfBitmap = 0;
    SlWlanScanParamCommand_t             ScanDefault = {0};
    SlWlanRxFilterOperationCommandBuff_t RxFilterIdMask = {{0}};
    static bool                          simplelink_powered = false;

    sl_init_mutex.lock();
    if (!simplelink_powered)
    {
        if (sl_Task_thread.start(callback(sl_task_entry)) != osOK)
        {
            printf("sl_Task failed\n");
            sl_init_mutex.unlock();
            return -1;
        }
        UDMACC32XX_init();
        SPI_init();

        /* Turn NWP on */
        Mode = sl_Start(0, 0, 0);
        if(Mode != ROLE_STA)
        {
            /* Set NWP role as STA */
            RetVal = sl_WlanSetMode(ROLE_STA);
            assert(RetVal == 0);

            /* For changes to take affect, we restart the NWP */
            RetVal = sl_Stop(0);
            assert(RetVal == 0);

            Mode = sl_Start(0, 0, 0);
            assert(RetVal == 0);
        }

        if(Mode != ROLE_STA)
        {
            printf("Failed to configure device to it's default state");
            sl_init_mutex.unlock();
            return -1;
        }

        /* Set policy to auto only */
        RetVal = sl_WlanPolicySet(SL_WLAN_POLICY_CONNECTION, SL_WLAN_CONNECTION_POLICY(1,0,0,0), NULL ,0);
        assert(RetVal == 0);

        /* Disable Auto Provisioning */
        RetVal = sl_WlanProvisioning(SL_WLAN_PROVISIONING_CMD_STOP, 0xFF, 0, NULL, 0x0);
        assert(RetVal == 0);

        /* Delete existing profiles */
        RetVal = sl_WlanProfileDel(0xFF);
        assert(RetVal == 0);

        /* enable DHCP client */
        RetVal = sl_NetCfgSet(SL_NETCFG_IPV4_STA_ADDR_MODE, SL_NETCFG_ADDR_DHCP, 0, 0);
        assert(RetVal == 0);

        /* Disable ipv6 */
        IfBitmap = !(SL_NETCFG_IF_IPV6_STA_LOCAL | SL_NETCFG_IF_IPV6_STA_GLOBAL);
        RetVal = sl_NetCfgSet(SL_NETCFG_IF, SL_NETCFG_IF_STATE, sizeof(IfBitmap),(const unsigned char *)&IfBitmap);
        assert(RetVal == 0);

        /* Configure scan parameters to default */
        ScanDefault.ChannelsMask = CHANNEL_MASK_ALL;
        ScanDefault.RssiThreshold = RSSI_TH_MAX;

        RetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS, sizeof(ScanDefault), (uint8_t *)&ScanDefault);
        assert(RetVal == 0);

        /* Disable scans */
        ucConfigOpt = SL_WLAN_SCAN_POLICY(0, 0);
        RetVal = sl_WlanPolicySet(SL_WLAN_POLICY_SCAN , ucConfigOpt, NULL, 0);
        assert(RetVal == 0);

        /* Set TX power lvl to max */
        ucPower = 0;
        RetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (uint8_t *)&ucPower);
        assert(RetVal == 0);

        /* Set NWP Power policy to 'normal' */
        RetVal = sl_WlanPolicySet(SL_WLAN_POLICY_PM, SL_WLAN_NORMAL_POLICY, NULL, 0);
        assert(RetVal == 0);

        /* Unregister mDNS services */
        RetVal = sl_NetAppMDNSUnRegisterService(0, 0, 0);
        assert(RetVal == 0);

        /* Remove all 64 RX filters (8*8) */
        memset(RxFilterIdMask.FilterBitmap , 0xFF, 8);

        RetVal = sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_REMOVE, sizeof(SlWlanRxFilterOperationCommandBuff_t),(uint8_t *)&RxFilterIdMask);
        assert(RetVal == 0);

        /* Set NWP role as STA */
        RetVal = sl_WlanSetMode(ROLE_STA);
        assert(RetVal == 0);

        /* For changes to take affect, we restart the NWP */
        RetVal = sl_Stop(0/*SL_STOP_TIMEOUT*/);
        assert(RetVal == 0);

        Mode = sl_Start(0, 0, 0);
        if(ROLE_STA != Mode)
        {
            printf("Failed to configure device to its default state\n");
            sl_init_mutex.unlock();
            return -1;
        }
        simplelink_powered = true;
    }
    sl_init_mutex.unlock();
    return 0;
}
