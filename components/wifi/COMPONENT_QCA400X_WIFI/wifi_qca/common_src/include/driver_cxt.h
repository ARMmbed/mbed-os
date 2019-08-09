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
#ifndef _DRIVER_CXT_H_
#define _DRIVER_CXT_H_

#include "a_types.h"
#include <athdefs.h>
#include <htc_api.h>
#include <wmi.h>
#include "ieee80211.h"
#include "atheros_wifi.h"
#include "mbed_rtos_storage.h"

#define MIN_STRRCL_MSEC (100)

#define AR4100_DEFAULT_CREDIT_COUNT 6

#define AR4100_BUFFER_SIZE 1664

#define AR4100_MAX_AMSDU_RX_BUFFERS 4
#define AR4100_AMSDU_REFILL_THRESHOLD 3

#define AR4100_AMSDU_BUFFER_SIZE (WMI_MAX_AMSDU_RX_DATA_FRAME_LENGTH + 128)

#if WLAN_CONFIG_AMSDU_ON_HOST
#define AR4100_MAX_RX_MESSAGE_SIZE (max(WMI_MAX_NORMAL_RX_DATA_FRAME_LENGTH, WMI_MAX_AMSDU_RX_DATA_FRAME_LENGTH))
/* The RX MESSAGE SIZE is ultimately dictated by the size of the firmware buffers used to
 * hold packets.  This is because any remaining space may be used by HTC for trailers. hence
 * a full size buffer must be anticipated by the host. */
#else
#define AR4100_MAX_RX_MESSAGE_SIZE (AR4100_BUFFER_SIZE)
#endif

#define MAX_NUM_CHANLIST_CHANNELS (16)

#define MAX_WEP_KEY_SZ (16)

#define STRRCL_ST_DISABLED 0
#define STRRCL_ST_INIT 1
#define STRRCL_ST_START 2
#define STRRCL_ST_ACTIVE 3
#define STRRCL_ST_ACTIVE_B 4
#define STRRCL_ST_AWAIT_FW 5

#define AES_KEY_SIZE_BYTES 16

#define ACCESS_REQUEST_IOCTL 0
#define ACCESS_REQUEST_TX 1
#define ACCESS_REQUEST_RX 2

#define DRIVER_SCOPE_RX 0x01
#define DRIVER_SCOPE_TX 0x02

#define DRIVER_STATE_IDLE 0x00                 /* the driver task is idle/sleeping */
#define DRIVER_STATE_RX_PROCESSING 0x01        /* the driver task is processing a RX request */
#define DRIVER_STATE_TX_PROCESSING 0x02        /* the driver task is processing a TX request */
#define DRIVER_STATE_INTERRUPT_PROCESSING 0x03 /* the driver task is processing a chip Interrupt */
#define DRIVER_STATE_ASYNC_PROCESSING 0x04     /* the driver task is processing an asynch operation */
#define DRIVER_STATE_PENDING_CONDITION_A 0x05  /* the driver task is waiting for a condition to be met */
#define DRIVER_STATE_PENDING_CONDITION_B 0x06
#define DRIVER_STATE_PENDING_CONDITION_C 0x07
#define DRIVER_STATE_PENDING_CONDITION_D 0x08
#define DRIVER_STATE_PENDING_CONDITION_E 0x09
#define DRIVER_STATE_PENDING_CONDITION_F 0x0a
#define DRIVER_STATE_PENDING_CONDITION_G 0x0b

#define MAX_CREDIT_DEADLOCK_ATTEMPTS 2
#define DEADLOCK_BLOCK_MSEC 10

typedef struct ar_wep_key
{
    uint8_t keyLen;
    uint8_t key[MAX_WEP_KEY_SZ];
} A_WEPKEY_T;

/* A_SCAN_SUMMARY provides useful information that
 * has been parsed from bss info headers and probe/beacon information elements.
 */
typedef struct scan_summary
{
    uint16_t beacon_period;
    uint16_t caps;
    uint8_t bssid[6];
    uint8_t ssid[32];
    uint8_t channel;
    uint8_t ssid_len;
    uint8_t rssi;
    uint8_t bss_type;
    uint8_t rsn_cipher;
    uint8_t rsn_auth;
    uint8_t wpa_cipher;
    uint8_t wpa_auth;
    uint8_t wep_support;
    uint8_t reserved; // keeps struct on 4-byte boundary
} A_SCAN_SUMMARY;

typedef struct
{
    uint16_t maxMsgLength;
    uint16_t serviceID;
    uint32_t intCredits;
    uint8_t intCreditCounter;
    uint8_t rptCreditCounter;
    uint8_t epIdx;
    uint8_t credits;
    uint8_t maxCredits;
    uint8_t seqNo;
} A_ENDPOINT_T;

#if USE_16BIT_CREDIT_COUNTER
typedef struct
{
    uint16_t maxMsgLength;
    uint16_t serviceID;
    uint32_t intCredits;
    uint16_t intCreditCounter;
    uint16_t rptCreditCounter;
    uint8_t epIdx;
    uint8_t rsvd;
    uint16_t credits;
    uint16_t maxCredits;
    uint8_t seqNo;
} A_ENDPOINT_T;
#endif

typedef A_STATUS (*TEMP_FUNC_T)(void *pCxt);

typedef struct _a_connection_elements
{
    int32_t ssidLen;
    uint8_t ssid[32];
    uint8_t networkType; /* one of NETWORK_TYPE enum */
    uint32_t connectCtrlFlags;
    uint8_t dot11AuthMode;
    uint8_t wpaAuthMode;
    uint8_t wpaPairwiseCrypto;
    uint8_t wpaPairwiseCryptoLen;
    uint8_t wpaGroupCrypto;
    uint8_t wpaGroupCryptoLen;
    uint8_t wepDefTxKeyIndex;
    A_WEPKEY_T wepKeyList[WMI_MAX_KEY_INDEX + 1];
    uint8_t wpaPmk[WMI_PMK_LEN];
    boolean wpaPmkValid;
    uint8_t bssid[ATH_MAC_LEN];
    uint8_t reqBssid[ATH_MAC_LEN];
    uint16_t channelHint;
    uint16_t bssChannel;
    boolean isConnected;
    boolean isConnectPending;
    uint8_t phyMode;
} A_CONNECTION_ELEMENTS_T;

typedef struct _a_spi_hcd_context
{
#define SDHD_BOOL_SHUTDOWN 0x00000001
#define SDHD_BOOL_EXTERNALIO_PENDING 0x00000002
#define SDHD_BOOL_DMA_WRITE_WAIT_FOR_BUFFER 0x00000004
#define SDHD_BOOL_DMA_IN_PROG 0x00000008
#define SDHD_BOOL_TRANSFER_DIR_RX 0x00000010
#define SDHD_BOOL_HOST_DMA_COPY_MODE 0x00000020
#define SDHD_BOOL_HOST_ACCESS_COPY_MODE 0x00000040
#define SDHD_BOOL_FATAL_ERROR 0x00000080
#define SDHD_BOOL_DMA_COMPLETE 0x00000100

#define BYTE_SWAP true
#define NO_BYTE_SWAP false

    /*******************************************
     *
     * the following fields must be filled in by the hardware specific layer
     *
     ********************************************/
    // from SDHCD START HERE
    uint8_t PendingIrqAcks; /* mask of pending interrupts that have not been ACK'd */
    uint8_t IrqsEnabled;
    uint8_t IrqDetected;
    // from SDHCD END HERE
    uint8_t CpuInterrupt;
    uint8_t CpuInterruptCause;

    uint16_t SpiIntEnableShadow; /* shadow copy of interrupt enables */
    uint16_t SpiConfigShadow;    /* shadow copy of configuration register */
    uint16_t irqMask;
    uint8_t HostAccessDataWidth;   /* data width to use for host access */
    uint8_t DMADataWidth;          /* data width to use for DMA access */
    uint32_t ReadBufferSpace;      /* cached copy of read buffer available register */
    uint32_t WriteBufferSpace;     /* cached copy of space remaining in the SPI write buffer */
    uint32_t MaxWriteBufferSpace;  /* max write buffer space that the SPI interface supports */
    uint32_t PktsInSPIWriteBuffer; /* number of packets in SPI write buffer so far */
    void *pCurrentRequest;         /* pointer to a request in-progress used only for deferred SPI ops. */

    uint32_t OperationalClock; /* spi module operational clock */
    uint32_t PowerUpDelay;     /* delay before the common layer should initialize over spi */
/* SPI SPECIFICE ELEMENTS Starts here */
#define HW_SPI_FRAME_WIDTH_8 0x01
#define HW_SPI_FRAME_WIDTH_16 0x02
#define HW_SPI_FRAME_WIDTH_24 0x04
#define HW_SPI_FRAME_WIDTH_32 0x08
#define HW_SPI_INT_EDGE_DETECT 0x80
#define HW_SPI_NO_DMA 0x40
    uint8_t SpiHWCapabilitiesFlags; /* SPI hardware capabilities flags */

#define MISC_FLAG_SPI_SLEEP_WAR 0x04
#define MISC_FLAG_RESET_SPI_IF_SHUTDOWN 0x02
#define MISC_FLAG_DUMP_STATE_ON_SHUTDOWN 0x01
    uint8_t MiscFlags;
    A_MUTEX_T lock;
} A_SPI_HCD_CXT;

typedef struct _a_driver_context
{
    uint16_t driver_state; /* maintains current state of driver one of DRIVER_STATE_... */
    uint8_t txQueryResult;
    boolean txQueryInProgress;
    boolean driver_up; /* maintains driver status for external access */
    boolean chipDown;
    boolean rxBufferStatus; /* maintains available status of rx buffers */
    boolean creditDeadlock;
    uint32_t creditDeadlockCounter;
    A_NETBUF_QUEUE_T txQueue;
    void *pRxReq;
    A_ENDPOINT_T ep[ENDPOINT_MANAGED_MAX];
    HTC_ENDPOINT_ID ac2EpMapping[WMM_NUM_AC];
    uint8_t ep2AcMapping[ENDPOINT_MAX];
    uint16_t creditCount; /* number of tx credits available at init */
    uint16_t creditSize; // size in bytes of a credit
    /* the 5 version values maintained by the context */
    uint32_t targetType;    /* general type value */
    uint32_t targetVersion; /* identifies the target chip */
    uint32_t wlanVersion;   /* identifies firmware running on target chip */
    uint32_t abiVersion;    /* identifies the interface compatible with wlan firmware */
    uint32_t hostVersion;   /* identifies version of host driver */

    uint8_t phyCapability; /* identifies chip phy capability */
    boolean wmiReady;
    boolean wmiEnabled;
    void *pWmiCxt;  /* context for wmi driver component */
    void *pAggrCxt; /* context for rx aggregate driver component */
    uint16_t txAggrTidMask;
    uint16_t rxAggrTidMask;

    int8_t numChannels;
    uint16_t channelList[MAX_NUM_CHANLIST_CHANNELS];

    uint32_t regCode;
    int8_t rssi;
    int32_t bitRate;
    boolean wmmEnabled;
    boolean tx_complete_pend; /* tracks new tx completes for task sync */
    TEMP_FUNC_T asynchRequest;
    /* STORE&RECALL SPECIFICE ELEMENTS Starts here */
    void *strrclCxt;
    int32_t strrclCxtLen;
    boolean strrclBlock;
    uint8_t strrclState;
    /* STORE&RECALL SPECIFICE ELEMENTS Ends here */
    /* WPS SPECIFICE ELEMENTS Starts here */
    void *wpsBuf;
    void *wpsEvent;
    boolean wpsState;
    /* WPS SPECIFIC ELEMENTS Ends here */
    uint8_t userPwrMode;
    uint8_t tkipCountermeasures;
    /* CONNECT SPECIFIC ELEMENTS Start Here */
    uint8_t devId;
    A_CONNECTION_ELEMENTS_T conn[WLAN_NUM_OF_DEVICES];
    /* CONNECT SPECIFIC ELEMENTS End Here */
    /* from hif_device START HERE */
    uint16_t enabledSpiInts;
    uint32_t lookAhead;
    uint32_t mboxAddress; /* the address for mailbox reads/writes. */
    uint32_t blockSize;   /* the mailbox block size */
    uint32_t blockMask;   /* the mask derived from BlockSize */
    uint32_t *padBuffer;
    uint16_t booleans;
    /* from hif_device ENDS HERE */
    /* SPI SPECIFICE ELEMENTS Starts here */
    A_SPI_HCD_CXT spi_hcd;

    /* SPI SPECIFIC ELEMENTS Ends here */
    boolean scanDone; /* scan done indicator, set by WMI_SCAN_COMPLETE_EVENTID */
    boolean driverSleep;
    boolean htcStart;
    uint8_t htc_creditInit;
    uint8_t *tempStorage;       /*temporary storage shared by independent modules*/
    uint16_t tempStorageLength; /*size of temp storage allocated at init time, decided by feature set*/
    uint8_t promiscuous_mode;   /* if 1 promiscuous mode is enabled */
    boolean rssiFlag;

    uint8_t apmodeWPS;
#if ENABLE_P2P_MODE
    boolean p2p_avail;
    uint8_t wpsRole;
#endif
    boolean macProgramming;
#if MANUFACTURING_SUPPORT
    boolean testResp;
#endif
    uint8_t rxMultiBufferStatus; /* maintains available status of rx buffers */
    boolean pfmDone;             /* scan done indicator, set by WMI_SCAN_COMPLETE_EVENTID */
    boolean wps_in_progress;
    uint32_t pwrStateSetting;
    boolean dset_op_done;    /* dset op done indicator, set by WMI_SCAN_COMPLETE_EVENTID */
    uint8_t raw_temperature; /*register value */
    uint32_t tempDegree;     /*temperature degree*/
    boolean temperatureValid;
    uint8_t raw_countryCode[3];
    boolean countryCodeValid;
    boolean setparamValid;
    uint32_t setparamStatus;
/* PORT_NOTE: rxFreeQueue stores pre-allocated rx net-buffers. If dynamic heap
*  allocation is preferred for RX net-buffers then this option should be disabled. */
#if DRIVER_CONFIG_IMPLEMENT_RX_FREE_QUEUE
    A_NETBUF_QUEUE_T rxFreeQueue;
#endif

    /* PORT_NOTE: pScanOut stores scan results in a buffer so that they
       *  can be retrieved aysnchronously by a User task. */
    uint8_t *pScanOut;     /* callers buffer to hold results. */
    uint16_t scanOutSize;  /* size of pScanOut buffer */
    uint16_t scanOutCount; /* current fill count of pScanOut buffer */

    boolean driverShutdown; /* used to shutdown the driver */
    uint32_t line;
    uint8_t securityType;  /* used to satisfy SEC_TYPE */
    uint8_t extended_scan; /* used to indicate whether an extended scan is in progress */
#if ENABLE_P2P_MODE
    void *p2pevtCB;
    boolean p2pEvtState;
    boolean p2pevtflag;
#endif
#if MANUFACTURING_SUPPORT
    uint32_t testCmdRespBufLen;
#endif
    boolean wps_init_key;
    uint32_t hb_sequence;
    boolean hb_challenge_done;
    WMI_SCAN_PARAMS_CMD scan_param; // save scan parameters for roam mode change
    osMutexId_t apiMutex;
    mbed_rtos_storage_mutex_t context_mutex_mem;
} A_DRIVER_CONTEXT;

extern uint32_t last_driver_error;

#define DEV_CALC_RECV_PADDED_LEN(pDCxt, length) (((length) + (pDCxt)->blockMask) & (~((pDCxt)->blockMask)))
#define DEV_CALC_SEND_PADDED_LEN(pDCxt, length) DEV_CALC_RECV_PADDED_LEN(pDCxt, length)
#define DEV_IS_LEN_BLOCK_ALIGNED(pDCxt, length) (((length) % (pDCxt)->blockSize) == 0)

#endif /* _DRIVER_CXT_H_ */
