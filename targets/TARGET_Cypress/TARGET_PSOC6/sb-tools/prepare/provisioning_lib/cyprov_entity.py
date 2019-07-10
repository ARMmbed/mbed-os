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

import json

# Cypress Entity
class Entity :
    def __init__( self , state_name , audit_name ) :
        self.state_name= state_name
        self.state_loaded= False
        self.audit_name= audit_name
        Entity.load_state(self)
        
    def load_state( self ) :
        try :
            with open(self.state_name,"r+") as f :
                self.state= json.loads(f.read())
                f.close()
                self.state_loaded= True
        except FileNotFoundError:
            self.state= {}
            self.state_loaded= False
            
    def save_state( self ) :
        if not self.state_loaded :
            raise Exception( "Internal error - state not loaded" )
        with open(self.state_name,"w") as f :
            f.write( json.dumps( self.state,indent=4 ) )
            f.close()
        
    def append_audit_record( self,record ) :
        with open(self.audit_name,"a") as f :
            f.write( json.dumps(record,indent=4) + "\n" )
            f.close()
