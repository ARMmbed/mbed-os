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
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifdef WIN_MOBILE7
#include <ntddk.h>
#endif

#include <a_config.h>
#include <athdefs.h>
#include <a_types.h>
#include <a_osapi.h>
#include "htc.h"
#include "htc_api.h"
#include "wmi.h"
#include <wlan_api.h>
#include <wmi_api.h>
#include <ieee80211.h>
#include "wmi_host.h"
#include "a_drv.h"
#include "a_drv_api.h"
#include "atheros_wifi_api.h"
#define ATH_MODULE_NAME wmi
#include "a_debug.h"
#include "dbglog_api.h"
#include "cust_netbuf.h"
#include "custom_wlan_api.h"

#if ENABLE_P2P_MODE
#include "p2p.h"
#endif

#include "driver_cxt.h"
#include "dset.h"

extern uint32_t last_driver_error;
// static uint16_t channel_no = 0;

#define ATH_DEBUG_WMI 1
#define ATH_DEBUG_ERR 1

#ifdef DEBUG_WMI_TRACE

static ATH_DEBUG_MASK_DESCRIPTION wmi_debug_desc[] = {
    {ATH_DEBUG_WMI, "General WMI Tracing"},
};

ATH_DEBUG_INSTANTIATE_MODULE_VAR(wmi,
                                 "wmi",
                                 "Wireless Module Interface",
                                 ATH_DEBUG_MASK_DEFAULTS,
                                 ATH_DEBUG_DESCRIPTION_COUNT(wmi_debug_desc),
                                 wmi_debug_desc);

#endif

#ifndef REXOS
#define DBGARG __func__
#define DBGFMT "%s() : "
#define DBG_WMI ATH_DEBUG_WMI
#define DBG_ERROR ATH_DEBUG_ERR
#define DBG_WMI2 ATH_DEBUG_WMI
#define A_DPRINTF AR_DEBUG_PRINTF
#endif

#if ENABLE_P2P_MODE
static A_STATUS wmi_p2p_goneg_result_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);

static A_STATUS wmi_p2p_list_peristent_network(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);

static A_STATUS wmi_p2p_req_auth_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint32_t len);

static A_STATUS wmi_p2p_node_list_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint32_t len);

static A_STATUS wmi_p2p_invite_sent_result_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);
static A_STATUS wmi_p2p_prov_disc_resp_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);
static A_STATUS wmi_p2p_prov_disc_req_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);
#if 1 // KK
static A_STATUS wmi_p2p_sdpd_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);
static A_STATUS wmi_p2p_invite_req_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);
static A_STATUS wmi_p2p_invite_rcvd_result_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len);
#endif
WMI_P2P_PROV_INFO p2p_key_val;

#endif
#if MANUFACTURING_SUPPORT
static A_STATUS wmi_test_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint32_t len);
#endif
static A_STATUS wmi_errorEvent_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, int len);

#define MODE_A_SUPPORT_RATE_START ((int32_t)4)
#define MODE_A_SUPPORT_RATE_STOP ((int32_t)11)

#define MODE_GONLY_SUPPORT_RATE_START MODE_A_SUPPORT_RATE_START
#define MODE_GONLY_SUPPORT_RATE_STOP MODE_A_SUPPORT_RATE_STOP

#define MODE_B_SUPPORT_RATE_START ((int32_t)0)
#define MODE_B_SUPPORT_RATE_STOP ((int32_t)3)

#define MODE_G_SUPPORT_RATE_START ((int32_t)0)
#define MODE_G_SUPPORT_RATE_STOP ((int32_t)11)

#define MODE_GHT20_SUPPORT_RATE_START ((int32_t)0)
#define MODE_GHT20_SUPPORT_RATE_STOP ((int32_t)19)

#define MAX_NUMBER_OF_SUPPORT_RATES (MODE_GHT20_SUPPORT_RATE_STOP + 1)

/* 802.1d to AC mapping. Refer pg 57 of WMM-test-plan-v1.2 */
const uint8_t up_to_ac[] = {
    WMM_AC_BE, WMM_AC_BK, WMM_AC_BK, WMM_AC_BE, WMM_AC_VI, WMM_AC_VI, WMM_AC_VO, WMM_AC_VO,
};

void *wmi_init(void *devt)
{
    struct wmi_t *wmip;

    A_REGISTER_MODULE_DEBUG_INFO(wmi);

    wmip = A_MALLOC(sizeof(struct wmi_t), MALLOC_ID_CONTEXT);
    if (wmip == NULL)
    {
        return (NULL);
    }
    A_MEMZERO(wmip, sizeof(*wmip));
    A_MUTEX_INIT(&wmip->wmi_lock);
    wmip->wmi_devt = devt;
    wmi_qos_state_init(wmip);

    return (wmip);
}

void wmi_qos_state_init(struct wmi_t *wmip)
{
    uint8_t i;

    if (wmip == NULL)
    {
        return;
    }
    LOCK_WMI(wmip);

    /* Initialize QoS States */
    wmip->wmi_numQoSStream = 0;

    wmip->wmi_fatPipeExists = 0;

    for (i = 0; i < WMM_NUM_AC; i++)
    {
        wmip->wmi_streamExistsForAC[i] = 0;
    }

    UNLOCK_WMI(wmip);

    A_WMI_SET_NUMDATAENDPTS(wmip->wmi_devt, 1);
}

void wmi_set_control_ep(struct wmi_t *wmip, HTC_ENDPOINT_ID eid)
{
    A_ASSERT(eid != ENDPOINT_UNUSED);
    wmip->wmi_endpoint_id = eid;
}

HTC_ENDPOINT_ID
wmi_get_control_ep(struct wmi_t *wmip)
{
    return (wmip->wmi_endpoint_id);
}

void wmi_shutdown(struct wmi_t *wmip)
{
    if (wmip != NULL)
    {
        if (A_IS_MUTEX_VALID(&wmip->wmi_lock))
        {
            A_MUTEX_DELETE(&wmip->wmi_lock);
        }
        A_FREE(wmip, MALLOC_ID_CONTEXT);
    }
}

/*
 *  performs DIX to 802.3 encapsulation for transmit packets.
 *  uses passed in buffer.  Returns buffer or NULL if failed.
 *  Assumes the entire DIX header is contigous and that there is
 *  enough room in the buffer for a 802.3 mac header and LLC+SNAP headers.
 */
A_STATUS
wmi_dix_2_dot3(struct wmi_t *wmip, void *osbuf)
{
    uint8_t *datap;
    uint16_t typeorlen;
    ATH_MAC_HDR macHdr;
    ATH_LLC_SNAP_HDR *llcHdr;

    UNUSED_ARGUMENT(wmip);

    A_ASSERT(osbuf != NULL);

    if (A_NETBUF_HEADROOM(osbuf) < (int32_t)(sizeof(ATH_LLC_SNAP_HDR) + sizeof(WMI_DATA_HDR)))
    {
        return A_NO_MEMORY;
    }

    datap = A_NETBUF_DATA(osbuf);

    typeorlen = *(uint16_t *)(datap + ATH_MAC_LEN + ATH_MAC_LEN);

    if (!IS_ETHERTYPE(A_BE2CPU16(typeorlen)))
    {
        /*
         * packet is already in 802.3 format - return success
         */
        A_DPRINTF(DBG_WMI, (DBGFMT "packet already 802.3\n", DBGARG));
        return (A_OK);
    }

    /*
     * Save mac fields and length to be inserted later
     */
    A_MEMCPY(macHdr.dstMac, datap, ATH_MAC_LEN);
    A_MEMCPY(macHdr.srcMac, datap + ATH_MAC_LEN, ATH_MAC_LEN);
    macHdr.typeOrLen = (uint16_t)A_CPU2BE16(A_NETBUF_LEN(osbuf) - sizeof(ATH_MAC_HDR) + sizeof(ATH_LLC_SNAP_HDR));

    /*
     * Make room for LLC+SNAP headers
     */
    if (A_NETBUF_PUSH(osbuf, sizeof(ATH_LLC_SNAP_HDR)) != A_OK)
    {
        return A_NO_MEMORY;
    }
    datap = A_NETBUF_DATA(osbuf);

    A_MEMCPY(datap, &macHdr, sizeof(ATH_MAC_HDR));

    llcHdr = (ATH_LLC_SNAP_HDR *)(datap + sizeof(ATH_MAC_HDR));
    llcHdr->dsap = 0xAA;
    llcHdr->ssap = 0xAA;
    llcHdr->cntl = 0x03;
    llcHdr->orgCode[0] = 0x0;
    llcHdr->orgCode[1] = 0x0;
    llcHdr->orgCode[2] = 0x0;
    llcHdr->etherType = typeorlen;

    return (A_OK);
}

A_STATUS
wmi_meta_add(struct wmi_t *wmip, void *osbuf, uint8_t *pVersion, void *pTxMetaS)
{
    WMI_TX_META_V3 v3In;
    WMI_TX_META_V3 *pV3 = NULL;
    WMI_TX_META_V5 v5In;
    WMI_TX_META_V5 *pV5 = NULL;
    UNUSED_ARGUMENT(wmip);

    switch (*pVersion)
    {
        case WMI_META_VERSION_3:
            A_ASSERT(osbuf != NULL);

            if (A_NETBUF_PUSH(osbuf, WMI_MAX_TX_META_SZ) != A_OK)
            {
                return A_NO_MEMORY;
            }

            A_MEMCPY(&v3In, pTxMetaS, sizeof(WMI_TX_META_V3));
            /* convert for any endianness */
            // no conversions for this structure
            /* copy to frame */
            pV3 = (WMI_TX_META_V3 *)A_NETBUF_DATA(osbuf);
            A_MEMCPY(pV3, &v3In, sizeof(WMI_TX_META_V3));

            A_ASSERT(pVersion != NULL);
            /* the version must be used to populate the meta field of the WMI_DATA_HDR */
            *pVersion = WMI_META_VERSION_3;
            break;

        case WMI_META_VERSION_5:
            A_ASSERT(osbuf != NULL);

            if (A_NETBUF_PUSH(osbuf, WMI_MAX_TX_META_SZ) != A_OK)
            {
                return A_NO_MEMORY;
            }

            A_MEMCPY(&v5In, pTxMetaS, sizeof(WMI_TX_META_V5));
            /* convert for any endianness */
            // no conversions for this structure
            /* copy to frame */
            pV5 = (WMI_TX_META_V5 *)A_NETBUF_DATA(osbuf);
            A_MEMCPY(pV5, &v5In, sizeof(WMI_TX_META_V5));

            A_ASSERT(pVersion != NULL);
            /* the version must be used to populate the meta field of the WMI_DATA_HDR */
            *pVersion = WMI_META_VERSION_5;
            break;
    }

    return A_OK;
}

/* Adds a WMI data header */
A_STATUS
wmi_data_hdr_add(struct wmi_t *wmip,
                 void *osbuf,
                 uint8_t msgType,
                 boolean bMoreData,
                 WMI_DATA_HDR_DATA_TYPE data_type,
                 uint8_t metaVersion,
                 void *pTxMetaS)
{
    WMI_DATA_HDR *dtHdr;
    //    uint8_t metaVersion = 0;
    A_STATUS status = A_OK;
    A_ASSERT(osbuf != NULL);

    if ((status = wmi_meta_add(wmip, osbuf, &metaVersion, pTxMetaS)) != A_OK)
    {
        return status;
    }

    if (A_NETBUF_PUSH(osbuf, sizeof(WMI_DATA_HDR)) != A_OK)
    {
        return A_NO_MEMORY;
    }

    dtHdr = (WMI_DATA_HDR *)A_NETBUF_DATA(osbuf);
    A_MEMZERO(dtHdr, sizeof(WMI_DATA_HDR));

    WMI_DATA_HDR_SET_MSG_TYPE(dtHdr, msgType);
    WMI_DATA_HDR_SET_DATA_TYPE(dtHdr, data_type);

    if (bMoreData)
    {
        WMI_DATA_HDR_SET_MORE_BIT(dtHdr);
    }

    WMI_DATA_HDR_SET_META(dtHdr, metaVersion);
    // dtHdr->rssi = 0;
    // lastly convert the U16 value to LE
    dtHdr->info2 = A_CPU2LE16(dtHdr->info2);
    return status;
}

/* This stuff is used when we want a simple layer-3 visibility */
typedef PREPACK struct _iphdr
{
    uint8_t ip_ver_hdrlen FIELD_PACKED; /* version and hdr length */
    uint8_t ip_tos FIELD_PACKED;        /* type of service */
    uint16_t ip_len FIELD_PACKED;       /* total length */
    uint16_t ip_id FIELD_PACKED;        /* identification */
    int16_t ip_off FIELD_PACKED;        /* fragment offset field */
#define IP_DF 0x4000                    /* dont fragment flag */
#define IP_MF 0x2000                    /* more fragments flag */
#define IP_OFFMASK 0x1fff               /* mask for fragmenting bits */
    uint8_t ip_ttl FIELD_PACKED;        /* time to live */
    uint8_t ip_p FIELD_PACKED;          /* protocol */
    uint16_t ip_sum FIELD_PACKED;       /* checksum */
    uint8_t ip_src[4] FIELD_PACKED;     /* source and dest address */
    uint8_t ip_dst[4] FIELD_PACKED;
} POSTPACK iphdr;

static uint8_t wmi_determine_userPriority(uint8_t *pkt, uint32_t layer2Pri)
{
    uint8_t ipPri;
    iphdr *ipHdr = (iphdr *)pkt;

    /* Determine IPTOS priority */
    /*
     * IP Tos format :
     *      (Refer Pg 57 WMM-test-plan-v1.2)
     * IP-TOS - 8bits
     *          : DSCP(6-bits) ECN(2-bits)
     *          : DSCP - P2 P1 P0 X X X
     *              where (P2 P1 P0) form 802.1D
     */
    ipPri = ipHdr->ip_tos >> 5;
    ipPri &= 0x7;

    if ((layer2Pri & 0x7) > ipPri)
        return ((uint8_t)layer2Pri & 0x7);
    else
        return ipPri;
}

uint8_t wmi_implicit_create_pstream(struct wmi_t *wmip, void *osbuf, uint32_t layer2Priority, boolean wmmEnabled)
{
    uint8_t *datap;
    uint8_t trafficClass = WMM_AC_BE;
    uint16_t ipType = IP_ETHERTYPE;
    WMI_DATA_HDR *dtHdr;
    uint8_t userPriority;
    uint32_t metasize;
    ATH_LLC_SNAP_HDR *llcHdr;

    // WMI_CREATE_PSTREAM_CMD  cmd;
    UNUSED_ARGUMENT(wmip);

    A_ASSERT(osbuf != NULL);

    datap = A_NETBUF_DATA(osbuf);
    dtHdr = (WMI_DATA_HDR *)datap;
    metasize = (WMI_DATA_HDR_GET_META(dtHdr)) ? WMI_MAX_TX_META_SZ : 0;

    if (!wmmEnabled)
    {
        /* If WMM is disabled all traffic goes as BE traffic */
        userPriority = 0;
    }
    else
    {
        llcHdr = (ATH_LLC_SNAP_HDR *)(datap + sizeof(WMI_DATA_HDR) + metasize + sizeof(ATH_MAC_HDR));

        if (llcHdr->etherType == A_CPU2BE16(ipType))
        {
            /* Extract the endpoint info from the TOS field in the IP header */

            userPriority = wmi_determine_userPriority(((uint8_t *)llcHdr) + sizeof(ATH_LLC_SNAP_HDR), layer2Priority);
        }
        else
        {
            userPriority = layer2Priority & 0x7;
        }
    }

    trafficClass = (up_to_ac[userPriority & 0x7]);

    WMI_DATA_HDR_SET_UP(dtHdr, userPriority);

    return trafficClass;
}

/*
 *  performs 802.3 to DIX encapsulation for received packets.
 *  Assumes the entire 802.3 header is contigous.
 */
A_STATUS
wmi_dot3_2_dix(void *osbuf)
{
    uint8_t *datap;
    ATH_MAC_HDR macHdr;
    ATH_LLC_SNAP_HDR *llcHdr;

    A_ASSERT(osbuf != NULL);
    datap = A_NETBUF_DATA(osbuf);

    A_MEMCPY(&macHdr, datap, sizeof(ATH_MAC_HDR));
    llcHdr = (ATH_LLC_SNAP_HDR *)(datap + sizeof(ATH_MAC_HDR));
    macHdr.typeOrLen = llcHdr->etherType;

    if (A_NETBUF_PULL(osbuf, sizeof(ATH_LLC_SNAP_HDR)) != A_OK)
    {
        return A_NO_MEMORY;
    }

    datap = A_NETBUF_DATA(osbuf);

    A_MEMCPY(datap, &macHdr, sizeof(ATH_MAC_HDR));

    return (A_OK);
}

/*
 * Removes a WMI data header
 */
A_STATUS
wmi_data_hdr_remove(struct wmi_t *wmip, void *osbuf)
{
    UNUSED_ARGUMENT(wmip);
    A_ASSERT(osbuf != NULL);

    return (A_NETBUF_PULL(osbuf, sizeof(WMI_DATA_HDR)));
}

static A_STATUS wmi_test_ev_length(uint16_t id, int32_t len)
{
    uint16_t i;
    const struct
    {
        uint16_t id;
        uint16_t len;
    } length_array[] = {{WMI_BSSINFO_EVENTID, sizeof(WMI_BSS_INFO_HDR)},
                        {WMI_CONNECT_EVENTID, sizeof(WMI_CONNECT_EVENT)},
                        {WMI_GET_CHANNEL_LIST_CMDID, sizeof(WMI_CHANNEL_LIST_REPLY)},
                        {WMI_READY_EVENTID, sizeof(WMI_READY_EVENT)},
                        {WMI_DISCONNECT_EVENTID, sizeof(WMI_DISCONNECT_EVENT)},
                        {WMI_BSSINFO_EVENTID, sizeof(WMI_BSS_INFO_HDR)},
                        {WMI_REGDOMAIN_EVENTID, sizeof(WMI_REG_DOMAIN_EVENT)},
                        {WMI_GET_PMK_EVENTID, WMI_PMK_LEN},
                        {WMI_TKIP_MICERR_EVENTID, sizeof(WMI_TKIP_MICERR_EVENT)},
                        {WMI_PEER_NODE_EVENTID, sizeof(WMI_PEER_NODE_EVENT)}};

    for (i = 0; i < sizeof(length_array) / sizeof(length_array[0]); i++)
    {
        if (id == length_array[i].id)
        {
            if (len < length_array[i].len)
            {
                return A_ERROR;
            }

            return A_OK;
        }
    }

    return A_OK; // could not find entry for eventID
}

/*
 * WMI Extended Event received from Target.
 */
A_STATUS
wmi_control_rx_xtnd(struct wmi_t *wmip, uint8_t devId, void *osbuf)
{
    WMIX_CMD_HDR *cmd;
    uint16_t id;
    uint8_t *datap;
    uint32_t len;
    A_STATUS status = A_OK;

    if (A_NETBUF_LEN(osbuf) < sizeof(WMIX_CMD_HDR))
    {
        A_DPRINTF(DBG_WMI, (DBGFMT "bad packet 1\n", DBGARG));
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    cmd = (WMIX_CMD_HDR *)A_NETBUF_DATA(osbuf);
    id = cmd->commandId;

    if (A_NETBUF_PULL(osbuf, sizeof(WMIX_CMD_HDR)) != A_OK)
    {
        A_DPRINTF(DBG_WMI, (DBGFMT "bad packet 2\n", DBGARG));
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    datap = A_NETBUF_DATA(osbuf);
    len = A_NETBUF_LEN(osbuf);

    switch (id)
    {
        case (WMIX_DBGLOG_EVENTID):
            A_WMIX_DBGLOG_EVENT(wmip, devId, datap, len);
            break;
        case (WMIX_GPIO_DATA_EVENTID):
            A_WMIX_GPIO_DATA_EVENT(wmip, devId, datap, len);
            break;

#if ENABLE_KF_PERFORMANCE
        case (WMIX_PFM_DATA_EVENTID):
            A_WMIX_PFM_DATA_EVENT(wmip, devId, datap, len);
            break;
        case (WMIX_PFM_DATA_DONE_EVENTID):
            A_WMIX_PFM_DATA_DONE_EVENT(wmip, devId, datap, len);
            break;
#endif

        case (WMIX_HB_CHALLENGE_RESP_EVENTID):
          WMIX_HB_CHALLENGE_RESP_RX(wmip->wmi_devt, devId, datap, len);
          break;

        default:
            A_DPRINTF(DBG_WMI | DBG_ERROR, (DBGFMT "Unknown id 0x%x\n", DBGARG, id));
            wmip->wmi_stats.cmd_id_err++;
            status = A_ERROR;
            break;
    }

    return status;
}

/*
 * Control Path
 */

A_STATUS
wmi_control_rx(struct wmi_t *wmip, void *osbuf)
{
    // printf("wmi_control_rx.\n");
    WMI_CMD_HDR *cmd;
    uint16_t id;
    uint8_t *datap;
    int32_t len;
    A_STATUS status = A_OK;
    uint8_t spareBuf = 0;
    WMI_BSS_INFO_HDR2 bih2;
    WMI_BSS_INFO_HDR *bih;
    union
    {
        void *void_ev; // used for initialization only
        WMI_CONNECT_EVENT *conn_ev;
        WMI_CHANNEL_LIST_REPLY *chan_ev;
        WMI_READY_EVENT *ready_ev;
        WMI_DISCONNECT_EVENT *disc_ev;
        WMI_REG_DOMAIN_EVENT *regdom_ev;
        WMI_SCAN_COMPLETE_EVENT *scan_comp_ev;
        WMI_TKIP_MICERR_EVENT *tkip_err_ev;
        WMI_PEER_NODE_EVENT *peer_ev;
    } stackU;
    uint8_t devId;

#define VOID_EV (stackU.void_ev)
#define CONN_EV (stackU.conn_ev)
#define CHAN_EV (stackU.chan_ev)
#define READY_EV (stackU.ready_ev)
#define DISC_EV (stackU.disc_ev)
#define REG_EV (stackU.regdom_ev)
#define SCAN_EV (stackU.scan_comp_ev)
#define TKIP_EV (stackU.tkip_err_ev)
#define PEER_EV (stackU.peer_ev)

    A_ASSERT(osbuf != NULL);
    if (A_NETBUF_LEN(osbuf) < sizeof(WMI_CMD_HDR))
    {
        A_NETBUF_FREE(osbuf);
        A_DPRINTF(DBG_WMI, (DBGFMT "bad packet 1\n", DBGARG));
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    cmd = (WMI_CMD_HDR *)A_NETBUF_DATA(osbuf);
    id = A_LE2CPU16(cmd->commandId);

    cmd->info1 = A_LE2CPU16(cmd->info1);
    devId = WMI_GET_DEVICE_ID(cmd->info1);

    if (A_NETBUF_PULL(osbuf, sizeof(WMI_CMD_HDR)) != A_OK)
    {
        A_NETBUF_FREE(osbuf);
        A_DPRINTF(DBG_WMI, (DBGFMT "bad packet 2\n", DBGARG));
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    datap = A_NETBUF_DATA(osbuf);
    VOID_EV = datap;
    len = (int32_t)A_NETBUF_LEN(osbuf);

    if (A_OK != wmi_test_ev_length(id, len))
    {
        A_ASSERT(0);
    }

    WMI_DEBUG_RX_EVENT(id, osbuf);

    switch (id)
    {
        case (WMI_GET_CHANNEL_LIST_CMDID):
            A_WMI_CHANNELLIST_RX(wmip->wmi_devt, devId, (int8_t)CHAN_EV->numChannels, (void*)CHAN_EV->channelList);
            break;
        case (WMI_GET_BITRATE_CMDID):
        {
            A_WMI_GET_BITRATE_EVEVT(wmip->wmi_devt, devId, datap);
        }
        break;
        case (WMI_GET_TEMPERATURE_REPLY_EVENTID):
        {
            A_WMI_GET_TEMPERATURE_REPLY(wmip->wmi_devt, datap, len);
        };
        break;
        case (WMI_GET_COUNTRY_CODE_REPLY_EVENTID):
        {
            //   printf("wmi code reply.\n");
            A_WMI_GET_COUNTRY_CODE_REPLY(wmip->wmi_devt, datap, len);
        };
        break;
        case (WMI_PARAM_SET_REPLY_EVENTID):
            A_WMI_SET_PARAM_REPLY(wmip->wmi_devt, datap);
            break;
        case (WMI_READY_EVENTID):
            A_WMI_READY_EVENT(wmip->wmi_devt, devId, READY_EV->macaddr, READY_EV->phyCapability,
                              A_LE2CPU32(READY_EV->sw_version), A_LE2CPU32(READY_EV->abi_version));
            A_WMI_SEND_EVENT_TO_APP(wmip->wmi_devt, id, datap, len);
            break;
        case (WMI_CONNECT_EVENTID):
            // convert for native endianness
            CONN_EV->channel = A_LE2CPU16(stackU.conn_ev->channel);
            CONN_EV->listenInterval = A_LE2CPU16(CONN_EV->listenInterval);
            CONN_EV->beaconInterval = A_LE2CPU16(CONN_EV->beaconInterval);
            CONN_EV->networkType = A_LE2CPU32(CONN_EV->networkType);
            A_WMI_CONNECT_EVENT(wmip->wmi_devt, devId, CONN_EV->channel, CONN_EV->bssid, CONN_EV->listenInterval,
                                CONN_EV->beaconInterval, (NETWORK_TYPE)CONN_EV->networkType, CONN_EV->beaconIeLen,
                                CONN_EV->assocReqLen, CONN_EV->assocRespLen, CONN_EV->assocInfo);

            A_WMI_SEND_GENERIC_EVENT_TO_APP(wmip->wmi_devt, id, datap, len);
            break;
        case (WMI_DISCONNECT_EVENTID):
            A_WMI_DISCONNECT_EVENT(wmip->wmi_devt, devId, DISC_EV->disconnectReason, DISC_EV->bssid,
                                   DISC_EV->assocRespLen, DISC_EV->assocInfo,
                                   A_LE2CPU16(DISC_EV->protocolReasonStatus));
            A_WMI_SEND_EVENT_TO_APP(wmip->wmi_devt, id, datap, len);
            break;
        case (WMI_PEER_NODE_EVENTID):
            /* this event will be sent from device to indicate 4 way success
             */
            // status = wmi_rsna_4way_rx(wmip, datap, len);
            if (PEER_EV->eventCode == PEER_NODE_JOIN_EVENT)
            {
                /* need to call an custom event which will give a callback
                 * to the application this event will indicate to application
                 * 4 way handshake success from the firmware
                 */
                A_WMI_RSNA_SUCCESS_EVENT(wmip->wmi_devt, devId, PEER_FIRST_NODE_JOIN_EVENT);
            }
            break;
        case (WMI_BSSINFO_EVENTID):
            A_DPRINTF(DBG_WMI, (DBGFMT "WMI_BSSINFO_EVENTID\n", DBGARG));
            {
                /*
                 * convert WMI_BSS_INFO_HDR2 to WMI_BSS_INFO_HDR
                 * Take a local copy of the WMI_BSS_INFO_HDR2 from the wmi buffer
                 * and reconstruct the WMI_BSS_INFO_HDR in its place
                */
                A_MEMCPY(&bih2, datap, sizeof(WMI_BSS_INFO_HDR2));

                A_NETBUF_PUSH(osbuf, 4);
                datap = A_NETBUF_DATA(osbuf);
                len = (int32_t)A_NETBUF_LEN(osbuf);
                bih = (WMI_BSS_INFO_HDR *)datap;

                bih->channel = A_LE2CPU16(bih2.channel);
                bih->frameType = bih2.frameType;
                bih->snr = bih2.snr;
                bih->rssi = (int16_t)(bih2.snr - 95);
                bih->ieMask = A_LE2CPU16(bih2.ieMask);
                A_MEMCPY(bih->bssid, bih2.bssid, ATH_MAC_LEN);

                if (bih->rssi <= 0)
                {
                    A_WMI_BSSINFO_EVENT_RX(wmip->wmi_devt, devId, datap, len);
                    A_WMI_SEND_GENERIC_EVENT_TO_APP(wmip->wmi_devt, id, datap, len);
                }
            }
            break;
        case (WMI_REGDOMAIN_EVENTID):
            A_WMI_REGDOMAIN_EVENT(wmip->wmi_devt, devId, A_LE2CPU32(REG_EV->regDomain));
            break;
        case (WMI_SCAN_COMPLETE_EVENTID):
            A_WMI_SCANCOMPLETE_EVENT(wmip->wmi_devt, devId, (A_STATUS)A_LE2CPU32(SCAN_EV->status));
            A_WMI_SEND_EVENT_TO_APP(wmip->wmi_devt, id, datap, len);
            break;
        case (WMI_CMDERROR_EVENTID):
            status = wmi_errorEvent_rx(wmip, devId, datap, len);
            break;
        case (WMI_REPORT_STATISTICS_EVENTID):
            A_WMI_TARGETSTATS_EVENT(wmip->wmi_devt, devId, datap, (uint32_t)len);
            break;
        case (WMI_EXTENSION_EVENTID):
            status = wmi_control_rx_xtnd(wmip, devId, osbuf);
            break;
        case (WMI_GET_PMK_EVENTID):
            A_WMI_GET_PMK_RX(wmip->wmi_devt, devId, datap);
            break;
        case (WMI_STORERECALL_STORE_EVENTID):
            A_WMI_STORERECALL_EVENT_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;

        /*  new store-recall event */
        case (WMI_STORERECALL_START_EVENTID):
            A_WMI_STORERECALL_START_EVENT_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_HOST_DSET_STORE_EVENTID):
            A_WMI_HOST_DSET_EVENT_STORE_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_HOST_DSET_READ_EVENTID):
            A_WMI_HOST_DSET_EVENT_READ_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_HOST_DSET_CREATE_EVENTID):
            A_WMI_HOST_DSET_EVENT_CREATE_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_HOST_DSET_LARGE_WRITE_EVENTID):
            A_WMI_HOST_DSET_EVENT_WRITE_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_HOST_DSET_LARGE_READ_EVENTID):
            A_WMI_HOST_DSET_EVENT_READBACK_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_HOST_DSET_SYNC_EVENTID):
            A_WMI_HOST_DSET_EVENT_SYNC_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        case (WMI_DSET_OP_EVENTID):
            A_WMI_DSET_EVENT_OP_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            break;
        /* */

        case (WMI_WPS_PROFILE_EVENTID):
            A_WMI_WPS_PROFILE_EVENT_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            spareBuf = 1; /* prevent buffer from A_NETBUF_FREE() below */
            break;
#if WLAN_CONFIG_11N_AGGR_SUPPORT
        case (WMI_ADDBA_REQ_EVENTID):
            A_WMI_AGGR_RECV_ADDBA_REQ_EVT(wmip->wmi_devt, devId, (WMI_ADDBA_REQ_EVENT *)datap);
            break;
        case (WMI_DELBA_REQ_EVENTID):
            A_WMI_AGGR_RECV_DELBA_REQ_EVT(wmip->wmi_devt, devId, (WMI_DELBA_EVENT *)datap);
            break;
#endif
        case (WMI_TKIP_MICERR_EVENTID):
            A_WMI_TKIP_MICERR_EVENT(wmip->wmi_devt, devId, TKIP_EV->keyid, TKIP_EV->ismcast);
            break;
        case (WMI_SOCKET_RESPONSE_EVENTID):
            status = A_WMI_SOCK_RESPONSE_EVENT_RX(wmip->wmi_devt, devId, datap, len, osbuf);
            spareBuf = 1; /* prevent buffer from A_NETBUF_FREE() below */
            break;

#if ENABLE_P2P_MODE
        case WMI_P2P_LIST_PERSISTENT_NETWORK_EVENTID:
            status = wmi_p2p_list_peristent_network(wmip, devId, datap, len);
            break;
        case WMI_P2P_GO_NEG_RESULT_EVENTID:
            status = wmi_p2p_goneg_result_event_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_NODE_LIST_EVENTID:
            status = wmi_p2p_node_list_event_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_REQ_TO_AUTH_EVENTID:
            status = wmi_p2p_req_auth_event_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_PROV_DISC_RESP_EVENTID:
            status = wmi_p2p_prov_disc_resp_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_PROV_DISC_REQ_EVENTID:
            status = wmi_p2p_prov_disc_req_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_INVITE_REQ_EVENTID:
            status = wmi_p2p_invite_req_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_INVITE_RCVD_RESULT_EVENTID:
            status = wmi_p2p_invite_rcvd_result_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_INVITE_SENT_RESULT_EVENTID:
            status = wmi_p2p_invite_sent_result_rx(wmip, devId, datap, len);
            break;

        case WMI_P2P_SDPD_RX_EVENTID:
            status = wmi_p2p_sdpd_event_rx(wmip, devId, datap, len);
            break;

#endif
#if MANUFACTURING_SUPPORT
        case WMI_TEST_EVENTID:
            status = wmi_test_event_rx(wmip, devId, datap, len);
            break;
#endif
        case WMI_RX_PROBE_REQ_EVENTID:
            A_WMI_PROBE_REQ_EVENT_RX(wmip->wmi_devt, datap, len);
            break;

        case WMI_WLAN_WPS_INIT_KEY_EVENTID:
            A_WMI_WLAN_WPS_INIT_REPLY_RX(wmip->wmi_devt, datap, len);
            break;

        default:
            if (A_OK != (status = A_WMI_EVENT(id, wmip->wmi_devt, datap, len, osbuf)))
            {
                A_DPRINTF(DBG_WMI | DBG_ERROR, (DBGFMT "Unknown id 0x%x\n", DBGARG, id));
                wmip->wmi_stats.cmd_id_err++;
                status = A_ERROR;
            }
            break;
    }
    /* spareBuf != 0 if the packet handler needed to keep the buffer.
     * it becomes the handler's responsibility to free it.
     */
    if (spareBuf == 0)
    {
        A_NETBUF_FREE(osbuf);
    }

    return status;
}

/*
 * Target is reporting a programming error.  This is for
 * developer aid only.  Target only checks a few common violations
 * and it is responsibility of host to do all error checking.
 * Behavior of target after wmi error event is undefined.
 * A reset is recommended.
 */
static A_STATUS wmi_errorEvent_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, int len)
{
    WMI_CMD_ERROR_EVENT *ev;
    UNUSED_ARGUMENT(wmip);
    UNUSED_ARGUMENT(len);
    UNUSED_ARGUMENT(devId);
    ev = (WMI_CMD_ERROR_EVENT *)datap;
    AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("Programming Error: cmd=%d ", ev->commandId));
    switch (ev->errorCode)
    {
        case (INVALID_PARAM):
            AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("Illegal Parameter\n"));
            break;
        case (ILLEGAL_STATE):
            AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("Illegal State\n"));
            break;
        case (INTERNAL_ERROR):
            AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("Internal Error\n"));
            break;
        case (STACK_ERROR):
            AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("TCP/IP stack not present on Target\n"));
            A_ASSERT(0);
    }

    return A_OK;
}

/*
 * Called to send a wmi command. Command specific data is already built
 * on osbuf and current osbuf->data points to it.
 */
A_STATUS
wmi_cmd_send(struct wmi_t *wmip, void *osbuf, WMI_COMMAND_ID cmdId, WMI_SYNC_FLAG syncflag)
{
    A_STATUS status = A_OK;
    WMI_CMD_HDR *cHdr;
    HTC_ENDPOINT_ID eid = wmip->wmi_endpoint_id;

    A_ASSERT(osbuf != NULL);

    if (syncflag >= END_WMIFLAG)
    {
        A_NETBUF_FREE(osbuf);
        return A_EINVAL;
    }

    WMI_DEBUG_TX_COMMAND(cmdId, osbuf);

#if 0
         WMI_SOCKET_CMD *cmd;
	cmd = (WMI_SOCKET_CMD *)A_NETBUF_DATA(osbuf);
	printf("cmd type is %d.\n",cmd->cmd_type);
#endif

#if WLAN_CONFIG_ENABLE_WMI_SYNC
    if ((syncflag == SYNC_BEFORE_WMIFLAG) || (syncflag == SYNC_BOTH_WMIFLAG))
    {
        /*
         * We want to make sure all data currently queued is transmitted before
         * the cmd execution.  Establish a new sync point.
         */
        wmi_sync_point(wmip);
    }
#endif

    if (A_NETBUF_PUSH(osbuf, sizeof(WMI_CMD_HDR)) != A_OK)
    {
        A_NETBUF_FREE(osbuf);
        return A_NO_MEMORY;
    }

    cHdr = (WMI_CMD_HDR *)A_NETBUF_DATA(osbuf);
    cHdr->commandId = A_CPU2LE16((uint16_t)cmdId);

    WMI_SET_DEVICE_ID(cHdr, wmip->deviceid);
    cHdr->info1 = A_CPU2LE16(cHdr->info1); // added for virtual interface

    A_WMI_CONTROL_TX(wmip->wmi_devt, osbuf, eid);

#if WLAN_CONFIG_ENABLE_WMI_SYNC
    if ((syncflag == SYNC_AFTER_WMIFLAG) || (syncflag == SYNC_BOTH_WMIFLAG))
    {
        /*
         * We want to make sure all new data queued waits for the command to
         * execute. Establish a new sync point.
         */
        wmi_sync_point(wmip);
    }
#endif

    return (status);
}

#if ENABLE_AP_MODE
A_STATUS
wmi_ap_set_param(struct wmi_t *wmip, void *data)
{
    ATH_AP_PARAM_STRUCT *ap_param = (ATH_AP_PARAM_STRUCT *)data;
    union
    {
        uint8_t hidden_ssid_flag;
        WMI_AP_CONN_INACT_CMD conn_inact_cmd;
        WMI_BEACON_INT_CMD beacon_int_cmd;
        WMI_AP_SET_DTIM_CMD set_dtim_cmd;
        WMI_AP_PSBUF_OFFLOAD_CMD psbuf_offload_cmd;
    } stackU;

    switch (ap_param->cmd_subset)
    {
        case AP_SUB_CMD_HIDDEN_FLAG:
        {
            stackU.hidden_ssid_flag = HIDDEN_SSID_TRUE;
            return (wmi_cmd_start(wmip, &stackU.hidden_ssid_flag, WMI_AP_HIDDEN_SSID_CMDID,
                                  sizeof(WMI_AP_HIDDEN_SSID_CMD)));
        }

        case AP_SUB_CMD_INACT_TIME:
        {
            A_MEMZERO(&stackU.conn_inact_cmd, sizeof(WMI_AP_CONN_INACT_CMD));
            stackU.conn_inact_cmd.period = A_CPU2LE32(*(uint32_t *)(ap_param->data));
            return (
                wmi_cmd_start(wmip, &stackU.conn_inact_cmd, WMI_AP_CONN_INACT_CMDID, sizeof(WMI_AP_CONN_INACT_CMD)));
        }

        case AP_SUB_CMD_BCON_INT:
        {
            A_MEMZERO(&stackU.beacon_int_cmd, sizeof(WMI_BEACON_INT_CMD));
            stackU.beacon_int_cmd.beaconInterval = A_CPU2LE16((*(uint16_t *)(ap_param->data)));
            return (wmi_cmd_start(wmip, &stackU.conn_inact_cmd, WMI_SET_BEACON_INT_CMDID, sizeof(WMI_BEACON_INT_CMD)));
        }

        case AP_SUB_CMD_DTIM_INT:
        {
            A_MEMZERO(&stackU.set_dtim_cmd, sizeof(WMI_AP_SET_DTIM_CMD));
            stackU.set_dtim_cmd.dtim = *(uint8_t *)(ap_param->data);
            return (wmi_cmd_start(wmip, &stackU.conn_inact_cmd, WMI_AP_SET_DTIM_CMDID, sizeof(WMI_AP_SET_DTIM_CMD)));
        }
        case AP_SUB_CMD_PSBUF:
        {
            A_MEMZERO(&stackU.psbuf_offload_cmd, sizeof(WMI_AP_PSBUF_OFFLOAD_CMD));
            stackU.psbuf_offload_cmd.enable = ((uint8_t *)ap_param->data)[0];
            stackU.psbuf_offload_cmd.psBufCount = ((uint8_t *)ap_param->data)[1];
            return (wmi_cmd_start(wmip, &stackU.conn_inact_cmd, WMI_AP_PSBUF_OFFLOAD_CMDID,
                                  sizeof(WMI_AP_PSBUF_OFFLOAD_CMD)));
        }
        default:
            break;
    }

    return A_OK;
}
#endif

#if WLAN_CONFIG_ENABLE_WMI_SYNC

A_STATUS
wmi_dataSync_send(struct wmi_t *wmip, void *osbuf, HTC_ENDPOINT_ID eid)
{
    WMI_DATA_HDR *dtHdr;

    A_ASSERT(eid != wmip->wmi_endpoint_id);
    A_ASSERT(osbuf != NULL);

    if (A_NETBUF_PUSH(osbuf, sizeof(WMI_DATA_HDR)) != A_OK)
    {
        return A_NO_MEMORY;
    }

    dtHdr = (WMI_DATA_HDR *)A_NETBUF_DATA(osbuf);
    dtHdr->info = (SYNC_MSGTYPE & WMI_DATA_HDR_MSG_TYPE_MASK) << WMI_DATA_HDR_MSG_TYPE_SHIFT;

    A_DPRINTF(DBG_WMI, (DBGFMT "Enter - eid %d\n", DBGARG, eid));

    return (A_WMI_CONTROL_TX(wmip->wmi_devt, osbuf, eid));
}

typedef struct _WMI_DATA_SYNC_BUFS
{
    uint8_t trafficClass;
    void *osbuf;
} WMI_DATA_SYNC_BUFS;

static A_STATUS wmi_sync_point(struct wmi_t *wmip)
{
    void *cmd_osbuf;
    WMI_SYNC_CMD *cmd;
    WMI_DATA_SYNC_BUFS dataSyncBufs[WMM_NUM_AC];
    uint8_t i, numPriStreams = 0;
    A_STATUS status = A_OK;

    A_DPRINTF(DBG_WMI, (DBGFMT "Enter\n", DBGARG));

    memset(dataSyncBufs, 0, sizeof(dataSyncBufs));

    /* lock out while we walk through the priority list and assemble our local array */
    LOCK_WMI(wmip);

    for (i = 0; i < WMM_NUM_AC; i++)
    {
        if (wmip->wmi_fatPipeExists & (1 << i))
        {
            numPriStreams++;
            dataSyncBufs[numPriStreams - 1].trafficClass = i;
        }
    }

    UNLOCK_WMI(wmip);

    /* dataSyncBufs is now filled with entries (starting at index 0) containing valid streamIDs */

    do
    {
        /*
         * We allocate all network buffers needed so we will be able to
         * send all required frames.
         */
        cmd_osbuf = A_NETBUF_ALLOC(sizeof(*cmd));
        if (cmd_osbuf == NULL)
        {
            status = A_NO_MEMORY;
            break;
        }

        A_NETBUF_PUT(cmd_osbuf, sizeof(*cmd));

        cmd = (WMI_SYNC_CMD *)(A_NETBUF_DATA(cmd_osbuf));
        A_MEMZERO(cmd, sizeof(*cmd));

        /* In the SYNC cmd sent on the control Ep, send a bitmap of the data
         * eps on which the Data Sync will be sent
         */
        cmd->dataSyncMap = wmip->wmi_fatPipeExists;

        for (i = 0; i < numPriStreams; i++)
        {
            dataSyncBufs[i].osbuf = A_NETBUF_ALLOC(0);
            if (dataSyncBufs[i].osbuf == NULL)
            {
                status = A_NO_MEMORY;
                break;
            }
        } // end for

        /* if Buffer allocation for any of the dataSync fails, then do not
         * send the Synchronize cmd on the control ep
         */
        if (A_FAILED(status))
        {
            break;
        }

        /*
         * Send sync cmd followed by sync data messages on all endpoints being
         * used
         */
        status = wmi_cmd_send(wmip, cmd_osbuf, WMI_SYNCHRONIZE_CMDID, NO_SYNC_WMIFLAG);

        if (A_FAILED(status))
        {
            break;
        }
        /* cmd buffer sent, we no longer own it */
        cmd_osbuf = NULL;

        for (i = 0; i < numPriStreams; i++)
        {
            A_ASSERT(dataSyncBufs[i].osbuf != NULL);
            status = wmi_dataSync_send(wmip, dataSyncBufs[i].osbuf,
                                       A_WMI_Ac2EndpointID(wmip->wmi_devt, dataSyncBufs[i].trafficClass));

            if (A_FAILED(status))
            {
                break;
            }
            /* we don't own this buffer anymore, NULL it out of the array so it
             * won't get cleaned up */
            dataSyncBufs[i].osbuf = NULL;
        } // end for

    } while (false);

    /* free up any resources left over (possibly due to an error) */

    if (cmd_osbuf != NULL)
    {
        A_NETBUF_FREE(cmd_osbuf);
    }

    for (i = 0; i < numPriStreams; i++)
    {
        if (dataSyncBufs[i].osbuf != NULL)
        {
            A_NETBUF_FREE(dataSyncBufs[i].osbuf);
        }
    }

    return (status);
}
#endif /* WLAN_CONFIG_ENABLE_WMI_SYNC */

A_STATUS
wmi_storerecall_recall_cmd(struct wmi_t *wmip, uint32_t length, void *pData)
{
    void *osbuf;
    WMI_STORERECALL_RECALL_CMD *p;
    uint32_t msgLength = sizeof(WMI_STORERECALL_RECALL_CMD) - sizeof(uint8_t);

    osbuf = A_NETBUF_ALLOC((int32_t)msgLength);
    if (osbuf == NULL)
    {
        return A_NO_MEMORY;
    }

    A_NETBUF_PUT(osbuf, (int32_t)msgLength);

    p = (WMI_STORERECALL_RECALL_CMD *)(A_NETBUF_DATA(osbuf));

    p->length = A_CPU2LE32(length);
    // A_MEMCPY(&p->data[0], pData, length);
    /* here we append the payload to the msg rather than copy it. this is a
     * memory and CPU optimization as the payload can be very large.
     */
    A_NETBUF_APPEND_FRAGMENT(osbuf, pData, length);

    return (wmi_cmd_send(wmip, osbuf, WMI_STORERECALL_RECALL_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_bssfilter_cmd(struct wmi_t *wmip, uint8_t filter, uint32_t ieMask)
{
    WMI_BSS_FILTER_CMD bssFilterParam = {0};

    bssFilterParam.bssFilter = filter;
    bssFilterParam.ieMask = A_CPU2LE32(ieMask);

    return wmi_cmd_start(wmip, &bssFilterParam, WMI_SET_BSS_FILTER_CMDID, sizeof(bssFilterParam));
}

#if ENABLE_STACK_OFFLOAD
A_STATUS wmi_socket_cmd(struct wmi_t *wmip, uint32_t cmd_type, void *pData, uint32_t length)
{
    WMI_SOCKET_CMD *cmd;

    if (pData == NULL || length < sizeof(WMI_SOCKET_CMD))
    {
        // Should not happen
        A_ASSERT(0);
    }

    cmd = (WMI_SOCKET_CMD *)pData;
    cmd->cmd_type = A_CPU2LE32(cmd_type);
    cmd->length = A_CPU2LE32((length - sizeof(WMI_SOCKET_CMD)));

    return (wmi_cmd_start(wmip, pData, WMI_SOCKET_CMDID, length));
}
#endif

#if MANUFACTURING_SUPPORT
static A_STATUS wmi_test_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint32_t len)
{
    UNUSED_ARGUMENT(devId);
    A_WMI_TEST_CMD_EVENT_RX(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}
#endif

#if ENABLE_P2P_MODE

static A_STATUS wmi_p2p_goneg_result_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    // A_WMI_P2P_GO_NEG_EVENT(wmip->wmi_devt, datap, len);
    A_WMI_P2P_GO_NEG_EVENT(wmip->wmi_devt, devId, datap, len, &p2p_key_val);
    return A_OK;
}

static A_STATUS wmi_p2p_list_peristent_network(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_P2P_PERSISTENT_LIST_NETWORK_EVENT(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

static A_STATUS wmi_p2p_req_auth_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint32_t len)
{
    A_WMI_P2P_REQ_AUTH_EVENT(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

static A_STATUS wmi_p2p_node_list_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint32_t len)
{
    A_WMI_P2P_NODE_LIST_EVENT(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

static A_STATUS wmi_p2p_prov_disc_resp_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_GET_P2P_CTX(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

static A_STATUS wmi_p2p_prov_disc_req_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_P2P_PROV_DISC_REQ(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}
#if 1 // KK
static A_STATUS wmi_p2p_sdpd_event_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_P2P_SERV_DISC_REQ(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

static A_STATUS wmi_p2p_invite_req_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_P2P_INVITE_REQ_RX(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

static A_STATUS wmi_p2p_invite_rcvd_result_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_P2P_INVITE_RCVD_RESULT_EVENT(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}
#endif

static A_STATUS wmi_p2p_invite_sent_result_rx(struct wmi_t *wmip, uint8_t devId, uint8_t *datap, uint8_t len)
{
    A_WMI_P2P_INVITE_SENT_RESULT_EVENT(wmip->wmi_devt, devId, datap, len);
    return A_OK;
}

A_STATUS wmi_sdpd_send_cmd(struct wmi_t *wmip, WMI_P2P_SDPD_TX_CMD *buf)
{
    void *osbuf;
    WMI_P2P_SDPD_TX_CMD *sdpd_tx_cmd;
    A_STATUS status;

    osbuf = A_NETBUF_ALLOC(sizeof(WMI_P2P_SDPD_TX_CMD));
    if (osbuf == NULL)
    {
        return A_NO_MEMORY;
    }

    A_NETBUF_PUT(osbuf, sizeof(WMI_P2P_SDPD_TX_CMD));

    sdpd_tx_cmd = (WMI_P2P_SDPD_TX_CMD *)(A_NETBUF_DATA(osbuf));
    A_MEMZERO(sdpd_tx_cmd, sizeof(WMI_P2P_SDPD_TX_CMD));
    A_MEMCPY(sdpd_tx_cmd, buf, sizeof(WMI_P2P_SDPD_TX_CMD));
    sdpd_tx_cmd->freq = A_CPU2LE16(sdpd_tx_cmd->freq);
    sdpd_tx_cmd->status_code = A_CPU2LE16(sdpd_tx_cmd->status_code);
    sdpd_tx_cmd->comeback_delay = A_CPU2LE16(sdpd_tx_cmd->comeback_delay);
    sdpd_tx_cmd->tlv_length = A_CPU2LE16(sdpd_tx_cmd->tlv_length);
    sdpd_tx_cmd->update_indic = A_CPU2LE16(sdpd_tx_cmd->update_indic);
    sdpd_tx_cmd->total_length = A_CPU2LE16(sdpd_tx_cmd->total_length);

    status = wmi_cmd_send(wmip, osbuf, WMI_P2P_SDPD_TX_CMDID, NO_SYNC_WMIFLAG);

    if (status != A_OK)
    {
        A_NETBUF_FREE(osbuf);
    }

    return status;
}

A_STATUS
wmi_p2p_set_oppps(struct wmi_t *wmip, WMI_OPPPS_INFO_STRUCT *pOpp)
{
    WMI_OPPPS_INFO_STRUCT *cmd;
    void *osbuf;

    osbuf = A_NETBUF_ALLOC(sizeof(WMI_OPPPS_INFO_STRUCT));
    if (osbuf == NULL)
    {
        return A_NO_MEMORY;
    }

    A_NETBUF_PUT(osbuf, sizeof(WMI_OPPPS_INFO_STRUCT));
    cmd = (WMI_OPPPS_INFO_STRUCT *)(A_NETBUF_DATA(osbuf));

    cmd->enable = pOpp->enable;
    cmd->ctwin = pOpp->ctwin;

    return (wmi_cmd_send(wmip, osbuf, WMI_P2P_FW_SET_OPPPS_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS wmi_p2p_set_noa(struct wmi_t *wmip, WMI_NOA_INFO_STRUCT *noa)
{
    void *osbuf;
    WMI_NOA_INFO_STRUCT *p2p_set_noa_cmd;
    int i;
    A_STATUS status;

    osbuf = A_NETBUF_ALLOC(sizeof(WMI_NOA_INFO_STRUCT) - 1 + noa->count * sizeof(P2P_NOA_DESCRIPTOR));
    if (osbuf == NULL)
    {
        return A_NO_MEMORY;
    }

    A_NETBUF_PUT(osbuf, sizeof(WMI_NOA_INFO_STRUCT) - 1 + noa->count * sizeof(P2P_NOA_DESCRIPTOR));

    p2p_set_noa_cmd = (WMI_NOA_INFO_STRUCT *)(A_NETBUF_DATA(osbuf));

    p2p_set_noa_cmd->enable = noa->enable;
    p2p_set_noa_cmd->count = noa->count;

    for (i = 0; i < p2p_set_noa_cmd->count; i++)
    {
        A_MEMCPY(((P2P_NOA_DESCRIPTOR *)p2p_set_noa_cmd->noas) + i, ((P2P_NOA_DESCRIPTOR *)noa->noas) + i,
                 sizeof(P2P_NOA_DESCRIPTOR));
    }

    status = wmi_cmd_send(wmip, osbuf, WMI_P2P_FW_SET_NOA_CMDID, NO_SYNC_WMIFLAG);

    if (status != A_OK)
    {
        A_NETBUF_FREE(osbuf);
    }

    return status;
}

void wmi_save_key_info(WMI_P2P_PROV_INFO *p2p_info)
{
    A_MEMCPY(&p2p_key_val, p2p_info, sizeof(WMI_P2P_PROV_INFO));
}

#endif

A_STATUS
wmi_cmd_start(struct wmi_t *wmip, const void *pInput, WMI_COMMAND_ID cmdID, uint16_t buffSize)
{
    A_STATUS status = A_ERROR;
    void *osbuf;

    do
    {
        /* allocate a buffer for communication to the wifi device */
        if (NULL == (osbuf = A_NETBUF_ALLOC(buffSize)))
        {
            status = A_NO_MEMORY;
            break;
        }

        if (buffSize)
        {
            /* reserve the necessary space in the buffer */
            A_NETBUF_PUT(osbuf, buffSize);
            A_MEMCPY(A_NETBUF_DATA(osbuf), pInput, buffSize);
        }
        /* send the buffer to the wifi device */
        if (A_OK != (status = wmi_cmd_send(wmip, osbuf, cmdID, NO_SYNC_WMIFLAG)))
        {
            // No need to free the buffer. wmi_cmd_send is freeing the
            // buffer in case of error (the error condition is very rare).
            // Hence commenting this out.
            // A_NETBUF_FREE(osbuf); //wmi_cmd_send is freeing the buffer
        }
    } while (0);

    return status;
}

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)

uint8_t reverse_credits_init = 0;

A_STATUS wmi_reverse_credit_cmd(void *handle, boolean enable, uint8_t *endpoints, uint8_t *credits)
{
    A_STATUS status = A_ERROR;
    uint8_t i;

    WMI_INIT_REVERSE_CREDIT_CMD reverse_credits;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(handle);

    for (i = 0; i < 8; i++)
    {
        if (endpoints[i] == 0)
            break;
    }

    reverse_credits.num_credit_queues = i;
    reverse_credits.max_queued_buffers = 64; /// GET_TOTAL_BUFFERS();

    memcpy(reverse_credits.endpoint_mapping, endpoints, sizeof(reverse_credits.endpoint_mapping));
    memcpy(reverse_credits.start_credits_per_queue, credits, sizeof(reverse_credits.start_credits_per_queue));

    do
    {
        status = wmi_cmd_start(pDCxt->pWmiCxt, &reverse_credits, WMI_SET_CREDIT_REVERSE_CMDID,
                               sizeof(WMI_INIT_REVERSE_CREDIT_CMD));

        if (status == A_OK)
        {
            break;
        }
        else if (status == A_NO_MEMORY)
        {
            pDCxt->tx_complete_pend = true;

            if (A_OK != DRIVER_WAIT_FOR_CONDITION(p_Global_Cxt, &(pDCxt->tx_complete_pend), false, 5000))
            {
                A_ASSERT(0);
            }
        }
        else
        {
            A_ASSERT(0);
        }
    } while (1);

    Driver_WakeDriver(p_Global_Cxt);
    A_MDELAY(10);

    reverse_credits_init = 1;
}

#define CLASS_HEAD_LEN(a) (sizeof((a)->offset) + sizeof((a)->shift) + sizeof((a)->mask) + sizeof((a)->count))
#define CLASS_MAPPING_LEN(n) (sizeof(uint32_t) * n + n)

A_STATUS wmi_rcv_data_classifier_cmd(void *handle,
                                     uint8_t offset,
                                     uint8_t shift,
                                     uint32_t mask,
                                     uint8_t count,
                                     uint32_t *category_mapping,
                                     uint8_t *ep_mapping)
{
    A_STATUS status = A_ERROR;
    WMI_INIT_RCV_DATA_CLASSIFIER_CMD DataCls;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(handle);
    uint16_t len;

    if (count > 8)
        return status;
#if 0
    len = CLASS_HEAD_LEN(pDataCls) + CLASS_MAPPING_LEN(count);
	pDataCls = (WMI_INIT_RCV_DATA_CLASSIFIER_CMD *)A_MALLOC(len, MALLOC_ID_TEMPORARY);
    if (pDataCls == NULL)
        return  status;
#endif

    DataCls.offset = offset;
    DataCls.shift = shift;
    DataCls.mask = mask;
    DataCls.count = count;

    memcpy(DataCls.category_mapping_data, category_mapping, sizeof(uint32_t) * count);
    memcpy(DataCls.ep_mapping_data, ep_mapping, count);

    do
    {
        status = wmi_cmd_start(pDCxt->pWmiCxt, &DataCls, WMI_SET_RCV_DATA_CLASSIFIER_CMDID,
                               sizeof(WMI_INIT_RCV_DATA_CLASSIFIER_CMD));

        if (status == A_OK)
        {
            break;
        }
        else if (status == A_NO_MEMORY)
        {
            pDCxt->tx_complete_pend = true;

            if (A_OK != DRIVER_WAIT_FOR_CONDITION(p_Global_Cxt, &(pDCxt->tx_complete_pend), false, 5000))
            {
                A_ASSERT(0);
            }
        }
        else
        {
            A_ASSERT(0);
        }
    } while (1);

#if 0
    A_FREE(pDataCls, MALLOC_ID_TEMPORARY);
#endif

    return status;
}

A_STATUS wmi_update_reverse_credits_cmd(void *handle, uint8_t endpoint, uint32_t nCredits)
{
    A_STATUS status = A_ERROR;

    return status;
}
#endif

#ifndef ENABLE_LARGE_DSET
A_STATUS wmi_dset_host_cfg_cmd(void *handle)
{
    A_STATUS status = A_ERROR;
    uint8_t i;

    WMI_DSET_HOST_CFG_CMD dset;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(handle);

    dset.length = 1;

    dset.dset[0].dset_id = 0x0901;
    //    dset.dset[0].dset_id = 0x0902;
    dset.dset[0].len = 2048;
    dset.dset[0].offset = 0;

    do
    {
        status = wmi_cmd_start(pDCxt->pWmiCxt, &dset, WMI_DSET_HOST_CFG_CMDID, sizeof(WMI_DSET_HOST_CFG_CMD));

        if (status == A_OK)
        {
            break;
        }
        else if (status == A_NO_MEMORY)
        {
            pDCxt->tx_complete_pend = true;

            if (A_OK != DRIVER_WAIT_FOR_CONDITION(p_Global_Cxt, &(pDCxt->tx_complete_pend), false, 5000))
            {
                A_ASSERT(0);
            }
        }
        else
        {
            A_ASSERT(0);
        }
    } while (1);

    Driver_WakeDriver(p_Global_Cxt);
    A_MDELAY(10);
}

#else

A_STATUS wmi_dset_host_cfg_cmd(void *handle)
{
    HOST_DSET *pDset;
    WMI_STORERECALL_RECALL_CMD *pStrrclCmd;
    WMI_STORERECALL_RECALL_DSET *pDsetInfo;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(handle);

    pStrrclCmd = (WMI_STORERECALL_RECALL_CMD *)pDCxt->tempStorage;
    pStrrclCmd->length = 0;

    pDsetInfo = (WMI_STORERECALL_RECALL_DSET *)pStrrclCmd->data;

    pDset = dset_get_first();

    while (pDset != NULL)
    {
        if (IS_STRRCL_DSET(pDset->dset_id))
        {
            pDsetInfo->dset_id = pDset->dset_id;
            pDsetInfo->length = pDset->length;

            pStrrclCmd->length++;
            pDsetInfo++;
        }
        pDset = dset_get_next();
    }

    return A_OK;
}
#endif

A_STATUS wmi_set_appie_cmd(struct wmi_t *wmip, void *data)
{
    ATH_APPIE_PARAM *appie_param = (ATH_APPIE_PARAM *)data;
    void *osbuf = NULL;
    WMI_SET_APPIE_CMD *set_appie_cmd;
    A_STATUS status;

    osbuf = A_NETBUF_ALLOC(sizeof(WMI_SET_APPIE_CMD) + appie_param->ieLen);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    A_NETBUF_PUT(osbuf, sizeof(WMI_SET_APPIE_CMD) + appie_param->ieLen);

    set_appie_cmd = (WMI_SET_APPIE_CMD *)(A_NETBUF_DATA(osbuf));
    set_appie_cmd->mgmtFrmType = appie_param->mgmtFrmType;
    set_appie_cmd->ieLen = appie_param->ieLen;

    A_MEMCPY(set_appie_cmd->ieInfo, appie_param->ieInfo, appie_param->ieLen);
    status = wmi_cmd_send(wmip, osbuf, WMI_SET_APPIE_CMDID, NO_SYNC_WMIFLAG);

    if (status != A_OK) {
        A_NETBUF_FREE(osbuf);
    }

    return status;
}

