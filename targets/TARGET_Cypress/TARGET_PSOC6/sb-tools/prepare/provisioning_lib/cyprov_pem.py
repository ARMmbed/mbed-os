# Copyright 2019 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from jwcrypto import jwk
import json

def pretty_search(dict_or_list, key_to_search, search_for_first_only=False):
    """
    Give it a dict or a list of dicts and a dict key (to get values of),
    it will search through it and all containing dicts and arrays
    for all values of dict key you gave, and will return you set of them
    unless you wont specify search_for_first_only=True

    :param dict_or_list: 
    :param key_to_search: 
    :param search_for_first_only: 
    :return: 
    """
    search_result = set()
    if isinstance(dict_or_list, dict):
        for key in dict_or_list:
            key_value = dict_or_list[key]
            if key == key_to_search:
                if search_for_first_only:
                    return key_value
                else:
                    search_result.add(key_value)
            if isinstance(key_value, dict) or isinstance(key_value, list) or isinstance(key_value, set):
                _search_result = pretty_search(key_value, key_to_search, search_for_first_only)
                if _search_result and search_for_first_only:
                    return _search_result
                elif _search_result:
                    for result in _search_result:
                        search_result.add(result)
    elif isinstance(dict_or_list, list) or isinstance(dict_or_list, set):
        for element in dict_or_list:
            if isinstance(element, list) or isinstance(element, set) or isinstance(element, dict):
                _search_result = pretty_search(element, key_to_search, search_result)
                if _search_result and search_for_first_only:
                    return _search_result
                elif _search_result:
                    for result in _search_result:
                        search_result.add(result)
    return search_result if search_result else None

class PemKey:
    
    def __init__(self, jwk_file = None, item = None):
        if jwk_file != None:
            with open(jwk_file) as f:
                jwk_str = f.read()
                self.jwk = json.loads(jwk_str)
        
        if item != None:
            self.jwk = pretty_search(self.jwk, item, search_for_first_only=True)
            
    def save(self, file = None, private_key=False):
        key = jwk.JWK(**self.jwk)
        pem_str = key.export_to_pem(private_key, password=None)
        if file != None:
            with open(file, 'wb') as f:
                f.write(pem_str)
        else:
            print(pem_str)
                
    def load(self, jwk):
        self.jwk = jwk
