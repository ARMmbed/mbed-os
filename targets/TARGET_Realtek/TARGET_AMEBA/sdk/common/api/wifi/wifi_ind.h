/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 *
 ******************************************************************************
 * @file    wifi_ind.h
 * @author
 * @version
 * @brief   This file provides the functions related to event handler mechanism.
 ******************************************************************************
 */
  
#ifndef _WIFI_INDICATE_H
#define _WIFI_INDICATE_H
#include "wifi_conf.h"  

typedef void (*rtw_event_handler_t)(char *buf, int buf_len, int flags, void* handler_user_data );

typedef struct
{
//	rtw_event_indicate_t	event_cmd;
	rtw_event_handler_t	handler;
	void*	handler_user_data;
} event_list_elem_t;

/** 
 * @brief  Initialize the event callback list.
 * @warning  Please make sure this function has been invoked before
 *  	 using the event handler related mechanism. 
 * @param  None
 * @return  None
 */
void init_event_callback_list(void);

/**
  * @brief  Wlan driver indicate event to upper layer through wifi_indication.
  * @param[in]  event: An event reported from driver to upper layer application. Please refer to rtw_event_indicate_t enum.
  * @param[in]  buf: If it is not NUL, buf is a pointer to the buffer for message string.
  * @param[in]  buf_len: The length of the buffer.
  * @param[in]  flags: Indicate some extra information, sometimes it is 0.
  * @retval None
  * @note  If upper layer application triggers additional operations on receiving of wext_wlan_indicate, 
  *			please strictly check current stack size usage (by using uxTaskGetStackHighWaterMark() ), 
  *			and tries not to share the same stack with wlan driver if remaining stack space is not available
  *			for the following operations. 
  *			ex: using semaphore to notice another thread instead of handing event directly in wifi_indication().  
  */
extern void wifi_indication( rtw_event_indicate_t event, char *buf, int buf_len, int flags);

/** 
 * @brief  Register the event listener.
 * @param[in] event_cmds : The event command number indicated.
 * @param[in] handler_func : the callback function which will 
 *  			  receive and process the event.
 * @param[in] handler_user_data : user specific data that will be 
 *  			   passed directly to the callback function.
 * @return  RTW_SUCCESS : if successfully registers the event.
 * @return  RTW_ERROR : if an error occurred.
 * @note  Set the same event_cmds with empty handler_func will
 *  	 unregister the event_cmds.
 */
extern void wifi_reg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func, void *handler_user_data);

/**
 * @brief  Un-register the event listener.
 * @param[in] event_cmds : The event command number indicated.
 * @param[in] handler_func : the callback function which will 
 *  			  receive and process the event.
 *  
 * @return  RTW_SUCCESS : if successfully un-registers the event .
 * @return  RTW_ERROR : if an error occurred.
 */
extern void wifi_unreg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func);

#endif //_WIFI_INDICATE_H

