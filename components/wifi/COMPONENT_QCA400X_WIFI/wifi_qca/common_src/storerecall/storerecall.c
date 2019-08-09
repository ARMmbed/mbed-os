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

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <hcd_api.h>
#include <wmi_api.h>
#include <spi_hcd_if.h>
#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#include <common_stack_offload.h>
#include <wlan_api.h>

A_STATUS setup_host_dset(void *handle);

#if DRIVER_CONFIG_ENABLE_STORE_RECALL
extern const uint8_t max_performance_power_param;

#if !ENABLE_STACK_OFFLOAD
extern int32_t power_state_for_module;
/*****************************************************************************/
/*  restore_power_state - Restores the original power state, which is changed by
 *                        "move_power_state_to_maxperf"
 *  int32_t - module which request the change
 * RETURNS: A_OK on success A_ERROR otherwise
 *****************************************************************************/
A_STATUS restore_power_state(void *pCxt, int32_t module)
{
    A_DRIVER_CONTEXT *pDCxt = (A_DRIVER_CONTEXT *)pCxt;

    /* Already some HIGH priority module changes state,
     * so don't change the state now */
    if (!power_state_for_module || (power_state_for_module > module))
    {
        return A_OK;
    }
    if (pDCxt->userPwrMode != max_performance_power_param)
    {
        if (A_OK !=
            wmi_cmd_start(pDCxt->pWmiCxt, &pDCxt->userPwrMode, WMI_SET_POWER_MODE_CMDID, sizeof(WMI_POWER_MODE_CMD)))
        {
            return A_ERROR;
        }
    }

    power_state_for_module = 0;
    return A_OK;
}
#endif
/*****************************************************************************/
/*  Strrcl_ChipUpFinish - Utility function to complete the chip_up process.
 *	 when WMI_READY is received from chip this function clears the chipDown
 * 	 boolean and sends the wmi store recall command to the chip so that its
 *	 associated State can be refreshed.
 *      void *pCxt - the driver context.
 *****************************************************************************/
static A_STATUS Strrcl_ChipUpFinish(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    WMI_STORERECALL_RECALL_CMD *pDsetInfo;
    uint32_t dset_info_len;

    if (pDCxt->wmiReady == true)
    {
        switch (pDCxt->strrclState)
        {
            case STRRCL_ST_ACTIVE:
                /* clear the chipDown boolean to allow outside activity to resume */
                pDCxt->chipDown = false;

                if (A_OK != STACK_INIT(pCxt))
                {
                    break;
                }

                pDCxt->strrclState = STRRCL_ST_ACTIVE_B;
            // INTENTIONAL FALL_THRU
            case STRRCL_ST_ACTIVE_B:
/* send recall */
#if 0
		    pEv = (WMI_STORERECALL_STORE_EVENT*)pDCxt->strrclCxt;

            if(A_OK != wmi_storerecall_recall_cmd(pDCxt->pWmiCxt, A_LE2CPU32(pEv->length), &pEv->data[0])){
                break; //try again later this is likely because a previous wmi cmd has not finished.
            }
#elifndef ENABLE_LARGE_DSET

		    setup_host_dset(pCxt);

		    pDsetData = pDCxt->tempStorage;

            if(A_OK != wmi_storerecall_recall_cmd(pDCxt->pWmiCxt, A_LE2CPU32(pDCxt->strrclCxtLen), pDsetData)){
                break; //try again later this is likely because a previous wmi cmd has not finished.
            }
#else
                setup_host_dset(pCxt);

                pDsetInfo = (WMI_STORERECALL_RECALL_CMD *)pDCxt->tempStorage;
                if (pDsetInfo->length == 0)
                {
                    dset_info_len = sizeof(WMI_STORERECALL_RECALL_CMD);
                }
                else
                {
                    dset_info_len = sizeof(WMI_STORERECALL_RECALL_CMD) - sizeof(uint8_t);
                    dset_info_len += sizeof(WMI_STORERECALL_RECALL_DSET) * pDsetInfo->length;
                }

                if (A_OK != wmi_storerecall_recall_cmd(pDCxt->pWmiCxt, dset_info_len, pDsetInfo))
                {
                    break; // try again later this is likely because a previous wmi cmd has not finished.
                }
#endif

                pDCxt->strrclCxt = NULL;
                pDCxt->strrclCxtLen = 0;

                //            pDCxt->strrclState = STRRCL_ST_AWAIT_FW;

                pDCxt->strrclState = STRRCL_ST_INIT;
                pDCxt->strrclBlock = false;

                /* Indicate completion to the custom layer */
                API_STORE_RECALL_EVENT(pCxt);

                /* clear this function from the driver's temporary calls */
                pDCxt->asynchRequest = NULL;
                /* Since we are setting MAX_PERF before going to store-recall we need to set the state
                   after we wakes up */
                restore_power_state(pDCxt, 1); /* POWER_STATE_MOVED_FOR_STRRCL */

                break; // done!!
            default:
                A_ASSERT(0); // should never happen
#if DRIVER_CONFIG_DISABLE_ASSERT
                break;
#endif
        }
    }

    return A_OK;
}

/*****************************************************************************/
/*  Strrcl_ChipUpStart - Utility function to bring bring the chip back up. Also
 * 	 sets up the driver tempFunc so that when the WMI_READY is received the
 *	 store recall process can be completed via Strrcl_ChipUpFinish.
 *      void *pCxt - the driver context.
 *****************************************************************************/
static A_STATUS Strrcl_ChipUpStart(void *pCxt)
{
    A_STATUS status = A_ERROR;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    A_MDELAY(2);
    /* assert pwd line */
    HW_PowerUpDown(pCxt, true);

    A_MDELAY(2);
    do
    {
        /* reset htc_creditInit to re-process the HTC interrupt */
        pDCxt->htc_creditInit = 0;

        if ((status = Hcd_ReinitTarget(pCxt)) != A_OK)
        {
            break;
        }

        A_MDELAY(1);

        if (A_OK != (status = Driver_BootComm(pCxt)))
        {
            break;
        }

#if (DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD)
        if (A_OK != (status = Driver_StoreRecallFirmwareDownload(pCxt)))
        {
            break;
        }
#endif /* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD */

        /* unmask the packet interrupt on-chip */
        Hcd_UnmaskInterrupts(pCxt, ATH_SPI_INTR_PKT_AVAIL);
        /* when the driver receives the WMI_READY event chip_up_finish
         * will complete the re-boot process and open the way for
         * outside requests to resume. */
        pDCxt->asynchRequest = Strrcl_ChipUpFinish;

    } while (0);

    return status;
}

/*****************************************************************************/
/*  Strrcl_ChipDown - Utility function to bring the chip down. Also sets
 *	 driver booleans to prevent any unwanted activity until the chip is
 *	 brought back up.
 *      void *pCxt - the driver context.
 *****************************************************************************/
static void Strrcl_ChipDown(void *pCxt)
{
    /* disable interrupt line - prevent un-expected INTs when chip powers down */
    //	EnableDisableSPIIRQHwDetect(pDevice,false);
    GET_DRIVER_COMMON(pCxt)->chipDown = true;
    GET_DRIVER_COMMON(pCxt)->wmiReady = false;
    /* assert pwd line */
    HW_PowerUpDown(pCxt, false);
}

/*****************************************************************************/
/*  Strrcl_Recall - Implements the store-recall procedure.  powers down the chip
 *	 waits for x msec powers up the chip, reboots the chip. Called when the
 * 	 store-recall event is received from the chip indicating that it can be
 *	 powered down for the specified timeout.
 *      void *pCxt - the driver context.
 *		uint32_t msec_sleep - number of milliseconds to wait
 *****************************************************************************/
void Strrcl_Recall(void *pCxt, uint32_t msec_sleep)
{
    /* bring down the chip */
    Strrcl_ChipDown(pCxt);
    /* this will block the driver thread */
    /* FIXME: should instead implement a way to block the driver at driver_main thus
     * allowing the thread to continue running in single-threaded systems.*/
    if (ath_custom_init.Custom_Delay != NULL)
    {
        /*It is possible that some hosts support low power modes, this would be a
          good place to invoke it. Call app defined function if available*/
        ath_custom_init.Custom_Delay(msec_sleep);
    }
    else
        A_MDELAY(msec_sleep);

    /* wake up chip */
    Strrcl_ChipUpStart(pCxt);
}

#endif // DRIVER_CONFIG_ENABLE_STORE_RECALL
