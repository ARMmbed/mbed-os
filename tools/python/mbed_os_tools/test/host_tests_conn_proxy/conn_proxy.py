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

import re
import sys
import uuid
from time import time
from ..host_tests_logger import HtrunLogger
from .conn_primitive_serial import SerialConnectorPrimitive
from .conn_primitive_remote import RemoteConnectorPrimitive
from .conn_primitive_fastmodel import FastmodelConnectorPrimitive

if (sys.version_info > (3, 0)):
    from queue import Empty as QueueEmpty # Queue here refers to the module, not a class
else:
    from Queue import Empty as QueueEmpty

class KiViBufferWalker():
    """! Simple auxiliary class used to walk through a buffer and search for KV tokens """
    def __init__(self):
        self.KIVI_REGEX = r"\{\{([\w\d_-]+);([^\}]+)\}\}"
        self.buff = str()
        self.kvl = []
        self.re_kv = re.compile(self.KIVI_REGEX)

    def append(self, payload):
        """! Append stream buffer with payload and process. Returns non-KV strings"""
        logger = HtrunLogger('CONN')
        try:
            self.buff += payload.decode('utf-8')
        except UnicodeDecodeError:
            logger.prn_wrn("UnicodeDecodeError encountered!")
            self.buff += payload.decode('utf-8','ignore')
        lines = self.buff.split('\n')
        self.buff = lines[-1]   # remaining
        lines.pop(-1)
        # List of line or strings that did not match K,V pair.
        discarded = []

        for line in lines:
            m = self.re_kv.search(line)
            if m:
                (key, value) = m.groups()
                self.kvl.append((key, value, time()))
                line = line.strip()
                match = m.group(0)
                pos = line.find(match)
                before = line[:pos]
                after = line[pos + len(match):]
                if len(before) > 0:
                    discarded.append(before)
                if len(after) > 0:
                    # not a K,V pair part
                    discarded.append(after)
            else:
                # not a K,V pair
                discarded.append(line)
        return discarded

    def search(self):
        """! Check if there is a KV value in buffer """
        return len(self.kvl) > 0

    def pop_kv(self):
        if len(self.kvl):
            return self.kvl.pop(0)
        return None, None, time()


def conn_primitive_factory(conn_resource, config, event_queue, logger):
    """! Factory producing connectors based on type and config
    @param conn_resource Name of connection primitive (e.g. 'serial' for
           local serial port connection or 'grm' for global resource manager)
    @param event_queue Even queue of Key-Value protocol
    @param config Global configuration for connection process
    @param logger Host Test logger instance
    @return Object of type <ConnectorPrimitive> or None if type of connection primitive unknown (conn_resource)
    """
    polling_timeout = int(config.get('polling_timeout', 60))
    logger.prn_inf("notify event queue about extra %d sec timeout for serial port pooling"%polling_timeout)
    event_queue.put(('__timeout', polling_timeout, time()))

    if conn_resource == 'serial':
        # Standard serial port connection
        # Notify event queue we will wait additional time for serial port to be ready

        # Get extra configuration related to serial port
        port = config.get('port')
        baudrate = config.get('baudrate')

        logger.prn_inf("initializing serial port listener... ")
        connector = SerialConnectorPrimitive(
            'SERI',
            port,
            baudrate,
            config=config)
    elif conn_resource == 'grm':
        # Start GRM (Gloabal Resource Mgr) collection
        logger.prn_inf("initializing global resource mgr listener... ")
        connector = RemoteConnectorPrimitive('GLRM', config=config)
    elif conn_resource == 'fmc':
        # Start Fast Model Connection collection
        logger.prn_inf("initializing fast model connection")
        connector = FastmodelConnectorPrimitive('FSMD', config=config)
    else:
        logger.pn_err("unknown connection resource!")
        raise NotImplementedError("ConnectorPrimitive factory: unknown connection resource '%s'!"% conn_resource)

    return connector


def conn_process(event_queue, dut_event_queue, config):

    def __notify_conn_lost():
        error_msg = connector.error()
        connector.finish()
        event_queue.put(('__notify_conn_lost', error_msg, time()))

    def __notify_sync_failed():
        error_msg = connector.error()
        connector.finish()
        event_queue.put(('__notify_sync_failed', error_msg, time()))

    logger = HtrunLogger('CONN')
    logger.prn_inf("starting connection process...")

    # Send connection process start event to host process
    # NOTE: Do not send any other Key-Value pairs before this!
    event_queue.put(('__conn_process_start', 1, time()))

    # Configuration of conn_opriocess behaviour
    sync_behavior = int(config.get('sync_behavior', 1))
    sync_timeout = config.get('sync_timeout', 1.0)
    conn_resource = config.get('conn_resource', 'serial')
    last_sync = False

    # Create connector instance with proper configuration
    connector = conn_primitive_factory(conn_resource, config, event_queue, logger)

    # If the connector failed, stop the process now
    if not connector.connected():
        logger.prn_err("Failed to connect to resource")
        __notify_conn_lost()
        return 0

    # Create simple buffer we will use for Key-Value protocol data
    kv_buffer = KiViBufferWalker()

    # List of all sent to target UUIDs (if multiple found)
    sync_uuid_list = []

    # We will ignore all kv pairs before we get sync back
    sync_uuid_discovered = False

    def __send_sync(timeout=None):
        sync_uuid = str(uuid.uuid4())
        # Handshake, we will send {{sync;UUID}} preamble and wait for mirrored reply
        if timeout:
            logger.prn_inf("Reset the part and send in new preamble...")
            connector.reset()
            logger.prn_inf("resending new preamble '%s' after %0.2f sec"% (sync_uuid, timeout))
        else:
            logger.prn_inf("sending preamble '%s'"% sync_uuid)

        if connector.write_kv('__sync', sync_uuid):
            return sync_uuid
        else:
            return None

    # Send simple string to device to 'wake up' greentea-client k-v parser
    if not connector.write("mbed" * 10, log=True):
        # Failed to write 'wake up' string, exit conn_process
        __notify_conn_lost()
        return 0


    # Sync packet management allows us to manipulate the way htrun sends __sync packet(s)
    # With current settings we can force on htrun to send __sync packets in this manner:
    #
    # * --sync=0        - No sync packets will be sent to target platform
    # * --sync=-10      - __sync packets will be sent unless we will reach
    #                     timeout or proper response is sent from target platform
    # * --sync=N        - Send up to N __sync packets to target platform. Response
    #                     is sent unless we get response from target platform or
    #                     timeout occur

    if sync_behavior > 0:
        # Sending up to 'n' __sync packets
        logger.prn_inf("sending up to %s __sync packets (specified with --sync=%s)"% (sync_behavior, sync_behavior))
        sync_uuid = __send_sync()

        if sync_uuid:
            sync_uuid_list.append(sync_uuid)
            sync_behavior -= 1
        else:
            __notify_conn_lost()
            return 0
    elif sync_behavior == 0:
        # No __sync packets
        logger.prn_wrn("skipping __sync packet (specified with --sync=%s)"% sync_behavior)
    else:
        # Send __sync until we go reply
        logger.prn_inf("sending multiple __sync packets (specified with --sync=%s)"% sync_behavior)

        sync_uuid = __send_sync()
        if sync_uuid:
            sync_uuid_list.append(sync_uuid)
            sync_behavior -= 1
        else:
            __notify_conn_lost()
            return 0

    loop_timer = time()
    while True:

        # Check if connection is lost to serial
        if not connector.connected():
            __notify_conn_lost()
            break

        # Send data to DUT
        try:
            (key, value, _) = dut_event_queue.get(block=False)
        except QueueEmpty:
            pass # Check if target sent something
        else:
            # Return if state machine in host_test_default has finished to end process
            if key == '__host_test_finished' and value == True:
                logger.prn_inf("received special event '%s' value='%s', finishing"% (key, value))
                connector.finish()
                return 0
            elif key == '__reset':
                logger.prn_inf("received special event '%s', resetting dut" % (key))
                connector.reset()
                event_queue.put(("reset_complete", 0, time()))
            elif not connector.write_kv(key, value):
                connector.write_kv(key, value)
                __notify_conn_lost()
                break

        # Since read is done every 0.2 sec, with maximum baud rate we can receive 2304 bytes in one read in worst case.
        data = connector.read(2304)
        if data:
            # Stream data stream KV parsing
            print_lines = kv_buffer.append(data)
            for line in print_lines:
                logger.prn_rxd(line)
                event_queue.put(('__rxd_line', line, time()))
            while kv_buffer.search():
                key, value, timestamp = kv_buffer.pop_kv()

                if sync_uuid_discovered:
                    event_queue.put((key, value, timestamp))
                    logger.prn_inf("found KV pair in stream: {{%s;%s}}, queued..."% (key, value))
                else:
                    if key == '__sync':
                        if value in sync_uuid_list:
                            sync_uuid_discovered = True
                            event_queue.put((key, value, time()))
                            idx = sync_uuid_list.index(value)
                            logger.prn_inf("found SYNC in stream: {{%s;%s}} it is #%d sent, queued..."% (key, value, idx))
                        else:
                            logger.prn_err("found faulty SYNC in stream: {{%s;%s}}, ignored..."% (key, value))
                            logger.prn_inf("Resetting the part and sync timeout to clear out the buffer...")
                            connector.reset()
                            loop_timer = time()
                    else:
                        logger.prn_wrn("found KV pair in stream: {{%s;%s}}, ignoring..."% (key, value))

        if not sync_uuid_discovered:
            # Resending __sync after 'sync_timeout' secs (default 1 sec)
            # to target platform. If 'sync_behavior' counter is != 0 we
            # will continue to send __sync packets to target platform.
            # If we specify 'sync_behavior' < 0 we will send 'forever'
            # (or until we get reply)

            if sync_behavior != 0:
                time_to_sync_again = time() - loop_timer
                if time_to_sync_again > sync_timeout:
                    sync_uuid = __send_sync(timeout=time_to_sync_again)

                    if sync_uuid:
                        sync_uuid_list.append(sync_uuid)
                        sync_behavior -= 1
                        loop_timer = time()
                        #Sync behavior will be zero and if last sync fails we should report connection
                        #lost
                        if sync_behavior == 0:
                            last_sync = True
                    else:
                        __notify_conn_lost()
                        break
            elif last_sync == True:
                #SYNC lost connection event : Device not responding, send sync failed
                __notify_sync_failed()
                break

    return 0
