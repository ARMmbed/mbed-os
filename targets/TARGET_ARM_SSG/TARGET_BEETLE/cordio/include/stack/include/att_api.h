/*************************************************************************************************/
/*!
 *  \file   att_api.h
 *
 *  \brief  Attribute protocol client and server API.
 *
 *          $Date: 2016-01-06 07:40:44 -0800 (Wed, 06 Jan 2016) $
 *          $Revision: 5284 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef ATT_API_H
#define ATT_API_H

#include "wsf_timer.h"
#include "att_defs.h"
#include "att_uuid.h"
#include "dm_api.h"
#include "cfg_stack.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! ATT server attribute settings */
#define ATTS_SET_UUID_128           0x01    /*! Set if the UUID is 128 bits in length */
#define ATTS_SET_WRITE_CBACK        0x02    /*! Set if the group callback is executed when
                                                this attribute is written by a client device */
#define ATTS_SET_READ_CBACK         0x04    /*! Set if the group callback is executed when
                                                this attribute is read by a client device */
#define ATTS_SET_VARIABLE_LEN       0x08    /*! Set if the attribute has a variable length */
#define ATTS_SET_ALLOW_OFFSET       0x10    /*! Set if writes are allowed with an offset */
#define ATTS_SET_CCC                0x20    /*! Set if the attribute is a client characteristic
                                                configuration descriptor */
#define ATTS_SET_ALLOW_SIGNED       0x40    /*! Set if signed writes are allowed */
#define ATTS_SET_REQ_SIGNED         0x80    /*! Set if signed writes are required if link
                                                is not encrypted */

/*! ATT server attribute permissions */
#define ATTS_PERMIT_READ            0x01    /*! Set if attribute can be read */
#define ATTS_PERMIT_READ_AUTH       0x02    /*! Set if attribute read requires authentication */
#define ATTS_PERMIT_READ_AUTHORIZ   0x04    /*! Set if attribute read requires authorization */
#define ATTS_PERMIT_READ_ENC        0x08    /*! Set if attribute read requires encryption */
#define ATTS_PERMIT_WRITE           0x10    /*! Set if attribute can be written */
#define ATTS_PERMIT_WRITE_AUTH      0x20    /*! Set if attribute write requires authentication */
#define ATTS_PERMIT_WRITE_AUTHORIZ  0x40    /*! Set if attribute write requires authorization */
#define ATTS_PERMIT_WRITE_ENC       0x80    /*! Set if attribute write requires encryption */

/*! ATT client characteristic discovery and configuration settings */
#define ATTC_SET_UUID_128           0x01    /*! Set if the UUID is 128 bits in length */
#define ATTC_SET_REQUIRED           0x02    /*! Set if characteristic must be discovered */
#define ATTC_SET_DESCRIPTOR         0x04    /*! Set if this is a characteristic descriptor */

/*! ATT callback events */
#define ATT_CBACK_START             0x02    /*! ATT callback event starting value */
enum                                        /*! Internal note: event values match method values */
{
  /*! ATT client callback events */
  ATTC_FIND_INFO_RSP = ATT_CBACK_START,     /*! Find information response */
  ATTC_FIND_BY_TYPE_VALUE_RSP,              /*! Find by type value response */
  ATTC_READ_BY_TYPE_RSP,                    /*! Read by type value response */
  ATTC_READ_RSP,                            /*! Read response */
  ATTC_READ_LONG_RSP,                       /*! Read long response */
  ATTC_READ_MULTIPLE_RSP,                   /*! Read multiple response */
  ATTC_READ_BY_GROUP_TYPE_RSP,              /*! Read group type response */
  ATTC_WRITE_RSP,                           /*! Write response */
  ATTC_WRITE_CMD_RSP,                       /*! Write command response */
  ATTC_PREPARE_WRITE_RSP,                   /*! Prepare write response */
  ATTC_EXECUTE_WRITE_RSP,                   /*! Execute write response */
  ATTC_HANDLE_VALUE_NTF,                    /*! Handle value notification */
  ATTC_HANDLE_VALUE_IND,                    /*! Handle value indication */
  /*! ATT server callback events */
  ATTS_HANDLE_VALUE_CNF,                    /*! Handle value confirmation */
  ATTS_CCC_STATE_IND                        /*! Client chracteristic configuration state change */
};

/*! ATT callback events */
#define ATT_CBACK_END               ATTS_CCC_STATE_IND  /*! ATT callback event ending value */

/*! Base value for HCI error status values passed through ATT */
#define ATT_HCI_ERR_BASE            0x20

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Configurable parameters */
typedef struct
{
  wsfTimerTicks_t   discIdleTimeout;  /*! ATT server service discovery connection idle timeout in seconds */
  uint16_t          mtu;              /*! desired ATT MTU */
  uint8_t           transTimeout;     /*! transcation timeout in seconds */
  uint8_t           numPrepWrites;    /*! number of queued prepare writes supported by server */
} attCfg_t;

/*!
 * Attribute server data types
 */

/*! Attribute structure */
typedef struct
{
  uint8_t const     *pUuid;           /*! Pointer to the attribute's UUID */
  uint8_t           *pValue;          /*! Pointer to the attribute's value */
  uint16_t          *pLen;            /*! Pointer to the length of the attribute's value */
  uint16_t          maxLen;           /*! Maximum length of attribute's value */
  uint8_t           settings;         /*! Attribute settings */
  uint8_t           permissions;      /*! Attribute permissions */
} attsAttr_t;

/*! Attribute group read callback */
typedef uint8_t (*attsReadCback_t)(dmConnId_t connId, uint16_t handle, uint8_t operation,
                                   uint16_t offset, attsAttr_t *pAttr);

/*! Attribute group write callback */
typedef uint8_t (*attsWriteCback_t)(dmConnId_t connId, uint16_t handle, uint8_t operation,
                                    uint16_t offset, uint16_t len, uint8_t *pValue,
                                    attsAttr_t *pAttr);

/*! Attribute group */
typedef struct attsGroup_tag
{
  struct attsGroup_tag  *pNext;       /*! For internal use only */
  attsAttr_t            *pAttr;       /*! Pointer to attribute list for this group */
  attsReadCback_t       readCback;    /*! Read callback function */
  attsWriteCback_t      writeCback;   /*! Write callback function */
  uint16_t              startHandle;  /*! The handle of the first attribute in this group */
  uint16_t              endHandle;    /*! The handle of the last attribute in this group */
} attsGroup_t;

/*! Client characteristc configuration settings */
typedef struct
{
  uint16_t          handle;           /*! Client characteristc configuration descriptor handle */
  uint16_t          valueRange;       /*! Acceptable value range of the descriptor value */
  uint8_t           secLevel;         /*! Security level of characteristic value */
} attsCccSet_t;

/*! ATT client structure for characteristic and descriptor discovery */
typedef struct attcDiscChar_tag
{
  uint8_t const           *pUuid;       /*! Pointer to UUID */
  uint8_t                 settings;     /*! Characteristic discovery settings */
} attcDiscChar_t;

/*! ATT client structure for characteristic and descriptor configuration */
typedef struct
{
  uint8_t const         *pValue;      /*! Pointer to default value or NULL */
  uint8_t               valueLen;     /*! Default value length */
  uint8_t               hdlIdx;       /*! Index of its handle in handle list */
} attcDiscCfg_t;

/*! ATT client discovery control block */
typedef struct
{
  attcDiscChar_t        **pCharList;  /*! Characterisic list for discovery */
  uint16_t              *pHdlList;    /*! Characteristic handle list */
  attcDiscCfg_t         *pCfgList;    /*! Characterisic list for configuration */
  uint8_t               charListLen;  /*! Characteristic and handle list length */
  uint8_t               cfgListLen;   /*! Configuration list length */

  /* the following are for internal use only */
  uint16_t              svcStartHdl;
  uint16_t              svcEndHdl;
  uint8_t               charListIdx;
  uint8_t               endHdlIdx;
} attcDiscCb_t;

/*!
 * ATT callback parameters:
 *
 * \param hdr.event     Callback event
 * \param hdr.param     DM connection ID
 * \param hdr.status    Event status:  ATT_SUCCESS or error status
 * \param pValue        Pointer to value data, valid if valueLen > 0
 * \param valueLen      Length of value data
 * \param handle        Attribute handle
 */
typedef struct
{
  wsfMsgHdr_t           hdr;          /*! Header structure */
  uint8_t               *pValue;      /*! Value */
  uint16_t              valueLen;     /*! Value length */
  uint16_t              handle;       /*! Attribute handle */
  bool_t                continuing;   /*! TRUE if more response packets expected */
} attEvt_t;

/*! ATTS client characteristic configuration callback structure */
typedef struct
{
  wsfMsgHdr_t           hdr;          /*! Header structure */
  uint16_t              handle;       /*! CCCD handle */
  uint16_t              value;        /*! CCCD value */
  uint8_t               idx;          /*! CCCD settings index */
} attsCccEvt_t;

/*! ATT callback type */
typedef void (*attCback_t)(attEvt_t *pEvt);

/*! ATTS authorization callback type */
typedef uint8_t (*attsAuthorCback_t)(dmConnId_t connId, uint8_t permit, uint16_t handle);

/*! ATTS client characteristic configuration callback */
typedef void (*attsCccCback_t)(attsCccEvt_t *pEvt);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! Configuration pointer */
extern attCfg_t *pAttCfg;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     AttRegister
 *
 *  \brief  Register a callback with ATT.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttRegister(attCback_t cback);

/*************************************************************************************************/
/*!
 *  \fn     AttConnRegister
 *
 *  \brief  Register a connection callback with ATT.  The callback is typically used to
 *          manage the attribute server database.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttConnRegister(dmCback_t cback);


/*************************************************************************************************/
/*!
 *  \fn     AttGetMtu
 *
 *  \brief  Get the attribute protocol MTU of a connection.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return MTU of the connection.
 */
/*************************************************************************************************/
uint16_t AttGetMtu(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \fn     AttsInit
 *
 *  \brief  Initialize ATT server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsInit(void);

/*************************************************************************************************/
/*!
 *  \fn     AttsIndInit
 *
 *  \brief  Initialize ATT server for indications/notifications.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsIndInit(void);

/*************************************************************************************************/
/*!
 *  \fn     AttsSignInit
 *
 *  \brief  Initialize ATT server for data signing.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsSignInit(void);

/*************************************************************************************************/
/*!
 *  \fn     AttsAuthorRegister
 *
 *  \brief  Register an authorization callback with the attribute server.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsAuthorRegister(attsAuthorCback_t cback);

/*************************************************************************************************/
/*!
 *  \fn     AttsAddGroup
 *
 *  \brief  Add an attribute group to the attribute server.
 *
 *  \param  pGroup    Pointer to an attribute group structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsAddGroup(attsGroup_t *pGroup);

/*************************************************************************************************/
/*!
 *  \fn     AttsRemoveGroup
 *
 *  \brief  Remove an attribute group from the attribute server.
 *
 *  \param  startHandle  Start handle of attribute group to be removed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsRemoveGroup(uint16_t startHandle);

/*************************************************************************************************/
/*!
 *  \fn     AttsSetAttr
 *
 *  \brief  Set an attribute value in the attribute server.
 *
 *  \param  handle    Attribute handle.
 *  \param  valueLen  Attribute length.
 *  \param  pValue    Attribute value.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 */
/*************************************************************************************************/
uint8_t AttsSetAttr(uint16_t handle, uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttsGetAttr
 *
 *  \brief  Get an attribute value in the attribute server.
 *
 *  \param  handle    Attribute handle.
 *  \param  pLen      Returned attribute length pointer.
 *  \param  pValue    Returned attribute value pointer.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 *  \return This function returns the attribute length in pLen and a pointer to the attribute
 *          value in pValue.
 */
/*************************************************************************************************/
uint8_t AttsGetAttr(uint16_t handle, uint16_t *pLen, uint8_t **pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttsHandleValueInd
 *
 *  \brief  Send an attribute protocol Handle Value Indication.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsHandleValueInd(dmConnId_t connId, uint16_t handle, uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttsHandleValueNtf
 *
 *  \brief  Send an attribute protocol Handle Value Notification.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsHandleValueNtf(dmConnId_t connId, uint16_t handle, uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttsCccRegister
 *
 *  \brief  Register the utility service for managing client characteristic
 *          configuration descriptors.  This function is typically called once on
 *          system initialization.
 *
 *  \param  setLen  Length of settings array.
 *  \param  pSet    Array of CCC descriptor settings.
 *  \param  cback   Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsCccRegister(uint8_t setLen, attsCccSet_t *pSet, attsCccCback_t cback);

/*************************************************************************************************/
/*!
 *  \fn     AttsCccInitTable
 *
 *  \brief  Initialize the client characteristic configuration descriptor value table for a
 *          connection.  The table is initialized with the values from pCccTbl.  If pCccTbl
 *          is NULL the table will be initialized to zero.
 *
 *          This function must be called when a connection is established or when a
 *          device is bonded.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCccTbl     Pointer to the descriptor value array.  The length of the array
 *                      must equal the value of setLen passed to AttsCccRegister().
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsCccInitTable(dmConnId_t connId, uint16_t *pCccTbl);

/*************************************************************************************************/
/*!
 *  \fn     AttsCccClearTable
 *
 *  \brief  Clear and deallocate the client characteristic configuration descriptor value
 *          table for a connection.  This function must be called when a connection is closed.
 *
 *  \param  connId      DM connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsCccClearTable(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \fn     AttsCccGet
 *
 *  \brief  Get the value of a client characteristic configuration descriptor by its index.
 *          If not found, return zero.
 *
 *  \param  connId      DM connection ID.
 *  \param  idx         Index of descriptor in CCC descriptor handle table.
 *
 *  \return Value of the descriptor.
 */
/*************************************************************************************************/
uint16_t AttsCccGet(dmConnId_t connId, uint8_t idx);

/*************************************************************************************************/
/*!
 *  \fn     AttsCccSet
 *
 *  \brief  Set the value of a client characteristic configuration descriptor by its index.
 *
 *  \param  connId      DM connection ID.
 *  \param  idx         Index of descriptor in CCC descriptor handle table.
 *  \param  value       Value of the descriptor.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsCccSet(dmConnId_t connId, uint8_t idx, uint16_t value);

/*************************************************************************************************/
/*!
 *  \fn     AttsCccEnabled
 *
 *  \brief  Check if a client characteristic configuration descriptor is enabled and if
 *          the characteristic's security level has been met.
 *
 *  \param  connId      DM connection ID.
 *  \param  idx         Index of descriptor in CCC descriptor handle table.
 *
 *  \return Value of the descriptor if security level is met, otherwise zero.
 */
/*************************************************************************************************/
uint16_t AttsCccEnabled(dmConnId_t connId, uint8_t idx);

/*************************************************************************************************/
/*!
 *  \fn     AttsSetCsrk
 *
 *  \brief  Set the peer's data signing key on this connection.  This function
 *          is typically called from the ATT connection callback when the connection is
 *          established.  The caller is responsible for maintaining the memory that
 *          contains the key.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCsrk       Pointer to data signing key (CSRK).
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsSetCsrk(dmConnId_t connId, uint8_t *pCsrk);

/*************************************************************************************************/
/*!
 *  \fn     AttsSetSignCounter
 *
 *  \brief  Set the peer's sign counter on this connection.  This function
 *          is typically called from the ATT connection callback when the connection is
 *          established.  ATT maintains the value of the sign counter internally and
 *          sets the value when a signed packet is successfully received.
 *
 *  \param  connId      DM connection ID.
 *  \param  signCounter Sign counter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsSetSignCounter(dmConnId_t connId, uint32_t signCounter);

/*************************************************************************************************/
/*!
 *  \fn     AttsGetSignCounter
 *
 *  \brief  Get the current value peer's sign counter on this connection.  This function
 *          is typically called from the ATT connection callback when the connection is
 *          closed so the application can store the sign counter for use on future
 *          connections.
 *
 *  \param  connId      DM connection ID.
 *
 *  \return Sign counter.
 */
/*************************************************************************************************/
uint32_t AttsGetSignCounter(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \fn     AttcInit
 *
 *  \brief  Initialize ATT client.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcInit(void);

/*************************************************************************************************/
/*!
 *  \fn     AttcSignInit
 *
 *  \brief  Initialize ATT client for data signing.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcSignInit(void);

/*************************************************************************************************/
/*!
 *  \fn     AttcFindInfoReq
 *
 *  \brief  Initiate an attribute protocol Find Information Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcFindInfoReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle, bool_t continuing);

/*************************************************************************************************/
/*!
 *  \fn     AttcFindByTypeValueReq
 *
 *  \brief  Initiate an attribute protocol Find By Type Value Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuid16      16-bit UUID to find.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcFindByTypeValueReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle,
                            uint16_t uuid16, uint16_t valueLen, uint8_t *pValue, bool_t continuing);

/*************************************************************************************************/
/*!
 *  \fn     AttcReadByTypeReq
 *
 *  \brief  Initiate an attribute protocol Read By Type Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadByTypeReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle,
                       uint8_t uuidLen, uint8_t *pUuid, bool_t continuing);

/*************************************************************************************************/
/*!
 *  \fn     AttcReadReq
 *
 *  \brief  Initiate an attribute protocol Read Request.
 *
 *  \param  connId    DM connection ID.
 *  \param  handle    Attribute handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadReq(dmConnId_t connId, uint16_t handle);

/*************************************************************************************************/
/*!
 *  \fn     AttcReadLongReq
 *
 *  \brief  Initiate an attribute protocol Read Long Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  offset      Read attribute data starting at this offset.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadLongReq(dmConnId_t connId, uint16_t handle, uint16_t offset, bool_t continuing);

/*************************************************************************************************/
/*!
 *  \fn     AttcReadMultipleReq
 *
 *  \brief  Initiate an attribute protocol Read Multiple Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  numHandles  Number of handles in attribute handle list.
 *  \param  pHandles    List of attribute handles.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadMultipleReq(dmConnId_t connId, uint8_t numHandles, uint16_t *pHandles);

/*************************************************************************************************/
/*!
 *  \fn     AttcReadByGroupTypeReq
 *
 *  \brief  Initiate an attribute protocol Read By Group Type Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadByGroupTypeReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle,
                            uint8_t uuidLen, uint8_t *pUuid, bool_t continuing);

/*************************************************************************************************/
/*!
 *  \fn     AttcWriteReq
 *
 *  \brief  Initiate an attribute protocol Write Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcWriteReq(dmConnId_t connId, uint16_t handle, uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttcWriteCmd
 *
 *  \brief  Initiate an attribute protocol Write Command.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcWriteCmd(dmConnId_t connId, uint16_t handle, uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttcSignedWriteCmd
 *
 *  \brief  Initiate an attribute protocol signed Write Command.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  signCounter Value of the sign counter.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcSignedWriteCmd(dmConnId_t connId, uint16_t handle, uint32_t signCounter,
                        uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \fn     AttcPrepareWriteReq
 *
 *  \brief  Initiate an attribute protocol Prepare Write Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  offset      Write attribute data starting at this offset.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *  \param  valueByRef  TRUE if pValue data is accessed by reference rather than copied.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcPrepareWriteReq(dmConnId_t connId, uint16_t handle, uint16_t offset, uint16_t valueLen,
                         uint8_t *pValue, bool_t valueByRef, bool_t continuing);

/*************************************************************************************************/
/*!
 *  \fn     AttcExecuteWriteReq
 *
 *  \brief  Initiate an attribute protocol Execute Write Request.
 *
 *  \param  connId    DM connection ID.
 *  \param  writeAll  TRUE to write all queued writes, FALSE to cancel all queued writes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcExecuteWriteReq(dmConnId_t connId, bool_t writeAll);

/*************************************************************************************************/
/*!
 *  \fn     AttcCancelReq
 *
 *  \brief  Cancel an attribute protocol request in progress.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcCancelReq(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscService
 *
 *  \brief  This utility function discovers the given service on a peer device.  Function
 *          AttcFindByTypeValueReq() is called to initiate the discovery procedure.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *  \param  uuidLen     Length of service UUID (2 or 16).
 *  \param  pUuid       Pointer to service UUID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcDiscService(dmConnId_t connId, attcDiscCb_t *pCb, uint8_t uuidLen, uint8_t *pUuid);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscServiceCmpl
 *
 *  \brief  This utility function processes a service discovery result.  It should be called
 *          when an ATTC_FIND_BY_TYPE_VALUE_RSP callback event is received after service
 *          discovery is initiated by calling AttcDiscService().
 *
 *  \param  pCb         Pointer to discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 */
/*************************************************************************************************/
uint8_t AttcDiscServiceCmpl(attcDiscCb_t *pCb, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscCharStart
 *
 *  \brief  This utility function starts characteristic and characteristic descriptor
 *          discovery for a service on a peer device.  The service must have been previously
 *          discovered by calling AttcDiscService() and AttcDiscServiceCmpl().
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcDiscCharStart(dmConnId_t connId, attcDiscCb_t *pCb);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscCharCmpl
 *
 *  \brief  This utility function processes a characteristic discovery result.  It should be
 *          called when an ATTC_READ_BY_TYPE_RSP or ATTC_FIND_INFO_RSP callback event is
 *          received after characteristic discovery is initiated by calling AttcDiscCharStart().
 *
 *  \param  pCb         Pointer to discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_CONTINUING if successful and the discovery procedure is continuing.
 *          ATT_SUCCESS if the discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
uint8_t AttcDiscCharCmpl(attcDiscCb_t *pCb, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscConfigStart
 *
 *  \brief  This utility function starts characteristic configuration for characteristics on a
 *          peer device.  The characteristics must have been previously discovered by calling
 *          AttcDiscCharStart() and AttcDiscCharCmpl().
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if nothing to configure.
 */
/*************************************************************************************************/
uint8_t AttcDiscConfigStart(dmConnId_t connId, attcDiscCb_t *pCb);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscConfigCmpl
 *
 *  \brief  This utility function initiates the next characteristic configuration procedure.
 *          It should be called when an ATTC_READ_RSP or ATTC_WRITE_RSP callback event is received
 *          after characteristic configuration is initiated by calling AttcDiscConfigStart().
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if configuration procedure completed successfully.
 */
/*************************************************************************************************/
uint8_t AttcDiscConfigCmpl(dmConnId_t connId, attcDiscCb_t *pCb);

/*************************************************************************************************/
/*!
 *  \fn     AttcDiscConfigResume
 *
 *  \brief  This utility function resumes the characteristic configuration procedure.  It can
 *          be called when an ATTC_READ_RSP or ATTC_WRITE_RSP callback event is received
 *          with failure status to attempt the read or write procedure again.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if configuration procedure completed successfully.
 */
/*************************************************************************************************/
uint8_t AttcDiscConfigResume(dmConnId_t connId, attcDiscCb_t *pCb);

/*************************************************************************************************/
/*!
 *  \fn     AttcMtuReq
 *
 *  \brief  For internal use only.
 *
 *  \param  connId    DM connection ID.
 *  \param  mtu       Attribute protocol MTU.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcMtuReq(dmConnId_t connId, uint16_t mtu);

/*************************************************************************************************/
/*!
 *  \fn     AttsErrorTest
 *
 *  \brief  For testing purposes only.
 *
 *  \param  status    ATT status
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsErrorTest(uint8_t status);

#ifdef __cplusplus
};
#endif

#endif /* ATT_API_H */
