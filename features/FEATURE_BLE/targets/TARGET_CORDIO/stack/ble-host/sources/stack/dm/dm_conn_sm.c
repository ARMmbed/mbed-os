/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Device manager connection management state machine.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Column position of next state */
#define DM_CONN_NEXT_STATE          0

/*! Column position of action */
#define DM_CONN_ACTION              1

/*! Number of columns in the state machine state tables */
#define DM_CONN_NUM_COLS            2

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! DM Conn state machine state tables */
static const uint8_t dmConnStateTbl[DM_CONN_SM_NUM_STATES][DM_CONN_NUM_MSGS][DM_CONN_NUM_COLS] =
{
  /* Idle state */
  {
  /* Event                                    Next state                   Action */
  /* API_OPEN */                             {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_OPEN},
  /* API_CLOSE */                            {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* API_ACCEPT */                           {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_ACCEPT},
  /* API_UPDATE_MASTER */                    {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_SLAVE */                     {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_IND */                       {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_CNF */                       {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_CMPL_FAIL */                {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_CMPL */                     {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_CONN_ACCEPTED},
  /* HCI_DISCONNECT_CMPL */                  {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_UPDATE_CMPL */              {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL */   {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE},
  /* INT_UPDATE_TIMEOUT */                   {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_NONE}
  },
  /* Connecting state */
  {
  /* Event                                    Next state                   Action */
  /* API_OPEN */                             {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* API_CLOSE */                            {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_CANCEL_OPEN},
  /* API_ACCEPT */                           {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_MASTER */                    {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_SLAVE */                     {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_IND */                       {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_CNF */                       {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_CMPL_FAIL */                {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_CONN_FAILED},
  /* HCI_LE_CONN_CMPL */                     {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_CONN_OPENED},
  /* HCI_DISCONNECT_CMPL */                  {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_CONN_FAILED},
  /* HCI_LE_CONN_UPDATE_CMPL */              {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL */   {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE},
  /* INT_UPDATE_TIMEOUT */                   {DM_CONN_SM_ST_CONNECTING,    DM_CONN_SM_ACT_NONE}
  },
  /* Accepting state */
  {
  /* Event                                    Next state                   Action */
  /* API_OPEN */                             {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* API_CLOSE */                            {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_CANCEL_ACCEPT},
  /* API_ACCEPT */                           {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_MASTER */                    {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_SLAVE */                     {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_IND */                       {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_CNF */                       {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_CMPL_FAIL */                {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_ACCEPT_FAILED},
  /* HCI_LE_CONN_CMPL */                     {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_CONN_ACCEPTED},
  /* HCI_DISCONNECT_CMPL */                  {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_ACCEPT_FAILED},
  /* HCI_LE_CONN_UPDATE_CMPL */              {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL */   {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE},
  /* INT_UPDATE_TIMEOUT */                   {DM_CONN_SM_ST_ACCEPTING,     DM_CONN_SM_ACT_NONE}
  },
  /* Connected state */
  {
  /* Event                                    Next state                   Action */
  /* API_OPEN */                             {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_NONE},
  /* API_CLOSE */                            {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_CLOSE},
  /* API_ACCEPT */                           {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_MASTER */                    {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_UPDATE_MASTER},
  /* API_UPDATE_SLAVE */                     {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_UPDATE_SLAVE},
  /* L2C_UPDATE_IND */                       {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_L2C_UPDATE_IND},
  /* L2C_UPDATE_CNF */                       {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_L2C_UPDATE_CNF},
  /* HCI_LE_CONN_CMPL_FAIL */                {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_CMPL */                     {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_NONE},
  /* HCI_DISCONNECT_CMPL */                  {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_CONN_CLOSED},
  /* HCI_LE_CONN_UPDATE_CMPL */              {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_HCI_UPDATED},
  /* HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL */   {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_NONE},
  /* INT_UPDATE_TIMEOUT */                   {DM_CONN_SM_ST_CONNECTED,     DM_CONN_SM_ACT_HCI_UPDATED}
  },
  /* Disconnecting state */
  {
  /* Event                                    Next state                   Action */
  /* API_OPEN */                             {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* API_CLOSE */                            {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* API_ACCEPT */                           {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_MASTER */                    {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* API_UPDATE_SLAVE */                     {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_IND */                       {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* L2C_UPDATE_CNF */                       {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CONN_CMPL_FAIL */                {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_CONN_CLOSED},
  /* HCI_LE_CONN_CMPL */                     {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_CLOSE},
  /* HCI_DISCONNECT_CMPL */                  {DM_CONN_SM_ST_IDLE,          DM_CONN_SM_ACT_CONN_CLOSED},
  /* HCI_LE_CONN_UPDATE_CMPL */              {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL */   {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE},
  /* INT_UPDATE_TIMEOUT */                   {DM_CONN_SM_ST_DISCONNECTING, DM_CONN_SM_ACT_NONE}
  }
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! State machine action set array */
dmConnAct_t *dmConnActSet[DM_CONN_NUM_ACT_SETS];

/*************************************************************************************************/
/*!
 *  \brief  Execute the DM connection state machine.
 *
 *  \param  pCcb  Connection control block.
 *  \param  pMsg  State machine message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmExecute(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmConnAct_t       *actSet;
  uint8_t           action;
  uint8_t           event;

  DM_TRACE_INFO2("dmConnSmExecute event=%d state=%d", pMsg->hdr.event, pCcb->state);

  /* get the event */
  event = DM_MSG_MASK(pMsg->hdr.event);

  /* get action */
  action = dmConnStateTbl[pCcb->state][event][DM_CONN_ACTION];

  /* set next state */
  pCcb->state = dmConnStateTbl[pCcb->state][event][DM_CONN_NEXT_STATE];

  /* look up action set */
  actSet = dmConnActSet[DM_CONN_ACT_SET_ID(action)];

  /* if action set present */
  if (actSet != NULL)
  {
    /* execute action function in action set */
    (*actSet[DM_CONN_ACT_ID(action)])(pCcb, pMsg);
  }
  else
  {
     /* no action */
     dmConnSmActNone(pCcb, pMsg);
  }
}
