"""
mbed SDK
Copyright (c) 2011-2014 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.wirkus@arm.com>
"""

from tools.utils import construct_enum, mkdir
from prettytable import PrettyTable
import os

ResultExporterType = construct_enum(HTML='Html_Exporter',
                                    JUNIT='JUnit_Exporter',
                                    JUNIT_OPER='JUnit_Exporter_Interoperability',
                                    BUILD='Build_Exporter',
                                    TEXT='Text_Exporter',
                                    PRINT='Print_Exporter')


class ReportExporter(object):
    """ Class exports extended test result Python data structure to
        different formats like HTML, JUnit XML.

    Parameter 'test_result_ext' format:

    u'uARM': {   u'LPC1768': {   'MBED_2': {   0: {   'copy_method': 'shutils.copy()',
                                                      'duration': 20,
                                                      'elapsed_time': 1.7929999828338623,
                                                      'output': 'Host test instrumentation on ...\r\n',
                                                      'result': 'OK',
                                                      'target_name': u'LPC1768',
                                                      'description': 'stdio',
                                                      'id': u'MBED_2',
                                                      'toolchain_name': u'uARM'}},
    """
    CSS_STYLE = """<style>
                   .name{
                    border: 1px solid;
                    border-radius: 25px;
                    width: 100px;
                   }
                   .tooltip{
                       position:absolute;
                       background-color: #F5DA81;
                       display:none;
                   }
                   </style>
                """

    JAVASCRIPT = """
                 <script type="text/javascript">
                 function show (elem) {
                     elem.style.display = "block";
                 }
                 function hide (elem) {
                     elem.style.display = "";
                 }
                 </script>
                 """

    def __init__(self, result_exporter_type, package="test"):
        self.result_exporter_type = result_exporter_type
        self.package = package

    def report(self, test_summary_ext, test_suite_properties=None,
               print_log_for_failures=True):
        """ Invokes report depending on exporter_type set in constructor
        """
        if self.result_exporter_type == ResultExporterType.HTML:
            # HTML exporter
            return self.exporter_html(test_summary_ext, test_suite_properties)
        elif self.result_exporter_type == ResultExporterType.JUNIT:
            # JUNIT exporter for results from test suite
            return self.exporter_junit(test_summary_ext, test_suite_properties)
        elif self.result_exporter_type == ResultExporterType.JUNIT_OPER:
            # JUNIT exporter for interoperability test
            return self.exporter_junit_ioper(test_summary_ext, test_suite_properties)
        elif self.result_exporter_type == ResultExporterType.PRINT:
            # JUNIT exporter for interoperability test
            return self.exporter_print(test_summary_ext, print_log_for_failures=print_log_for_failures)
        elif self.result_exporter_type == ResultExporterType.TEXT:
            return self.exporter_text(test_summary_ext)
        return None

    def report_to_file(self, test_summary_ext, file_name, test_suite_properties=None):
        """ Stores report to specified file
        """
        report = self.report(test_summary_ext, test_suite_properties=test_suite_properties)
        self.write_to_file(report, file_name)

    def write_to_file(self, report, file_name):
        if report is not None:
            dirname = os.path.dirname(file_name)
            if dirname:
                mkdir(dirname)
            with open(file_name, 'w') as f:
                f.write(report)

    def get_tooltip_name(self, toolchain, target, test_id, loop_no):
        """ Generate simple unique tool-tip name which can be used.
            For example as HTML <div> section id attribute.
        """
        return "target_test_%s_%s_%s_%s"% (toolchain.lower(), target.lower(), test_id.lower(), loop_no)

    def get_result_div_sections(self, test, test_no):
        """ Generates separate <DIV> sections which contains test results output.
        """

        RESULT_COLORS = {'OK': 'LimeGreen',
                         'FAIL': 'Orange',
                         'ERROR': 'LightCoral',
                         'OTHER': 'LightGray',
                        }

        tooltip_name = self.get_tooltip_name(test['toolchain_name'], test['target_name'], test['id'], test_no)
        background_color = RESULT_COLORS[test['result'] if test['result'] in RESULT_COLORS else 'OTHER']
        result_div_style = "background-color: %s"% background_color

        result = """<div class="name" style="%s" onmouseover="show(%s)" onmouseout="hide(%s)">
                       <center>%s</center>
                       <div class = "tooltip" id= "%s">
                       <b>%s</b><br />
                       <hr />
                       <b>%s</b> in <b>%.2f sec</b><br />
                       <hr />
                       <small>
                       %s
                       </small>
                       </div>
                    </div>
                 """% (result_div_style,
                       tooltip_name,
                       tooltip_name,
                       test['result'],
                       tooltip_name,
                       test['target_name_unique'],
                       test['description'],
                       test['elapsed_time'],
                       test['output'].replace('\n', '<br />'))
        return result

    def get_result_tree(self, test_results):
        """ If test was run in a loop (we got few results from the same test)
            we will show it in a column to see all results.
            This function produces HTML table with corresponding results.
        """
        result = ''
        for i, test_result in enumerate(test_results):
            result += '<table>'
            test_ids = sorted(test_result.keys())
            for test_no in test_ids:
                test = test_result[test_no]
                result += """<tr>
                                 <td valign="top">%s</td>
                             </tr>"""% self.get_result_div_sections(test, "%d_%d" % (test_no, i))
            result += '</table>'
        return result

    def get_all_unique_test_ids(self, test_result_ext):
        """ Gets all unique test ids from all ran tests.
            We need this to create complete list of all test ran.
        """
        result = []
        targets = test_result_ext.keys()
        for target in targets:
            toolchains = test_result_ext[target].keys()
            for toolchain in toolchains:
                tests = test_result_ext[target][toolchain].keys()
                result.extend(tests)
        return sorted(list(set(result)))

    #
    # Exporters functions
    #

    def exporter_html(self, test_result_ext, test_suite_properties=None):
        """ Export test results in proprietary HTML format.
        """
        result = """<html>
                    <head>
                        <title>mbed SDK test suite test result report</title>
                        %s
                        %s
                    </head>
                    <body>
                 """% (self.CSS_STYLE, self.JAVASCRIPT)

        unique_test_ids = self.get_all_unique_test_ids(test_result_ext)
        targets = sorted(test_result_ext.keys())
        result += '<table>'
        for target in targets:
            toolchains = sorted(test_result_ext[target].keys())
            for toolchain in toolchains:
                result += '<tr>'
                result += '<td></td>'
                result += '<td></td>'

                tests = sorted(test_result_ext[target][toolchain].keys())
                for test in unique_test_ids:
                    result += """<td align="center">%s</td>"""% test
                result += """</tr>
                              <tr>
                              <td valign="center">%s</td>
                              <td valign="center"><b>%s</b></td>
                          """% (toolchain, target)

                for test in unique_test_ids:
                    test_result = self.get_result_tree(test_result_ext[target][toolchain][test]) if test in tests else ''
                    result += '<td>%s</td>'% (test_result)

                result += '</tr>'
        result += '</table>'
        result += '</body></html>'
        return result

    def exporter_junit_ioper(self, test_result_ext, test_suite_properties=None):
        from junit_xml import TestSuite, TestCase
        test_suites = []
        test_cases = []

        for platform in sorted(test_result_ext.keys()):
            # {platform : ['Platform', 'Result', 'Scope', 'Description'])
            test_cases = []
            for tr_result in test_result_ext[platform]:
                result, name, scope, description = tr_result

                classname = 'test.ioper.%s.%s.%s' % (platform, name, scope)
                elapsed_sec = 0
                _stdout = description
                _stderr = ''
                # Test case
                tc = TestCase(name, classname, elapsed_sec, _stdout, _stderr)
                # Test case extra failure / error info
                if result == 'FAIL':
                    tc.add_failure_info(description, _stdout)
                elif result == 'ERROR':
                    tc.add_error_info(description, _stdout)
                elif result == 'SKIP' or result == 'NOT_SUPPORTED':
                    tc.add_skipped_info(description, _stdout)

                test_cases.append(tc)
            ts = TestSuite("test.suite.ioper.%s" % (platform), test_cases)
            test_suites.append(ts)
        return TestSuite.to_xml_string(test_suites)

    def exporter_junit(self, test_result_ext, test_suite_properties=None):
        """ Export test results in JUnit XML compliant format
        """
        from junit_xml import TestSuite, TestCase
        test_suites = []
        test_cases = []

        targets = sorted(test_result_ext.keys())
        for target in targets:
            toolchains = sorted(test_result_ext[target].keys())
            for toolchain in toolchains:
                test_cases = []
                tests = sorted(test_result_ext[target][toolchain].keys())
                for test in tests:
                    test_results = test_result_ext[target][toolchain][test]
                    for test_res in test_results:
                        test_ids = sorted(test_res.keys())
                        for test_no in test_ids:
                            test_result = test_res[test_no]
                            name = test_result['description']
                            classname = '%s.%s.%s.%s'% (self.package, target, toolchain, test_result['id'])
                            elapsed_sec = test_result['elapsed_time']
                            _stdout = test_result['output']

                            if 'target_name_unique' in test_result:
                                _stderr = test_result['target_name_unique']
                            else:
                                _stderr = test_result['target_name']

                            # Test case
                            tc = TestCase(name, classname, elapsed_sec, _stdout, _stderr)

                            # Test case extra failure / error info
                            message = test_result['result']
                            if test_result['result'] == 'FAIL':
                                tc.add_failure_info(message, _stdout)
                            elif test_result['result'] == 'SKIP' or test_result["result"] == 'NOT_SUPPORTED':
                                tc.add_skipped_info(message, _stdout)
                            elif test_result['result'] != 'OK':
                                tc.add_error_info(message, _stdout)

                            test_cases.append(tc)

                ts = TestSuite("test.suite.%s.%s"% (target, toolchain), test_cases, properties=test_suite_properties[target][toolchain])
                test_suites.append(ts)
        return TestSuite.to_xml_string(test_suites)

    def exporter_print_helper(self, array, print_log=False):
        for item in array:
            print("  * %s::%s::%s" % (item["target_name"],
                                      item["toolchain_name"],
                                      item["id"]))
            if print_log:
                log_lines = item["output"].split("\n")
                for log_line in log_lines:
                    print("        %s" % log_line)

    def exporter_print(self, test_result_ext, print_log_for_failures=False):
        """ Export test results in print format.
        """
        failures = []
        skips = []
        successes = []

        unique_test_ids = self.get_all_unique_test_ids(test_result_ext)
        targets = sorted(test_result_ext.keys())

        for target in targets:
            toolchains = sorted(test_result_ext[target].keys())
            for toolchain in toolchains:
                tests = sorted(test_result_ext[target][toolchain].keys())
                for test in tests:
                    test_runs = test_result_ext[target][toolchain][test]
                    for test_runner in test_runs:
                        #test_run = test_result_ext[target][toolchain][test][test_run_number][0]
                        test_run = test_runner[0]
                        
                        if "result" in test_run:
                            if test_run["result"] == "FAIL":
                                failures.append(test_run)
                            elif test_run["result"] == "SKIP" or test_run["result"] == "NOT_SUPPORTED":
                                skips.append(test_run)
                            elif test_run["result"] == "OK":
                                successes.append(test_run)
                            else:
                                raise Exception("Unhandled result type: %s" % (test_run["result"]))
                        else:
                            raise Exception("'test_run' did not have a 'result' value")

        if successes:
            print("\n\nBuild successes:")
            self.exporter_print_helper(successes)

        if skips:
            print("\n\nBuild skips:")
            self.exporter_print_helper(skips)

        if failures:
            print("\n\nBuild failures:")
            self.exporter_print_helper(failures, print_log=print_log_for_failures)
            return False
        else:
            return True

    def exporter_text(self, test_result_ext):
        """ Prints well-formed summary with results (SQL table like)
            table shows target x test results matrix across
        """
        success_code = 0    # Success code that can be leter returned to
        # Pretty table package is used to print results
        pt = PrettyTable(["Result", "Target", "Toolchain", "Test ID", "Test Description",
                          "Elapsed Time", "Timeout"])
        pt.align["Result"] = "l" # Left align
        pt.align["Target"] = "l" # Left align
        pt.align["Toolchain"] = "l" # Left align
        pt.align["Test ID"] = "l" # Left align
        pt.align["Test Description"] = "l" # Left align
        pt.padding_width = 1 # One space between column edges and contents (default)

        result_dict = {"OK" : 0,
                       "FAIL" : 0,
                       "ERROR" : 0,
                       "UNDEF" : 0,
                       "IOERR_COPY" : 0,
                       "IOERR_DISK" : 0,
                       "IOERR_SERIAL" : 0,
                       "TIMEOUT" : 0,
                       "NO_IMAGE" : 0,
                       "MBED_ASSERT" : 0,
                       "BUILD_FAILED" : 0,
                       "NOT_SUPPORTED" : 0
        }
        unique_test_ids = self.get_all_unique_test_ids(test_result_ext)
        targets = sorted(test_result_ext.keys())
        for target in targets:
            toolchains = sorted(test_result_ext[target].keys())
            for toolchain in toolchains:
                test_cases = []
                tests = sorted(test_result_ext[target][toolchain].keys())
                for test in tests:
                    test_results = test_result_ext[target][toolchain][test]
                    for test_res in test_results:
                        test_ids = sorted(test_res.keys())
                        for test_no in test_ids:
                            test_result = test_res[test_no]
                            result_dict[test_result['result']] += 1
                            pt.add_row([test_result['result'],
                                        test_result['target_name'],
                                        test_result['toolchain_name'],
                                        test_result['id'],
                                        test_result['description'],
                                        test_result['elapsed_time'],
                                        test_result['duration']])
        result = pt.get_string()
        result += "\n"

        # Print result count
        result += "Result: " + ' / '.join(['%s %s' % (value, key) for (key, value) in {k: v for k, v in result_dict.items() if v != 0}.items()])
        return result
