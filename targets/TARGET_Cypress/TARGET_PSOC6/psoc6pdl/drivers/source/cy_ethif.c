/***************************************************************************//**
* \file cy_ethif.c
* \version 1.0
*
* Provides an API implementation of the ETHIF driver
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXETH)

#include "cy_ethif.h"
#include <string.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*       Local Variables
***************************************/
/** Cadence driver instantiation    */
static CEDI_OBJ *cyp_ethif_gemgxlobj;

/** Ethernet configurations */
static CEDI_Config  cy_ethif_cfg;

/** Cadence driver memory requirements */
static CEDI_SysReq  cy_ethif_sysreq;

/** Private data structures required for each instance of Ethernet IPs  */
static CEDI_PrivateData * cyp_ethif_pd;

/** Variables holding Statistics register values    */
static CEDI_Statistics  * cyp_ethif_statistic;

/** Tx and Rx buffer */
uint8_t u8TxBuf[CY_ETH_TOTAL_TX_BUF][CY_ETH_SIZE_MAX_FRAME];
uint8_t u8RxBuf[CY_ETH_TOTAL_RX_BUF][CY_ETH_SIZE_MAX_FRAME];

/** Tx and Rx buffer status */
cy_stc_ethif_bufstatus_t cy_ethif_txBufStatus[CY_ETH_TOTAL_TX_BUF];       /** Buffer status will be CY_ETHIF_BUFFER_UNINIT */
cy_stc_ethif_bufstatus_t cy_ethif_rxBufStatus[CY_ETH_TOTAL_RX_BUF];       /** Buffer status will be CY_ETHIF_BUFFER_UNINIT */

/** Private data memory allocation  */
uint8_t cy_ethif_privatedata[1800] = {0};

/** Tx descriptors */
uint8_t cy_ethif_tx_desc_list[CY_ETH_DEFINE_NUM_TXQS][CY_ETH_DEFINE_TOTAL_BD_PER_TXQUEUE + 1][CY_ETH_BD_SIZE];

/** Rx descriptors */
/** Cadence driver requires another set of buffers to replace the existing
* buffers after a frame has been received. so, number of required BDs would not
* be same as number of buffers */
uint8_t cy_ethif_rx_desc_list[CY_ETH_DEFINE_NUM_RXQS][((CY_ETH_DEFINE_TOTAL_BD_PER_TXQUEUE + 1) * 2)][CY_ETH_BD_SIZE];

/** Statistics registers    */
uint8_t cy_ethif_statistic[160];

cy_stc_ethif_cb_t stccallbackfunctions;
cy_stc_ethif_queue_disablestatus_t stcQueueDisStatus;
bool bBufferInitialized = false;

/*****************************************************************************
* Local function prototypes ('static')
*****************************************************************************/
static void Cy_ETHIF_EnableInterrupts(cy_stc_ethif_interruptconfig_t * pstcInterruptList);
static void Cy_ETHIF_PrepareConfiguration(cy_stc_ethif_config_t * pstcEthIfConfig);
static void Cy_ETHIF_AssignMemory(void);
static void Cy_ETHIF_EventTx(void *pcy_privatedata, uint32_t u32event, uint8_t u8qnum);
static void Cy_ETHIF_EventTxError(void *pcy_privatedata, uint32_t u32event, uint8_t u8qnum);
static void Cy_ETHIF_EventRxFrame(void *pcy_privatedata, uint8_t u8qnum);
static void Cy_ETHIF_EventRxError(void *pcy_privatedata, uint32_t a_event, uint8_t a_qnum);
static void Cy_ETHIF_EventPhyManComplete(void *pcy_privatedata, uint8_t u8read, uint16_t u16ReadData);
static void Cy_ETHIF_EventhrespError(void *pcy_privatedata, uint8_t u8qnum);
static void Cy_ETHIF_EventLpPageRx(void* pcy_privatedata, struct CEDI_LpPageRx* pageRx);
static void Cy_ETHIF_EventAn(void* pcy_privatedata, struct CEDI_NetAnStatus* netStat);
static void Cy_ETHIF_EventLinkChange(void *pcy_privatedata, uint8_t a_linkstate);
static void Cy_ETHIF_EventTsu(void *pcy_privatedata, uint32_t u32event);
static void Cy_ETHIF_EventPauseFrame(void *pcy_privatedata, uint32_t u32event);
static void Cy_ETHIF_EventPtp(void* pcy_privatedata, uint32_t u32type, struct CEDI_1588TimerVal* time);
static void Cy_ETHIF_EventExternalInt(void * pcy_privatedata);
static void Cy_ETHIF_EventWol(void * pcy_privatedata);
static void Cy_ETHIF_EventLpi(void * pcy_privatedata);
static void Cy_ETHIF_InitializeBuffers(void);
static void Cy_ETHIF_ClearBuffer(uint32_t * pu32Buffer);
static uint8_t Cy_ETHIF_GetBuf(bool bTransmitBuf);
static cy_en_ethif_status_t Cy_ETHIF_WrapperConfig(cy_stc_ethif_wrapper_config_t * pstcWrapperConfig);
static void Cy_ETHIF_IPEnable(void);
static void Cy_ETHIF_IPDisable(void);
static cy_en_ethif_status_t Cy_ETHIF_TSUInit(cy_stc_ethif_tsu_config_t * pstcTSUConfig);
static cy_en_ethif_status_t Cy_ETHIF_DisableQueues(cy_stc_ethif_config_t * pstcEthIfConfig);

/*****************************************************************************
* Local Call back function supplied to Cadence driver                                                                        
*****************************************************************************/
CEDI_Callbacks Cy_ETHIF_Callbacks = {
  .phyManComplete = (CEDI_CbPhyManComplete)Cy_ETHIF_EventPhyManComplete,
  .txEvent        = (CEDI_CbTxEvent)Cy_ETHIF_EventTx,
  .txError        = (CEDI_CbTxError)Cy_ETHIF_EventTxError,
  .rxFrame        = (CEDI_CbRxFrame)Cy_ETHIF_EventRxFrame,
  .rxError        = (CEDI_CbRxError)Cy_ETHIF_EventRxError,
  .hrespError     = (CEDI_CbHrespError)Cy_ETHIF_EventhrespError,
  .lpPageRx       = (CEDI_CbLpPageRx)Cy_ETHIF_EventLpPageRx,
  .anComplete     = (CEDI_CbAnComplete)Cy_ETHIF_EventAn,
  .linkChange     = (CEDI_CbLinkChange)Cy_ETHIF_EventLinkChange,
  .tsuEvent       = (CEDI_CbTsuEvent)Cy_ETHIF_EventTsu,
  .pauseEvent     = (CEDI_CbPauseEvent)Cy_ETHIF_EventPauseFrame,
  .ptpPriFrameTx  = (CEDI_CbPtpPriFrameTx)Cy_ETHIF_EventPtp,
  .ptpPeerFrameTx = (CEDI_CbPtpPeerFrameTx)Cy_ETHIF_EventPtp,
  .ptpPriFrameRx  = (CEDI_CbPtpPriFrameRx)Cy_ETHIF_EventPtp,
  .ptpPeerFrameRx = (CEDI_CbPtpPeerFrameRx)Cy_ETHIF_EventPtp,
  .lpiStatus      = (CEDI_CbLpiStatus)Cy_ETHIF_EventLpi,
  .wolEvent       = (CEDI_CbWolEvent)Cy_ETHIF_EventWol,
  .extInpIntr     = (CEDI_CbExtInpIntr)Cy_ETHIF_EventExternalInt
};

/*******************************************************************************
* Function Name: Cy_ETHIF_Init
****************************************************************************//**
*
* \brief Initializes the Ethernet MAC, Cadence Driver, EthIf and PHY 
*
* \param pstcEthIfConfig
* Pointer to Ethernet configuration parameters
*
* \param pstcInterruptList
* List of Interrupts to enable
* 
* \return CY_ETHIF_SUCCESS      Ethernet MAC has been initialized along with 
*                               Cadence driver and external PHY
* \return CY_ETHIF_BAD_PARAM    If following conditions are met:
*                               pstcEth == NULL
*                               pstcEthIfConfig == NULL
*                               pstcInterruptList == NULL
*                               GEM_GXL Object could not be created
*                               Memory assigned by Interface layer is not enough
*                               Cadence driver could not initialize Ethernet MAC
*         CY_ETHIF_MEMORY_NOT_ENOUGH
*                               Assigned memory for BDs or for Private data is
*                               not enough 
*         CY_ETHIF_LINK_DOWN    Link is not yet up
* 
* \ Note:
* This function Initializes the Ethernet MAC, Cadence driver, EthIf layer and
* external PHY with the provided parameters. Port init for the Ethernet must be
* done before calling Cy_EthIf_Init function. Buffer configuration parameters
* shall be done in EthIf.h file
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_Init(cy_stc_ethif_config_t * pstcEthIfConfig, cy_stc_ethif_interruptconfig_t * pstcInterruptList)
{
    // local variable declaration
    uint32_t u32RetValue = 0;
    uint8_t u8tmpcounter = 0;
    uint8_t u8tmpintrcntr = 0;
    bool bClearAll = true;
    bool bTransmitBuf = true;
    uint16_t u16SysReqTxBDLength = 0;
    uint16_t u16SysReqRxBDLength = 0;

    /* Parameter checks */
    if ((pstcEthIfConfig == NULL) || (pstcInterruptList == NULL))
        return CY_ETHIF_BAD_PARAM;
    else if (pstcEthIfConfig->pstcWrapperConfig == NULL)
    {
        return CY_ETHIF_BAD_PARAM;
    }

    if (bBufferInitialized == false)
    {
        /* Create GEM_GXL object  */
        cyp_ethif_gemgxlobj = CEDI_GetInstance();
        if (cyp_ethif_gemgxlobj == NULL)
        {
            return CY_ETHIF_BAD_PARAM;
        }
    }

    /* Load Init parameter */
    Cy_ETHIF_PrepareConfiguration(pstcEthIfConfig);
    
    if (pstcEthIfConfig->bintrEnable == true)
    {
        /* Configure Interrupts */
        Cy_ETHIF_EnableInterrupts(pstcInterruptList);
        
        /* Init Callback functions */
        stccallbackfunctions.rxframecb       = NULL;
        stccallbackfunctions.txerrorcb       = NULL;
        stccallbackfunctions.txcompletecb    = NULL;
        stccallbackfunctions.tsuSecondInccb  = NULL;
    }

    /* Initialize ENET MAC Wrapper */
    if (CY_ETHIF_BAD_PARAM == Cy_ETHIF_WrapperConfig(pstcEthIfConfig->pstcWrapperConfig))
    {
        Cy_ETHIF_IPDisable();
        return CY_ETHIF_BAD_PARAM;
    }

    /* Enable the IP to access EMAC registers set */
    Cy_ETHIF_IPEnable();

    /* Probe for checking configuration parameters and calculating memory size */
    (void)cyp_ethif_gemgxlobj->probe(&cy_ethif_cfg, &cy_ethif_sysreq);
    
    /* Check for assigned memory and required memory match */
    u16SysReqTxBDLength = ((cy_ethif_sysreq.txDescListSize /  CY_ETH_DEFINE_NUM_TXQS)/(CY_ETH_DEFINE_TOTAL_BD_PER_TXQUEUE + 1));
    u16SysReqRxBDLength = ((cy_ethif_sysreq.rxDescListSize /  CY_ETH_DEFINE_NUM_RXQS)/(CY_ETH_DEFINE_TOTAL_BD_PER_RXQUEUE + 1));

    if ((u16SysReqTxBDLength != CY_ETH_BD_SIZE) || (u16SysReqRxBDLength != CY_ETH_BD_SIZE))
    {
        /* Memory not enough */
        return CY_ETHIF_MEMORY_NOT_ENOUGH;
    }

    if (sizeof(cy_ethif_privatedata)< cy_ethif_sysreq.privDataSize)
    {
        /* Memory not enough */
        return CY_ETHIF_MEMORY_NOT_ENOUGH;
    }

    /* assign starting addresses to local variable */
    Cy_ETHIF_AssignMemory();

    /* Initialization EMAC registers */
    u32RetValue = cyp_ethif_gemgxlobj->init((void *)cyp_ethif_pd, &cy_ethif_cfg, &Cy_ETHIF_Callbacks);
    if (u32RetValue == EINVAL || u32RetValue == ENOTSUP)
    {
        Cy_ETHIF_IPDisable();
        return CY_ETHIF_BAD_PARAM;
    }

    /* Disable Transmit and Receive Queues */
    Cy_ETHIF_DisableQueues(pstcEthIfConfig);

    /* TSU Initialization */
    if (NULL != pstcEthIfConfig->pstcTSUConfig)
    {
        if (CY_ETHIF_BAD_PARAM == Cy_ETHIF_TSUInit(pstcEthIfConfig->pstcTSUConfig))
        {
            Cy_ETHIF_IPDisable();
            return CY_ETHIF_BAD_PARAM;
        }
    }

    /* Initialize Buffer status */
    if (bBufferInitialized == false)
    {
        Cy_ETHIF_InitializeBuffers();
        bBufferInitialized = true;
    }
    else
    {
        /* clear all released transmit buffer */
        Cy_ETHIF_ClearReleasedBuf(bClearAll, bTransmitBuf);
        bTransmitBuf = false;
        /* clear all released receive buffer   */
        Cy_ETHIF_ClearReleasedBuf(bClearAll, bTransmitBuf);
    }

    /* allocate assign buffers to RX BDs */
    for (u8tmpcounter = 0; u8tmpcounter < cy_ethif_cfg.rxQs; u8tmpcounter++)
    {
        for (u8tmpintrcntr = 0; u8tmpintrcntr < cy_ethif_cfg.rxQLen[u8tmpcounter]; u8tmpintrcntr++)
        {            
            cyp_ethif_gemgxlobj->addRxBuf((void *)cyp_ethif_pd, 
                                            u8tmpcounter, 
                                           (CEDI_BuffAddr *)&cy_ethif_rxBufStatus[(u8tmpintrcntr + (u8tmpcounter*CY_ETH_DEFINE_TOTAL_BD_PER_RXQUEUE))].cy_ethif_bufaddr.vAddr, 
                                            0);
            cy_ethif_rxBufStatus[(u8tmpintrcntr + (u8tmpcounter*CY_ETH_DEFINE_TOTAL_BD_PER_RXQUEUE))].enBufStatus = CY_ETHIF_BUFFER_OCCUPIED;
        }
    }

    /* additional Receive configurations */
    cyp_ethif_gemgxlobj->setCopyAllFrames((void *)cyp_ethif_pd, CY_ETH_ENABLE_1);
    cyp_ethif_gemgxlobj->setRxBadPreamble((void *)cyp_ethif_pd, CY_ETH_ENABLE_1);

    /* Do not drop frames with CRC error */
    cyp_ethif_gemgxlobj->setIgnoreFcsRx((void *)cyp_ethif_pd, CY_ETH_ENABLE_1);

    // Optional: Setting Filter configuration
    // Optional: setting screen registers

    /* Enable MDIO */
    cyp_ethif_gemgxlobj->setMdioEnable((void *)(void *)cyp_ethif_pd, CY_ETH_ENABLE_1);
    /* driver start */
    cyp_ethif_gemgxlobj->start((void *)cyp_ethif_pd);
    
    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_Init
****************************************************************************//**
*
* \brief Function loads callback functions to its local data structures
*
* \param cbFuncsList pointer to callback function list
*
*******************************************************************************/
void Cy_ETHIF_RegisterCallbacks(cy_stc_ethif_cb_t *cbFuncsList)
{
    /* Load Callback functions */
    stccallbackfunctions.rxframecb       = cbFuncsList->rxframecb;
    stccallbackfunctions.txerrorcb       = cbFuncsList->txerrorcb;
    stccallbackfunctions.txcompletecb    = cbFuncsList->txcompletecb;
    stccallbackfunctions.tsuSecondInccb  = cbFuncsList->tsuSecondInccb;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_TransmitFrame
****************************************************************************//**
*
* \brief Function initiates transmission based on passed arguments
*
* \param pu8TxBuffer pointer to Transmit source buffer
*        u16Length Length of data to transmit from source buffer, Length should include source
*                  and destination buffer address. CRC bytes shall not be included in the length
*        u8QueueIndex Queue to be used to transmit the frame
*        bEndBuffer True - Last buffer of the frame to be transmitted
*                   False - Other Buffers to be provided after function call
*                           IP will not start transmitting until it gets EndBuffer True
*
* \param u16Length u16Length
*
* \param u8QueueIndex u8QueueIndex
*
* \param bEndBuffer bEndBuffer
*
* \return CY_ETHIF_SUCCESS Frame transmission started
*         CY_ETHIF_BAD_PARAM Parameter passed contains invalid values
*         CY_ETHIF_BUFFER_NOT_AVAILABLE Buffer not available to load the source data
* 
* \note
* 
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_TransmitFrame(uint8_t * pu8TxBuffer, uint16_t u16Length, uint8_t u8QueueIndex, bool bEndBuffer)
{
    uint8_t u8BufferIndex = 0;
    uint8_t * pu8LocalBuf = 0;
    bool bTransmitBuffer = true;
    bool bClearAll = false;
    uint8_t u8flags = 0;
    // uint16_t u16idx = 0;
    uint32_t u32result = 0;

    /* Check for arguments */
    if ((pu8TxBuffer == NULL) || (u16Length > CY_ETH_SIZE_MAX_FRAME) || (u8QueueIndex > CY_ETH_QS2_2))
    {
        return CY_ETHIF_BAD_PARAM;
    }

    /* Check requested queue enabled or not */
    if (stcQueueDisStatus.bTxQueueDisable[u8QueueIndex] == true)
    {
        /* Requested Queue is disabled */
        return CY_ETHIF_BAD_PARAM;
    }

    /* Get available Tx Buffer from the Transmit buffer Pool */
    u8BufferIndex = Cy_ETHIF_GetBuf(bTransmitBuffer);
    if (CY_ETHIF_NO_BUFFER_AVAILABLE == u8BufferIndex)
    {
        /* check for first released buffer */
        u8BufferIndex = Cy_ETHIF_ClearReleasedBuf(bClearAll, bTransmitBuffer);
        if (CY_ETHIF_NO_BUFFER_AVAILABLE == u8BufferIndex)
        {
            return CY_ETHIF_BUFFER_NOT_AVAILABLE;     // No buffer available
        }
    }

    /* Load the buffer address of available Buffer */
    pu8LocalBuf = (uint8_t *)cy_ethif_txBufStatus[u8BufferIndex].cy_ethif_bufaddr.vAddr;
    
    /* change buffer's status to OCCUPIED, Buffer will be released in case of error or after Tx complete interrupt occurs */
    cy_ethif_txBufStatus[u8BufferIndex].enBufStatus = CY_ETHIF_BUFFER_OCCUPIED;
    
    /* Copy data to un-cached aligned EthIf Tx buffer */
    memcpy((&pu8LocalBuf[0]), &pu8TxBuffer[0], u16Length);

    /* Typecast bEndBuffer to Flag type    */
    // TODO: adapt function for CEDI_TXB_NO_AUTO_CRC and CEDI_TXB_NO_AUTO_START
    if (bEndBuffer == true)
    {
        u8flags = CEDI_TXB_LAST_BUFF;
    }

    /* Clear transmit status register before begin to transmit */
    cyp_ethif_gemgxlobj->clearTxStatus((void *)cyp_ethif_pd, CY_ETHIF_TX_STATUS_CLEAR);

    /* Trigger Internal transmit function  */
    u32result = cyp_ethif_gemgxlobj->queueTxBuf((void *)cyp_ethif_pd,
                                       u8QueueIndex,
                                       &cy_ethif_txBufStatus[u8BufferIndex].cy_ethif_bufaddr,
                                       u16Length,
                                       u8flags);
    if (u32result != 0)
    {
        /* error   */
        cy_ethif_txBufStatus[u8BufferIndex].enBufStatus = CY_ETHIF_BUFFER_RELEASED;
        return CY_ETHIF_BAD_PARAM;
    }

    // TODO: wait until tx done successfully?

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_TxPauseFrame
****************************************************************************//**
*
* \brief Transmits IEEE 802.3X standard Pause Frame 
*
* \param bZeroTQ  True - Zero Time Quanta 
*        bZeroTQ  False - Transmit pause frame with set time quanta
*
* \return none
* \note: If interrupt enabled, interrupt will be triggered at the end of the transmission
*         
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_TxPauseFrame(bool bZeroTQ)
{
    /* check for arguments */

    if (bZeroTQ == true)
    {
        /* trigger Pause frame with Zero Time Quanta */
        cyp_ethif_gemgxlobj->txZeroQPause((void *)cyp_ethif_pd);
    }
    else
    {
        cyp_ethif_gemgxlobj->txPauseFrame((void *)cyp_ethif_pd);
    }

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_ConfigPause
****************************************************************************//**
*
* \brief Configures the Pause Frame transmission according to IEEE 802.3X standard
*
* \param u16PauseQuanta Time Quanta
*
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_ConfigPause(uint16_t u16PauseQuanta)
{
    /* Set Tx Pause Quanta for Priority 0   */
    if (EOK != cyp_ethif_gemgxlobj->setTxPauseQuantum((void *)cyp_ethif_pd, u16PauseQuanta, CY_ETHIF_PAUSE_P0))
    {
        return CY_ETHIF_BAD_PARAM;
    }

    /* Enable Receive Pause Frames */
    cyp_ethif_gemgxlobj->setCopyPauseDisable((void *)cyp_ethif_pd, CY_ETH_DISABLE_0);

    /* Enable Pause Frames */
    cyp_ethif_gemgxlobj->setPauseEnable((void *)cyp_ethif_pd, CY_ETH_ENABLE_1);

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_SetNoBroadCast
****************************************************************************//**
*
* \brief Disable/Enable receipt of broadcast frames
*
* \param rejectBC if =0 broadcasts are accepted, if =1 they are rejected.
* 
*******************************************************************************/
void Cy_ETHIF_SetNoBroadCast(bool rejectBC)
{
    cyp_ethif_gemgxlobj->setNoBroadcast((void *)cyp_ethif_pd,rejectBC);
}


/*******************************************************************************
* Function Name: Cy_EthIf_SetCopyAllFrames
****************************************************************************//**
*
* \brief Enable/Disable copy all frames mode
*
* \param  toBeEnabled if =1 enables copy all frames mode, if =0 then this is disabled
* 
*******************************************************************************/
void Cy_ETHIF_SetCopyAllFrames(bool toBeEnabled)
{
    cyp_ethif_gemgxlobj->setCopyAllFrames((void *)cyp_ethif_pd,toBeEnabled);
}

/*******************************************************************************
* Function : Cy_ETHIF_SetFilterAddress
****************************************************************************//**
*
* \brief Set Filter Address with specific filter number
*
* \param  filterNo Filter number
*         config Filter configuration for Ethernet MAC
*
* \param  config config
*
* \return CY_ETHIF_SUCCESS Filter is set
*         CY_ETHIF_BAD_PARAM Parameter passed contains invalid values
*
* \note
* Maximum 4 filters can be set
*
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_SetFilterAddress(cy_en_ethif_filter_num_t filterNo, const cy_stc_ethif_filter_config_t* config)
{
    if(filterNo >= CY_ETH_FILTER_NUM_INV)
    {
        return CY_ETHIF_BAD_PARAM;
    }

    if(config == NULL)
    {
        return CY_ETHIF_BAD_PARAM;
    }

    /* add some address filters */
    cyp_ethif_gemgxlobj->setSpecificAddr((void *)cyp_ethif_pd,
                                        filterNo,
                                        (CEDI_MacAddress*)&config->filterAddr,
                                        config->typeFilter, 
                                        config->ignoreBytes);

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_PhyRegRead
****************************************************************************//**
*
* \brief Local function used by other APIs to read the PHY register
*
* \param u8RegNo Register to read
*
* \param u8PHYAddr u8PHYAddr
*
* \return read data from the register
*
* \note
* CY_ETH_PHY_ADDR must match with PHY.
* PHY data-sheet and hardware schematic shall be checked
*
*******************************************************************************/
uint32_t Cy_ETHIF_PhyRegRead(uint8_t u8RegNo, uint8_t u8PHYAddr)
{
    uint32_t    u32result;
    uint16_t    u16ReadData;
    uint32_t    nw_status;

    cyp_ethif_gemgxlobj->phyStartMdioRead( (void *)cyp_ethif_pd, CY_ETHIF_PHY_FLAG, u8PHYAddr, u8RegNo);
    while(1)
    {
        nw_status = cyp_ethif_gemgxlobj->getMdioIdle((void *)cyp_ethif_pd);
        if (nw_status != CY_ETH_MDIO_BUSY_0)
            break;
    }

    /* additional wait */
    Cy_SysLib_DelayUs(800);            // 800us delay

    u32result = cyp_ethif_gemgxlobj->getMdioReadData((void *)cyp_ethif_pd, &u16ReadData);
    if (u32result!=0) 
    {
        // debug_printf("[ETH] PHY register read not success.\r\n");
    }

    return (uint32_t)u16ReadData;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_PhyRegWrite
****************************************************************************//**
*
* \brief Local function used by other APIs to write the PHY register
*
* \param u8RegNo Register to write
* \param u16Data data to write
* \param u8PHYAddr u8PHYAddr
* \return 
*
* \note
* u8PHYAddr must match with PHY.
* PHY data-sheet and hardware schematic shall be checked 
*
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_PhyRegWrite(uint8_t u8RegNo, uint16_t u16Data, uint8_t u8PHYAddr)
{
    cyp_ethif_gemgxlobj->phyStartMdioWrite( (void *)cyp_ethif_pd, CY_ETHIF_PHY_FLAG, u8PHYAddr, u8RegNo, u16Data );
    while (cyp_ethif_gemgxlobj->getMdioIdle((void *)cyp_ethif_pd) != CY_ETH_MDIO_BUSY_0);

    /* additional wait */
    Cy_SysLib_DelayUs(200);       // 200usec
    return CY_ETHIF_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_ETHIF_ClearReleasedBuf
****************************************************************************//**
*
* \brief Each buffer goes through cycle of FREE - OCCUPIED - RELEASED - FREE 
* Function looks into status of each buffer and reset them to default values if status is RELEASED
*
* \param bClearAll  True  - Free all buffers which have status "CY_ETHIF_BUFFER_RELEASED"
*                     False - Only first buffer found to be "CY_ETHIF_BUFFER_RELEASED" will be free
*        bTransmitBuf True  - Free buffer from Transmit pool
*                     False - Free buffer from Receive pool
*
* \param bTransmitBuf bTransmitBuf
*
* \return CY_ETHIF_BUFFER_AVAILABLE when Free all released buffers 
*         CY_ETHIF_NO_BUFFER_AVAILABLE No Buffer in a state of "CY_ETHIF_BUFFER_RELEASED"
*         u8tmpcounter in case of bClearAll to false, function will break right after locating first released buffer
*         
*******************************************************************************/
uint8_t Cy_ETHIF_ClearReleasedBuf (bool bClearAll, bool bTransmitBuf)
{
    bool bBufferReleased = false;

    if (bTransmitBuf)
    {
        for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_TX_BUF; u8tmpcounter++)
        {
            if (cy_ethif_txBufStatus[u8tmpcounter].enBufStatus == CY_ETHIF_BUFFER_RELEASED)
            {
                Cy_ETHIF_ClearBuffer((uint32_t *)cy_ethif_txBufStatus[u8tmpcounter].cy_ethif_bufaddr.vAddr);
                cy_ethif_txBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_FREE;
                
                bBufferReleased = true;
                if (bClearAll == false)
                {
                    return u8tmpcounter;
                }
            }
        }
    }
    else
    {
        for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_RX_BUF; u8tmpcounter++)
        {
            if (cy_ethif_rxBufStatus[u8tmpcounter].enBufStatus == CY_ETHIF_BUFFER_RELEASED)
            {
                Cy_ETHIF_ClearBuffer((uint32_t *)cy_ethif_rxBufStatus[u8tmpcounter].cy_ethif_bufaddr.vAddr);
                cy_ethif_rxBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_FREE;
                
                bBufferReleased = true;
                if (bClearAll == false)
                {
                    return u8tmpcounter;
                }
            }
        }
    }

    if (bClearAll && bBufferReleased)
        return CY_ETHIF_BUFFER_AVAILABLE;
    else
        return CY_ETHIF_NO_BUFFER_AVAILABLE;
}


/*******************************************************************************
* Function Name: Cy_ETHIF_GetTimerValue
****************************************************************************//**
*
* \brief Returns the current timer value from TSU register
*
* \param pstcRetTmrValue [out] pointer to data structure to return the values
*
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_Get1588TimerValue(CEDI_1588TimerVal* pstcRetTmrValue)
{
    if (pstcRetTmrValue == NULL)
    {
        return CY_ETHIF_BAD_PARAM;
    }

    cyp_ethif_gemgxlobj->get1588Timer((void *)cyp_ethif_pd, pstcRetTmrValue);

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_SetTimerValue
****************************************************************************//**
*
* \brief Setting the current timer value in TSU register
*
* \param pstcTmrValue pointer to data structure to configure register with
*
*******************************************************************************/
cy_en_ethif_status_t Cy_ETHIF_Set1588TimerValue(CEDI_1588TimerVal * pstcTmrValue)
{
    if (pstcTmrValue == NULL)
    {
        return CY_ETHIF_BAD_PARAM;
    }

    if (EOK != cyp_ethif_gemgxlobj->set1588Timer((void *)cyp_ethif_pd, pstcTmrValue))
    {
        /* Reason could be Null pointer, hardware does not support TSU or pstcTimerValue.nanosecs>0x3FFFFFFF */
        return CY_ETHIF_BAD_PARAM;
    }

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_DecodeEvent
****************************************************************************//**
*
* \brief Interrupt handler for Ethernet instance ETH0
* Function calls isr function to decode the event. 
*
*******************************************************************************/
void Cy_ETHIF_DecodeEvent(void)
{
//    volatile uint32_t        u32result;
    
    cyp_ethif_gemgxlobj->isr((void *)cyp_ethif_pd);
    //if (u32result!=0) debug_printf("NG4 (0x%08x)\r\n", result);

    return;
}


/*############################################################################*/

/*******************************************************************************
* Function Name: Cy_ETHIF_EnableInterrupts
****************************************************************************//**
*
* \brief Function initializes enabled Interrupt
*
* \param pstcInterruptList pointer to structure list 
* 
*******************************************************************************/
static void Cy_ETHIF_EnableInterrupts (cy_stc_ethif_interruptconfig_t * pstcInterruptList)
{
    uint32_t u32InterruptEn = 0;
    
    if (pstcInterruptList->btsu_time_match == true)
        u32InterruptEn |= CEDI_EV_TSU_TIME_MATCH;    
    if (pstcInterruptList->bwol_rx == true)
        u32InterruptEn |= CEDI_EV_WOL_RX;    
    if (pstcInterruptList->blpi_ch_rx == true)
        u32InterruptEn |= CEDI_EV_LPI_CH_RX;    
    if (pstcInterruptList->btsu_sec_inc == true)
        u32InterruptEn |= CEDI_EV_TSU_SEC_INC;    
    if (pstcInterruptList->bptp_tx_pdly_rsp == true)
        u32InterruptEn |= CEDI_EV_PTP_TX_PDLY_RSP;
    if (pstcInterruptList->bptp_tx_pdly_req == true)
        u32InterruptEn |= CEDI_EV_PTP_TX_PDLY_REQ;    
    if (pstcInterruptList->bptp_rx_pdly_rsp == true)
        u32InterruptEn |= CEDI_EV_PTP_RX_PDLY_RSP;    
    if (pstcInterruptList->bptp_rx_pdly_req == true)
        u32InterruptEn |= CEDI_EV_PTP_RX_PDLY_REQ;    
    if (pstcInterruptList->bptp_tx_sync == true)
        u32InterruptEn |= CEDI_EV_PTP_TX_SYNC;    
    if (pstcInterruptList->bptp_tx_dly_req == true)
        u32InterruptEn |= CEDI_EV_PTP_TX_DLY_REQ;    
    if (pstcInterruptList->bptp_rx_sync == true)
        u32InterruptEn |= CEDI_EV_PTP_RX_SYNC;    
    if (pstcInterruptList->bptp_rx_dly_req == true)
        u32InterruptEn |= CEDI_EV_PTP_RX_DLY_REQ;    
    if (pstcInterruptList->bpause_frame_tx == true)
        u32InterruptEn |= CEDI_EV_PAUSE_FRAME_TX;
    if (pstcInterruptList->bpause_time_zero == true)
        u32InterruptEn |= CEDI_EV_PAUSE_TIME_ZERO;
    if (pstcInterruptList->bpause_nz_qu_rx == true)
        u32InterruptEn |= CEDI_EV_PAUSE_NZ_QU_RX;
    if (pstcInterruptList->bhresp_not_ok == true)
        u32InterruptEn |= CEDI_EV_HRESP_NOT_OK;
    if (pstcInterruptList->brx_overrun == true)
        u32InterruptEn |= CEDI_EV_RX_OVERRUN;
    if (pstcInterruptList->btx_complete == true)
        u32InterruptEn |= CEDI_EV_TX_COMPLETE;
    if (pstcInterruptList->btx_fr_corrupt == true)
        u32InterruptEn |= CEDI_EV_TX_FR_CORRUPT;
    if (pstcInterruptList->btx_retry_ex_late_coll == true)
        u32InterruptEn |= CEDI_EV_TX_RETRY_EX_LATE_COLL;
    if (pstcInterruptList->btx_underrun == true)
        u32InterruptEn |= CEDI_EV_TX_UNDERRUN;
    if (pstcInterruptList->btx_used_read == true)
        u32InterruptEn |= CEDI_EV_TX_USED_READ;
    if (pstcInterruptList->brx_used_read == true)
        u32InterruptEn |= CEDI_EV_RX_USED_READ;
    if (pstcInterruptList->brx_complete == true)
        u32InterruptEn |= CEDI_EV_RX_COMPLETE;
    if (pstcInterruptList->bman_frame == true)
        u32InterruptEn |= CEDI_EV_MAN_FRAME;  
    
    cy_ethif_cfg.intrEnable = u32InterruptEn;    
}

/*******************************************************************************
* Function Name: Cy_ETHIF_PrepareConfiguration
****************************************************************************//**
*
* \brief prepares configuration based on the parameter passed to Cy_ETHIF_Init
* function. This prepared configuration which is dedicated for specific Ethernet
* instance would be used to initialize Ethernet MAC
*
* \param config Pointer to Ethernet configuration passed from Application layer 
* 
* \Note:
*
*******************************************************************************/
static void Cy_ETHIF_PrepareConfiguration( cy_stc_ethif_config_t * config )
{
    uint8_t u8QueueCounter = 0;

    /* Clear configuration table */
    memset((void *)&cy_ethif_cfg, 0, sizeof(cy_ethif_cfg));

    /* Load GEM_GXL register base address */
    cy_ethif_cfg.regBase = CY_ETH0_GEMGXL_ADDR_REGBASE;

    /* Prepare Queues */
    cy_ethif_cfg.rxQs = CY_ETH_DEFINE_NUM_RXQS;
    cy_ethif_cfg.txQs = CY_ETH_DEFINE_NUM_TXQS;

    for (u8QueueCounter=0; u8QueueCounter<cy_ethif_cfg.rxQs; u8QueueCounter++) {
        cy_ethif_cfg.rxQLen[u8QueueCounter] = CY_ETH_DEFINE_TOTAL_BD_PER_RXQUEUE;
    }

    for (u8QueueCounter=0; u8QueueCounter<cy_ethif_cfg.txQs; u8QueueCounter++) {
        cy_ethif_cfg.txQLen[u8QueueCounter] = CY_ETH_DEFINE_TOTAL_BD_PER_TXQUEUE;
    }

    /* Prepare Network control Register */
    cy_ethif_cfg.pfcMultiQuantum    = 0;                                        /* bit25    pfc_ctrl              */
    cy_ethif_cfg.enExtTsuPort       = 0;                                        /* bit23    ext_tsu_port_enable, Not supported by hardware  */
    cy_ethif_cfg.storeUdpTcpOffset  = 0;                                        /* bit22    store_udp_offset      */
    cy_ethif_cfg.altSgmiiEn         = 0;                                        /* bit21    alt_sgmii_mode, Not supported by hardware       */
    cy_ethif_cfg.enableMdio         = 0;                                        /* bit4     man_port_en, Bit is separately enabled during MDIO operation  */ 

    /* Prepare Network Configuration Register */
    cy_ethif_cfg.uniDirEnable       = 0;                                        /* bit31    uni_direction_enable, Not supported by hardware    */
    cy_ethif_cfg.ignoreIpgRxEr      = 0;                                        /* bit30    ignore_ipg_rx_er, Not supported by hardware        */
    cy_ethif_cfg.enRxBadPreamble    = config->u8enRxBadPreamble;      /* bit29    nsp_change           */
    // cy_ethif_cfg[u8EthIfInstance].ifTypeSel          = pstcEthIfConfig->ifTypeSel;              /* bit27    sgmii_mode_enable  (reserved)  */
                                                                                                /*          (see the following)  */
    // don't care                                                                               /* bit26    ignore_rx_fcs        */
    cy_ethif_cfg.enRxHalfDupTx      = 0;                                        /* bit25    en_half_duplex_rx, not supported by hardware    */
    cy_ethif_cfg.chkSumOffEn        = config->u8chkSumOffEn;                    /* bit24    receive_checksum_offload_enable */
    cy_ethif_cfg.disCopyPause       = config->u8disCopyPause;                   /* bit23    disable_copy_of_pause_frames */
    if(ETH_AXI_MASTER_PRESENT == 1)
    {
        cy_ethif_cfg.dmaBusWidth        = CEDI_DMA_BUS_WIDTH_64;                /* bit22:21 data bus with        */
    }
    else
    {
        cy_ethif_cfg.dmaBusWidth        = CEDI_DMA_BUS_WIDTH_32;                /* bit22:21 data bus with        */
    }
                                                                                /* 00:32bit 01:64bit    */
    cy_ethif_cfg.mdcPclkDiv         = config->mdcPclkDiv;                       /* bit20:18 mdc_clock_division   */
                                                                                                /*          010: Divide 32       */
                                                                                                /*          011: Divide 48       */
    // don't care                                                                               /* bit17    fcs_remove           */
    cy_ethif_cfg.rxLenErrDisc       = config->u8rxLenErrDisc;                   /* bit16    length_field_error_frame_discard */
    cy_ethif_cfg.rxBufOffset        = 0;                                        /* bit15:14 receive_buffer_offset */
    // don't care                                                                               /* bit13    pause_enable         */
    // don't care                                                                               /* bit12    retry_test           */
    //cy_ethif_cfg[u8EthIfInstance].ifTypeSel        = pstcEthIfConfig->ifTypeSel               /* bit11    pcs_select (reserved)*/
                                                                                                /*          (see the following)  */
    //cy_ethif_cfg[u8EthIfInstance].ifTypeSel        = pstcEthIfConfig->ifTypeSel               /* bit10    gigabit_mode_enable  */
                                                                                                /*          (see the following)  */
    cy_ethif_cfg.extAddrMatch       = 0;                                        /* bit9     external_address_match_enable, not supported by hardware */
    cy_ethif_cfg.rx1536ByteEn       = config->u8rx1536ByteEn;                   /* bit8     receive_1536_byte_frames */
    // don't care                                                                               /* bit7     unicast_hash_enable  */
    // don't care                                                                               /* bit6     multicast_hash_enable  */
    // don't care                                                                               /* bit5     no_broadcast         */
    // don't care                                                                               /* bit4     copy_all_frames      */
    cy_ethif_cfg.rxJumboFrEn        = config->u8rxJumboFrEn;                    /* bit3     jumbo_frames         */
    // don't care                                                                               /* bit2     discard_non_vlan_frames */
    cy_ethif_cfg.fullDuplex         = 1;                                        /* bit1     full_duplex          */
    //cy_ethif_cfg[u8EthIfInstance].ifTypeSel        = pstcEthIfConfig->ifTypeSel               /* bit0     speed                */

    /* configuration for cy_ethif_cfg[u8EthIfInstance].ifTypeSel */
    if ((config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_MII_10) ||
       (config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RGMII_10) ||
       (config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RMII_10))
    {
       cy_ethif_cfg.ifTypeSel = CEDI_IFSP_10M_MII;
    }
    else if ((config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_MII_100) ||
        (config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RMII_100) ||
        (config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RGMII_100))
    {
        cy_ethif_cfg.ifTypeSel = CEDI_IFSP_100M_MII;
    }
    else if ((config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_GMII_1000) ||
            (config->pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RGMII_1000))
    {
        cy_ethif_cfg.ifTypeSel = CEDI_IFSP_1000M_GMII;
    }
    
    /*=================================================================================================*/
    /* CTL.ETH_MODE   | Network_config[0] |  Network_config[10]   |     PHY mode                       */
    /*                |     (speed)       | (gigabit_mode_enable) |                                    */
    /*=================================================================================================*/
    /*       2'd0   |          0        |          0            |   MII - 10Mbps                     */
    /*       2'd0   |          1        |          0            |   MII - 100Mbps                    */
    /*       2'd1   |          0        |          1            |   GMII - 1000Mbps                  */
    /*       2'd2   |          0        |          0            |   RGMII - 10Mbps (4bits/Cycle)     */
    /*       2'd2   |          1        |          0            |   RGMII - 100Mbps (4bits/Cycle)    */
    /*       2'd2   |          0        |          1            |   RGMII - 1000Mbps (8bits/Cycle)   */
    /*       2'd3   |          0        |          0            |   RMII - 10Mbps                    */
    /*       2'd3   |          1        |          0            |   RMII - 100Mbps                   */
    /*=================================================================================================*/
    
    /* Prepare DMA Config register */
    cy_ethif_cfg.dmaAddrBusWidth    = 0;                                        /* bit30    dma_addr_bus_width_1    */
                                                                                                /* 0:32bit 1:64bit         */
    cy_ethif_cfg.enTxExtBD          = CY_ETH_DEFINE_BD;                               /* bit29  tx_bd_extended_mode_en  */
    cy_ethif_cfg.enRxExtBD          = CY_ETH_DEFINE_BD;                               /* bit28  rx_bd_extended_mode_en  */
    cy_ethif_cfg.dmaCfgFlags        = config->u8dmaCfgFlags;                    /* bit26  force_max_amba_burst_tx */
                                                                                                /* bit25  force_max_amba_burst_rx */
                                                                                                /* bit24  force_discard_on_err    */
    for (u8QueueCounter=0; u8QueueCounter<cy_ethif_cfg.rxQs; u8QueueCounter++) {
        cy_ethif_cfg.rxBufLength[u8QueueCounter] = CY_ETH_SIZE_BUF_TXQ_RXQ >> 6;            /* bit23:16 rx_buf_size       */
    }
    
    cy_ethif_cfg.txPktBufSize       = CY_ETH_TX_PBUF_SIZE;                      /* bit10  tx_pbuf_size            */
    cy_ethif_cfg.rxPktBufSize       = CY_ETH_RX_PBUF_SIZE;                      /* bit9:8 rx_pbuf_size            */
    cy_ethif_cfg.dmaEndianism       = 0;                                        /* bit7   endian_swap_packet  */
                                                                                /*        0: little endian mode */
                                                                                /*        1: endian swap mode enable for packet data (CEDI_END_SWAP_DATA) */
                                                                                /* bit6   endian_swap_management   */
                                                                                /*        0: little endian mode    */
                                                                                /*        1: endian swap mode enable for management descriptor (CEDI_END_SWAP_DESC) */
    cy_ethif_cfg.dmaDataBurstLen    = config->dmaDataBurstLen;                  /* bit4:0   amba_burst_length                                         */
                                                                                /* 1xxxx: attempt use burst up to 16 (CEDI_DMA_DBUR_LEN_16)  */
                                                                                /* 01xxx: attempt use burst up to  8 (CEDI_DMA_DBUR_LEN_8)   */
                                                                                /* 001xx: attempt use burst up to  4 (CEDI_DMA_DBUR_LEN_4)   */
                                                                                /* 0001x: always use single burst                            */
                                                                                /* 00001: always use single burst    (CEDI_AMBD_BURST_LEN_1) */
                                                                                /* 00000: best AXI burst up to 256 beats                     */
                                       
    /* Prepare ddr and upper_rx_q_base_addr register (0x4c8, 0x4D4) */
    cy_ethif_cfg.upper32BuffTxQAddr = 0;                                        /* bit31:0  not used              */
    cy_ethif_cfg.upper32BuffRxQAddr = 0;                                        /* bit31:0  not used              */

    /* axi_max_    */
    cy_ethif_cfg.aw2wMaxPipeline    = config->u8aw2wMaxPipeline;                /* bit15:8  aw2w_max_pipeline     */
    cy_ethif_cfg.ar2rMaxPipeline    = config->u8ar2rMaxPipeline;                /* bit 7:0  ar2r_max_pipeline     */
}


/*******************************************************************************
* Function Name: Cy_ETHIF_WrapperConfig
****************************************************************************//**
*
* \brief Function initializes Ethernet Wrapper to configure Interface mode, reference clock/divider etc
*
* \param u8EthIfInstance Ethernet Instance 
* \param pstcInterruptList pointer to structure list 
* 
*******************************************************************************/
static cy_en_ethif_status_t Cy_ETHIF_WrapperConfig( cy_stc_ethif_wrapper_config_t * pstcWrapperConfig )
{
    uint32_t mode=0;

    if (pstcWrapperConfig->stcInterfaceSel > CY_ETHIF_CTL_RMII_100)
    {
        return CY_ETHIF_BAD_PARAM;
    }

    if ((pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_MII_10) ||
       (pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_MII_100))
    {
        mode = 0;
    }
    else if (pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_GMII_1000)
    {
        mode = 1;
    }
    else if ((pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RGMII_10) ||
            (pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RGMII_100) ||
            (pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RGMII_1000))
    {
        mode = 2;
    }
    else if ((pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RMII_10) ||
            (pstcWrapperConfig->stcInterfaceSel == CY_ETHIF_CTL_RMII_100))
    {
        mode = 3;
    }

    ETH_CTL = (ETH_CTL & (uint32_t)~(ETH_CTL_ETH_MODE_Msk |
                            ETH_CTL_REFCLK_SRC_SEL_Msk |
                            ETH_CTL_REFCLK_DIV_Msk)) |
                _VAL2FLD(ETH_CTL_ETH_MODE,mode) |
                _VAL2FLD(ETH_CTL_REFCLK_SRC_SEL, pstcWrapperConfig->bRefClockSource) |
                _VAL2FLD(ETH_CTL_REFCLK_DIV, (pstcWrapperConfig->u8RefClkDiv - 1));

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_IPEnable
****************************************************************************//**
*
* \brief Function enables Ethernet MAC
*
* \param
* 
*******************************************************************************/
static void Cy_ETHIF_IPEnable(void)
{
    ETH_CTL |=_VAL2FLD(ETH_CTL_ENABLED, CY_ETH_ENABLE_1);
}

/*******************************************************************************
* Function Name: Cy_ETHIF_IPDisable
****************************************************************************//**
*
* \brief Function Disables Ethernet MAC
*
* \param
* 
*******************************************************************************/
static void Cy_ETHIF_IPDisable(void)
{
    ETH_CTL &= (uint32_t)~(ETH_CTL_ENABLED_Msk);
}

/*******************************************************************************
* Function Name: Cy_ETHIF_AssignMemory
****************************************************************************//**
*
* \brief Local function is used to initialize private data structure, tx and rx queue start address 
*
* \param
*
*******************************************************************************/
static void Cy_ETHIF_AssignMemory(void)
{
    cyp_ethif_pd = (CEDI_PrivateData *)cy_ethif_privatedata;
    cy_ethif_cfg.rxQAddr   = (uintptr_t)cy_ethif_rx_desc_list;
    cy_ethif_cfg.txQAddr   = (uintptr_t)cy_ethif_tx_desc_list;
    cyp_ethif_statistic = (CEDI_Statistics *)cy_ethif_statistic;
    cy_ethif_cfg.statsRegs = (uintptr_t)cyp_ethif_statistic;
    /** get the physical address */
    cy_ethif_cfg.rxQPhyAddr = cy_ethif_cfg.rxQAddr;
    /** get the physical address */
    cy_ethif_cfg.txQPhyAddr = cy_ethif_cfg.txQAddr;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_DisableQueues
****************************************************************************//**
* \brief By default all activated queues in the IP are enabled. However, only required 
* queues for tx and rx shall be enabled to make internal process faster. 
*
* \brief Function Disables Ethernet MAC
*
* \param u8EthIfInstance Ethernet Instance 
* 
*******************************************************************************/
static cy_en_ethif_status_t Cy_ETHIF_DisableQueues (cy_stc_ethif_config_t * pstcEthIfConfig)
{
    uint8_t u8TxQueueCounter = 0;
    uint8_t u8RxQueueCounter = 0;

    /* Transmit Queue 0 */
    if (pstcEthIfConfig->btxq0enable == false)
    {
        /* Disable the queue pointer */
        ETH_TX_Q_PTR = (ETH_TX_Q_PTR&(uint32_t)~(ETH_TRANSMIT_Q_PTR_DMA_TX_DIS_Q_Msk)) | _VAL2FLD(ETH_TRANSMIT_Q_PTR_DMA_TX_DIS_Q, 1UL);
        stcQueueDisStatus.bTxQueueDisable[0] = true;
    }
    else
        u8TxQueueCounter++;

    /* Transmit Queue 1 */
    if (pstcEthIfConfig->btxq1enable == false)
    {
        /* Disable the queue pointer */
        ETH_TX_Q1_PTR = (ETH_TX_Q1_PTR&(uint32_t)~(ETH_TRANSMIT_Q1_PTR_DMA_TX_DIS_Q_Msk)) | _VAL2FLD(ETH_TRANSMIT_Q1_PTR_DMA_TX_DIS_Q, 1UL);
        stcQueueDisStatus.bTxQueueDisable[1] = true;
    }
    else
        u8TxQueueCounter++;

    /* Transmit Queue 2 */
    if (pstcEthIfConfig->btxq2enable == false)
    {
        /* Disable the queue pointer */
        ETH_TX_Q2_PTR = (ETH_TX_Q2_PTR&(uint32_t)~(ETH_TRANSMIT_Q2_PTR_DMA_TX_DIS_Q_Msk)) | _VAL2FLD(ETH_TRANSMIT_Q2_PTR_DMA_TX_DIS_Q, 1UL);
        stcQueueDisStatus.bTxQueueDisable[2] = true;
    }
    else
        u8TxQueueCounter++;

    /* Receive Queue 0 */
    if (pstcEthIfConfig->brxq0enable == false)
    {
        /* Disable the queue pointer */
        ETH_RX_Q_PTR = (ETH_RX_Q_PTR&(uint32_t)~(ETH_RECEIVE_Q_PTR_DMA_RX_DIS_Q_Msk)) | _VAL2FLD(ETH_RECEIVE_Q_PTR_DMA_RX_DIS_Q, 1UL);
        stcQueueDisStatus.bRxQueueDisable[0] = true;
    }
    else
        u8RxQueueCounter++;

    /* Receive Queue 1 */
    if (pstcEthIfConfig->brxq1enable == false)
    {
        /* Disable the queue pointer */
        ETH_RX_Q1_PTR = (ETH_RX_Q1_PTR&(uint32_t)~(ETH_RECEIVE_Q1_PTR_DMA_RX_DIS_Q_Msk)) | _VAL2FLD(ETH_RECEIVE_Q1_PTR_DMA_RX_DIS_Q, 1UL);
        stcQueueDisStatus.bRxQueueDisable[1] = true;
    }
    else
        u8RxQueueCounter++;

    /* Receive Queue 2 */
    if (pstcEthIfConfig->brxq2enable == false)
    {
        /* Disable the queue pointer */
        ETH_RX_Q2_PTR = (ETH_RX_Q2_PTR&(uint32_t)~(ETH_RECEIVE_Q2_PTR_DMA_RX_DIS_Q_Msk)) | _VAL2FLD(ETH_RECEIVE_Q2_PTR_DMA_RX_DIS_Q, 1UL);
        stcQueueDisStatus.bRxQueueDisable[2] = true;
    }
    else    
        u8RxQueueCounter++;

    /* TODO: Temporarily in the driver. MUST be removed once IP is updated || Q3 for both Transmit and Receive has been removed from the IP 
        Changes will apply to bitfile after 0513 release */

    /* TODO: Idea of cross checking BD memory vs enabled queues */

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_TSUInit
****************************************************************************//**
*
* \brief Function enables Time stamp unit in EMAC
*
* \param pstcTSUConfig Pointer to TSU parameters 
*
* \return
* 
*******************************************************************************/
static cy_en_ethif_status_t Cy_ETHIF_TSUInit (cy_stc_ethif_tsu_config_t * pstcTSUConfig)
{
    /* set 1588 timer value */
    /* Load Timer Value */
    if (EOK != cyp_ethif_gemgxlobj->set1588Timer((void *)cyp_ethif_pd, pstcTSUConfig->pstcTimerValue))
    {
        /* Reason could be Null pointer, hardware does not support TSU or pstcTimerValue.nanosecs>0x3FFFFFFF */
        return CY_ETHIF_BAD_PARAM;
    }

    /* Timer increment register to achieve 1 second as precise as possible */
    if (EOK != cyp_ethif_gemgxlobj->set1588TimerInc((void *)cyp_ethif_pd, pstcTSUConfig->pstcTimerIncValue))
    {
        /* Reason could be Null pointer, hardware does not support TSU */
        return CY_ETHIF_BAD_PARAM;
    }

    /* one step sync enabled    */
    if (EOK != cyp_ethif_gemgxlobj->set1588OneStepTxSyncEnable((void *)cyp_ethif_pd, (uint8_t) pstcTSUConfig->bOneStepTxSyncEnable))
    {
        /* Reason could be Null pointer, hardware does not support TSU or bOneStepTxSyncEnable > 1 */
        return CY_ETHIF_BAD_PARAM;
    }

    /* Set the descriptor time stamp Mode */
    if (EOK != cyp_ethif_gemgxlobj->setDescTimeStampMode((void *)cyp_ethif_pd, 
               pstcTSUConfig->enTxDescStoreTimeStamp, pstcTSUConfig->enRxDescStoreTimeStamp))
    {
        /** Reason could be Null pointer, hardware does not support TSU, enTxDescStoreTimeStamp > CEDI_TX_TS_ALL, enRxDescStoreTimeStamp > CEDI_RX_TS_ALL  */
        return CY_ETHIF_BAD_PARAM;
    }

    /* disabled storing nanosecond in CRC field of received frame */
    if (EOK != cyp_ethif_gemgxlobj->setStoreRxTimeStamp((void *)cyp_ethif_pd, (uint8_t) pstcTSUConfig->bStoreNSinRxDesc))
    {
        /* Reason could be Null pointer, hardware does not support TSU or bStoreNSinRxDesc > 1 */
        return CY_ETHIF_BAD_PARAM;
    }

    return CY_ETHIF_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_InitializeBuffers
****************************************************************************//**
*
* \brief Function initializes the buffer status and clears the memory with
*        CY_EHTIF_EMPTYVALUE value.
*
* \param none
*
*******************************************************************************/
static void Cy_ETHIF_InitializeBuffers (void)
{
    uint8_t u8tmpcounter; 

    // Clear all TX buffers
    for (u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_TX_BUF; u8tmpcounter++)
    {
        cy_ethif_txBufStatus[u8tmpcounter].cy_ethif_bufaddr.vAddr = (uintptr_t) &u8TxBuf[u8tmpcounter][0];
        cy_ethif_txBufStatus[u8tmpcounter].cy_ethif_bufaddr.pAddr = (uintptr_t) &u8TxBuf[u8tmpcounter][0];
        cy_ethif_txBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_FREE; 
        
        // Load Buffer with dummy values
        Cy_ETHIF_ClearBuffer((uint32_t*)&u8TxBuf[u8tmpcounter][0]);
    }

    // Clear all RX buffers
    for (u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_RX_BUF; u8tmpcounter++)
    {
        cy_ethif_rxBufStatus[u8tmpcounter].cy_ethif_bufaddr.vAddr = (uintptr_t) &u8RxBuf[u8tmpcounter][0];
        cy_ethif_rxBufStatus[u8tmpcounter].cy_ethif_bufaddr.pAddr = (uintptr_t) &u8RxBuf[u8tmpcounter][0];
        cy_ethif_rxBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_FREE; 

        // Load Buffer with dummy values
        Cy_ETHIF_ClearBuffer((uint32_t*)&u8RxBuf[u8tmpcounter][0]);
    }
}

/*******************************************************************************
* Function Name: Cy_ETHIF_ClearBuffer
****************************************************************************//**
*
* \brief Initializes buffer with the CY_EHTIF_EMPTYVALUE value 
*
* \param pu32Buffer start address for the buffer
* 
*******************************************************************************/
static void Cy_ETHIF_ClearBuffer (uint32_t * pu32Buffer)
{
    for (uint16_t u16tmpcounter = 0; u16tmpcounter < (CY_ETH_SIZE_MAX_FRAME/4); u16tmpcounter++)
    {
        /* Load Buffer with dummy values */
        pu32Buffer[u16tmpcounter] = CY_EHTIF_EMPTYVALUE;
    }
}

/*******************************************************************************
* Function Name: Cy_ETHIF_GetBuf
****************************************************************************//**
*
* \brief returns the free buffer number which driver can start working with.
*
* \param bTransmitBuf To identify which pool caller wants to find free Buffer from
*
* \return u8invalid CY_ETHIF_NO_BUFFER_AVAILABLE when no buffer is free 
*         u8tmpcounter counter value loaded with the free buffer instance
*
*******************************************************************************/
static uint8_t Cy_ETHIF_GetBuf (bool bTransmitBuf)
{
    uint8_t u8invalid = CY_ETHIF_NO_BUFFER_AVAILABLE;
    static uint8_t u8TxBufferPtr = 0;
    static uint8_t u8RxBufferPtr = 0;

    if (bTransmitBuf)
    {
        for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_TX_BUF; u8tmpcounter++)
        {
            if (cy_ethif_txBufStatus[(u8TxBufferPtr + u8tmpcounter)].enBufStatus == CY_ETHIF_BUFFER_FREE)
            {
                u8tmpcounter = u8TxBufferPtr;

                u8TxBufferPtr++;
                if (u8TxBufferPtr >= CY_ETH_TOTAL_TX_BUF)
                    u8TxBufferPtr = 0;

                return u8tmpcounter;
            }

            u8TxBufferPtr++; 
            if (u8TxBufferPtr >= CY_ETH_TOTAL_TX_BUF)
                u8TxBufferPtr = 0;
        }
    }
    else
    {
        for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_RX_BUF; u8tmpcounter++)
        {
            if (cy_ethif_rxBufStatus[(u8RxBufferPtr + u8tmpcounter)].enBufStatus == CY_ETHIF_BUFFER_FREE)
            {
                u8tmpcounter = u8RxBufferPtr;

                u8RxBufferPtr++;
                if (u8RxBufferPtr >= CY_ETH_TOTAL_RX_BUF)
                    u8RxBufferPtr = 0;

                return u8tmpcounter;
            }

            u8RxBufferPtr++;
            if (u8RxBufferPtr >= CY_ETH_TOTAL_RX_BUF)
                u8RxBufferPtr = 0;
        }
    }

    return u8invalid;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventTx
****************************************************************************//**
*
* \brief Function called by cadence driver upon getting Tx Event
*
* \param pcy_privatedata    Instance specific private data
* \param u32event occurred event
* \param u8qnum Queue number 
*
*******************************************************************************/
static void Cy_ETHIF_EventTx(void *pcy_privatedata, uint32_t u32event, uint8_t u8qnum)
{
    CEDI_TxDescData Tx_DescData;

    if ((u32event&CEDI_EV_TX_COMPLETE) != 0) 
    {
        cyp_ethif_gemgxlobj->freeTxDesc((void *)cyp_ethif_pd, u8qnum, &Tx_DescData);

        /* application callback function */
        if (stccallbackfunctions.txcompletecb != NULL)
        {
            stccallbackfunctions.txcompletecb(u8qnum);
        }

        /* Release the buffer  */
        for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_TX_BUF; u8tmpcounter++)
        {
            if ((uint32_t*)Tx_DescData.bufAdd.pAddr == (uint32_t*)cy_ethif_txBufStatus[u8tmpcounter].cy_ethif_bufaddr.pAddr)
            {
                /* Release the buffer  */
                cy_ethif_txBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_RELEASED;
                break;
            }
        }
    }

    return;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventTxError
****************************************************************************//**
*
* \brief Function called by cadence driver upon getting Tx Error Event
*
* \param pcy_privatedata    Instance specific private data
* \param u32event occurred event
* \param u8qnum Queue number 
*
*******************************************************************************/
static void Cy_ETHIF_EventTxError(void *pcy_privatedata, uint32_t u32event, uint8_t u8qnum)
{
    CEDI_TxDescData Tx_DescData;

    cyp_ethif_gemgxlobj->freeTxDesc((void *)cyp_ethif_pd, u8qnum, &Tx_DescData);

    /** application callback function */
    if (stccallbackfunctions.txerrorcb != NULL)
    {
        stccallbackfunctions.txerrorcb(u8qnum);
    }

    /* Release the buffer */
    for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_TX_BUF; u8tmpcounter++)
    {
        if ((uint32_t*)Tx_DescData.bufAdd.pAddr == (uint32_t*)cy_ethif_txBufStatus[u8tmpcounter].cy_ethif_bufaddr.pAddr)
        {
            cy_ethif_txBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_RELEASED;
            break;
        }
    }

    return;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventRxFrame
****************************************************************************//**
*
* \brief Function called by cadence driver upon getting Rx Event
*
* \param pcy_privatedata    Instance specific private data
* \param u8qnum Queue number 
*
*******************************************************************************/
static void Cy_ETHIF_EventRxFrame(void *pcy_privatedata, uint8_t u8qnum)
{
    uint32_t        u32Result;
    uint32_t        u32RxNum;
    // uint32_t        u32RxStatus = 0;
    CEDI_BuffAddr   TmpBufAddr;
    CEDI_RxDescData Rx_DescData;
    CEDI_RxDescStat Rx_DescStat;
    bool bReceiveBuffer = false;    /* Value must be false for Receive buffers */
    uint8_t u8BufferIndex = 255;
    bool bClearAll = false;

    /* number of used buffers */
    u32RxNum = cyp_ethif_gemgxlobj->numRxUsed((void *)cyp_ethif_pd, u8qnum);

    /** read receive queue */
    while (u32RxNum!=0)
    {
        /* Get the new Buffer to replace the used one  */
        u8BufferIndex = Cy_ETHIF_GetBuf(bReceiveBuffer);
        if (CY_ETHIF_NO_BUFFER_AVAILABLE == u8BufferIndex)
        {
            /* check for first released buffer */
            u8BufferIndex = Cy_ETHIF_ClearReleasedBuf(bClearAll, bReceiveBuffer);
            if (CY_ETHIF_NO_BUFFER_AVAILABLE == u8BufferIndex)
            {
                return;     // No buffer available
            }
        }
        
        TmpBufAddr.vAddr = cy_ethif_rxBufStatus[u8BufferIndex].cy_ethif_bufaddr.vAddr;
        TmpBufAddr.pAddr = cy_ethif_rxBufStatus[u8BufferIndex].cy_ethif_bufaddr.pAddr;
        
        u32Result = cyp_ethif_gemgxlobj->readRxBuf((void *)cyp_ethif_pd,
                                          u8qnum,
                                          &TmpBufAddr,
                                          CY_ETHIF_BUFFER_CLEARED_0,
                                          &Rx_DescData);
        if (u32Result != 0)
        {
            // debug_printf("[ETH] NG4 (0x%08x)\r\n", u32Result);
        }
        else
        {
            /* change buffer status to OCCUPIED */
            cy_ethif_rxBufStatus[u8BufferIndex].enBufStatus = CY_ETHIF_BUFFER_OCCUPIED;
        }
        
        switch (Rx_DescData.status)
        {
          case CEDI_RXDATA_SOF_EOF:     // 0
            /* receive start and end frame */
            cyp_ethif_gemgxlobj->getRxDescStat((void *)cyp_ethif_pd,
                                         Rx_DescData.rxDescStat,
                                         &Rx_DescStat);
                                         
            /* application callback function */
            if (stccallbackfunctions.rxframecb != NULL)
            {
                stccallbackfunctions.rxframecb((uint8_t*)TmpBufAddr.pAddr, Rx_DescStat.bufLen);
            }
            
            /* Release the buffer */
            for (uint8_t u8tmpcounter = 0; u8tmpcounter < CY_ETH_TOTAL_RX_BUF; u8tmpcounter++)
            {
                if ((uint32_t*)TmpBufAddr.pAddr == (uint32_t*)cy_ethif_rxBufStatus[u8tmpcounter].cy_ethif_bufaddr.pAddr)
                {
                    cy_ethif_rxBufStatus[u8tmpcounter].enBufStatus = CY_ETHIF_BUFFER_RELEASED;
                    break;
                }                
            }            
            break;
          case CEDI_RXDATA_SOF_ONLY:    // 1
            /* fragment start */
            // debug_printf("[ETH] (SOF)Don't use fragment yet...\r\n");
            break; //return;
          case CEDI_RXDATA_NO_FLAG:     // 2
            /* fragment */
            // debug_printf("[ETH] (NOF)Don't use fragment yet...\r\n");
            break; //return;
          case CEDI_RXDATA_EOF_ONLY:    // 3
            /* fragment end */
            // debug_printf("[ETH] (EOF)Don't use fragment yet...\r\n");
            break;
          case CEDI_RXDATA_NODATA:      // 4
            /* normal leaving */
            // debug_printf("[ETH] NG5 RXDATA_NODATA\r\n");
            return;     /* from here it breaks while loop   */
          default:
            /* Unknown status */
            break;
        }
        u32RxNum--;
    }
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventRxError
****************************************************************************//**
*
* \brief Function called by cadence driver upon getting Rx Error Event
*
* \param pcy_privatedata    Instance specific private data
* \param u32event occurred event
* \param u8qnum Queue number 
*
*******************************************************************************/
static void Cy_ETHIF_EventRxError(void *pcy_privatedata, uint32_t a_event, uint8_t a_qnum)
{
    //printf("[ETH] (Event) RxError received.\r\n");    
    return;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventPhyManComplete
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventPhyManComplete(void *pcy_privatedata, uint8_t u8read, uint16_t u16ReadData)
{
    // uint32_t    u32temp;
    // uint32_t    u32addr;

    // u32temp = *((volatile uint32_t*)0xXXXX);
    // u32addr = (u32temp & 0x007C0000) >> 18;

    // debug_printf("[ETH] (Event) PhyManComplete.\r\n");
    // debug_printf("[ETH] w/r=%d, addr=0x%02x, data=0x%04x\r\n", (uint32_t)u8read, addr, (uint32_t)u16ReadData);

    return;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventhrespError
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventhrespError(void *pcy_privatedata, uint8_t u8qnum)
{
    // debug_printf("[ETH] (Event) hrespError received.\r\n");
    // debug_printf("[ETH] queue num=%d\r\n", u8qnum);

    return;
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventLpPageRx
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventLpPageRx(void* pcy_privatedata, struct CEDI_LpPageRx* pageRx)
{
    // debug_printf("Reports PCS auto-negotiation page received");
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventAn
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventAn(void* pcy_privatedata, struct CEDI_NetAnStatus* netStat)
{
    // debug_printf("Auto Negotiation Event");
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventLinkChange
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventLinkChange(void *pcy_privatedata, uint8_t a_linkstate)
{
    // debug_printf("[ETH] (Event) linkChange received.\r\n");
    // debug_printf("[ETH] link state=%d\r\n", a_linkstate);

    return;
}
/*******************************************************************************
* Function Name: Cy_ETHIF_EventTsu
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventTsu (void *pcy_privatedata, uint32_t u32event)
{
    /** Event generated if second count of the timer has incremented */
    if (u32event & CEDI_EV_TSU_SEC_INC)
    {
        /** application callback function     */
        if (stccallbackfunctions.tsuSecondInccb != NULL)
        {
            stccallbackfunctions.tsuSecondInccb();
        }
    }

    /** Timer count matched event    */
    if (u32event & CEDI_EV_TSU_TIME_MATCH)
    {

    }
}
/*******************************************************************************
* Function Name: Cy_ETHIF_EventPauseFrame
****************************************************************************//**
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventPauseFrame(void *pcy_privatedata, uint32_t u32event)
{
    if (u32event & CEDI_EV_PAUSE_FRAME_TX)
    {
        // debug_printf("Pause frame transmitted");
    }
    
    if (u32event & CEDI_EV_PAUSE_NZ_QU_RX)
    {
        // debug_printf("Pause frame received");
    }
}
/*******************************************************************************
* Function Name: Cy_ETHIF_EventPtp
********************************************************************************
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventPtp (void* pcy_privatedata, uint32_t u32type, struct CEDI_1588TimerVal* time)
{
    if (u32type & CEDI_EV_PTP_TX_SYNC)
    {
        // printf("Sync frame transmitted");
    }

    if (u32type & CEDI_EV_PTP_TX_PDLY_REQ)
    {
        // printf("PTP PDelay Req frame transmitted");
    }

    if (u32type & CEDI_EV_PTP_TX_PDLY_RSP)
    {
        // printf("PTP PDelay Resp frame transmitted");
    }

    if (u32type & CEDI_EV_PTP_RX_SYNC)
    {
        // printf("Sync frame received");
    }

    if (u32type & CEDI_EV_PTP_RX_PDLY_REQ)
    {
        // printf("PTP PDelay Req frame received");
    }

    if (u32type & CEDI_EV_PTP_RX_PDLY_RSP)
    {
        // printf("PTP PDelay Resp frame received");
    }
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventExternalInt
********************************************************************************
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventExternalInt(void * pcy_privatedata)
{
   // debug_printf("External Event Occurred");
}

/*******************************************************************************
* Function Name: Cy_ETHIF_EventWol
********************************************************************************
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventWol(void * pcy_privatedata)
{
   // debug_printf("Wake On LAN Event Occurred");
}
/*******************************************************************************
* Function Name: Cy_ETHIF_EventLpi
********************************************************************************
*
* \brief 
*
* \param 
* 
*
*******************************************************************************/
static void Cy_ETHIF_EventLpi(void * pcy_privatedata)
{
    // debug_printf("LPI Status changed Event");
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXETH */
/* [] END OF FILE */
