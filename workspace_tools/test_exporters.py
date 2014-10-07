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

from workspace_tools.utils import construct_enum


ResultExporterType = construct_enum(JSON='Json_Exporter',
                                    TEXT='Text_Exporter',
                                    HTML='Html_Exporter')


def exporter_factory(result_exporter_type):
    pass


def exporter_html(test_result_ext):
    """
    Parameter 'test_result_ext' format:

    u'uARM': {   u'LPC1768': {   'MBED_2': {   0: {   'copy_method': 'shutils.copy()',
                                                      'duration': 20,
                                                      'elapsed_time': 1.7929999828338623,
                                                      'single_test_output': 'Host test instrumentation on ...\r\n',
                                                      'single_test_result': 'OK',
                                                      'target_name': u'LPC1768',
                                                      'test_description': 'stdio',
                                                      'test_id': u'MBED_2',
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

    def get_tooltip_name(toolchain, target, test_id, loop_no):
        return "target_test_%s_%s_%s_%d"% (toolchain.lower(), target.lower(), test_id.lower(), loop_no)

    def get_result_div_sections(test, test_no):

        RESULT_COLORS = {'OK' : 'LimeGreen',
                         'FAIL' : 'Orange',
                         'ERROR' : 'LightCoral',
                         }

        tooltip_name = get_tooltip_name(test['toolchain_name'], test['target_name'], test['test_id'], test_no)
        background_color = RESULT_COLORS[test['single_test_result'] if test['single_test_result'] in RESULT_COLORS else 'ERROR']
        result_div_style = "background-color: %s"% background_color
        result = """ <div class="name" style="%s" onmouseover="show(%s)" onmouseout="hide(%s)">
                        <center>%s</center>
                        <div class = "tooltip" id= "%s">
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
                       test['single_test_result'],
                       tooltip_name,
                       test['test_description'],
                       test['elapsed_time'],
                       test['single_test_output'].replace('\n', '<br />'))
        return result

    def get_result_tree(test_results):
        result = '<table>'
        test_ids = sorted(test_results.keys())
        for test_no in test_ids:
            test = test_results[test_no]
            result += """<tr>
                             <td valign="top">%s</td>
                         </tr>"""% get_result_div_sections(test, test_no)
        result += '</table>'
        return result

    def get_all_unique_test_ids(test_result_ext):
        result = []
        toolchains = test_result_ext.keys()
        for toolchain in toolchains:
            targets = test_result_ext[toolchain].keys()
            for target in targets:
                tests = test_result_ext[toolchain][target].keys()
                result.extend(tests)
        return sorted(list(set(result)))

    result = """<html>
                <head>
                    <title>mbed SDK test suite test result report</title>
                    %s
                    %s
                </head>
                <body>
             """% (CSS_STYLE, JAVASCRIPT)

    unique_test_ids = get_all_unique_test_ids(test_result_ext)

    toolchains = sorted(test_result_ext.keys())
    for toolchain in toolchains:
        result += '<h2>%s</h2>'% toolchain
        targets = sorted(test_result_ext[toolchain].keys())
        result += '<table><tr>'
        for target in targets:
            result += '<td valign="center">%s</td>'% (target)

            tests = sorted(test_result_ext[toolchain][target].keys())
            for test in unique_test_ids:
                result += """<td align="center">%s</td>"""% test
            result += """</tr>
                          <tr>
                          <td></td>
                      """

            for test in unique_test_ids:
                test_result = get_result_tree(test_result_ext[toolchain][target][test]) if test in tests else ''
                result += '<td>%s</td>'% (test_result)

            result += '</tr>'
        result += '</table>'
    result += '</body></html>'
    return result
