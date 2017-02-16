/*
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
 */
#ifndef ROM_WLAN_RAM_MAP_H
#define ROM_WLAN_RAM_MAP_H

struct _rom_wlan_ram_map {
	unsigned char * (*rtw_malloc)(unsigned int sz);
	void (*rtw_mfree)(unsigned char *pbuf, unsigned int sz);
};

#endif	/* ROM_WLAN_RAM_MAP_H */
