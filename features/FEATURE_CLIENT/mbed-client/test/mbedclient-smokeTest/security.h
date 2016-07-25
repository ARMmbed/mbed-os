

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

#ifndef __SECURITY_H__

#define __SECURITY_H__

 

#include <inttypes.h>

 

#define MBED_DOMAIN "eeforester"

#define MBED_ENDPOINT_NAME "a3e4593c-4e03-492a-9e32-320550fe442e"

 

const unsigned char PSK[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x31,0x32,0x33,0x34,0x35,0x36};

const unsigned char PSK_IDENTITY[] = {0x0F,0x0F};

 

const uint8_t SERVER_CERT[] = "-----BEGIN CERTIFICATE-----\r\n"

"MIIBmDCCAT6gAwIBAgIEVUCA0jAKBggqhkjOPQQDAjBLMQswCQYDVQQGEwJGSTEN\r\n"

"MAsGA1UEBwwET3VsdTEMMAoGA1UECgwDQVJNMQwwCgYDVQQLDANJb1QxETAPBgNV\r\n"

"BAMMCEFSTSBtYmVkMB4XDTE1MDQyOTA2NTc0OFoXDTE4MDQyOTA2NTc0OFowSzEL\r\n"

"MAkGA1UEBhMCRkkxDTALBgNVBAcMBE91bHUxDDAKBgNVBAoMA0FSTTEMMAoGA1UE\r\n"

"CwwDSW9UMREwDwYDVQQDDAhBUk0gbWJlZDBZMBMGByqGSM49AgEGCCqGSM49AwEH\r\n"

"A0IABLuAyLSk0mA3awgFR5mw2RHth47tRUO44q/RdzFZnLsAsd18Esxd5LCpcT9w\r\n"

"0tvNfBv4xJxGw0wcYrPDDb8/rjujEDAOMAwGA1UdEwQFMAMBAf8wCgYIKoZIzj0E\r\n"

"AwIDSAAwRQIhAPAonEAkwixlJiyYRQQWpXtkMZax+VlEiS201BG0PpAzAiBh2RsD\r\n"

"NxLKWwf4O7D6JasGBYf9+ZLwl0iaRjTjytO+Kw==\r\n"

"-----END CERTIFICATE-----\r\n";

 

const uint8_t CERT[] = "-----BEGIN CERTIFICATE-----\r\n"

"MIIBjDCCATCgAwIBAgIEFGtTOzAMBggqhkjOPQQDAgUAMBAxDjAMBgNVBAMTBW5z\r\n"

"cDAxMB4XDTE1MDkwMjE0MjE0MVoXDTE2MDkwMTE0MjE0MVowgYcxODA2BgNVBAMT\r\n"

"L2EzZTQ1OTNjLTRlMDMtNDkyYS05ZTMyLTMyMDU1MGZlNDQyZS9lZWZvcmVzdGVy\r\n"

"MQwwCgYDVQQLEwNBUk0xEjAQBgNVBAoTCW1iZWQgdXNlcjENMAsGA1UEBxMET3Vs\r\n"

"dTENMAsGA1UECBMET3VsdTELMAkGA1UEBhMCRkkwWTATBgcqhkjOPQIBBggqhkjO\r\n"

"PQMBBwNCAAQuxHp1a1V8pCVziDMhuuSt5zbrduEe+0y+UXuPMj2k6uoEuVCYI4+k\r\n"

"JU6WSfVYAFSVVka0JhIMKB289xU2G5+EMAwGCCqGSM49BAMCBQADSAAwRQIgeWQK\r\n"

"su17hcY+fQxY8H8BgrGy0z+RKJr51WZl2LR9ZSwCIQDKPx9LYzwSw8y1hugpcsc/\r\n"

"ul5++FNrYwLE44l3pW87yw==\r\n"

"-----END CERTIFICATE-----\r\n";

 

const uint8_t KEY[] = "-----BEGIN PRIVATE KEY-----\r\n"

"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg9RFcAVbOKG1LbIF8\r\n"

"kNqA8dznz4wnD+B/ghYNQMHtL/qhRANCAAQuxHp1a1V8pCVziDMhuuSt5zbrduEe\r\n"

"+0y+UXuPMj2k6uoEuVCYI4+kJU6WSfVYAFSVVka0JhIMKB289xU2G5+E\r\n"

"-----END PRIVATE KEY-----\r\n";

 

#endif //__SECURITY_H__
