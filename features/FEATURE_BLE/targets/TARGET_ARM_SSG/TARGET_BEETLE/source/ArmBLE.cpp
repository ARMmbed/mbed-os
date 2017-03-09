/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "ArmBLE.h"

extern "C" {
#include "cordio_sdk_version.h"
#include "chip.h"
#include "board.h"
#include "utils.h"

#include "hpal_blep.h"
#include "hpal_hci.h"

#include "ble_init.h"
#include "main_board.h"

#include "wsf_assert.h"
#include "wsf_buf.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "wsf_mbed_os.h"
#include "wsf_sec.h"
#include "wsf_timer.h"
#include "wsf_types.h"
#include "wsf_trace.h"
#include "bstream.h"

#include "hci_handler.h"
#include "hci_tr.h"
#include "dm_handler.h"
#include "l2c_handler.h"
#include "att_handler.h"
#include "smp_handler.h"
#include "l2c_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "hci_core.h"
#include "hci_drv.h"
}

/* Number of WSF buffer pools. */
#define WSF_BUF_POOLS           4

/* Buffer size. */
#define ACL_BUF_SIZE            256

/* Total number of buffers. */
#define NUM_BUFS                8

/* Total size in bytes for buffer memory. */
#define BUF_MEM_SIZE            (1024 * 4)

/*! Free memory for pool buffers. */
static uint32_t mainBufMem[BUF_MEM_SIZE/sizeof(uint32_t)];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  { 16,   8 },
  { 32,   8 },
  { 64,   8 },
  { ACL_BUF_SIZE + 16, NUM_BUFS }
};

/* Store the Event signalling */
bool isEventsSignaled = false;

/*! WSF handler ID */
wsfHandlerId_t armHandlerId;
wsfHandlerId_t BleHandlerId;
static volatile int reset_complete;

/* Beetle Internal Functions */

/* HCI callback prototypes. */
static uint8_t *mainHciBufAlloc(uint16_t len);
static void mainHciBufFree(uint8_t *buf);
static void mainHciReadDone(uint8_t type, uint8_t *pData, uint16_t len);
static void mainHciWriteDone(uint8_t type, uint8_t *pData, int32_t err, void *pContext);

/*! \brief      HCI callbacks. */
static const hpalHciCbacks_t mainHciCbacks =
{
  mainHciBufAlloc,
  mainHciBufFree,
  mainHciReadDone,
  mainHciWriteDone
};

/**************************************************************************************************
  Stack HCI
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief    Allocate a buffer for a received message.
 *
 *  This function is called from an interrupt context.
 *
 *  \param    len     Length of buffer
 *
**  \return   Pointer to buffer or NULL if buffer could not be allocated
 */
/*************************************************************************************************/
static uint8_t *mainHciBufAlloc(uint16_t len)
{
  return (uint8_t*)WsfMsgAlloc(len);
}

/*************************************************************************************************/
/*!
 *  \brief    Free a buffer previously allocated with `BufAlloc()`.
 *
 *  \param    pData     Pointer to buffer
 */
/*************************************************************************************************/
static void mainHciBufFree(uint8_t *pData)
{
  WsfMsgFree(pData);
}

/*************************************************************************************************/
/*!
 *  \brief    Handle read completion.
 *
 *  This function is called from an interrupt context.
 *
 *  \param    type    Packet type
 *  \param    pData   Packet data, which was allocated with `BufAlloc()`.  The caller must free
 *                    this buffer
 *  \param    len     Length of packet data, in bytes
 *
 *  \return   None.
 */
/*************************************************************************************************/
static void mainHciReadDone(uint8_t type, uint8_t *pData, uint16_t len)
{
  WSF_ASSERT(align == 0);

  switch (type)
  {
    /* Route standard types to stack. */
    case HCI_CMD_TYPE:
    case HCI_ACL_TYPE:
    case HCI_EVT_TYPE:
    {
      hciCoreRecv(type, pData);
      break;
    }

    /* We should not get anything else. */
    default:
    {
      WsfMsgFree(pData);
      break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief    Handle write completion.
 *
 *  This function is called from an interrupt context.
 *
 *  \param    type      Packet type.
 *  \param    pData     Pointer to buffer that held written data, which was passed to
 *                      `HpalHciWrite()`
 *  \param    err       Indicates success (0) or error (one of the `HPAL_HCI_ERROR_####` codes)
 *  \param    pContext  Context pointer passed to `HpalHciWrite()`
 *
 *  \return   None.
 */
/*************************************************************************************************/
static void mainHciWriteDone(uint8_t type, uint8_t *pData, int32_t err, void *pContext)
{
  switch (type)
  {
    case HCI_CMD_TYPE:
    {
      WsfMsgFree(pData);
      break;
    }

    case HCI_ACL_TYPE:
    {
      hciCoreTxAclComplete((hciCoreConn_t *)pContext, pData);
      break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \fn     hciTrSendAclData
 *
 *  \brief  Send a complete HCI ACL packet to the transport.
 *
 *  \param  pContext Connection context.
 *  \param  pData    WSF msg buffer containing an ACL packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciTrSendAclData(void *pContext, uint8_t *pData)
{
  uint16_t   len;

  /* get 16-bit length */
  BYTES_TO_UINT16(len, &pData[2]);
  len += HCI_ACL_HDR_LEN;

  /* transmit ACL header and data */
  HpalHciWrite(HCI_ACL_TYPE, pData, len, pContext);
}

/*************************************************************************************************/
/*!
 *  \fn     hciTrSendCmd
 *
 *  \brief  Send a complete HCI command to the transport.
 *
 *  \param  pData    WSF msg buffer containing an HCI command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciTrSendCmd(uint8_t *pData)
{
  uint8_t   len;

  /* get length */
  len = pData[2] + HCI_CMD_HDR_LEN;

  /* dump event for protocol analysis */
  HCI_PDUMP_CMD(len, pData);

  /* transmit ACL header and data */
  HpalHciWrite(HCI_CMD_TYPE, pData, len, NULL);
}

void beetleCordioReset(void)
{
    CORDIO_LLCCTRL_LLC_RESET_ASSERT();
    CORDIO_LLCCTRL_RESETX_ASSERT();
    CORDIO_LLCCTRL_VMEM_ON_ASSERT();
    wait_ms(2);
    CORDIO_LLCCTRL_RESETX_NEGATE();
    CORDIO_LLCCTRL_LLC_RESET_NEGATE();
}

/* End Beetle Internal Functions */

/**
 * The singleton which represents the ArmBLE transport for the BLE.
 */
static ArmBLE deviceInstance;

/**
 * BLE-API requires an implementation of the following function in order to
 * obtain its transport handle.
 */
BLEInstanceBase *createBLEInstance(void)
{
    return (&deviceInstance);
}

ArmBLE::ArmBLE(void) : initialized(false), instanceID(BLE::DEFAULT_INSTANCE)
{
}

ArmBLE::~ArmBLE(void)
{
}

const char *ArmBLE::getVersion(void)
{
    static char versionString[32];

    strncpy(versionString, "unknown", sizeof(versionString));

    return versionString;
}

static void DmCback(dmEvt_t *pDmEvt)
{
    dmEvt_t *pMsg;

    if ((pMsg = (dmEvt_t*)WsfMsgAlloc(sizeof(dmEvt_t))) != NULL)
    {
        memcpy(pMsg, pDmEvt, sizeof(dmEvt_t));
        WsfMsgSend(armHandlerId, pMsg);
    }
}

static void armHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    if (pMsg != NULL)
    {
        switch(pMsg->event)
        {
            case DM_RESET_CMPL_IND:
                reset_complete = 1;
                break;
            case DM_ADV_START_IND:
                break;
            case DM_ADV_STOP_IND:
                ArmGap::getInstance().advertisingStopped();
                break;
            case DM_SCAN_REPORT_IND:
                {
                    dmEvt_t *scan = (dmEvt_t*)pMsg;
                    ArmGap::getInstance().processAdvertisementReport( scan->scanReport.addr,
                                                                        scan->scanReport.rssi,
                                                                        (scan->scanReport.eventType == DM_ADV_SCAN_RESPONSE) ? true : false,
                                                                        (GapAdvertisingParams::AdvertisingType_t)scan->scanReport.eventType,
                                                                        scan->scanReport.len,
                                                                        scan->scanReport.pData);
                }
                break;
            case DM_CONN_OPEN_IND:
                {
                    dmEvt_t *conn = (dmEvt_t*)pMsg;
                    ArmGap::getInstance().setConnectionHandle(conn->hdr.param);
                    Gap::ConnectionParams_t params = { conn->connOpen.connInterval, conn->connOpen.connInterval, conn->connOpen.connLatency, conn->connOpen.supTimeout };
                    Gap::AddressType_t ownAddrType;
                    Gap::Address_t ownAddr;
                    ArmGap::getInstance().getAddress(&ownAddrType, ownAddr);
                    ArmGap::getInstance().processConnectionEvent(conn->hdr.param,
                                                                   Gap::PERIPHERAL,
                                                                   (Gap::AddressType_t)conn->connOpen.addrType,
                                                                   conn->connOpen.peerAddr,
                                                                   ownAddrType,
                                                                   ownAddr,
                                                                   &params);
                }
                break;
            case DM_CONN_CLOSE_IND:
                {
                    dmEvt_t *conn = (dmEvt_t*)pMsg;
                    ArmGap::getInstance().setConnectionHandle(DM_CONN_ID_NONE);
                    ArmGap::getInstance().processDisconnectionEvent(conn->hdr.param, (Gap::DisconnectionReason_t)conn->connClose.reason);
                }
                break;
            default:
                break;
        }
    }
}

/*
 * AttServerInitDeInitCback callback is used to Initialize/Deinitialize
 * the CCC Table of the ATT Server when a remote peer requests to Open
 * or Close the connection.
 */
static void AttServerInitDeInitCback(dmEvt_t *pDmEvt)
{
  dmConnId_t connId = (dmConnId_t)pDmEvt->hdr.param;

  switch (pDmEvt->hdr.event)
  {
    case DM_CONN_OPEN_IND:
      /* set up CCC table with uninitialized (all zero) values */
      AttsCccInitTable(connId, NULL);
      break;
    case DM_CONN_CLOSE_IND:
      /* clear CCC table on connection close */
      AttsCccClearTable(connId);
      break;
    default:
      break;
  }
}

/*
 * This function will signal to the user code by calling signalEventsToProcess.
 * It is registered and called into the Wsf Stack.
 */
 static uint32_t signalEvent()
 {
     if(isEventsSignaled == false) {
         isEventsSignaled = true;
         deviceInstance.signalEventsToProcess(BLE::DEFAULT_INSTANCE);
     }
     return BLE_ERROR_NONE;
 }

static void ArmBLEInit(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    wsfHandlerId_t handlerId;

    /*
     * The init sequence is taken from the Cordio Stack Porting Guide version
     * 1.1 Chapter 2.6.
     */

    /* init OS subsystems */
    /* initialize WSF services */
    WsfSecInit();
    /* Disabled WsfSecAesInit(); */

    /* initialize HCI */
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    /* initialize DM */
    handlerId = WsfOsSetNextHandler(DmHandler);
    DmAdvInit();
    DmConnInit();
    DmConnSlaveInit();
    DmSecInit();
    DmHandlerInit(handlerId);

    /* initialize L2CAP */
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    /* initialize ATT */
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();

    /* initialize SMP */
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmprInit();

    /* store handler ID */
    armHandlerId = WsfOsSetNextHandler(armHandler);

    /* Register for stack callbacks */
    HpalHciSetCbacks(&mainHciCbacks);
    HpalHciSetLoggingLevel(HPAL_HCI_LOGGING_LEVEL_VERBOSE);
    HpalHciTakeUp();

    DmRegister(DmCback);
    DmConnRegister(DM_CLIENT_ID_APP, DmCback);
    AttConnRegister(AttServerInitDeInitCback);

    /* Register MBED OS Signal Event */
    mbedOSRegisterSignalEventCallback(&signalEvent);

    /* Reset the device */
    reset_complete = 0;
    DmDevReset();
}

static void ArmBLEStartCallback(bool_t ok)
{
    if (ok)
    {
        /* If started up, allow remaining application to run. */
        WsfSetEvent(BleHandlerId, 1);
    }
}

ble_error_t ArmBLE::init(BLE::InstanceID_t instanceID, FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> initCallback)
{
    wsfHandlerId_t handlerId;

    beetleCordioReset();
    Main_BoardInit();

    /* init OS subsystems */
    WsfTimerInit();
    WsfBufInit(sizeof(mainBufMem), (uint8_t*)mainBufMem, WSF_BUF_POOLS, mainPoolDesc);

    /* init stack */
    handlerId = WsfOsSetNextHandler(HpalBlepHandler);
    HpalBlepInit(handlerId);

    BleHandlerId = WsfOsSetNextHandler(ArmBLEInit);
    HpalBlepSetStartupCback(ArmBLEStartCallback);

    /*
     * Initialize BLE:
     * - Download the Cordio FW
     * - Initialize the lower layers
     */
    BleInitStart();

    /* Wait for BLE device to boot */
    while (!reset_complete) {
        callDispatcher();
    }

    initialized = true;
    BLE::InitializationCompleteCallbackContext context = {
        BLE::Instance(instanceID),
        BLE_ERROR_NONE
    };
    initCallback.call(&context);
    return BLE_ERROR_NONE;
}

ble_error_t ArmBLE::shutdown(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void ArmBLE::waitForEvent(void)
{
    timestamp_t nextTimestamp;
    bool_t pTimerRunning;

    processEvents();

    if (wsfOsReadyToSleep()) {
        // setup an mbed timer for the next Wicentric timeout
        nextTimestamp = (timestamp_t)WsfTimerNextExpiration(&pTimerRunning) * 1000;
        if (pTimerRunning) {
            /*
            * go to sleep to nextTimestamp: The ble stack here has to inform
            * the system that is ready and not really go to sleep.
            * TO BE DONE
            */
        }
    }
}

void ArmBLE::processEvents()
{
    if (isEventsSignaled) {
        isEventsSignaled = false;
        callDispatcher();
    }
}

void ArmBLE::timeoutCallback(void)
{
    // do nothing. just an interrupt for wake up.
}

void ArmBLE::callDispatcher(void)
{
    static uint32_t lastTimeUs = us_ticker_read();
    uint32_t currTimeUs, deltaTimeMs;

    // Update the current Wicentric time
    currTimeUs = us_ticker_read();
    deltaTimeMs = (currTimeUs - lastTimeUs) / 1000;
    if (deltaTimeMs > 0) {
        WsfTimerUpdate(deltaTimeMs);
        lastTimeUs += deltaTimeMs * 1000;
    }

    wsfOsDispatcher();
}
