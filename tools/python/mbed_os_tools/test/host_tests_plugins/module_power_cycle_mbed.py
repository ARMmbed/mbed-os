# Copyright (c) 2018, Arm Limited and affiliates.
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

import os
import json
import time
import requests
from .host_test_plugins import HostTestPluginBase


class HostTestPluginPowerCycleResetMethod(HostTestPluginBase):

    # Plugin interface
    name = 'HostTestPluginPowerCycleResetMethod'
    type = 'ResetMethod'
    stable = True
    capabilities = ['power_cycle']
    required_parameters = ['target_id', 'device_info']

    def __init__(self):
        """ ctor
        """
        HostTestPluginBase.__init__(self)

    def setup(self, *args, **kwargs):
        """! Configure plugin, this function should be called before plugin execute() method is used.
        """
        return True

    def execute(self, capability, *args, **kwargs):
        """! Executes capability by name

        @param capability Capability name
        @param args Additional arguments
        @param kwargs Additional arguments
        @details Each capability e.g. may directly just call some command line program or execute building pythonic function
        @return Capability call return value
        """
        if 'target_id' not in kwargs or not kwargs['target_id']:
            self.print_plugin_error("Error: This plugin requires mbed target_id")
            return False

        if 'device_info' not in kwargs or type(kwargs['device_info']) is not dict:
            self.print_plugin_error("Error: This plugin requires dict parameter 'device_info' passed by the caller.")
            return False

        result = False
        if self.check_parameters(capability, *args, **kwargs) is True:
            if capability in HostTestPluginPowerCycleResetMethod.capabilities:
                target_id = kwargs['target_id']
                device_info = kwargs['device_info']
                ret = self.__get_mbed_tas_rm_addr()
                if ret:
                    ip, port = ret
                    result = self.__hw_reset(ip, port, target_id, device_info)
        return result

    def __get_mbed_tas_rm_addr(self):
        """
        Get IP and Port of mbed tas rm service.
        :return:
        """
        try:
            ip = os.environ['MBED_TAS_RM_IP']
            port = os.environ['MBED_TAS_RM_PORT']
            return ip, port
        except KeyError as e:
            self.print_plugin_error("HOST: Failed to read environment variable (" + str(e) + "). Can't perform hardware reset.")

        return None

    def __hw_reset(self, ip, port, target_id, device_info):
        """
        Reset target device using TAS RM API

        :param ip:
        :param port:
        :param target_id:
        :param device_info:
        :return:
        """

        switch_off_req = {
            "name": "switchResource",
            "sub_requests": [
                {
                    "resource_type": "mbed_platform",
                    "resource_id": target_id,
                    "switch_command": "OFF"
                }
            ]
        }


        switch_on_req = {
            "name": "switchResource",
            "sub_requests": [
                {
                    "resource_type": "mbed_platform",
                    "resource_id": target_id,
                    "switch_command": "ON"
                }
            ]
        }

        result = False

        # reset target
        switch_off_req = self.__run_request(ip, port, switch_off_req)
        if switch_off_req is None:
            self.print_plugin_error("HOST: Failed to communicate with TAS RM!")
            return result

        if "error" in switch_off_req['sub_requests'][0]:
            self.print_plugin_error("HOST: Failed to reset target. error = %s" % switch_off_req['sub_requests'][0]['error'])
            return result

        def poll_state(required_state):
            switch_state_req = {
                "name": "switchResource",
                "sub_requests": [
                    {
                        "resource_type": "mbed_platform",
                        "resource_id": target_id,
                        "switch_command": "STATE"
                    }
                ]
            }
            resp = self.__run_request(ip, port, switch_state_req)
            start = time.time()
            while resp and (resp['sub_requests'][0]['state'] != required_state or (required_state == 'ON' and
                            resp['sub_requests'][0]["mount_point"] == "Not Connected")) and (time.time() - start) < 300:
                time.sleep(2)
                resp = self.__run_request(ip, port, resp)
            return resp

        poll_state("OFF")

        self.__run_request(ip, port, switch_on_req)
        resp = poll_state("ON")
        if resp and resp['sub_requests'][0]['state'] == 'ON' and resp['sub_requests'][0]["mount_point"] != "Not Connected":
            for k, v in resp['sub_requests'][0].viewitems():
                device_info[k] = v
            result = True
        else:
            self.print_plugin_error("HOST: Failed to reset device %s" % target_id)

        return result

    @staticmethod
    def __run_request(ip, port, request):
        """

        :param request:
        :return:
        """
        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
        get_resp = requests.get("http://%s:%s/" % (ip, port), data=json.dumps(request), headers=headers)
        resp = get_resp.json()
        if get_resp.status_code == 200:
            return resp
        else:
            return None


def load_plugin():
    """! Returns plugin available in this module
    """
    return HostTestPluginPowerCycleResetMethod()
