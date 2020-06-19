/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "fsl_device_registers.h"
#if (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U)) && \
    ((defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U)))
#include "usb_hsdcd.h"
#endif
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#if ((defined FSL_FEATURE_USBHSD_HAS_EXIT_HS_ISSUE) && (FSL_FEATURE_USBHSD_HAS_EXIT_HS_ISSUE > 0U))
#include "usb_phy.h"
#endif
#endif
#if (((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)) || \
     ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)))
#include "usb_device_dci.h"
#include "usb_device_lpcip3511.h"

#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))

#define USB_LPC3511IP_INTSTAT_DEV_INT_MASK USBHSD_INTSTAT_DEV_INT_MASK
#define USB_LPC3511IP_INTSTAT_FRAME_INT_MASK USBHSD_INTSTAT_FRAME_INT_MASK

#define USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AO_MASK USBHSD_DEVCMDSTAT_INTONNAK_AO_MASK
#define USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AI_MASK USBHSD_DEVCMDSTAT_INTONNAK_AI_MASK

#define USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_MASK USBHSD_DEVCMDSTAT_LPM_REWP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_SHIFT USBHSD_DEVCMDSTAT_LPM_REWP_SHIFT

#define USB_LPC3511IP_DEVCMDSTAT_Speed_MASK USBHSD_DEVCMDSTAT_Speed_MASK

#define USB_LPC3511IP_DEVCMDSTAT_DCON_MASK USBHSD_DEVCMDSTAT_DCON_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DEV_EN_MASK USBHSD_DEVCMDSTAT_DEV_EN_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_SUP_MASK USBHSD_DEVCMDSTAT_LPM_SUP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK USBHSD_DEVCMDSTAT_FORCE_NEEDCLK_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_SUS_MASK USBHSD_DEVCMDSTAT_LPM_SUS_MASK

#define USB_LPC3511IP_USB_LPM_HIRD_SW USBHSD_LPM_HIRD_SW

#define USB_LPC3511IP_DEVCMDSTAT_DEV_ADDR_MASK USBHSD_DEVCMDSTAT_DEV_ADDR_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DSUS_MASK USBHSD_DEVCMDSTAT_DSUS_MASK
#define USB_LPC3511IP_INFO_ERR_CODE_MASK USBHSD_INFO_ERR_CODE_MASK
#define USB_LPC3511IP_DEVCMDSTAT_SETUP_MASK USBHSD_DEVCMDSTAT_SETUP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DRES_C_MASK USBHSD_DEVCMDSTAT_DRES_C_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DSUS_C_MASK USBHSD_DEVCMDSTAT_DSUS_C_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DCON_C_MASK USBHSD_DEVCMDSTAT_DCON_C_MASK
#define USB_LPC3511IP_DEVCMDSTAT_VBUS_DEBOUNCED_MASK USBHSD_DEVCMDSTAT_VBUS_DEBOUNCED_MASK
#else
#define USB_LPC3511IP_INTSTAT_DEV_INT_MASK USB_INTSTAT_DEV_INT_MASK
#define USB_LPC3511IP_INTSTAT_FRAME_INT_MASK USB_INTSTAT_FRAME_INT_MASK

#define USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AO_MASK USB_DEVCMDSTAT_INTONNAK_AO_MASK
#define USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AI_MASK USB_DEVCMDSTAT_INTONNAK_AI_MASK

#define USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_MASK USB_DEVCMDSTAT_LPM_REWP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_SHIFT USB_DEVCMDSTAT_LPM_REWP_SHIFT

#define USB_LPC3511IP_DEVCMDSTAT_DCON_MASK USB_DEVCMDSTAT_DCON_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DEV_EN_MASK USB_DEVCMDSTAT_DEV_EN_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_SUP_MASK USB_DEVCMDSTAT_LPM_SUP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK USB_DEVCMDSTAT_FORCE_NEEDCLK_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_SUP_MASK USB_DEVCMDSTAT_LPM_SUP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_LPM_SUS_MASK USB_DEVCMDSTAT_LPM_SUS_MASK

#define USB_LPC3511IP_USB_LPM_HIRD_SW USB_LPM_HIRD_SW

#define USB_LPC3511IP_DEVCMDSTAT_DEV_ADDR_MASK USB_DEVCMDSTAT_DEV_ADDR_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DSUS_MASK USB_DEVCMDSTAT_DSUS_MASK
#define USB_LPC3511IP_INFO_ERR_CODE_MASK USB_INFO_ERR_CODE_MASK
#define USB_LPC3511IP_DEVCMDSTAT_SETUP_MASK USB_DEVCMDSTAT_SETUP_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DRES_C_MASK USB_DEVCMDSTAT_DRES_C_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DSUS_C_MASK USB_DEVCMDSTAT_DSUS_C_MASK
#define USB_LPC3511IP_DEVCMDSTAT_DCON_C_MASK USB_DEVCMDSTAT_DCON_C_MASK
#define USB_LPC3511IP_DEVCMDSTAT_VBUS_DEBOUNCED_MASK USB_DEVCMDSTAT_VBUSDEBOUNCED_MASK
#endif

#define USB_LPC3511IP_USB_LPM_ADPPROBE_MASK (0x00100000u)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* on Aruba IP3511 (USB0 FS), there are 8 physical EPs, on IP3511 HS (USB1 FS), there are 10 physical EPs. */
#define USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK (0xFFFFu)

/*! @brief endpoint command status, buffer address offset */
#define USB_LPC3511IPHS_ENDPOINT_BUFFER_ADDRESS_OFFSET_MASK (0x000007FFu)
#define USB_LPC3511IPHS_ENDPOINT_BUFFER_NBYTES_SHIFT (11)
#define USB_LPC3511IPHS_ENDPOINT_BUFFER_NBYTES_MASK (0x03FFF800u)
#define USB_LPC3511IPFS_ENDPOINT_BUFFER_ADDRESS_OFFSET_MASK (0x0000FFFFu)
#define USB_LPC3511IPFS_ENDPOINT_BUFFER_NBYTES_SHIFT (16)
#define USB_LPC3511IPFS_ENDPOINT_BUFFER_NBYTES_MASK (0x03FF0000u)

#define USB_LPC3511IP_ENDPOINT_ENDPOINT_TYPE_MASK (0x01U << 26)
#define USB_LPC3511IP_ENDPOINT_RFTV_MASK (0x01U << 27)
#define USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK (0x01U << 28)
#define USB_LPC3511IP_ENDPOINT_STALL_MASK (0x01U << 29)
#define USB_LPC3511IP_ENDPOINT_STALL_SHIFT (29)
#define USB_LPC3511IP_ENDPOINT_DISABLE_MASK (0x01U << 30)
#define USB_LPC3511IP_ENDPOINT_ACTIVE_MASK (0x01U << 31)
#define USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT (26)

#define USB_LPC3511IP_DEVCMDSTAT_INTERRUPT_WC_MASK (0x0F000000u)

#define USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(lpcState, index, odd, value)                         \
    *((volatile uint32_t *)(((uint32_t)(lpcState->epCommandStatusList)) | ((uint32_t)(index) << 3) | \
                            (((uint32_t)(odd)&1U) << 2U))) &= (value)

/*! @brief Set endpoint command/status value */
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#define USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpcState, index, odd, value, NBytes, address)                \
                                                                                                         \
    *((volatile uint32_t *)(((uint32_t)(lpcState->epCommandStatusList)) | ((uint32_t)(index) << 3) |     \
                            (((uint32_t)(odd & 1U)) << 2U))) =                                           \
        ((lpc3511IpState->controllerSpeed) ?                                                             \
                                                                                                         \
             ((uint32_t)(value) | ((uint32_t)(NBytes) << USB_LPC3511IPHS_ENDPOINT_BUFFER_NBYTES_SHIFT) | \
              (((uint32_t)(address) >> 6) & USB_LPC3511IPHS_ENDPOINT_BUFFER_ADDRESS_OFFSET_MASK)) :      \
                                                                                                         \
             ((uint32_t)(value) | ((uint32_t)(NBytes) << USB_LPC3511IPFS_ENDPOINT_BUFFER_NBYTES_SHIFT) | \
              (((uint32_t)(address) >> 6) & USB_LPC3511IPFS_ENDPOINT_BUFFER_ADDRESS_OFFSET_MASK)))
#else
#define USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpcState, index, odd, value, NBytes, address)            \
                                                                                                     \
    *((volatile uint32_t *)(((uint32_t)(lpcState->epCommandStatusList)) | ((uint32_t)(index) << 3) | \
                            (((uint32_t)(odd & 1U)) << 2U))) =                                       \
        ((uint32_t)(value) | ((uint32_t)(NBytes) << USB_LPC3511IPFS_ENDPOINT_BUFFER_NBYTES_SHIFT) |  \
         (((uint32_t)(address) >> 6) & USB_LPC3511IPFS_ENDPOINT_BUFFER_ADDRESS_OFFSET_MASK))
#endif

#define USB_LPC3511IP_ENDPOINT_DES_INDEX(endpoint) \
    (((((uint32_t)endpoint) & 0x0F) << 1) +        \
     ((((uint32_t)endpoint) & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ? (1) : (0)))

#define USB_LPC3511IP_GET_MULTIPLE_OF_64(n) ((n + 63U) & 0xFFFFFFC0U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern usb_status_t USB_DeviceNotificationTrigger(void *handle, void *msg);
static usb_status_t USB_DeviceLpc3511IpTransaction(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                   usb_device_lpc3511ip_endpoint_state_struct_t *epState,
                                                   uint8_t endpointIndex);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* define the reserved buffer for endpoint max packet copy */

#define SETUP_TRANSFER_DATA_OFFSET (0u)
#define CONTROL_TRANSFER_DATA_OFFSET ((USB_DATA_ALIGN_SIZE_MULTIPLE(8U) >> 2))
#define ZERO_TRANSFER_DATA_OFFSET ((USB_DATA_ALIGN_SIZE_MULTIPLE(8U) >> 2) + (USB_DATA_ALIGN_SIZE_MULTIPLE(64U) >> 2))
#define RESERVED_EP_DATA_OFFSET                                                           \
    ((USB_DATA_ALIGN_SIZE_MULTIPLE(8U) >> 2) + (USB_DATA_ALIGN_SIZE_MULTIPLE(64U) >> 2) + \
     (USB_DATA_ALIGN_SIZE_MULTIPLE(4) >> 2))

#if defined(USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)
USB_GLOBAL USB_RAM_ADDRESS_ALIGNMENT(USB_DATA_ALIGN_SIZE) static uint32_t
    s_SetupAndEpReservedData[USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY]
                            [USB_DATA_ALIGN_SIZE_MULTIPLE((USB_DEVICE_IP3511_ENDPOINT_RESERVED_BUFFER_SIZE >> 2)) +
                             RESERVED_EP_DATA_OFFSET];
#else
USB_GLOBAL USB_RAM_ADDRESS_ALIGNMENT(USB_DATA_ALIGN_SIZE) static uint32_t
    s_SetupAndEpReservedData[USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS][RESERVED_EP_DATA_OFFSET];
#endif

static usb_device_lpc3511ip_state_struct_t
    s_UsbDeviceLpc3511IpState[USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS];

/* LPC3511IP controller driver instances and endpoint command/status list, EPLISTSTART's value is the buffer pointer. */
#if ((USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS) == 1U)
USB_CONTROLLER_DATA USB_RAM_ADDRESS_ALIGNMENT(256) static uint32_t
    s_EpCommandStatusList1[((USB_DEVICE_IP3511_ENDPOINTS_NUM)) * 4];
#define LPC_CONTROLLER_ENDPOINT_LIST_ARRAY \
    {                                      \
        &s_EpCommandStatusList1[0]         \
    }

#elif ((USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS) == 2U)
USB_CONTROLLER_DATA USB_RAM_ADDRESS_ALIGNMENT(256) static uint32_t
    s_EpCommandStatusList1[(USB_DEVICE_IP3511_ENDPOINTS_NUM)*4];
USB_CONTROLLER_DATA USB_RAM_ADDRESS_ALIGNMENT(256) static uint32_t
    s_EpCommandStatusList2[(USB_DEVICE_IP3511_ENDPOINTS_NUM)*4];
#define LPC_CONTROLLER_ENDPOINT_LIST_ARRAY                     \
    {                                                          \
        &s_EpCommandStatusList1[0], &s_EpCommandStatusList2[0] \
    }

#else
#error "increase the instance count."
#endif

#if (defined USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)
#if (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY == (USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS))
#define USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER 1u
#elif ((USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY == USB_DEVICE_CONFIG_LPCIP3511FS) && \
       (!USB_DEVICE_CONFIG_LPCIP3511HS))
#define USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER 1u
#elif ((USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY == USB_DEVICE_CONFIG_LPCIP3511HS) && \
       (!USB_DEVICE_CONFIG_LPCIP3511FS))
#define USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER 1u
#else
#define USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER 0u
#endif

#else
#define USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER 0u

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (defined USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)

static inline uint8_t USB_DeviceLpcIp3511MaxPacketNeedCopy(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
#if (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY == (USB_DEVICE_CONFIG_LPCIP3511HS + USB_DEVICE_CONFIG_LPCIP3511FS))
    return 1u;
#elif (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY == USB_DEVICE_CONFIG_LPCIP3511HS)
    return (lpc3511IpState->controllerSpeed);
#elif (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY == USB_DEVICE_CONFIG_LPCIP3511FS)
#if (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
    if (lpc3511IpState->controllerSpeed)
    {
        return 0u;
    }
    else
    {
        return 1u;
    }
#else
    return 1u;
#endif
#endif
}

static uint8_t *USB_DeviceLpcIp3511MallocMaxPacketBuffer(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                         uint32_t multile64)
{
    uint32_t bitsIndex;
    uint8_t numIndex;
    OSA_SR_ALLOC();

    multile64 = ((multile64 + 63) / 64);
    bitsIndex = 0;
    OSA_ENTER_CRITICAL();
    do
    {
        numIndex = 0;
        for (; numIndex < multile64; ++numIndex)
        {
            if (bitsIndex >= USB_DEVICE_IP3511_BITS_FOR_RESERVED_BUFFER)
            {
                OSA_EXIT_CRITICAL();
                return NULL; /* fail */
            }
            if (lpc3511IpState->epReservedBufferBits[(bitsIndex / 8)] &
                (uint8_t)(0x01u << (bitsIndex & 0x00000007u))) /* has allocated */
            {
                bitsIndex++;
                break;
            }
            bitsIndex++;
        }
    } while (numIndex < multile64);

    if (numIndex >= multile64)
    {
        /* set the bits */
        for (numIndex = 0; numIndex < multile64; ++numIndex)
        {
            lpc3511IpState->epReservedBufferBits[((bitsIndex - multile64 + numIndex) / 8)] |=
                (uint8_t)(0x01u << ((bitsIndex - multile64 + numIndex) & 0x00000007u));
        }
        OSA_EXIT_CRITICAL();
        return lpc3511IpState->epReservedBuffer + ((bitsIndex - multile64) * 64);
    }
    else
    {
        OSA_EXIT_CRITICAL();
        return NULL;
    }
}

static void USB_DeviceLpcIp3511ReleaseMaxPacketBuffer(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                      uint8_t *buffer,
                                                      uint32_t bufferSize)
{
    uint32_t bitsIndex;
    uint8_t bitsNum;
    OSA_SR_ALLOC();

    if ((buffer < lpc3511IpState->epReservedBuffer) ||
        (buffer >= (lpc3511IpState->epReservedBuffer + USB_DEVICE_IP3511_ENDPOINT_RESERVED_BUFFER_SIZE)))
    {
        return;
    }
    bitsIndex = (buffer - lpc3511IpState->epReservedBuffer) / 64;

    OSA_ENTER_CRITICAL();
    for (bitsNum = 0; bitsNum < ((bufferSize + 63) / 64); ++bitsNum)
    {
        lpc3511IpState->epReservedBufferBits[((bitsIndex + bitsNum) / 8)] &=
            (uint8_t)(~(0x01u << ((bitsIndex + bitsNum) & 0x00000007u))); /* clear the bit */
    }
    OSA_EXIT_CRITICAL();
}
#endif

static usb_device_lpc3511ip_endpoint_state_struct_t *USB_DeviceLpc3511IpGetEndpointStateStruct(
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState, uint8_t endpointIndex)
{
    if (endpointIndex <= (USB_DEVICE_IP3511_ENDPOINTS_NUM * 2))
    {
        return &(lpc3511IpState->endpointState[endpointIndex]);
    }

    return NULL;
}

/*!
 * @brief Write the command/status entry to start a transfer.
 *
 * The function is used to start a transfer by writing the command/status entry.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 * @param endpoint         Endpoint number.
 * @param direction        The direction of the endpoint, 0U - USB_OUT, 1U - USB_IN.
 * @param buffer           The memory address to save the received data, or the memory address to hold the data need to
 * be sent.
 * @param length           The length of the data.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpEndpointPrime(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                     usb_device_lpc3511ip_endpoint_state_struct_t *epState,
                                                     uint8_t endpointIndex,
                                                     uint8_t *buffer,
                                                     uint32_t length)
{
    uint8_t odd;

    OSA_SR_ALLOC();

    /* Enter critical */
    OSA_ENTER_CRITICAL();

    /* Flag the endpoint is busy. */
    epState->stateUnion.stateBitField.transferring = 1U;

    /* update the endpoint status */
    epState->transferPrimedLength += length;
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
    if ((endpointIndex >> 1U) != USB_ENDPOINT_CONTROL)
    {
        odd = epState->stateUnion.stateBitField.producerOdd;
        epState->stateUnion.stateBitField.doubleBufferBusy++;
        epState->stateUnion.stateBitField.producerOdd ^= 1;
    }
    else
#endif
    {
        odd = 0U;
    }
    epState->epBufferStatusUnion[odd].epBufferStatusField.transactionLength = length;

    /* when receive the zero length packet, the controller will set 4 bytes buffer as 0x00 */
    if (buffer == NULL)
    {
        buffer = lpc3511IpState->zeroTransactionData;
    }

    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(
        lpc3511IpState, endpointIndex, odd,
        (epState->stateUnion.stateBitField.epControlDefault << USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT) |
            USB_LPC3511IP_ENDPOINT_ACTIVE_MASK,
        length, (uint32_t)buffer);
    if (epState->stateUnion.stateBitField.epControlDefault &
        ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT))
    {
        epState->stateUnion.stateBitField.epControlDefault &=
            (~((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT));
    }
    /* Exit critical */
    OSA_EXIT_CRITICAL();
    return kStatus_USB_Success;
}

#if 0
/*!
 * @brief Prime a next setup transfer.
 *
 * The function is used to prime a buffer in control out pipe to wait for receiving the host's setup packet.
 *
 * @param lpc3511IpState       Pointer of the controller state structure.
 *
 */
static void USB_DeviceLpc3511IpPrimeNextSetup(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpc3511IpState, 0, 1, 0, 8, lpc3511IpState->setupData);
}
#endif

/*!
 * @brief reset ip3511.
 *
 * @param lpc3511IpState       Pointer of the controller state structure.
 *
 */
static void USB_DeviceLpc3511IpSetDefaultState(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    uint32_t index = 0;
    uint8_t usbAddress;

    /* zero the command/status list buffer and disable all endpoints */
    for (index = 0; index < 4; ++index)
    {
        lpc3511IpState->epCommandStatusList[index] = 0x00000000U;
    }
    for (index = 4; index < USB_DEVICE_IP3511_ENDPOINTS_NUM * 4; ++index)
    {
        lpc3511IpState->epCommandStatusList[index] = USB_LPC3511IP_ENDPOINT_DISABLE_MASK;
    }

    /* set address as 0 */
    usbAddress = 0U;
    USB_DeviceLpc3511IpControl(lpc3511IpState, kUSB_DeviceControlPreSetDeviceAddress, &usbAddress);

    lpc3511IpState->registerBase->EPLISTSTART = (uint32_t)lpc3511IpState->epCommandStatusList;
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if (lpc3511IpState->controllerSpeed)
    {
        if ((USBHSD_DATABUFSTART_DA_BUF_MASK & (uint32_t)lpc3511IpState->setupData) !=
            lpc3511IpState->registerBase->DATABUFSTART)
        {
            /* please use the dedicated ram */
        }
    }
    else
#endif
    {
        /* all data buffer is in the same 4M range with this setup data buffer */
        lpc3511IpState->registerBase->DATABUFSTART = (uint32_t)lpc3511IpState->setupData;
    }
    /* reset registers */
    lpc3511IpState->registerBase->EPINUSE = 0x0;
    lpc3511IpState->registerBase->EPSKIP  = 0x0;
/* enable all double-buffer */
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
    lpc3511IpState->registerBase->EPBUFCFG = USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK;
#else
    lpc3511IpState->registerBase->EPBUFCFG = 0x00000000u;
#endif
    /* clear interrupts
     * don't clear DEV_INT because the vbus valid interrupt may occurs with keeping usb connected and reseting device.
     */
    lpc3511IpState->registerBase->INTSTAT =
        (USB_LPC3511IP_INTSTAT_FRAME_INT_MASK | USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK);
    /* enable interrupts */
    lpc3511IpState->registerBase->INTEN = USB_LPC3511IP_INTSTAT_DEV_INT_MASK | USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK;

    /* Clear reset flag */
    lpc3511IpState->isResetting = 0U;
}

/* Config and Enable endpoint */
static usb_status_t USB_DeviceLpc3511IpEndpointInit(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                    usb_device_endpoint_init_struct_t *epInit)
{
    uint8_t endpointIndex = USB_LPC3511IP_ENDPOINT_DES_INDEX(epInit->endpointAddress);
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);
    uint16_t maxPacketSize = epInit->maxPacketSize;

    /* clear the endpoint status bits */
    epState->stateUnion.state = 0x00000000u;
    lpc3511IpState->registerBase->EPINUSE &= (~(0x01u << endpointIndex));
    /* Save the max packet size of the endpoint */
    epState->stateUnion.stateBitField.maxPacketSize = maxPacketSize;
    /* Set the ZLT field */
    epState->stateUnion.stateBitField.zlt          = epInit->zlt;
    epState->stateUnion.stateBitField.endpointType = epInit->transferType;

    /* get the endpoint default control value */
    if (USB_ENDPOINT_ISOCHRONOUS == epInit->transferType)
    {
        epState->stateUnion.stateBitField.epControlDefault =
            (USB_LPC3511IP_ENDPOINT_ENDPOINT_TYPE_MASK >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT);
    }
#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U))
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    else if (
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
        (!(lpc3511IpState->hsInterruptIssue)) &&
#endif
        (lpc3511IpState->controllerSpeed) && (USB_ENDPOINT_INTERRUPT == epInit->transferType))
    {
        epState->stateUnion.stateBitField.epControlDefault =
            ((USB_LPC3511IP_ENDPOINT_ENDPOINT_TYPE_MASK | USB_LPC3511IP_ENDPOINT_RFTV_MASK) >>
             USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT);
    }
#endif
#endif
    else
    {
        epState->stateUnion.stateBitField.epControlDefault = 0x00U;
    }
    /* set the command/status value */
    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(
        lpc3511IpState, endpointIndex, 0,
        (epState->stateUnion.stateBitField.epControlDefault << USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT), 0, 0);
    if ((epInit->endpointAddress & USB_ENDPOINT_NUMBER_MASK) == USB_CONTROL_ENDPOINT)
    {
        if (!(epInit->endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            /* Prime setup packet when the endpoint is control out endpoint. */
            USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpc3511IpState, 0, 1, 0, 0, (uint32_t)lpc3511IpState->setupData);
        }
    }
    else
    {
        USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(
            lpc3511IpState, endpointIndex, 1,
            (epState->stateUnion.stateBitField.epControlDefault << USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT), 0, 0);
    }
    if ((endpointIndex >> 1) != USB_CONTROL_ENDPOINT)
    {
        /* toggle reset for the toggle */
        epState->stateUnion.stateBitField.epControlDefault |=
            ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT);
    }

    epState->epPacketBuffer = NULL;
    if ((endpointIndex >> 1U) == USB_CONTROL_ENDPOINT) /* control endpoint */
    {
        epState->epPacketBuffer = lpc3511IpState->controlData;
    }
#if (defined USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)
    else
    {
        if (USB_DeviceLpcIp3511MaxPacketNeedCopy(lpc3511IpState))
        {
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
            uint8_t *maxPacketBuffer = USB_DeviceLpcIp3511MallocMaxPacketBuffer(
                lpc3511IpState, USB_LPC3511IP_GET_MULTIPLE_OF_64(maxPacketSize) * 2);
#else
            uint8_t *maxPacketBuffer = USB_DeviceLpcIp3511MallocMaxPacketBuffer(lpc3511IpState, maxPacketSize);
#endif
            if (maxPacketBuffer == NULL)
            {
                return kStatus_USB_AllocFail;
            }
            epState->epPacketBuffer = maxPacketBuffer;
        }
    }
#endif

    return kStatus_USB_Success;
}

/*!
 * @brief De-initialize a specified endpoint.
 *
 * The function is used to de-initialize a specified endpoint.
 * Current transfer of the endpoint will be canceled and the specified endpoint will be disabled.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 * @param ep               The endpoint address, Bit7, 0U - USB_OUT, 1U - USB_IN.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpEndpointDeinit(usb_device_lpc3511ip_state_struct_t *lpc3511IpState, uint8_t ep)
{
    uint8_t endpointIndex = USB_LPC3511IP_ENDPOINT_DES_INDEX(ep);
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);

    /* Cancel the transfer of the endpoint */
    USB_DeviceLpc3511IpCancel(lpc3511IpState, ep);

#if (defined USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)
    if (USB_DeviceLpcIp3511MaxPacketNeedCopy(lpc3511IpState))
    {
        if ((endpointIndex >> 1U) != USB_CONTROL_ENDPOINT) /* control endpoint */
        {
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
            USB_DeviceLpcIp3511ReleaseMaxPacketBuffer(
                lpc3511IpState, epState->epPacketBuffer,
                USB_LPC3511IP_GET_MULTIPLE_OF_64(epState->stateUnion.stateBitField.maxPacketSize) * 2);
#else
            USB_DeviceLpcIp3511ReleaseMaxPacketBuffer(lpc3511IpState, epState->epPacketBuffer,
                                                      epState->stateUnion.stateBitField.maxPacketSize);
#endif
        }
        epState->epPacketBuffer = NULL;
    }
#endif

    /* reset the double buffer */
    lpc3511IpState->registerBase->EPINUSE &= ~(0x01U << endpointIndex);
    /* Disable the endpoint */
    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpc3511IpState, endpointIndex, 0, USB_LPC3511IP_ENDPOINT_DISABLE_MASK, 0, 0);
    /* Clear the max packet size */
    epState->stateUnion.stateBitField.maxPacketSize = 0U;

    return kStatus_USB_Success;
}

/*!
 * @brief Stall a specified endpoint.
 *
 * The function is used to stall a specified endpoint.
 * Current transfer of the endpoint will be canceled and the specified endpoint will be stalled.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 * @param ep               The endpoint address, Bit7, 0U - USB_OUT, 1U - USB_IN.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpEndpointStall(usb_device_lpc3511ip_state_struct_t *lpc3511IpState, uint8_t ep)
{
    uint8_t endpointIndex = USB_LPC3511IP_ENDPOINT_DES_INDEX(ep);
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);

    /* Set endpoint stall flag. */
    epState->stateUnion.stateBitField.stalled = 1U;
    /* lpc3511IpState->registerBase->EPINUSE &= (~(0x01u << endpointIndex)); */
    /* stall the endpoint */
    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpc3511IpState, endpointIndex, 0, USB_LPC3511IP_ENDPOINT_STALL_MASK, 0, 0);
    if ((ep & USB_ENDPOINT_NUMBER_MASK) != USB_CONTROL_ENDPOINT)
    {
        /* toggle reset for the toggle */
        epState->stateUnion.stateBitField.epControlDefault |=
            ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT);
        USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(lpc3511IpState, endpointIndex, 1, USB_LPC3511IP_ENDPOINT_STALL_MASK, 0, 0);
    }
#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)) && \
    (!(defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) &&            \
       (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK)))
#else
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
    if (lpc3511IpState->hsInterruptIssue)
    {
#endif
        if ((lpc3511IpState->controllerSpeed) &&
            (USB_ENDPOINT_INTERRUPT == epState->stateUnion.stateBitField.endpointType))
        {
            lpc3511IpState->registerBase->DEVCMDSTAT |=
                (USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AO_MASK | USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AI_MASK);
            epState->stateUnion.stateBitField.epControlDefault &=
                (~((USB_LPC3511IP_ENDPOINT_ENDPOINT_TYPE_MASK | USB_LPC3511IP_ENDPOINT_RFTV_MASK) >>
                   USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT));
        }
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
    }
#endif
#endif
#endif

    /* cancel the transfer in the endpoint */
    USB_DeviceLpc3511IpCancel(lpc3511IpState, ep);
    return kStatus_USB_Success;
}

/*!
 * @brief Un-stall a specified endpoint.
 *
 * The function is used to un-stall a specified endpoint.
 * Current transfer of the endpoint will be canceled and the specified endpoint will be un-stalled.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 * @param ep               The endpoint address, Bit7, 0U - USB_OUT, 1U - USB_IN.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpEndpointUnstall(usb_device_lpc3511ip_state_struct_t *lpc3511IpState, uint8_t ep)
{
    uint8_t endpointIndex = USB_LPC3511IP_ENDPOINT_DES_INDEX(ep);
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);

    /* Clear the endpoint stall state, the hardware resets the endpoint
     * toggle to one for both directions when a setup token is received */
    epState->stateUnion.stateBitField.stalled = 0U;

    /* unstall the endpoint for double buffers */
    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(lpc3511IpState, endpointIndex, 0, (~USB_LPC3511IP_ENDPOINT_STALL_MASK));
    if ((ep & USB_ENDPOINT_NUMBER_MASK) != USB_CONTROL_ENDPOINT)
    {
        USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(lpc3511IpState, endpointIndex, 1, (~USB_LPC3511IP_ENDPOINT_STALL_MASK));

        /* toggle reset for the toggle */
        epState->stateUnion.stateBitField.epControlDefault |=
            ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT);
#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)) && \
    (!(defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) &&            \
       (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK)))
#else
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
        if (lpc3511IpState->hsInterruptIssue)
        {
#endif
            if ((lpc3511IpState->controllerSpeed) &&
                (USB_ENDPOINT_INTERRUPT == epState->stateUnion.stateBitField.endpointType))
            {
                epState->stateUnion.stateBitField.epControlDefault &=
                    (~((USB_LPC3511IP_ENDPOINT_ENDPOINT_TYPE_MASK | USB_LPC3511IP_ENDPOINT_RFTV_MASK) >>
                       USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT));
            }
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
        }
#endif
#endif
#endif
    }

    if (epState->stateUnion.stateBitField.stallPrimed)
    {
        epState->stateUnion.stateBitField.stallPrimed = 0u;
        USB_DeviceLpc3511IpTransaction(lpc3511IpState, epState, endpointIndex);
    }
    /* cancel the transfer in the endpoint */
    USB_DeviceLpc3511IpCancel(lpc3511IpState, ep);
    return kStatus_USB_Success;
}

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
/*!
 * @brief Un-stall a specified endpoint.
 *
 * The function is used to un-stall a specified endpoint.
 * Current transfer of the endpoint will be canceled and the specified endpoint will be un-stalled.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpInterruptSuspend(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    usb_device_callback_message_struct_t message;

    message.buffer  = (uint8_t *)NULL;
    message.code    = kUSB_DeviceNotifySuspend;
    message.length  = 0U;
    message.isSetup = 0U;

    /* Notify up layer the USB suspend signal detected. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);

    return kStatus_USB_Success;
}

/*!
 * @brief Un-stall a specified endpoint.
 *
 * The function is used to un-stall a specified endpoint.
 * Current transfer of the endpoint will be canceled and the specified endpoint will be un-stalled.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpInterruptResume(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    usb_device_callback_message_struct_t message;

    message.buffer  = (uint8_t *)NULL;
    message.code    = kUSB_DeviceNotifyResume;
    message.length  = 0U;
    message.isSetup = 0U;

    /* Notify up layer the USB suspend signal detected. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);

    return kStatus_USB_Success;
}
#if (defined(USB_DEVICE_CONFIG_LPM_L1) && (USB_DEVICE_CONFIG_LPM_L1 > 0U))
/*!
 * @brief Un-stall a specified endpoint.
 *
 * The function is used to un-stall a specified endpoint.
 * Current transfer of the endpoint will be canceled and the specified endpoint will be un-stalled.
 *
 * @param lpc3511IpState      Pointer of the controller state structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceLpc3511IpInterruptLPMSleep(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    usb_device_callback_message_struct_t message;
    uint8_t remotewakeup;

    message.buffer  = (uint8_t *)NULL;
    message.code    = kUSB_DeviceNotifyLPMSleep;
    message.length  = 0U;
    message.isSetup = 0U;

    remotewakeup = (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_MASK) >>
                   USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_SHIFT;
    USB_DeviceSetStatus(lpc3511IpState->deviceHandle, kUSB_DeviceStatusRemoteWakeup, &remotewakeup);

    /* Notify up layer the USB suspend signal detected. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);

    return kStatus_USB_Success;
}

#endif
#endif

/* need copy the data before the trasaction buffer is used again */
static void USB_DeviceLpc3511IpDoPreviousTransactionMemcpy(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                           usb_device_lpc3511ip_endpoint_state_struct_t *epState,
                                                           uint32_t length,
                                                           uint8_t endpointIndex,
                                                           uint8_t odd)
{
    uint8_t *destBuffer;
    uint8_t *sourceBuffer;

#if ((defined(USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY > 0U))
    /*control out doesn't support buffer toggle*/
    if (0U == endpointIndex)
    {
        odd = 0u;
    }
#if USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER
    if ((epState->epBufferStatusUnion[odd].epBufferStatusField.epPacketCopyed) && (length > 0U) &&
        ((endpointIndex & 0x01u) == 0U))
#else
    if (USB_DeviceLpcIp3511MaxPacketNeedCopy(lpc3511IpState) &&
        (epState->epBufferStatusUnion[odd].epBufferStatusField.epPacketCopyed) && (length > 0U) &&
        ((endpointIndex & 0x01u) == 0U))
#endif
#else
    /* control data buffer align is used */
    if (((endpointIndex >> 1U) == USB_CONTROL_ENDPOINT) &&
        (epState->epBufferStatusUnion[odd].epBufferStatusField.epPacketCopyed) && (length > 0U) &&
        ((endpointIndex & 0x01u) == 0U))
#endif
    {
#if ((defined(USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER)) && (USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER > 0U))
        destBuffer = &(epState->transferBuffer[epState->transferDone - length]);
#else
        destBuffer   = &(epState->transferBuffer[epState->transferDone]);
#endif
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
        sourceBuffer = epState->epPacketBuffer +
                       odd * USB_LPC3511IP_GET_MULTIPLE_OF_64(epState->stateUnion.stateBitField.maxPacketSize);
#else
        sourceBuffer = epState->epPacketBuffer;
#endif
        memcpy(destBuffer, sourceBuffer, length);
    }
}

static uint32_t USB_DeviceLpc3511IpTokenUpdate(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                               usb_device_lpc3511ip_endpoint_state_struct_t *epState,
                                               uint8_t endpointIndex,
                                               uint8_t changedOdd)
{
    uint32_t length;
    uint8_t odd;

#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
    if (changedOdd)
    {
        odd = epState->stateUnion.stateBitField.consumerOdd;
        epState->stateUnion.stateBitField.consumerOdd ^= 1;
        epState->stateUnion.stateBitField.doubleBufferBusy--;
    }
    else
#endif
    {
        odd = 0U;
    }

/* for OUT packet, compute the actual packet size. */
#if ((defined(FSL_FEATURE_USB_VERSION) && (FSL_FEATURE_USB_VERSION >= 200U)) || \
     (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)))
#else
    if ((endpointIndex & 0x01U) == 0x00u) /* OUT */
#endif
    {
        /* get the transaction length */
        length = *(lpc3511IpState->epCommandStatusList + endpointIndex * 2 + odd);

#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
        if (lpc3511IpState->controllerSpeed)
        {
            length =
                (length & USB_LPC3511IPHS_ENDPOINT_BUFFER_NBYTES_MASK) >> USB_LPC3511IPHS_ENDPOINT_BUFFER_NBYTES_SHIFT;
        }
        else
#endif
        {
            length =
                (length & USB_LPC3511IPFS_ENDPOINT_BUFFER_NBYTES_MASK) >> USB_LPC3511IPFS_ENDPOINT_BUFFER_NBYTES_SHIFT;
        }
        length = epState->epBufferStatusUnion[odd].epBufferStatusField.transactionLength - length;
    }
#if ((defined(FSL_FEATURE_USB_VERSION) && (FSL_FEATURE_USB_VERSION >= 200U)) || \
     (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)))
#else
    else /* for IN packet, if there is no error, the packet lenght is the primed length. */
    {
        /* don't judge the actual packet size */
        length = epState->epBufferStatusUnion[odd].epBufferStatusField.transactionLength;
    }
#endif

#if !((defined(USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER)) && (USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER > 0U))
    USB_DeviceLpc3511IpDoPreviousTransactionMemcpy(lpc3511IpState, epState, length, endpointIndex, odd);
#endif
    /* update the transferred length */
    epState->transferDone += length;

    return length;
}

static void USB_DeviceLpc3511IpInterruptToken(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                              uint8_t endpointIndex,
                                              uint8_t isSetup,
                                              uint32_t errorStatus)
{
    usb_device_callback_message_struct_t message;
    uint32_t len = 0;
    uint32_t length;
    uint32_t remainLength;
    usb_setup_struct_t *setupPacket;
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);

#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)) && \
    (!(defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) &&            \
       (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK)))
#else
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
    if (lpc3511IpState->hsInterruptIssue)
    {
#endif
        if ((epState->stateUnion.stateBitField.epControlDefault &
             ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT)) &&
            (USB_ENDPOINT_INTERRUPT == epState->stateUnion.stateBitField.endpointType) &&
            (lpc3511IpState->controllerSpeed) &&
            (lpc3511IpState->epCommandStatusList[endpointIndex * 2 + epState->stateUnion.stateBitField.consumerOdd] &
             USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK))
        {
            if (!(lpc3511IpState->registerBase->EPTOGGLE & (0x01u << endpointIndex)))
            {
                uint32_t index;
                length = 0;
                for (index = 0; index < (USB_DEVICE_IP3511_ENDPOINTS_NUM)*4; ++index)
                {
                    if ((lpc3511IpState->epCommandStatusList[index] & USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) &&
                        (USB_ENDPOINT_INTERRUPT ==
                         lpc3511IpState->endpointState[index / 2].stateUnion.stateBitField.endpointType))
                    {
                        length++;
                    }
                }

                if (length <= 1)
                {
                    lpc3511IpState->registerBase->DEVCMDSTAT &=
                        ~(USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AO_MASK | USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AI_MASK);
                }
                epState->stateUnion.stateBitField.epControlDefault &=
                    (~((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT));
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
                /* high-speed */
                epState->stateUnion.stateBitField.epControlDefault |=
                    ((USB_LPC3511IP_ENDPOINT_RFTV_MASK | USB_LPC3511IP_ENDPOINT_ENDPOINT_TYPE_MASK) >>
                     USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT);
                USB_DeviceLpc3511IpTransaction(lpc3511IpState, epState, endpointIndex);
#endif
            }
            return;
        }
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
    }
#endif
#endif
#endif

    if ((!isSetup) && (0U == epState->stateUnion.stateBitField.transferring))
    {
        return;
    }
    if (isSetup)
    {
        message.length = 8U;
        message.buffer = (lpc3511IpState->setupData);
        /* clear the primed control transactions */
        if (epState->stateUnion.stateBitField.transferring)
        {
            epState->stateUnion.stateBitField.transferring = 0U;
            if (lpc3511IpState->epCommandStatusList[0] & USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)
            {
                USB_DeviceLpc3511IpCancel(lpc3511IpState, USB_CONTROL_ENDPOINT);
            }
        }
        if (lpc3511IpState->endpointState[1].stateUnion.stateBitField.transferring)
        {
            lpc3511IpState->endpointState[1].stateUnion.stateBitField.transferring = 0U;
            if (lpc3511IpState->epCommandStatusList[2] & USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)
            {
                USB_DeviceLpc3511IpCancel(lpc3511IpState, (0x80u | USB_CONTROL_ENDPOINT));
            }
        }

        USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(
            lpc3511IpState, 0, 0, (~(USB_LPC3511IP_ENDPOINT_STALL_MASK | USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)));
        USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(
            lpc3511IpState, 1, 0, (~(USB_LPC3511IP_ENDPOINT_STALL_MASK | USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)));

        lpc3511IpState->registerBase->INTSTAT = 0x03u; /* clear interrupt */
        /* W1 to clear the setup flag */
        lpc3511IpState->registerBase->DEVCMDSTAT |= USB_LPC3511IP_DEVCMDSTAT_SETUP_MASK;
    }
    else
    {
        length = 0;
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
        if (lpc3511IpState->epCommandStatusList[endpointIndex * 2 + epState->stateUnion.stateBitField.consumerOdd] &
            USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)
        {
            return;
        }
#else
        if (lpc3511IpState->epCommandStatusList[endpointIndex * 2] & USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)
        {
            return;
        }
#endif

#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
        if ((endpointIndex >> 1U) != USB_CONTROL_ENDPOINT)
        {
            len = USB_DeviceLpc3511IpTokenUpdate(lpc3511IpState, epState, endpointIndex, 1U);
            length += len;

            if ((epState->stateUnion.stateBitField.doubleBufferBusy > 0) &&
                (!(lpc3511IpState
                       ->epCommandStatusList[endpointIndex * 2 + epState->stateUnion.stateBitField.consumerOdd] &
                   USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)))
            {
#if ((defined(USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER)) && (USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER > 0U))
                USB_DeviceLpc3511IpDoPreviousTransactionMemcpy(lpc3511IpState, epState, len, endpointIndex,
                                                               epState->stateUnion.stateBitField.consumerOdd ^ 1);
#endif
                len = USB_DeviceLpc3511IpTokenUpdate(lpc3511IpState, epState, endpointIndex, 1U);
                length += len;
            }
        }
        else
#endif
        {
            length = USB_DeviceLpc3511IpTokenUpdate(lpc3511IpState, epState, endpointIndex, 0U);
            len    = length;
        }

        /* update remaining length */
        remainLength = epState->transferLength - epState->transferDone;

        /* Whether the transfer is completed or not.
         * The transfer is completed when one of the following conditions meet:
         * 1. The remaining length is zero.
         * 2. The length of current tansaction is not the multiple of max packet size.
         */
        if ((length > 0U) && (!(length % epState->stateUnion.stateBitField.maxPacketSize)) && (remainLength > 0U))
        {
            (void)USB_DeviceLpc3511IpTransaction(lpc3511IpState, epState, endpointIndex);
#if ((defined(USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER)) && (USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER > 0U))
            USB_DeviceLpc3511IpDoPreviousTransactionMemcpy(lpc3511IpState, epState, len, endpointIndex,
                                                           epState->stateUnion.stateBitField.consumerOdd ^ 1);
#endif
            return;
        }
        else
        {
            epState->stateUnion.stateBitField.transferring = 0U;
            message.length                                 = epState->transferDone;
            message.buffer                                 = epState->transferBuffer;

            /* process ZLT
             * 1. IN endpoint;
             * 2. transfer length is the multiple of max packet size.
             */
            if ((endpointIndex & 0x01U) && (length) && (!(length % epState->stateUnion.stateBitField.maxPacketSize)))
            {
                if ((endpointIndex >> 1U) == USB_CONTROL_ENDPOINT)
                {
                    setupPacket = (usb_setup_struct_t *)(&(lpc3511IpState->setupData[0]));
                    /*
                     * Send ZLT transaction if setup transfer and the required length is longer than actual length
                     */
                    if (USB_SHORT_FROM_LITTLE_ENDIAN(setupPacket->wLength) > epState->transferLength)
                    {
                        (void)USB_DeviceLpc3511IpEndpointPrime(lpc3511IpState, epState, 1U, NULL, 0U);
                        return;
                    }
                }
                else if ((epState->stateUnion.stateBitField.zlt))
                {
                    (void)USB_DeviceLpc3511IpEndpointPrime(lpc3511IpState, epState, endpointIndex, NULL, 0U);
                    return;
                }
                else
                {
                }
            }
        }
    }

    message.isSetup = isSetup;
    message.code    = ((uint8_t)(endpointIndex >> 1) | (uint8_t)(((uint32_t)(endpointIndex & 0x01U) << 0x07U)));

#if ((defined(USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER)) && (USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER > 0U))
    USB_DeviceLpc3511IpDoPreviousTransactionMemcpy(lpc3511IpState, epState, len, endpointIndex,
                                                   epState->stateUnion.stateBitField.consumerOdd ^ 1);
#endif
    /* Notify the up layer the controller status changed. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);
}

/*!
 * @brief Handle the USB bus reset interrupt.
 *
 * The function is used to handle the USB bus reset interrupt.
 *
 * @param lpc3511IpState       Pointer of the controller state structure.
 *
 */
static void USB_DeviceLpc3511IpInterruptReset(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    usb_device_callback_message_struct_t message;

    /* Set reset flag */
    lpc3511IpState->isResetting = 1U;

#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if (lpc3511IpState->controllerSpeed)
    {
        if (((lpc3511IpState->registerBase->DEVCMDSTAT & USBHSD_DEVCMDSTAT_Speed_MASK) >>
             USBHSD_DEVCMDSTAT_Speed_SHIFT) == 0x02U)
        {
            lpc3511IpState->deviceSpeed = USB_SPEED_HIGH;
        }
        else if (((lpc3511IpState->registerBase->DEVCMDSTAT & USBHSD_DEVCMDSTAT_Speed_MASK) >>
                  USBHSD_DEVCMDSTAT_Speed_SHIFT) == 0x01U)
        {
            lpc3511IpState->deviceSpeed = USB_SPEED_FULL;
        }
        else
        {
        }
    }
    else
#endif
    {
        lpc3511IpState->deviceSpeed = USB_SPEED_FULL;
    }

    message.buffer  = (uint8_t *)NULL;
    message.code    = kUSB_DeviceNotifyBusReset;
    message.length  = 0U;
    message.isSetup = 0U;
    /* Notify up layer the USB bus reset signal detected. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);
}

#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE))
/*!
 * @brief Handle detach interrupt.
 *
 * The function is used to handle the detach interrupt.
 *
 * @param lpc3511IpState       Pointer of the controller state structure.
 *
 */
static void USB_DeviceLpc3511IpInterruptDetach(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    usb_device_callback_message_struct_t message;

    message.buffer  = (uint8_t *)NULL;
    message.code    = kUSB_DeviceNotifyDetach;
    message.length  = 0U;
    message.isSetup = 0U;

    /* Notify up layer the USB VBUS falling signal detected. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);
}

/*!
 * @brief Handle Attach interrupt.
 *
 * The function is used to handle the attach interrupt.
 *
 * @param lpc3511IpState       Pointer of the controller state structure.
 *
 */
static void USB_DeviceLpc3511IpInterruptAttach(usb_device_lpc3511ip_state_struct_t *lpc3511IpState)
{
    usb_device_callback_message_struct_t message;

    message.buffer  = (uint8_t *)NULL;
    message.code    = kUSB_DeviceNotifyAttach;
    message.length  = 0U;
    message.isSetup = 0U;

    /* Notify up layer the USB VBUS rising signal detected. */
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);
}
#endif
#if (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U)) && \
    (defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U))
/* The device dcd callback */
static usb_hsdcd_status_t USB_DeviceLpcIp3511IsrHSDCDCallback(void *handle, uint32_t event, void *param)
{
    usb_hsdcd_status_t error = kStatus_hsdcd_Success;
    usb_device_callback_message_struct_t message;
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)handle;

    if (lpc3511IpState == NULL)
    {
        return kStatus_hsdcd_Error;
    }

    /*messsgae buffer contain event information*/
    message.buffer  = (uint8_t *)param;
    message.length  = 0U;
    message.isSetup = 0U;
    message.code    = kUSB_DeviceNotifyDcdDetectFinished;
    USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);
    return error;
}

void USB_DeviceLpcIp3511IsrDCDFunction(void *deviceHandle)
{
    usb_device_struct_t *handle = (usb_device_struct_t *)deviceHandle;
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState;
    if (NULL == deviceHandle)
    {
        return;
    }
    lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)(handle->controllerHandle);
    USB_HSDcdIsrFunction(lpc3511IpState->dcdHandle);
}
#endif
usb_status_t USB_DeviceLpc3511IpInit(uint8_t controllerId,
                                     usb_device_handle handle,
                                     usb_device_controller_handle *controllerHandle)
{
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState = NULL;
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    uint32_t ip3511FsBases[] = USB_BASE_ADDRS;
#endif
    uint32_t *endpointListArray[] = LPC_CONTROLLER_ENDPOINT_LIST_ARRAY;

#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U)) && \
    (defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U))
    uint32_t hsdcd_base[] = USBHSDCD_BASE_ADDRS;
    USBHSDCD_Type *base;
    usb_hsdcd_config_struct_t dcdParamConfig;
    usb_hsdcd_status_t dcdError = kStatus_hsdcd_Success;
#endif

    uint32_t ip3511HsBases[] = USBHSD_BASE_ADDRS;
    if ((controllerId >= kUSB_ControllerLpcIp3511Hs0) && (controllerId <= kUSB_ControllerLpcIp3511Hs1))
    {
        if ((uint32_t)(controllerId - kUSB_ControllerLpcIp3511Hs0) >= (sizeof(ip3511HsBases) / sizeof(uint32_t)))
        {
            return kStatus_USB_ControllerNotFound;
        }
        lpc3511IpState =
            &s_UsbDeviceLpc3511IpState[controllerId - kUSB_ControllerLpcIp3511Hs0 + USB_DEVICE_CONFIG_LPCIP3511FS];
        lpc3511IpState->controlData =
            (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Hs0 +
                                                 USB_DEVICE_CONFIG_LPCIP3511FS][CONTROL_TRANSFER_DATA_OFFSET];
        lpc3511IpState->setupData =
            (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Hs0 +
                                                 USB_DEVICE_CONFIG_LPCIP3511FS][SETUP_TRANSFER_DATA_OFFSET];
        lpc3511IpState->zeroTransactionData =
            (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Hs0 +
                                                 USB_DEVICE_CONFIG_LPCIP3511FS][ZERO_TRANSFER_DATA_OFFSET];
        /* set the endpoint list */
        lpc3511IpState->epCommandStatusList =
            endpointListArray[controllerId - kUSB_ControllerLpcIp3511Hs0 + USB_DEVICE_CONFIG_LPCIP3511FS];
        /* get the ip base address */
        lpc3511IpState->registerBase = (USB_LPC3511IP_Type *)ip3511HsBases[controllerId - kUSB_ControllerLpcIp3511Hs0];
#if (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U)) && \
    (defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U))
        base                            = (USBHSDCD_Type *)hsdcd_base[controllerId - kUSB_ControllerLpcIp3511Hs0];
        dcdParamConfig.dcdCallback      = USB_DeviceLpcIp3511IsrHSDCDCallback;
        dcdParamConfig.dcdCallbackParam = (void *)lpc3511IpState;
        dcdError                        = USB_HSDCD_Init(base, &dcdParamConfig, &lpc3511IpState->dcdHandle);
        if (kStatus_hsdcd_Success != dcdError)
        {
            return kStatus_USB_Error;
        }
#endif
    }
    else
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    {
        /* get the controller instance */
        if ((controllerId < kUSB_ControllerLpcIp3511Fs0) ||
            ((controllerId - kUSB_ControllerLpcIp3511Fs0) >= (uint8_t)USB_DEVICE_CONFIG_LPCIP3511FS) ||
            ((uint32_t)(controllerId - kUSB_ControllerLpcIp3511Fs0) >= (sizeof(ip3511FsBases) / sizeof(uint32_t))))
        {
            return kStatus_USB_ControllerNotFound;
        }
        lpc3511IpState              = &s_UsbDeviceLpc3511IpState[controllerId - kUSB_ControllerLpcIp3511Fs0];
        lpc3511IpState->controlData = (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Fs0]
                                                                          [CONTROL_TRANSFER_DATA_OFFSET];
        lpc3511IpState->setupData = (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Fs0]
                                                                        [SETUP_TRANSFER_DATA_OFFSET];
        lpc3511IpState->zeroTransactionData =
            (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Fs0][ZERO_TRANSFER_DATA_OFFSET];
        /* set the endpoint list */
        lpc3511IpState->epCommandStatusList = endpointListArray[controllerId - kUSB_ControllerLpcIp3511Fs0];
        /* get the ip base address */
        lpc3511IpState->registerBase = (USB_LPC3511IP_Type *)ip3511FsBases[controllerId - kUSB_ControllerLpcIp3511Fs0];
    }
#else
    {
        return kStatus_USB_ControllerNotFound;
    }
#endif

    lpc3511IpState->controllerId = controllerId;
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if ((lpc3511IpState->controllerId >= kUSB_ControllerLpcIp3511Hs0) &&
        (lpc3511IpState->controllerId <= kUSB_ControllerLpcIp3511Hs1))
    {
        lpc3511IpState->controllerSpeed = 1U;
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
        lpc3511IpState->hsInterruptIssue = ((Chip_GetVersion() == FSL_ROM_VERSION_1B) ? 0 : 1);
#endif
    }
    else
    {
        lpc3511IpState->controllerSpeed = 0U;
    }
#endif

#if ((defined(USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY > 0U))
    /* this controller need max packet buffer copy */
    if (USB_DeviceLpcIp3511MaxPacketNeedCopy(lpc3511IpState))
    {
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
        if ((controllerId >= kUSB_ControllerLpcIp3511Hs0) && (controllerId <= kUSB_ControllerLpcIp3511Hs1))
        {
#if (USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER)
            /* for allocating the max packet buffer */
            lpc3511IpState->epReservedBuffer =
                (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Hs0 +
                                                     USB_DEVICE_CONFIG_LPCIP3511FS][RESERVED_EP_DATA_OFFSET];
#else
            lpc3511IpState->epReservedBuffer =
                (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Hs0]
                                                    [RESERVED_EP_DATA_OFFSET];
#endif
        }
        else
#endif
        {
            lpc3511IpState->epReservedBuffer =
                (uint8_t *)&s_SetupAndEpReservedData[controllerId - kUSB_ControllerLpcIp3511Fs0]
                                                    [RESERVED_EP_DATA_OFFSET];
        }
    }
    for (controllerId = 0; controllerId < ((USB_DEVICE_IP3511_BITS_FOR_RESERVED_BUFFER + 7) / 8); ++controllerId)
    {
        lpc3511IpState->epReservedBufferBits[controllerId] = 0u;
    }
#endif

    /* disable the controller */
    lpc3511IpState->registerBase->DEVCMDSTAT &=
        (~(USB_LPC3511IP_DEVCMDSTAT_DCON_MASK | USB_LPC3511IP_DEVCMDSTAT_DEV_EN_MASK |
           USB_LPC3511IP_DEVCMDSTAT_LPM_SUP_MASK));
    /* reset and enalbe the controller */
    USB_DeviceLpc3511IpSetDefaultState(lpc3511IpState);
    /* enable USB */
    lpc3511IpState->registerBase->DEVCMDSTAT |= (USB_LPC3511IP_DEVCMDSTAT_DEV_EN_MASK
#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
#else
                                                 | USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK
#endif
    );
#if (defined(USB_DEVICE_CONFIG_LPM_L1) && (USB_DEVICE_CONFIG_LPM_L1 > 0U))
    lpc3511IpState->registerBase->DEVCMDSTAT |= USB_LPC3511IP_DEVCMDSTAT_LPM_SUP_MASK;
    lpc3511IpState->registerBase->LPM |= USB_LPC3511IP_USB_LPM_HIRD_SW(4);
    lpc3511IpState->registerBase->DEVCMDSTAT &= ~(USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK);
#endif
    lpc3511IpState->deviceHandle = handle;
    *controllerHandle            = lpc3511IpState;

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceLpc3511IpDeinit(usb_device_controller_handle controllerHandle)
{
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)controllerHandle;
    uint32_t usbAddress;

    if (controllerHandle == NULL)
    {
        return kStatus_USB_InvalidHandle;
    }
    /* Clear all interrupt flags. */
    lpc3511IpState->registerBase->INTSTAT = (USB_LPC3511IP_INTSTAT_DEV_INT_MASK | USB_LPC3511IP_INTSTAT_FRAME_INT_MASK |
                                             USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK);
    /* Disable all interrupts. */
    lpc3511IpState->registerBase->INTEN = 0U;
    /* Clear device address. */
    usbAddress = 0U;
    USB_DeviceLpc3511IpControl(lpc3511IpState, kUSB_DeviceControlPreSetDeviceAddress, &usbAddress);
#if (defined(USB_DEVICE_CONFIG_LPM_L1) && (USB_DEVICE_CONFIG_LPM_L1 > 0U))
    lpc3511IpState->registerBase->DEVCMDSTAT &= ~USB_LPC3511IP_DEVCMDSTAT_LPM_SUP_MASK;
#endif
    /* disable the controller */
    lpc3511IpState->registerBase->DEVCMDSTAT &=
        (~(USB_LPC3511IP_DEVCMDSTAT_DCON_MASK | USB_LPC3511IP_DEVCMDSTAT_DEV_EN_MASK |
           USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK));
#if (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U)) && \
    (defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U))
    USB_HSDCD_Deinit(lpc3511IpState->dcdHandle);
#endif

    return kStatus_USB_Success;
}

static usb_status_t USB_DeviceLpc3511IpGetActualBufferAndPrime(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                               usb_device_lpc3511ip_endpoint_state_struct_t *epState,
                                                               uint8_t endpointIndex,
                                                               uint8_t changedOdd)
{
    uint32_t index;
    uint8_t *destBuffer;
    uint8_t *actualBuffer;
    uint32_t length;
    uint8_t odd;

#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
    if (changedOdd)
    {
        odd = epState->stateUnion.stateBitField.producerOdd;
    }
    else
#endif
    {
        odd = 0;
    }
    actualBuffer = epState->transferBuffer + epState->transferPrimedLength;
    length       = epState->transferLength - epState->transferPrimedLength;
    /* Data length needs to less than max packet size. */
    if (length > epState->stateUnion.stateBitField.maxPacketSize)
    {
        length = epState->stateUnion.stateBitField.maxPacketSize;
    }

    epState->epBufferStatusUnion[odd].epBufferStatusField.epPacketCopyed = 0;

    index = (endpointIndex & 0x01u); /* index mean IN endpoint here */
    if (length > 0U)
    {
#if ((defined(USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY)) && (USB_DEVICE_IP3511_RESERVED_BUFFER_FOR_COPY > 0U))
/* if all the enabled IP support the reserved buffer, then don't need the judgement. */
#if (!USB_DEVICE_IP3511_ALL_IP_SUPPORT_RESERVED_BUFFER)
#define USB_DEVICE_IP3511_NEED_CHECK_BUFFER (1u)
        /* lengt > 0 && ((buffer not align with 64) || (buffer is not in the deticated ram))) */
        if (USB_DeviceLpcIp3511MaxPacketNeedCopy(lpc3511IpState))
#endif
#else
#define USB_DEVICE_IP3511_NEED_CHECK_BUFFER (1u)
        /* align the buffer for control transfer */
        if (((endpointIndex >> 1U) == USB_CONTROL_ENDPOINT))
#endif
        {
            /* not 64 bytes align || not in the dedicated ram || ( OUT && not mutiple of 4 ) */
            if ((((uint32_t)actualBuffer & 0x0000003FU) != 0U) ||
                (((uint32_t)actualBuffer & 0xFFC00000U) != (lpc3511IpState->registerBase->DATABUFSTART & 0xFFC00000U))
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
                || ((lpc3511IpState->controllerSpeed) && (!index) &&
                    (length != epState->stateUnion.stateBitField.maxPacketSize)))
#else
            )
#endif
            {
                epState->epBufferStatusUnion[odd].epBufferStatusField.epPacketCopyed = 1U;
/* usb copy buffer for this packet */
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
                destBuffer =
                    (uint8_t *)(epState->epPacketBuffer + (odd * USB_LPC3511IP_GET_MULTIPLE_OF_64(
                                                                     epState->stateUnion.stateBitField.maxPacketSize)));
#else
                destBuffer = (uint8_t *)(epState->epPacketBuffer);
#endif
                if (index) /* USB_IN */
                {
                    memcpy(destBuffer, actualBuffer, length);
                }
                else
                {
#if ((defined(FSL_FEATURE_USB_VERSION) && (FSL_FEATURE_USB_VERSION >= 200U)) || \
     (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)))
#else
                    length = epState->stateUnion.stateBitField.maxPacketSize;
#endif
                }
                actualBuffer = destBuffer;
            }
        }
#if (defined USB_DEVICE_IP3511_NEED_CHECK_BUFFER) && (USB_DEVICE_IP3511_NEED_CHECK_BUFFER)
        else /* cannot do memory copy */
        {
            /* not 64 bytes align || not in the dedicated ram || ( OUT && HS && not mutiple of 4 ) */
            if ((((uint32_t)actualBuffer & 0x0000003FU) != 0U) ||
                (((uint32_t)actualBuffer & 0xFFC00000U) != (lpc3511IpState->registerBase->DATABUFSTART & 0xFFC00000U))
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
                || ((lpc3511IpState->controllerSpeed) && (!index) && ((length & 0x00000003u) != 0u)))
#else
            )
#endif
            {
                return kStatus_USB_Error;
            }
        }
#endif
    }

    /* Send/Receive data when the device is not resetting. */
    if (0U == lpc3511IpState->isResetting)
    {
        return USB_DeviceLpc3511IpEndpointPrime(lpc3511IpState, epState, endpointIndex, actualBuffer, length);
    }
    else
    {
        return kStatus_USB_Error;
    }
}
static usb_status_t USB_DeviceLpc3511IpTransaction(usb_device_lpc3511ip_state_struct_t *lpc3511IpState,
                                                   usb_device_lpc3511ip_endpoint_state_struct_t *epState,
                                                   uint8_t endpointIndex)
{
    usb_status_t status = kStatus_USB_Error;
    OSA_SR_ALLOC();

#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)) && \
    (!(defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) &&            \
       (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK)))
#else
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
    if (lpc3511IpState->hsInterruptIssue)
    {
#endif
        /* high-speed */
        if ((epState->stateUnion.stateBitField.epControlDefault &
             ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT)) &&
            (USB_ENDPOINT_INTERRUPT == epState->stateUnion.stateBitField.endpointType) &&
            (lpc3511IpState->controllerSpeed))
        {
            /* users can use NVIC to disable/enable the USB interrupt to improve the system performance */
            OSA_ENTER_CRITICAL();

            lpc3511IpState->registerBase->DEVCMDSTAT |=
                (USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AO_MASK | USB_LPC3511IP_DEVCMDSTAT_INTONNAK_AI_MASK);

#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
            USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(
                lpc3511IpState, endpointIndex, epState->stateUnion.stateBitField.producerOdd,
                (epState->stateUnion.stateBitField.epControlDefault << USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT) |
                    (epState->stateUnion.stateBitField.stalled << USB_LPC3511IP_ENDPOINT_STALL_SHIFT),
                0U, (uint32_t)lpc3511IpState->zeroTransactionData);
#else
        USB_LPC3511IP_ENDPOINT_SET_ENDPOINT(
            lpc3511IpState, endpointIndex, 0,
            (epState->stateUnion.stateBitField.epControlDefault << USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT) |
                (epState->stateUnion.stateBitField.stalled << USB_LPC3511IP_ENDPOINT_STALL_SHIFT),
            0U, (uint32_t)lpc3511IpState->zeroTransactionData);
#endif
            /* users can use NVIC to disable/enable the USB interrupt to improve the system performance */
            OSA_EXIT_CRITICAL();
            return kStatus_USB_Success;
        }
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
    }
#endif
#endif
#endif

    /* Enter critical */
    OSA_ENTER_CRITICAL();
    if (epState->stateUnion.stateBitField.stalled)
    {
        if ((endpointIndex >> 1U) != USB_ENDPOINT_CONTROL)
        {
            epState->stateUnion.stateBitField.stallPrimed = 1u;
            status                                        = kStatus_USB_Success;
        }
        status = kStatus_USB_Error;
        OSA_EXIT_CRITICAL();
        return status;
    }
    OSA_EXIT_CRITICAL();

    /* 1. transfer size is 0; 2. All are primed */
    if ((epState->transferLength <= epState->transferPrimedLength) && (epState->transferLength != 0))
    {
        return kStatus_USB_Success;
    }
#if (defined USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE) && (USB_DEVICE_IP3511_DOUBLE_BUFFER_ENABLE)
    if ((endpointIndex >> 1U) != USB_CONTROL_ENDPOINT)
    {
        /* disable endpoint interrupts, users can use NVIC to disable/enable the USB interrupt to improve the system
         * performance */
        OSA_ENTER_CRITICAL();
        /* lpc3511IpState->registerBase->INTEN &= (uint32_t)(~(USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK)); */
#if ((defined(USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER)) && (USB_DEVICE_IP3511_DISABLE_OUT_DOUBLE_BUFFER > 0U))
        /* for out endpoint,only use buffer toggle, disable prime double buffer at the same time*/
        /*host send data less than maxpacket size and in endpoint prime length more more than maxpacketsize, there will
         * be state mismtach*/
        if (0U == (endpointIndex & 0x1U))
        {
            status = USB_DeviceLpc3511IpGetActualBufferAndPrime(lpc3511IpState, epState, endpointIndex, 1U);
        }
        else
#endif
        {
            do
            {
                status = USB_DeviceLpc3511IpGetActualBufferAndPrime(lpc3511IpState, epState, endpointIndex, 1U);
                if (status != kStatus_USB_Success)
                {
                    break;
                }
            } while ((epState->transferLength > epState->transferPrimedLength) &&
                     (epState->stateUnion.stateBitField.doubleBufferBusy < 2));
        }
        /* enable endpoint interrupt again, users can use NVIC to disable/enable the USB interrupt to improve the system
         * performance */
        OSA_EXIT_CRITICAL();
    }
    else
#endif
    {
        status = USB_DeviceLpc3511IpGetActualBufferAndPrime(lpc3511IpState, epState, endpointIndex, 0U);
    }
    return status;
}

usb_status_t USB_DeviceLpc3511IpSend(usb_device_controller_handle controllerHandle,
                                     uint8_t endpointAddress,
                                     uint8_t *buffer,
                                     uint32_t length)
{
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)controllerHandle;
    uint8_t endpointIndex                               = USB_LPC3511IP_ENDPOINT_DES_INDEX(endpointAddress);
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);

    if (1U == epState->stateUnion.stateBitField.transferring)
    {
        return kStatus_USB_Error;
    }

    /* Save the transfer information */
    epState->transferDone         = 0U;
    epState->transferBuffer       = buffer;
    epState->transferLength       = length;
    epState->transferPrimedLength = 0U;

    return USB_DeviceLpc3511IpTransaction(lpc3511IpState, epState, endpointIndex);

/* prime the control setup transfer if it is control in endpoint and data length is zero
 * For IP3511 there is no need to prime, the buffer is always in the command/status list
 */
#if 0
    if ((0U == length) && (USB_CONTROL_ENDPOINT == (endpointAddress & USB_ENDPOINT_NUMBER_MASK)))
    {
        USB_DeviceLpc3511IpPrimeNextSetup(lpc3511IpState);
    }
#endif
}

usb_status_t USB_DeviceLpc3511IpRecv(usb_device_controller_handle controllerHandle,
                                     uint8_t endpointAddress,
                                     uint8_t *buffer,
                                     uint32_t length)
{
    return USB_DeviceLpc3511IpSend(controllerHandle, endpointAddress, buffer, length);
}

usb_status_t USB_DeviceLpc3511IpCancel(usb_device_controller_handle controllerHandle, uint8_t ep)
{
    /* users can use NVIC to disable/enable the USB interrupt to improve the system performance */
    OSA_SR_ALLOC();

    usb_device_lpc3511ip_state_struct_t *lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)controllerHandle;
    usb_device_callback_message_struct_t message;
    uint8_t endpointIndex = USB_LPC3511IP_ENDPOINT_DES_INDEX(ep);
    usb_device_lpc3511ip_endpoint_state_struct_t *epState =
        USB_DeviceLpc3511IpGetEndpointStateStruct(lpc3511IpState, endpointIndex);

    /* Cancel the transfer and notify the up layer when the endpoint is busy. */
    if ((epState->stateUnion.stateBitField.transferring)
#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)) && \
    (!(defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) &&            \
       (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK)))
#else
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
        || (
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
               (lpc3511IpState->hsInterruptIssue) &&
#endif
               (epState->stateUnion.stateBitField.epControlDefault &
                ((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT)) &&
               (USB_ENDPOINT_INTERRUPT == epState->stateUnion.stateBitField.endpointType) &&
               (lpc3511IpState->controllerSpeed) &&
               (lpc3511IpState->epCommandStatusList[endpointIndex * 2 + epState->stateUnion.stateBitField.consumerOdd] &
                USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) &&
               (!(lpc3511IpState->registerBase->EPTOGGLE & (0x01u << endpointIndex))))
#endif
#endif
    )
    {
        /* disable endpoint interrupts, users can use NVIC to disable/enable the USB interrupt to improve the system
         * performance */
        OSA_ENTER_CRITICAL();
#if (defined(FSL_FEATURE_USBHSD_VERSION) && (FSL_FEATURE_USBHSD_VERSION >= 300U)) && \
    (!(defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) &&            \
       (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK)))
#else
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
        if (lpc3511IpState->hsInterruptIssue)
        {
#endif
            epState->stateUnion.stateBitField.epControlDefault &=
                (~((USB_LPC3511IP_ENDPOINT_TOGGLE_RESET_MASK) >> USB_LPC3511IP_ENDPOINT_CONFIGURE_BITS_SHIFT));
#if (defined(FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK) && \
     (FSL_FEATURE_USBHSD_INTERRUPT_DATAX_ISSUE_VERSION_CHECK))
        }
#endif
#endif
#endif
        do
        {
            if (epState->stateUnion.stateBitField.doubleBufferBusy == 2)
            {
                epState->stateUnion.stateBitField.doubleBufferBusy = 1;
            }
            else
            {
                epState->stateUnion.stateBitField.doubleBufferBusy = 0;
            }
            /* cancel the transfer in the endpoint command/status */
            lpc3511IpState->registerBase->EPSKIP |= (0x01U << endpointIndex);
            while (lpc3511IpState->registerBase->EPSKIP & (0x01U << endpointIndex))
            {
            }
            if (epState->stateUnion.stateBitField.doubleBufferBusy)
            {
                epState->stateUnion.stateBitField.producerOdd =
                    ((lpc3511IpState->registerBase->EPINUSE & ((uint32_t)(0x01 << endpointIndex))) >> endpointIndex);
                epState->stateUnion.stateBitField.producerOdd ^= 1;
                lpc3511IpState->registerBase->EPINUSE |=
                    (uint32_t)(epState->stateUnion.stateBitField.producerOdd << endpointIndex);
            }
        } while (epState->stateUnion.stateBitField.doubleBufferBusy);

        /* clear interrupt status, enable endpoint interrupt again */
        lpc3511IpState->registerBase->INTSTAT = (0x01U << endpointIndex);

        /* users can use NVIC to disable/enable the USB interrupt to improve the system performance */
        OSA_EXIT_CRITICAL();

        message.length                                 = USB_UNINITIALIZED_VAL_32;
        message.buffer                                 = epState->transferBuffer;
        message.code                                   = ep;
        message.isSetup                                = 0U;
        epState->stateUnion.stateBitField.transferring = 0U;
        epState->stateUnion.stateBitField.producerOdd =
            ((lpc3511IpState->registerBase->EPINUSE & ((uint32_t)(0x01 << endpointIndex))) >> endpointIndex);
        epState->stateUnion.stateBitField.consumerOdd =
            ((lpc3511IpState->registerBase->EPINUSE & ((uint32_t)(0x01 << endpointIndex))) >> endpointIndex);
        USB_DeviceNotificationTrigger(lpc3511IpState->deviceHandle, &message);
    }
    return kStatus_USB_Success;
}

usb_status_t USB_DeviceLpc3511IpControl(usb_device_controller_handle controllerHandle,
                                        usb_device_control_type_t type,
                                        void *param)
{
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)controllerHandle;
    usb_status_t error                                  = kStatus_USB_Error;
    uint32_t tmp32Value;
    uint8_t tmp8Value;
#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    usb_device_struct_t *deviceHandle;
#endif
    usb_device_lpc3511ip_endpoint_state_struct_t *epState;

    if (controllerHandle == NULL)
    {
        return kStatus_USB_InvalidHandle;
    }

#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    deviceHandle = (usb_device_struct_t *)lpc3511IpState->deviceHandle;
#endif

    switch (type)
    {
        case kUSB_DeviceControlRun:
            lpc3511IpState->registerBase->DEVCMDSTAT |= (USB_LPC3511IP_DEVCMDSTAT_DCON_MASK);
            lpc3511IpState->registerBase->DEVCMDSTAT &= ~(USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK);
            break;

        case kUSB_DeviceControlStop:
            lpc3511IpState->registerBase->DEVCMDSTAT |= USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK;
            lpc3511IpState->registerBase->DEVCMDSTAT &= (~USB_LPC3511IP_DEVCMDSTAT_DCON_MASK);
            break;

        case kUSB_DeviceControlEndpointInit:
            if (param)
            {
                error = USB_DeviceLpc3511IpEndpointInit(lpc3511IpState, (usb_device_endpoint_init_struct_t *)param);
            }
            break;

        case kUSB_DeviceControlEndpointDeinit:
            if (param)
            {
                tmp8Value = *((uint8_t *)param);
                error     = USB_DeviceLpc3511IpEndpointDeinit(lpc3511IpState, tmp8Value);
            }
            break;

        case kUSB_DeviceControlEndpointStall:
            if (param)
            {
                tmp8Value = *((uint8_t *)param);
                error     = USB_DeviceLpc3511IpEndpointStall(lpc3511IpState, tmp8Value);
            }
            break;

        case kUSB_DeviceControlEndpointUnstall:
            if (param)
            {
                tmp8Value = *((uint8_t *)param);
                error     = USB_DeviceLpc3511IpEndpointUnstall(lpc3511IpState, tmp8Value);
            }
            break;

        case kUSB_DeviceControlGetDeviceStatus:
            if (param)
            {
                *((uint16_t *)param) =
                    (USB_DEVICE_CONFIG_SELF_POWER << (USB_REQUEST_STANDARD_GET_STATUS_DEVICE_SELF_POWERED_SHIFT))
#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
                    | ((uint16_t)(((uint32_t)deviceHandle->remotewakeup)
                                  << (USB_REQUEST_STANDARD_GET_STATUS_DEVICE_REMOTE_WARKUP_SHIFT)))
#endif
                    ;
                error = kStatus_USB_Success;
            }
            break;

        case kUSB_DeviceControlGetEndpointStatus:
            if (param)
            {
                usb_device_endpoint_status_struct_t *endpointStatus = (usb_device_endpoint_status_struct_t *)param;

                if (((endpointStatus->endpointAddress) & USB_ENDPOINT_NUMBER_MASK) < USB_DEVICE_IP3511_ENDPOINTS_NUM)
                {
                    epState = USB_DeviceLpc3511IpGetEndpointStateStruct(
                        lpc3511IpState, USB_LPC3511IP_ENDPOINT_DES_INDEX(endpointStatus->endpointAddress));
                    endpointStatus->endpointStatus = (uint16_t)(epState->stateUnion.stateBitField.stalled == 1U) ?
                                                         kUSB_DeviceEndpointStateStalled :
                                                         kUSB_DeviceEndpointStateIdle;
                    error = kStatus_USB_Success;
                }
            }
            break;

        case kUSB_DeviceControlPreSetDeviceAddress:
            if (param)
            {
                tmp8Value  = *((uint8_t *)param);
                tmp32Value = lpc3511IpState->registerBase->DEVCMDSTAT;
                tmp32Value &= (~USB_LPC3511IP_DEVCMDSTAT_DEV_ADDR_MASK);
                tmp32Value |= (tmp8Value & USB_LPC3511IP_DEVCMDSTAT_DEV_ADDR_MASK);
                lpc3511IpState->registerBase->DEVCMDSTAT = tmp32Value;
                error                                    = kStatus_USB_Success;
            }
            break;

        case kUSB_DeviceControlSetDeviceAddress:
            error = kStatus_USB_Success;
            break;

        case kUSB_DeviceControlGetSynchFrame:
            break;

#if defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U)
        case kUSB_DeviceControlResume:
            /* todo: turn on USB clock and enable the USB clock source */
            lpc3511IpState->registerBase->DEVCMDSTAT |= USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK;
            lpc3511IpState->registerBase->DEVCMDSTAT &= ~USB_LPC3511IP_DEVCMDSTAT_DSUS_MASK;
            while (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_DSUS_MASK)
            {
            }
            /* the W1C bits */
            lpc3511IpState->registerBase->DEVCMDSTAT &=
                ~(USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK | USB_LPC3511IP_DEVCMDSTAT_INTERRUPT_WC_MASK);
            error = kStatus_USB_Success;
            break;
#if (defined(USB_DEVICE_CONFIG_LPM_L1) && (USB_DEVICE_CONFIG_LPM_L1 > 0U))
        case kUSB_DeviceControlSleepResume:
            /* todo: turn on USB clock and enable the USB clock source */
            lpc3511IpState->registerBase->DEVCMDSTAT |= USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK;
            lpc3511IpState->registerBase->DEVCMDSTAT &= ~USB_LPC3511IP_DEVCMDSTAT_LPM_SUS_MASK;
            while (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_LPM_SUS_MASK)
            {
                __NOP();
            }
            /* the W1C bits */
            lpc3511IpState->registerBase->DEVCMDSTAT &=
                ~(USB_LPC3511IP_DEVCMDSTAT_FORCE_NEEDCLK_MASK | USB_LPC3511IP_DEVCMDSTAT_INTERRUPT_WC_MASK);
            error = kStatus_USB_Success;
            break;
#endif
        case kUSB_DeviceControlGetRemoteWakeUp:
            *((uint8_t *)param) = !!(lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_LPM_REWP_MASK);
            break;
#endif /* USB_DEVICE_CONFIG_REMOTE_WAKEUP */

        case kUSB_DeviceControlSetDefaultStatus:
            for (tmp32Value = 0U; tmp32Value < USB_DEVICE_IP3511_ENDPOINTS_NUM; tmp32Value++)
            {
                USB_DeviceLpc3511IpEndpointDeinit(lpc3511IpState, (tmp32Value | (USB_IN << 0x07U)));
                USB_DeviceLpc3511IpEndpointDeinit(lpc3511IpState, (tmp32Value | (USB_OUT << 0x07U)));
            }
            USB_DeviceLpc3511IpSetDefaultState(lpc3511IpState);
            error = kStatus_USB_Success;
            break;

        case kUSB_DeviceControlGetSpeed:
            if (param)
            {
                *((uint8_t *)param) = lpc3511IpState->deviceSpeed;
                error               = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceControlGetOtgStatus:
            break;
        case kUSB_DeviceControlSetOtgStatus:
            break;
#if (defined(USB_DEVICE_CONFIG_USB20_TEST_MODE) && (USB_DEVICE_CONFIG_USB20_TEST_MODE > 0U))
        case kUSB_DeviceControlSetTestMode:
            if (param)
            {
                tmp8Value = *((uint8_t *)param);
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
                lpc3511IpState->registerBase->DEVCMDSTAT |=
                    ((uint32_t)(tmp8Value) << USBHSD_DEVCMDSTAT_PHY_TEST_MODE_SHIFT);
#endif
                error = kStatus_USB_Success;
            }
            break;
#endif
#if (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U)) && \
    (defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U))
        case kUSB_DeviceControlDcdEnable:
            if (kStatus_hsdcd_Success == USB_HSDCD_Control(lpc3511IpState->dcdHandle, kUSB_DeviceHSDcdEnable, NULL))
            {
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceControlDcdDisable:
            if (kStatus_hsdcd_Success == USB_HSDCD_Control(lpc3511IpState->dcdHandle, kUSB_DeviceHSDcdDisable, NULL))
            {
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceControlUpdateHwTick:
            /*udpate 1ms time tick*/
            error = kStatus_USB_Success;
            break;

#endif
        default:
            break;
    }

    return error;
}

void USB_DeviceLpcIp3511IsrFunction(void *deviceHandle)
{
    usb_device_struct_t *handle = (usb_device_struct_t *)deviceHandle;
    usb_device_lpc3511ip_state_struct_t *lpc3511IpState;
    uint32_t interruptStatus;
    uint32_t usbErrorCode;
    uint32_t devState;

    if (NULL == deviceHandle)
    {
        return;
    }

    lpc3511IpState = (usb_device_lpc3511ip_state_struct_t *)(handle->controllerHandle);
    /* get and clear interrupt status */
    interruptStatus                       = lpc3511IpState->registerBase->INTSTAT;
    lpc3511IpState->registerBase->INTSTAT = interruptStatus;
    interruptStatus &= lpc3511IpState->registerBase->INTEN;

    usbErrorCode = (lpc3511IpState->registerBase->INFO & USB_LPC3511IP_INFO_ERR_CODE_MASK);

    /* device status change interrupt */
    if (interruptStatus & USB_LPC3511IP_INTSTAT_DEV_INT_MASK)
    {
        /* get and clear device state change status */
        devState = lpc3511IpState->registerBase->DEVCMDSTAT;
        devState &= ~(USB_LPC3511IP_DEVCMDSTAT_SETUP_MASK);
        lpc3511IpState->registerBase->DEVCMDSTAT = (devState | USB_LPC3511IP_DEVCMDSTAT_INTERRUPT_WC_MASK);

        /* For HS: there is interrupt with DEV_INT=1, OTG_C=1 and ADPPROBE=1 when vbus rising.
         * For FS: there is no interrupt when vbus rising. The only way is: check the VBUS_DEBOUNCED in the DRES_C
         * interrupt with DCON set.
         */
#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE))
        if ((0U == lpc3511IpState->deviceState) &&
            ((devState & USB_LPC3511IP_DEVCMDSTAT_VBUS_DEBOUNCED_MASK)
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
             || ((lpc3511IpState->registerBase->LPM & USB_LPC3511IP_USB_LPM_ADPPROBE_MASK) &&
                 (1U == lpc3511IpState->controllerSpeed))
#endif
                 ))
        {
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if ((defined(USB_DEVICE_IP3511HS_LPM_ADPPROBE_ATTACH_DEBOUNCE_COUNT)) && \
     (USB_DEVICE_IP3511HS_LPM_ADPPROBE_ATTACH_DEBOUNCE_COUNT > 0U))
            /* add one little debounce for HS's attach detection because ADPPROBE is undebounced value */
            uint32_t debounceCount = USB_DEVICE_IP3511HS_LPM_ADPPROBE_ATTACH_DEBOUNCE_COUNT;
            if (1U == lpc3511IpState->controllerSpeed)
            {
                while ((debounceCount) && (!(devState & USB_LPC3511IP_DEVCMDSTAT_VBUS_DEBOUNCED_MASK)))
                {
                    if (!(lpc3511IpState->registerBase->LPM & USB_LPC3511IP_USB_LPM_ADPPROBE_MASK))
                    {
                        break;
                    }
                    debounceCount--;
                }
            }

            if ((debounceCount == 0) || (devState & USB_LPC3511IP_DEVCMDSTAT_VBUS_DEBOUNCED_MASK))
#endif
#endif
            {
                lpc3511IpState->deviceState = 1U;
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#if ((defined FSL_FEATURE_USBHSD_HAS_EXIT_HS_ISSUE) && (FSL_FEATURE_USBHSD_HAS_EXIT_HS_ISSUE > 0U))
                USB_PhyDeviceForceEnterFSMode(lpc3511IpState->controllerId, 0);
#endif
#endif
                USB_DeviceLpc3511IpInterruptAttach(lpc3511IpState);
#if (defined(FSL_FEATURE_SOC_USBHSDCD_COUNT) && (FSL_FEATURE_SOC_USBHSDCD_COUNT > 0U)) && \
    (defined(USB_DEVICE_CONFIG_CHARGER_DETECT) && (USB_DEVICE_CONFIG_CHARGER_DETECT > 0U))
                USB_HSDCD_Control(lpc3511IpState->dcdHandle, kUSB_DeviceHSDcdRun, NULL);
#endif
            }
        }
        /* For HS: there is interrupt with DEV_INT=1, OTG_C=1 and ADPPROBE=0 when vbus falling.
         * For HS and FS: there is interrupt when vbus falling if DCON is set.
         */
        else if ((1U == lpc3511IpState->deviceState) &&
                 (((lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_DCON_MASK) &&
                   (!(devState & USB_LPC3511IP_DEVCMDSTAT_VBUS_DEBOUNCED_MASK)))
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
                  || ((!(lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_DCON_MASK)) &&
                      (!(lpc3511IpState->registerBase->LPM & USB_LPC3511IP_USB_LPM_ADPPROBE_MASK)) &&
                      (1U == lpc3511IpState->controllerSpeed))
#endif
                      ))
        {
            lpc3511IpState->deviceState = 0U;
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#if ((defined FSL_FEATURE_USBHSD_HAS_EXIT_HS_ISSUE) && (FSL_FEATURE_USBHSD_HAS_EXIT_HS_ISSUE > 0U))
            USB_PhyDeviceForceEnterFSMode(lpc3511IpState->controllerId, 1);
#endif
#endif
            USB_DeviceLpc3511IpInterruptDetach(lpc3511IpState);
        }
#endif

        /* reset change */
        if (devState & USB_LPC3511IP_DEVCMDSTAT_DRES_C_MASK)
        {
            USB_DeviceLpc3511IpInterruptReset(lpc3511IpState);
        }

/* Suspend/Resume */
#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
        if (devState & USB_LPC3511IP_DEVCMDSTAT_DSUS_C_MASK)
        {
            if (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_DSUS_MASK)
            {
                USB_DeviceLpc3511IpInterruptSuspend(lpc3511IpState);
            }
#if (defined(USB_DEVICE_CONFIG_LPM_L1) && (USB_DEVICE_CONFIG_LPM_L1 > 0U))
            else if (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_LPM_SUS_MASK)
            {
                USB_DeviceLpc3511IpInterruptLPMSleep(lpc3511IpState);
            }
#endif
            else
            {
                USB_DeviceLpc3511IpInterruptResume(lpc3511IpState);
            }
        }

#endif

#if 0U /* some soc don't support this bit, need check according to the feature macro */
        /* OTG Status change */
        if (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_OTG_C_MASK)
        {
        }
#endif
    }

    /* endpoint transfers interrupt */
    if (interruptStatus & USB_LPC3511IP_MAX_PHY_ENDPOINT_MASK)
    {
        devState = 0;                /* devState means index here */
        if (interruptStatus & 0x01u) /* control OUT */
        {
            if (lpc3511IpState->registerBase->DEVCMDSTAT & USB_LPC3511IP_DEVCMDSTAT_SETUP_MASK)
            {
                devState = 2;
                if ((lpc3511IpState->endpointState[0].stateUnion.stateBitField.stalled == 1U) ||
                    (lpc3511IpState->endpointState[1].stateUnion.stateBitField.stalled == 1U))
                {
                    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(
                        lpc3511IpState, 0, 0,
                        (~(USB_LPC3511IP_ENDPOINT_STALL_MASK | USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)));
                    USB_LPC3511IP_ENDPOINT_SET_ENDPOINT_AND(
                        lpc3511IpState, 1, 0,
                        (~(USB_LPC3511IP_ENDPOINT_STALL_MASK | USB_LPC3511IP_ENDPOINT_ACTIVE_MASK)));
                    lpc3511IpState->endpointState[0].stateUnion.stateBitField.stalled = 0U;
                    lpc3511IpState->endpointState[1].stateUnion.stateBitField.stalled = 0U;
                }

                /* todo: setup token interrupt */
                USB_DeviceLpc3511IpInterruptToken(lpc3511IpState, 0U, 1, usbErrorCode);
            }
        }

        for (; devState < (USB_DEVICE_IP3511_ENDPOINTS_NUM * 2); ++devState)
        {
            /* check the endpoint interrupt */
            if (interruptStatus & (0x01U << devState))
            {
                USB_DeviceLpc3511IpInterruptToken(lpc3511IpState, devState, 0, usbErrorCode);
            }
        }
    }

#if 0U
    if (interruptStatus & USB_LPC3511IP_INTSTAT_FRAME_INT_MASK)
    {
    }
#endif
}

#endif
