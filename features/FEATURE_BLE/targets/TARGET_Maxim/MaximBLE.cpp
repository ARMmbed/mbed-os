/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "mbed.h"
#include "us_ticker_api.h"
#include "MaximBLE.h"
#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_sec.h"
#include "wsf_timer.h"
#include "hci_handler.h"
#include "dm_handler.h"
#include "l2c_handler.h"
#include "att_handler.h"
#include "smp_handler.h"
#include "l2c_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "hci_drv.h"
#include "hci_vs.h"

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS 5

/*! Free memory for pool buffers. */
static uint8_t mainBufMem[1040];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  2 },
  { 128,  2 },
  { 272,  1 }
};

/* Store the Event signalling */
bool isEventsSignaled = false;

/*! WSF handler ID */
wsfHandlerId_t maximHandlerId;
static volatile int reset_complete;

#ifdef BLE_HCI_UART
static DigitalIn _rts(BT_CTS);
static DigitalIn _cts(BT_RTS);
static DigitalIn _clk(BT_CLK);
static DigitalOut _shutdown(BT_RST, 0);
static Serial _uart(BT_TX, BT_RX, 115200);
#else
/* Current mbed SPI API does not support HW slave selects. Configured in HCI driver. */
static DigitalOut _csn(HCI_CSN, 1);
static SPI _spi(HCI_MOSI, HCI_MISO, HCI_SCK, HCI_CSN);
static DigitalOut _rst(HCI_RST, 0);
static InterruptIn _irq(HCI_IRQ);
#endif

/**
 * The singleton which represents the MaximBLE transport for the BLE.
 */
static MaximBLE deviceInstance;

extern "C" {

/*
 * This function will signal to the user code by calling signalEventsToProcess.
 * It is registered and called into the Wsf Stack.
 */
void wsf_mbed_ble_signal_event(void)
{
    if (isEventsSignaled == false) {
        isEventsSignaled = true;
        deviceInstance.signalEventsToProcess(::BLE::DEFAULT_INSTANCE);
    }
}

}

/**
 * BLE-API requires an implementation of the following function in order to
 * obtain its transport handle.
 */
BLEInstanceBase *createBLEInstance(void)
{
    return (&deviceInstance);
}

MaximBLE::MaximBLE(void) : initialized(false), instanceID(BLE::DEFAULT_INSTANCE)
{
}

MaximBLE::~MaximBLE(void)
{
}

const char *MaximBLE::getVersion(void)
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
        WsfMsgSend(maximHandlerId, pMsg);
    }
}

static void maximHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
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
                MaximGap::getInstance().advertisingStopped();
                break;
            case DM_SCAN_REPORT_IND:
                {
                    hciLeAdvReportEvt_t *scanReport = (hciLeAdvReportEvt_t*)pMsg;
                    MaximGap::getInstance().processAdvertisementReport( scanReport->addr,
                                                                        scanReport->rssi,
                                                                        (scanReport->eventType == DM_ADV_SCAN_RESPONSE) ? true : false,
                                                                        (GapAdvertisingParams::AdvertisingType_t)scanReport->eventType,
                                                                        scanReport->len,
                                                                        scanReport->pData);
                }
                break;
            case DM_CONN_OPEN_IND:
                {
                    hciLeConnCmplEvt_t *connOpen = (hciLeConnCmplEvt_t*)pMsg;
                    MaximGap::getInstance().setConnectionHandle(connOpen->handle);
                    Gap::ConnectionParams_t params = { connOpen->connInterval, connOpen->connInterval, connOpen->connLatency, connOpen->supTimeout };
                    Gap::AddressType_t ownAddrType;
                    Gap::Address_t ownAddr;
                    MaximGap::getInstance().getAddress(&ownAddrType, ownAddr);
                    MaximGap::getInstance().processConnectionEvent(connOpen->handle,
                                                                   Gap::PERIPHERAL,
                                                                   (Gap::AddressType_t)connOpen->addrType,
                                                                   connOpen->peerAddr,
                                                                   ownAddrType,
                                                                   ownAddr,
                                                                   &params);
                }
                break;
            case DM_CONN_CLOSE_IND:
                {
                    hciDisconnectCmplEvt_t *connClose = (hciDisconnectCmplEvt_t*)pMsg;
                    MaximGap::getInstance().setConnectionHandle(DM_CONN_ID_NONE);
                    MaximGap::getInstance().processDisconnectionEvent(connClose->handle, (Gap::DisconnectionReason_t)connClose->reason);
                }
                break;
            case DM_HW_ERROR_IND:
                {
                    hciHwErrorEvt_t *error = (hciHwErrorEvt_t*)pMsg;
                    printf("HCI Hardware Error 0x%02x occurred\n", error->code);
                }
                break;
            default:
                break;
        }
    }
}

static void AppServerConnCback(dmEvt_t *pDmEvt)
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

ble_error_t MaximBLE::init(BLE::InstanceID_t instanceID, FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> initCallback)
{
    wsfHandlerId_t handlerId;

    /* init OS subsystems */
    WsfTimerInit(1);
    WsfBufInit(sizeof(mainBufMem), mainBufMem, WSF_BUF_POOLS, mainPoolDesc);
    WsfSecInit();

    /* init stack */
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DmHandler);
    DmAdvInit();
    DmScanInit();
    DmConnInit();
    DmConnSlaveInit();
    DmSecInit();
    DmHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cMasterInit();
    L2cSlaveInit();

    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();
    AttcInit();

    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmpiInit();
    SmprInit();

    /* store handler ID */
    maximHandlerId = WsfOsSetNextHandler(maximHandler);

    /* init HCI */
#ifdef BLE_HCI_UART
    hciDrvInit(BT_TX, BT_RST, BT_CLK);
#else
    _irq.disable_irq();
    _irq.rise(hciDrvIsr);
    _irq.fall(NULL);
    hciDrvInit(HCI_CSN, HCI_RST, HCI_IRQ);
#endif

    /* Register for stack callbacks */
    DmRegister(DmCback);
    DmConnRegister(DM_CLIENT_ID_APP, DmCback);
    AttConnRegister(AppServerConnCback);
    
    /* Reset the device */
    reset_complete = 0;
    DmDevReset();

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

ble_error_t MaximBLE::shutdown(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void MaximBLE::waitForEvent(void)
{
    static LowPowerTimeout nextTimeout;
    timestamp_t nextTimestamp;
    bool_t pTimerRunning;

    callDispatcher();

    if (wsfOsReadyToSleep()) {
        // setup an mbed timer for the next Wicentric timeout
        nextTimestamp = (timestamp_t)WsfTimerNextExpiration(&pTimerRunning) * 1000;
        if (pTimerRunning) {
            nextTimeout.attach_us(timeoutCallback, nextTimestamp);
        }

        // go to sleep
        if (hciDrvReadyToSleep()) {
            // go to deep sleep
            deepsleep();
            hciDrvResume();
        }
        else {
            sleep();
        }
    }
}

void MaximBLE::processEvents()
{
    if (isEventsSignaled) {
        isEventsSignaled = false;
        callDispatcher();
    }
}

void MaximBLE::timeoutCallback(void)
{
    wsf_mbed_ble_signal_event();
}

void MaximBLE::callDispatcher(void)
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
