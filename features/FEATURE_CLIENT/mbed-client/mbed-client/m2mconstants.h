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
#ifndef M2MCONSTANTS_H
#define M2MCONSTANTS_H

#include <stdint.h>
#include "m2mconfig.h"

const int MAX_VALUE_LENGTH = 256;
const int BUFFER_LENGTH = 1152;

const int32_t MINIMUM_REGISTRATION_TIME = 60; //in seconds
const uint64_t ONE_SECOND_TIMER = 1;
const uint16_t MAX_ALLOWED_STRING_LENGTH = 64;
const uint16_t OPTIMUM_LIFETIME = 3600;
const uint16_t REDUCE_LIFETIME = 900;
const float REDUCTION_FACTOR = 0.75f;

// values per: draft-ietf-core-observe-16
// OMA LWM2M CR ref.
#define START_OBSERVATION 0
#define STOP_OBSERVATION 1

#define COAP "coap://"
#define COAPS  "coaps://"
#define BOOTSTRAP_URI "bs"
// PUT attributes to be checked from server
#define EQUAL  "="
#define AMP  "&"
#define PMIN  "pmin"
#define PMAX  "pmax"
#define GT  "gt"
#define LT  "lt"
#define ST_SIZE  "st"
#define STP  "stp"
#define CANCEL  "cancel"

// just a helper for "String default_value = "";" pattern
extern const String EMPTY;

//LWM2MOBJECT NAME/ID
#define M2M_SECURITY_ID  "0"
#define M2M_SERVER_ID  "1"
#define M2M_ACCESS_CONTROL_ID  "2"
#define M2M_DEVICE_ID  "3"
#define M2M_CONNECTIVITY_MONITOR_ID  "4"
#define M2M_FIRMWARE_ID  "5"
#define M2M_LOCATION_ID  "6"
#define M2M_CONNECTIVITY_STATISTICS_ID  "7"
#define RESERVED_ID  "8"

//OMA RESOURCE TYPE
#define OMA_RESOURCE_TYPE  "" //oma.lwm2m

//DEVICE RESOURCES
#define DEVICE_MANUFACTURER  "0"
#define DEVICE_DEVICE_TYPE  "17"
#define DEVICE_MODEL_NUMBER  "1"
#define DEVICE_SERIAL_NUMBER  "2"
#define DEVICE_HARDWARE_VERSION  "18"
#define DEVICE_FIRMWARE_VERSION  "3"
#define DEVICE_SOFTWARE_VERSION  "19"
#define DEVICE_REBOOT  "4"
#define DEVICE_FACTORY_RESET  "5"
#define DEVICE_AVAILABLE_POWER_SOURCES  "6"
#define DEVICE_POWER_SOURCE_VOLTAGE  "7"
#define DEVICE_POWER_SOURCE_CURRENT  "8"
#define DEVICE_BATTERY_LEVEL  "9"
#define DEVICE_BATTERY_STATUS  "20"
#define DEVICE_MEMORY_FREE  "10"
#define DEVICE_MEMORY_TOTAL  "21"
#define DEVICE_ERROR_CODE  "11"
#define DEVICE_RESET_ERROR_CODE  "12"
#define DEVICE_CURRENT_TIME  "13"
#define DEVICE_UTC_OFFSET  "14"
#define DEVICE_TIMEZONE  "15"
#define DEVICE_SUPPORTED_BINDING_MODE  "16"
#define BINDING_MODE_UDP  "U"
#define BINDING_MODE_UDP_QUEUE  "UQ"
#define BINDING_MODE_SMS  "S"
#define BINDING_MODE_SMS_QUEUE  "SQ"
#define ERROR_CODE_VALUE  "0"

//SECURITY RESOURCES
#define SECURITY_M2M_SERVER_URI  "0"
#define SECURITY_BOOTSTRAP_SERVER  "1"
#define SECURITY_SECURITY_MODE  "2"
#define SECURITY_PUBLIC_KEY  "3"
#define SECURITY_SERVER_PUBLIC_KEY  "4"
#define SECURITY_SECRET_KEY  "5"
#define SECURITY_SMS_SECURITY_MODE  "6"
#define SECURITY_SMS_BINDING_KEY  "7"
#define SECURITY_SMS_BINDING_SECRET_KEY  "8"
#define SECURITY_M2M_SERVER_SMS_NUMBER  "9"
#define SECURITY_SHORT_SERVER_ID  "10"
#define SECURITY_CLIENT_HOLD_OFF_TIME  "11"

//SERVER RESOURCES
#define SERVER_SHORT_SERVER_ID  "0"
#define SERVER_LIFETIME  "1"
#define SERVER_DEFAULT_MIN_PERIOD  "2"
#define SERVER_DEFAULT_MAX_PERIOD  "3"
#define SERVER_DISABLE  "4"
#define SERVER_DISABLE_TIMEOUT  "5"
#define SERVER_NOTIFICATION_STORAGE  "6"
#define SERVER_BINDING  "7"
#define SERVER_REGISTRATION_UPDATE  "8"

//FIRMWARE RESOURCES
#define FIRMWARE_PACKAGE  "0"
#define FIRMWARE_PACKAGE_URI  "1"
#define FIRMWARE_UPDATE  "2"
#define FIRMWARE_STATE  "3"
#define FIRMWARE_UPDATE_SUPPORTED_OBJECTS  "4"
#define FIRMWARE_UPDATE_RESULT  "5"
#define FIRMWARE_PACKAGE_NAME  "6"
#define FIRMWARE_PACKAGE_VERSION  "7"

// TLV serializer / deserializer
const uint8_t TYPE_RESOURCE = 0xC0;
const uint8_t TYPE_MULTIPLE_RESOURCE = 0x80;
const uint8_t TYPE_RESOURCE_INSTANCE = 0x40;
const uint8_t TYPE_OBJECT_INSTANCE = 0x0;

const uint8_t ID8 = 0x0;
const uint8_t ID16 = 0x20;

const uint8_t LENGTH8 = 0x08;
const uint8_t LENGTH16 = 0x10;
const uint8_t LENGTH24 = 0x18;

const uint8_t COAP_CONTENT_OMA_TLV_TYPE = 99;
const uint8_t COAP_CONTENT_OMA_JSON_TYPE = 100;
const uint8_t COAP_CONTENT_OMA_OPAQUE_TYPE = 42;

const uint16_t MAX_UNINT_16_COUNT = 65535;

#endif // M2MCONSTANTS_H
