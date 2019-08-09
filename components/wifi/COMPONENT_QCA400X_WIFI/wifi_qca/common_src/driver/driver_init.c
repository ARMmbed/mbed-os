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
#include <wifi_common.h>
#include <a_types.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <hcd_api.h>
#include <wmi_api.h>
#include <AR6K_version.h>
#include <hif_internal.h>
#include <targaddrs.h>
#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#include <htc_services.h>
#include <bmi.h>
#include <htc.h>
#include <spi_hcd_if.h>
#include <aggr_recv_api.h>
#include <common_api.h>
#include "hw20_apb_map.h"
#include "cmsis_os.h"

//#include "hw4.0/hw/rtc_reg.h"
#if (DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD)
#include <ath_fw_binary.h>
#include "hw20_mbox_reg.h"
#endif /* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD */

#define HI_OPTION_NUM_DEV_SHIFT 0x9
#define HI_OPTION_FW_MODE_BSS_STA 0x1
#define HI_OPTION_FW_MODE_AP 0x2
#define HI_OPTION_FW_MODE_SHIFT 0xC

/* 2 bits of hi_option flag are usedto represent 4 submodes */
#define HI_OPTION_FW_SUBMODE_NONE 0x0      /* Normal mode */
#define HI_OPTION_FW_SUBMODE_P2PDEV 0x1    /* p2p device mode */
#define HI_OPTION_FW_SUBMODE_P2PCLIENT 0x2 /* p2p client mode */
#define HI_OPTION_FW_SUBMODE_P2PGO 0x3     /* p2p go mode */
#define HI_OPTION_FW_SUBMODE_BITS 0x2
#define HI_OPTION_FW_SUBMODE_SHIFT 0x14

/* hi_option_flag3 options */
#define HI_OPTION_USE_OFFLOAD_P2P 0x01

#if (DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD)
uint32_t ar4XXX_boot_param = AR4XXX_PARAM_MODE_NORMAL | AR4XXX_PARAM_MODE_BMI; // AR4XXX_PARAM_RAW_QUAD;
#else
uint32_t ar4XXX_boot_param = AR4XXX_PARAM_MODE_NORMAL; // AR4XXX_PARAM_RAW_QUAD;
#endif /* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD */
uint32_t ar4xx_reg_domain = AR4XXX_PARAM_REG_DOMAIN_DEFAULT;
uint32_t last_driver_error; // Used to store the last error encountered by the driver

/*****************************************************************************/
/*  ConnectService - Utility to set up a single endpoint service.
 * 		void *pCxt - the driver context.
 *	 	HTC_SERVICE_CONNECT_REQ  *pConnect - pointer to connect request
 *		 structure.
 *****************************************************************************/
static A_STATUS ConnectService(void *pCxt, HTC_SERVICE_CONNECT_REQ *pConnect)
{
    A_STATUS status;
    HTC_SERVICE_CONNECT_RESP response;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        A_MEMZERO(&response, sizeof(response));

        if (A_OK != (status = HTC_ConnectService(pCxt, pConnect, &response)))
        {
            break;
        }

        switch (pConnect->ServiceID)
        {
            case WMI_CONTROL_SVC:
                if (pDCxt->wmiEnabled == true)
                {
                    /* set control endpoint for WMI use */
                    wmi_set_control_ep(pDCxt->pWmiCxt, response.Endpoint);
                }
                break;
            case WMI_DATA_BE_SVC:
                pDCxt->ac2EpMapping[WMM_AC_BE] = response.Endpoint;
                pDCxt->ep2AcMapping[response.Endpoint] = WMM_AC_BE;
                break;
            case WMI_DATA_BK_SVC:
                pDCxt->ac2EpMapping[WMM_AC_BK] = response.Endpoint;
                pDCxt->ep2AcMapping[response.Endpoint] = WMM_AC_BK;
                break;
            case WMI_DATA_VI_SVC:
                pDCxt->ac2EpMapping[WMM_AC_VI] = response.Endpoint;
                pDCxt->ep2AcMapping[response.Endpoint] = WMM_AC_VI;
                break;
            case WMI_DATA_VO_SVC:
                pDCxt->ac2EpMapping[WMM_AC_VO] = response.Endpoint;
                pDCxt->ep2AcMapping[response.Endpoint] = WMM_AC_VO;
                break;
            default:
                status = A_EINVAL;
                break;
        }

    } while (0);

    return status;
}

/*****************************************************************************/
/*  SetupServices - Utility to set up endpoint services.
 * 		void *pCxt - the driver context.
 *****************************************************************************/
static A_STATUS SetupServices(void *pCxt)
{
    A_STATUS status;
    HTC_SERVICE_CONNECT_REQ connect;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
   (void)(pDCxt);

    do
    {
        A_MEMZERO(&connect, sizeof(connect));
        /* meta data is unused for now */
        connect.pMetaData = NULL;
        connect.MetaDataLength = 0;
        /* connect to control service */
        connect.ServiceID = WMI_CONTROL_SVC;

        if (A_OK != (status = ConnectService(pCxt, &connect)))
        {
            break;
        }

        connect.LocalConnectionFlags |= HTC_LOCAL_CONN_FLAGS_ENABLE_SEND_BUNDLE_PADDING;
        /* limit the HTC message size on the send path, although we can receive A-MSDU frames of
         * 4K, we will only send ethernet-sized (802.3) frames on the send path. */
        connect.MaxSendMsgSize = WMI_MAX_TX_DATA_FRAME_LENGTH;

        /* for the remaining data services set the connection flag to reduce dribbling,
         * if configured to do so */
        if (WLAN_CONFIG_REDUCE_CREDIT_DRIBBLE)
        {
            connect.ConnectionFlags |= HTC_CONNECT_FLAGS_REDUCE_CREDIT_DRIBBLE;
            /* the credit dribble trigger threshold is (reduce_credit_dribble - 1) for a value
             * of 0-3 */
            connect.ConnectionFlags &= ~HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
            connect.ConnectionFlags |= HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_ONE_HALF;
        }
        /* connect to best-effort service */
        connect.ServiceID = WMI_DATA_BE_SVC;

        if (A_OK != (status = ConnectService(pCxt, &connect)))
        {
            break;
        }

        /* connect to back-ground
         * map this to WMI LOW_PRI */
        connect.ServiceID = WMI_DATA_BK_SVC;

        if (A_OK != (status = ConnectService(pCxt, &connect)))
        {
            break;
        }

        /* connect to Video service, map this to
         * to HI PRI */
        connect.ServiceID = WMI_DATA_VI_SVC;

        if (A_OK != (status = ConnectService(pCxt, &connect)))
        {
            break;
        }

        /* connect to VO service, this is currently not
         * mapped to a WMI priority stream due to historical reasons.
         * WMI originally defined 3 priorities over 3 mailboxes
         * We can change this when WMI is reworked so that priorities are not
         * dependent on mailboxes */
        connect.ServiceID = WMI_DATA_VO_SVC;

        if (A_OK != (status = ConnectService(pCxt, &connect)))
        {
            break;
        }

        A_ASSERT(pDCxt->ac2EpMapping[WMM_AC_BE] != 0);
        A_ASSERT(pDCxt->ac2EpMapping[WMM_AC_BK] != 0);
        A_ASSERT(pDCxt->ac2EpMapping[WMM_AC_VI] != 0);
        A_ASSERT(pDCxt->ac2EpMapping[WMM_AC_VO] != 0);
    } while (0);

    return status;
}

#if (DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD)
/*****************************************************************************/
/*  Driver_DownloadFirmwareBinary - Utility to download Board data
                                    and athwlan firmware binary to target.
 *  void *pCxt - the driver context.
 *****************************************************************************/
uint32_t Driver_DownloadFirmwareBinary(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint32_t address = 0, param = 0;

    /* Temporarily disable system sleep */
    address = MBOX_BASE_ADDRESS + LOCAL_SCRATCH_ADDRESS;
    (BMIReadSOCRegister(pCxt, address, &param));
    param |= A_CPU2LE32(AR6K_OPTION_SLEEP_DISABLE | AR6K_OPTION_WDT_DISABLE);
    (BMIWriteSOCRegister(pCxt, address, param));

    address = RTC_BASE_ADDRESS + SYSTEM_SLEEP_ADDRESS;
    (BMIReadSOCRegister(pCxt, address, &param));
    param |= A_CPU2LE32(WLAN_SYSTEM_SLEEP_DISABLE_SET(1));
    (BMIWriteSOCRegister(pCxt, address, param));

    /* Set Option3 flag in host interest item to boot up
       hostproxy application in firmware download mode */
    if (Driver_ReadDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag3)), (uint8_t *)&param, 4) != A_OK)
    {
        return A_ERROR;
    }

    param |= HI_OPTION_EN_HOST_PROXY_FW_DLOAD_MODE;

    if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag3)), (uint8_t *)&param, 4) !=
        A_OK)
    {
        return A_ERROR;
    }

#if ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
    /* Set the board data address in HOST interest item for
   target to use it */
    param = A_CPU2LE32(AR400X_BOARD_DATA_ADDRESS);

    if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_board_data)), (uint8_t *)&param, 4) != A_OK)
    {
        return A_ERROR;
    }

    /* non-zero value of hi_board_data_initialized indicates that the
    board data is valid for the target */
    param = A_CPU2LE32(1);
    if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_board_data_initialized)), (uint8_t *)&param,
                             4) != A_OK)
    {
        return A_ERROR;
    }
#endif /* ATH_FIRMWARE_TARGET */

    /* Download firmware binary only if exitAtBmi is false as possibly flash
       agent could be attempting to flash the binary onto target flash memory */
    if (true != ath_custom_init.exitAtBmi)
    {
        /* Clear BMI mode in hi_flash_is_present as we are already in BMI mode */
        param = A_CPU2LE32(AR4XXX_PARAM_MODE_NORMAL | ar4xx_reg_domain);

        if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_flash_is_present)), (uint8_t *)&param,
                                 4) != A_OK)
        {
            return A_ERROR;
        }

        /* This is just a reference implementation to download the firmware
       binary. If memory needs to be optimized, the binary file can be stored
       in the host flash file system and can be opened, read in chunks using file
        system APIs and can be downloaded using below BMI API */
        BMIWriteMemory(pCxt, BMI_SEGMENTED_WRITE_ADDR, (uint8_t *)&ath_fw_binary_buff, sizeof(ath_fw_binary_buff));
    }

    return 0;
}

#endif /* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD */

/*****************************************************************************/
/*  Driver_ContextInit - Initializes the drivers COMMON context. The allocation
 *	 for the context should be made by the caller. This function will call
 *	 the Custom_DRiverContextInit to perform any system specific initialization
 *	 Look to Driver_ContextDeInit() to undo what gets done by this function.
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Driver_ContextInit(void *pCxt)
{
    uint8_t i;
    osMutexAttr_t attr = { 0 };
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    /* most elements are initialized by setting to zero the rest are explicitly
     * initialized.  boolean are explicit as it is not obvious what false might be */
    A_MEMZERO(pDCxt, sizeof(A_DRIVER_CONTEXT));
    pDCxt->driver_up = false;
    pDCxt->rxBufferStatus = false;
    pDCxt->hostVersion = AR6K_SW_VERSION;
    pDCxt->wmiReady = false;
    pDCxt->wmiEnabled = false;
    pDCxt->wmmEnabled = false;
    pDCxt->chipDown = true;
    pDCxt->strrclState = STRRCL_ST_DISABLED;
    pDCxt->strrclBlock = false;
    pDCxt->wpsState = false;

    attr.name = "qca400x_mutex";
    attr.cb_mem = &pDCxt->context_mutex_mem;
    attr.cb_size = sizeof(pDCxt->context_mutex_mem);
    attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    pDCxt->apiMutex = osMutexNew(&attr);
    if (NULL == pDCxt->apiMutex)
        return A_ERROR;

    /* Connection element for first device */
    pDCxt->conn[0].networkType = INFRA_NETWORK;
    pDCxt->conn[0].dot11AuthMode = OPEN_AUTH;
    pDCxt->conn[0].wpaAuthMode = NONE_AUTH;
    pDCxt->conn[0].wpaPairwiseCrypto = NONE_CRYPT;
    pDCxt->conn[0].wpaGroupCrypto = NONE_CRYPT;
    pDCxt->conn[0].wpaPmkValid = false;
    pDCxt->conn[0].isConnected = false;
    pDCxt->conn[0].isConnectPending = false;

#if (WLAN_NUM_OF_DEVICES > 1)
    pDCxt->conn[1].networkType = INFRA_NETWORK;
    pDCxt->conn[1].dot11AuthMode = OPEN_AUTH;
    pDCxt->conn[1].wpaAuthMode = NONE_AUTH;
    pDCxt->conn[1].wpaPairwiseCrypto = NONE_CRYPT;
    pDCxt->conn[1].wpaGroupCrypto = NONE_CRYPT;
    pDCxt->conn[1].wpaPmkValid = false;
    pDCxt->conn[1].isConnected = false;
    pDCxt->conn[1].isConnectPending = false;
#endif

    pDCxt->scanDone = false;
    /* through measurement, it's in power save mode by default.*/
    pDCxt->userPwrMode = REC_POWER;
    pDCxt->enabledSpiInts = ATH_SPI_INTR_PKT_AVAIL | ATH_SPI_INTR_LOCAL_CPU_INTR;
    pDCxt->mboxAddress = HIF_MBOX_START_ADDR(HIF_ACTIVE_MBOX_INDEX); /* the address for mailbox reads/writes. */
    pDCxt->blockSize = HIF_MBOX_BLOCK_SIZE;                          /* the mailbox block size */
    pDCxt->blockMask = pDCxt->blockSize - 1;                         /* the mask derived from BlockSize */
    pDCxt->padBuffer = A_MALLOC(pDCxt->blockSize, MALLOC_ID_CONTEXT);
    pDCxt->htcStart = false;
    pDCxt->macProgramming = false;
#if MANUFACTURING_SUPPORT
    pDCxt->testResp = false;
#endif
    for (i = 0; i < ENDPOINT_MANAGED_MAX; i++)
    {
        pDCxt->ep[i].epIdx = i;
    }

    if (NULL == pDCxt->padBuffer)
        return A_ERROR;

    return CUSTOM_DRIVER_CXT_INIT(pCxt);
}

/*****************************************************************************/
/*  Driver_ContextDeInit - Undoes any work performed by Driver_ContextInit.
 *	 Used as part of the shutdown process. Calls Driver_ContextDeInit()
 *	 to cleanup any work done by Driver_ContextInit().
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Driver_ContextDeInit(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    CUSTOM_DRIVER_CXT_DEINIT(pCxt);

    if (pDCxt->padBuffer != NULL)
    {
        A_FREE(pDCxt->padBuffer, MALLOC_ID_CONTEXT);
        pDCxt->padBuffer = NULL;
    }

    return A_OK;
}

/*****************************************************************************/
/*  Driver_Init - inits any HW resources for HW access.
 *      Turns CHIP on via GPIO if necessary.
 *      Performs initial chip interaction.
 *      If the system has a dedicated driver thread then
 *      this function should be called by the dedicated thread.
 *      If not then this function should be called between
 *      calls to api_InitStart and api_InitFinish
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Driver_Init(void *pCxt)
{
    A_STATUS status;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    A_EVENT_INIT(&GET_DRIVER_CXT(p_Global_Cxt)->driverWakeEvent, kEventAutoClear);
    A_EVENT_INIT(&GET_DRIVER_CXT(p_Global_Cxt)->userWakeEvent, kEventManualClear);
#if T_SELECT_VER1
    A_EVENT_INIT(&GET_DRIVER_CXT(p_Global_Cxt)->sockSelectWakeEvent, kEventManualClear);
#endif // T_SELECT_VER1

    do
    {
        /* initialize various protection elements */
        if (A_OK != (status = RXBUFFER_ACCESS_INIT(pCxt)))
        {
            break;
        }

        if (A_OK != (status = TXQUEUE_ACCESS_INIT(pCxt)))
        {
            break;
        }

        if (A_OK != (status = IRQEN_ACCESS_INIT(pCxt)))
        {
            break;
        }

        if (A_OK != (status = DRIVER_SHARED_RESOURCE_ACCESS_INIT(pCxt)))
        {
            break;
        }
        /* - init hw resources */
        if (A_OK != (status = CUSTOM_HW_INIT(pCxt)))
        {
            break;
        }
        /* - Power up device */
        HW_PowerUpDown(pCxt, 1);

        Api_BootProfile(pCxt, BOOT_PROFILE_POWER_UP);

        /* - bring chip CPU out of reset */
        if (A_OK != (status = Hcd_Init(pCxt)))
        {
            break;
        }

        /* It is necessary to postpone further SPI commands,
         * until WiFI gets ready. Empirical value is 100ms */
        A_MDELAY(100);

        /* - initiate communication with chip firmware */
        if (A_OK != (status = Driver_BootComm(pCxt)))
        {
            break;
        }

        if ((ar4XXX_boot_param & AR4XXX_PARAM_MODE_MASK) == AR4XXX_PARAM_MODE_BMI)
        {
            BMIInit(pCxt);
        }

        /* - acquire target type */
        if (A_OK != (status = Driver_GetTargetInfo(pCxt)))
        {
            break;
        }
        /* - perform any BMI chip configuration */
        if (ath_custom_init.Driver_BMIConfig != NULL)
        {
            if (A_OK != (status = ath_custom_init.Driver_BMIConfig(pCxt)))
            {
                break;
            }
        }

#if (DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD)
        Driver_DownloadFirmwareBinary(pCxt);
#endif /* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD  */

        /* - perform any target configuration */
        if (ath_custom_init.Driver_TargetConfig != NULL)
        {
            if (A_OK != (status = ath_custom_init.Driver_TargetConfig(pCxt)))
            {
                break;
            }
        }
        /* - choose wmi or not wmi */
        if (ath_custom_init.skipWmi == false)
        {
            pDCxt->pWmiCxt = wmi_init(pCxt);
            pDCxt->wmiEnabled = true;
#if WLAN_CONFIG_11N_AGGR_SUPPORT
            pDCxt->pAggrCxt = aggr_init();
#endif
        }

        /* - alternate boot mode - exit driver init with BMI active */
        if (ath_custom_init.exitAtBmi)
        {
            break;
        }

        if ((ar4XXX_boot_param & AR4XXX_PARAM_MODE_MASK) == AR4XXX_PARAM_MODE_BMI)
        {
            /* - done with BMI; call BMIDone */
            if (A_OK != (status = BMIDone(pCxt)))
            {
                break;
            }
        }

        /* - wait for HTC setup complete */
        if (A_OK != (status = HTC_WaitTarget(pCxt)))
        {
            break;
        }
        /* - setup other non-htc endpoints */
        if (A_OK != (status = SetupServices(pCxt)))
        {
            break;
        }

        /* - start HTC */
        if (A_OK != (status = HTC_Start(pCxt)))
        {
            break;
        }

    } while (0);

    if (status == A_OK)
    {
        pDCxt->chipDown = false;
        pDCxt->driver_up = true;
        pDCxt->driverShutdown = false;
    }

    return status;
}

/*****************************************************************************/
/*  Driver_DeInit - Undoes any work performed by Driver_Init.
 *	 Used as part of the shutdown process.
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Driver_DeInit(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    Hcd_Deinitialize(pCxt); /* not necessary if HW_PowerUpDown() actually
           shuts down chip */
    HW_PowerUpDown(pCxt, 0);
    CUSTOM_HW_DEINIT(pCxt);

    if (pDCxt->pWmiCxt)
    {
        wmi_shutdown(pDCxt->pWmiCxt);
        pDCxt->pWmiCxt = NULL;
    }
#if WLAN_CONFIG_11N_AGGR_SUPPORT
    if (pDCxt->pAggrCxt)
    {
        aggr_deinit(pDCxt->pAggrCxt);
        pDCxt->pAggrCxt = NULL;
    }
#endif

    RXBUFFER_ACCESS_DESTROY(pCxt);
    TXQUEUE_ACCESS_DESTROY(pCxt);
    IRQEN_ACCESS_DESTROY(pCxt);
    DRIVER_SHARED_RESOURCE_ACCESS_DESTROY(pCxt);

    pDCxt->driver_up = false;
    A_EVENT_DELETE(&GET_DRIVER_CXT(p_Global_Cxt)->userWakeEvent);
    A_EVENT_DELETE(&GET_DRIVER_CXT(p_Global_Cxt)->driverWakeEvent);
#if T_SELECT_VER1
    A_EVENT_DELETE(&GET_DRIVER_CXT(p_Global_Cxt)->sockSelectWakeEvent);
#endif // T_SELECT_VER1

    return A_OK;
}

/*****************************************************************************/
/*  Driver_GetTargetInfo - Used to get the device target info which includes
 *	 the target version and target type. the results are stored in the
 *	 context.
 * 		void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Driver_GetTargetInfo(void *pCxt)
{
    A_STATUS status = A_OK;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    struct bmi_target_info targ_info = {0};

    if ((ar4XXX_boot_param & AR4XXX_PARAM_MODE_MASK) == AR4XXX_PARAM_MODE_BMI)
    {
        status = BMIGetTargetInfo(pCxt, &targ_info);
        pDCxt->targetVersion = targ_info.target_ver;
        pDCxt->targetType = targ_info.target_type;
    }
    else
    {
        /* hardcoded for boottime speedup */
        pDCxt->targetVersion = ATH_FIRMWARE_TARGET;
        pDCxt->targetType = TARGET_TYPE;
    }

    return status;
}

/*****************************************************************************/
/*  Driver_TargReset - This function performs a warm reset operation on the
 *   wifi CPU after setting up firmware so that it can discover the
 *   nvram config structure.
 * 		void *pCxt - the driver context.
 *****************************************************************************/
static uint16_t Driver_TargReset(void *pCxt)
{
#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2
    volatile uint32_t param;

    if ((ar4XXX_boot_param & AR4XXX_PARAM_MODE_MASK) == AR4XXX_PARAM_MODE_NORMAL)
    {
        do
        {
            if (Driver_ReadDataDiag(pCxt, TARG_VTOP(AR4100_NVRAM_SAMPLE_ADDR), (uint8_t *)&param, 4) != A_OK)
            {
                return A_ERROR;
            }
        } while (param != A_CPU2LE32(AR4100_NVRAM_SAMPLE_VAL));

        if (Driver_ReadDataDiag(pCxt, TARG_VTOP(AR4100_CONFIGFOUND_ADDR), (uint8_t *)&param, 4) != A_OK)
        {
            return A_ERROR;
        }

        if (param == A_CPU2LE32(AR4100_CONFIGFOUND_VAL))
        {
            /* force config_found value */
            param = 0xffffffff;
            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(AR4100_CONFIGFOUND_ADDR), (uint8_t *)&param, 4) != A_OK)
            {
                return A_ERROR;
            }
            /* instruct ROM to preserve nvram values */
            param = A_CPU2LE32(HI_RESET_FLAG_PRESERVE_NVRAM_STATE);
            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_reset_flag)), (uint8_t *)&param,
                                     4) != A_OK)
            {
                return A_ERROR;
            }
            /* instruct ROM to parse hi_reset_flag */
            param = A_CPU2LE32(HI_RESET_FLAG_IS_VALID);
            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_reset_flag_valid)),
                                     (uint8_t *)&param, 4) != A_OK)
            {
                return A_ERROR;
            }
            /* clear hi_refclk_hz to identify reset point */
            param = A_CPU2LE32(0x00);
            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_refclk_hz)), (uint8_t *)&param, 4) !=
                A_OK)
            {
                return A_ERROR;
            }
            /* reset CPU */
            param = A_CPU2LE32(WLAN_RESET_CONTROL_WARM_RST_MASK);
            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(RTC_BASE_ADDRESS | RESET_CONTROL_ADDRESS), (uint8_t *)&param, 4) !=
                A_OK)
            {
                return A_ERROR;
            }
            /* force delay before further chip access after reset */
            CUSTOM_HW_USEC_DELAY(pCxt, 1000);

            return 1;
        }
    }
#else
    UNUSED_ARGUMENT(pCxt);
#endif
    return 0;
}

/*****************************************************************************/
/*  Driver_BootComm - Communicates with the device using the diagnostic
 * 	 window to perform various interactions during the boot process.
 * 		void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Driver_BootComm(void *pCxt)
{
    volatile uint32_t param;
    uint8_t resetpass = 0;

    if (ath_custom_init.Driver_BootComm != NULL)
    {
        return ath_custom_init.Driver_BootComm(pCxt);
    }

#if (ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2 || ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1)
    /* interact with host_proxy via HOST_INTEREST to control BMI active */

    do
    {
#if ENABLE_FPGA_BUILD
        HW_USEC_DELAY(pCxt, 4000);
#endif
        // On the QCA4004, the code sometimes hits one of the A_ASSERTs if this delay
        // is not present. To be studied in detail later (THK, 2016-Jan)
        A_MDELAY(4);
        do
        {
            if (Driver_ReadDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_refclk_hz)), (uint8_t *)&param, 4) !=
                A_OK)
            {
                return A_ERROR;
            }
            // printf("%d\r\n", param);
        } while (param != A_CPU2LE32(EXPECTED_REF_CLK_AR4100) && param != A_CPU2LE32(EXPECTED_REF_CLK_AR400X));

        Api_BootProfile(pCxt, BOOT_PROFILE_READ_REFCLK);

#if 0 // Enabling firmware UART print and setting baud rate to 115200
        param = A_CPU2LE32(1);
        if (Driver_WriteDataDiag(pCxt,
                                TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_serial_enable)),
                                 (uint8_t *)&param,
                                 4) != A_OK)
        {
             return A_ERROR;
        }

        param = A_CPU2LE32(115200);
        if (Driver_WriteDataDiag(pCxt,
                                TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_desired_baud_rate)),
                                 (uint8_t *)&param,
                                 4) != A_OK)
        {
             return A_ERROR;
        }
#endif

        param = A_CPU2LE32(0x31);
        if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_pwr_save_flags)), (uint8_t *)&param,
                                 4) != A_OK)
        {
            return A_ERROR;
        }

/* wait host_proxy ready */
#if ENABLE_FPGA_BUILD
        do
        {
            if (Driver_ReadDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_flash_is_present)), (uint8_t *)&param,
                                    4) != A_OK)
            {
                return A_ERROR;
            }
        } while ((param & HOST_PROXY_BOOTCTL_MASK) != A_CPU2LE32(HOST_PROXY_INIT));
#endif
        /* this code is called prior to BMIGetTargInfo so we must assume AR6003 */
        param = A_CPU2LE32(ar4XXX_boot_param | ar4xx_reg_domain);

        Api_BootProfile(pCxt, BOOT_PROFILE_BOOT_PARAMETER);

        if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_flash_is_present)), (uint8_t *)&param,
                                 4) != A_OK)
        {
            return A_ERROR;
        }

        //        Api_BootProfile(pCxt, 0x02);
        if (WLAN_NUM_OF_DEVICES == 1)
        {
            param = ((1 << HI_OPTION_NUM_DEV_SHIFT) | (HI_OPTION_FW_MODE_BSS_STA << HI_OPTION_FW_MODE_SHIFT) |
                     (1 << HI_OPTION_FW_BRIDGE_SHIFT)) |
                    HI_OPTION_DISABLE_DBGLOG;

            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag)), (uint8_t *)&param,
                                     4) != A_OK)
            {
                return A_ERROR;
            }
        }
        else
        {
            /* Set P2P Offload support */
            param = HI_OPTION_USE_OFFLOAD_P2P;

            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag3)), (uint8_t *)&param,
                                     4) != A_OK)
            {
                return A_ERROR;
            }
            /* Set number of Device and device mode
               device 0 - P2P Device ; device 1 - IEEE STA */
            param = ((2 << HI_OPTION_NUM_DEV_SHIFT) |
                     (((HI_OPTION_FW_MODE_BSS_STA << 2 | HI_OPTION_FW_MODE_BSS_STA)) << HI_OPTION_FW_MODE_SHIFT) |
                     (((HI_OPTION_FW_SUBMODE_NONE << HI_OPTION_FW_SUBMODE_BITS) | HI_OPTION_FW_SUBMODE_P2PDEV)
                      << HI_OPTION_FW_SUBMODE_SHIFT) |
                     (1 << HI_OPTION_FW_BRIDGE_SHIFT));

            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag)), (uint8_t *)&param,
                                     4) != A_OK)
            {
                return A_ERROR;
            }

#if !ENABLE_SCC_MODE
            /* Enable MCC */
            if (Driver_ReadDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag2)), (uint8_t *)&param,
                                    4) != A_OK)
            {
                return A_ERROR;
            }

            param |= HI_OPTION_MCC_ENABLE;
            if (Driver_WriteDataDiag(pCxt, TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_option_flag2)), (uint8_t *)&param,
                                     4) != A_OK)
            {
                return A_ERROR;
            }
#endif /* ENABLE_SCC_MODE */
        }

    } while (resetpass++ < 1 && Driver_TargReset(pCxt));

#else
    UNUSED_ARGUMENT(param);
    UNUSED_ARGUMENT(loop);
#endif
    return A_OK;
}

#if (DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD)
A_STATUS Driver_StoreRecallFirmwareDownload(void *pCxt)
{
    A_STATUS status = A_OK;

    do
    {
        if ((ar4XXX_boot_param & AR4XXX_PARAM_MODE_MASK) == AR4XXX_PARAM_MODE_BMI)
        {
            BMIInit(pCxt);
        }

        /* - acquire target type */
        if (A_OK != (status = Driver_GetTargetInfo(pCxt)))
        {
            break;
        }
        /* - perform any BMI chip configuration */
        if (ath_custom_init.Driver_BMIConfig != NULL)
        {
            if (A_OK != (status = ath_custom_init.Driver_BMIConfig(pCxt)))
            {
                break;
            }
        }

        /* Download the firmware binary */
        Driver_DownloadFirmwareBinary(pCxt);

        /* - perform any target configuration */
        if (ath_custom_init.Driver_TargetConfig != NULL)
        {
            if (A_OK != (status = ath_custom_init.Driver_TargetConfig(pCxt)))
            {
                break;
            }
        }

        if ((ar4XXX_boot_param & AR4XXX_PARAM_MODE_MASK) == AR4XXX_PARAM_MODE_BMI)
        {
            /* - done with BMI; call BMIDone */
            if (A_OK != (status = BMIDone(pCxt)))
            {
                break;
            }
        }
    } while (0);

    return status;
}
#endif /* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD  */
