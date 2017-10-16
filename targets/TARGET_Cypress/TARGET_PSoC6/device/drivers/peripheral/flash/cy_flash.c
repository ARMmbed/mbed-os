/***************************************************************************//**
* \file cy_flash.c
* \version 2.0
*
* \brief
* Provides the public functions for the API for the PSoC 6 Flash Driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_flash.h"


/** \cond INTERNAL */
/** Slow control register */
#define CY_FLASH_TST_DDFT_SLOW_CTL_REG             (*(reg32 *) 0x40260108U)
/** Slow control register */                       
#define CY_FLASH_TST_DDFT_FAST_CTL_REG             (*(reg32 *) 0x40260104U)
/** Slow output register */                        
#define CY_FLASH_CLK_OUTPUT_SLOW_REG               (*(reg32 *) 0x40260518U)
/** Slow clock calibration register */             
#define CY_FLASH_CLK_CAL_CNT1_REG                  (*(reg32 *) 0x4026051CU)
/** Define to set the IMO to perform a delay after the flash operation started */
#define CY_FLASH_TST_DDFT_SLOW_CTL_MASK            (0x40001F1EU)
/** Slow output register */
#define CY_FLASH_CLK_OUTPUT_SLOW_MASK              (0x06U)
/** Define to indicate that clock is finished counting */
#define CY_FLASH_CLK_CAL_CNT1_DONE                 ((uint32_t) ((uint32_t) 1U << CY_FLASH_CLK_CAL_CNT1_DONE_POS))
/** Define to indicate that clock is finished counting */
#define CY_FLASH_CLK_CAL_CNT1_DONE_POS             (31U)
/* Calculates the time in microseconds to wait for the number of the CM0P ticks */
#define CY_FLASH_DEL_CORRECTIVE(ticks)             ((((uint32)Cy_SysClk_ClkPeriGetDivider() + 1UL) * (Cy_SysClk_ClkSlowGetDivider() + 1UL) * (ticks) * 1000UL)\
                                                   / ((uint32_t)cy_Hfclk0FreqHz / 1000UL))
/* Number of the CM0P ticks for StartWrite function delay corrective time */
#define CY_FLASH_START_WRITE_DEL_TICKS             (19000UL)
/* Number of the CM0P ticks for StartProgram function delay corrective time */
#define CY_FLASH_START_PROGRAM_DEL_TICKS           (5250UL)
/* Number of the CM0P ticks for StartErase function delay corrective time */
#define CY_FLASH_START_ERASE_DEL_TICKS             (8500UL)
/* Delay time for StartWrite function in us */
#define CY_FLASH_START_WRITE_DEL_TIME              (9800UL)
/* Delay time for StartProgram function in us */
#define CY_FLASH_START_PROGRAM_DEL_TIME            (120UL)
/* Delay time for StartErase function in us */
#define CY_FLASH_START_ERASE_DEL_TIME              (1000UL)
/** Delay time for Start Write function in us with corrective time */
#define CY_FLASH_START_WRITE_DELAY                 (uint32_t)(CY_FLASH_START_WRITE_DEL_TIME +\
                                                   CY_FLASH_DEL_CORRECTIVE(CY_FLASH_START_WRITE_DEL_TICKS))
/** Delay time for Start Program function in us with corrective time */
#define CY_FLASH_START_PROGRAM_DELAY               (uint32_t)(CY_FLASH_START_PROGRAM_DEL_TIME +\
                                                   CY_FLASH_DEL_CORRECTIVE(CY_FLASH_START_PROGRAM_DEL_TICKS))
/** Delay time fot Start Erase function in uS with corrective time */
#define CY_FLASH_START_ERASE_DELAY                 (uint32_t)(CY_FLASH_START_ERASE_DEL_TIME +\
                                                   CY_FLASH_DEL_CORRECTIVE(CY_FLASH_START_ERASE_DEL_TICKS))
/** Number of ticks to wait 1 uS */
#define CY_FLASH_TICKS_FOR_1US                     (8U)
/** Disable delay */                               
#define CY_FLASH_NO_DELAY                          (0U)
/** Slow control register */                       
#define CY_FLASH_TST_DDFT_FAST_CTL_MASK            (62U)
/** Command completed with no errors */
#define CY_FLASH_ROMCODE_SUCCESS                   (0xA0000000UL)
/** Invalid device protection state */
#define CY_FLASH_ROMCODE_INVALID_PROTECTION        (0xF0000001UL)
/** Invalid flash page latch address */
#define CY_FLASH_ROMCODE_INVALID_FM_PL             (0xF0000003UL)
/** Invalid flash address */
#define CY_FLASH_ROMCODE_INVALID_FLASH_ADDR        (0xF0000004UL)
/** Row is write protected */
#define CY_FLASH_ROMCODE_ROW_PROTECTED             (0xF0000005UL)
/** Comparison between Page Latches and FM row failed */
#define CY_FLASH_ROMCODE_PL_ROW_COMP_FA            (0xF0000022UL)
/** Command in progress; no error */
#define CY_FLASH_ROMCODE_IN_PROGRESS_NO_ERROR      (0xA0000009UL)
/** Flash operation is successfully initiated */
#define CY_FLASH_IS_OPERATION_STARTED              (0x00000010UL)
/** Flash is under operation */
#define CY_FLASH_IS_BUSY                           (0x00000040UL)
/** IPC structure is already locked by another process */
#define CY_FLASH_IS_IPC_BUSY                       (0x00000080UL)
/** Input parameters passed to Flash API are not valid */
#define CY_FLASH_IS_INVALID_INPUT_PARAMETERS       (0x00000100UL)
/** Result mask */
#define CY_FLASH_RESULT_MASK                       (0xFFFFFFFUL)
/** Error shift */
#define CY_FLASH_ERROR_SHIFT                       (28UL)
/** No error */
#define CY_FLASH_ERROR_NO_ERROR                    (0xAUL)
/** CM4 Flash Proxy address */
#define CY_FLASH_CM4_FLASH_PROXY_ADDR              (0x00007001UL)

#if (CY_CPU_CORTEX_M0P)
#define IS_CY_PIPE_FREE(...)       (!Cy_IPC_Drv_IsLockAcquired(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_INTR_CYPIPE_EP1)))
#define NOTIFY_PEER_CORE(a)         Cy_IPC_Pipe_SendMessage(CY_IPC_EP_CYPIPE_CM4_ADDR, CY_IPC_EP_CYPIPE_CM0_ADDR, a, NULL)
#else
#define IS_CY_PIPE_FREE(...)       (!Cy_IPC_Drv_IsLockAcquired(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_INTR_CYPIPE_EP0)))
#define NOTIFY_PEER_CORE(a)         Cy_IPC_Pipe_SendMessage(CY_IPC_EP_CYPIPE_CM0_ADDR, CY_IPC_EP_CYPIPE_CM4_ADDR, a, NULL)
#endif
/** \endcond */

/* Static functions */
static bool Cy_Flash_BoundsCheck(uint32_t flashAddr);
static uint32_t Cy_Flash_PrepareContext(uint32_t rowAddr, const uint32_t *data, uint32_t blocking);
static cy_en_flashdrv_status_t Cy_Flash_ProcessOpcode(uint32_t opcode);
static cy_en_flashdrv_status_t Cy_Flash_OperationStatus(void);
static cy_en_flashdrv_status_t Cy_Flash_SendCmd(uint32_t mode, uint32_t microseconds);
static void Cy_Flash_RAMDelay(uint32_t microseconds);
static cy_stc_flash_context_t context;

/** \cond INTERNAL */
CY_ALIGN(4) static IPC_MSG ipcWaitMessage = 
{
    /* .clientID    */ CY_FLASH_IPC_CLIENT_ID,
    /* .pktType     */ CY_FLASH_ENTER_WAIT_LOOP,
    /* .intrRelMask */ 0
};
/** \endcond */

/*******************************************************************************
* Function Name: Cy_Flash_SendCmd
****************************************************************************//**
*
* Sends a command to the SROM via the IPC channel. The function is placed to the
* SRAM memory to guarantee successful operation. After an IPC message is sent,
* the function waits for a defined time before exiting the function.
*
* \param mode Sets the blocking or non-blocking Flash operation.
*
* \param microseconds The number of microseconds to wait before exiting the functions.
*
* \return Returns the status of the Flash operation,
* see \ref cy_en_flashdrv_status_t.
*
*******************************************************************************/
#if defined (__ICCARM__)
    __ramfunc 
#else
    CY_SECTION(".cy_ramfunc")
#endif
static cy_en_flashdrv_status_t Cy_Flash_SendCmd(uint32_t mode, uint32_t microseconds)
{
    uint32_t intr;
    cy_en_flashdrv_status_t result = CY_FLASH_DRV_IPC_BUSY;

    if (IS_CY_PIPE_FREE())
    {
        if (CY_IPC_SEMA_STATUS_LOCKED != Cy_IPC_Sema_Status(CY_FLASH_WAIT_SEMA))
        {
            if (CY_IPC_PIPE_SUCCESS == NOTIFY_PEER_CORE(&ipcWaitMessage))
            {
                /* Wait for SEMA lock by peer core */
                while(CY_IPC_SEMA_STATUS_LOCKED != Cy_IPC_Sema_Status(CY_FLASH_WAIT_SEMA))
                {
                }

                intr = Cy_SysLib_EnterCriticalSection();

                /* Tries to acquire the IPC structure and pass the arguments to SROM API */
                if (Cy_IPC_Drv_SendMsgPtr(CY_FLASH_IPC_STRUCT, CY_FLASH_IPC_NOTIFY_STRUCT0, (void*)&context) == CY_IPC_DRV_SUCCESS)
                {
                    if (mode == CY_FLASH_NON_BLOCKING_MODE)
                    {
                        Cy_Flash_RAMDelay(microseconds);

                        /* The Flash operation is successfully initiated */
                        result = CY_FLASH_DRV_OPERATION_STARTED;
                    }
                    else
                    {
                        /* Polls whether the IPC is released and the Flash operation is performed */
                        do
                        {
                            result = Cy_Flash_IsWriteComplete();
                        }
                        while (result == CY_FLASH_DRV_OPCODE_BUSY);
                    }
                }
                else
                {
                    /* The IPC structure is already locked by another process */
                    result = CY_FLASH_DRV_IPC_BUSY;
                }

                while ( CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Clear(CY_FLASH_WAIT_SEMA, false))
                {
                }
                
                Cy_SysLib_ExitCriticalSection(intr);
            }
        }
    }

    return (result);
}

/*******************************************************************************
* Function Name: Cy_Flash_EraseRow
****************************************************************************//**
*
* This function erases a single row of flash. Reports success or
* a reason for failure. Does not return until the Write operation is
* complete. Returns immediately and reports a \ref CY_FLASH_DRV_IPC_BUSY error in
* the case when another process is writing to flash or erasing the row.
* User firmware should not enter the hibernate mode until flash erase is
* complete. The Flash operation is allowed in Sleep and Deep-sleep modes.
* During the Flash operation, the device should not be reset, including the
* XRES pin, a software reset, and watchdog reset sources. Also, low-voltage
* detect circuits should be configured to generate an interrupt instead of a
* reset. Otherwise, portions of flash may undergo unexpected changes.
*
* \param rowAddr Address of the flash row number. The number of the flash rows
* is defined by the \ref CY_FLASH_NUMBER_ROWS macro for the selected device.
* The Read-while-Write violation occurs when the flash read operation is
* initiated in the same flash sector where the flash write operation is
* performing. Refer to the device datasheet for the details.
* Address must match row start address.
*
* \return Returns the status of the Flash operation,
* see \ref cy_en_flashdrv_status_t.
*
* \sideeffect If this function is called by the CM4 the user code on CM0P and
* the user code on CM4 are blocked until erase flash row operation is finished.
* If this function is called by the CM0P the user code on CM4 is not blocked and
* the user code on CM0P is blocked until erase flash row operation is finished.
* Plan your task allocation accordingly.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_EraseRow(uint32_t rowAddr)
{
    cy_en_flashdrv_status_t result;

    /* Prepares arguments to be passed to SROM API */
    if (Cy_Flash_BoundsCheck(rowAddr) != false)
    {
        context.opcode = CY_FLASH_OPCODE_ERASE_ROW | CY_FLASH_BLOCKING_MODE;
        context.arg1 = rowAddr;
        context.arg2 = 0UL;
        context.arg3 = 0UL;

#if (CY_CPU_CORTEX_M0P)
        result = Cy_Flash_SendCmd(CY_FLASH_BLOCKING_MODE, CY_FLASH_NO_DELAY);
#else
        Cy_Flash_Proxy Cy_Flash_Cm4Proxy;
        Cy_Flash_Cm4Proxy = (Cy_Flash_Proxy)CY_FLASH_CM4_FLASH_PROXY_ADDR;
        result = Cy_Flash_Cm4Proxy(&context);
        result = Cy_Flash_ProcessOpcode((uint32_t)result);

#endif /* (CY_CPU_CORTEX_M0P) */
    }
    else
    {
        result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }

    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_WriteRow
****************************************************************************//**
*
* This function writes an array of data to a single row of flash.
* Reports success or a reason for failure. Does not return until the Write
* operation is complete.
* Returns immediately and reports a \ref CY_FLASH_DRV_IPC_BUSY error in the case
* when another process is writing to flash. User firmware should not enter the
* hibernate mode until flash Write is complete. The Flash operation is allowed
* in Sleep and Deep-sleep modes. During the Flash operation, the device should
* not be reset, including the XRES pin, a software reset, and watchdog reset
* sources. Also, low-voltage detect circuits should be configured to generate
* an interrupt instead of a reset. Otherwise, portions of flash may undergo
* unexpected changes.
*
* \param rowAddr Address of the flash row number. The number of the flash rows
* is defined by the \ref CY_FLASH_NUMBER_ROWS macro for the selected device.
* The Read-while-Write violation occurs when the flash read operation is
* initiated in the same flash sector where the flash write operation is
* performing. Refer to the device datasheet for the details.
* Address must match row start address.
*
* \param data The pointer to the data which has to be written to flash. The size
* of the data array must be equal to the flash row size. The flash row size for
* the selected device is defined by the \ref CY_FLASH_SIZEOF_ROW macro. Refer to
* the device datasheet for the details.
*
* \return Returns the status of the Flash operation,
* see \ref cy_en_flashdrv_status_t.
*
* \sideeffect If this function is called by the CM4 the user code on CM0P and
* the user code on CM4 are blocked until erase flash row operation is finished.
* If this function is called by the CM0P the user code on CM4 is not blocked and
* the user code on CM0P is blocked until erase flash row operation is finished.
* Plan your task allocation accordingly.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_WriteRow(uint32_t rowAddr, const uint32_t* data)
{
    cy_en_flashdrv_status_t result;

#if (CY_CPU_CORTEX_M0P)
    /* Checks whether the input parameters are valid */
    if (Cy_Flash_PrepareContext(rowAddr, data, CY_FLASH_BLOCKING_MODE) != (uint32_t)CY_FLASH_DRV_INVALID_INPUT_PARAMETERS)
    {
        result = Cy_Flash_SendCmd(CY_FLASH_BLOCKING_MODE, CY_FLASH_NO_DELAY);
    }
    else
    {
        result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }

#else
    if (Cy_Flash_PrepareContext(rowAddr, data, CY_FLASH_BLOCKING_MODE) != (uint32_t)CY_FLASH_DRV_INVALID_INPUT_PARAMETERS)
    {
        Cy_Flash_Proxy Cy_Flash_Cm4Proxy;
        Cy_Flash_Cm4Proxy = (Cy_Flash_Proxy)CY_FLASH_CM4_FLASH_PROXY_ADDR;
        result = Cy_Flash_Cm4Proxy(&context);
        result = Cy_Flash_ProcessOpcode((uint32_t)result);
    }
    else
    {
        result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }
#endif /* (CY_CPU_CORTEX_M0P) */


    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_StartWrite
****************************************************************************//**
*
* Starts writing an array of data to a single row of flash. Returns immediately
* and reports a successful start or reason for failure. Returns immediately and
* reports a \ref CY_FLASH_DRV_IPC_BUSY error in the case when another process is writing
* to flash. User firmware should not enter the hibernate mode until flash Write
* is complete. The Flash operation is allowed in Sleep and Deep-sleep modes.
* During the flash operation, the device should not be reset, including the
* XRES pin, a software reset, and watchdog reset sources. Also, the low-voltage
* detect circuits should be configured to generate an interrupt instead of a reset.
* Otherwise, portions of flash may undergo unexpected changes.
* \note Before reading data from previously programmed/erased flash rows, the
* user must clear the flash cache with the Cy_SysLib_ClearFlashCacheAndBuffer()
* function.
*
* \param rowAddr Address of the flash row number. The number of the flash rows
* is defined by the \ref CY_FLASH_NUMBER_ROWS macro for the selected device.
* The Read-while-Write violation occurs when the flash read operation is
* initiated in the same flash sector where the flash write operation is
* performing. Refer to the device datasheet for the details.
* Address must match row start address.
*
* \param data The pointer to the data to be written to flash. The size
* of the data array must be equal to the flash row size. The flash row size for
* the selected device is defined by the \ref CY_FLASH_SIZEOF_ROW macro. Refer to
* the device datasheet for the details.
*
* \return Returns the status of the Flash operation,
* see \ref cy_en_flashdrv_status_t.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_StartWrite(uint32_t rowAddr, const uint32_t* data)
{
    cy_en_flashdrv_status_t result;

    /* Checks whether the input parameters are valid */
    if (Cy_Flash_PrepareContext(rowAddr, data, CY_FLASH_NON_BLOCKING_MODE) != (uint32_t)CY_FLASH_DRV_INVALID_INPUT_PARAMETERS)
    {
        result = Cy_Flash_SendCmd(CY_FLASH_NON_BLOCKING_MODE, CY_FLASH_START_WRITE_DELAY);
    }
    else
    {
        result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }

    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_IsWriteComplete
****************************************************************************//**
*
* Reports a successful Write, reason of failure or busy status
* ( \ref CY_FLASH_DRV_OPCODE_BUSY ).
*
* \return Returns the status of the Flash operation (see \ref cy_en_flashdrv_status_t).
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_IsWriteComplete(void)
{
    return (Cy_Flash_OperationStatus());
}


/*******************************************************************************
* Function Name: Cy_Flash_StartErase
****************************************************************************//**
*
* Starts erasing a single row of flash. Returns immediately
* and reports a successful start or reason for failure. Returns immediately and
* reports a \ref CY_FLASH_DRV_IPC_BUSY error in the case when IPC structure is locked
* by another process. User firmware should not enter the hibernate mode until
* flash Erase is complete. The Flash operation is allowed in Sleep and Deep-sleep modes.
* During the flash operation, the device should not be reset, including the
* XRES pin, a software reset, and watchdog reset sources. Also, the low-voltage
* detect circuits should be configured to generate an interrupt instead of a reset.
* Otherwise, portions of flash may undergo unexpected changes.
* \note Before reading data from previously programmed/erased flash rows, the
* user must clear the flash cache with the Cy_SysLib_ClearFlashCacheAndBuffer()
* function.
*
* \param rowAddr Address of the flash row number. The number of the flash rows
* is defined by the \ref CY_FLASH_NUMBER_ROWS macro for the selected device.
* The Read-while-Write violation occurs when the flash read operation is
* initiated in the same flash sector where the flash erase operation is
* performing. Refer to the device datasheet for the details.
* Address must match row start address.
*
* \return Returns the status of the Flash operation,
* see \ref cy_en_flashdrv_status_t.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_StartErase(uint32_t rowAddr)
{
    cy_en_flashdrv_status_t retVal;

    /* Prepares arguments to be passed to SROM API */
    if (Cy_Flash_BoundsCheck(rowAddr) != false)
    {
        context.opcode = CY_FLASH_OPCODE_ERASE_ROW;
        context.arg1 = rowAddr;
        context.arg2 = 0UL;
        context.arg3 = 0UL;
        retVal = Cy_Flash_SendCmd(CY_FLASH_NON_BLOCKING_MODE, CY_FLASH_START_ERASE_DELAY);
    }
    else
    {
        retVal = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }

    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_Flash_IsEraseComplete
****************************************************************************//**
*
* Reports a successful Erase, reason of failure or busy status
* ( \ref CY_FLASH_DRV_OPCODE_BUSY ).
*
* \return Returns the status of the Flash operation (see \ref cy_en_flashdrv_status_t).
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_IsEraseComplete(void)
{
    return (Cy_Flash_OperationStatus());
}


/*******************************************************************************
* Function Name: Cy_Flash_StartProgram
****************************************************************************//**
* Starts writing an array of data to a single row of flash. Returns immediately 
* and reports a successful start or reason for failure. Returns immediately and
* reports a \ref CY_FLASH_DRV_IPC_BUSY error if another process is writing
* to flash. The user firmware should not enter Hibernate mode until flash Write
* is complete. The Flash operation is allowed in Sleep and Deep-Sleep modes.
* During the Flash operation, the device should not be reset, including the
* XRES pin, a software reset, and watchdog reset sources. Also, the low-voltage
* detect circuits should be configured to generate an interrupt instead of a reset.
* Otherwise, portions of flash may undergo unexpected changes.\n
* Before calling this function, the target flash region must be erased by
* the StartErase/EraseRow function.\n
* Data to be programmed must be located in the SRAM memory region.
* \note Before reading data from previously programmed/erased flash rows, the
* user must clear the flash cache with the Cy_SysLib_ClearFlashCacheAndBuffer()
* function.
*
* \param rowAddr The address of the flash row number. The number of the flash rows
* is defined by the \ref CY_FLASH_NUMBER_ROWS macro for the selected device.
* The Read-while-Write violation occurs when the Flash Write operation is
* performing. Refer to the device datasheet for the details.
* The address must match the row start address.
*
* \param data The pointer to the data to be written to flash. The size
* of the data array must be equal to the flash row size. The flash row size for
* the selected device is defined by the \ref CY_FLASH_SIZEOF_ROW macro. Refer to
* the device datasheet for the details.
*
* \return Returns the status of the Flash operation,
* see \ref cy_en_flashdrv_status_t.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_StartProgram(uint32_t rowAddr, const uint32_t* data)
{
    cy_en_flashdrv_status_t result;

    /* Prepares arguments to be passed to SROM API */
    if (Cy_Flash_BoundsCheck(rowAddr) != false)
    {
        context.opcode = CY_FLASH_OPCODE_PROGRAM_ROW | CY_FLASH_NON_BLOCKING_MODE;
        context.arg1 = CY_FLASH_CONFIG_DATASIZE | CY_FLASH_DATA_LOC_SRAM;
        context.arg2 = rowAddr;
        context.arg3 = (uint32_t)data;

        result = Cy_Flash_SendCmd(CY_FLASH_NON_BLOCKING_MODE, CY_FLASH_START_PROGRAM_DELAY);
    }
    else
    {
        result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }
    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_IsProgramComplete
****************************************************************************//**
*
* Reports a successful program operation, failure or busy status
* ( \ref CY_FLASH_DRV_OPCODE_BUSY ).
*
* \return Returns the status of the Flash operation (see \ref cy_en_flashdrv_status_t).
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_IsProgramComplete(void)
{
    return (Cy_Flash_OperationStatus());
}

/*******************************************************************************
* Function Name: Cy_Flash_RowChecksum
****************************************************************************//**
*
* Returns a checksum value of the specified flash row.
*
* \param rowNum The Checksum is calculated to the flash row.
*
* \param checksumPtr The pointer to the address where checksum is to be stored
*
* \return Returns the status of the Flash operation.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_RowChecksum (uint32_t rowNum, uint32_t* checksumPtr)
{
    cy_en_flashdrv_status_t result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    uint32_t resTmp;

    /* Checks whether the input parameters are valid */
    if (rowNum < CY_FLASH_NUMBER_ROWS)
    {
        /* Prepares arguments to be passed to SROM API */
        context.opcode = CY_FLASH_OPCODE_CHECKSUM | (rowNum << CY_FLASH_OPCODE_CHECKSUM_ROW_SHIFT);

        /* Tries to acquire the IPC structure and pass the arguments to SROM API */
        if (Cy_IPC_Drv_SendMsgPtr(CY_FLASH_IPC_STRUCT, CY_FLASH_IPC_NOTIFY_STRUCT0, (void*)&context) == CY_IPC_DRV_SUCCESS)
        {
            /* Polls whether IPC is released and the Flash operation is performed */
            while (Cy_IPC_Drv_IsLockAcquired(CY_FLASH_IPC_STRUCT) != false)
            {
                /* Wait till IPC is released */
            }

            resTmp = context.opcode;

            if((resTmp >> CY_FLASH_ERROR_SHIFT) == CY_FLASH_ERROR_NO_ERROR)
            {
                result = CY_FLASH_DRV_SUCCESS;
                *checksumPtr = context.opcode & CY_FLASH_RESULT_MASK;
            }
        }
        else
        {
            /* The IPC structure is already locked by another process */
            result = CY_FLASH_DRV_IPC_BUSY;
        }
    }

    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_CalculateHash
****************************************************************************//**
*
* Returns a hash value of the specified region of flash.
*
* \param data Start the data address.
*
* \param numberOfBytes The hash value is calculated for the number of bytes after the
* start data address (0 - 1 byte, 1- 2 bytes etc).
*
* \param hashPtr The pointer to the address where hash is to be stored
*
* \return Returns the status of the Flash operation.
*
*******************************************************************************/
cy_en_flashdrv_status_t Cy_Flash_CalculateHash (const uint32_t* data, uint32_t numberOfBytes,  uint32_t* hashPtr)
{
    cy_en_flashdrv_status_t result = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    volatile uint32_t resTmp;

    /* Checks whether the input parameters are valid */
    if (data != NULL)
    {
        /* Prepares arguments to be passed to SROM API */
        context.opcode = CY_FLASH_OPCODE_HASH;
        context.arg1 = (uint32_t)data;
        context.arg2 = numberOfBytes;

        /* Tries to acquire the IPC structure and pass the arguments to SROM API */
        if (Cy_IPC_Drv_SendMsgPtr(CY_FLASH_IPC_STRUCT, CY_FLASH_IPC_NOTIFY_STRUCT0, (void*)&context) == CY_IPC_DRV_SUCCESS)
        {
            /* Polls whether IPC is released and the Flash operation is performed */
            while (Cy_IPC_Drv_IsLockAcquired(CY_FLASH_IPC_STRUCT) != false)
            {
                /* Wait till IPC is released */
            }

            resTmp = context.opcode;

            if((resTmp >> CY_FLASH_ERROR_SHIFT) == CY_FLASH_ERROR_NO_ERROR)
            {
                result = CY_FLASH_DRV_SUCCESS;
                *hashPtr = context.opcode & CY_FLASH_RESULT_MASK;
            }
        }
        else
        {
            /* The IPC structure is already locked by another process */
            result = CY_FLASH_DRV_IPC_BUSY;
        }
    }

    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_BoundsCheck
****************************************************************************//**
*
* Returns false if Flash address is out of boundary, otherwise returns true.
*
* \param flashAddr Address to be checked
*
* \return false - out of bound, true - in flash bounds
*
*******************************************************************************/
static bool Cy_Flash_BoundsCheck(uint32_t flashAddr)
{
    bool result = true;

    if ((flashAddr < CY_FLASH_BASE) || (flashAddr >= (CY_FLASH_BASE + CY_FLASH_SIZE)))
    {
        if ((flashAddr < CY_WFLASH_BASE) || (flashAddr >= (CY_WFLASH_BASE + CY_WFLASH_SIZE)))
        {
            if ((flashAddr < SFLASH_BASE) || (flashAddr >= (SFLASH_BASE + SFLASH_SECTION_SIZE)))
            {
                result = false;
            }
        }
    }

    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_PrepareContext
****************************************************************************//**
*
* Returns 1 if parameters are correct and initialises context, otherwise returns
* CY_FLASH_DRV_INVALID_INPUT_PARAMETERS.
*
* \param rowAddr Address of the flash row number. The number of the flash rows
* is defined by the \ref CY_FLASH_NUMBER_ROWS macro for the selected device.
* The Read-while-Write violation occurs when the flash read operation is
* initiated in the same flash sector where the flash write operation is
* performing. Refer to the device datasheet for the details.
* Address must match row start address.
*
* \param data The pointer to the data which has to be written to flash. The size
* of the data array must be equal to the flash row size. The flash row size for
* the selected device is defined by the \ref CY_FLASH_SIZEOF_ROW macro. Refer to
* the device datasheet for the details.
*
* \param blocking The parameter of how the context will be executed.
* * 1 - blocking, If this function is called by the CM4 the user code on CM0P
* and the user code on CM4 are blocked until flash write operation is finished.
* If this function is called by the CM0P the user code on CM4 is not blocked
* and the user code on CM0P is blocked until flash write operation is finished.
* * 0 - non-blocking, the user code is not blocked while flash write operation
* is executed.
*
* \return 1 - Context ready, CY_FLASH_DRV_INVALID_INPUT_PARAMETERS - Wrong
* arguments
*
*******************************************************************************/
static uint32_t Cy_Flash_PrepareContext(uint32_t rowAddr, const uint32_t* data, uint32_t blocking)
{
    uint32_t result;

    if ((Cy_Flash_BoundsCheck(rowAddr) == false) || (NULL == data))
    {
        result = (uint32_t)CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
    }
    else
    {
        if ((rowAddr%CY_FLASH_SIZEOF_ROW) != 0UL)
        {
            result = (uint32_t)CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
        }
        else
        {
            /* Prepares arguments to be passed to SROM API */
            context.opcode = CY_FLASH_OPCODE_WRITE_ROW | blocking;
            context.arg1 = CY_FLASH_CONFIG_DATASIZE;
            context.arg2 = rowAddr;
            context.arg3 = (uint32_t)data;

            result = 1UL;
        }
    }

    return (result);
}


/*******************************************************************************
* Function Name: Cy_Flash_ProcessOpcode
****************************************************************************//**
*
* Converts System Call returns to the Flash driver return defines.
*
* \param opcode The value returned by the System Call.
*
* \return Flash driver return.
*
*******************************************************************************/
static cy_en_flashdrv_status_t Cy_Flash_ProcessOpcode(uint32_t opcode)
{
    cy_en_flashdrv_status_t retVal;

    switch (opcode)
    {
        case 0UL:
        {
            retVal = CY_FLASH_DRV_SUCCESS;
            break;
        }
        case CY_FLASH_ROMCODE_SUCCESS:
        {
            retVal = CY_FLASH_DRV_SUCCESS;
            break;
        }
        case CY_FLASH_ROMCODE_INVALID_PROTECTION:
        {
            retVal = CY_FLASH_DRV_INV_PROT;
            break;
        }
        case CY_FLASH_ROMCODE_INVALID_FM_PL:
        {
            retVal = CY_FLASH_DRV_INVALID_FM_PL;
            break;
        }
        case CY_FLASH_ROMCODE_INVALID_FLASH_ADDR:
        {
            retVal = CY_FLASH_DRV_INVALID_FLASH_ADDR;
            break;
        }
        case CY_FLASH_ROMCODE_ROW_PROTECTED:
        {
            retVal = CY_FLASH_DRV_ROW_PROTECTED;
            break;
        }
        case CY_FLASH_ROMCODE_IN_PROGRESS_NO_ERROR:
        {
            retVal = CY_FLASH_DRV_PROGRESS_NO_ERROR;
            break;
        }
        case (uint32_t)CY_FLASH_DRV_INVALID_INPUT_PARAMETERS:
        {
            retVal = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
            break;
        }
        case CY_FLASH_IS_OPERATION_STARTED :
        {
            retVal = CY_FLASH_DRV_OPERATION_STARTED;
            break;
        }
        case CY_FLASH_IS_BUSY :
        {
            retVal = CY_FLASH_DRV_OPCODE_BUSY;
            break;
        }
        case CY_FLASH_IS_IPC_BUSY :
        {
            retVal = CY_FLASH_DRV_IPC_BUSY;
            break;
        }
        case CY_FLASH_IS_INVALID_INPUT_PARAMETERS :
        {
            retVal = CY_FLASH_DRV_INVALID_INPUT_PARAMETERS;
            break;
        }
        default:
        {
            retVal = CY_FLASH_DRV_ERR_UNC;
            break;
        }
    }

    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_Flash_OperationStatus
****************************************************************************//**
*
* Checks the status of the Flash Operation, and returns it.
*
* \return Returns the status of the Flash operation
* (see \ref cy_en_flashdrv_status_t).
*
*******************************************************************************/
static cy_en_flashdrv_status_t Cy_Flash_OperationStatus(void)
{
    cy_en_flashdrv_status_t retVal = CY_FLASH_DRV_OPCODE_BUSY;

    /* Checks if the IPC structure is not locked */
    if (Cy_IPC_Drv_IsLockAcquired(CY_FLASH_IPC_STRUCT) == false)
    {
        /* The result of SROM API calling is returned to the driver context */
        retVal = Cy_Flash_ProcessOpcode(context.opcode);
    }

    return (retVal);
}

/*******************************************************************************
* Function Name: Cy_Flash_RAMDelay
****************************************************************************//**
*
* Wait for a defined time in the SRAM memory region.
*
*******************************************************************************/
#if defined (__ICCARM__)
    __ramfunc 
#else
    CY_SECTION(".cy_ramfunc")
#endif
static void Cy_Flash_RAMDelay(uint32_t microseconds)
{
    uint32_t ticks = microseconds * CY_FLASH_TICKS_FOR_1US;
    if (ticks != CY_FLASH_NO_DELAY)
    {
        CY_FLASH_TST_DDFT_SLOW_CTL_REG = CY_FLASH_TST_DDFT_SLOW_CTL_MASK;
        CY_FLASH_CLK_OUTPUT_SLOW_REG   = CY_FLASH_CLK_OUTPUT_SLOW_MASK;
        CY_FLASH_TST_DDFT_FAST_CTL_REG = CY_FLASH_TST_DDFT_FAST_CTL_MASK;

        /* Load the down-counter */
        CY_FLASH_CLK_CAL_CNT1_REG = ticks;

        /* Make sure that the counter is started */
        (void) CY_FLASH_CLK_CAL_CNT1_REG;

        while(0U == (CY_FLASH_CLK_CAL_CNT1_REG & CY_FLASH_CLK_CAL_CNT1_DONE))
        {
            /* Wait until the counter stops counting */
        }
    }
}

/* [] END OF FILE */
