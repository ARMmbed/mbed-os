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

import os
from cyprov_types import types
from cyprov_entity import Entity
from cyprov_crypto import crypto
from datetime import datetime
from datetime import timedelta


# HSM Entity
class HsmEntity(Entity):
    def __init__(self, state_name, audit_name):
        Entity.__init__(self, state_name, audit_name)

    def create_entity(self):
        """
        Creates the HSM entity.
        Creates the hsm_priv_key,hsm_pub_key key-pair.
        """
        hsm_priv_key, hsm_pub_key = crypto.create_jwk()
        self.state["hsm_priv_key"] = hsm_priv_key
        self.state["hsm_pub_key"] = hsm_pub_key

    def request_disti_authorization(self):
        """
            HSM creates a request for authorization to Distributor
            It includes just the HSM public key and a one week expiration limit
            We use a JWT to prove knowledge of the private key and convey an expiration limit
        """
        hsm_priv_key = self.state["hsm_priv_key"]
        hsm_pub_key = self.state["hsm_pub_key"]

        # create the request
        exp = datetime.now() + timedelta(7)
        payload = {}
        payload["type"] = types.HSM_REQ_DISTI_AUTH
        payload["hsm_pub_key"] = hsm_pub_key
        payload["iat"] = int(datetime.now().timestamp())
        payload["exp"] = int(exp.timestamp())
        auth_req = crypto.create_jwt(payload, hsm_priv_key)

        # Create audit record
        record = {}
        record["type"] = types.HSM_REQ_DISTI_AUTH
        record["iat"] = datetime.now().isoformat(' ')
        record["auth_req"] = crypto.readable_jwt(auth_req)
        self.append_audit_record(record)

        return auth_req

    def install_disti_auth(self, disti_auth):
        """
            Install distributor authorization.
            Returns nothingraise Exception(
        """
        hsm_pub_key = self.state["hsm_pub_key"]

        # Validate the distributor authorization
        disti_auth_payload = crypto.jwt_payload(disti_auth)
        disti_pub_key = disti_auth_payload["disti_pub_key"]
        if not crypto.validate_jwt(disti_auth, disti_pub_key):
            raise Exception("Invalid signature for distributor authorization")
        if disti_auth_payload["type"] != types.DISTI_AUTH_HSM:
            raise Exception("Invalid type for distributor authorization")
        if disti_auth_payload["hsm_pub_key"] != hsm_pub_key:
            raise Exception("Distributor authorization is not for this HSM")
        if datetime.fromtimestamp(disti_auth_payload["exp"]) < datetime.now():
            raise Exception("Distributor authorization expired")

        # install distributor key and authorization
        self.state["disti_pub_key"] = disti_pub_key
        self.state["disti_auth"] = disti_auth
        self.state["disti_auth_readable"] = crypto.readable_jwt(disti_auth)

        # create audit record
        record = {}
        record["type"] = types.DISTI_AUTH_HSM
        record["iat"] = datetime.now().isoformat(' ')
        record["disti_auth"] = crypto.readable_jwt(disti_auth)
        self.append_audit_record(record)

    def request_cy_authorization(self, cy_pub_key=None):
        """
            HSM creates a request for authorization to CY
            It contains the HSM public key, the distributor authorization and a 1 week expiration
            The JWT proves knowledge of the private key and the distributor authorization is used by CY to identify the request
        """
        hsm_priv_key = self.state["hsm_priv_key"]
        hsm_pub_key = self.state["hsm_pub_key"]
        disti_auth = self.state["disti_auth"]

        # create the authorization request
        exp = datetime.now() + timedelta(7)
        payload = {}
        payload["type"] = types.HSM_REQ_CY_AUTH
        payload["hsm_pub_key"] = hsm_pub_key
        payload["disti_auth"] = disti_auth
        payload["iat"] = int(datetime.now().timestamp())
        payload["exp"] = int(exp.timestamp())
        if cy_pub_key != None:
            payload["cy_pub_key"] = cy_pub_key
        auth_req = crypto.create_jwt(payload, hsm_priv_key)

        # create audit record
        cy_auth_readable = crypto.readable_jwt(auth_req)
        cy_auth_readable["payload"]["disti_auth"] = crypto.readable_jwt(cy_auth_readable["payload"]["disti_auth"])
        record = {}
        record["type"] = types.HSM_REQ_CY_AUTH
        record["iat"] = datetime.now().isoformat(' ')
        record["cy_auth_req"] = cy_auth_readable
        self.append_audit_record(record)

        # install the cy_pub_key if one is given (allows for explicit binding to Cypress or some other supplier)
        self.state["cy_pub_key"] = cy_pub_key

        return auth_req

    def install_cy_auth(self, cy_auth, cy_pub_key=None):
        """
            Install Cypress authorization.
            First checks the signature on the token and then just stores it
            The expiration date on the token is enforced by the HSM only (since device has not time).
            The authorization privilages (which may contain serial number and/or wounding limitations) are enforced by the device.
        """
        hsm_pub_key = self.state["hsm_pub_key"]

        # validate the authorization
        cy_auth_payload = crypto.jwt_payload(cy_auth)
        if cy_pub_key == None:
            cy_pub_key = cy_auth_payload["cy_pub_key"]
        elif cy_auth_payload["cy_pub_key"] != cy_pub_key:
            raise Exception("Cypress authorization is not from Cypress")
        if not crypto.validate_jwt(cy_auth, cy_pub_key):
            raise Exception("Invalid signature for CY HSM authorization")
        if cy_auth_payload["type"] != types.CY_AUTH_HSM:
            raise Exception("Invalid type for CY HSM authoriation")
        if datetime.fromtimestamp(cy_auth_payload["exp"]) < datetime.now():
            raise Exception("CY HSM authorization is expired")
        if cy_auth_payload["cy_pub_key"] != cy_pub_key:
            raise Exception("Authorization appears to be not from Cypress")

        # install the authorization
        self.state["cy_pub_key"] = cy_pub_key
        self.state["cy_auth"] = cy_auth
        self.state["cy_auth_readable"] = crypto.readable_jwt(cy_auth)
        self.state["products"] = {}

        # create audit record
        record = {}
        record["type"] = types.CY_AUTH_HSM
        record["iat"] = datetime.now().isoformat(' ')
        record["cy_auth"] = crypto.readable_jwt(cy_auth)
        self.append_audit_record(record)

    def create_signing_key(self, prod_id):
        """
            Creates a signing-key package that an OEM uses to create its chain of trust
            One or more OEM projects may share the same signing key
            An HSM can maintain multiple signing keys for multiple OEMs, identified by a string "prod_id"
            HSM operator must make sure that "prod_id" is unique across all of its customers, e.g. by including the customer name or ID
        """
        cy_auth = self.state["cy_auth"]
        disti_auth = self.state["disti_auth"]
        hsm_priv_key = self.state["hsm_priv_key"]

        # create the signing key
        signing_priv_key, signing_pub_key = crypto.create_jwk()

        # create the token
        exp = datetime.now() + timedelta(7)
        payload = {}
        payload["type"] = types.HSM_SIGNING_KEY_PKG
        payload["iat"] = int(datetime.now().timestamp())
        payload["exp"] = int(exp.timestamp())
        payload["prod_id"] = prod_id
        payload["signing_pub_key"] = signing_pub_key
        payload["cy_auth"] = cy_auth
        payload["disti_auth"] = disti_auth
        signing_pkg = crypto.create_jwt(payload, hsm_priv_key)

        # store signing key for later use
        prod = {}
        prod["iat"] = int(datetime.now().timestamp())
        prod["signing_priv_key"] = signing_priv_key
        prod["signing_pub_key"] = signing_pub_key
        self.state["products"][prod_id] = prod

        # create audit record
        signing_pkg_readable = crypto.readable_jwt(signing_pkg)
        signing_pkg_readable["payload"]["cy_auth"] = crypto.readable_jwt(signing_pkg_readable["payload"]["cy_auth"])
        signing_pkg_readable["payload"]["disti_auth"] = crypto.readable_jwt(
            signing_pkg_readable["payload"]["disti_auth"])
        record = {}
        record["type"] = types.HSM_SIGNING_KEY_PKG
        record["iat"] = datetime.now().isoformat(' ')
        record["signing_pkg"] = signing_pkg_readable
        self.append_audit_record(record)

        return signing_pkg

    def install_rot_authorization(self, rot_auth_pkg):
        """
            The HSM checks the response package and the rot_auth token itself and simply stores it for later use
        """
        hsm_pub_key = self.state["hsm_pub_key"]

        rot_auth_pkg_payload = crypto.jwt_payload(rot_auth_pkg)
        prod_id = rot_auth_pkg_payload["prod_id"]
        rot_auth = rot_auth_pkg_payload["rot_auth"]
        chain_of_trust = rot_auth_pkg_payload["chain_of_trust"]

        # validate the RoT authorization
        rot_auth_payload = crypto.jwt_payload(rot_auth)
        oem_pub_key = rot_auth_payload["oem_pub_key"]
        if not crypto.validate_jwt(rot_auth, oem_pub_key):
            raise Exception("Invalid signature on OEM root-of-trust authorization")
        if rot_auth_payload["type"] != types.OEM_ROT_AUTH:
            raise Exception("Invalid type for OEM root-of-trust authorization")
        if rot_auth_payload["hsm_pub_key"] != hsm_pub_key:
            raise Exception("Invalid HSM public key in OEM root-of-trust authorization")
        if rot_auth_payload["prod_id"] != prod_id:
            raise Exception("Invalid prod_id in OEM root-of-trust authorization")

        # validate the package itself
        if not crypto.validate_jwt(rot_auth_pkg, oem_pub_key):
            raise Exception("Invalid signature on OEM root-of-trust authorization package")
        if rot_auth_pkg_payload["type"] != types.OEM_ROT_AUTH_PKG:
            raise Exception("Invalid type for OEM root-of-trust authorization package")
        if datetime.fromtimestamp(rot_auth_pkg_payload["exp"]) < datetime.now():
            raise Exception("OEM root-of-trust authorization package is expired")

        # store the result for later usage
        prod = self.state["products"][prod_id]
        prod["oem_pub_key"] = oem_pub_key
        prod["rot_auth"] = rot_auth
        prod["rot_auth_readable"] = crypto.readable_jwt(rot_auth)
        prod["chain_of_trust"] = chain_of_trust

        # create audit record
        rot_auth_pkg_readable = crypto.readable_jwt(rot_auth_pkg)
        rot_auth_pkg_readable["payload"]["rot_auth"] = crypto.readable_jwt(rot_auth_pkg_readable["payload"]["rot_auth"])
        record = {}
        record["type"] = types.OEM_ROT_AUTH_PKG
        record["iat"] = datetime.now().isoformat(' ')
        record["rot_auth_pkg"] = rot_auth_pkg_readable
        self.append_audit_record(record)

    def create_rot_command(self, prod_id):
        """
            The cy_auth token means Cypress authorized the HSM for provisioning
            The rot_auth token means that the OEM authorized the HSM to provision RoT on its behalf
            Together, they form the RoT command to the device
        """
        hsm_priv_key = self.state["hsm_priv_key"]
        cy_auth = self.state["cy_auth"]
        rot_auth = self.state["products"][prod_id]["rot_auth"]

        # check CY authorization has not expired
        cy_auth_payload = crypto.jwt_payload(cy_auth)
        if datetime.fromtimestamp(cy_auth_payload["exp"]) < datetime.now():
            raise Exception("Cypress authorization for HSM expired")

        # create the RoT command
        payload = {}
        payload["type"] = types.HSM_ROT_CMD
        payload["prod_id"] = prod_id
        payload["cy_auth"] = cy_auth
        payload["rot_auth"] = rot_auth
        rot_cmd = crypto.create_jwt(payload, hsm_priv_key)

        # create audit record
        rot_cmd_readable = crypto.readable_jwt(rot_cmd)
        rot_cmd_readable["payload"]["cy_auth"] = crypto.readable_jwt(rot_cmd_readable["payload"]["cy_auth"])
        rot_cmd_readable["payload"]["rot_auth"] = crypto.readable_jwt(rot_cmd_readable["payload"]["rot_auth"])
        record = {}
        record["type"] = types.HSM_ROT_CMD
        record["iat"] = datetime.now().isoformat()
        record["rot_cmd"] = rot_cmd_readable
        self.append_audit_record(record)

        return rot_cmd

    def accept_provision_authorization(self, prov_auth):
        """
            Accepts a provisioning request from an OEM, that is authorized by the distributor
                prov_req= jwt( payload={ OEM_PROV_REQ , blob } , key=oem_priv_key )
                prov_auth= jwt( payload={ DISTI_PROV_AUTH , prov_req , cnt , exp } , key=disti_priv_key )
            When successful 'installs' the provisioning request in the HSM as the active request for given prod_id
        """
        disti_pub_key = self.state["disti_pub_key"]

        # validate the Disti's command
        prov_auth_payload = crypto.jwt_payload(prov_auth)
        if not crypto.validate_jwt(prov_auth, disti_pub_key):
            raise Exception("Invalid signature for provisioning authorization")
        if prov_auth_payload["type"] != types.DISTI_PROV_AUTH:
            raise Exception("Invalid type for provisioning authorization")
        prov_req = prov_auth_payload["prov_req"]
        prov_req_cnt = prov_auth_payload["cnt"]
        prov_req_exp = datetime.fromtimestamp(prov_auth_payload["exp"])

        # validate the OEM's provisioning request
        prov_req_payload = crypto.jwt_payload(prov_req)
        prod_id = prov_req_payload["prod_id"]
        oem_pub_key = self.state["products"][prod_id]["oem_pub_key"]
        if not crypto.validate_jwt(prov_req, oem_pub_key):
            raise Exception("Invalid signature for provisioning request")

        # install the provisioning request for execution
        prod = self.state["products"][prod_id]
        prod["prov_req"] = prov_req
        prod["prov_req_readable"] = crypto.readable_jwt(prov_req)
        prod["prov_req_cnt"] = prov_req_cnt
        prod["prov_req_exp"] = int(prov_req_exp.timestamp())

        # create audit record
        record = {}
        record["type"] = types.DISTI_PROV_AUTH
        record["iat"] = datetime.now().isoformat(' ')
        record["prod_id"] = prod_id
        record["prov_req"] = crypto.readable_jwt(prov_req)
        self.append_audit_record(record)

    def create_provision_cmd(self, prod_id, dev_rsp, image_jwt_file):
        """
            Creates and returns a provisioning command prov_cmd that can be send to a device.
                prov_req= jwt( payload={ OEM_PROV_REQ , blob } , key=oem_priv_key )
                chain_of_trust= [ x509_certs ]
                hsm_auth= jwt( payload={ CY_HSM_AUTH , hsm_pub_key , auth } , key=cy_priv_key )
                prov_cmd= jwt( payload={ HSM_PROV_CMD , prov_req , chain_of_trust , hsm_auth } , key=hsm_priv_key )
            Requires the device response to the RoT command (which contains the device identity and keys).
                dev_rsp= jwt( payload= { DEV_ROT_RSP , prod_id,die_id,dev_id, dev_pub_key } , key=dev_priv_key )
            There must be an active provisioning request in the HSM for the given product ID.
        """
        prod = self.state["products"][prod_id]
        oem_pub_key = prod["oem_pub_key"]
        signing_priv_key = prod["signing_priv_key"]
        chain_of_trust = prod["chain_of_trust"]
        prov_req = prod["prov_req"]
        prov_req_cnt = prod["prov_req_cnt"]
        prov_req_exp = datetime.fromtimestamp(prod["prov_req_exp"])
        hsm_priv_key = self.state["hsm_priv_key"]
        cy_auth = self.state["cy_auth"]
        image_cert = crypto.read_jwt(image_jwt_file)

        # validate the OEM's provisioning request
        if prov_req_exp < datetime.now():
            raise Exception("Provisioning request expired")
        if prov_req_cnt <= 0:
            raise Exception("Provisioning request count exceeded")
        prov_req_cnt -= 1;

        # validate the device response
        dev_rsp_payload = crypto.jwt_payload(dev_rsp)
        dev_pub_key = dev_rsp_payload["dev_pub_key"]
        die_id = dev_rsp_payload["die_id"]
        dev_id = dev_rsp_payload["dev_id"]
        if dev_rsp_payload["prod_id"] != prod_id:
            raise Exception("Product ID in provision request does not match device (" + prod_id + ")")
        if not crypto.validate_jwt(dev_rsp, dev_pub_key):
            raise Exception("Invalid signature on device response to RoT command")

        # create the device chain of trust (X509)
        dev_cert = crypto.create_x509_cert(prod_id=prod_id, die_id=die_id, dev_id=dev_id, pub_key=dev_pub_key,
                                           priv_key=signing_priv_key)
        chain_of_trust = chain_of_trust + [dev_cert]

        # create the provisioning command
        payload = {}
        payload["type"] = types.HSM_PROV_CMD
        payload["prov_req"] = prov_req
        payload["image_cert"] = image_cert
        payload["chain_of_trust"] = chain_of_trust
        payload["cy_auth"] = cy_auth
        prov_cmd = crypto.create_jwt(payload, hsm_priv_key)

        # create audit record
        prov_cmd_readable = crypto.readable_jwt(prov_cmd)
        prov_cmd_readable["payload"]["cy_auth"] = crypto.readable_jwt(prov_cmd_readable["payload"]["cy_auth"])
        prov_cmd_readable["payload"]["prov_req"] = crypto.readable_jwt(prov_cmd_readable["payload"]["prov_req"])
        prov_cmd_readable["payload"]["image_cert"] = crypto.readable_jwt(prov_cmd_readable["payload"]["image_cert"])
        record = {}
        record["type"] = types.HSM_PROV_CMD
        record["iat"] = datetime.now().isoformat(' ')
        record["prod_id"] = prod_id
        record["dev_rsp"] = crypto.readable_jwt(dev_rsp)
        record["prov_cmd"] = prov_cmd_readable
        self.append_audit_record(record)

        self.state["products"][prod_id]["prov_req_cnt"] = prov_req_cnt
        return prov_cmd

    def pack_rot_command(self, prod_id, cy_auth_file, rot_auth):
        payload = {}

        payload['type'] = types.HSM_ROT_CMD
        payload['prod_id'] = prod_id
        payload['cy_auth'] = crypto.read_jwt(cy_auth_file)
        payload['rot_auth'] = rot_auth

        hsm_priv_key = self.state["hsm_priv_key"]
        rot_cmd = crypto.create_jwt(payload, hsm_priv_key)

        return rot_cmd

    def pack_provision_cmd(self, **kwargs):
        payload = {}
        print(kwargs)

        if kwargs is not None:
            for k, v in kwargs.items():
                if type(v) is tuple:
                    sequence = []
                    for cert_file in v:
                        with open(cert_file) as cert:
                            sequence.append(cert.read())
                    payload[k] = sequence
                else:
                    if os.path.isfile(v):
                        payload[k] = crypto.read_jwt(v)
                    else:
                        payload[k] = v

        payload['type'] = types.HSM_PROV_CMD

        hsm_priv_key = self.state['hsm_priv_key']
        prov_cmd = crypto.create_jwt(payload, hsm_priv_key)

        return prov_cmd
