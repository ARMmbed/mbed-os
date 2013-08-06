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
import json
from os.path import join, abspath, dirname, exists
import datetime
from time import time

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.append(ROOT)

from workspace_tools.build_api import build_project, build_mbed_libs
from workspace_tools.tests import TEST_MAP
from workspace_tools.client import request_test, get_muts
from workspace_tools.settings import *
from workspace_tools.paths import BUILD_DIR
from workspace_tools.utils import error
from workspace_tools.targets import TARGET_MAP


class TestServer:
    def __init__(self):
        self.muts = get_muts()
    
    def available(self, target, peripherals=None):
        if peripherals is not None:
            peripherals = set(peripherals)
        
        for id, mut in self.muts.iteritems():
            # Target check
            if mut["mcu"] != target: continue
            
            # Peripherals check
            if peripherals is not None:
                if 'peripherals' not in mut: continue
                if not peripherals.issubset(set(mut['peripherals'])): continue
            
            return True
        
        return False
    
    def request(self, target, path, duration, test_id):
        return request_test(target, path, test_id, duration)


def print_results(results, label):
    print "=== %d %s" % (len(results), label)
    for r in results:
        print "   [%s::%s]: %s" % (r['target'], r['toolchain'], TEST_MAP[r["test_id"]].get_description())


def store_data(path, data):
    f = open(path, "w")
    f.write(json.dumps(data))

def load_data(path):
    f = open(path)
    return json.loads(f.read())


if __name__ == "__main__":
    start = time()
    
    # Load test specification
    if len(sys.argv) != 2: error("You should provide a test specification file")
    test_spec_file = sys.argv[1]
    if not exists(test_spec_file):
        error('The selected test spec file does not exists: "%s"' % test_spec_file)
    f = open(test_spec_file)
    test_spec = json.load(f)
    clean = test_spec.get('clean', False)
    test_ids = test_spec.get('test_ids', None)
    
    # Test Server
    test_server = TestServer()
    
    # Test Results
    tests_results = []
    test_time = datetime.datetime.utcnow()
    test_report = {
        "date": test_time.isoformat(),
        "test_server": SERVER_ADDRESS,
        "test_results": tests_results
    }
    successes = []
    failures = []
    
    for target, toolchains in test_spec['targets'].iteritems():
        for toolchain in toolchains:
            print '=== %s::%s ===' % (target, toolchain)
            
            T = TARGET_MAP[target]
            build_mbed_libs(T, toolchain)
            
            build_dir = join(BUILD_DIR, "test", target, toolchain)
            
            for test_id, test in TEST_MAP.iteritems():
                if test_ids is not None and test_id not in test_ids:
                    continue
                
                if test.automated and test.is_supported(target, toolchain):
                    # Check if the server has the capability to serve our test request
                    if not test_server.available(target, test.peripherals):
                        print "The test server does not have such device: %s, %s" % (target, test.peripherals)
                        continue
                    
                    test_result = {
                        'target': target,
                        'toolchain': toolchain,
                        'test_id': test_id,
                    }
                    
                    path = build_project(test.source_dir, join(build_dir, test_id),
                                 T, toolchain, test.dependencies, clean=clean)
                    test_result_cache = join(dirname(path), "test_result.json")
                    
                    if not clean and exists(test_result_cache):
                        test_result = load_data(test_result_cache)
                    else:
                        # For an automated test the duration act as a timeout after
                        # which the test gets interrupted
                        report = test_server.request(target, path, test.duration, test_id)
                        test_result['result'] = report['result']
                        store_data(test_result_cache, test_result)
                    
                    tests_results.append(test_result)
                    print str(test_result) + '\n'
                    
                    if test_result['result'] == 'success':
                        successes.append(test_result)
                    else:
                        failures.append(test_result)
    
    # Print a basic summary
    print_results(successes, "Successes")
    print_results(failures, "Failures")
    
    # Save the report on a file
    report_filename = "test_%s.json" % test_time.strftime("%Y-%m-%d_%H-%M-%S")
    store_data(report_filename, test_report)
    
    print "\nCompleted in (%d)sec" % (time() - start)
