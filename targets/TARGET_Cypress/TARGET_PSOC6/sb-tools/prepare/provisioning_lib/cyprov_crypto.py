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

from jwcrypto import jwk, jws, jwt
from jwcrypto.common import base64url_decode
from jwcrypto.common import json_decode
from datetime import datetime
import json
import copy

class crypto :
    
    def create_jwk() :
        key = jwk.JWK.generate(kty='EC', crv='P-256', use='sig')
        pub_key= json.loads( key.export(private_key=False) )
        priv_key= json.loads( key.export(private_key=True) )
    
        print( "create_jwk()= " + json.dumps( pub_key, indent=4 ) )
        return priv_key,pub_key

    def create_jwt( payload , key ) :
        token= jwt.JWT( header={"alg": "ES256"},claims=payload )
        token.make_signed_token( jwk.JWK( **key ) )
        txt= token.serialize(compact=True)
          
        print( "create_jwt()= " + json.dumps( crypto.readable_jwt(txt) ,indent=4 ))
        return txt
        
    def readable_jwt( txt ) :
        """
            Convert a JWT token in base64url into a readable dictionary object
            with decoded payload and header for printing and logging
        """
        lst= txt.split('.')
        readable= {}
        readable["protected"]= json_decode(base64url_decode(lst[0]))
        readable["payload"]= json_decode(base64url_decode(lst[1]))
        readable["signature"]= lst[2]
          
        # create readable timestamps for exp/iat claims
        payload= readable["payload"]
        if "iat" in payload :
            t= payload["iat"]
            if isinstance(t,int) :
                t= datetime.fromtimestamp(t).isoformat(' ')  
                payload["iat"]= t
        if "exp" in payload :
            t= payload["exp"]
            if isinstance(t,int) :
                t= datetime.fromtimestamp(t).isoformat(' ')  
                payload["exp"]= t
                
        print(json.dumps(readable,indent=4,sort_keys=False))
        return readable
        
    def dump_jwt( txt,file_name ) :
        """
            Dumps a JWT dictionary object into a text file
        """
        with open(file_name,"w") as f :
            f.write(txt)
            f.close()

    def read_jwt( file_name ) :
        """
            Reads a JWT dictionary object from a text file
        """
        with open(file_name,"r") as f :
            txt = f.read()
            f.close()
            
        return txt

    def jwt_payload( txt ) :
        """
            Returns the payload of a JWT without validating it's signature
            Sometimes used for tokens that contain a public key in its payload, where the signature proves possesion of the corresponding private key
            In that case, the payload is needed to obtain the public key with which to then validate the JWT
        """
        # split the token
        lst= txt.split('.')
        payload= lst[1]
        obj= json_decode(base64url_decode(payload))        
        return obj
        
    def validate_jwt( txt , key ) :
        """
            Validates a signed JWT
        """
        try :
            jwt.JWT(key=jwk.JWK(**key),jwt=txt)
            print( "  JWT signature is valid" )
            return True
        except :
            print( "  JWT signature is not valid" )
            return False
             
    def create_x509_cert( pub_key , priv_key , prod_id , die_id=None , dev_id=None ) :
        """
            TODO: create a X.509 certificate here certifying pub_key, signed with private_key
        """
        cert= "CertificateToBeDone(die_id={},dev_id={},prod_id={})".format(die_id,dev_id,prod_id)
        return cert
    