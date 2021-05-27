/***************************************************************************//**
* \file cy_i3c.c
* \version 1.00
*
* Provides API implementation for the I3C Controller.
*
********************************************************************************
* \copyright
* Copyright 2019-2020 Cypress Semiconductor Corporation
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

#if defined (CY_IP_MXI3C)

#include "cy_i3c.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*                             Function Prototypes
*******************************************************************************/
static uint8_t even_parity(uint8_t address);
static uint32_t ffs(uint32_t value);
static void SetAddrslotStatus(uint8_t address, cy_en_i3c_addr_slot_status_t status, cy_stc_i3c_context_t *context);
static void InitAddrslots(cy_stc_i3c_context_t *context);
static void DeInitAddrslots(cy_stc_i3c_context_t *context);
static uint32_t GetI2CDevAddrPos(I3C_CORE_Type *base, uint8_t staticAddress, cy_stc_i3c_context_t *context);
static uint32_t GetI3CDevAddrPos(I3C_CORE_Type *base, uint8_t dynamicAddress, cy_stc_i3c_context_t *context);
static uint32_t GetDATFreePos(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static cy_en_i3c_addr_slot_status_t GetAaddrslotStatus(uint8_t address, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t ResponseError(uint32_t respCmd);
static uint32_t ResponseErrorEvent(uint32_t respCmd);
static void WriteArray(I3C_CORE_Type *base, void *buffer, uint32_t size);
static void ReadArray(I3C_CORE_Type *base, void *buffer, uint32_t size);
static void MasterHandleDataTransmit(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static void MasterHandleDataReceive(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static void RetrieveI3CDeviceInfo(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, bool basicInfo, cy_stc_i3c_context_t *context);
static void CCC_Set(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static void CCC_Get(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t enec_disec_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t rstdaa_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t setmrwl_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static void setda_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t setdasa_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t setnewda_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t entas_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t enthdr0_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getmrwl_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getpid_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getbcr_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getdcr_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getstatus_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getmxds_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t gethdrcap_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t getaccmst_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t defslvs_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t MasterHDRWrite(I3C_CORE_Type *base, cy_stc_i3c_hdr_cmd_t *hdrCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t MasterHDRRead(I3C_CORE_Type *base, cy_stc_i3c_hdr_cmd_t *hdrCmd, cy_stc_i3c_context_t *context);
static void MasterRespReadyStsHandle(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static void MasterHandleWriteInterrupt(I3C_CORE_Type *base, uint32_t respCmdPort, cy_stc_i3c_context_t *context);
static void MasterHandleReadInterrupt(I3C_CORE_Type *base, uint32_t respCmdPort, cy_stc_i3c_context_t *context);
static void MasterHandleIBIInterrupt(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t RearrangeAddrTable(I3C_CORE_Type *base, uint32_t devIndex, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t DeviceIBIControl(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, uint8_t cccCmd, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t CCCSlaveAddressValidation(uint8_t address, bool unicastOnly, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t MasterHandleCCCResponse(I3C_CORE_Type *base, uint32_t *resp, cy_stc_i3c_context_t *context);
static void SlaveRespReadyStsHandle(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static void SlaveHandleDataReceive(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static void SlaveHandleDataTransmit(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
static cy_en_i3c_status_t SecondaryMasterInit(I3C_CORE_Type *base, bool isMaster, cy_stc_i3c_context_t *context);

/*******************************************************************************
*  Function Name: Cy_I3C_Init
****************************************************************************//**
*
* Initializes the I3C block.
*
* \param base
* The pointer to the I3C instance.
* 
* \param config
* The pointer to the configuration structure \ref cy_stc_i3c_config_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
* \note
* Ensure that the I3C block is disabled before calling this function.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_Init(I3C_CORE_Type *base, cy_stc_i3c_config_t const *config, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == config) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    CY_ASSERT_L3(CY_I3C_IS_MODE_VALID(config->i3cMode));
    CY_ASSERT_L3(CY_I3C_IS_BUS_MODE_VALID(config->i3cBusMode));

    /* Main master init */
    I3C->CTRL= I3C_CTRL_ENABLED_Msk; //Outside core register

    if(CY_I3C_MASTER == config->i3cMode)
    {
        cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
        uint8_t address; 
        cy_en_i3c_status_t retStatus;

        i3cMaster->freePos = 0x7FFUL;
        i3cMaster->lastAddress = 0;
        i3cMaster->devCount = 0;
        i3cMaster->i2cDeviceCount = 0;
        i3cMaster->dynAddrDevCount = 0;

        // Prepare the address slot status array
        InitAddrslots(context);

        //Assigning address to the master
        retStatus = Cy_I3C_MasterGetFreeDeviceAddress(base, &address, context);
        if(CY_I3C_MASTER_FREE_ADDR_UNAVIAL == retStatus)
            return retStatus;
        
        I3C_CORE_DEVICE_ADDR(base) = _VAL2FLD( I3C_CORE_DEVICE_ADDR_DYNAMIC_ADDR, address) | I3C_CORE_DEVICE_ADDR_DYNAMIC_ADDR_VALID_Msk;

        SetAddrslotStatus(address, CY_I3C_ADDR_SLOT_I3C_DEV, context); 

        i3cMaster->lastAddress = address;
        I3C_CORE_DEVICE_CTRL(base) = I3C_CORE_DEVICE_CTRL_IBA_INCLUDE_Msk;
        //Setting the device operation mode to Master
        I3C_CORE_DEVICE_CTRL_EXTENDED(base) = _VAL2FLD(I3C_CORE_DEVICE_CTRL_EXTENDED_DEV_OPERATION_MODE, 0);

        Cy_I3C_SetDataRate(base, config->i3cSclRate, config->i3cClockHz, context); 

        Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
    
    }

    if((config->i3cMode == CY_I3C_SLAVE) || (config->i3cMode == CY_I3C_SECONDARY_MASTER))
    {
        if(config->staticAddress)
        {
            /* The reset value of these fields are set from strap input static_addr_en and statis_addr. 
               The application can overwrite this value by programming this register before enabling the controller */

            I3C->STATIC_ADDR = I3C_STATIC_ADDR_STATIC_ADDR_EN_Msk;
            
            I3C_CORE_DEVICE_ADDR(base) |= _VAL2FLD(I3C_CORE_DEVICE_ADDR_STATIC_ADDR, config->staticAddress) |
                                          I3C_CORE_DEVICE_ADDR_STATIC_ADDR_VALID_Msk;
        }

        /*
            1. Configure the SLV_CHAR_CTRL register
            2. Configure the two PID registers
            3. Configure DEVICE_CTRL_EXTENDED register
            4. Configure ADAPTIVE_I2C_I3C field in DEVICE_CTRL register
        */
        
        I3C_CORE_SLV_CHAR_CTRL(base) &= _VAL2FLD(I3C_CORE_SLV_CHAR_CTRL_MAX_DATA_SPEED_LIMIT, config->speedLimit) |
                                        _VAL2FLD(I3C_CORE_SLV_CHAR_CTRL_HDR_CAPABLE, config->hdrCapable) |
                                        _VAL2FLD(I3C_CORE_SLV_CHAR_CTRL_DEVICE_ROLE, 0);
        
        I3C_CORE_DEVICE_CTRL_EXTENDED(base) = _VAL2FLD(I3C_CORE_DEVICE_CTRL_EXTENDED_DEV_OPERATION_MODE, config->i3cMode);
        
        //I3C_CORE_SLV_PID_VALUE(base) = (uint32_t)config->pid;

        /* On setting this ADAPTIVE_I2C_I3C bit, the controller generates hot-join request only when the is in/changes to I3C mode of operation. */
        //I3C_CORE_DEVICE_CTRL(base) |= I3C_CORE_DEVICE_CTRL_ADAPTIVE_I2C_I3C_Msk; //tbd - ask about the idle count multiplier

    /* Note:
        1. If this bit is set, the controller is allowed to send hot-join request intr on the bus. This bit, if set, can be set or cleared by the I3C master
           via ENEC or DISEC CCCs.
        2. If this bit is set to 0, the controller will not be allowed to send hot-join request intr on the bus. The CCCs will not have any effects on this field.
    */
    
        I3C_CORE_SLV_EVENT_STATUS(base) &= (config->hotjoinEnable)?I3C_CORE_SLV_EVENT_STATUS_HJ_EN_Msk:(~I3C_CORE_SLV_EVENT_STATUS_HJ_EN_Msk);

        //Sets the number of entries in the Receive FIFO that trigger the interrupt to 1 word
        I3C_CORE_DATA_BUFFER_THLD_CTRL(base) &= _VAL2FLD(I3C_CORE_DATA_BUFFER_THLD_CTRL_RX_BUF_THLD, 5) |
                                                _VAL2FLD(I3C_CORE_DATA_BUFFER_STATUS_LEVEL_TX_BUF_EMPTY_LOC, 1);

        if(config->i3cMode == CY_I3C_SECONDARY_MASTER)
        {
            Cy_I3C_SetInterruptMask(base, CY_I3C_SLV_INTR_Msk | CY_I3C_INTR_DEFSLV_STS);
            Cy_I3C_SetInterruptStatusMask(base, CY_I3C_SLV_INTR_Msk | CY_I3C_INTR_DEFSLV_STS);
        }

        else
        {
            Cy_I3C_SetInterruptMask(base, CY_I3C_SLV_INTR_Msk);
            Cy_I3C_SetInterruptStatusMask(base, CY_I3C_SLV_INTR_Msk);
        }

        context->i3cSclRate = config->i3cSclRate;
    }

    // Configure the registers
    
    //Sets the number of entries in the Response Queue that trigger the interrupt to 1 word
    //and the number of entries in the IBI Queue that trigger the interrupt to 1 word
    
    I3C_CORE_QUEUE_THLD_CTRL(base) = _VAL2FLD(I3C_CORE_QUEUE_THLD_CTRL_RESP_BUF_THLD, 0) |
                                     _VAL2FLD(I3C_CORE_QUEUE_THLD_CTRL_IBI_STATUS_THLD, 0);
    
    /* Initialize the context */
    context->i3cMode = config->i3cMode;
    context->i3cBusMode = config->i3cBusMode;
    context->i3cClockHz = config->i3cClockHz;
    context->state = CY_I3C_IDLE;
    
    /* Master specific */
    context->masterBuffer = NULL;
    context->masterStatus = 0UL;
    context->masterBufferIdx = 0UL;
    context->masterBufferSize = 0UL;
    context->destDeviceAddr = 0UL;
    context->hdrCmd = NULL;
    
    /*Slave specific */
    context->slaveStatus = 0UL;
    
    context->slaveRxBuffer = NULL;
    context->slaveRxBufferIdx = 0UL;
    context->slaveRxBufferSize = 0UL;
    context->slaveRxBufferCnt = 0UL;
    
    context->slaveTxBuffer = NULL;
    context->slaveTxBufferIdx = 0UL;
    context->slaveTxBufferSize = 0UL;
    context->slaveTxBufferCnt = 0UL;
    
    /* Unregister callbacks */
    context->cbEvents = NULL;
    context->cbIbi = NULL;

    return CY_I3C_SUCCESS;
    
}


/*******************************************************************************
*  Function Name: Cy_I3C_Deinit
****************************************************************************//**
*
* Deinitializes the I3C block.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_DeInit(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    
    uint8_t index;

    Cy_I3C_Disable(base);
    if(CY_I3C_SLAVE != context->i3cMode) //Test: Should this be done for secondary master too?
    {
        //Set the address slot statuses to zero
        DeInitAddrslots(context);

        for(index = 0; (index < CY_I3C_MAX_DEVS); index++)
        {
            //Clears the DAT entries
            Cy_I3C_WriteIntoDeviceAddressTable(base, index, 0UL);
        }
    }
    I3C_CORE_QUEUE_THLD_CTRL(base) = I3C_CORE_QUEUE_THLD_CTRL_DEF_VAL;
    I3C_CORE_DATA_BUFFER_THLD_CTRL(base) = I3C_CORE_DATA_BUFFER_THLD_CTRL_DEF_VAL;

    I3C_CORE_INTR_STATUS_EN(base) = 0UL;
    I3C_CORE_INTR_SIGNAL_EN(base) = 0UL;

}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterI2CAttachDevice
****************************************************************************//**
*
* Attaches an I2C device to the bus as defined by \ref cy_stc_i2c_device_t.
* It is required to provide device static address and lvr information 
* in \ref cy_stc_i2c_device_t.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i2cDevice
* The pointer to the I2C device description structure \ref cy_stc_i2c_device_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterAttachI2CDevice(I3C_CORE_Type *base, cy_stc_i2c_device_t *i2cDevice, cy_stc_i3c_context_t *context)
{
    
    if((NULL == base) || (NULL == i2cDevice) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    uint32_t value, pos;
    uint8_t lvr;

    if(CY_I3C_MAX_DEVS <= (i3cMaster->devCount))  //cannot initialize the device as the bus already has 11 devices on it
        return CY_I3C_MASTER_MAX_DEVS_PRESENT;
        
    /* LVR: Legacy Virtual Register

    bit[7-5]: 000 - Legacy I2C only with 50ns IO Spike Filter
              001 and 010 - Legacy I2C only without 50ns IO Spike Filter
              others - reserved */
              
    lvr = i2cDevice->lvr;

    if( CY_I3C_ADDR_SLOT_FREE != GetAaddrslotStatus(i2cDevice->staticAddress, context))
        return CY_I3C_MASTER_FREE_ADDR_UNAVIAL;
    
    if(0 != (CY_I3C_LVR_LEGACY_I2C_INDEX_MASK & lvr)) //does not have a 50ns Spike Filter, So return - BROS mentions not to support devices that lack the spike filters
        return CY_I3C_MASTER_BAD_I2C_DEVICE;  
    
    SetAddrslotStatus(i2cDevice->staticAddress, CY_I3C_ADDR_SLOT_I2C_DEV, context);

    // populate the device address table with the static address of the device
    value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_STATIC_ADDR, i2cDevice->staticAddress) |
            I3C_CORE_DEV_ADDR_TABLE_LOC1_LEGACY_I2C_DEVICE_Msk;

    pos = GetDATFreePos(base, context);
    
    Cy_I3C_WriteIntoDeviceAddressTable(base, pos, value);

    //Maintaining a local list of i2c devices
    Cy_I3C_UpdateI2CDevInList(i2cDevice, pos, context);
    
    //increment the free position index of the device address table
    (i3cMaster->freePos) = ~ (CY_I3C_BIT(pos));
    (i3cMaster->devCount)++;
    (i3cMaster->i2cDeviceCount)++;

    I3C_CORE_DEVICE_CTRL(base) |= I3C_CORE_DEVICE_CTRL_I2C_SLAVE_PRESENT_Msk;

    return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterDetachI2CDevice
****************************************************************************//**
*
* Detaches an I2C device from the bus defined by \ref cy_stc_i2c_device_t.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i2cDevice
* pointer to the I2C device description structure \ref cy_stc_i2c_device_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterDetachI2CDevice(I3C_CORE_Type *base, cy_stc_i2c_device_t *i2cDevice, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == i2cDevice) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    uint32_t devIndex;

    //"devIndex" - Position of the device which has to be detached
    devIndex = GetI2CDevAddrPos(base, i2cDevice->staticAddress, context);
    if(devIndex == CY_I3C_BAD_PARAM)
        return CY_I3C_BAD_PARAM;
    
    RearrangeAddrTable(base, devIndex, context);

    context->i3cMaster.devCount--;
    context->i3cMaster.i2cDeviceCount--;
    SetAddrslotStatus((i2cDevice->staticAddress), CY_I3C_ADDR_SLOT_FREE, context);
    
    return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterI3CAttachDevice
****************************************************************************//**
*
* Attaches an I3C device to the bus defined by \ref cy_stc_i3c_device_t.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDevice
* The pointer to the i3c device description structure \ref cy_stc_i3c_device_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
/* This is blocking function, where we wait for the response packet, 
   assuming the master treats no response as NACK after waiting for a decent amount of time */
cy_en_i3c_status_t Cy_I3C_MasterAttachI3CDevice(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == i3cDevice) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    uint8_t dynamicAddress = 0;
    cy_en_i3c_status_t retStatus;
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    cy_stc_i3c_ccc_cmd_t cmd;
    cy_stc_i3c_ccc_setda_t i3cCccSetda;
    cy_stc_i3c_ccc_payload_t payload;

    if(CY_I3C_MAX_DEVS <= (i3cMaster->devCount))  //cannot initialize the device as the bus already has 11 devices on it
        return CY_I3C_MASTER_MAX_DEVS_PRESENT;

    if(0 != i3cDevice->dynamicAddress)
    {
        /* device has a expected dynamic address */

        /* check if the expected dynamic address is available to be assigned */ 
        /* If not available, return */
        
        if(CY_I3C_ADDR_SLOT_FREE != GetAaddrslotStatus(i3cDevice->dynamicAddress, context))
            return CY_I3C_MASTER_FREE_ADDR_UNAVIAL;
        
        dynamicAddress = i3cDevice->dynamicAddress;
    }

    else
    {
        /* If the Slave device doesn't have a expected dynamic address, pick a free address from the list */    
        retStatus = Cy_I3C_MasterGetFreeDeviceAddress(base, &dynamicAddress, context);
        if(CY_I3C_MASTER_FREE_ADDR_UNAVIAL == retStatus)
            return retStatus;

        i3cMaster->lastAddress = dynamicAddress;
    }

    //Send SETDASA CCC command
    i3cCccSetda.address = dynamicAddress;
    cmd.data = &payload;
    cmd.data->data = &(i3cCccSetda.address); //check
    cmd.data->len = sizeof(i3cCccSetda);
    cmd.cmd = CY_I3C_CCC_SETDASA;
    cmd.address = i3cDevice->staticAddress;
    retStatus = setdasa_ccc(base, &cmd, context);

    return retStatus;
    
}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterDetachI3CDevice
****************************************************************************//**
*
* Detaches an I3C device from the bus defined by \ref cy_stc_i3c_device_t.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDevice
* The pointer to the I3C device description structure \ref cy_stc_i3c_device_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
//TBD - Check if the device is hot-join capable, if yes send DISEC with HOT-JOIN event
cy_en_i3c_status_t Cy_I3C_MasterDetachI3CDevice(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == i3cDevice) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    uint32_t devIndex ;
    cy_en_i3c_status_t retstatus;
    cy_stc_i3c_ccc_cmd_t cccCmd;
    cy_stc_i3c_ccc_payload_t payload;
    
    devIndex = GetI3CDevAddrPos(base, i3cDevice->dynamicAddress, context);
    if(CY_I3C_BAD_PARAM == devIndex)
        return CY_I3C_BAD_PARAM;

    cccCmd.address = i3cDevice->dynamicAddress;
    cccCmd.cmd = CY_I3C_CCC_RSTDAA(false);
    cccCmd.data = &payload;
    cccCmd.data->data = NULL;
    cccCmd.data->len = 0;

    retstatus = rstdaa_ccc(base, &cccCmd, context);
    if(CY_I3C_SUCCESS != retstatus)
        return retstatus;

    RearrangeAddrTable(base, devIndex, context);

    context->i3cMaster.devCount--;
    SetAddrslotStatus((i3cDevice->dynamicAddress), CY_I3C_ADDR_SLOT_FREE, context);

    return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: Cy_I3C_Enable
****************************************************************************//**
* Enables the I3C block.
*
* \param base
* The pointer to the I3C instance.
*
*******************************************************************************/
void Cy_I3C_Enable(I3C_CORE_Type *base)
{
    CY_ASSERT_L1(NULL != base);
    
    I3C_CORE_DEVICE_CTRL(base) |= I3C_CORE_DEVICE_CTRL_ENABLE_Msk;
}


/*******************************************************************************
*  Function Name: Cy_I3C_Disable
****************************************************************************//**
* Disables the I3C block.
*
* \param base
* The pointer to the I3C instance.
*
* \note
* The I3C controller will complete any pending bus transaction before it gets
* disabled.
*******************************************************************************/
void Cy_I3C_Disable(I3C_CORE_Type *base)
{
    CY_ASSERT_L1(NULL != base);

    I3C_CORE_DEVICE_CTRL(base) |= (~I3C_CORE_DEVICE_CTRL_ENABLE_Msk);

    do
    {
        //Wait until the master comes to IDLE state
    }while(I3C_CORE_PRESENT_STATE(base));
}


/*******************************************************************************
*  Function Name: Cy_I3C_Resume
****************************************************************************//**
*
* Resumes the I3C Controller after an error state.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \note
* When the controller enters any error condition it pauses all operation until 
* user resumes manually. User must call this API once error status is returned 
* for any I3C bus transactions like Read/Write or other operations.
*
*******************************************************************************/
void Cy_I3C_Resume(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);

    (void)context;
    
    //uint32_t status;
    //status = Cy_I3C_GetBusStatus(base, context);
#if 0
    if(status &  CY_I3C_MASTER_HALT_STATE)
    {
        I3C_CORE_DEVICE_CTRL(base) |= I3C_CORE_DEVICE_CTRL_RESUME_Msk;
    }
#endif

    I3C_CORE_DEVICE_CTRL(base) |= I3C_CORE_DEVICE_CTRL_RESUME_Msk;

}


/*******************************************************************************
*  Function Name: Cy_I3C_GetI2CDeviceCount
****************************************************************************//**
*
* Provides the number of I2C devices attached to the bus.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Number of I2C devices attached to the bus.
*
*******************************************************************************/
uint32_t Cy_I3C_GetI2CDeviceCount(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    /* Suppress a compiler warning about unused variables */
    (void) base;

    return (context->i3cMaster.i2cDeviceCount);
}


/*******************************************************************************
*  Function Name: Cy_I3C_GetI3CDeviceCount
****************************************************************************//**
*
* Provides the number of I3C devices attached to the bus.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Number of I3C devices attached to the bus.
*
*******************************************************************************/
uint32_t Cy_I3C_GetI3CDeviceCount(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    /* Suppress a compiler warning about unused variables */
    (void) base;
    
    return ((context->i3cMaster.devCount)-(context->i3cMaster.i2cDeviceCount));
}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterGetI2CDevices
****************************************************************************//**
*
* Provides the list of I2C devices on the bus.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i2cDeviceList
* The pointer to the I2C device list array.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterGetI2CDevices(I3C_CORE_Type *base, cy_stc_i2c_device_t *i2cDeviceList, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == i2cDeviceList) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    uint8_t index;
    cy_stc_i3c_master_devlist_t *ptr = context->devList;

    for(index = 0; index < i3cMaster->i2cDeviceCount;)
    {
        if(ptr->i2c)
        {
            *i2cDeviceList = ptr->i2cDevice;
            index++;
            i2cDeviceList++;
        }
        ptr++;
    }
    return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterGetI3CDevices
****************************************************************************//**
*
* Provides the list of I3C devices on the bus.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDeviceList
* The pointer to the I3C device list array.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterGetI3CDevices(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDeviceList, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == i3cDeviceList) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    uint8_t index, count;
    cy_stc_i3c_master_devlist_t *ptr = context->devList;
    count = (i3cMaster->devCount) - (i3cMaster->i2cDeviceCount);

    for(index = 0; index < count;)
    {
        if(!ptr->i2c)
        {
            *i3cDeviceList = ptr->i3cDevice;
            index++;
            i3cDeviceList++;
        }
        ptr++;
    }
    return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: Cy_I3C_GetFreeDeviceAddress
****************************************************************************//**
*
* Provides the next dynamic address available to be assigned.
*
* \param base
* The pointer to the I3C instance.
*
* \param address
* The pointer to the address.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterGetFreeDeviceAddress(I3C_CORE_Type *base, uint8_t *address, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == address) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    cy_en_i3c_addr_slot_status_t status;
    uint8_t freeAddress;
    uint8_t startAddress = (i3cMaster->lastAddress) + 1;

    for(freeAddress = startAddress; freeAddress < CY_I3C_MAX_ADDR; freeAddress++)
    {
        status = GetAaddrslotStatus(freeAddress, context);
        if(CY_I3C_ADDR_SLOT_FREE == status)
        {
            *address = freeAddress;
            return CY_I3C_SUCCESS;
        }
    }

    //case when a device is detached from the bus and the address is available
    for(freeAddress = 1; freeAddress < startAddress; freeAddress++)
    {
        status = GetAaddrslotStatus(freeAddress, context);
        if(CY_I3C_ADDR_SLOT_FREE == status)
        {
            *address = freeAddress;
            return CY_I3C_SUCCESS;
        }
    }

    return CY_I3C_MASTER_FREE_ADDR_UNAVIAL;
}


/*******************************************************************************
*  Function Name: Cy_I3C_isCCCCmdSupported
****************************************************************************//**
*
* Checks if the CCC command is supported.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The CCC command to be checked.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ture: If the specified command is supported
* false: If the specified command is not supported.
*
*******************************************************************************/
bool Cy_I3C_isCCCCmdSupported(I3C_CORE_Type *base, uint8_t cccCmd, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    /* Suppress a compiler warning about unused variables */
    (void) base;

    switch (cccCmd)
        {
            case CY_I3C_CCC_ENEC(true):
            case CY_I3C_CCC_ENEC(false):
            case CY_I3C_CCC_DISEC(true):
            case CY_I3C_CCC_DISEC(false):
            case CY_I3C_CCC_ENTAS(0, true):
            case CY_I3C_CCC_ENTAS(0, false):
            case CY_I3C_CCC_RSTDAA(true):
            case CY_I3C_CCC_RSTDAA(false):
            case CY_I3C_CCC_ENTDAA:
            case CY_I3C_CCC_SETMWL(true):
            case CY_I3C_CCC_SETMWL(false):
            case CY_I3C_CCC_SETMRL(true):
            case CY_I3C_CCC_SETMRL(false):
            case CY_I3C_CCC_ENTHDR(0):
            case CY_I3C_CCC_SETDASA:
            case CY_I3C_CCC_SETNEWDA:
            case CY_I3C_CCC_GETMWL:
            case CY_I3C_CCC_GETMRL:
            case CY_I3C_CCC_GETPID:
            case CY_I3C_CCC_GETBCR:
            case CY_I3C_CCC_GETDCR:
            case CY_I3C_CCC_GETSTATUS:
            case CY_I3C_CCC_GETMXDS:
            case CY_I3C_CCC_GETHDRCAP:
            case CY_I3C_CCC_GETACCMST:
            case CY_I3C_CCC_DEFSLVS:
                return true;
            default:
                return false;
        }
}


/*******************************************************************************
*  Function Name: Cy_I3C_SendCCCCmd
****************************************************************************//**
*
* Post the specified CCC command to command queue.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the CCC command description structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_SendCCCCmd(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == cccCmd) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    cy_en_i3c_status_t retStatus;

    context->masterStatus = CY_I3C_MASTER_BUSY;
    switch(cccCmd->cmd)
    {
        case CY_I3C_CCC_ENEC(true):
        case CY_I3C_CCC_ENEC(false): 
        case CY_I3C_CCC_DISEC(true):
        case CY_I3C_CCC_DISEC(false):
            {
                retStatus = enec_disec_ccc(base, cccCmd, context); 
                break;
            }
        case CY_I3C_CCC_RSTDAA(true):
        case CY_I3C_CCC_RSTDAA(false):
            {
                retStatus = rstdaa_ccc(base, cccCmd, context); 
                break;
            }
        case CY_I3C_CCC_SETMWL(true):
        case CY_I3C_CCC_SETMWL(false):
        case CY_I3C_CCC_SETMRL(true):
        case CY_I3C_CCC_SETMRL(false):
            {
                retStatus = setmrwl_ccc(base, cccCmd, context); 
                break;
            }
        case CY_I3C_CCC_SETDASA:
            {
                retStatus = setdasa_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_SETNEWDA:
            {
                retStatus = setnewda_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETMWL:
        case CY_I3C_CCC_GETMRL:
            {
                retStatus = getmrwl_ccc(base, cccCmd, context);
                break;
            }
        
        case CY_I3C_CCC_GETPID:
            {
                retStatus = getpid_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETBCR:
            {
                retStatus = getbcr_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETDCR:
            {
                retStatus = getdcr_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETSTATUS:
            {
                retStatus = getstatus_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETMXDS:
            {
                retStatus = getmxds_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETHDRCAP:
            {
                retStatus = gethdrcap_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_ENTAS(0, true):
        case CY_I3C_CCC_ENTAS(0, false):
        case CY_I3C_CCC_ENTAS(1, true):
        case CY_I3C_CCC_ENTAS(1, false):
        case CY_I3C_CCC_ENTAS(2, true):
        case CY_I3C_CCC_ENTAS(2, false):
        case CY_I3C_CCC_ENTAS(3, true):
        case CY_I3C_CCC_ENTAS(3, false):
            {
                retStatus = entas_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_ENTHDR(0):
            {
                retStatus = enthdr0_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_GETACCMST:
            {
                retStatus = getaccmst_ccc(base, cccCmd, context);
                break;
            }
        case CY_I3C_CCC_DEFSLVS:
            {
                retStatus = defslvs_ccc(base, cccCmd, context);
                break;
            }
        default:
            {
                retStatus = CY_I3C_MASTER_CCC_NOT_SUPPORTED;
            }
    }

    context->masterStatus &= (~CY_I3C_MASTER_BUSY);
    return retStatus;
}


/*******************************************************************************
*  Function Name: Cy_I3C_DisableDeviceIbi
****************************************************************************//**
*
* Disables all IBI events from specified device.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDevice
* pointer to the i3c device description structure \ref cy_stc_i3c_device_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_DisableDeviceIbi(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context)
{    
    if((NULL == base) || (NULL == i3cDevice) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    cy_en_i3c_status_t retStatus;

    retStatus = DeviceIBIControl(base, i3cDevice, CY_I3C_CCC_DISEC(false), context);

    return retStatus;
}


/*******************************************************************************
*  Function Name: Cy_I3C_EnableDeviceIbi
****************************************************************************//**
*
* Enables all IBI events from specified device.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDevice
* pointer to the i3c device description structure \ref cy_stc_i3c_device_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_config_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_EnableDeviceIbi(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == i3cDevice) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    uint32_t sirmap, bitpos, mrmap;
    cy_en_i3c_status_t retStatus;

    //Slave Interrupt Request
    sirmap = I3C_CORE_IBI_SIR_REQ_REJECT(base);
    bitpos = CY_I3C_IBI_SIR_REQ_ID(i3cDevice->dynamicAddress);
    sirmap &= ~((1UL) << bitpos); //Setting the corresponding bit to 0: 0 -> Ack the SIR from the corresponding device
    I3C_CORE_IBI_SIR_REQ_REJECT(base) = sirmap;

    //Mastership Request
    mrmap = I3C_CORE_IBI_MR_REQ_REJECT(base);
    mrmap &= ~((1UL) << bitpos); //Setting the corresponding bit to 0: 0 -> Ack the MR from the corresponding device
    I3C_CORE_IBI_MR_REQ_REJECT(base) = mrmap;

    retStatus = DeviceIBIControl(base, i3cDevice, CY_I3C_CCC_ENEC(false), context);

    return retStatus;
}


/*******************************************************************************
*  Function Name: Cy_I3C_SetDataRate
****************************************************************************//**
*
* Sets desired maximum I3C bus data rate for transfers.
*
* \param base
* The pointer to the I3C instance.
* 
* \param dataRateHz
* The desired I3C data rate in Hz.
*
* \param i3cClockHz
* The frequency of the clock connected to the I3C Block in Hz.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* The achieved data rate in Hz.
* When zero value is returned there is an error in the input parameters: 
* data rate or clk_i3c is out of valid range.
*
*******************************************************************************/
uint32_t Cy_I3C_SetDataRate(I3C_CORE_Type *base, uint32_t dataRateHz, uint32_t i3cClockHz, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    CY_ASSERT_L2(i3cClockHz > 0UL);
    CY_ASSERT_L3(CY_I3C_IS_SDR_DATA_RATE_VALID(dataRateHz));
    
    uint32_t i3cClockPeriod; //in nanoSeconds
    uint32_t resDataRate; //stores the achieved data rate
    uint8_t hcnt,lcnt;

    //CY_I3C_DIV_ROUND_UP - quotient will be rounded to the next number in non-integer cases
    i3cClockPeriod = CY_I3C_DIV_ROUND_UP(1000000000, i3cClockHz); //time period in nanoseconds

    //high phase will be 41ns irrespective of whether the bus is in PureBusMode or MixedBusMode
    
    hcnt = CY_I3C_DIV_ROUND_UP(CY_I3C_BUS_THIGH_MAX_NS, i3cClockPeriod) - 1;
    if (hcnt < CY_I3C_SCL_I3C_TIMING_CNT_MIN)
        hcnt = CY_I3C_SCL_I3C_TIMING_CNT_MIN;

    lcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, dataRateHz) - hcnt;
    if (lcnt < CY_I3C_SCL_I3C_TIMING_CNT_MIN)
        lcnt = CY_I3C_SCL_I3C_TIMING_CNT_MIN;

    I3C_CORE_SCL_I3C_PP_TIMING(base) = _VAL2FLD(I3C_CORE_SCL_I3C_PP_TIMING_I3C_PP_HCNT, hcnt) |
                                         _VAL2FLD(I3C_CORE_SCL_I3C_PP_TIMING_I3C_PP_LCNT, lcnt);
    
    //configure the BUS_FREE_AVAIL_TIMING register
    I3C_CORE_BUS_FREE_AVAIL_TIMING(base) = _VAL2FLD(I3C_CORE_BUS_FREE_AVAIL_TIMING_BUS_FREE_TIME, lcnt);

    resDataRate = CY_I3C_DIV_ROUND_UP(1000000000,((hcnt + lcnt) * (i3cClockPeriod)));

    lcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, (dataRateHz < CY_I3C_SDR1_DATA_RATE) ? dataRateHz : CY_I3C_SDR1_DATA_RATE) - hcnt;
    I3C_CORE_SCL_EXT_LCNT_TIMING(base) |= _VAL2FLD(I3C_CORE_SCL_EXT_LCNT_TIMING_I3C_EXT_LCNT_1, lcnt);
    lcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, (dataRateHz < CY_I3C_SDR2_DATA_RATE) ? dataRateHz : CY_I3C_SDR2_DATA_RATE) - hcnt;
    I3C_CORE_SCL_EXT_LCNT_TIMING(base) |= _VAL2FLD(I3C_CORE_SCL_EXT_LCNT_TIMING_I3C_EXT_LCNT_2, lcnt);
    lcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, (dataRateHz < CY_I3C_SDR3_DATA_RATE) ? dataRateHz : CY_I3C_SDR3_DATA_RATE) - hcnt;
    I3C_CORE_SCL_EXT_LCNT_TIMING(base) |= _VAL2FLD(I3C_CORE_SCL_EXT_LCNT_TIMING_I3C_EXT_LCNT_3, lcnt);
    lcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, (dataRateHz < CY_I3C_SDR4_DATA_RATE) ? dataRateHz : CY_I3C_SDR4_DATA_RATE) - hcnt;
    I3C_CORE_SCL_EXT_LCNT_TIMING(base) |= _VAL2FLD(I3C_CORE_SCL_EXT_LCNT_TIMING_I3C_EXT_LCNT_4, lcnt);

    if(CY_I3C_BUS_PURE != context->i3cBusMode)
    {
        //Mixed mode bus
        
        lcnt = CY_I3C_DIV_ROUND_UP(CY_I3C_BUS_I2C_FMP_TLOW_MIN_NS, i3cClockPeriod);
        hcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, CY_I3C_I2C_FMP_DATA_RATE) - lcnt;

        I3C_CORE_SCL_I2C_FMP_TIMING(base) = _VAL2FLD(I3C_CORE_SCL_I2C_FMP_TIMING_I2C_FMP_HCNT, hcnt) |
                                            _VAL2FLD(I3C_CORE_SCL_I2C_FMP_TIMING_I2C_FMP_LCNT, lcnt);

        lcnt = CY_I3C_DIV_ROUND_UP(CY_I3C_BUS_I2C_FM_TLOW_MIN_NS, i3cClockPeriod);
        hcnt = CY_I3C_DIV_ROUND_UP(i3cClockHz, CY_I3C_I2C_FM_DATA_RATE) - lcnt;

        I3C_CORE_SCL_I2C_FM_TIMING(base) = _VAL2FLD(I3C_CORE_SCL_I2C_FM_TIMING_I2C_FM_HCNT, hcnt) |
                                            _VAL2FLD(I3C_CORE_SCL_I2C_FM_TIMING_I2C_FM_LCNT, lcnt);

        //configure the BUS_FREE_AVAIL_TIMING register with lcnt from Fast-Mode 

        I3C_CORE_BUS_FREE_AVAIL_TIMING(base) = _VAL2FLD(I3C_CORE_BUS_FREE_AVAIL_TIMING_BUS_FREE_TIME, lcnt);

    }

    context->i3cSclRate = resDataRate;
    return resDataRate;

}


/*******************************************************************************
*  Function Name: Cy_I3C_I3CGetDataRate
****************************************************************************//**
*
* Provides the supported I3C bus data rate for I3C devices.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_data_speed_t
*
*******************************************************************************/
uint32_t Cy_I3C_GetDataRate(I3C_CORE_Type const *base, cy_stc_i3c_context_t *context) 
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    /* Suppress a compiler warning about unused variables */
    (void) base;

    return (context->i3cSclRate);
}


/*******************************************************************************
*  Function Name: Cy_I3C_IsBusBusy
****************************************************************************//**
*
* Indicates the whether the bus is busy or not.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* true: if bus is BUSY.
* false: if bus is IDLE.
*
*******************************************************************************/
bool Cy_I3C_IsBusBusy(I3C_CORE_Type const *base)
{
    CY_ASSERT_L1(NULL != base);
    
    uint32_t retStatus;
    
    retStatus = I3C_CORE_PRESENT_STATE(base) & I3C_CORE_PRESENT_STATE_CM_TFR_STS_Msk;

    if(0 == retStatus)
        return false;

    return true;
}


/*******************************************************************************
*  Function Name: Cy_I3C_IsMaster
****************************************************************************//**
*
* Indicates the whether the bus is busy or not.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* true: if controller is current bus master.
* false: otherwise.
*
*******************************************************************************/
bool  Cy_I3C_IsMaster(I3C_CORE_Type const *base)
{
    CY_ASSERT_L1(NULL != base);
    
    uint32_t value = _FLD2VAL(I3C_CORE_DEVICE_CTRL_EXTENDED_DEV_OPERATION_MODE, I3C_CORE_DEVICE_CTRL_EXTENDED(base));

    if(value)
        return false;

    else
        return true;
}


/*******************************************************************************
*  Function Name: Cy_I3C_GetMode
****************************************************************************//**
*
* Provides the mode of the device.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* \ref cy_en_i3c_mode_t
*
*******************************************************************************/
cy_en_i3c_mode_t Cy_I3C_GetMode(I3C_CORE_Type const *base)
{
    CY_ASSERT_L1(NULL != base);

    uint32_t mode;
    
    mode = I3C_CORE_DEVICE_CTRL_EXTENDED(base);
    mode = _FLD2VAL(I3C_CORE_DEVICE_CTRL_EXTENDED_DEV_OPERATION_MODE, mode);

    if(mode)
        return CY_I3C_SLAVE;

    else
        return CY_I3C_MASTER;
}


/*******************************************************************************
*  Function Name: Cy_I3C_MasterWrite
****************************************************************************//**
*
* Writes data provided by xferConfig structure \ref cy_stc_i3c_master_xfer_config_t
* to a specific device.
*
* \param base
* The pointer to the I3C instance.
* 
* \param xferConfig
* Master transfer configuration structure.
* \ref cy_stc_i3c_master_xfer_config_t
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterWrite(I3C_CORE_Type *base, cy_stc_i3c_master_xfer_config_t *xferConfig, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == xferConfig) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    CY_ASSERT_L1(CY_IS_I3C_BUFFER_VALID(xferConfig->buffer, xferConfig->bufferSize));
    CY_ASSERT_L2(CY_IS_I3C_ADDR_VALID(xferConfig->slaveAddress));

    if(CY_I3C_FIFO_SIZE < xferConfig->bufferSize)
        return CY_I3C_BAD_BUFFER_SIZE;

    cy_en_i3c_status_t retStatus = CY_I3C_MASTER_NOT_READY;
    
    if(0 != (CY_I3C_IDLE_MASK & context->state))
    {
        uint8_t validBytes, res;
        uint8_t *data;
        cy_stc_i3c_ccc_t cmd;
        uint8_t pos = 0;
        cy_stc_i3c_master_devlist_t *i3cDeviceList;
        uint8_t writeDSMode = CY_I3C_SDR0;

        context->masterBuffer = xferConfig->buffer;
        context->masterBufferSize = xferConfig->bufferSize;
        context->masterBufferIdx = 0;
        data = context->masterBuffer;
        context->masterStatus = CY_I3C_MASTER_BUSY;
        
        cmd.cmdHigh = 0UL;
        cmd.cmdLow = 0UL;

        //Clear the interrupts
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);

        //Reset the Tx FIFO
        I3C_CORE_RESET_CTRL(base) |= I3C_CORE_RESET_CTRL_TX_FIFO_RST_Msk;
        
        res = GetAaddrslotStatus(xferConfig->slaveAddress, context);

        //This helps to check if the device is actively present on the bus
        if((CY_I3C_ADDR_SLOT_I3C_DEV != res ) && (CY_I3C_ADDR_SLOT_I2C_DEV != res))
            return CY_I3C_BAD_PARAM; 

        if(CY_I3C_ADDR_SLOT_I2C_DEV == res)
        {
            //The target device is an i2cDevice
            pos = GetI2CDevAddrPos(base, xferConfig->slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            writeDSMode = (i3cDeviceList->i2cDevice.lvr & CY_I3C_LVR_I2C_MODE_INDICATOR) ? CY_I3C_FM_I2C: CY_I3C_FMP_I2C;
            context->masterStatus |= CY_I3C_MASTER_I2C_SDR_WR_XFER;
        }

        else
        {
            //The target is an i3cDevice
            pos = GetI3CDevAddrPos(base, xferConfig->slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            if(CY_I3C_CORE_BCR_MAX_DATA_SPEED_LIM_Msk == (i3cDeviceList->i3cDevice.bcr))
            {
                writeDSMode = (i3cDeviceList->i3cDevice.maxWriteDs) & 0x07;
            }
            context->masterStatus |= CY_I3C_MASTER_I3C_SDR_WR_XFER;
        }
        
        switch(xferConfig->bufferSize)
        {
            case 1: {
                      validBytes = CY_I3C_BYTE_STROBE1; 
                      cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, validBytes) |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0,*data);
                      context->masterBufferSize -= 1;
                      cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk |
                                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;
                      break; }
            case 2: {
                      validBytes = CY_I3C_BYTE_STROBE2;
                      cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, validBytes) |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0,*(data)) |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_1,*(data + 1));
                      context->masterBufferSize -= 2;
                      cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk |
                                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;
                      break; }
            case 3: {
                      validBytes = CY_I3C_BYTE_STROBE3;
                      cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, validBytes) |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0,*data) |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_1,*(data + 1)) |
                                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_2,*(data + 2));
                      context->masterBufferSize -= 3;
                      cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk |
                                      I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;
                      break; }
            default: {

                      //Write transfer with data greater than 3 bytes
                      MasterHandleDataTransmit(base, context);
                      if(xferConfig->toc)
                      {
                        cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;
                      }
                      
                      cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                                     _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, context->masterBufferSize);
                      break; }
        }

        cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                      _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TID, CY_I3C_MASTER_SDR_WRITE_TID) |
                      _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) |
                      _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED, writeDSMode) |
                      I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk;

        context->state = CY_I3C_MASTER_TX;
        //send the command
        Cy_I3C_SetInterruptMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
        Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
        
        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;

        retStatus = CY_I3C_SUCCESS;
    }
                  
    return retStatus;
}

/*******************************************************************************
*  Function Name: Cy_I3C_MasterRead
****************************************************************************//**
*
* Reads data from a device specified by xferConfig structure 
* \ref cy_stc_i3c_master_xfer_config_t.
*
* \param base
* The pointer to the I3C instance.
* 
* \param xferConfig
* Master transfer configuration structure.
* \ref cy_stc_i3c_master_xfer_config_t
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterRead (I3C_CORE_Type *base, cy_stc_i3c_master_xfer_config_t* xferConfig, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == xferConfig) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    CY_ASSERT_L1(CY_IS_I3C_BUFFER_VALID(xferConfig->buffer, xferConfig->bufferSize));
    CY_ASSERT_L2(CY_IS_I3C_ADDR_VALID(xferConfig->slaveAddress));

    if(CY_I3C_FIFO_SIZE < xferConfig->bufferSize)
        return CY_I3C_BAD_BUFFER_SIZE;

    cy_en_i3c_status_t retStatus = CY_I3C_MASTER_NOT_READY;

    if(0 != (CY_I3C_IDLE_MASK & context->state))
    {
        cy_stc_i3c_ccc_t cmd;
        uint8_t pos = 0;
        cy_stc_i3c_master_devlist_t *i3cDeviceList;
        uint8_t readDSMode = CY_I3C_SDR0;
        uint8_t res;
        
        context->masterBuffer = xferConfig->buffer;
        context->masterBufferSize = xferConfig->bufferSize;
        context->masterBufferIdx = 0;
        context->masterStatus = CY_I3C_MASTER_BUSY;

        cmd.cmdHigh = 0UL;
        cmd.cmdLow = 0UL;

        //Clear the interrupts
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
        
        //Reset the Rx FIFO
        I3C_CORE_RESET_CTRL(base) |= I3C_CORE_RESET_CTRL_RX_FIFO_RST_Msk;

        res = GetAaddrslotStatus(xferConfig->slaveAddress, context);

        //This helps to check if the device is actively present on the bus
        if((CY_I3C_ADDR_SLOT_I3C_DEV != res ) && (CY_I3C_ADDR_SLOT_I2C_DEV != res))
            return CY_I3C_BAD_PARAM; 

        if(CY_I3C_ADDR_SLOT_I2C_DEV == res)
        {
            //The target device is an i2cDevice
            pos = GetI2CDevAddrPos(base, xferConfig->slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            readDSMode = (i3cDeviceList->i2cDevice.lvr & CY_I3C_LVR_I2C_MODE_INDICATOR) ? CY_I3C_FM_I2C: CY_I3C_FMP_I2C;
            context->masterStatus |= CY_I3C_MASTER_I2C_SDR_RD_XFER;
        }
        else
        {
            //The target is an i3cDevice
            pos = GetI3CDevAddrPos(base, xferConfig->slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            if(CY_I3C_CORE_BCR_MAX_DATA_SPEED_LIM_Msk == (i3cDeviceList->i3cDevice.bcr))
            {
                readDSMode = (i3cDeviceList->i3cDevice.maxReadDs) & 0x07;
            }
            context->masterStatus |= CY_I3C_MASTER_I3C_SDR_RD_XFER;
        }

        if(xferConfig->toc)
        {
            //There are no bytes remaining to be read from the next READ, so terminate this READ with STOP
            cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;
        }
        cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                        _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, context->masterBufferSize);

        cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TID, CY_I3C_MASTER_SDR_READ_TID) |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED, readDSMode) |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) |
                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_RnW_Msk |
                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk;

        context->state = CY_I3C_MASTER_RX;

        Cy_I3C_SetInterruptMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
        Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
        
        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;


        return CY_I3C_SUCCESS;
    }
    return retStatus;
}

/*******************************************************************************
*  Function Name: Cy_I3C_MasterAbortTransfer
****************************************************************************//**
*
* Aborts an ongoing transfer.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_MasterAbortTransfer(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    
    if(CY_I3C_IDLE != context->state)
    {
        uint32_t intrState;
        I3C_CORE_DEVICE_CTRL(base) = I3C_CORE_DEVICE_CTRL_ABORT_Msk;

        intrState = Cy_SysLib_EnterCriticalSection();
        Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_TRANSFER_ABORT_STS);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_TRANSFER_ABORT_STS);
        Cy_SysLib_ExitCriticalSection(intrState);
    }
}

/*******************************************************************************
*  Function Name: Cy_I3C_MasterWriteByte
****************************************************************************//**
*
* Sends one byte to a slave.
* This function is blocking.
*
* \param base
* The pointer to the I3C instance.
* 
* \param slaveAddress
* The dynamic address of the target I3C slave device.
*
* \param data
* The byte to write to the slave.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterWriteByte(I3C_CORE_Type *base, uint8_t slaveAddress, int8_t data, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    CY_ASSERT_L2(CY_IS_I3C_ADDR_VALID(slaveAddress));
    
    cy_en_i3c_status_t retStatus = CY_I3C_MASTER_NOT_READY;
    
    if(0 != (CY_I3C_IDLE_MASK & context->state))
    {
        uint8_t res;
        cy_stc_i3c_ccc_t cmd;
        uint8_t pos = 0;
        cy_stc_i3c_master_devlist_t *i3cDeviceList;
        uint8_t writeDSMode = CY_I3C_SDR0;
        uint32_t respCmdPort;

        context->masterStatus = CY_I3C_MASTER_BUSY;
        
        cmd.cmdHigh = 0UL;
        cmd.cmdLow = 0UL;

        //Clear the interrupts
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);

        //Reset the Tx FIFO
        I3C_CORE_RESET_CTRL(base) |= I3C_CORE_RESET_CTRL_TX_FIFO_RST_Msk;
        
        res = GetAaddrslotStatus(slaveAddress, context);

        //This helps to check if the device is actively present on the bus
        if((CY_I3C_ADDR_SLOT_I3C_DEV != res ) && (CY_I3C_ADDR_SLOT_I2C_DEV != res))
            return CY_I3C_BAD_PARAM; 

        if(CY_I3C_ADDR_SLOT_I2C_DEV == res)
        {
            //The target device is an i2cDevice
            pos = GetI2CDevAddrPos(base, slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            writeDSMode = (i3cDeviceList->i2cDevice.lvr & CY_I3C_LVR_I2C_MODE_INDICATOR) ? CY_I3C_FM_I2C: CY_I3C_FMP_I2C;
            context->masterStatus |= CY_I3C_MASTER_I2C_SDR_WR_XFER;
        }

        else
        {
            //The target is an i3cDevice
            pos = GetI3CDevAddrPos(base, slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            if(CY_I3C_CORE_BCR_MAX_DATA_SPEED_LIM_Msk == (i3cDeviceList->i3cDevice.bcr))
            {
                writeDSMode = (i3cDeviceList->i3cDevice.maxWriteDs) & 0x07;
            }
            context->masterStatus |= CY_I3C_MASTER_I3C_SDR_WR_XFER;
        }

        cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, 1) |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0, data);

        cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                      _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) |
                      _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED, writeDSMode) |
                      I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk |
                      I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk |
                      I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;

        context->state = CY_I3C_MASTER_TX;

        // Disbale the interrupt signals by clearing the bits
        Cy_I3C_SetInterruptMask(base, 0UL);
        Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS));

        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;

        do
        {
            //wait till the interrupt of response is received
        }while(0 == ((CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS) & Cy_I3C_GetInterruptStatus(base)));

        respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
        Cy_I3C_SetInterruptStatusMask(base, 0);
    
        if(0 != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_Msk))
        {
            retStatus = ResponseError(respCmdPort);    //unsuccessful due to transfer error
            context->masterStatus |= CY_I3C_MASTER_HALT_STATE;
        }

        else
        {
            retStatus = CY_I3C_SUCCESS;
        }
        context->masterStatus &= (~CY_I3C_MASTER_BUSY);
        context->state = CY_I3C_IDLE;
    }

    return retStatus;
}

/*******************************************************************************
*  Function Name: Cy_I3C_MasterReadByte
****************************************************************************//**
*
* Reads one byte from a slave.
* This function is blocking.
*
* \param base
* The pointer to the I3C instance.
* 
* \param slaveAddress
* The dynamic address of the target I3C slave device.
*
* \param data
* The pointer to the location to store the Read byte.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterReadByte(I3C_CORE_Type *base, uint8_t slaveAddress, int8_t *data, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    CY_ASSERT_L2(CY_IS_I3C_ADDR_VALID(slaveAddress));
    
    cy_en_i3c_status_t retStatus = CY_I3C_MASTER_NOT_READY;
    
    if(0 != (CY_I3C_IDLE_MASK & context->state))
    {
        uint8_t res;
        cy_stc_i3c_ccc_t cmd;
        uint8_t pos = 0;
        cy_stc_i3c_master_devlist_t *i3cDeviceList;
        uint8_t readDSMode = CY_I3C_SDR0;
        uint32_t respCmdPort, datalen;

        context->masterStatus = CY_I3C_MASTER_BUSY;
        
        cmd.cmdHigh = 0UL;
        cmd.cmdLow = 0UL;

        //Clear the interrupts
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);

        //Reset the Tx FIFO
        I3C_CORE_RESET_CTRL(base) |= I3C_CORE_RESET_CTRL_TX_FIFO_RST_Msk;
        
        res = GetAaddrslotStatus(slaveAddress, context);

        //This helps to check if the device is actively present on the bus
        if((CY_I3C_ADDR_SLOT_I3C_DEV != res ) && (CY_I3C_ADDR_SLOT_I2C_DEV != res))
            return CY_I3C_BAD_PARAM; 

        if(CY_I3C_ADDR_SLOT_I2C_DEV == res)
        {
            //The target device is an i2cDevice
            pos = GetI2CDevAddrPos(base, slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            readDSMode = (i3cDeviceList->i2cDevice.lvr & CY_I3C_LVR_I2C_MODE_INDICATOR) ? CY_I3C_FM_I2C: CY_I3C_FMP_I2C;
            context->masterStatus |= CY_I3C_MASTER_I2C_SDR_RD_XFER;
        }

        else
        {
            //The target is an i3cDevice
            pos = GetI3CDevAddrPos(base, slaveAddress, context);
            i3cDeviceList = &(context->devList[pos]); //pointing to the device position in the local list of devices on the bus
            if(CY_I3C_CORE_BCR_MAX_DATA_SPEED_LIM_Msk == (i3cDeviceList->i3cDevice.bcr))
            {
                readDSMode = (i3cDeviceList->i3cDevice.maxReadDs) & 0x07;
            }
            context->masterStatus |= CY_I3C_MASTER_I3C_SDR_RD_XFER;
        }

        cmd.cmdHigh |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, 1);

        cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED, readDSMode) |
                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) |
                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_RnW_Msk |
                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk |
                       I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;

        context->state = CY_I3C_MASTER_RX;

        // Disbale the interrupt signals by clearing the bits
        Cy_I3C_SetInterruptMask(base, 0UL);
        Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS));

        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
        I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;

        do
        {
            //wait till the interrupt of response is received
        }while(0 == ((CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS) & Cy_I3C_GetInterruptStatus(base)));

        respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
        Cy_I3C_SetInterruptStatusMask(base, 0);
    
        if(0 != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_Msk))
        {
            retStatus = ResponseError(respCmdPort);    //unsuccessful due to transfer error
            context->masterStatus |= CY_I3C_MASTER_HALT_STATE;
        }

        else
        {
            datalen = (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk);
            if(1 == datalen)
            {
                *data = (uint8_t)Cy_I3C_ReadRxFIFO(base);
                retStatus = CY_I3C_SUCCESS;
            }
            else
            {
                retStatus = CY_I3C_MASTER_ERROR_M0;
            }
        }
        context->masterStatus &= (~CY_I3C_MASTER_BUSY);
        context->state = CY_I3C_IDLE;
    }

    return retStatus;
}

/*******************************************************************************
*  Function Name: Cy_I3C_GetBusStatus
****************************************************************************//**
*
* Returns the current I2C master status.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref group_i3c_macros_master_status.
* Note that not all I3C master statuses are returned by this function. Refer to
* more details of each status.
*
*******************************************************************************/
uint32_t Cy_I3C_GetBusStatus(I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    
    (void) base;
    #if 0
    uint32_t ret;
    ret = _FLD2VAL(I3C_CORE_PRESENT_STATE_CM_TFR_STS, I3C_CORE_PRESENT_STATE(base));
    //Gives the state of the transfer currently being executed by the controller

    return ret;
    #endif
    
    if(CY_I3C_MASTER == Cy_I3C_GetMode(base))
        return (context->masterStatus);

    else
        return(context->slaveStatus);
    
}

/* DAA */
/* About DAA:
1. This has the responsibility of finding the number of devices required to be assigned dynamic addresses
2. Getting the free addresses
3. Polpulating the DAT
4. Populating the COMMAND DATA PORT 
5. Poll for the response and update the DAT free pos index based on the response received */

/*******************************************************************************
*  Function Name: Cy_I3C_MasterStartEntDaa
****************************************************************************//**
*
* Issues ENTDAA CCC command to discover the i3c devices on the bus and assigns 
* valid dynamic addreses to the discovered devices.
* This CCC is to be issued also when a device hot joins the bus.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterStartEntDaa(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    cy_stc_i3c_device_t i3cDev;
    uint8_t i3cDevCount; //to store the number of devices to be assigned the dynamic address
    uint8_t dynamicAddress;
    uint8_t index, parity, pos;
    uint32_t respCmdPort;
    uint16_t respLeftDevCount;
    uint32_t value;
    cy_en_i3c_status_t retStatus;
    cy_stc_i3c_ccc_cmd_t cccCmd;
    cy_stc_i3c_ccc_payload_t payload;
    
    if(CY_I3C_MAX_DEVS <= (i3cMaster->devCount))  //cannot initialize the device as the bus already has 11 devices on it
        return CY_I3C_MASTER_MAX_DEVS_PRESENT; 
    
    i3cDevCount = CY_I3C_MAX_DEVS - (i3cMaster->devCount);
    
    //Get "free address" position
    pos = GetDATFreePos(base, context);

    //Get the 'i3cDevCount' number of free addresses and populate the DAT before issuing ENTDAA CCC
    for(index = pos; index < i3cDevCount; index++)
    {
        retStatus = Cy_I3C_MasterGetFreeDeviceAddress(base, &dynamicAddress, context);
        if(CY_I3C_MASTER_FREE_ADDR_UNAVIAL == retStatus)
            return retStatus;

        parity = even_parity(dynamicAddress);
        i3cMaster->lastAddress = dynamicAddress;
        dynamicAddress |= (parity << 7);

        //populate the device address table with the dynmaic address of the device
        value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, dynamicAddress);
    
        Cy_I3C_WriteIntoDeviceAddressTable(base, index, value);
    }

    context->masterStatus = CY_I3C_MASTER_BUSY | CY_I3C_MASTER_ENTDAA_XFER;
    
    // Disbale the interrupt signals by clearing the bits
    Cy_I3C_SetInterruptMask(base, 0UL);
    Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS));

    I3C_CORE_COMMAND_QUEUE_PORT(base) = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_ADDR_ASSGN_CMD | 
                                        _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_CMD, CY_I3C_CCC_ENTDAA) |
                                        _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_INDX, pos) |
                                        _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_COUNT, i3cDevCount) |
                                        I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_ROC_Msk | 
                                        I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_TOC_Msk;

    //Handling the response
    do
    {
        //wait till the interrupt of response is received
    }while(0 == (CY_I3C_INTR_MASK & Cy_I3C_GetInterruptStatus(base)));

    respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);
    Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
    
    respLeftDevCount = (uint16_t)(respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk); //represents the remaining device count

    if(0 != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_Msk))
    {
        if(4 != (_FLD2VAL(I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS, respCmdPort))) //TO DO: Use a macro instead of '4'
        {
            /* Error in transfer */
            retStatus = ResponseError(respCmdPort);
            context->masterStatus |= CY_I3C_MASTER_HALT_STATE;
            context->masterStatus &= (~CY_I3C_MASTER_BUSY);
            return retStatus;
        }
    }

    Cy_I3C_Resume(base, context);

    i3cMaster->dynAddrDevCount =  i3cDevCount - respLeftDevCount;  //Doubt - Will NACK be received here too?
    i3cMaster->devCount += i3cMaster->dynAddrDevCount;

    //Maintaining a local list of i3c devices
    for( index = 0; index < i3cMaster->dynAddrDevCount; index++)
    {
        Cy_I3C_ReadFromDevCharTable(base, index, &i3cDev);
        SetAddrslotStatus(i3cDev.dynamicAddress, CY_I3C_ADDR_SLOT_I3C_DEV, context);
        RetrieveI3CDeviceInfo(base, &i3cDev, false, context);
        i3cDev.staticAddress = 0;
        Cy_I3C_UpdateI3CDevInList(&i3cDev, pos, context);
        i3cMaster->freePos &= ~(CY_I3C_BIT(pos));
        pos++;
    }

    if(0 != i3cMaster->dynAddrDevCount)
    {
        //Send DEFSLVS and ENEC CCC commands
        cccCmd.address = CY_I3C_BROADCAST_ADDR;
        cccCmd.cmd = CY_I3C_CCC_DEFSLVS;
        cccCmd.data = &payload;
    
        defslvs_ccc(base, &cccCmd, context);
    }

    context->masterStatus &= (~CY_I3C_MASTER_BUSY);
    return CY_I3C_SUCCESS;

        
}

/*******************************************************************************
*  Function Name: Cy_I3C_MasterSendHdrCmds
****************************************************************************//**
*
* Writes data provided by xferConfig structure \ref cy_stc_i3c_master_xfer_config_t
* to a specific device.
*
* \param base
* The pointer to the I3C instance.
* 
* \param slaveAddress
* The dynamic address of the target I3C device.
*
* \param hdrCmd
* The pointer to HDR command description structure \ref cy_stc_i3c_hdr_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_MasterSendHdrCmds(I3C_CORE_Type *base, uint8_t slaveAddress, cy_stc_i3c_hdr_cmd_t *hdrCmd, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == hdrCmd) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    uint8_t pos, res;
    cy_en_i3c_status_t retStatus = CY_I3C_MASTER_NOT_READY;

    if(0 != (CY_I3C_IDLE_MASK & context->state))
    {
        res = GetAaddrslotStatus(slaveAddress, context);

        //This helps to check if the device is actively present on the bus    
        if((CY_I3C_ADDR_SLOT_I3C_DEV != res ) && (CY_I3C_ADDR_SLOT_I2C_DEV != res))
            return CY_I3C_BAD_PARAM; 
        
        pos = GetI3CDevAddrPos( base, slaveAddress, context);

        cy_stc_i3c_master_devlist_t *i3cDevice = &(context->devList[pos]);

        if(false == (i3cDevice->i3cDevice.hdrSupport))
        {
            // The device doesn't support HDR mode
            return CY_I3C_NOT_HDR_CAP;
        }

        if(hdrCmd->code & CY_I3C_HDR_IS_READ_CMD)
        {
            uint8_t numToRead;
            numToRead = (hdrCmd->ndatawords*2);
            //Read command
            context->hdrCmd = hdrCmd;
            context->masterBuffer = (uint8_t *)hdrCmd->data.in;
            context->masterBufferSize = numToRead;
            context->destDeviceAddr = slaveAddress;
            context->masterStatus = CY_I3C_MASTER_BUSY;

            MasterHDRRead(base, hdrCmd, context);
        }

        else
        {
            // WRITE command
            context->hdrCmd = hdrCmd;
            context->masterBuffer = (uint8_t *)hdrCmd->data.out;
            context->masterBufferSize = (hdrCmd->ndatawords * 2);
            context->destDeviceAddr = slaveAddress;
            context->masterStatus = CY_I3C_MASTER_BUSY;

            MasterHDRWrite(base, hdrCmd, context);
        }
        retStatus = CY_I3C_SUCCESS;
    }

    return retStatus;
}

/*******************************************************************************
*  Function Name: Cy_I3C_Interrupt
****************************************************************************//**
*
* This is an I3C interrupt handler helper function.
* This function must be called inside the user-defined interrupt service.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_Interrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    
    if (0UL == (I3C_CORE_DEVICE_CTRL_EXTENDED(base) & I3C_CORE_DEVICE_CTRL_EXTENDED_DEV_OPERATION_MODE_Msk))
    {
        /* Execute a transfer as the master */
        Cy_I3C_MasterInterrupt(base, context);
    }
    else
    {
        /* Execute a transfer as the slave */
        Cy_I3C_SlaveInterrupt(base, context);
    }
}


/*******************************************************************************
* Function Name: Cy_I3C_RegisterEventCallback
****************************************************************************//**
*
* Registers an event handler callback function of type cy_cb_i3c_handle_events_t
* which will be invoked by the PDL to indicate i3c events and results.
*
* \param base
* The pointer to the I3C instance.
*
* \param callback
* The pointer to a callback function.
* See \ref cy_cb_i3c_handle_events_t for the function prototype.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to the callback function.
*
*******************************************************************************/
void Cy_I3C_RegisterEventCallback(I3C_CORE_Type const *base, cy_cb_i3c_handle_events_t callback, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    
    /* Suppress a compiler warning about unused variable */
    (void) base;

    context->cbEvents = callback;
}

/*******************************************************************************
* Function Name: Cy_I3C_RegisterIbiCallback
****************************************************************************//**
*
* Registers an IBI handler callback function of type cy_cb_i3c_handle_ibi_t 
* which will be invoked when an IBI event is triggered on bus.
*
* \param base
* The pointer to the I3C instance.
*
* \param callback
* The pointer to a callback function.
* See \ref cy_cb_i3c_handle_events_t for the function prototype.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to the callback function.
*
*******************************************************************************/
void Cy_I3C_RegisterIbiCallback (I3C_CORE_Type const *base, cy_cb_i3c_handle_ibi_t callback, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    
    /* Suppress a compiler warning about unused variable */
    (void) base;

    context->cbIbi = callback;

}


/*******************************************************************************
*  Function Name: Cy_I3C_SlaveGetDynamicAddress
****************************************************************************//**
*
* Provides the master assigned dynamic address of the slave device.
*
* \param base
* The pointer to the I3C instance.
*
* \param address
* The pointer to the address.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_SlaveGetDynamicAddress(I3C_CORE_Type const *base, uint8_t *address, cy_stc_i3c_context_t const *context)
{
    if((NULL == base) || (NULL == address) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    if(_FLD2VAL(I3C_CORE_DEVICE_ADDR_DYNAMIC_ADDR_VALID, I3C_CORE_DEVICE_ADDR(base)))
    {
        *address = (uint8_t)_FLD2VAL(I3C_CORE_DEVICE_ADDR_DYNAMIC_ADDR, I3C_CORE_DEVICE_ADDR(base));
        return CY_I3C_SUCCESS;
    }

    return CY_I3C_ADDR_INVALID;
}

/*******************************************************************************
*  Function Name: Cy_I3C_SlaveGetMaxReadLength
****************************************************************************//**
*
* Provides the maximum data read lenght of the slave.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Max data read length.
*
*******************************************************************************/
uint32_t Cy_I3C_SlaveGetMaxReadLength(I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
        
    return (uint32_t)_FLD2VAL(I3C_CORE_SLV_MAX_LEN_MRL, I3C_CORE_SLV_MAX_LEN(base));
}


/*******************************************************************************
*  Function Name: Cy_I3C_SlaveGetMaxWriteLength
****************************************************************************//**
*
* Provides the maximum data write lenght of the slave.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Max write data length.
*
*******************************************************************************/
uint32_t Cy_I3C_SlaveGetMaxWriteLength(I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    
    return (uint32_t)_FLD2VAL(I3C_CORE_SLV_MAX_LEN_MWL, I3C_CORE_SLV_MAX_LEN(base));
}


/*******************************************************************************
*  Function Name: Cy_I3C_SlaveGenerateIbi
****************************************************************************//**
*
* Generates the specified IBI on the bus.
*
* \param base
* The pointer to the I3C instance.
*
* \param ibitype
* The pointer to the ibi structure \ref cy_stc_i3c_ibi_t containing the 
* type of ibi event to be generated.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_SlaveGenerateIbi(I3C_CORE_Type *base, cy_stc_i3c_ibi_t *ibitype, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == ibitype) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    uint8_t res;
    uint32_t value = 0;
    cy_en_i3c_status_t ret = CY_I3C_SUCCESS;
    
    //Generate a SIR
    if(CY_I3C_IBI_SIR == ibitype->event)
    {
        if(_FLD2VAL(I3C_CORE_SLV_EVENT_STATUS_SIR_EN, I3C_CORE_SLV_EVENT_STATUS(base)))
        {
            //When set, the controller attempts to issue the SIR on the bus. Once set, the applicatio cannot clear this bit
            //SIR_CTRL field of this reg is to be set to 0
            
            Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
            // Disbale the interrupt signals by clearing the bits
            Cy_I3C_SetInterruptMask(base, 0UL);
            Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_UPDATED_STS);
            I3C_CORE_SLV_INTR_REQ(base) = I3C_CORE_SLV_INTR_REQ_SIR_Msk;
        }
        else
        {
            //Return that Master has disabled SIR IBI
            return CY_I3C_SIR_DISABLED;
        }
    }

    //Generate a mastership request
    else if((CY_I3C_IBI_MASTER_REQ == ibitype->event) && (CY_I3C_SECONDARY_MASTER == context->i3cMode))
    {    
        //ASK - Should the DEVICE_ROLE be checked here for secondary master??
        if( _FLD2VAL(I3C_CORE_SLV_EVENT_STATUS_MR_EN, I3C_CORE_SLV_EVENT_STATUS(base)))
        {
            
            Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
            // Disbale the interrupt signals by clearing the bits
            Cy_I3C_SetInterruptMask(base, 0UL);
            Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_UPDATED_STS);
            //When set, the controller attempts to issue the MR on the bus. Once set, the application cannot clear this bit
            I3C_CORE_SLV_INTR_REQ(base) = I3C_CORE_SLV_INTR_REQ_MR_Msk;
        }
        else
        {
            //Return that Master has disabled MR IBI
            return CY_I3C_MR_DISABLED;
        }
    }

    else
    {
        /*
            When slaves initiates a hot-join ibi genearation event or 
            when a non-secondary master tries requesting for Mastership
        */
        
        return CY_I3C_BAD_EVENT_REQ;
    }
    
    //Handling the response
    do
    {
        //wait till the interrupt of response is received
    }while(0 == (CY_I3C_INTR_IBI_UPDATED_STS & Cy_I3C_GetInterruptStatus(base)));
    
    //Handle the response
    res = (uint8_t)_FLD2VAL(I3C_CORE_SLV_INTR_REQ_IBI_CPLT_STS, I3C_CORE_SLV_INTR_REQ(base));

    if(3 == res)
    {
        /* IBI not attempted, if
        *  1. Master has not assigned the dynamic address
        *  2. Master has cleared the assigned dynamic address
        *  3. Master has disabled the IBI (DISEC)
        *  4. The controller has switched to master mode */
        
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
        Cy_I3C_SetInterruptMask(base, CY_I3C_SLV_INTR_Msk);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_SLV_INTR_Msk);
        ret = CY_I3C_IBI_NOT_ATTEMPTED;
    }
    
    else
    {
        /* IBI is ACKed */
        if(CY_I3C_IBI_MASTER_REQ == ibitype->event)
        {
                /* IBI is acked, check if the mastership is delivered */
                Cy_SysLib_Delay(1000);
    
                value = I3C_CORE_DEVICE_CTRL_EXTENDED(base);

                Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);

                if(0x01 & value)
                {
                    ret = CY_I3C_MR_DISABLED;
                    Cy_I3C_SetInterruptMask(base, CY_I3C_SLV_INTR_Msk);
                    Cy_I3C_SetInterruptStatusMask(base, CY_I3C_SLV_INTR_Msk);
                }

                else
            {
                Cy_I3C_Resume(base, context);
                SecondaryMasterInit(base, true, context);
            }
        }
    }
    return ret;
}


/*******************************************************************************
* Function Name: Cy_I3C_SlaveConfigReadBuf
****************************************************************************//**
*
* Configures the buffer pointer and the read buffer size. This is the buffer
* from which the master reads data. After this function is called, data
* transfer from the read buffer to the master is handled by
* \ref Cy_I3C_Interrupt.
*
* When the Read transaction is completed \ref CY_I3C_SLAVE_WR_CMPLT is set.
* Also the \ref CY_I3C_SLAVE_WR_CMPLT_EVENT event is generated.
*
* \param base
* The pointer to the I3C instance.
*
* \param buffer
* The pointer to the buffer with data to be read by the master.
*
* \param size
* Size of the buffer.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_SlaveConfigReadBuf (I3C_CORE_Type const *base, uint8_t *buffer, uint32_t size, cy_stc_i3c_context_t *context)
{
     CY_ASSERT_L1(NULL != base);
     CY_ASSERT_L1(NULL != context);
     CY_ASSERT_L1(CY_IS_I3C_BUFFER_VALID(buffer, size));
    
    /* Suppress a compiler warning about unused variables */
    (void) base;

    context->slaveTxBuffer     = buffer;
    context->slaveTxBufferSize = size;
    context->slaveTxBufferIdx  = 0UL;
    context->slaveTxBufferCnt  = 0UL;
}


/*******************************************************************************
* Function Name: Cy_I3C_SlaveGetReadTransferCount
****************************************************************************//**
*
* Returns the number of bytes read by the master since the last time
* \ref Cy_I3C_SlaveConfigReadBuf was called.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* The number of bytes read by the master.
*
*******************************************************************************/
uint32_t Cy_I3C_SlaveGetReadTransferCount (I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context)
{
    CY_ASSERT_L1(NULL != base);
    
    /* Suppress a compiler warning about unused variables */
    (void)base;
    
    return (context->slaveTxBufferCnt);
}

//TBD - Check RD/WR macro from I2C
/*******************************************************************************
* Function Name: Cy_I3C_SlaveConfigWriteBuf
****************************************************************************//**
*
* Configures the buffer pointer and the write buffer size. This is the buffer
* that the master writes data to. After this function is called, data
* transfer from the master into the write buffer is handled by
* \ref Cy_I3C_Interrupt.
*
* When the write transaction is completed \ref CY_I3C_SLAVE_RD_CMPLT is set.
* Also the \ref CY_I3C_SLAVE_RD_CMPLT_EVENT event is generated.
*
* \param base
* The pointer to the I3C instance.
*
* \param buffer
* The pointer to buffer to store data written by the master.
*
* \param size
* Size of the buffer.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_SlaveConfigWriteBuf(I3C_CORE_Type const *base, uint8_t *buffer, uint32_t size, cy_stc_i3c_context_t *context)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != context);
    CY_ASSERT_L1(CY_IS_I3C_BUFFER_VALID(buffer, size));

    /* Suppress a compiler warning about unused variables */
    (void) base;

    context->slaveRxBuffer     = buffer;
    context->slaveRxBufferSize = size;
    context->slaveRxBufferIdx  = 0UL;
}


/*******************************************************************************
* Function Name: Cy_I3C_SlaveGetWriteTransferCount
****************************************************************************//**
*
* Returns the number of bytes written by the master since the last time
* \ref Cy_I3C_SlaveConfigWriteBuf was called.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* The number of bytes written by the master.
*
*******************************************************************************/
uint32_t Cy_I3C_SlaveGetWriteTransferCount (I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context)
{
    CY_ASSERT_L1(NULL != base);
    
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return (context->slaveRxBufferCnt);
}


/*******************************************************************************
* Function Name: Cy_I3C_DeliverMastership
****************************************************************************//**
* Delivers the bus mastership to the requesting secondary master.
*
* \param base
* The pointer to the I3C instance.
*
* \param SecMasterAddress
* The address of the I3C device to which the mastership has to be delivered.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \note
* This should be called by the user when a mastership request IBI is received 
* through cy_cb_i3c_handle_ibi_t callback and user wants to deliver master ship.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_DeliverMastership(I3C_CORE_Type *base, uint8_t SecMasterAddress, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }
    cy_stc_i3c_ccc_cmd_t cccCmd;
    cy_stc_i3c_ccc_getaccmst_t getaccmst;
    cy_stc_i3c_ccc_payload_t payload;
    uint8_t addr;
    cy_en_i3c_status_t errorStatus;
    
    addr = SecMasterAddress;

    cccCmd.address = addr; //Check this slave address
    cccCmd.cmd = CY_I3C_CCC_GETACCMST;
    cccCmd.data = &payload;
    cccCmd.data->data = &(getaccmst.newmaster);
    
    errorStatus = getaccmst_ccc(base, &cccCmd, context);

    if(CY_I3C_SUCCESS == errorStatus)
    {
        if(addr != (getaccmst.newmaster >> 1))
        {
            errorStatus = CY_I3C_ADDR_MISMATCH;
        }

        else
        {
            //Mastership is transferred and the controller is in the Slave Mode

            uint32_t value;

            value = _FLD2VAL(I3C_CORE_DEVICE_CTRL_EXTENDED_DEV_OPERATION_MODE,I3C_CORE_DEVICE_CTRL_EXTENDED(base));

            if(value)
            {
                //Mode is changed to slave
                cy_en_i3c_status_t  status;

                Cy_I3C_Resume(base, context);
                I3C_CORE_RESET_CTRL(base) |= (I3C_CORE_RESET_CTRL_IBI_QUEUE_RST_Msk | I3C_CORE_RESET_CTRL_SOFT_RST_Msk);
                status = SecondaryMasterInit(base, false, context);

                return status;
            }
            
        }
    }
    return errorStatus;
}


/*******************************************************************************
* Function Name: Cy_I3C_RequestMastership
****************************************************************************//**
*
* Requests mastership from the current master.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
cy_en_i3c_status_t Cy_I3C_RequestMastership(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    if((NULL == base) || (NULL == context))
    {
        return CY_I3C_BAD_PARAM;
    }

    if(CY_I3C_SECONDARY_MASTER != context->i3cMode)
    {
        return CY_I3C_NOT_SECONDARY_MASTER;
    }

    cy_en_i3c_status_t retStatus;
    cy_stc_i3c_ibi_t ibi;

    retStatus = Cy_I3C_SlaveGetDynamicAddress(base, &(ibi.slaveAddress), context);

    if(CY_I3C_SUCCESS == retStatus)
    {
        ibi.event = CY_I3C_IBI_MASTER_REQ;
        retStatus = Cy_I3C_SlaveGenerateIbi(base, &ibi, context);
    }

    return retStatus;
}

/*******************************************************************************
* Function Name: SecondaryMasterInit
****************************************************************************//**
*
* This function does the basic Master Mode configurations when the operation
* mode of the Secondary Master is changed from I3C Slave to I3C Master.
*
* \param base
* The pointer to the I3C instance.
*
* \isMaster
* true: Master mode initializations.
* false: Slave mode initializations.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
cy_en_i3c_status_t SecondaryMasterInit(I3C_CORE_Type *base, bool isMaster, cy_stc_i3c_context_t *context)
{
    if(isMaster)
    {
        cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
        uint8_t address, idx; 
        uint32_t value;
        uint32_t staticAddress, dynamicAddress, bcr, dcr, i2c;
        cy_stc_i3c_device_t i3cDevice;
        cy_stc_i2c_device_t i2cDevice;
        
        i3cMaster->freePos = 0x7FFUL;
        i3cMaster->lastAddress = 0;
        i3cMaster->devCount = 0;
        i3cMaster->i2cDeviceCount = 0;
        i3cMaster->dynAddrDevCount = 0;
    
            address = _FLD2VAL(I3C_CORE_DEVICE_ADDR_DYNAMIC_ADDR, I3C_CORE_DEVICE_ADDR(base));

        InitAddrslots(context);
        SetAddrslotStatus(address, CY_I3C_ADDR_SLOT_I3C_DEV, context);
        
        Cy_I3C_SetDataRate(base, context->i3cSclRate, context->i3cClockHz, context); 
    
        Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);

        for(idx = 0; idx < context->slaveRxBufferCnt; idx++)
        {
            dynamicAddress = I3C_CORE_DEV_CHAR_TABLE1_LOC1(base) & 0xFFUL;
            staticAddress = (I3C_CORE_DEV_CHAR_TABLE1_LOC1(base) & 0xFF000000UL) >> 24;
            dcr = (I3C_CORE_DEV_CHAR_TABLE1_LOC1(base) & 0xFF00UL) >> 8;
            bcr = (I3C_CORE_DEV_CHAR_TABLE1_LOC1(base) & 0xFF0000UL) >> 16;

            i2c = dynamicAddress? 0 : 1;

            value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, dynamicAddress) |
                    _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_STATIC_ADDR, staticAddress) |
                    _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC10_LEGACY_I2C_DEVICE, i2c);
            
            Cy_I3C_WriteIntoDeviceAddressTable(base, idx, value);

            if(i2c)
            {
                /* I2C Device */
                SetAddrslotStatus(staticAddress, CY_I3C_ADDR_SLOT_I2C_DEV, context);
                i2cDevice.lvr = dcr;
                i2cDevice.staticAddress = staticAddress;

                Cy_I3C_UpdateI2CDevInList(&i2cDevice, idx, context);
    
                (i3cMaster->freePos) = ~ (CY_I3C_BIT(idx));
                (i3cMaster->devCount)++;
                (i3cMaster->i2cDeviceCount)++;
            }
            else
            {
                /* I3C Device */
                SetAddrslotStatus(dynamicAddress, CY_I3C_ADDR_SLOT_I3C_DEV, context);

                i3cDevice.bcr = bcr;
                i3cDevice.dcr = dcr;
                i3cDevice.dynamicAddress = dynamicAddress;
                RetrieveI3CDeviceInfo(base, &i3cDevice, false, context);
                i3cDevice.staticAddress = 0;
                Cy_I3C_UpdateI3CDevInList(&i3cDevice, idx, context);

                i3cMaster->freePos &= ~(CY_I3C_BIT(idx));
                (i3cMaster->devCount)++;
            }
        }
    }

    else
    {
        I3C_CORE_SLV_EVENT_STATUS(base) &= (~I3C_CORE_SLV_EVENT_STATUS_HJ_EN_Msk);
        
        //Sets the number of entries in the Receive FIFO that trigger the interrupt to 1 word
        I3C_CORE_DATA_BUFFER_THLD_CTRL(base) &= _VAL2FLD(I3C_CORE_DATA_BUFFER_THLD_CTRL_RX_BUF_THLD, 5) |
                                                _VAL2FLD(I3C_CORE_DATA_BUFFER_STATUS_LEVEL_TX_BUF_EMPTY_LOC, 1);

        Cy_I3C_SetInterruptMask(base, CY_I3C_SLV_INTR_Msk | CY_I3C_INTR_DEFSLV_STS);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_SLV_INTR_Msk | CY_I3C_INTR_DEFSLV_STS);
    }
    
    return CY_I3C_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_I3C_SlaveInterrupt
****************************************************************************//**
*
* This is the interrupt function for the I3C configured in slave mode.
* This function should be called inside the user-defined interrupt service routine.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_SlaveInterrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    uint32_t intrCause;
    intrCause = Cy_I3C_GetInterruptStatus(base);

    if(0 != (CY_I3C_INTR_TX_BUFFER_THLD_STS & intrCause))
    {
        SlaveHandleDataTransmit(base, context);
    }

    if(0 != (CY_I3C_INTR_RX_BUFFER_THLD_STS & intrCause))//TBD - Add callbacks
    {
    //TBD - Check if the Rx buffer is configured or not
        SlaveHandleDataReceive(base, context);
    }

    if(0 != (CY_I3C_INTR_RESP_READY_STS & intrCause))
    {
        SlaveRespReadyStsHandle(base, context);
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_RESP_READY_STS);
    }

    if(0 != (CY_I3C_INTR_CCC_UPDATED_STS & intrCause))
    {
        if(_FLD2VAL(I3C_CORE_SLV_EVENT_STATUS_MRL_UPDATED, I3C_CORE_SLV_EVENT_STATUS(base)))
        {
            context->cbEvents(CY_I3C_SLAVE_MAX_RD_LEN_UPDT_EVENT);
            I3C_CORE_SLV_EVENT_STATUS(base) |= I3C_CORE_SLV_EVENT_STATUS_MRL_UPDATED_Msk;
        }
        else if(_FLD2VAL(I3C_CORE_SLV_EVENT_STATUS_MWL_UPDATED, I3C_CORE_SLV_EVENT_STATUS(base)))
        {
            context->cbEvents(CY_I3C_SLAVE_MAX_WR_LEN_UPDT_EVENT);
            I3C_CORE_SLV_EVENT_STATUS(base) |= I3C_CORE_SLV_EVENT_STATUS_MWL_UPDATED_Msk;
        }
        else
        {
            context->cbEvents(CY_I3C_SLAVE_CCC_REG_UPDATED_EVENT);
        }
        
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_CCC_UPDATED_STS);
    }

    if(0 != (CY_I3C_INTR_DYN_ADDR_ASSGN_STS & intrCause))
    {
        context->cbEvents(CY_I3C_SLAVE_ASSIGNED_DYN_ADDR_EVENT);
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_DYN_ADDR_ASSGN_STS);
    }

    /* Read request from the master */
    if(0 != (CY_I3C_INTR_READ_REQ_RECV_STS & intrCause))
    {
        if(NULL != (context->cbEvents))
        {
            //Callback - in case of error 
            
            uint32_t event;
            event = CY_I3C_SLAVE_NO_VALID_CMD_IN_CMDQ_EVENT;
            if(_FLD2VAL(I3C_CORE_CCC_DEVICE_STATUS_DATA_NOT_READY, I3C_CORE_CCC_DEVICE_STATUS(base)))
            {
                event |= CY_I3C_SLAVE_DATA_NOT_READY_EVENT;
            }
            context->cbEvents(event);
        }
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK); 
    }

    if(0 != (CY_I3C_INTR_BUSOWNER_UPDATED_STS & intrCause))
    {
        if(NULL != (context->cbEvents))
        {
            //Callback - in case of error 
            context->cbEvents(CY_I3C_CONTROLLER_ROLE_UPDATED_EVENT);
        }
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK); 
        return;
    }

    if(0 != (CY_I3C_INTR_DEFSLV_STS & intrCause))
    {
        uint32_t idx;
        idx = context->slaveRxBufferCnt;

        I3C_CORE_DEV_CHAR_TABLE_POINTER(base) |= _VAL2FLD(I3C_CORE_DEV_CHAR_TABLE_POINTER_PRESENT_DEV_CHAR_TABLE_INDX,idx);
        if(NULL != (context->cbEvents))
        {
            //Callback - in case of error 
            context->cbEvents(CY_I3C_DEFSLV_EVENT);
        }
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK); 
        return;
    }
}


/*******************************************************************************
* Function Name: Cy_I3C_MasterInterrupt
****************************************************************************//**
*
* This is the interrupt function for the I3C configured in master mode.
* This function should be called inside the user-defined interrupt service routine.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_I3C_MasterInterrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    uint32_t intrCause;
    intrCause = Cy_I3C_GetInterruptStatus(base);

    if(0 != (CY_I3C_INTR_RESP_READY_STS & intrCause))
    {
        if(0 != (CY_I3C_INTR_TRANSFER_ERR_STS & intrCause))
        {
            uint32_t respCmdPort, errorEvent;
            respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);
            errorEvent = ResponseErrorEvent(respCmdPort); 
            context->masterStatus |= CY_I3C_MASTER_HALT_STATE;
            context->masterStatus &= (~CY_I3C_MASTER_BUSY);
            context->state = CY_I3C_IDLE;
            
            if(NULL != (context->cbEvents))
            {
                //Callback - in case of error 
                context->cbEvents(errorEvent);
            }
            Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK); //Check if this clearing in not harmful
            Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
            Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
            return;
        }
        
        else
        {    
            MasterRespReadyStsHandle(base, context);
        }

        Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
        return;
    }

    if(0 != (CY_I3C_INTR_TRANSFER_ABORT_STS & intrCause))
    {
        //Test - Check: Does this set the TRANSFER_ERROR_STS and the error field too?
        
        context->masterStatus |= CY_I3C_MASTER_XFER_ABORTED | CY_I3C_MASTER_HALT_STATE;
        context->masterStatus &= (~CY_I3C_MASTER_BUSY);
        context->state = CY_I3C_IDLE;

        I3C_CORE_RESET_CTRL(base) = I3C_CORE_RESET_CTRL_SOFT_RST_Msk;

        if(NULL != context->cbEvents)
        {
            context->cbEvents(CY_I3C_XFER_ABORTED_ERROR_EVENT);
        }

        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
        Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
        Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
        
        return;
    }

    if(0 != (CY_I3C_INTR_IBI_BUFFER_THLD_STS & intrCause))
    {
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);

        MasterHandleIBIInterrupt(base, context);

        return;
    }
    
    if(0 != (CY_I3C_INTR_BUSOWNER_UPDATED_STS & intrCause))
    {
        if(NULL != (context->cbEvents))
        {
            //Callback - in case of error 
            context->cbEvents(CY_I3C_CONTROLLER_ROLE_UPDATED_EVENT);
        }
        Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK); 
        return;
    }

}

/*******************************************************************************
* Function Name: Cy_I3C_DeepSleepCallback
****************************************************************************//**
*
* This function handles transition of I3C controller into and out of 
* deep sleep mode.
*
* \param callbackParams
* The pointer to the callback parameters structure
* \ref cy_stc_syspm_callback_params_t.
*
* \param mode
* Callback mode, see \ref cy_en_syspm_callback_mode_t
*
* \return
* \ref cy_en_syspm_status_t
*
*******************************************************************************/
cy_en_syspm_status_t Cy_I3C_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode)
{
    //TBD
    return CY_SYSPM_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_I3C_HibernateCallback
****************************************************************************//**
*
* This function handles transition of I3C controller into hibernate mode.
*
* \param callbackParams
* The pointer to the callback parameters structure
* \ref cy_stc_syspm_callback_params_t.
*
* \param mode
* Callback mode, see \ref cy_en_syspm_callback_mode_t
*
* \return
* \ref cy_en_syspm_status_t
*
*******************************************************************************/
cy_en_syspm_status_t Cy_I3C_HibernateCallback(cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode)
{
    //TBD
    return CY_SYSPM_SUCCESS;
}

/*******************************************************************************
* Function Name: even_parity
****************************************************************************//**
*
* Finds the parity of the address.
*
* \param address
* 7-bit right justified address.
*
* \return
* The parity of the address.
* 0: odd number of one bits in the address
* 1: even number of one bits in the address.
*
*******************************************************************************/
static uint8_t even_parity(uint8_t address)
{
    address ^= address >> 4;
    address &= 0xF;

    return ((0x9669 >> address) & 1);
}

/*******************************************************************************
* Function Name: ffs
****************************************************************************//**
*
* Finds the position of the first set bit in the given value.
*
* \param address
* 7-bit right justified address.
*
* \return
* The position of the first set bit.
*
*******************************************************************************/
static uint32_t ffs(uint32_t value)
{
    uint32_t r = 1;

    if (!value)
        return 0;
    if (!(value & 0xffff)) {
        value >>= 16;
        r += 16;
    }
    if (!(value & 0xff)) {
        value >>= 8;
        r += 8;
    }
    if (!(value & 0xf)) {
        value >>= 4;
        r += 4;
    }
    if (!(value & 3)) {
        value >>= 2;
        r += 2;
    }
    if (!(value & 1)) {
        value >>= 1;
        r += 1;
    }
    return r;
}

/*******************************************************************************
*  Function Name: SetAddrslotStatus
****************************************************************************//**
*
* Marks the status of the address parameter as defined 
* by \ref cy_en_i3c_addr_slot_status_t.
*
* \param address
* 7-bit right justified address.
*
* \param status
* The status to be assigned to the address.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_config_t allocated
* by the user. The structure is used during the I2C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
* 
*******************************************************************************/
static void SetAddrslotStatus(uint8_t address, cy_en_i3c_addr_slot_status_t status, cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    uint8_t bitPos = address * 2;
    unsigned long *ptr;

    ptr = i3cMaster->addrslotsStatusArray + (bitPos / CY_I3C_BITS_PER_LONG);
    *ptr &= ~((unsigned long)CY_I3C_ADDR_SLOT_STATUS_MASK << (bitPos % CY_I3C_BITS_PER_LONG));
    *ptr |= (unsigned long)status << (bitPos % CY_I3C_BITS_PER_LONG);
}

/*******************************************************************************
*  Function Name: InitAddrslots
****************************************************************************//**
*
* Initializes the status \ref cy_en_i3c_addr_slot_status_t of the addresses.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_config_t allocated
* by the user. The structure is used during the I2C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void InitAddrslots(cy_stc_i3c_context_t *context)
{
    uint8_t index;

    //Set all the addresses as free initially
    for(index = 0; index < ((CY_I3C_MAX_ADDR + 1) * 2) / CY_I3C_BITS_PER_LONG; index++)
    {
        SetAddrslotStatus(index, CY_I3C_ADDR_SLOT_FREE, context);
    }

    //assigning reserved status for addresses from 1-7

    for(index = 0; index < 8; index++)
    {
        SetAddrslotStatus(index, CY_I3C_ADDR_SLOT_RSVD, context);
    }

    // assigning reserved status for the broadcast and other I3C reserved broadcast addresses

    SetAddrslotStatus(CY_I3C_BROADCAST_ADDR, CY_I3C_ADDR_SLOT_RSVD, context);

    for (index = 0; index < 7; index++)
    {
        SetAddrslotStatus(CY_I3C_BROADCAST_ADDR ^ CY_I3C_BIT(index), CY_I3C_ADDR_SLOT_RSVD, context);
    }
    
}

/*******************************************************************************
*  Function Name: DeInitAddrslots
****************************************************************************//**
*
* Sets the status of the addresses to free.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void DeInitAddrslots(cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    uint8_t index;
    // Sets status of all the addresses free
    for(index = 0; index < (((CY_I3C_MAX_ADDR + 1) * 2) / CY_I3C_BITS_PER_LONG); index++)
    {
            i3cMaster->addrslotsStatusArray[index] = 0UL;
    }
}

/*******************************************************************************
*  Function Name: GetI2CDevAddrPos
****************************************************************************//**
*
* Obtains the position of the I2C device with the specified static address
* in the DAT.
*
* \param base
* The pointer to the I3C instance.
*
* \param staticAddress
* The static address of the I2C device.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Position of the I2C device in the DAT.
* 
*******************************************************************************/
static uint32_t GetI2CDevAddrPos(I3C_CORE_Type *base, uint8_t staticAddress, cy_stc_i3c_context_t *context)
{
    uint8_t index;
    for(index = 0; index < CY_I3C_MAX_DEVS; index++)
    {
        if(staticAddress == Cy_I3C_ReadStaticAddrFromDAT(base, index))
            return index;
    }

    return CY_I3C_BAD_PARAM;
}

/*******************************************************************************
*  Function Name: GetI3CDevAddrPos
****************************************************************************//**
*
* Obtains the position of the I3C device with the specified dynamic address
* in the DAT.
*
* \param base
* The pointer to the I3C instance.
*
* \param dynamicAddress
* The dynamic address of the I3C device.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* Position of the I3C device in the DAT.
*
*******************************************************************************/
static uint32_t GetI3CDevAddrPos(I3C_CORE_Type *base, uint8_t dynamicAddress, cy_stc_i3c_context_t *context)
{
    uint8_t index;
    for(index = 0; index < CY_I3C_MAX_DEVS; index++)
    {
        uint32_t ret;
        ret = Cy_I3C_ReadDynAddrFromDAT(base, index);
        if(dynamicAddress == ret)
            return index;
    }

    return CY_I3C_BAD_PARAM;
}

/*******************************************************************************
*  Function Name: GetDATFreePos
****************************************************************************//**
*
* Obtains the position of free location in the DAT.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return 
* Position of the free location in the DAT.
*
*******************************************************************************/
static uint32_t GetDATFreePos(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    uint32_t ret;
    ret = ffs(context->i3cMaster.freePos) - 1;

    return ret;
}

/*******************************************************************************
*  Function Name: GetAaddrslotStatus
****************************************************************************//**
*
* Obtains the status of the address parameter.
*
* \param address
* 7-bit right justified address.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return 
* \ref cy_en_i3c_addr_slot_status_t.
*
*******************************************************************************/
static cy_en_i3c_addr_slot_status_t GetAaddrslotStatus(uint8_t address, cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    int status, bitPos = address * 2;

    if (address > CY_I3C_MAX_ADDR)
        return CY_I3C_ADDR_SLOT_RSVD;

    status = i3cMaster->addrslotsStatusArray[bitPos / CY_I3C_BITS_PER_LONG];
    status >>= bitPos % CY_I3C_BITS_PER_LONG;

    return (cy_en_i3c_addr_slot_status_t)(status & CY_I3C_ADDR_SLOT_STATUS_MASK);
}

/*******************************************************************************
*  Function Name: ResponseError
****************************************************************************//**
*
* Provides the error type recieved in the response status.
*
* \param respCmd.
* The value read from the response queue port register 
*
* \return 
* \ref cy_en_i3c_status_t.

*******************************************************************************/
static cy_en_i3c_status_t ResponseError(uint32_t respCmd)
{
    uint8_t error;
    error = _FLD2VAL(I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS, respCmd);
    int ret = 0U;//DOUBT - What should be the default return value?
    
    switch (error)
    {
        case 1: ret = CY_I3C_MASTER_CRC_ERROR; break;
        case 2: ret = CY_I3C_MASTER_PARITY_ERROR; break;
        case 3: ret = CY_I3C_MASTER_FRAME_ERROR; break;
        case 4: ret = CY_I3C_MASTER_BROADCAST_ADDR_NACK_ERROR; break;
        case 5: ret = CY_I3C_MASTER_ADDR_NACK_ERROR; break; //This is same as ERROR M2
        case 6: ret = CY_I3C_MASTER_BUFFER_OVERFLOW_ERROR; break;
        case 8: ret = CY_I3C_MASTER_XFER_ABORTED_ERROR; break;
        case 9: ret = CY_I3C_MASTER_I2C_SLV_WDATA_NACK_ERROR; break;
        default: /* Unknow Error */ break;
    }
    return (cy_en_i3c_status_t)ret;
}

/*******************************************************************************
*  Function Name: ResponseErrorEvent
****************************************************************************//**
*
* Provides error events to be passed by \ref cy_cb_i3c_handle_events_t callback
* based on the type of error type received in the response status.
*
* \param respCmd.
* The value read from the response queue port register.
*
* \return 
* \ref group_i3c_macros_callback_events.

******************************************************************************/
static uint32_t ResponseErrorEvent(uint32_t respCmd)
{
    uint8_t error;
    error = _FLD2VAL(I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS, respCmd);
    uint32_t ret = 0; //DOUBT - What should be the default return value?
    
    switch (error)
    {
        case 1: ret = CY_I3C_CRC_ERROR_EVENT; break;
        case 2: ret = CY_I3C_PARITY_ERROR_EVENT; break;
        case 3: ret = CY_I3C_FRAME_ERROR_EVENT; break;
        case 4: ret = CY_I3C_BROADCAST_ADDR_NACK_ERROR_EVENT; break;
        case 5: ret = CY_I3C_ADDR_NACK_ERROR_EVENT; break;
        case 6: ret = CY_I3C_BUFFER_OVERFLOW_ERROR_EVENT; break;
        case 8: ret = CY_I3C_XFER_ABORTED_ERROR_EVENT; break;
        case 9: ret = CY_I3C_I2C_SLV_WDATA_NACK_ERROR_EVENT; break;
        case 10: ret = CY_I3C_MASTER_EARLY_TERMINATION_EVENT; break;
        default: /* Unknown Error */ break;

    }
    return ret;
}

/*******************************************************************************
* Function Name: WriteArray
****************************************************************************//**
*
* Places an array of data in the transmit FIFO.
* This function does not block. It returns how many data elements were
* placed in the transmit FIFO.
*
* \param base
* The pointer to the I3C instance.
*
* \param buffer
* The pointer to data to place in the transmit FIFO.
*
* \param size
* The number of data elements to transmit.
*
*******************************************************************************/
static void WriteArray(I3C_CORE_Type *base, void *buffer, uint32_t size)
{
    uint32_t *buf = buffer;
    uint32_t index;

    /*Put data in TX FIFO */
    for(index = 0UL; index < (size/4); ++index)
    {
        Cy_I3C_WriteTxFIFO(base, *(buf + index));
    }

    if(size & 3)
    {
        uint8_t cnt = size & 3;
        uint8_t *ptr;
        uint32_t value = 0UL;
        
        ptr = (uint8_t *)(buf + index);
        
        for(index = 0; index < cnt; index++)
        {
            value |= (*ptr) << (index * 8);
            ptr++;
        }
        Cy_I3C_WriteTxFIFO(base, value);
    }
}


/*******************************************************************************
* Function Name: ReadArray
****************************************************************************//**
*
* Reads an array of data out of the receive FIFO.
* This function does not block.
*
* \param base
* The pointer to the I3C instance.
*
* \param buffer
* The pointer to location to place data read from receive FIFO.
*
* \param size
* The number of data elements to read from the receive FIFO.
*
*******************************************************************************/
static void ReadArray(I3C_CORE_Type *base, void *buffer, uint32_t size)
{
    uint32_t index;
    uint32_t *buf = buffer;
    
    for(index = 0UL; index < size/4; ++index)
    {
        *(buf + index) = Cy_I3C_ReadRxFIFO(base);
    }

    if(size & 3)
    {
        *(buf + index) = Cy_I3C_ReadRxFIFO(base);
    }
}

/*******************************************************************************
* Function Name: MasterHandleDataTransmit
****************************************************************************//**
*
* Loads TX FIFO with data provided by \ref Cy_I3C_MasterWrite.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void MasterHandleDataTransmit(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
        if (context->masterBufferSize > 1UL)
        {
            /* Get the number of bytes to copy into TX FIFO */
            uint32_t numToCopy = context->masterBufferSize;

            /* Write data into TX FIFO */
            WriteArray(base, context->masterBuffer, numToCopy);
        }
}

/*******************************************************************************
* Function Name: MasterHandleDataReceive
****************************************************************************//**
*
* Reads data from RX FIFO into the buffer provided by \ref Cy_I3C_MasterRead.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void MasterHandleDataReceive(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    uint32_t numToRead;
    numToRead = context->masterBufferSize;
    //Read from the RX FIFO
    ReadArray(base, context->masterBuffer, numToRead);
}


/*******************************************************************************
*  Function Name: RetrieveI3CDeviceInfo
****************************************************************************//**
*
* To retrieve device information which includes: max write len, max read len,
* max data speed, HDR capability, BCR, DCR and PID.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDevice
* The pointer to the I3C device description structure \ref cy_stc_i3c_device_t.
*
* \basicInfo
* true: obtains BCR, DCR and PID of the device
* false: otherwise
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static void RetrieveI3CDeviceInfo(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, bool basicInfo, cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_ccc_cmd_t cccCmd;
    cy_stc_i3c_ccc_payload_t payload;
    cccCmd.address = i3cDevice->dynamicAddress;
    cccCmd.data = &payload;
    
    if(true == basicInfo)
    {
        cy_stc_i3c_ccc_getbcr_t bcr;
        cccCmd.data->data = &bcr;
        cccCmd.cmd = CY_I3C_CCC_GETBCR;
        Cy_I3C_SendCCCCmd(base, &cccCmd, context);
        i3cDevice->bcr = bcr.bcr;

        cy_stc_i3c_ccc_getdcr_t dcr;
        cccCmd.data->data = &dcr;
        cccCmd.cmd = CY_I3C_CCC_GETDCR;
        Cy_I3C_SendCCCCmd(base, &cccCmd, context);
        i3cDevice->dcr = dcr.dcr;

        cy_stc_i3c_ccc_getpid_t pid;
        cccCmd.data->data = &pid;
        cccCmd.cmd = CY_I3C_CCC_GETPID;
        Cy_I3C_SendCCCCmd(base, &cccCmd, context);
        
        uint8_t *ptr;
        uint8_t cnt, shift;
        ptr = pid.pid;
        i3cDevice->provisonalID = 0;

        for(cnt = 0; cnt < sizeof(pid); cnt++)
        {    
            shift = (sizeof(pid) - cnt - 1) * 8;
            (i3cDevice->provisonalID) |= ((uint64_t)(*ptr)) << shift;
            ptr++;
        }
    }

    //Get max read len
    cy_stc_i3c_ccc_mrwl_t mrwl;
    cccCmd.data->data = &mrwl;
    cccCmd.cmd = CY_I3C_CCC_GETMRL;
    Cy_I3C_SendCCCCmd(base, &cccCmd, context);
    
    i3cDevice->mrl = mrwl.len;

    //Get max write len
    cccCmd.cmd = CY_I3C_CCC_GETMWL;
    Cy_I3C_SendCCCCmd(base, &cccCmd, context);
    i3cDevice->mwl = mrwl.len;

    if(0 != (CY_I3C_CORE_BCR_MAX_DATA_SPEED_LIM_Msk & (i3cDevice->bcr)))
    {
        cy_stc_i3c_ccc_getmxds_t mxds;
        i3cDevice->speedLimit = true;
        cccCmd.data->data = &mxds;
        cccCmd.cmd = CY_I3C_CCC_GETMXDS;
        Cy_I3C_SendCCCCmd(base, &cccCmd, context);
        i3cDevice->maxReadDs = mxds.maxrd;
        i3cDevice->maxWriteDs = mxds.maxwr;
        i3cDevice->maxReadTurnaround[0] = mxds.maxrdturn[0];
        i3cDevice->maxReadTurnaround[1] = mxds.maxrdturn[1];
        i3cDevice->maxReadTurnaround[2] = mxds.maxrdturn[2];
    }

    else
    {
        i3cDevice->speedLimit = false;
    }

    if(0!= (CY_I3C_CORE_BCR_HDR_CAP_Msk & (i3cDevice->bcr)))
    {
        cy_stc_i3c_ccc_gethdrcap_t hdrcap;
        i3cDevice->hdrSupport = true;
        cccCmd.data->data = &hdrcap;
        cccCmd.cmd = CY_I3C_CCC_GETHDRCAP;
        Cy_I3C_SendCCCCmd(base, &cccCmd, context);
        i3cDevice->HDRCap = hdrcap.modes;
    }

    else
    {
        i3cDevice->hdrSupport = false;
    }

}

/*******************************************************************************
*  Function Name: CCC_Set
****************************************************************************//**
*
* Prepares commands for scheduling CCC transfers where the CCCs are not required
* to retrieve data from the slave device..
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
* 
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void CCC_Set(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd,  cy_stc_i3c_context_t *context)
{
    uint8_t pos = 0;   //pos = 0 in case of broadcast command.
    uint8_t dataLen = cccCmd->data->len;
    cy_stc_i3c_ccc_t cmd;

    //Check if the cmd is a Direct or broadcast command;
    //If Direct command, get the address offset in the DAT
    if(cccCmd->address & CY_I3C_CCC_DIRECT)
        pos = GetI3CDevAddrPos(base, cccCmd->address, context);

      uint8_t validBytes;
      uint8_t *data;
      data = (uint8_t *)cccCmd->data->data;

      switch(dataLen)
       {
        case 0: { cmd.cmdHigh = 0;
                  cmd.cmdLow = 0;
                  break; }
        
        case 1: { 
                  validBytes = CY_I3C_BYTE_STROBE1; 
                  cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, validBytes) |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0,*data);

                  cmd.cmdLow = I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk;
                  break; }
        case 2: { 
                  validBytes = CY_I3C_BYTE_STROBE2;
                  cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, validBytes) |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0,*(data + 1)) |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_1,*(data));
            
                  cmd.cmdLow = I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk;

                  break; }
        case 3: { 
                  validBytes = CY_I3C_BYTE_STROBE3;
                  cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB, validBytes) |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0,*(data+2))|
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_1,*(data + 1)) |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_2,*data);

                  cmd.cmdLow = I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk;

                  break; }
        default: {
                  //Commands with greater than 3 bytes of payload    
                  //Commands without payload; Also broadcast commands
                  cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, dataLen);

                  cmd.cmdLow = 0UL;
                  break; }
    }

    cmd.cmdLow |= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                   I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CP_Msk | 
                   _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CMD, cccCmd->cmd) | 
                   _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) | 
                   I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk |
                   I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;
      
    // Disbale the interrupt signals by clearing the bits
    Cy_I3C_SetInterruptMask(base, 0UL);
    Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS));

    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;
}

/*******************************************************************************
*  Function Name: CCC_Get
****************************************************************************//**
*
* Prepares commands for scheduling CCC transfers where the CCCs should retrieve 
* data from the slave device.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param cmd
* The 64-bit command \ref cy_stc_i3c_ccc_t .
* 
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void CCC_Get(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint8_t pos = 0;
    cy_stc_i3c_ccc_t cmd;
    uint8_t dataLen = cccCmd->data->len;

    //Direct command, get the address offset in the DAT
    pos = GetI3CDevAddrPos(base, cccCmd->address, context);

    // DATA_LENGTH field indicates the expected number of bytes from the Slave.
    // DATA_LENGTH should be provided by the calling API and not the user
    
    cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                    _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, dataLen);
                            
    
    cmd.cmdLow = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                 I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CP_Msk |
                 _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CMD, cccCmd->cmd) |
                 _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) |
                 I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_RnW_Msk |
                 I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk |
                 I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;

    // Disbale the interrupt signals by clearing the bits
    Cy_I3C_SetInterruptMask(base, 0UL);
    Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS));

    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh ;
    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow ;
}

/*******************************************************************************
*  Function Name: enec_disec_ccc
****************************************************************************//**
*
* Enable/Disable Slave Events Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t enec_disec_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, false, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;
        
    /* Expects 1 byte of data */
    if(0 == cccCmd->data->len)
        return CY_I3C_BAD_PARAM;

    CCC_Set(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, NULL, context);

    return retStatus;
}

/*******************************************************************************
*  Function Name: rstdaa_ccc
****************************************************************************//**
*
* Reset Dynamic Address Assignment Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t rstdaa_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    uint32_t pos = 0;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, false, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    CCC_Set(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, NULL, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;
    
    //handling of unicast command
    if(CY_I3C_BROADCAST_ADDR != cccCmd->address)
    {
        pos = GetI3CDevAddrPos(base, cccCmd->address, context);

        Cy_I3C_WriteIntoDeviceAddressTable(base, pos, 0);
        
        context->i3cMaster.freePos |= CY_I3C_BIT(pos);
        context->i3cMaster.devCount--;

        SetAddrslotStatus(cccCmd->address, CY_I3C_ADDR_SLOT_FREE, context);
    }

    //handling of broadcast command
    else
    {
        for(pos = 0; pos < (context->i3cMaster.devCount); pos++)
        {
            Cy_I3C_WriteIntoDeviceAddressTable(base, pos, 0);

            context->i3cMaster.freePos |= CY_I3C_BIT(pos);
            context->i3cMaster.devCount--;
        }

        DeInitAddrslots(context);
        
    }
    
    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: setmrwl_ccc
****************************************************************************//**
*
* Set Maximum Read/Write Length Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t setmrwl_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint8_t pos, idx;
    cy_en_i3c_status_t retStatus;
    uint16_t *data;

    retStatus = CCCSlaveAddressValidation(cccCmd->address, false, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;
    
    /* Expects 2 byte of data */
    if(0 == cccCmd->data->len)
        return CY_I3C_BAD_PARAM;

    CCC_Set(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, NULL, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

    if( CY_I3C_CCC_SETMRL(false) == cccCmd->cmd) //Set maximum read length for the target device
    {
        pos = GetI3CDevAddrPos(base, cccCmd->address, context);
        data = (uint16_t *)(cccCmd->data->data);
        context->devList[pos].i3cDevice.mrl = *data;
    }

    else if(CY_I3C_CCC_SETMWL(false) == cccCmd->cmd) //Set maximum write length for the target device
    {
        pos = GetI3CDevAddrPos(base, cccCmd->address, context);
        data = (uint16_t *)(cccCmd->data->data);
        context->devList[pos].i3cDevice.mwl = *data;
    }

    else if(CY_I3C_CCC_SETMRL(true) == cccCmd->cmd) //Set maximum read length for all the slave I3C devices - broadcast command
    {
        cy_stc_i3c_master_devlist_t *devList = context->devList;
        for(idx = 0; idx < (Cy_I3C_GetI3CDeviceCount(base, context)); idx++)
        {
            if(!devList->i2c)
            {
                data = (uint16_t *)(cccCmd->data->data);
                devList->i3cDevice.mrl = *data;
            }
            devList++;
        }
    }

    else //Set maximum write length for all the slave I3C devices - broadcast command
    {
        cy_stc_i3c_master_devlist_t *devList = context->devList;
        for(idx = 0; idx < (Cy_I3C_GetI3CDeviceCount(base, context)); idx++)
        {
            if(!devList->i2c)
            {
                data = (uint16_t *)(cccCmd->data->data);
                devList->i3cDevice.mwl = *data;
            }
            devList++;
        }
    }
    
    context->masterStatus &= (~CY_I3C_MASTER_BUSY);
    return CY_I3C_SUCCESS;

}

/*******************************************************************************
*  Function Name: setda_ccc
****************************************************************************//**
*
* Prepares and sends commands into the command queue for SETDASA and SETNEWDA CCCs.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void setda_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint8_t pos;
    uint8_t *data;
    uint32_t value;
    data = (uint8_t *)cccCmd->data->data;

    value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, *data) |
            _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_STATIC_ADDR, cccCmd->address);
    
    pos = GetDATFreePos(base, context);
    Cy_I3C_WriteIntoDeviceAddressTable(base, pos, value);
    
    // Disbale the interrupt signals by clearing the bits
    Cy_I3C_SetInterruptMask(base, 0UL);
    Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS));

    I3C_CORE_COMMAND_QUEUE_PORT(base)= I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_ADDR_ASSGN_CMD | 
                                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_CMD, cccCmd->cmd) | 
                                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_INDX, pos) | 
                                       _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_COUNT, 1) |
                                       I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_ROC_Msk |
                                       I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_TOC_Msk;

}

/*******************************************************************************
*  Function Name: setdasa_ccc
****************************************************************************//**
*
* Set Dynamic Address from Static Address Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t setdasa_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint32_t pos ;
    cy_en_i3c_status_t retStatus;
    uint8_t dynAddr;
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    cy_stc_i3c_device_t i3cDevice;

    /* Expects 1 byte of data */
    if(0 == cccCmd->data->len)
        return CY_I3C_BAD_PARAM;

    context->masterStatus |= CY_I3C_MASTER_DIRECTED_CCC_WR_XFER;

    pos = GetDATFreePos(base, context);

    setda_ccc(base, cccCmd, context);

    //Handling the response
    retStatus = MasterHandleCCCResponse(base, NULL, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

    dynAddr = *(uint8_t *)(cccCmd->data->data);

    i3cDevice.dynamicAddress = dynAddr;
    i3cDevice.staticAddress = cccCmd->address;

    //Maintaining a local list of i3c devices
    i3cDevice.bcr = 0;
    SetAddrslotStatus(dynAddr, CY_I3C_ADDR_SLOT_I3C_DEV, context);
    RetrieveI3CDeviceInfo(base, &i3cDevice, true, context);
    Cy_I3C_UpdateI3CDevInList(&i3cDevice, pos, context);

    (i3cMaster->freePos) =    ~ (CY_I3C_BIT(pos));
    (i3cMaster->devCount)++;
    
    //Update the local list
    
    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: setnewda_ccc
****************************************************************************//**
*
* Set New Dynamic Address Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t setnewda_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint32_t pos, value;
    cy_en_i3c_status_t retStatus;
    uint8_t *data;
    uint8_t parity;
    
    /* Expects 1 byte of data */
    if(0 == cccCmd->data->len)
        return CY_I3C_BAD_PARAM;

    data = (uint8_t *)cccCmd->data->data;

    (*data) = (*data << 1);

    cccCmd->data->data = data;

    context->masterStatus |= CY_I3C_MASTER_DIRECTED_CCC_WR_XFER;
    
    CCC_Set(base, cccCmd, context);

    (*data) = (*data >> 1);

    //Handling the response
    retStatus = MasterHandleCCCResponse(base, NULL, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

    pos = GetI3CDevAddrPos(base, cccCmd->address, context);

    SetAddrslotStatus(*data, CY_I3C_ADDR_SLOT_I3C_DEV, context);

    context->devList[pos].i3cDevice.dynamicAddress = *data; //Updating the local list

    parity = even_parity(*data);

    (*data) |= (parity << 7);

    value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, *data);

    Cy_I3C_WriteIntoDeviceAddressTable(base, pos, value); //Updating the Device Address Table

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: entas_ccc
****************************************************************************//**
*
* Enter Activity State Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t entas_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus; //response queue port
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, false, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    CCC_Set(base, cccCmd, context);

    Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);

    Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
    Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);

    return retStatus;
}

/*******************************************************************************
*  Function Name: enthdr0_ccc
****************************************************************************//**
*
* Enter HDR0 Mode Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t enthdr0_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    if(CY_I3C_BROADCAST_ADDR != cccCmd->address)
        return CY_I3C_BAD_PARAM;

    cccCmd->data->len = 0;
    
    context->masterStatus |= CY_I3C_MASTER_BROADCAST_CCC_WR_XFER;
        
    CCC_Set(base, cccCmd, context);

    Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
    Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getmrwl_ccc
****************************************************************************//**
*
* Get Maximum Read/Write Length Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getmrwl_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    uint16_t data;
    cy_stc_i3c_ccc_mrwl_t *mrwl;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    cccCmd->data->len = sizeof(*mrwl); //Expects 2 bytes of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;
/*    if(sizeof(*mrwl) != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */

    //Read data from the FIFO to the data_field of the cmd structure
    data = Cy_I3C_ReadRxFIFO(base);
    data = CY_I3C_SWAP16(data); //MSB byte will be returned first and then LSB

    mrwl = (cy_stc_i3c_ccc_mrwl_t *)cccCmd->data->data;
    mrwl->len = data;

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getpid_ccc
****************************************************************************//**
*
* Get Provisional ID Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getpid_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint8_t index;
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    uint8_t data[6];
    cy_stc_i3c_ccc_getpid_t *getpid;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;
    
    cccCmd->data->len = sizeof(*getpid); //Expects 2 bytes of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

    /*if(sizeof(*getpid) != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */
    
    //Read data from the FIFO to the data_field of the cmd structure 
    ReadArray(base, data, sizeof(*getpid));
    getpid = (cy_stc_i3c_ccc_getpid_t *)cccCmd->data->data;

    for(index = 0; index< sizeof(*getpid); index++)
    {
        getpid->pid[5 - index] = data[index];//MSB byte will be received first
    }

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getbcr_ccc
****************************************************************************//**
*
* Get Bus Characteristic Register Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getbcr_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    uint8_t data;
    cy_stc_i3c_ccc_getbcr_t *getbcr;

    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;
    
    cccCmd->data->len = sizeof(*getbcr); //Expects 1 byte of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

/*    if(sizeof(*getbcr) != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */
        

    //Read data from the FIFO to the data_field of the cmd structure
    data = Cy_I3C_ReadRxFIFO(base);

    getbcr = (cy_stc_i3c_ccc_getbcr_t *)cccCmd->data->data;
    getbcr->bcr = data;

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getdcr_ccc
****************************************************************************//**
*
* Get Bus Characteristic Register Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getdcr_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    uint8_t data;
    cy_stc_i3c_ccc_getdcr_t *getdcr;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;
    
    cccCmd->data->len = sizeof(*getdcr); //Expects 1 byte of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

    /*if(sizeof(*getdcr) != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */
        
    //Read data from the FIFO to the data_field of the cmd structure
    data = Cy_I3C_ReadRxFIFO(base);

    getdcr = (cy_stc_i3c_ccc_getdcr_t *)cccCmd->data->data;
    getdcr->dcr = data;

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getstatus_ccc
****************************************************************************//**
*
* Get Devie Status Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getstatus_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    uint16_t data;
    cy_stc_i3c_ccc_getstatus_t *getStatus;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    cccCmd->data->len = sizeof(*getStatus); //Expects 2 bytes of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

/*    if(sizeof(*getStatus) != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */

    //Read data from the FIFO to the data_field of the cmd structure
    data = Cy_I3C_ReadRxFIFO(base);
    data = CY_I3C_SWAP16(data);

    getStatus = (cy_stc_i3c_ccc_getstatus_t *)cccCmd->data->data;
    getStatus->status = data;

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getmxds_ccc
****************************************************************************//**  
*
* Get Max Data Speed Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getmxds_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{

    uint8_t idx;
    uint16_t data;
    uint8_t dataMTR[5];
    uint32_t recvDataLen;
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    cy_stc_i3c_ccc_getmxds_t *getmxds;

    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    cccCmd->data->len = sizeof(*getmxds); //Expects 2 or 5 bytes of data

    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;

    recvDataLen = respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk;
    if(!((2 & recvDataLen) || (5 & recvDataLen)))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC

    //Read data from the FIFO to the data_field of the cmd structure

    if(2 == recvDataLen)
    {
        data = Cy_I3C_ReadRxFIFO(base);
        getmxds = (cy_stc_i3c_ccc_getmxds_t *)cccCmd->data->data;

        getmxds->maxwr = (uint8_t)data;
        getmxds->maxrd = (uint8_t)(data >> 8);
    }

    //Read data from the FIFO to the data_field of the cmd structure 

    else if(5 == recvDataLen)
    {
        ReadArray(base, dataMTR, sizeof(*getmxds));
        getmxds = (cy_stc_i3c_ccc_getmxds_t *)cccCmd->data->data;
        getmxds->maxwr = dataMTR[0];
        getmxds->maxrd = dataMTR[1];

        for( idx = 2; idx < sizeof(*getmxds); idx++)
        {
            getmxds->maxrdturn[idx - 2] = dataMTR[idx]; //CHECK - if MSB of maxrdturn is copied into first element of struct array while checking 
        }
    }
    return CY_I3C_SUCCESS;

}

/*******************************************************************************
*  Function Name: gethdrcap_ccc
****************************************************************************//**  
*
* Get HDR Capability Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t gethdrcap_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    uint32_t respCmdPort = 0;
    uint8_t data;
    cy_stc_i3c_ccc_gethdrcap_t *gethdrcap;
    
    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    cccCmd->data->len = sizeof(*gethdrcap); //Expects 1 byte of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;


/*    if(sizeof(*gethdrcap) != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk))
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */

    //Read data from the FIFO to the data_field of the cmd structure
    data = Cy_I3C_ReadRxFIFO(base);

    gethdrcap = (cy_stc_i3c_ccc_gethdrcap_t *)cccCmd->data->data;
    gethdrcap->modes = data;

    return CY_I3C_SUCCESS;
}

/*******************************************************************************
*  Function Name: getaccmst_ccc
****************************************************************************//**  
*
* Get Accept Mastership Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t getaccmst_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    cy_en_i3c_status_t retStatus;
    //uint32_t recvDataLen;
    uint32_t respCmdPort = 0;
    uint8_t data;
    cy_stc_i3c_ccc_getaccmst_t *getaccmst;

    retStatus = CCCSlaveAddressValidation(cccCmd->address, true, context);

    if(retStatus != CY_I3C_SUCCESS)
        return retStatus;

    cccCmd->data->len = sizeof(getaccmst); //Excepts 1 byte of data
    
    CCC_Get(base, cccCmd, context);

    retStatus = MasterHandleCCCResponse(base, &respCmdPort, context);

    if(CY_I3C_SUCCESS != retStatus)
        return retStatus;


/*    recvDataLen = respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk;
    if(sizeof(*getaccmst) & recvDataLen)
        return CY_I3C_MASTER_ERROR_M0; //ERROR_M0 - Illegally formatted CCC */

    data = Cy_I3C_ReadRxFIFO(base);

    getaccmst = (cy_stc_i3c_ccc_getaccmst_t *)cccCmd->data->data;
    getaccmst->newmaster = data;

    return CY_I3C_SUCCESS;
    
}


/*******************************************************************************
*  Function Name: defslvs_ccc
****************************************************************************//**
*
* Define List of Slaves Command.
*
* \param base
* The pointer to the I3C instance.
* 
* \param cccCmd
* The pointer to the I3C CCC structure \ref cy_stc_i3c_ccc_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t defslvs_ccc(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context)
{
    uint8_t index;
    cy_stc_i3c_master_t *i3cMaster = &(context->i3cMaster);
    cy_stc_i3c_master_devlist_t *devList = context->devList;
    uint8_t i3cDevCnt;
    bool send = false;
    cy_stc_i3c_ccc_defslvs_t defslvs; //Stores the list to be sent - the slaves list
    cy_stc_i3c_ccc_dev_desc_t slaves[CY_I3C_MAX_DEVS];
    
    //Where to get the master related info - like bcr, dcr and    pid?!
    // The data to this CCC is prepared here

    //Check for the presence of any secondary masters on the bus
    i3cDevCnt = i3cMaster->devCount - i3cMaster->i2cDeviceCount;

    for(index = 0; index < (i3cDevCnt);)
    {
        if(!devList->i2c)
        {
            //it is an i3c device
            if((devList->i3cDevice.bcr & CY_I3C_CORE_BCR_DEVICE_ROLE_Msk) == CY_I3C_CORE_BCR_I3C_MASTER)
            {
                //The device is secondary master capable
                //Secondary Master will also have MASTER value in this field
                send = true;
            }
            index++;
        }
        devList++;
    }
    
    if(!send)
    {
        return CY_I3C_NO_SECONDARY_MASTER_DEVICES;
    }

    context->masterStatus |= CY_I3C_MASTER_BROADCAST_CCC_WR_XFER;

    //Maintain the list of devices
    devList = context->devList;

    for(index = 0; index < (i3cMaster->devCount); index++)
    {
        if(devList->i2c)
        {
            //i2c device
            slaves[index].staticAddress = (devList->i2cDevice.staticAddress) << 1;
            slaves[index].dynAddress = 0;
            slaves[index].lvr = devList->i2cDevice.lvr;
            slaves[index].bcr = 0;
        }

        else
        {
            //i3c device
            slaves[index].staticAddress = (devList->i3cDevice.staticAddress) << 1;
            slaves[index].dynAddress = (devList->i3cDevice.dynamicAddress) << 1;
            slaves[index].dcr = devList->i3cDevice.dcr;
            slaves[index].bcr = devList->i3cDevice.bcr;
        }
    }

    //DOUBT - Where to get the master info from?
    //defslvs.master = 
    defslvs.count = i3cMaster->devCount; //DOUBT - Should this count include the main master too?
    defslvs.slaves = slaves;

    cccCmd->data->data = &(defslvs);
    /* The second term here is the number of bytes holding the I3C Slaves' information */
    cccCmd->data->len = sizeof(defslvs) + ((sizeof(slaves)/CY_I3C_MAX_DEVS)*(index-1)) - sizeof(cy_stc_i3c_ccc_dev_desc_t);

    //Put the data into the TX fifo
    WriteArray(base, &defslvs, cccCmd->data->len);
    
    CCC_Set(base, cccCmd, context);
    
    //Should the response be handled here?
    return CY_I3C_SUCCESS;
    
}

/*******************************************************************************
*  Function Name: MasterHDRWrite
****************************************************************************//**
*
* Writes data provided in the HDR command structure \ref cy_stc_i3c_hdr_cmd_t
* to a specific device in HDR Mode.
*
* \param base
* The pointer to the I3C instance.
* 
* \param hdrCmd
* The pointer to the I3C HDR command structure \ref cy_stc_i3c_hdr_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t MasterHDRWrite(I3C_CORE_Type *base, cy_stc_i3c_hdr_cmd_t *hdrCmd, cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_ccc_t cmd;
    uint32_t pos;

    //Clear the interrupts
    Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
    
    //Reset the Tx FIFO
    I3C_CORE_RESET_CTRL(base) |= I3C_CORE_RESET_CTRL_TX_FIFO_RST_Msk;
    
    pos = GetI3CDevAddrPos(base, context->destDeviceAddr, context);

    //Prepare the data for the transfer
    MasterHandleDataTransmit(base, context);
    
    cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                   _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, (context->masterBufferSize));

                   
    cmd.cmdLow = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CP_Msk | 
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TID, CY_I3C_MASTER_HDR_WRITE_TID) |
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CMD, hdrCmd->code) | 
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) | 
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED, CY_I3C_HDR_DDR) |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;

    context->masterStatus |= CY_I3C_MASTER_HDR_DDR_WR_XFER;
    context->state = CY_I3C_MASTER_TX;
    
    Cy_I3C_SetInterruptMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
    Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
    
    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;
    
    return CY_I3C_SUCCESS;

}

/*******************************************************************************
*  Function Name: MasterHDRWrite
****************************************************************************//**
*
* Reads data from a device specified by HDR command structure 
* \ref cy_stc_i3c_hdr_cmd_t.
*
* \param base
* The pointer to the I3C instance.
* 
* \param hdrCmd
* The pointer to the I3C HDR command structure \ref cy_stc_i3c_hdr_cmd_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t MasterHDRRead(I3C_CORE_Type *base, cy_stc_i3c_hdr_cmd_t *hdrCmd, cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_ccc_t cmd;
    uint8_t pos;

    pos = GetI3CDevAddrPos(base, context->destDeviceAddr, context);
    
    cmd.cmdHigh = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG |
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH, (context->masterBufferSize));

    cmd.cmdLow = I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CP_Msk | 
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TID, CY_I3C_MASTER_HDR_READ_TID) |
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CMD, hdrCmd->code) | 
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX, pos) | 
                  _VAL2FLD(I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED, CY_I3C_HDR_DDR) |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_RnW_Msk |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk |
                  I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk;

    context->masterStatus |= CY_I3C_MASTER_HDR_DDR_RD_XFER;
    context->state = CY_I3C_MASTER_RX;

    Cy_I3C_SetInterruptMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
    Cy_I3C_SetInterruptStatusMask(base, (CY_I3C_INTR_TRANSFER_ERR_STS | CY_I3C_INTR_RESP_READY_STS));
    
    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdHigh;
    I3C_CORE_COMMAND_QUEUE_PORT(base) = cmd.cmdLow;

    return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: MasterRespReadyStsHandle
****************************************************************************//**
*
* Handles the response queue ready status interrupt \ref group_i3c_intr_macros
* for master mode of operation.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void MasterRespReadyStsHandle(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    uint32_t respCmdPort;
    uint32_t tid;


    respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);
    tid = _FLD2VAL(I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_TID, respCmdPort);

        
    /* Handling the receive response for Master SDR WRITE transfer */
    /* --------------------SDR/HDR WRITE TRANSFER----------------*/
    if((CY_I3C_MASTER_SDR_WRITE_TID == tid) || (CY_I3C_MASTER_HDR_WRITE_TID == tid))
    {
        MasterHandleWriteInterrupt(base, respCmdPort, context);
    }

    /* Handling the receive response for Master SDR READ transfer */
    /* --------------------SDR/HDR READ TRANSFER----------------*/

    else if((CY_I3C_MASTER_SDR_READ_TID == tid) || (CY_I3C_MASTER_HDR_READ_TID == tid))
    {
        MasterHandleReadInterrupt(base, respCmdPort, context);
    }
        
    else
    {
        //Unkonwn TID 
        //Do Nothing
    }
}


/*******************************************************************************
*  Function Name: MasterHandleWriteInterrupt
****************************************************************************//**
*
* Handles the response queue ready status interrupt  \ref group_i3c_intr_macros 
* with HDR/SDR write TID \ref cy_en_i3c_tid_t in the response status.
*
* \param base
* The pointer to the I3C instance.
* 
* \param respCmdPort
* The value read from the response queue port register.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static void MasterHandleWriteInterrupt(I3C_CORE_Type *base, uint32_t respCmdPort, cy_stc_i3c_context_t *context)
{
    //Expected number of bytes were not transmitted, WRITE terminated early
    context->masterStatus &= (~CY_I3C_MASTER_BUSY);
    context->state = CY_I3C_IDLE;

    uint32_t dataLen;

    dataLen = (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk);

    if(NULL != context->cbEvents)
    {
        (dataLen == 0) ? context->cbEvents(CY_I3C_MASTER_WR_CMPLT_EVENT): context->cbEvents(CY_I3C_MASTER_WR_EARLY_TERMINATION_EVENT);
    }
}

/*******************************************************************************
*  Function Name: MasterHandleReadInterrupt
****************************************************************************//**
*
* Handles the response queue ready status interrupt \ref group_i3c_intr_macros
* with HDR/SDR read TID \ref cy_en_i3c_tid_t in the response status.
*
* \param base
* The pointer to the I3C instance.
* 
* \param respCmdPort
* The value read from the response queue port register.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static void MasterHandleReadInterrupt(I3C_CORE_Type *base, uint32_t respCmdPort, cy_stc_i3c_context_t *context)
{
    if(respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk)
    {
        MasterHandleDataReceive(base, context);
        context->state = CY_I3C_IDLE;
        context->masterStatus &= (~CY_I3C_MASTER_BUSY);
            
        if(NULL != context->cbEvents)
        {
            context->cbEvents(CY_I3C_MASTER_RD_CMPLT_EVENT);
        }
    }
}

/*******************************************************************************
*  Function Name: MasterHandleIBIInterrupt
****************************************************************************//**
*
* Handles the IBI interrupts \ref group_i3c_intr_macros.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void MasterHandleIBIInterrupt(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
        uint32_t ibiData;
        uint8_t ibiType, ibiStatus;
        cy_stc_i3c_ibi_t ibiCallback;
        
        ibiData = I3C_CORE_IBI_QUEUE_STATUS(base);

        ibiType = _FLD2VAL(I3C_CORE_IBI_QUEUE_STATUS_IBI_ID, ibiData);
        ibiStatus = _FLD2VAL(I3C_CORE_IBI_QUEUE_STATUS_IBI_RESP_STS, ibiData);

        /* Hot-join */
        if(CY_I3C_IBI_TYPE_HOT_JOIN(ibiType))
        {
            ibiCallback.event = CY_I3C_IBI_HOTJOIN;
            ibiCallback.slaveAddress = 0;
            ibiCallback.status = (0 == ibiStatus) ? (uint32_t)CY_I3C_MASTER_HOTJOIN_IBI_ACK: (uint32_t)CY_I3C_MASTER_IBI_NACK;
            
            if(NULL != context->cbIbi)
            {
                context->cbIbi(&ibiCallback);
            }
        }

        /* Mastership request */
        else if(CY_I3C_IBI_TYPE_MASTERSHIP_REQUEST(ibiType))
        {
            ibiCallback.event = CY_I3C_IBI_MASTER_REQ;
            ibiCallback.slaveAddress = (uint8_t)(_FLD2VAL(I3C_CORE_IBI_QUEUE_STATUS_IBI_ID, ibiData) & 0x7FUL); //Check the msb/lsb alignment
            ibiCallback.status = (0 == ibiStatus) ? (uint32_t)CY_I3C_MASTER_MR_IBI_ACK : (uint32_t)CY_I3C_MASTER_IBI_NACK;
            
            if(NULL != context->cbIbi)
            {
                context->cbIbi(&ibiCallback);
            }
        }

        /* Slave Interrupt Request */
        else if(CY_I3C_IBI_TYPE_SIR_REQUEST(ibiType))
        {
            ibiCallback.event = CY_I3C_IBI_SIR;
            ibiCallback.slaveAddress = (uint8_t)(_FLD2VAL(I3C_CORE_IBI_QUEUE_STATUS_IBI_ID, ibiData) & 0x7FUL);
            ibiCallback.status = (0 == ibiStatus) ? (uint32_t)CY_I3C_MASTER_SIR_IBI_ACK : (uint32_t)CY_I3C_MASTER_IBI_NACK;
            
            if(NULL != context->cbIbi)
            {
                context->cbIbi(&ibiCallback);
            }
        }
}


/*******************************************************************************
*  Function Name: RearrangeAddrTable
****************************************************************************//**
*
* Rearranges the DAT and local list of devices /ref cy_stc_i3c_master_devlist_t
* when a device detaches from the bus and thus maintaining continuous free 
* locations on the top of the DAT and local list of devices.
*
* \param base
* The pointer to the I3C instance.
* 
* \param devIndex
* The position of the device being detached.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t RearrangeAddrTable(I3C_CORE_Type *base, uint32_t devIndex,  cy_stc_i3c_context_t *context)
{
    cy_stc_i3c_device_t i3cDeviceTop;
    cy_stc_i2c_device_t i2cDeviceTop;
    uint32_t topDevPos, value;

    
    topDevPos = (context->i3cMaster.devCount) - 1;
    //Case where the device being detached is not the top device on the list
        if(devIndex != topDevPos)
        {
        /* 
            1. Check if the top device is an i2c device or i3c device
            2. Update the DAT and the local list
            3. Also check for the case if the device to be detached is the top most device? Then do not perform the exchange
        */
            if(context->devList[topDevPos].i2c)
            {
                //Top device is an i2c device
    
                //Moving the top device to the detached device position
                i2cDeviceTop = context->devList[topDevPos].i2cDevice;
                context->devList[devIndex].i2cDevice=i2cDeviceTop;
                context->devList[devIndex].i2c = true;
    
                //Erase the top position entries and mark the position free
                context->i3cMaster.freePos |= CY_I3C_BIT(topDevPos);
    
                value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_STATIC_ADDR, i2cDeviceTop.staticAddress) |
                        I3C_CORE_DEV_ADDR_TABLE_LOC1_LEGACY_I2C_DEVICE_Msk;
    
                Cy_I3C_WriteIntoDeviceAddressTable(base, devIndex, value);
    
                Cy_I3C_WriteIntoDeviceAddressTable(base, topDevPos, 0UL);
            }
    
            else
            {
                //Top Device is an i3c device
    
                //Moving the top device to the detached device position
                i3cDeviceTop = context->devList[topDevPos].i3cDevice;
                context->devList[devIndex].i3cDevice = i3cDeviceTop;
                context->devList[devIndex].i2c = false;
                
                //Erase the top position entries and mark the position free
                context->i3cMaster.freePos |= CY_I3C_BIT(topDevPos);
                
                value = _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, i3cDeviceTop.dynamicAddress) |
                        _VAL2FLD(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, i3cDeviceTop.staticAddress);
                
                Cy_I3C_WriteIntoDeviceAddressTable(base, devIndex, value);
                
                Cy_I3C_WriteIntoDeviceAddressTable(base, topDevPos, 0UL);
            }
        }
    
        //Case where the device getting detached is the top device on the list and thus no rearrangement is not required
        else
        {
            Cy_I3C_WriteIntoDeviceAddressTable(base, devIndex, 0UL);
            
            context->i3cMaster.freePos |= CY_I3C_BIT(devIndex);
        }

        return CY_I3C_SUCCESS;
}


/*******************************************************************************
*  Function Name: DeviceIBIControl
****************************************************************************//**
*
* Handles enabling and disabling of IBI events from specified devices.
*
* \param base
* The pointer to the I3C instance.
* 
* \param i3cDevice
* The pointer to the i3c device description structure \ref cy_stc_i3c_device_t.
*
* \param cccCmd
* The CCC command to be sent ENEC/DISEC.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t DeviceIBIControl(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, uint8_t cccCmd, cy_stc_i3c_context_t *context)
{
    uint32_t sirmap, bitpos, mrmap;
    cy_en_i3c_status_t retStatus;
    cy_stc_i3c_ccc_cmd_t cmd;
    cy_stc_i3c_ccc_events_t i3cCccENEC;
    cy_stc_i3c_ccc_payload_t payload;

    //Sending DISEC command
    i3cCccENEC.events = CY_I3C_CCC_EVENT_SIR | CY_I3C_CCC_EVENT_MR;
    cmd.cmd = cccCmd;
    cmd.data = &payload;
    cmd.data->len = sizeof(i3cCccENEC);
    cmd.data->data = &(i3cCccENEC.events);
    cmd.address = i3cDevice->dynamicAddress;

    retStatus = enec_disec_ccc(base, &cmd, context); //Should the response be checked for any errors?

    //In case of successful CCC command
    if(retStatus == CY_I3C_SUCCESS)
    {
        sirmap = I3C_CORE_IBI_SIR_REQ_REJECT(base);
        bitpos = CY_I3C_IBI_SIR_REQ_ID(i3cDevice->dynamicAddress);
        sirmap |= ((1UL) << bitpos); //Setting the corresponding bit to 1: 1 -> Nack the SIR from the corresponding device
        I3C_CORE_IBI_SIR_REQ_REJECT(base) = sirmap;

        mrmap = I3C_CORE_IBI_MR_REQ_REJECT(base);
        mrmap |= ((1UL) << bitpos); //Setting the corresponding bit to 1: 1 -> Nack the MR from the corresponding device
        I3C_CORE_IBI_MR_REQ_REJECT(base) = mrmap;
    }

    return retStatus;
}


/*******************************************************************************
*  Function Name: MasterHandleCCCResponse
****************************************************************************//**
*
* Validates whether the slave address is an I3C device address on the bus/ 
* broadcast address.
*
* \param address
* The address to be validated.
* 
* \param uincastOnly
* true: address is checked for I3C device address on the bus only.
* false: otherwise.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t CCCSlaveAddressValidation(uint8_t address, bool uincastOnly, cy_stc_i3c_context_t *context)
{
    uint32_t res;

    res = GetAaddrslotStatus(address, context);

    if(uincastOnly)
    {
        if(CY_I3C_ADDR_SLOT_I3C_DEV != res)
            return CY_I3C_BAD_PARAM; 
        
        context->masterStatus |=  CY_I3C_MASTER_DIRECTED_CCC_WR_XFER;
        
        return CY_I3C_SUCCESS;
    }
        
    else
    {
        //This helps to check if the device is actively present on the bus
        if((CY_I3C_ADDR_SLOT_I3C_DEV != res ) && (CY_I3C_ADDR_SLOT_RSVD != res))
            return CY_I3C_BAD_PARAM;

        if(CY_I3C_BROADCAST_ADDR == address)
        {
            context->masterStatus |= CY_I3C_MASTER_BROADCAST_CCC_WR_XFER;
        }

        else
        {
            context->masterStatus |=  CY_I3C_MASTER_DIRECTED_CCC_WR_XFER;
        }

        return CY_I3C_SUCCESS;
    }
}


/*******************************************************************************
*  Function Name: MasterHandleCCCResponse
****************************************************************************//**
*
* Handles the response of CCCs sent.
*
* \param base
* The pointer to the I3C instance.
* 
* \param resp
* The pointer to the response queue port register value.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_i3c_status_t
*
*******************************************************************************/
static cy_en_i3c_status_t MasterHandleCCCResponse(I3C_CORE_Type *base, uint32_t *resp, cy_stc_i3c_context_t *context)
{
    uint32_t respCmdPort;
    cy_en_i3c_status_t retStatus = CY_I3C_SUCCESS;
    
    //Handling the response
    do
    {
        //wait till the interrupt of response is received
    }while(0 == ((CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_TRANSFER_ERR_STS) & Cy_I3C_GetInterruptStatus(base)));

    respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);
    Cy_I3C_ClearInterrupt(base, CY_I3C_INTR_MASK);
    
    if(0 != (respCmdPort & I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_Msk))
    {
        retStatus = ResponseError(respCmdPort);    //unsuccessful due to transfer error
        context->masterStatus |= CY_I3C_MASTER_HALT_STATE;
    }

    Cy_I3C_SetInterruptMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
    Cy_I3C_SetInterruptStatusMask(base, CY_I3C_INTR_IBI_BUFFER_THLD_STS);
    
    return retStatus;
}

/*******************************************************************************
*  Function Name: SlaveRespReadyStsHandle
****************************************************************************//**
*
* Handles the response queue ready status interrupt \ref group_i3c_intr_macros
* for slave mode of operation.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void SlaveRespReadyStsHandle(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    uint32_t respCmdPort;
    uint32_t event;

    respCmdPort = I3C_CORE_RESPONSE_QUEUE_PORT(base);

    if(0 != (respCmdPort & I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_ERR_STS_Msk))
    {
        uint32_t errorEvent;
    
        errorEvent = ResponseErrorEvent(respCmdPort);
        if(NULL != context->cbEvents)
        {
            context->cbEvents(errorEvent);
        }
        return;
    }
    if(respCmdPort & I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_RX_RSP_Msk)
    {
        context->slaveRxBufferCnt = _FLD2VAL(I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_DATA_LENGTH, respCmdPort);

        if(CY_I3C_CCC_DEFSLVS != _FLD2VAL(I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_CCC_HDR_HEADER, respCmdPort))
        {
            //Receive Response - master write
            SlaveHandleDataReceive(base, context);

            I3C_CORE_RESET_CTRL(base) |= I3C_CORE_RESET_CTRL_RX_FIFO_RST_Msk;

            context->slaveStatus = CY_I3C_SLAVE_RD_CMPLT;
            event = CY_I3C_SLAVE_RD_CMPLT_EVENT;

            context->state = CY_I3C_IDLE;
            if(NULL != context->cbEvents)
            {
                context->cbEvents(event);
            }
        }
    }

    else
    {
        //Transmit Response - master read
        uint32_t size;

        size = (CY_I3C_FIFO_SIZE/4) - Cy_I3C_GetFreeEntriesInTxFifo(base); //Gets the number of entries in the Tx Fifo

        context->slaveTxBufferCnt = (context->slaveTxBufferIdx) - size; 
        context->slaveStatus = CY_I3C_SLAVE_WR_CMPLT;
        event = CY_I3C_SLAVE_WR_CMPLT_EVENT;

        context->state = CY_I3C_IDLE;
        if(NULL != context->cbEvents)
        {
            context->cbEvents(event);
        }
    }
}


/*******************************************************************************
* Function Name: SlaveHandleDataReceive
****************************************************************************//**
*
* Reads data from RX FIFO into the buffer provided by
* \ref Cy_I3C_SlaveConfigWriteBuf.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void SlaveHandleDataReceive(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    if(context->slaveRxBufferSize > 0UL)
    {
        context->state = CY_I3C_SLAVE_RX;
        context->slaveStatus = CY_I3C_SLAVE_RD_BUSY;
        uint32_t numToCopy;

        numToCopy =  ((CY_I3C_FIFO_SIZE/4) - Cy_I3C_GetFreeEntriesInRxFifo(base));

        ReadArray(base, context->slaveRxBuffer, numToCopy);
        context->slaveRxBufferIdx += numToCopy;
        context->slaveRxBufferSize -= numToCopy;

        context->slaveRxBuffer = &(context->slaveRxBuffer[context->slaveRxBufferIdx]);
    }
}


/*******************************************************************************
* Function Name: SlaveHandleDataTransmit
****************************************************************************//**
*
* Loads TX FIFO with data provided by \ref Cy_I3C_SlaveConfigReadBuf.
*
* \param base
* The pointer to the I3C instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void SlaveHandleDataTransmit(I3C_CORE_Type *base, cy_stc_i3c_context_t *context)
{
    if(context->slaveTxBufferSize)
    {
        context->state = CY_I3C_SLAVE_TX;
        context->slaveStatus = CY_I3C_SLAVE_WR_BUSY;
        
        uint32_t numToCopy;

        numToCopy = Cy_I3C_GetFreeEntriesInTxFifo(base)*4;

        if(numToCopy > (context->slaveTxBufferSize))
        {
            numToCopy = context->slaveTxBufferSize;
        }
        WriteArray(base, context->slaveTxBuffer, numToCopy);
        context->slaveTxBufferIdx += numToCopy;
        context->slaveTxBufferSize -= numToCopy;
        context->slaveTxBuffer = &context->slaveTxBuffer[context->slaveTxBufferIdx];
    }
}

#if defined(__cplusplus)
    }
#endif

#endif /* CY_IP_MXI3C */
/* [] END OF FILE */
