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
#include "wifi_common.h"

#include <a_config.h>
#include <a_osapi.h>
#include <a_types.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wlan_api.h>

// This is A_CUSTOM_DRIVER_CONTEXT, the top-level driver context.
// It contains pCommonCxt which points to the internal (aka "common")
// driver context (of type A_DRIVER_CONTEXT)
void *p_Global_Cxt = NULL;

/*The following pointers store the custom driver context used by QAPIs*/
// The pointers are of type QCA_CONTEXT_STRUCT_PTR
// and first entry is initialized by Custom_Api_Initialize(QCA_CONTEXT_STRUCT_PTR qca_ptr)
// (near the bottom) while the second is initialized in same function near the top
// Custom_Api_Initialize() also initializes p_Global_Cxt->pUpperCxt[] which
// is the exact same array as this one!
void *p_Driver_Cxt[2];

static uint32_t Custom_Api_Readmii(struct qca_context_struct *p_ctxt, uint32_t val, uint32_t *p_val, uint32_t val2);
static uint32_t Custom_Api_Writemii(struct qca_context_struct *p_ctxt, uint32_t val1, uint32_t val2, uint32_t val3);

static uint32_t Custom_Api_Join(struct qca_context_struct *p_ctxt, struct qca_mcb_struct *p_mcb);
static uint32_t Custom_Api_Rejoin(struct qca_context_struct *p_ctxt);
static uint32_t Custom_Api_Initialize(QCA_CONTEXT_STRUCT_PTR qca_ptr);
static uint32_t Custom_Api_Shutdown(struct qca_context_struct *qca_ptr);
extern uint32_t Custom_Api_Send(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, PCB_PTR pcb_ptr, uint32_t size, uint32_t frags, uint32_t flags);
extern uint32_t Custom_Api_Mediactl(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t command_id, void *inout_param);

uint32_t chip_state_ctrl(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t state);

const QCA_MAC_IF_STRUCT ATHEROS_WIFI_IF = {Custom_Api_Initialize, Custom_Api_Shutdown, Custom_Api_Send,
                                           Custom_Api_Readmii,    Custom_Api_Writemii, Custom_Api_Join,
                                           Custom_Api_Rejoin,     Custom_Api_Mediactl};

static uint32_t Custom_Api_Readmii(struct qca_context_struct *p_ctxt, uint32_t val, uint32_t *p_val, uint32_t val2)
{
    /* NOTHING TO DO HERE */
    UNUSED_ARGUMENT(p_ctxt);
    UNUSED_ARGUMENT(val);
    UNUSED_ARGUMENT(p_val);
    UNUSED_ARGUMENT(val2);

    return 0;
}

static uint32_t Custom_Api_Writemii(struct qca_context_struct *p_ctxt, uint32_t val1, uint32_t val2, uint32_t val3)
{
    /* NOTHING TO DO HERE */
    UNUSED_ARGUMENT(p_ctxt);
    UNUSED_ARGUMENT(val1);
    UNUSED_ARGUMENT(val2);
    UNUSED_ARGUMENT(val3);

    return 0;
}

static uint32_t Custom_Api_Join(struct qca_context_struct *p_ctxt, struct qca_mcb_struct *p_mcb)
{
    /* NOTHING TO DO HERE */
    UNUSED_ARGUMENT(p_ctxt);
    UNUSED_ARGUMENT(p_mcb);

    return 0;
}

static uint32_t Custom_Api_Rejoin(struct qca_context_struct *p_ctxt)
{
    /* NOTHING TO DO HERE */
    UNUSED_ARGUMENT(p_ctxt);

    return 0;
}

uint32_t chip_state_ctrl(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t state)
{
    uint32_t error = QCA_OK;

    if (state == 0)
    {
        /* confirm that the driver is not already down */
        if (qca_ptr->MAC_CONTEXT_PTR != NULL)
        {
            /* shutdown chip and driver */
            error = Custom_Api_Shutdown(qca_ptr);
        }
    }
    else
    {
        /* confirm that the driver is not already up */
        if (qca_ptr->MAC_CONTEXT_PTR == NULL)
        {
            /* bring up chip and driver */
            error = Custom_Api_Initialize(qca_ptr);
        }
    }

    return error;
}

/*****************************************************************************/
/*  Custom_Api_Initialize - Entry point for MQX to initialize the Driver.
 *      QCA_CONTEXT_STRUCT_PTR qca_ptr - pointer to MQX ethernet object.
 * RETURNS: QCA_OK on success or QCA_ERROR otherwise.
 *****************************************************************************/
static uint32_t Custom_Api_Initialize(QCA_CONTEXT_STRUCT_PTR qca_ptr)
{
    uint32_t error = QCA_OK;

    /* If it is the second device which is getting initialised,
     * Fill the pointers and generate the MAC address from the
     * first device
     */
    if (p_Global_Cxt)
    {
        QCA_CONTEXT_STRUCT_PTR tempqca_ptr = (QCA_CONTEXT_STRUCT_PTR)GET_DRIVER_CXT(p_Global_Cxt)->pUpperCxt[0];
        uint8_t devId;

        devId = qca_ptr->PARAM_PTR->QCA_IF->PHY_NUMBER;
        if (devId >= WLAN_NUM_OF_DEVICES) {
            return QCA_ERROR;
        }
        /* Generating the MAC Address of the second interface using the first interface MAC */
        A_MEMCPY(qca_ptr->ADDRESS, tempqca_ptr->ADDRESS, ATH_MAC_LEN);
        qca_ptr->ADDRESS[0] = (((qca_ptr->ADDRESS[0]) ^ (1 << devId)) | 0x2);
        // qca_ptr->ADDRESS[0] = ((qca_ptr->ADDRESS[0])+ (devId << 4)) | 0x2;

        qca_ptr->MAC_CONTEXT_PTR = p_Global_Cxt;
        GET_DRIVER_CXT(p_Global_Cxt)->pUpperCxt[devId] = qca_ptr;
        p_Driver_Cxt[1] = qca_ptr;
        return error;
    }

    do
    {
        /* allocate the driver context and assign it to the qca_ptr mac_param */
        if (NULL == (p_Global_Cxt = (void *)A_MALLOC(sizeof(A_CUSTOM_DRIVER_CONTEXT), MALLOC_ID_CONTEXT)))
        {
            error = QCA_ERROR;
            break;
        }

        A_MEMZERO(p_Global_Cxt, sizeof(A_CUSTOM_DRIVER_CONTEXT));
        /* alloc the driver common context */
        void *pCommonCxt = A_MALLOC(sizeof(A_DRIVER_CONTEXT), MALLOC_ID_CONTEXT);
        GET_DRIVER_CXT(p_Global_Cxt)->pCommonCxt = pCommonCxt;
        if (NULL == pCommonCxt)
        {
            error = QCA_ERROR;
            break;
        }

        qca_ptr->MAC_CONTEXT_PTR = p_Global_Cxt;
        /* initialize backwards pointers */
        GET_DRIVER_CXT(p_Global_Cxt)->pUpperCxt[0] = qca_ptr;
        GET_DRIVER_CXT(p_Global_Cxt)->pDriverParam = qca_ptr->PARAM_PTR->MAC_PARAM;
        /* create the 2 driver events. */

        /* Api_InitStart() will perform any additional allocations that are done as part of
         * the common_driver initialization */
        if (A_OK != Api_InitStart(p_Global_Cxt))
        {
            error = QCA_ERROR;
            break;
        }
/* CreateDriverThread is a custom function to create or restart the driver thread.
 * the bulk of the driver initialization is handled by the driver thread.
 */
        // Start the driver thread with maximum priority.
        // It will be scheduled immediately ensuring that Driver_Init() is called before
        // the main thread calls Api_InitFinish() below
        if (A_OK != Driver_CreateThread(p_Global_Cxt))
        {
            error = QCA_ERROR;
            break;
        }

        /* Api_InitFinish waits for wmi_ready event from chip. */
        if (A_OK != Api_InitFinish(p_Global_Cxt))
        {
            error = QCA_ERROR;
            break;
        }

        g_driverState = DRIVER_STATE_RUN;

        Api_WMIInitFinish(p_Global_Cxt);
    } while (0);

    if (error != QCA_OK)
    {
        if (p_Global_Cxt != NULL)
        {
#if 0
            void * tmp_addr;
            /* NOTE: ***do not free memory*** in case of timeout/failure 
             * there is no proper initialization fallback ! */

            /* Free driver context, may be interrupted */
            tmp_addr = ((cust_context_t *)p_Global_Cxt)->pCommonCxt;
            ((cust_context_t *)p_Global_Cxt)->pCommonCxt = NULL;
            A_FREE(tmp_addr, MALLOC_ID_CONTEXT);

            /* Free custom driver context, may be interrupted */
            tmp_addr = p_Global_Cxt;
            p_Global_Cxt = NULL;
            A_FREE(tmp_addr, MALLOC_ID_CONTEXT);
#endif
            return error;
        }
    }
    else
    {
        p_Driver_Cxt[0] = qca_ptr;
    }

#if 0       
if(g_totAlloc){
    A_PRINTF("init alloc: %d\n", g_totAlloc);	
    //for more information one can implement _mem_get_free() to 
    //determine how much free memory exists in the system pool.
}
#endif

    Api_BootProfile(p_Global_Cxt, BOOT_PROFILE_DRIVE_READY);

    return error;
}

/*****************************************************************************/
/*  Custom_Api_Shutdown - Entry point for MQX to shutdown the Driver.
 *      QCA_CONTEXT_STRUCT_PTR qca_ptr - pointer to MQX ethernet object.
 * RETURNS: QCA_OK on success or QCA_ERROR otherwise.
 *****************************************************************************/
static uint32_t Custom_Api_Shutdown(struct qca_context_struct *qca_ptr)
{
    void *pCxt = qca_ptr->MAC_CONTEXT_PTR;

    // Notify driver task to reset itself
    Driver_DestroyThread(pCxt);

//TODO: It simply cannot work
#if 0
    if (pCxt != NULL)
    {
        Api_DeInitStart(pCxt);
        Driver_DestroyThread(pCxt);
        Api_DeInitFinish(pCxt);

        if (NULL != GET_DRIVER_COMMON(pCxt))
        {
            A_FREE(GET_DRIVER_COMMON(pCxt), MALLOC_ID_CONTEXT);
        }

        A_FREE(pCxt, MALLOC_ID_CONTEXT);
        p_Global_Cxt = NULL;
        qca_ptr->MAC_CONTEXT_PTR = NULL;
    }
#endif

    return (uint32_t)QCA_OK;
}

/*****************************************************************************/
/*  Custom_Api_GetDriverCxt - return driver context based on device ID
 *      QCA_CONTEXT_STRUCT_PTR qca_ptr - pointer to MQX ethernet object.
 * RETURNS: QCA_OK on success or QCA_ERROR otherwise.
 *****************************************************************************/
void *Custom_Api_GetDriverCxt(uint8_t device_id)
{
    return (p_Driver_Cxt[device_id]);
}
