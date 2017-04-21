/*************************************************************************************************/
/*!
 *  \file   hpal_blep.h
 *
 *  \brief  HPAL BLEP initialization.
 *
 *  \internal

 *  __LICENSE__
 *
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

 *  \endinternal
 *
 *  __USAGE NOTE__
 *
 *  main() will usually start Cordio initialization:
 *
 *  \code
 *
 *    int main()
 *    {
 *      wsfHandlerId_t handlerId;
 *
 *      // ... initialize platform ...
 *
 *      handlerId = WsfOsSetNextHandler(HpalBlepHandler);
 *      HpalBlepInit(handlerId);
 *
 *      // ... add other handlers ...
 *
 *      HpalBlepSetStartupCback(mainBlepStartCallback);
 *      HpalBlepStart(&myBlobCbacks, (void *)&myContext);
 *
 *      // ... dispatcher loop ...
 *
 *    }
 *
 *  \endcode
 *
 *  The startup callback will receive a pass/fail indication after startup complees.  The application
 *  can then assign callbacks for the HCI interface and "take up" the HCI to begin sending commands
 *  and receiving events.
 *
 *  \code
 *
 *  static void mainBlepStartCallback(bool_t ok)
 *  {
 *    // ... start BLE stack initialization ...
 *
 *    HpalHciSetCbacks(&myHciCbacks);
 *    HpalHciTakeUp();
 *
 *    // ... more BLE stack initialization ...
 *
 *    // ... start using BLE ...
 *  }
 *
 *  \endcode
 *
 *  __STARTUP PROCESS__
 *
 *  Setup of the BLEP begins with a @ref STARTUP "VS_Startup event" sent from Cordio.
 *  The Startup_Flags parameter indicates whether the currently-executing firmware (here the
 *  bootloader) requires a firmware update to perform Bluetooth LE operations if bit 0x02 is set.
 *
 *  If firmware must be updated, the firmware should be retrieved from storage, and a description
 *  provided to Cordio in a @ref FW_LOAD "VS_Firmware_Load command".  The Status and Extended_Status
 *  should be checked in the Command_Complete event returned in response.  The firmware data should
 *  then be supplied with a series of @ref FW_DATA "VS_Firmware_Data commands".  Again, from each
 *  Command_Complete event returned in response, the Status and Extended_Status should be checked.
 *  After the Command_Complete event following the final VS_FW_Data command, Cordio will reset and
 *  the new firmware will begin executing.  Another @ref STARTUP "VS_Startup event" will be sent
 *  from Cordio.
 *
 *  The Startup_Flags parameter from the VS_Startup event will also indicate whether Cordio needs
 *  a trim update to operate if bit 0x01 is set.  If trim must be updated, the trim should be
 *  retrieved from storage, and a description provided to Cordio in a @ref TRIM_LOAD "VS_Trim_Load command".
 *  The Status and Extended_Status should be checked in the Command_Complete event returned in
 *  response.  The trim data should then be supplied with a series of @ref TRIM_DATA "VS_Trim_Data commands".
 *  Again, from each Command_Complete event returned in response, the Status and Extended_Status
 *  should be checked.  Multiple trim blobs may be uploaded sequentially.
 *
 *  After the firmware update and trim update (if necessary), Cordio should be ready to process
 *  standard Bluetooth LE commands.  The Startup_Flags from the last VS_Startup event should
 *  confirm this by setting bit 0x04.
 *
 *  \dot
 *    digraph G {
 *      node [shape="oval", color="darkorange", width=2, height=0.6, fixedsize=true];
 *      edge [color="darkorange"];
 *
 *      WAIT_FOR_STARTUP    [label="Wait for STARTUP\n from bootloader"];
 *      START_FW_LOAD       [label="Start firmware load"];
 *      DO_FW_LOAD          [label="Load firmware"];
 *      WAIT_FOR_FW_STARTUP [label="Wait for STARTUP\n from firmware"];
 *      START_TRIM_LOAD     [label="Start trim load"];
 *      DO_TRIM_LOAD        [label="Load a trim blob"];
 *      DONE_FAILURE        [label="Done with error"];
 *      DONE_SUCCESS        [label="Done with success"];
 *
 *      WAIT_FOR_STARTUP -> START_FW_LOAD;
 *
 *      START_FW_LOAD -> START_TRIM_LOAD     [label="doesn't\n need fw"];
 *      START_FW_LOAD -> DO_FW_LOAD;
 *      DO_FW_LOAD    -> DO_FW_LOAD          [label="more data"];
 *      DO_FW_LOAD    -> WAIT_FOR_FW_STARTUP [label="no more\n data"];
 *
 *      WAIT_FOR_FW_STARTUP -> START_TRIM_LOAD;
 *
 *      START_TRIM_LOAD -> DONE_SUCCESS    [label="doesn't\n need trim"];
 *      START_TRIM_LOAD -> DO_TRIM_LOAD    [label="another\n trim blob"];
 *      START_TRIM_LOAD -> DONE_SUCCESS    [label="no more\n trim blobs"];
 *      DO_TRIM_LOAD    -> DO_TRIM_LOAD    [label="more data"];
 *      DO_TRIM_LOAD    -> START_TRIM_LOAD [label="no more\n data"];
 *
 *      WAIT_FOR_STARTUP    -> DONE_FAILURE [label="timeout\n or error"];
 *      START_FW_LOAD       -> DONE_FAILURE [label="timeout\n or error"];
 *      DO_FW_LOAD          -> DONE_FAILURE [label="timeout\n or error"];
 *      WAIT_FOR_FW_STARTUP -> DONE_FAILURE [label="timeout\n or error"];
 *      START_TRIM_LOAD     -> DONE_FAILURE [label="timeout\n or error"];
 *      DO_TRIM_LOAD        -> DONE_FAILURE [label="timeout\n or error"];
 *    }
 *  \enddot
 */
/*************************************************************************************************/

#ifndef HPAL_BLEP_H
#define HPAL_BLEP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cordio_bt4_defs.h"

#include "wsf_types.h"
#include "wsf_os.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Header preceding each blob of data.
 */
/*************************************************************************************************/
typedef CordioTc2ImgHdr_t hpalBlepBlobHeader_t;

/*************************************************************************************************/
/*!
 *  \brief  Callback for BLEP startup status.
 */
/*************************************************************************************************/
typedef void (*hpalBlepStartupCback_t)(bool_t ok);

/*************************************************************************************************/
/*!
 *  \brief  Storage callbacks invoked during startup to read patch and trim data.
 */
/*************************************************************************************************/
typedef struct
{
  /***********************************************************************************************/
  /*!
   *  \brief  Setup device for reading from beginning of blob storage.
   *
   *  \param  pContext    Context given to HpalBlepStart()
   *
   *  \return TRUE if successful
   */
  /***********************************************************************************************/
  bool_t (*StartStorage)(void *pContext);

  /***********************************************************************************************/
  /*!
   *  \brief  Storage device is no longer needed, so it can be powered down.
   *
   *  \param  pContext    Context given to HpalBlepStart()
   *
   *  \return TRUE if successful
   */
  /***********************************************************************************************/
  bool_t (*EndStorage)(void *pContext);

  /***********************************************************************************************/
  /*!
   *  \brief  Read next blob header from storage device.
   *
   *  \param  pContext    Context given to HpalBlepStart()
   *  \param  pHdr        Pointer to structure that will receive header
   *
   *  \return TRUE if successful
   */
  /***********************************************************************************************/
  bool_t (*ReadNextBlobHeader)(void *pContext, hpalBlepBlobHeader_t *pHdr);

  /***********************************************************************************************/
  /*!
   *  \brief  Read more data from current blob at current offset.  Reading data advances the
   *          offset.
   *
   *  \param  pContext    Context given to HpalBlep_Startup()
   *  \param  pData       Storage for data
   *  \param  length      Number of bytes to read
   *
   *  \return TRUE if successful
   */
  /***********************************************************************************************/
  bool_t (*ReadMoreBlobData)(void *pContext, uint8_t *pData, uint32_t length);

  /***********************************************************************************************/
  /*!
   * \brief   Advance the offset of the current blob.
   *
   * \param   pContext    Context given to HpalBlep_Startup()
   * \param   length      Number of bytes to skip
   *
   * \return  TRUE if successful
   */
  /***********************************************************************************************/
  bool_t (*SkipBlobData)(void *pContext, uint32_t length);
} hpalBlepStorageCbacks_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the BLEP startup.
 *
 *  \param  handlerId       Handler ID for HpalBlepHandler().
 *
 *  \return None.
 */
/*************************************************************************************************/
void HpalBlepInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \brief  Begin BLEP startup.
 *
 *  \param  pCbacks         Storage callbacks.
 *  \param  pCbackContext   Storage callback context.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HpalBlepStart(const hpalBlepStorageCbacks_t *pCbacks, void *pCbackContext);

/*************************************************************************************************/
/*!
 *  \brief  Set callback that will indicate startup status.
 *
 *  \param  cback           Application callback.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HpalBlepSetStartupCback(hpalBlepStartupCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Handler for BLEP startup messages.
 *
 *  \param  event         WSF event mask.
 *  \param  pMsg          WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HpalBlepHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  @mainpage Host Software Architecture
 *
 *  __OVERVIEW__
 *
 *  The architecture of a typical host's software can be divided into four basic components: the
 *  application, a host BLE stack, an RTOS or bare-metal event framework, and the CORDIO Host
 *  Peripheral Access Library (HPAL).
 *
 *  The host BLE stack will provide some API, generally proprietary, for managing connections to
 *  devices (advertising, scanning, connecting, etc.) and organizing attributes into services and
 *  accessing the attribute values.
 *
 *  The CORDIO HPAL provides to the stack an interface for writing to the standard Bluetooth Host
 *  Controller Interface (HCI) or receiving alerts when new messages have been received, as well as
 *  CORDIO-specific startup routines the application (perhaps through the stack) must call before
 *  HCI transactions begin.  The provided HPAL implementation is independent of host MCU, BLE stack,
 *  and any (if any) RTOS.  However, the provided startup code does submit and service messages on
 *  the WSF event-driven OS framework.
 *
 *  \dot
 *    graph G {
 *      node [shape="polygon", sides=4 color="darkorange", width=2.5 fixedsize=true];
 *      edge [color="darkorange"];
 *      splines="ortho";
 *
 *      subgraph cluster0 {
 *        app -- host;
 *        host -- hpal [label="HCI messages", color="darkorange"];
 *        hpal -- peripheral [label="LLCC Bus", color="darkorange"];
 *        color=white;
 *        ratio=fill;
 *        edge [style="invis"];
 *      }
 *      rtos -- wsf [constraint=false];
 *      app -- wsf [constraint=false];
 *      host -- wsf [constraint=false headport="s", tailport="e"];
 *      hpal -- wsf [constraint=false headport="s", tailport="e"];
 *
 *      app [label="Application Profiles\n or Services"];
 *      host [label="Host\n BLE Stack"];
 *      hpal [label="CORDIO HPAL"];
 *      peripheral [label="CORDIO Peripheral"];
 *
 *      rtos [label="RTOS or\n Bare-Metal\n Event Framework", width=1.5 height=1.0];
 *      wsf [label="WSF", width=1.5, height=0.5];
 *    }
 *  \enddot
 *
 *  __CORDIO HPAL__
 *
 *  The CORDIO peripheral has two operational modes.  It enters _statup mode_ after reset, when
 *  the host software must load firmware and trim patches.  If that sequence is successful, the
 *  peripheral will enter _HCI mode_, during which the standard HCI interface expected by the host
 *  BLE stack will be alive.
 *
 *  The passage from startup to HCI modes is guided by the module _hpal_blep_ and kicked off with
 *  a call of the API function \ref HpalBlepStart().  During this process, a series of
 *  vendor-specific HCI commands and events are exchanged across the LLCC bus.  When startup has
 *  completed, the startup callback (set with \ref HpalBlepSetStartupCback()) will return
 *  a status indication; in the event of an error, the LLCC bus will be disabled and the HCI
 *  interface will be locked to prevent inadvertent access.
 *
 *  After a successful startup, the HCI can be accessed directly with the functions in the module
 *  _hpal_hci_.  Management functions can "take up" (\ref HpalHciTakeUp()) or "take down" (\ref
 *  HpalHciTakeDown()) the interface.  Writes are performed directly (@\refHpalHci_Write()) and
 *  completed reads or writes indicated through callbacks (\ref HpalHciSetCbacks()).  A basic
 *  logging facility will, optionally, dump packet information or contents to a console for
 *  debugging.
 */
/*************************************************************************************************/

#ifdef __cplusplus
};
#endif

#endif /* HPAL_BLEP_H */
