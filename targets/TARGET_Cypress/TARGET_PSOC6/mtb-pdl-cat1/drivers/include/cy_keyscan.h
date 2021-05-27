/***************************************************************************//**
* \file cy_keyscan.h
* \version 1.0
*
* \brief
* Provides an API declaration of the KEYSCAN driver
*
********************************************************************************
* \copyright
* Copyright 2020-2021, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \addtogroup group_keyscan
* \{
* MXKEYSCAN is a DEEPSLEEP peripheral IP that performs autonomous key-matrix scan and system notification. 
* Key processing detects both press and un-press actions, 
* includes micro and macro de-bouncing filters and ghost key detection.
*
* Configurable key-matrix size supports up to 20x8 keys. 
* Up to 20 columns are driven as the output and up to 8 rows are processed as the input. 
* Key actions are stored in the FIFO with interrupt notification available based on the FIFO threshold.
*
* The Scan matrix support up to 8X20 matrix, maximum of 160 keys.
* Any key press will be translated into an index corresponding column and corresponding row.
* Before any key is pressed, the Key Matrix Scan Logic is disabled.
* Once a key press is detected by the Key Detection Logic, it will enable the gate for clock to drive the key Matrix Scan Logic for GPIO scanning.
* GPIO scanning is done one column at a time by driving each column "low" and reading from the row GPIO pins to find out which input is low.
* After the Key Scan Logic had scanned through the matrix for a specific number for debounce times configured by firmware though the configuration register,  
* keycode representing the pressed key is pushed into the key FIFO for firmware to read and an interrupt to CPU will be generated. 
* There are two types of debounce mechanisms build into this scan matrix block. 
* The micro-debounce logic will provide a small debounce period to debounce the break type of mechanical vibration. 
* The macro debounce logic will scan through the matrix for a number of times for qualify a key as being pressed. 
*
* Features:
* * Ability to turn off its clock if no keys pressed.
* * Sequential scanning of up to 160 keys in a 8X20 matrix
* * Programmable number of columns from 1 to 20.
* * Programmable number of rows from 1 to 8.
* * 20-byte key-code buffer.
* * 128-kHz clock allow scanning of full 160 key matrix in about 1.2ms.
* * Keys are buffered until host microcontroller has a chance to read it, or until overflow occurs.
* * Hardware debouncing and noice/glitch filtering.
*
* This driver provides the user an easy method for accessing KeyScan registers and provides some simple functionality for reading key index from keyscan FIFO.
*
* \section group_keyscan_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_keyscan_cmd_macro KeyScan command type definition
* \defgroup group_keyscan_intr_mask_macro KeyScan ALL interrupt mask definition
* \defgroup group_keyscan_functions Functions
* \defgroup group_keyscan_data_structures Data Structures
* \defgroup group_keyscan_enums Enumerated Types
*/


#if !defined(CY_KEYSCAN_H)
#define CY_KEYSCAN_H
#include "cy_device.h"
#if defined (CY_IP_MXKEYSCAN)
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
 ** \defgroup KeyScanGroup (KEYSCAN)
 **
 ** \brief This section describes the interface for the KEYSCAN.
 **
 *****************************************************************************/
/** @{ */


/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
/** Driver major version */
#define CY_KEYSCAN_DRV_VERSION_MAJOR    1

/** Driver minor version */
#define CY_KEYSCAN_DRV_VERSION_MINOR    0

/** KEYSCAN driver ID */
#define CY_KEYSCAN_ID CY_PDL_DRV_ID(0x51u)
/**
* \addtogroup group_keyscan_enums
* \{
*/
/** KEYSCAN API status definition */
typedef enum
{
    CY_KEYSCAN_SUCCESS         = 0x00u,                                         /**< Returned successful */
    CY_KEYSCAN_BAD_PARAM       = CY_KEYSCAN_ID | CY_PDL_STATUS_ERROR | 0x01u,   /**< Bad parameter was passed */
    CY_KEYSCAN_BUSY            = CY_KEYSCAN_ID | CY_PDL_STATUS_ERROR | 0x02u,   /**< Change settings while tx/rx on-going */
    CY_KEYSCAN_QUEUE_OVERFLOW  = CY_KEYSCAN_ID | CY_PDL_STATUS_ERROR | 0x03u,   /**< QUEUE Over flow condition */
    CY_KEYSCAN_QUEUE_EMPTY     = CY_KEYSCAN_ID | CY_PDL_STATUS_ERROR | 0x04u,   /**< QUEUE Empty condition */
    CY_KEYSCAN_EVENT_NONE      = CY_KEYSCAN_ID | CY_PDL_STATUS_ERROR | 0x05u,   /**< Event None */
} cy_en_ks_status_t;

/**
 *****************************************************************************
 ** \brief Keycode enumerations
 **
  *****************************************************************************/
typedef enum {
    KEYSCAN_KEYCODE_GHOST             = 0xf5,   /*!< Ghost key keycode */
    KEYSCAN_KEYCODE_NONE              = 0xfd,   /*!< Keycode value if no key is pressed.*/
    KEYSCAN_KEYCODE_END_OF_SCAN_CYCLE = 0xfe,   /*!< Event returned to indicate the end of a scan cycle.*/
    KEYSCAN_KEYCODE_ROLLOVER          = 0xff,   /*!< Rollover event generated by the keyscan driver in case of an error (ghost or overflow) */
}cy_en_ks_keycode_t;


/** \} group_keyscan_enums */
/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/
/**
* \addtogroup group_keyscan_cmd_macro KeyScan command type definition
* \{
* Specifies the mask value for interrupt status/mask
*/
/** Mia keyevent HW FIFO size. */
#define MIA_KEY_EVENT_FIFO_SIZE                 (20u)
/** keyscan FW FIFO size. This FIFO  is implemented with KeyscanQueue. */
#define KEYSCAN_FW_FIFO_SIZE                    (2u*MIA_KEY_EVENT_FIFO_SIZE + 6u)

/** keyscan CTL register KeyScan enable Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_KS_EN_DEFAULT            0U
/** keyscan CTL register ghost enable Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_GHOST_EN_DEFAULT         1U
/** keyscan CTL register keyscan int enable Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_KS_INT_EN_DEFAULT        1U
/** keyscan CTL register keyscan reset enable Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_KYS_RST_EN_DEFAULT       0U
/** keyscan CTL register Idle duration between column scans Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_RC_EXT_DEFAULT           3U
/** keyscan CTL register No of Rows Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_RCTC_ROW_DEFAULT         7U
/** keyscan CTL register No of Columns Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_RCTC_COLUMN_DEFAULT      19U
/** keyscan CTL register used to pull the columns high after each column scan to alleviate slow rise-time due to a large key matrix capacitance. 
 ** Default value definition. */
#define MXKEYSCAN_KEYSCAN_CTL_PULL_HIGH_DEFAULT        1U
/** keyscan CTL register Default Value.  */
#define MXKEYSCAN_KEYSCAN_CTL_KSI_DRV_HIGH_DEFAULT     1U
/** keyscan CTL register  keyscan clock will stay on when set; otherwise, the clock will be gated off by when no activity is detected.
 ** Default value definition  */
#define MXKEYSCAN_KEYSCAN_CTL_KYSCLK_STAYON_DEFAULT    1U
/** keyscan CTL register  All Default values. */
#define KEYSCAN_CTL_DEFAULT (_VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_KS_EN, MXKEYSCAN_KEYSCAN_CTL_KS_EN_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_GHOST_EN, MXKEYSCAN_KEYSCAN_CTL_GHOST_EN_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_KS_INT_EN, MXKEYSCAN_KEYSCAN_CTL_KS_INT_EN_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_KYS_RST_EN, MXKEYSCAN_KEYSCAN_CTL_KYS_RST_EN_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_RC_EXT, MXKEYSCAN_KEYSCAN_CTL_RC_EXT_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_RCTC_ROW, MXKEYSCAN_KEYSCAN_CTL_RCTC_ROW_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_RCTC_COLUMN, MXKEYSCAN_KEYSCAN_CTL_RCTC_COLUMN_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_PULL_HIGH, MXKEYSCAN_KEYSCAN_CTL_PULL_HIGH_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_KSI_DRV_HIGH, MXKEYSCAN_KEYSCAN_CTL_KSI_DRV_HIGH_DEFAULT) | \
                                _VAL2FLD(MXKEYSCAN_KEYSCAN_CTL_KYSCLK_STAYON, MXKEYSCAN_KEYSCAN_CTL_KYSCLK_STAYON_DEFAULT))
/** \} group_keyscan_cmd_macro */

/**
* \addtogroup group_keyscan_intr_mask_macro KeyScan ALL interrupt mask definition
* \{
* Specifies the mask value for interrupt status/mask
*/
/** keyscan Interrupt KEY EDGE value definition. */
#define MXKEYSCAN_INTR_KEY_EDGE_DONE        (MXKEYSCAN_INTR_KEY_EDGE_Msk)
/** keyscan Interrupt FIFO THRESHOLD value definition. */
#define MXKEYSCAN_INTR_FIFO_THRESH_DONE     (MXKEYSCAN_INTR_FIFO_THRESH_Msk)
/** keyscan ALL Interrupt value definition. */
#define MXKEYSCAN_INTR_ALL                   (MXKEYSCAN_INTR_KEY_EDGE_Msk  |\
                                             MXKEYSCAN_INTR_FIFO_THRESH_Msk)

/** \} group_keyscan_intr_mask_macro */

/**
* \addtogroup group_keyscan_data_structures
* \{
*/
/** keyscan callback  function definition. 
 ** Application has to register for callback for receiving the key press events.
*/
typedef void (* cy_cb_keyscan_handle_events_t)(void);
/**
 *****************************************************************************
 ** \brief keyscan configuration
 **        This settings are per KEYSCAN instance.
 *****************************************************************************/
typedef struct cy_stc_ks_config_t 
{
    uint8_t macroDownDebCnt;             /**< macro down debounce count */

    uint8_t macroUpDebCnt;               /**< macro Up debounce count */

    uint8_t microDebCnt;                 /**< macro debounce count */

    uint8_t noofRows;                    /**< set the number of rows of the key matrx */

    uint8_t noofColumns;                 /**< set the number of columns of the key matrx */

    bool ghostEnable;                    /**< Enable ghost detection. */

    bool hostWakeupEnable;               /**< enables the keyscan block to wake the MCU module if key is detected. */

    bool clkStayOn;                      /**< The keyscan clock will stay on when set; otherwise, the clock will be gated off by when no activity is detected*/
}cy_stc_ks_config_t ;

/**
 *****************************************************************************
 ** \brief keyscan key event
 *****************************************************************************/
typedef struct cy_stc_key_event
{

    uint8_t keyCode;            /**< Key code. This is the location in the keyscan matrix that is pressed/released.
                                    May be implemented as ((row * numCols) + col) or ((col * numRows) + row. */

    uint8_t upDownFlag;         /**< Up/down flag */

    uint8_t scanCycleFlag;      /**< Should be toggled for every scan cycle in which a new event is queued.
                                   Use of this flag is optional. If used, it allows the consumer to determine whether
                                   an event is detected in the same scan cycle as the previous event or a different one.
                                   Note that this flag does not indicate any separation in time. */
}cy_stc_key_event;


/**
 *****************************************************************************
 ** \brief KEYSCAN  Context configuration
 **        This settings are per keyscan context.
 *****************************************************************************/
typedef struct cy_stc_keyscan_context_t
{
    cy_stc_key_event bufStart[KEYSCAN_FW_FIFO_SIZE];   /**< Location where the queue starts. Provided during initialization. */

    uint8_t elementSize;                      /**< Maximum size of elements. Provided during initialization. */

    uint8_t maxNumElements;                   /**< Maximum number of elements that can be placed in the queue. Provided during initialization. */

    uint8_t curNumElements;                   /**< Number of elements currently in the queue */

    uint8_t readIndex;                        /**< Read index into the queue. */

    uint8_t writeIndex;                       /**< Write index into the queue. */

    uint8_t savedWriteIndexForRollBack;       /**< Saved write index for rollback. */

    uint8_t savedNumElements;                 /**< Saved number of elements for rollback. */
    
    uint8_t keysPressedCount;                 /**< Number of key down events that are not yet matched by key up events,
                                                  which gives the number of keys currently being pressed */
    
    bool keyscan_pollingKeyscanHw;            /**< Whether HW polling is done from Keyscan */
    
    cy_cb_keyscan_handle_events_t cbEvents;   /**< callback function */
}cy_stc_keyscan_context_t;

/** \} group_keyscan_data_structures */


/**
* \addtogroup group_keyscan_functions
* \{
*/
/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
                                             
/**
 *****************************************************************************
 ** \brief  Registers for callback 
 ** Application has to register for callback for receiving the key press events.
 ** In the interrupt handler data from HW Fifo is copied to fw fifo and
 ** application is notied to get the data using this callback function.
 **
 ** \param [in]  cbEvents    Pointer to the callback function.
 **
 ** \param [in]  context     Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_Register_Callback(cy_cb_keyscan_handle_events_t cbEvents, cy_stc_keyscan_context_t* context);

/**
 *****************************************************************************
 ** \brief Register Context with the driver
 ** This Function registers for the event callback and firmware queue buffer.
 **
 ** \pre The Application must configure corresponding keyscan pins 
 **      according to requirements and settings of keyscan instance.
 **
 ** \param [in] base       Pointer to KeyScan instance register area
 ** \param [in] config     KeyScan module configuration. See #cy_stc_ks_config_t.
 ** \param [in] context   Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t  Cy_Keyscan_Init(MXKEYSCAN_Type* base, cy_stc_ks_config_t* config, cy_stc_keyscan_context_t *context );

/**
 *****************************************************************************
 ** \brief Reset Keyscan.
 ** Reset's the Keyscan HW
 ** Clear FW FIFO after a HW reset.
 ** Restores the control data and enables scans if they were enabled before this function was called.
 **
 ** \param  base     [in]        Pointer to Keyscan instance register area.
 ** \param  context  [in]        Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_Reset(MXKEYSCAN_Type* base, cy_stc_keyscan_context_t *context);

/**
 *****************************************************************************
 ** \brief Enable Keyscan.
 ** Enables the keyscan hardware.
 **
 ** \param  base    [in]  Pointer to KeyScan instance register area.
 ** \param  context [in]  Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_Enable(MXKEYSCAN_Type* base, cy_stc_keyscan_context_t *context);

/**
 *****************************************************************************
 ** \brief Disable keyscan
 ** Reset's the Keyscan hardware and disables the keyscan hardware.
 **
 ** \param  base    [in]  Pointer to KeyScan instance register area.
 ** \param  context [in]  Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_Disable(MXKEYSCAN_Type* base, cy_stc_keyscan_context_t *context);

/**
 *****************************************************************************
 ** \brief Events pending
 ** Return whether any events are in the FW fifo or not.
 **
 ** \param  base          [in]  Pointer to KeyScan instance register area.
 ** \param  context       [in]  Pointer to the context.
 ** \param  eventsPending [out] Pointer to the eventsPending, True if events pending and otherwise False.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_EventsPending(MXKEYSCAN_Type* base, cy_stc_keyscan_context_t *context, bool *eventsPending);

/**
 *****************************************************************************
 ** \brief Get next event from FW FIFO.
 ** Applications have to call \ref Cy_Keyscan_Interrupt_handler from keyscan interrupt handler.
 ** After successfully reading from HW FIFO and writing to SW FIFO applications
 ** will be notified through the registered callback.
 ** Applications to read from the sw fifo has to call this function in a loop till 
 ** the return value is CY_KEYSCAN_EVENT_NONE
 **
 ** \param  base          [in]  Pointer to KeyScan instance register area.
 ** \param  context       [in]  Pointer to the context.
 ** \param  event         [out] Pointer to the next event.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_GetNextEvent(MXKEYSCAN_Type* base, cy_stc_key_event *event, cy_stc_keyscan_context_t *context);

/**
 *****************************************************************************
 ** \brief Setup interrupt source to be accepted.
 **
 ** \param  base    [in]  Pointer to KeyScan instance register area.
 ** \param  mask    [in]  The mask with the OR of the interrupt source to be accepted.
 **                       See \ref group_keyscan_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_SetInterruptMask(MXKEYSCAN_Type* base, uint32_t mask);

/**
 *****************************************************************************
 ** \brief Return interupt mask setting.
 **
 ** \param  base [in]  Pointer to KeyScan instance register area.
 ** \param  mask  [out] The mask with the OR of the interrupt source which is masked.
 **                       See \ref group_keyscan_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_GetInterruptMask(MXKEYSCAN_Type* base, uint32_t *mask);

/**
 *****************************************************************************
 ** \brief Return interupt masked status.
 **
 ** \param  base    [in]  Pointer to KeyScan instance register area.
 ** \param  status  [out] The mask with the OR of the interrupt source which occurs.
 **                       See \ref group_keyscan_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_GetInterruptMaskedStatus(MXKEYSCAN_Type* base, uint32_t *status);

/**
 *****************************************************************************
 ** \brief Return interupt raw status.
 **
 ** \param  base    [in]  Pointer to Keyscan instance register area.
 ** \param  status  [out] The mask with the OR of the interrupt source which occurs.
 **                       See \ref group_keyscan_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_GetInterruptStatus(MXKEYSCAN_Type* base, uint32_t *status);

/**
 *****************************************************************************
 ** \brief Clear interupt status.
 **
 ** \param  base    [in]  Pointer to Keyscan instance register area.
 ** \param  mask    [in]  The mask with the OR of the interrupt source to be cleared.
 **                       See \ref group_keyscan_intr_mask_macro for the set of constants.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_ClearInterrupt(MXKEYSCAN_Type* base, uint32_t mask);

/**
 *****************************************************************************
 ** \brief Handler for keyscan interrupts.
 ** Applications have to call this function from keyscan interrupt handler.
 ** After successfully reading from HW FIFO and writing to SW FIFO this function
 ** will notify application to read from the sw fifo.
 ** Application has to call Cy_Keyscan_getNextEvent() in a loop till 
 ** the return value is CY_KEYSCAN_EVENT_NONE
 **
 ** \param  base    [in]  Pointer to Keyscan instance register area.
 ** \param  context [in]  Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_Interrupt_handler(MXKEYSCAN_Type *base, cy_stc_keyscan_context_t *context);

/**
 *****************************************************************************
 ** \brief Flush events from HW queue.
 ** Flush the Fw FIFO also and it is equallent to no key press events.
 **
 ** \param  base    [in]  Pointer to Keyscan instance register area.
 ** \param  context [in]  Pointer to the context.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_FlushHwEvents(MXKEYSCAN_Type *base, cy_stc_keyscan_context_t *context);

/**
 *****************************************************************************
 ** \brief Enables Ghost detection
 **
 ** \param  base    [in]  Pointer to Keyscan instance register area.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_EnableGhostDetection(MXKEYSCAN_Type *base);

/**
 *****************************************************************************
 ** \brief Disables Ghost detection
 **
 ** \param  base    [in]  Pointer to Keyscan instance register area.
 **
 ** \retval Refer \ref cy_en_ks_status_t
 **
 *****************************************************************************/
cy_en_ks_status_t Cy_Keyscan_DisableGhostDetection(MXKEYSCAN_Type *base);

/** \} group_keyscan_functions */
/** @} */




#ifdef __cplusplus
}
#endif
#endif /*(CY_IP_MXKEYSCAN) */
#endif /* __CY_KEYSCAN_H__ */

/** \} group_keyscan */
/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
