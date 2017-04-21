/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 */

#ifndef __GATT_SERVER_EVENTS_H__
#define __GATT_SERVER_EVENTS_H__

/**
 *  @brief The base class used to abstract away the callback events that can be
 *         triggered with the GATT Server.
 */
class GattServerEvents
{
public:
    /**
     * Enumeration for GattServer events.
     */
    typedef enum gattEvent_e {
        GATT_EVENT_DATA_SENT               = 1,  /**< Fired when a message was successfully sent out (notify only?) */
        GATT_EVENT_DATA_WRITTEN            = 2,  /**< Client wrote data to the server (separate into char and descriptor writes?) */
        GATT_EVENT_UPDATES_ENABLED         = 3,  /**< Notify/Indicate enabled in CCCD. */
        GATT_EVENT_UPDATES_DISABLED        = 4,  /**< Notify/Indicate disabled in CCCD. */
        GATT_EVENT_CONFIRMATION_RECEIVED   = 5,  /**< Response received from Indicate message. */
        GATT_EVENT_READ_AUTHORIZATION_REQ  = 6,  /**< Request application to authorize read. */
        GATT_EVENT_WRITE_AUTHORIZATION_REQ = 7,  /**< Request application to authorize write. */
    } gattEvent_t;
};

#endif /* ifndef __GATT_SERVER_EVENTS_H__ */
