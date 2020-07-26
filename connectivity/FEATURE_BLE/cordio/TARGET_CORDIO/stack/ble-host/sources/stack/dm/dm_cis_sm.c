/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM Connected Isochronous Stream (CIS) management state machine.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "dm_cis.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Column position of next state */
#define DM_CIS_NEXT_STATE           0

/*! Column position of action */
#define DM_CIS_ACTION               1

/*! Number of columns in the state machine state tables */
#define DM_CIS_NUM_COLS             2

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! DM CIS state machine state tables */
static const uint8_t dmCisStateTbl[DM_CIS_SM_NUM_STATES][DM_CIS_NUM_MSGS][DM_CIS_NUM_COLS] =
{
  /* Idle state */
  {
  /* Event                                    Next state                    Action */
  /* API_OPEN */                              {DM_CIS_SM_ST_CONNECTING,     DM_CIS_SM_ACT_OPEN},
  /* API_CLOSE */                             {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_NONE},
  /* API_ACCEPT */                            {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_NONE},
  /* API_REJECT */                            {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL_FAIL */          {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL */               {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_CIS_EST},
  /* HCI_DISCONNECT_CMPL */                   {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_REQ */                        {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_REQUEST}
  },
  /* Connecting state */
  {
  /* Event                                    Next state                    Action */
  /* API_OPEN */                              {DM_CIS_SM_ST_CONNECTING,     DM_CIS_SM_ACT_NONE},
  /* API_CLOSE */                             {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_CANCEL_OPEN},
  /* API_ACCEPT */                            {DM_CIS_SM_ST_CONNECTING,     DM_CIS_SM_ACT_NONE},
  /* API_REJECT */                            {DM_CIS_SM_ST_CONNECTING,     DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL_FAIL */          {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_CIS_EST_FAILED},
  /* HCI_LE_CIS_EST_CMD_CMPL */               {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_CIS_EST},
  /* HCI_DISCONNECT_CMPL */                   {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_CIS_EST_FAILED},
  /* HCI_LE_CIS_REQ */                        {DM_CIS_SM_ST_CONNECTING,     DM_CIS_SM_ACT_NONE}
  },
  /* Requested state */
  {
  /* Event                                    Next state                    Action */
  /* API_OPEN */                              {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_NONE},
  /* API_CLOSE */                             {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_NONE},
  /* API_ACCEPT */                            {DM_CIS_SM_ST_ACCEPTING,      DM_CIS_SM_ACT_ACCEPT},
  /* API_REJECT */                            {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_REJECT},
  /* HCI_LE_CIS_EST_CMD_CMPL_FAIL */          {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL */               {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_NONE},
  /* HCI_DISCONNECT_CMPL */                   {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_REQ */                        {DM_CIS_SM_ST_REQUESTING,     DM_CIS_SM_ACT_NONE}
  },
  /* Accepting state */
  {
  /* Event                                    Next state                    Action */
  /* API_OPEN */                              {DM_CIS_SM_ST_ACCEPTING,      DM_CIS_SM_ACT_NONE},
  /* API_CLOSE */                             {DM_CIS_SM_ST_ACCEPTING,      DM_CIS_SM_ACT_NONE},
  /* API_ACCEPT */                            {DM_CIS_SM_ST_ACCEPTING,      DM_CIS_SM_ACT_NONE},
  /* API_REJECT */                            {DM_CIS_SM_ST_ACCEPTING,      DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL_FAIL */          {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_CIS_EST_FAILED},
  /* HCI_LE_CIS_EST_CMD_CMPL */               {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_CIS_EST},
  /* HCI_DISCONNECT_CMPL */                   {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_CIS_EST_FAILED},
  /* HCI_LE_CIS_REQ */                        {DM_CIS_SM_ST_ACCEPTING,      DM_CIS_SM_ACT_NONE}
  },
  /* Connected state */
  {
  /* Event                                    Next state                    Action */
  /* API_OPEN */                              {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_NONE},
  /* API_CLOSE */                             {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_CLOSE},
  /* API_ACCEPT */                            {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_NONE},
  /* API_REJECT */                            {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL_FAIL */          {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL */               {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_NONE},
  /* HCI_DISCONNECT_CMPL */                   {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_CIS_CLOSED},
  /* HCI_LE_CIS_REQ */                        {DM_CIS_SM_ST_CONNECTED,      DM_CIS_SM_ACT_NONE}
  },
  /* Disconnecting state */
  {
  /* Event                                    Next state                    Action */
  /* API_OPEN */                              {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_NONE},
  /* API_CLOSE */                             {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_NONE},
  /* API_ACCEPT */                            {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_NONE},
  /* API_REJECT */                            {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL_FAIL */          {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_NONE},
  /* HCI_LE_CIS_EST_CMD_CMPL */               {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_CLOSE},
  /* HCI_DISCONNECT_CMPL */                   {DM_CIS_SM_ST_IDLE,           DM_CIS_SM_ACT_CIS_CLOSED},
  /* HCI_LE_CIS_REQ */                        {DM_CIS_SM_ST_DISCONNECTING,  DM_CIS_SM_ACT_NONE}
  }
};

/*! DM CIS CIG state machine state tables */
static const uint8_t dmCisCigStateTbl[DM_CIS_CIG_SM_NUM_STATES][DM_CIS_CIG_NUM_MSGS][DM_CIS_NUM_COLS] =
{
  /* Idle state */
  {
  /* Event                                    Next state                    Action */
  /* API_CONFIG */                            {DM_CIS_CIG_SM_ST_CONFIGING,  DM_CIS_CIG_SM_ACT_CONFIG},
  /* API_REMOVE */                            {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_NONE},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL_FAIL */   {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_NONE},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL */        {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_CONFIGED},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL_FAIL */       {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_NONE},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL */            {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_NONE}
  },
  /* Configuring state */
  {
  /* Event                                    Next state                    Action */
  /* API_CONFIG */                            {DM_CIS_CIG_SM_ST_CONFIGING,  DM_CIS_CIG_SM_ACT_NONE},
  /* API_REMOVE */                            {DM_CIS_CIG_SM_ST_CONFIGING,  DM_CIS_CIG_SM_ACT_NONE},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL_FAIL */   {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_CONFIG_FAILED},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL */        {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_CONFIGED},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL_FAIL */       {DM_CIS_CIG_SM_ST_CONFIGING,  DM_CIS_SM_ACT_NONE},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL */            {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_CONFIG_FAILED}
  },
  /* Configured state */
  {
  /* Event                                    Next state                    Action */
  /* API_CONFIG */                            {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_CONFIG},
  /* API_REMOVE */                            {DM_CIS_CIG_SM_ST_REMOVING,   DM_CIS_CIG_SM_ACT_REMOVE},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL_FAIL */   {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_CONFIG_FAILED},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL */        {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_CONFIGED},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL_FAIL */       {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_CONFIG_FAILED},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL */            {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_REMOVED}
  },
  /* Removing state */
  {
  /* Event                                    Next state                    Action */
  /* API_CONFIG */                            {DM_CIS_CIG_SM_ST_REMOVING,   DM_CIS_CIG_SM_ACT_NONE},
  /* API_REMOVE */                            {DM_CIS_CIG_SM_ST_REMOVING,   DM_CIS_CIG_SM_ACT_NONE},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL_FAIL */   {DM_CIS_CIG_SM_ST_REMOVING,   DM_CIS_CIG_SM_ACT_NONE},
  /* HCI_LE_SET_CIG_PARAMS_CMD_CMPL */        {DM_CIS_CIG_SM_ST_REMOVING,   DM_CIS_CIG_SM_ACT_REMOVE},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL_FAIL */       {DM_CIS_CIG_SM_ST_CONFIGED,   DM_CIS_CIG_SM_ACT_REMOVE_FAILED},
  /* HCI_LE_REMOVE_CIG_CMD_CMPL */            {DM_CIS_CIG_SM_ST_IDLE,       DM_CIS_CIG_SM_ACT_REMOVED}
  }
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! State machine action set array */
dmCisAct_t *dmCisActSet[DM_CIS_NUM_ACT_SETS];

/*************************************************************************************************/
/*!
 *  \brief  Execute the DM CIS connection state machine.
 *
 *  \param  pCcb  CIS connection control block.
 *  \param  pMsg  State machine message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmCisSmExecute(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg)
{
  dmCisAct_t  *actSet;
  uint8_t     action;
  uint8_t     event;
  uint8_t     state = pCcb->state;

  DM_TRACE_INFO2("dmCisSmExecute event=%d state=%d", pMsg->hdr.event, state);

  /* get the event */
  event = DM_MSG_MASK(pMsg->hdr.event);

  /* get action */
  action = dmCisStateTbl[state][event][DM_CIS_ACTION];

  /* set next state */
  pCcb->state = dmCisStateTbl[state][event][DM_CIS_NEXT_STATE];

  /* look up action set */
  actSet = dmCisActSet[DM_CIS_ACT_SET_ID(action)];

  /* if action set present */
  if (actSet != NULL)
  {
    /* execute action function in action set */
    (*actSet[DM_CIS_ACT_ID(action)])(pCcb, pMsg);
  }
  else
  {
     /* no action */
     dmCisSmActNone(pCcb, pMsg);
  }

  /* if restore old state requested */
  if (pCcb->inUse && (pCcb->state == DM_CIS_SM_RESTORE_OLD_STATE))
  {
    pCcb->state = state;
  }
}

/*************************************************************************************************/
/*!
*  \brief  Execute the DM CIS CIG state machine.
*
*  \param  pCigCb  CIG control block.
*  \param  pMsg    WSF message.
*
*  \return None.
*/
/*************************************************************************************************/
void dmCisCigSmExecute(dmCisCigCb_t *pCigCb, dmCisMsg_t *pMsg)
{
  uint8_t   action;
  uint8_t   event;
  uint8_t   state = pCigCb->state;

  DM_TRACE_INFO2("dmCisCigSmExecute event=%d state=%d", pMsg->hdr.event, state);

  /* get the event */
  event = DM_MSG_MASK(pMsg->hdr.event);

  /* get action */
  action = dmCisCigStateTbl[state][event][DM_CIS_ACTION];

  /* set next state */
  pCigCb->state = dmCisCigStateTbl[state][event][DM_CIS_NEXT_STATE];

  /* execute action function */
  (*dmCisCigAct[action])(pCigCb, pMsg);

  /* if restore old state requested */
  if (pCigCb->inUse && (pCigCb->state == DM_CIS_SM_RESTORE_OLD_STATE))
  {
    pCigCb->state = state;
  }
}
