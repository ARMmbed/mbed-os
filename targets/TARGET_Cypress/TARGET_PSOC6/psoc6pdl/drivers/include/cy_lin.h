/***************************************************************************//**
* \file cy_lin.h
* \version 1.0
*
* \brief
* Provides an API declaration of the LIN driver
*
********************************************************************************
* \copyright
* Copyright 2020-2021, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \addtogroup group_lin
* \{
* The LIN driver provides a function API to manage Local Interconnect Network.
*
* The functions and other declarations used in this driver are in cy_lin.h.
* You can include cy_pdl.h to get access to all functions
* and declarations in the PDL.
*
* The Local Interconnect Network (LIN) bus was developed to create a standard for low-cost, low-end multiplexed communication. The use of a standard * bus protocol promotes the interoperability of network nodes.
* The LIN bus is a sub-bus system based on a serial communications protocol. The bus is a single master / multiple slave bus that uses a single wire * to transmit data.
* A LIN cluster exchanges messages with a pre-defined message frame format. The master node initiates a message exchange. Both the master node and   * the slave nodes can transmit (TX) and receive (RX).
* The LIN protocol is half-duplex: a LIN node is either transmitting or receiving, but it cannot transmit and receive at the same time. Messages are * exchanged when the LIN cluster is in operational mode.
* A LIN cluster also exchanges wake-up signals. Both the master node and the slave nodes can initiate a wake-up. Wake-up signals are exchanged when   * the LIN cluster is in sleep mode.
* The LIN bus can have a length of 10's of meters and has a bit-rate in the range of 1 kbps to 20 kbps. Most bus timing is expressed in bit periods  * (e.g. a 20 kbps LIN bus has a 50 us bit period).
* The LIN bus uses single wire communication using a "lin" line with an operating Voltage of 12 V. Most master and slave nodes use discrete           * transceiver devices.
*
* Features:
* * Single LIN hardware unit supporting multiple LIN channels.
* * Unified interrupt model.
* * Per LIN channel:
* * * Master and slave functionality.
* * * Master node autonomous header transmission. Master node autonomous response transmission and reception.
* * * Slave node autonomous header reception. Slave node autonomous response transmission and reception.
* * * Message buffer for PID, data and checksum fields.
* * * Break detection during message reception.
* * * Classic and enhanced checksum.
* * * Wakeup signaling.
* * * Timeout detection.
* * * Error detection.
*
* The LIN bus is an industry standard.
*
* \section group_lin_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_lin_cmd_macro LIN command type definition
* \defgroup group_lin_intr_mask_macro LIN ALL error mask definition
* \defgroup group_lin_functions Functions
* \defgroup group_lin_data_structures Data Structures
* \defgroup group_lin_enums Enumerated Types
*/


#if !defined(CY_LIN_H)
#define CY_LIN_H
#include "cy_device.h"
#if defined (CY_IP_MXLIN)
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *****************************************************************************
 ** \defgroup LinGroup Local Interconnect Network (LIN)
 **
 ** \brief This section describes the interface for the Local Interconnect Network.
 **
 *****************************************************************************/
/** @{ */


/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
/** Driver major version */
#define CY_LIN_DRV_VERSION_MAJOR    1

/** Driver minor version */
#define CY_LIN_DRV_VERSION_MINOR    0

/** LIN driver ID */
#define CY_LIN_ID CY_PDL_DRV_ID(0x37u)

/** Maximum ID */
#define LIN_ID_MAX                              (0x3Fu)
/** Maximum data length */
#define LIN_DATA_LENGTH_MAX                     (8u)
/** Minimum data length */
#define LIN_DATA_LENGTH_MIN                     (1u)
/** Maximum time out length */
#define LIN_TIMEOUT_LENGTH_MAX                  (255u)
/** Minimum wakeup peroid value = 250usec*/
#define LIN_WAKEUP_PERIOD_MIN                   (250u)
/** Maximum break_wakeup period bit length */
#define LIN_BREAK_WAKEUP_LENGTH_BITS_MAX        (31u)
/** LIN Master minimum break filed detection length */
#define LIN_MASTER_BREAK_FILED_LENGTH_MIN       (13u)
/** \} LinGroup */

/**
* \addtogroup group_lin_enums
* \{
*/
/** LIN API status definition */
typedef enum
{
    CY_LIN_SUCCESS   = 0x00u,                                     /**< Returned successful */
    CY_LIN_BAD_PARAM = CY_LIN_ID | CY_PDL_STATUS_ERROR | 0x01u,   /**< Bad parameter was passed */
    CY_LIN_BUSY      = CY_LIN_ID | CY_PDL_STATUS_ERROR | 0x02u,   /**< Change settings while tx/rx on-going */
    CY_LIN_FRAME_NOT_RECEIVED = CY_LIN_ID | CY_PDL_STATUS_ERROR | 0x03u,   /**< No frame received */
} cy_en_lin_status_t;
/** \} group_lin_enums */

/**
* \addtogroup group_lin_cmd_macro LIN command type definition
* \{
* Specifies the parameter values passed to LIN command API
*/
/** LIN Command TX Header */
#define LIN_CMD_TX_HEADER                   (LIN_CH_CMD_TX_HEADER_Msk)
/** LIN Command TX Response */
#define LIN_CMD_TX_RESPONSE                 (LIN_CH_CMD_TX_RESPONSE_Msk)
/** LIN Command RX Response */
#define LIN_CMD_RX_RESPONSE                 (LIN_CH_CMD_RX_RESPONSE_Msk)
/** LIN Command TX Header and TX Response */
#define LIN_CMD_TX_HEADER_TX_RESPONSE       (LIN_CH_CMD_TX_HEADER_Msk | LIN_CH_CMD_TX_RESPONSE_Msk)
/** LIN Command TX Header and RX Response */
#define LIN_CMD_TX_HEADER_RX_RESPONSE       (LIN_CH_CMD_TX_HEADER_Msk | LIN_CH_CMD_RX_RESPONSE_Msk)
/** LIN Command RX Header and RX Response */
#define LIN_CMD_RX_HEADER_RX_RESPONSE       (LIN_CH_CMD_RX_HEADER_Msk | LIN_CH_CMD_RX_RESPONSE_Msk)
/** LIN Command TX Wake up */
#define LIN_CMD_TX_WAKEUP                   (LIN_CH_CMD_TX_WAKEUP_Msk)
/** \} group_lin_cmd_macro */

/**
* \addtogroup group_lin_intr_mask_macro LIN ALL error mask definition
* \{
* Specifies the mask value for interrupt status/mask
*/
/** Mask for TX Header DONE */
#define LIN_INTR_TX_HEADER_DONE             (LIN_CH_INTR_TX_HEADER_DONE_Msk)
/** Mask for TX Response DONE */
#define LIN_INTR_TX_RESPONSE_DONE           (LIN_CH_INTR_TX_RESPONSE_DONE_Msk)
/** Mask for TX Wake up DONE */
#define LIN_INTR_TX_WAKEUP_DONE             (LIN_CH_INTR_TX_WAKEUP_DONE_Msk)
/** Mask for RX Header DONE */
#define LIN_INTR_RX_HEADER_DONE             (LIN_CH_INTR_RX_HEADER_DONE_Msk)
/** Mask for RX Respinse DONE */
#define LIN_INTR_RX_RESPONSE_DONE           (LIN_CH_INTR_RX_RESPONSE_DONE_Msk)
/** Mask for RX Wake up DONE */
#define LIN_INTR_RX_WAKEUP_DONE             (LIN_CH_INTR_RX_WAKEUP_DONE_Msk)
/** Mask for RX Header Sync DONE */
#define LIN_INTR_RX_HEADER_SYNC_DONE        (LIN_CH_INTR_RX_HEADER_SYNC_DONE_Msk)
/** Mask for RX Noise Detect */
#define LIN_INTR_RX_NOISE_DETECT            (LIN_CH_INTR_RX_NOISE_DETECT_Msk)
/** Mask for timeout */
#define LIN_INTR_TIMEOUT                    (LIN_CH_INTR_TIMEOUT_Msk)
/** Mask for TX Header Bit error */
#define LIN_INTR_TX_HEADER_BIT_ERROR        (LIN_CH_INTR_TX_HEADER_BIT_ERROR_Msk)
/** Mask for TX Response Bit error */
#define LIN_INTR_TX_RESPONSE_BIT_ERROR      (LIN_CH_INTR_TX_RESPONSE_BIT_ERROR_Msk)
/** Mask for RX Header frame error */
#define LIN_INTR_RX_HEADER_FRAME_ERROR      (LIN_CH_INTR_RX_HEADER_FRAME_ERROR_Msk)
/** Mask for Rx header sync error */
#define LIN_INTR_RX_HEADER_SYNC_ERROR       (LIN_CH_INTR_RX_HEADER_SYNC_ERROR_Msk)
/** Mask for Rx header parity error */
#define LIN_INTR_RX_HEADER_PARITY_ERROR     (LIN_CH_INTR_RX_HEADER_PARITY_ERROR_Msk)
/** Mask for Rx Respinse frame error */
#define LIN_INTR_RX_RESPONSE_FRAME_ERROR    (LIN_CH_INTR_RX_RESPONSE_FRAME_ERROR_Msk)
/** Mask for Rx response checksum error */
#define LIN_INTR_RX_RESPONSE_CHECKSUM_ERROR (LIN_CH_INTR_RX_RESPONSE_CHECKSUM_ERROR_Msk)
/** Mask for all slave errors */
#define LIN_INTR_ALL_ERROR_MASK_SLAVE       (LIN_CH_INTR_RX_NOISE_DETECT_Msk           |\
                                             LIN_CH_INTR_TIMEOUT_Msk                   |\
                                             LIN_CH_INTR_TX_RESPONSE_BIT_ERROR_Msk     |\
                                             LIN_CH_INTR_RX_HEADER_FRAME_ERROR_Msk     |\
                                             LIN_CH_INTR_RX_HEADER_SYNC_ERROR_Msk      |\
                                             LIN_CH_INTR_RX_HEADER_PARITY_ERROR_Msk    |\
                                             LIN_CH_INTR_RX_RESPONSE_FRAME_ERROR_Msk   |\
                                             LIN_CH_INTR_RX_RESPONSE_CHECKSUM_ERROR_Msk)
/** Mask for all master errors */
#define LIN_INTR_ALL_ERROR_MASK_MASTER      (LIN_INTR_ALL_ERROR_MASK_SLAVE | LIN_CH_INTR_TX_HEADER_BIT_ERROR_Msk)


/** \} group_lin_intr_mask_macro */

/**
* \addtogroup group_lin_enums
* \{
*/
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
/**
 *****************************************************************************
 ** \brief LIN break delimiter length
 **
 ** This configuration is effective only when corresponding channel = master mode.
 *****************************************************************************/
typedef enum cy_en_lin_break_delimiter_length{
    LIN_BREAK_DILIMITER_LENGTH_1BITS = 0,   /*!< 1-bit length */
    LIN_BREAK_DILIMITER_LENGTH_2BITS = 1,   /*!< 2-bit length */
    LIN_BREAK_DILIMITER_LENGTH_3BITS = 2,   /*!< 3-bit length */
    LIN_BREAK_DILIMITER_LENGTH_4BITS = 3    /*!< 4-bit length */
}cy_en_lin_break_delimiter_length_t;

/**
 *****************************************************************************
 ** \brief Stop bit selection.
 **
 *****************************************************************************/
typedef enum cy_en_lin_stopbit
{
    LIN_ONE_STOP_BIT           = 1,    /*!< 1 stop bit */
    LIN_TWO_STOP_BIT           = 3     /*!< 2 stop bits */
} cy_en_lin_stopbit_t;

/**
 *****************************************************************************
 ** \brief Checksum type selection.
 **
 *****************************************************************************/
typedef enum cy_en_lin_checksum_type
{
    LIN_CHECKSUM_TYPE_NORMAL = 0,      /*!< Normal (classic) checksum */
    LIN_CHECKSUM_TYPE_EXTENDED = 1     /*!< Extended (enhanced) checksum */
}cy_en_lin_checksum_type_t;

/**
 *****************************************************************************
 ** \brief timeout type selection.
 **
 *****************************************************************************/
typedef enum cy_en_lin_timeout_sel_type
{
    LIN_TIMEOUT_NONE = 0,                     /*!< No timeout */
    LIN_TIMEOUT_FROM_BREAK_TO_CHECKSUM = 1,    /*!< Frame mode: from the start of break field to checksum field STOP bits */
    LIN_TIMEOUT_FROM_BREAK_TO_PID = 2,         /*!< Frame header mode: detects timeout from the start of break field to PID field STOP bits */
    LIN_TIMEOUT_FROM_RESPONSE_TO_CHECKSUM = 3 /*!< Frame response mode: detects timeout from the first data field START bit to checksum field STOP bits. */
}cy_en_lin_timeout_sel_type_t;
/** \} group_lin_enums */

/**
* \addtogroup group_lin_data_structures
* \{
*/

/**
 *****************************************************************************
 ** \brief LIN configuration
 **        This settings are per LIN instance.
 *****************************************************************************/
typedef struct cy_stc_lin_config
{
    
    bool masterMode;                    /**< If TRUE, corresponding channel = master mode, If FALSE, slave mode. */
    
    bool linTransceiverAutoEnable;     /**< If TRUE, corresponding LIN channel transceiver is enabled automatically,
                                         If FALSE, firmware has to handle the transceiver enable signal manually */
    
    uint8_t breakFieldLength;           /**< Break field length. */
    
    cy_en_lin_break_delimiter_length_t breakDelimiterLength; /**< Break delimiter length. See #cy_en_lin_break_delimiter_length_t */
    
    cy_en_lin_stopbit_t stopBit;        /**< Stop bit length. See #cy_en_lin_stopbit_t. */
    
    bool filterEnable;                  /**< If TRUE, lin_rx_in filter operates.
                                        Median 3 operates on the last three "lin_rx_in" values.
                                        The sequences '000', '001', '010' and '100' result in a filtered value '0'.
                                        The sequences '111', '110', '101' and '011' result in a filtered value '1'.
                                        */
}cy_stc_lin_config_t;

/** \} group_lin_data_structures */

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/
/**
* \addtogroup group_lin_intr_mask_macro LIN ALL error mask definition
* \{
* Specifies the mask value for interrupt status/mask
*/

/** Mask for Stop Bits */
#define LIN_CH_CTL0_STOP_BITS_DEFAULT                LIN_ONE_STOP_BIT
/** Mask for Auto Enable */
#define LIN_CH_CTL0_AUTO_EN_DEFAULT                  0U
/** Mask for break delimiter length */
#define LIN_CH_CTL0_BREAK_DELIMITER_LENGTH_DEFAULT   1U
/** Mask for break wake up length */
#define LIN_CH_CTL0_BREAK_WAKEUP_LENGTH_DEFAULT      12U
/** Mask for mode */
#define LIN_CH_CTL0_MODE_DEFAULT                     0U
/** Mask for error ignore */
#define LIN_CH_CTL0_BIT_ERROR_IGNORE_DEFAULT         0U
/** Mask for parity */
#define LIN_CH_CTL0_PARITY_DEFAULT                   0U
/** Mask for parity enable */
#define LIN_CH_CTL0_PARITY_EN_DEFAULT                0U
/** Mask for filter enable */
#define LIN_CH_CTL0_FILTER_EN_DEFAULT                1U
/** Mask for enabled */
#define LIN_CH_CTL0_ENABLED_DEFAULT                  0U
/** Mask for default */
#define LIN_CH_CTL0_DEFAULT (_VAL2FLD(LIN_CH_CTL0_STOP_BITS, LIN_CH_CTL0_STOP_BITS_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_AUTO_EN, LIN_CH_CTL0_AUTO_EN_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_BREAK_DELIMITER_LENGTH, LIN_CH_CTL0_BREAK_DELIMITER_LENGTH_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_BREAK_WAKEUP_LENGTH, LIN_CH_CTL0_BREAK_WAKEUP_LENGTH_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_MODE, LIN_CH_CTL0_MODE_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_BIT_ERROR_IGNORE, LIN_CH_CTL0_BIT_ERROR_IGNORE_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_PARITY, LIN_CH_CTL0_PARITY_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_PARITY_EN, LIN_CH_CTL0_PARITY_EN_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_FILTER_EN, LIN_CH_CTL0_FILTER_EN_DEFAULT) | \
                                _VAL2FLD(LIN_CH_CTL0_ENABLED, LIN_CH_CTL0_ENABLED_DEFAULT))
/** \} group_lin_intr_mask_macro */

/**
* \addtogroup group_lin_functions
* \{
*/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
/**
 *****************************************************************************
 ** \brief DeInitialisation of a LIN module.
 ** This Function deinitialises the selected LIN channel.
 **
 ** \param [in]  base       Pointer to LIN instance channel register
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_DeInit( LIN_CH_Type* base );

/**
 *****************************************************************************
 ** \brief Initialisation of a LIN module.
 ** This Function initialises the LIN according the Options setup in the
 ** passed Config Struct. Several Checkings are done before that and an error
 ** is returned if invalid Modes are requested.
 **
 ** \pre The Application must configure corresponding LIN pins (SIN, SOT)
 **      according to requirements and settings of LIN instance.
 **      And must set baudrate using SysClk. LIN I/F has internal fixed
 **      oversampling value (16). Therefore LIN clock / 16 is actual
 **      baudrate.
 **
 ** \param [in]  base        Pointer to LIN instance register area
 ** \param [in]  pstcConfig     LIN module configuration. See #cy_stc_lin_config_t.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_Init( LIN_CH_Type* base, const cy_stc_lin_config_t *pstcConfig);

/**
 *****************************************************************************
 ** \brief Read response data.
 **
 ** \pre Must be run after RX_RESPONSE_DONE.
 **
 ** \note This function return last received data set.
 **
 ** \param  base     [in]        Pointer to LIN instance register area.
 ** \param  data     [out]       Pointer to received data.
 **                              Must have enough space for current Rx data.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_ReadData( LIN_CH_Type* base, uint8_t *data);

/**
 *****************************************************************************
 ** \brief Write response data.
 **
 ** \param  base         [in]        Pointer to LIN instance register area.
 ** \param  data         [in]        Pointer to response data.
 ** \param  dataLength   [in]        Data length in bytes.
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_WriteData( LIN_CH_Type* base, const uint8_t *data, uint8_t dataLength );

/**
 *****************************************************************************
 ** \brief Enable LIN channel.
 **
 ** \param  base  [in]  Pointer to LIN instance register area.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_Enable(LIN_CH_Type* base);

/**
 *****************************************************************************
 ** \brief Disable LIN channel.
 **        Disabling LIN channel causes clearing none retained registers.
 **        e.g) CMD register, INTR register, STATUS regiser.
 **        This behavior can be uses re-initialization after error,
 **        abort RX operation.
 **
 ** \param  base  [in]  Pointer to LIN instance register area.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_Disable(LIN_CH_Type* base);

/**
 *****************************************************************************
 ** \brief Setup LIN break/wakeup field length.
 **        Normaly this I/F is used for detection of the wakeup pulse.
 **
 ** \param  base  [in]  Pointer to LIN instance register area.
 ** \param  length   [in]  Bit length of the break/wakeup field.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_SetBreakWakeupFieldLength(LIN_CH_Type* base, uint8_t length);

/**
 *****************************************************************************
 ** \brief Setup LIN response field data length
 **
 ** \param  base     [in]  Pointer to LIN instance register area.
 ** \param  length   [in]  Data length in byte
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_SetDataLength(LIN_CH_Type* base, uint8_t length);

/**
 *****************************************************************************
 ** \brief Setup LIN checksum type setting
 **
 ** \param  base     [in]  Pointer to LIN instance register area.
 ** \param  type     [in]  Checksum type. Refer \ref cy_en_lin_checksum_type_t
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_SetChecksumType(LIN_CH_Type* base, cy_en_lin_checksum_type_t type);

/**
 *****************************************************************************
 ** \brief Setup LIN operation command
 **
 ** \note As a normal usage, following comobinations are used.
 **       - Master
 **         Header TX only          : LIN_CMD_TX_HEADER
 **         Header TX & TX response : LIN_CMD_TX_HEADER_TX_RESPONSE
 **         Header TX & RX response : LIN_CMD_TX_HEADER_RX_RESPONSE
 **       - Slave
 **         Header RX               : LIN_CMD_RX_HEADER_RX_RESPONSE
 **         TX response             : LIN_CMD_TX_RESPONSE
 **         RX response             : Already set when header RX
 **         As a typical usage, RX response is always set at header RX phase.
 **       - Wakeup frame            : LIN_CMD_TX_WAKEUP
 **       Instead of above macro, you can use raw definition LIN_CH_CMD_*_Msk defined in IO header file.
 **
 ** \param  base     [in]  Pointer to LIN instance register area.
 ** \param  command  [in]  Required operation command. Refer \ref group_lin_cmd_macro
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_SetCmd(LIN_CH_Type* base, uint32_t command);

/**
 *****************************************************************************
 ** \brief Setup LIN header for master tx header operation
 **
 ** \param  base    [in]  Pointer to LIN instance register area.
 ** \param  id      [in]  ID value
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_SetHeader(LIN_CH_Type* base, uint8_t id);

/**
 *****************************************************************************
 ** \brief Return received LIN header
 **
 ** \param  base    [in]  Pointer to LIN instance register area.
 ** \param  id      [out] Received ID value.
 ** \param  parity  [out] Received parity value.
 **                       Note that parity check is always done by HW automatically.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_GetHeader(LIN_CH_Type* base, uint8_t *id, uint8_t *parity);

/**
 *****************************************************************************
 ** \brief Setup interrupt source to be accepted.
 **
 ** \param  base    [in]  Pointer to LIN instance register area.
 ** \param  mask    [in]  The mask with the OR of the interrupt source to be accepted.
 **                       See \ref group_lin_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_SetInterruptMask(LIN_CH_Type* base, uint32_t mask);

/**
 *****************************************************************************
 ** \brief Return interupt mask setting.
 **
 ** \param  base [in]  Pointer to LIN instance register area.
 ** \param  mask  [out] The mask with the OR of the interrupt source which is masked.
 **                       See \ref group_lin_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_GetInterruptMask(LIN_CH_Type* base, uint32_t *mask);

/**
 *****************************************************************************
 ** \brief Return interupt masked status.
 **
 ** \param  base    [in]  Pointer to LIN instance register area.
 ** \param  status  [out] The mask with the OR of the interrupt source which occurs.
 **                       See \ref group_lin_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_GetInterruptMaskedStatus(LIN_CH_Type* base, uint32_t *status);

/**
 *****************************************************************************
 ** \brief Return interupt raw status.
 **
 ** \param  base    [in]  Pointer to LIN instance register area.
 ** \param  status  [out] The mask with the OR of the interrupt source which occurs.
 **                       See \ref group_lin_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_GetInterruptStatus(LIN_CH_Type* base, uint32_t *status);

/**
 *****************************************************************************
 ** \brief Clear interupt status.
 **
 ** \param  base [in]  Pointer to LIN instance register area.
 ** \param  mask    [in]  The mask with the OR of the interrupt source to be cleared.
 **                       See \ref group_lin_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_ClearInterrupt(LIN_CH_Type* base, uint32_t mask);

/**
 *****************************************************************************
 ** \brief Return LIN module status
 **
 ** \param  base [in]  Pointer to LIN instance register area.
 ** \param  status  [out] LIN module status.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Lin_GetStatus(LIN_CH_Type* base, uint32_t *status);

/**
 *****************************************************************************
 ** \brief  Enables LIN channel 'en' out
 **
 ** If linTransceiverAutoEnable in cy_stc_lin_config_t config strucuture is set to true
 ** then tranceiver is set automatically and user need not call this funcition,
 ** else user has to call Cy_Status_EnOut_Enable() before sending a message and call
 ** Cy_Status_EnOut_Disable() after sending the message.
 **
 ** \param  base [in]  Pointer to LIN instance register area.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Status_EnOut_Enable(LIN_CH_Type* base);

/**
 *****************************************************************************
 ** \brief  Disables LIN channel 'en' out
 **
 ** If linTransceiverAutoEnable in cy_stc_lin_config_t config strucuture is set to true
 ** then tranceiver is set automatically and user need not call this funcition,
 ** else user has to call Cy_Status_EnOut_Enable() before sending a message and call
 ** Cy_Status_EnOut_Disable() after sending the message.
 **
 ** \param  base [in]  Pointer to LIN instance register area.
 **
 ** \retval Refer \ref cy_en_lin_status_t
 **
 *****************************************************************************/
cy_en_lin_status_t Cy_Status_EnOut_Disable(LIN_CH_Type* base);

/** \} group_lin_functions */
/** @} */

#ifdef __cplusplus
}
#endif
#endif /*(CY_IP_MXLIN) */
#endif /* __CY_LIN_H__ */

/** \} group_lin */

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
