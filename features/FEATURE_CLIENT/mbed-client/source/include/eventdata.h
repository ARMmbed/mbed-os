/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EVENT_DATA_H
#define EVENT_DATA_H

#include "mbed-client/m2mvector.h"

//FORWARD DECLARATION
class M2MObject;


typedef Vector<M2MObject *> M2MObjectList;

class M2MSecurity;

class EventData
{
public:
    virtual ~EventData() {}
};

class M2MSecurityData : public EventData
{
public:
    M2MSecurityData()
    :_object(NULL){}
    virtual ~M2MSecurityData() {}
    M2MSecurity  *_object;
};

class ResolvedAddressData : public EventData
{
public:
    ResolvedAddressData()
    :_address(NULL),
    _port(0){}
    virtual ~ResolvedAddressData() {}
    const M2MConnectionObserver::SocketAddress    *_address;
    uint16_t                                       _port;
};

class ReceivedData : public EventData
{
public:
    ReceivedData()
    :_data(NULL),
    _size(0),
    _port(0),
    _address(NULL){}
    virtual ~ReceivedData() {}
    uint8_t                                         *_data;
    uint16_t                                        _size;
    uint16_t                                        _port;
    const M2MConnectionObserver::SocketAddress      *_address;
};

class M2MRegisterData : public EventData
{
public:
    M2MRegisterData()
    :_object(NULL){}
    virtual ~M2MRegisterData() {}
    M2MSecurity     *_object;
    M2MObjectList    _object_list;
};

class M2MUpdateRegisterData : public EventData
{
public:
    M2MUpdateRegisterData()
    :_object(NULL),
    _lifetime(0){}
    virtual ~M2MUpdateRegisterData() {}
    M2MSecurity     *_object;
    uint32_t        _lifetime;
};


#endif //EVENT_DATA_H

