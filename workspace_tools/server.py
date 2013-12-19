"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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
import sys
from os.path import join, abspath, dirname, exists
from shutil import copy
from time import sleep, time
from SocketServer import BaseRequestHandler, TCPServer
from subprocess import Popen, PIPE
from threading import Thread
from Queue import Queue, Empty
import json


# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from workspace_tools.utils import delete_dir_files
from workspace_tools.settings import * 
from workspace_tools.tests import *
from workspace_tools.targets import TARGET_MAP


class ProcessObserver(Thread):
    def __init__(self, proc):
        Thread.__init__(self)
        self.proc = proc
        self.queue = Queue()
        self.daemon = True
        self.active = True
        self.start()
    
    def run(self):
        while self.active:
            c = self.proc.stdout.read(1)
            self.queue.put(c)
    
    def stop(self):
        self.active = False
        try:
            self.proc.terminate()
        except Exception, _:
            pass


def run_host_test(client, name, disk, port, duration, extra_serial):
    print "{%s}"  % name,
    cmd = ["python", "%s.py" % name, '-p', port, '-d', disk, '-t', str(duration), "-e", extra_serial]
    proc = Popen(cmd, stdout=PIPE, cwd=HOST_TESTS)
    obs = ProcessObserver(proc)
    start = time()
    line = ''
    output = []
    while (time() - start) < duration:
        # Give the client a way to interrupt the test
        try:
            c = client.recv(1)
            if c == '!':
                break
        except Exception, _:
            pass
        
        try:
            c = obs.queue.get(block=True, timeout=1)
        except Empty, _:
            c = None
        
        if c:
            output.append(c)
            # Give the mbed under test a way to communicate the end of the test
            if c in ['\n', '\r']:
                if '{end}' in line: break
                line = ''
            else:
                line += c
    
    # Stop test process
    obs.stop()
    
    # Parse Output
    result = None
    for line in ''.join(output).splitlines():
        if '{success}' in line: result = "success"
        if '{failure}' in line: result = "failure"
        if '{error}' in line: result = "error"
        if '{end}' in line: break
    if result is None:
        result = "{error}"
    return result


class Tester(BaseRequestHandler):
    def send_result(self, result):
        report_string = json.dumps({'result': result})
        self.request.send(report_string)
        print "Result: %s" % report_string
    
    def handle(self):
        print "\nRequest",
        test_spec = self.request.recv(1024)
        print test_spec,
        data = json.loads(test_spec)
        
        # The MUTs list can be different from server to server, so we have to
        # provide a way for the client to query the capabilities of this server
        if 'info' in data:
            if data['info'] == 'muts':
                muts_info = json.dumps(MUTs)
                print muts_info
                self.request.send(muts_info)
                return
        
        test_id = data['test_id']
        test = TEST_MAP[test_id]
        
        image = data["image"]
        duration = data.get("duration", 10)
        
        # Find a suitable MUT:
        mut = None
        for id, m in MUTs.iteritems():
            if m['mcu'] == data['mcu']:
                mut = m
                break
        
        if mut is None:
            print "No mbed available: %s" % data['mcu']
            self.send_result("{error}")
            return
        
        disk = mut['disk']
        port = mut['port']
        extra_serial = mut.get('extra_serial', "")
        target = TARGET_MAP[mut['mcu']]
        
        # Program
        # When the build and test system were separate, this was relative to a
        # base network folder base path: join(NETWORK_BASE_PATH, )
        image_path = image
        if not exists(image_path):
            print "Image file does not exist: %s" % image_path
            self.send_result("{error}")
            return
        
        if not target.is_disk_virtual:
            delete_dir_files(disk)
        
        copy(image_path, disk)
        
        # Copy Extra Files
        if not target.is_disk_virtual and test.extra_files:
            for f in test.extra_files:
                copy(f, disk)
        
        sleep(target.program_cycle_s())
        
        # Host test
        self.request.setblocking(0)
        result = run_host_test(self.request, test.host_test, disk, port, duration, extra_serial)
        self.send_result(result)


def run_test_server():
    while True:
        try:
            server = TCPServer((SERVER_ADDRESS, SERVER_PORT), Tester)
            break
        except Exception, e:
            print e
            sleep(1)
    
    print "[Test server]"
    try:
        server.serve_forever()
    except KeyboardInterrupt, e:
        print "\n[CTRL+c] shutdown server"
        server.shutdown()
        server.server_close()


if __name__ == '__main__':
    run_test_server()
