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

from cyprov_entity import Entity
from cyprov_crypto import crypto
from cyprov_types import types
from datetime import datetime
from datetime import timedelta

# Oem (Customr) Entity
class OemEntity(Entity) :
    def __init__( self , state_name , audit_name ) :
        Entity.__init__(self,state_name,audit_name)
        
    def create_entity( self , chain_of_trust=[] ) :
        """
        Creates the Oem entity.
        Creates the Oem main key-pair and returns nothing.
        """
        oem_priv_key,oem_pub_key= crypto.create_jwk()
        self.state["oem_priv_key"]= oem_priv_key
        self.state["oem_pub_key"]= oem_pub_key
        self.state["oem_chain_of_trust"]= chain_of_trust
        
    def create_rot_authorization( self , signing_pkg ) :
        """
            OEM creates a root-of-trust authorization package based on the signing key package from the HSM
            It's a JWT with two main parts: 
            - a rot_auth authorization token that is sent to the device
            - a chain of X509 certificates that establishses the OEM's trust in the HSM signing key
              Note that this chain is not used inside the device and used only by 3rd parties
        """
        oem_pub_key= self.state["oem_pub_key"]
        oem_priv_key= self.state["oem_priv_key"]
        oem_chain_of_trust= self.state["oem_chain_of_trust"]
        
        # get HSM public key and check CY authorization
        signing_pkg_payload= crypto.jwt_payload(signing_pkg)
        cy_auth= signing_pkg_payload["cy_auth"]
        cy_auth_payload= crypto.jwt_payload(cy_auth)
        cy_pub_key= cy_auth_payload["cy_pub_key"]
        hsm_pub_key= cy_auth_payload["hsm_pub_key"]
        if not crypto.validate_jwt( cy_auth , cy_pub_key ) :
            raise Exception( "Invalid signature on Cypress HSM authorization in request" )
        if cy_auth_payload["type"] != types.CY_AUTH_HSM :
            raise Exception( "Invalid type for Cypress HSM authorization in request" )
        if datetime.fromtimestamp(cy_auth_payload["exp"]) < datetime.now() :
            raise Exception( "Cypress HSM authorization expired" )

        # validate HSM request itself
        signing_pub_key= signing_pkg_payload["signing_pub_key"]
        prod_id= signing_pkg_payload["prod_id"]
        if not crypto.validate_jwt( signing_pkg , hsm_pub_key ) :
            raise Exception( "Invalid signature on HSM signing key package" )
        if signing_pkg_payload["type"] != types.HSM_SIGNING_KEY_PKG :
            raise Exception( "Invalid type on HSM signing key package" )
        if datetime.fromtimestamp(signing_pkg_payload["exp"]) < datetime.now() :
            raise Exception( "HSM signing key package expired" )
            
        # create the RoT transfer authorization (that will go to the device)
        payload= {}
        payload["type"]= types.OEM_ROT_AUTH
        payload["oem_pub_key"]= oem_pub_key
        payload["hsm_pub_key"]= hsm_pub_key
        payload["prod_id"]= prod_id
        payload["iat"]= int(datetime.now().timestamp())
        rot_auth= crypto.create_jwt( payload , oem_priv_key )

        # create the chain of trust
        cert= crypto.create_x509_cert( signing_pub_key , oem_priv_key , prod_id )
        chain_of_trust= oem_chain_of_trust + [ cert ]

        # create the response
        exp= datetime.now() + timedelta(7)
        payload= {}
        payload["type"]= types.OEM_ROT_AUTH_PKG
        payload["iat"]= int(datetime.now().timestamp())
        payload["exp"]= int(exp.timestamp())
        payload["prod_id"]= prod_id
        payload["rot_auth"]= rot_auth
        payload["chain_of_trust"]= chain_of_trust
        rot_auth_pkg= crypto.create_jwt( payload , oem_priv_key )
        
        # create audit record
        signing_pkg_readable= crypto.readable_jwt(signing_pkg)
        signing_pkg_readable["payload"]["cy_auth"]= crypto.readable_jwt(signing_pkg_readable["payload"]["cy_auth"])
        signing_pkg_readable["payload"]["disti_auth"]= crypto.readable_jwt(signing_pkg_readable["payload"]["disti_auth"])
        rot_auth_pkg_readable= crypto.readable_jwt(rot_auth_pkg)
        rot_auth_pkg_readable["payload"]["rot_auth"]= crypto.readable_jwt(rot_auth_pkg_readable["payload"]["rot_auth"])
        record= {}
        record["type"]= types.OEM_ROT_AUTH_PKG
        record["iat"]= datetime.now().isoformat(' ')
        record["signing_pkg"]= signing_pkg_readable
        record["rot_auth_pkg"]= rot_auth_pkg_readable
        self.append_audit_record(record)
        
        return rot_auth_pkg

    def create_provision_request( self , blob ) :
        """
            The OEM can create a request for provisioning by signing a keys & policies blob with its private key
            Note that blob must contain at least the prod_id field
        """
        # create the request
        oem_priv_key= self.state["oem_priv_key"]
        prov_req= crypto.create_jwt( blob , oem_priv_key )
        
        # create audit record
        record= {}
        record["type"]= types.OEM_PROV_REQ
        record["iat"]= datetime.now().isoformat(' ')
        record["prod_id"]= blob["prod_id"]
        record["prov_req"]= crypto.readable_jwt(prov_req)
        self.append_audit_record(record)
        
        return prov_req
    
    def pack_rot_auth( self, prod_id, hsm_pub_key ):
        oem_pub_key = self.state["oem_pub_key"]
        oem_priv_key = self.state["oem_priv_key"]
        
        payload= {}
        payload["type"] = types.OEM_ROT_AUTH
        payload["oem_pub_key"] = oem_pub_key
        payload["hsm_pub_key"] = hsm_pub_key
        payload["prod_id"] = prod_id
        payload["iat"] = int(datetime.now().timestamp())
        rot_auth = crypto.create_jwt( payload , oem_priv_key )
        
        return rot_auth
 