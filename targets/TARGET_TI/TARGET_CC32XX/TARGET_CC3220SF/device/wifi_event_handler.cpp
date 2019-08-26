/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*****************************************************************************

 Application Name     - Network terminal

 Application Overview - Network Terminal is a command line interface (cli) based application,
 used to demonstrate the CC32XX/CC31XX networking capabilities.
 It does that by offering a list of commands, divided into four silos:

    Wlan        : Contains link layer functions like scan, connect, etc.
    NetApp      : Demonstrates the usage of networking applications.
    Socket      : Shows variety of socket API and responsible for sending and receiving packets.
    Transceiver : Gives the user a direct interface to the NWP radio for RF tests, raw sockets (L1) and more.

 Application Details  - Refer to 'Network Terminal' README.html

*****************************************************************************/

/* Standard includes */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "rtos.h"

#include <CC3220SF_WiFiInterface.h>
#include <ti/drivers/net/wifi/simplelink.h>

#define HANDLER_TRACE_ENABLE 0

/****************************************************************************
                      LOCAL FUNCTION PROTOTYPES
****************************************************************************/


/****************************************************************************
                      GLOBAL VARIABLES
****************************************************************************/



/*****************************************************************************
                  Callback Functions
*****************************************************************************/

/*!
    \brief          SimpleLinkWlanEventHandler

    This handler gets called whenever a WLAN event is reported
    by the host driver / NWP. Here user can implement he's own logic
    for any of these events. This handler is used by 'network_terminal'
    application to show case the following scenarios:

    1. Handling connection / Disconnection.
    2. Handling Addition of station / removal.
    3. RX filter match handler.
    4. P2P connection establishment.

    \param          pWlanEvent       -   pointer to Wlan event data.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) sections 4.3.4, 4.4.5 and 4.5.5.

    \sa             cmdWlanConnectCallback, cmdEnableFilterCallback, cmdWlanDisconnectCallback,
                    cmdP2PModecallback.

*/
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    if(!pWlanEvent)
    {
        return;
    }

    switch(pWlanEvent->Id)
    {
        case SL_WLAN_EVENT_CONNECT:
        {
            #if HANDLER_TRACE_ENABLE == 1
            printf("\n\r[WLAN EVENT] STA Connected to the AP: %s , "
                "BSSID: %x:%x:%x:%x:%x:%x\n\r",
                      pWlanEvent->Data.Connect.SsidName, pWlanEvent->Data.Connect.Bssid[0],
                      pWlanEvent->Data.Connect.Bssid[1],pWlanEvent->Data.Connect.Bssid[2],
                      pWlanEvent->Data.Connect.Bssid[3],pWlanEvent->Data.Connect.Bssid[4],
                      pWlanEvent->Data.Connect.Bssid[5]);
            #endif
            CC3220SFInterface::cc3200sf_wifi_instance->_connect_sem.release();
        }
        break;

        case SL_WLAN_EVENT_DISCONNECT:
        {
            SlWlanEventDisconnect_t  *pEventData = NULL;

            pEventData = &pWlanEvent->Data.Disconnect;

            /* If the user has initiated 'Disconnect' request,
              'reason_code' is SL_WLAN_DISCONNECT_USER_INITIATED */
            if(SL_WLAN_DISCONNECT_USER_INITIATED == pEventData->ReasonCode)
            {
                #if HANDLER_TRACE_ENABLE == 1
                printf("\n\r[WLAN EVENT] Device disconnected from the AP\n\r");
                #endif
            }
            else
            {
                printf("\n\r[WLAN ERROR] Device disconnected from the AP\n\r");
            }
        }
        break;

        case SL_WLAN_EVENT_PROVISIONING_STATUS:
        {
            /* Do nothing, this suppress provisioning event is because simplelink is configured to default state. */
        }
        break;

        case SL_WLAN_EVENT_STA_ADDED:
        {
            printf("\n\r[WLAN EVENT] STA was added to AP: BSSID: %x:%x:%x:%x:%x:%x\n\r",
                    pWlanEvent->Data.STAAdded.Mac[0],pWlanEvent->Data.STAAdded.Mac[1],
                    pWlanEvent->Data.STAAdded.Mac[2],pWlanEvent->Data.STAAdded.Mac[3],
                    pWlanEvent->Data.STAAdded.Mac[4],pWlanEvent->Data.STAAdded.Mac[5]);
        }
        break;

        case SL_WLAN_EVENT_STA_REMOVED:
        {
            printf("\n\r[WLAN EVENT] STA was removed from AP\n\r");
        }
        break;

        case SL_WLAN_EVENT_RXFILTER:
        {
            SlWlanEventRxFilterInfo_t  *triggred_filter = NULL;

            triggred_filter = &(pWlanEvent->Data.RxFilterInfo) ;

            printf("\n\r[WLAN EVENT] Rx filter match triggered. Set filters in filter bitmap :0x%x.\n\r", triggred_filter->UserActionIdBitmap[0]);

            /*
             *     User can write he's / her's rx filter match handler here.
             *     Be advised, you can use the 'triggred_filter' structure info to determine which filter
             *     has received a match. (Bit X is set if user action id X was passed to a filter that matched a packet.)
             */
        }
        break;

        case SL_WLAN_EVENT_P2P_DEVFOUND:
        {
            printf("\n\r[WLAN EVENT] P2P Remote device found\n\r");
        }
        break;

        case SL_WLAN_EVENT_P2P_REQUEST:
        {
            printf("\n\r[WLAN EVENT] P2P Negotiation request received\n\r");
        }
        break;

        case SL_WLAN_EVENT_P2P_CONNECT:
        {
            printf("n\r[WLAN EVENT] P2P connection was successfully completed as CLIENT\n\r");
            printf("n\rBSSID is %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                                            pWlanEvent->Data.STAAdded.Mac[0],
                                            pWlanEvent->Data.STAAdded.Mac[1],
                                            pWlanEvent->Data.STAAdded.Mac[2],
                                            pWlanEvent->Data.STAAdded.Mac[3],
                                            pWlanEvent->Data.STAAdded.Mac[4],
                                            pWlanEvent->Data.STAAdded.Mac[5]);
        }
        break;

        case SL_WLAN_EVENT_P2P_CLIENT_ADDED:
        {
            printf("n\r[WLAN EVENT] P2P connection was successfully completed as GO\n\r");
            printf("n\rBSSID is %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                                            pWlanEvent->Data.P2PClientAdded.Mac[0],
                                            pWlanEvent->Data.P2PClientAdded.Mac[1],
                                            pWlanEvent->Data.P2PClientAdded.Mac[2],
                                            pWlanEvent->Data.P2PClientAdded.Mac[3],
                                            pWlanEvent->Data.P2PClientAdded.Mac[4],
                                            pWlanEvent->Data.P2PClientAdded.Mac[5]);
        }
        break;

        case SL_WLAN_EVENT_P2P_DISCONNECT:
        {
            printf("\n\r[WLAN EVENT] STA disconnected from device.\n\r");
        }
        break;

        default:
        {
            printf("\n\r[WLAN EVENT] Unexpected event [0x%x]\n\r", (unsigned int)pWlanEvent->Id);
        }
        break;
    }
}

/*!
    \brief          SimpleLinkNetAppEventHandler

    This handler gets called whenever a Netapp event is reported
    by the host driver / NWP. Here user can implement he's own logic
    for any of these events. This handler is used by 'network_terminal'
    application to show case the following scenarios:

    1. Handling IPv4 / IPv6 IP address acquisition.
    2. Handling IPv4 / IPv6 IP address Dropping.

    \param          pNetAppEvent     -   pointer to Netapp event data.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) section 5.7

*/
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    if(!pNetAppEvent)
    {
        return;
    }

    switch(pNetAppEvent->Id)
    {
        case SL_NETAPP_EVENT_IPV4_ACQUIRED:
        {
            #if HANDLER_TRACE_ENABLE == 1
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            /* Ip Acquired Event Data */
            pEventData = &pNetAppEvent->Data.IpAcquiredV4;

            printf("\n\r[NETAPP EVENT] IP set to: IPv4=%d.%d.%d.%d , "
                    "Gateway=%d.%d.%d.%d\n\r",

                    (unsigned int)SL_IPV4_BYTE(pEventData->Ip,3),
                    (unsigned int)SL_IPV4_BYTE(pEventData->Ip,2),
                    (unsigned int)SL_IPV4_BYTE(pEventData->Ip,1),
                    (unsigned int)SL_IPV4_BYTE(pEventData->Ip,0),

                    (unsigned int)SL_IPV4_BYTE(pEventData->Gateway,3),
                    (unsigned int)SL_IPV4_BYTE(pEventData->Gateway,2),
                    (unsigned int)SL_IPV4_BYTE(pEventData->Gateway,1),
                    (unsigned int)SL_IPV4_BYTE(pEventData->Gateway,0));
            #endif
            CC3220SFInterface::cc3200sf_wifi_instance->_ip_set_sem.release();
        }
        break;

        case SL_NETAPP_EVENT_IPV6_ACQUIRED:
        {
            uint32_t i = 0;

            printf("\n\r[NETAPP EVENT] IP Acquired: IPv6=");

            for(i = 0; i < 3 ; i++)
            {
                printf("%04x:%04x:\n", ((unsigned int)(pNetAppEvent->Data.IpAcquiredV6.Ip[i]>>16) & 0xffff), (unsigned int)pNetAppEvent->Data.IpAcquiredV6.Ip[i] & 0xffff);
            }

            printf("%04x:%04x\n", ((unsigned int)(pNetAppEvent->Data.IpAcquiredV6.Ip[3]>>16) & 0xffff), (unsigned int)pNetAppEvent->Data.IpAcquiredV6.Ip[3] & 0xffff);
        }
        break;

        case SL_NETAPP_EVENT_DHCPV4_LEASED:
        {
            printf("\n\r[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d \n\r",
                    (unsigned int)SL_IPV4_BYTE(pNetAppEvent->Data.IpLeased.IpAddress ,3), (unsigned int)SL_IPV4_BYTE(pNetAppEvent->Data.IpLeased.IpAddress ,2),
                    (unsigned int)SL_IPV4_BYTE(pNetAppEvent->Data.IpLeased.IpAddress ,1), (unsigned int)SL_IPV4_BYTE(pNetAppEvent->Data.IpLeased.IpAddress ,0));
        }
        break;

        case SL_NETAPP_EVENT_DHCPV4_RELEASED:
        {
            printf("\n\r[NETAPP EVENT] IP is released.\n\r");
        }
        break;

        default:
        {
            printf("\n\r[NETAPP EVENT] Unexpected event [0x%x] \n\r", (unsigned int)pNetAppEvent->Id);
        }
        break;
    }
}

/*!
    \brief          SimpleLinkHttpServerEventHandler

    This handler gets called whenever a HTTP event is reported
    by the NWP internal HTTP server.

    \param          pHttpEvent       -   pointer to http event data.

    \param          pHttpEvent       -   pointer to http response.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) chapter 9.

*/
void SimpleLinkHttpServerEventHandler(SlNetAppHttpServerEvent_t *pHttpEvent,
                                      SlNetAppHttpServerResponse_t *pHttpResponse)
{
    /* Unused in this application */
}

/*!
    \brief          SimpleLinkGeneralEventHandler

    This handler gets called whenever a general error is reported
    by the NWP / Host driver. Since these errors are not fatal,
    application can handle them.

    \param          pDevEvent    -   pointer to device error event.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) section 17.9.

*/
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    if(!pDevEvent)
    {
        return;
    }
    /*
      Most of the general errors are not FATAL are are to be handled
      appropriately by the application
    */
    printf("\n\r[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->Data.Error.Code,
               pDevEvent->Data.Error.Source);
}

/*!
    \brief          SimpleLinkSockEventHandler

    This handler gets called whenever a socket event is reported
    by the NWP / Host driver.

    \param          SlSockEvent_t    -   pointer to socket event data.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) section 7.6.

*/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    #if HANDLER_TRACE_ENABLE == 1
    printf("\n\r[INFO] - event detected: "
                            "Event=%d, AbortData=0x%x\n\r",
                            (unsigned int)pSock->Event,
                            (unsigned int)pSock->SocketAsyncEvent.SockTxFailData.Status);
    #endif

}

/*!
    \brief          SimpleLinkFatalErrorEventHandler

    This handler gets called whenever a socket event is reported
    by the NWP / Host driver. After this routine is called, the user's
    application must restart the device in order to recover.

    \param          slFatalErrorEvent    -   pointer to fatal error event.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) section 17.9.

*/
void SimpleLinkFatalErrorEventHandler(SlDeviceFatal_t *slFatalErrorEvent)
{

    switch (slFatalErrorEvent->Id)
    {
        case SL_DEVICE_EVENT_FATAL_DEVICE_ABORT:
        {
            printf("\n\r[ERROR] - FATAL ERROR: Abort NWP event detected: "
                        "AbortType=%d, AbortData=0x%x\n\r",
                        (unsigned int)slFatalErrorEvent->Data.DeviceAssert.Code,
                        (unsigned int)slFatalErrorEvent->Data.DeviceAssert.Value);
        }
        break;

        case SL_DEVICE_EVENT_FATAL_DRIVER_ABORT:
        {
            printf("\n\r[ERROR] - FATAL ERROR: Driver Abort detected. \n\r");
        }
        break;

        case SL_DEVICE_EVENT_FATAL_NO_CMD_ACK:
        {
            printf("\n\r[ERROR] - FATAL ERROR: No Cmd Ack detected "
                        "[cmd opcode = 0x%x] \n\r",
                        (unsigned int)slFatalErrorEvent->Data.NoCmdAck.Code);
        }
        break;

        case SL_DEVICE_EVENT_FATAL_SYNC_LOSS:
        {
            printf("\n\r[ERROR] - FATAL ERROR: Sync loss detected n\r");
        }
        break;

        case SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT:
        {
            printf("\n\r[ERROR] - FATAL ERROR: Async event timeout detected "
                        "[event opcode =0x%x]  \n\r",
                        (unsigned int)slFatalErrorEvent->Data.CmdTimeout.Code);
        }
        break;

        default:
            printf("\n\r[ERROR] - FATAL ERROR: Unspecified error detected \n\r");
        break;
    }
}

/*!
    \brief          SimpleLinkNetAppRequestEventHandler

    This handler gets called whenever a NetApp event is reported
    by the NWP / Host driver. User can write he's logic to handle
    the event here.

    \param          pNetAppRequest     -   Pointer to NetApp request structure.

    \param          pNetAppResponse    -   Pointer to NetApp request Response.

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) section 17.9.

    \return         void

*/
void SimpleLinkNetAppRequestEventHandler(SlNetAppRequest_t *pNetAppRequest, SlNetAppResponse_t *pNetAppResponse)
{
    /* Unused in this application */
}

/*!
    \brief          SimpleLinkNetAppRequestMemFreeEventHandler

    This handler gets called whenever the NWP is done handling with
    the buffer used in a NetApp request. This allows the use of
    dynamic memory with these requests.

    \param          pNetAppRequest     -   Pointer to NetApp request structure.

    \param          pNetAppResponse    -   Pointer to NetApp request Response.

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC3120/CC3220 NWP programmer's
                    guide (SWRU455) section 17.9.

    \return         void

*/
void SimpleLinkNetAppRequestMemFreeEventHandler(uint8_t *buffer)
{
    /* Unused in this application */
}

