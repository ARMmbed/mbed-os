"""
 mbed CMSIS-DAP debugger
 Copyright (c) 2006-2013 ARM Limited

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
"""

try:
    from websocket import create_connection
except:
    pass
        
class GDBWebSocket():
    def __init__(self, url):
        self.url = url
        self.wss = None
        return
    
    def connect(self):
        self.wss = None
        try:
            self.wss = create_connection(self.url)
        except:
            pass
        return self.wss
    
    def read(self):
        return self.wss.recv()
    
    def write(self, data):
        return self.wss.send(data)
    
    def close(self):
        return self.wss.close()
    
    def setBlocking(self, blocking):
        if blocking != 0:
            self.wss.settimeout(None)
        else:
            self.wss.settimeout(0)
            