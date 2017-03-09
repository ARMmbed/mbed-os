/*************************************************************************************************/
/*!
 *  \file   hpal_hci.h
 *
 *  \brief  HPAL HCI abstraction.
 *
 *  \internal

 *  Copyright (c) 2015 ARM, Ltd., all rights reserved.
 *  ARM confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM, Ltd. prior
 *  to any use, copying or further distribution of this software.
 *
 *  \endinternal
 */
/*************************************************************************************************/

#ifndef HPAL_HCI_H
#define HPAL_HCI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wsf_types.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \struct hpalHciCbacks_t
 *
 *  \brief  Callbacks for allocating buffers and handling read and write completion.
 */
/*************************************************************************************************/
typedef struct
{
    /*********************************************************************************************/
    /*!
     * \brief   Allocate a buffer for a received message.
     *
     * This function is called from an interrupt context.
     *
     * \param   len  Length of buffer
     *
     * \return  Pointer to buffer or NULL if buffer could not be allocated
     */
    /*********************************************************************************************/
    uint8_t *(*BufAlloc) (uint16_t len);

    /*********************************************************************************************/
    /*!
     * \brief   Free a buffer previously allocated with `BufAlloc()`.
     *
     * \param   buf  Pointer to buffer
     */
    /*********************************************************************************************/
    void (*BufFree)  (uint8_t *buf);

    /*********************************************************************************************/
    /*!
     * \brief   Handle read completion.
     *
     * This function is called from an interrupt context.
     *
     * \param   type    Packet type
     * \param   pData   Packet data, which was allocated with `BufAlloc()`.  The caller must free
     *                  this buffer
     * \param   len     Length of packet data, in bytes
     *
     * \return  None.
     */
    /*********************************************************************************************/
    void (*ReadDone) (uint8_t type, uint8_t *pData, uint16_t len);

    /*********************************************************************************************/
    /*!
     * \brief   Handle write completion.
     *
     * This function is called from an interrupt context.
     *
     * \parma   type      Packet type.
     * \param   pData     Pointer to buffer that held written data, which was passed to
     *                   `HpalHciWrite()`
     * \param   err       Indicates success (0) or error (one of the `HPAL_HCI_ERROR_####` codes)
     * \param   pContext  Context pointer passed to `HpalHciWrite()`
     *
     * \return  None.
     *
     * \ref     ERROR_CODES "error codes"
     */
    /*********************************************************************************************/
    void (*WriteDone)(uint8_t type, uint8_t *pData, int32_t err, void *pContext);
} hpalHciCbacks_t;

/*************************************************************************************************/
/*!
 *  \name   Logging levels
 *  \anchor LOGGING_LEVELS
 *  \brief  Level of logging information printed about HCI packets received and transmitted.
 */
/*************************************************************************************************/
/*! \{ */
enum
{
    HPAL_HCI_LOGGING_LEVEL_OFF     = 0, /*!< No information will be logged. */
    HPAL_HCI_LOGGING_LEVEL_INFO    = 1, /*!< Basic details about packets will be interpreted. */
    HPAL_HCI_LOGGING_LEVEL_VERBOSE = 2  /*!< The full byte contents of packets will be logged. */
};
/*! \} */

/*! \brief The default log level. */
#define HPAL_HCI_LOGGING_LEVEL_DEFAULT  HPAL_HCI_LOGGING_LEVEL_OFF

/*************************************************************************************************/
/*!
 *  \name   Error codes
 *  \anchor ERROR_CODES
 *  \brief  An error code returned in the `WriteDone()` callback.
 */
/*************************************************************************************************/
/*! \{ */
enum
{
    HPAL_HCI_ERROR_OK      =  0,  /*!< No error; the operation succeeded. */
    HPAL_HCI_ERROR_BAD_ACK = -1,  /*!< The write failed because a bad ACK was received. */
    HPAL_HCI_ERROR_ABORTED = -2,  /*!< The write was aborted. */
    HPAL_HCI_ERROR_ALLOC   = -3,  /*!< Allocation failed. */
};
/*! \} */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 * \brief   Initialize the HCI and prepare software for reading and writing.
 *
 * \return  None.
 *
 * This function is called by `HpalBlepInit()` and generally should not be called by the
 * application.
 */
/*************************************************************************************************/
void HpalHciInit(void);

/*************************************************************************************************/
/*!
 * \brief   Set callbacks for HCI notifications and buffer allocations.
 *
 * \param   pCbacks   Pointer to callbacks.  If NULL, the reference to the existing callbacks will be
 *                    cleared.
 *
 * \return  None.
 */
/*************************************************************************************************/
void HpalHciSetCbacks(const hpalHciCbacks_t *pCbacks);

/*************************************************************************************************/
/*!
 * \brief   Take up the interface (i.e., start receiving messages and be able to transmit).
 *
 * \return  None.
 *
 * HCI messages will be received and transmissions will be allowed.  The HCI callbacks should have
 * been set before the interface is taken up.
 */
/*************************************************************************************************/
void HpalHciTakeUp(void);

/*************************************************************************************************/
/*!
 * \brief   Take down the interface (i.e., stop receiving messages and block transmissions).
 *
 * \return  None.
 *
 * HCI messages will no longer be received and transmissions (i.e., calls to `HpalHciWrite()`) will
 * be blocked.  The HCI callbacks can only safely be cleared after the interface is taken down.
 */
/*************************************************************************************************/
void HpalHciTakeDown(void);

/*************************************************************************************************/
/*!
 * \brief   Fully disable the interface.
 *
 * \return  None.
 *
 * Disabling the interface may be necessary upon an unexpected event, such as a BLEP-specific
 * command received after startup is complete.
 */
/*************************************************************************************************/
void HpalHciDisable(void);

/*************************************************************************************************/
/*!
 * \brief   Enable or disable logging for HCI packets.
 *
 * \param   level  logging level
 *
 * \return  None.
 *
 * \see     \ref LOGGING_LEVELS "logging levels"
 */
/*************************************************************************************************/
void HpalHciSetLoggingLevel(uint8_t level);

/*************************************************************************************************/
/*!
 * \brief   Write packet to HCI.
 *
 * \param   type      Packet type.
 * \param   pData     Packet data.
 * \param   len       Packet length, in bytes
 * \param   pContext  Context pointer that will be returned in the `WriteDone()` callback
 *
 * \return  None.
 */
/*************************************************************************************************/
void HpalHciWrite(uint8_t type, const uint8_t *pData, uint16_t len, void *pContext);

#ifdef __cplusplus
};
#endif

#endif /* HPAL_HCI_H */
