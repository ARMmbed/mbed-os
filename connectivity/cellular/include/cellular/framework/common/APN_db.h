/* mbed Microcontroller Library
 * Copyright (c) 2017 ublox, ARM Limited
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

/* ----------------------------------------------------------------
   APN stands for Access Point Name, a setting on your modem or phone
   that identifies an external network your phone can access for data
   (e.g. 3G or 4G Internet service on your phone).

   The APN settings can be forced when calling the join function.
   Below is a list of known APNs that us used if no apn config
   is forced. This list could be extended by other settings.

   For further reading:
   wiki apn: http://en.wikipedia.org/wiki/Access_Point_Name
   wiki mcc/mnc: http://en.wikipedia.org/wiki/Mobile_country_code
   google: https://www.google.de/search?q=APN+list
---------------------------------------------------------------- */
#ifndef APN_DB_H_
#define APN_DB_H_

#include <string.h>

/**
 * Helper to extract a field from the cfg string
 */
#define _APN_GET(cfg) \
    *cfg ? cfg : NULL; \
    cfg  += strlen(cfg) + 1

/**
 * Configuring APN by extraction from IMSI and matching the table.
 *
 * @param imsi  string containing IMSI
 */
const char *apnconfig(const char *imsi);

#endif // #define APN_DB_H_
