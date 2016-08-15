#!/bin/bash
# Copyright (c) 2015 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# * http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

echo
echo "Build mbed Client API unit tests"
echo
yt target x86-linux-native-coverage
yt clean
yt build
yt test --no-build -- -ojunit
echo
echo Create results
echo
rm -rf results
rm -rf coverage
mkdir results
mkdir coverage

find ./build -name '*.xml' | xargs cp -t ./results/
find ./build/x86-linux-native-coverage/test -name '*.gcno' | xargs cp -t ./coverage/
find ./build/x86-linux-native-coverage/test -name '*.gcda' | xargs cp -t ./coverage/
touch coverage/*.gcda
exclude_files="${PWD}/test/"
gcovr -r ./ --gcov-filter='.*source*.' --exclude-unreachable-branches --exclude $exclude_files --object-directory ./coverage -x -o ./results/gcovr.xml
echo
echo "Create coverage document"
echo
lcov -b ./ -d ./coverage -c -o ./coverage/app.info
lcov -q -r ./coverage/app.info "/test/mbedclient/*" -o ./coverage/app.info
lcov -q -r ./coverage/app.info "/usr*" -o ./coverage/app.info
genhtml -q --no-branch-coverage --function-coverage --demangle-cpp --title "mbed Client coverage" ./coverage/app.info -o ./coverage
rm -f ./coverage/*.gcno
rm -f ./coverage/*.gcda
echo 
echo "Creating report"
echo 
echo '<?xml version="1.0" encoding="UTF-8" ?>
<?xml-stylesheet type="text/xsl" href="junit_xsl.xslt"?>
<list>' >> index.xml

for f in results/*.xml
do
name=${f##*/}
echo '<entry name="results/'"$name"'" />'>> index.xml
done

echo '</list>' >> index.xml

echo
echo "Report created to index.xml (outputs html)"
echo
xsltproc -o results/testresults.html junit_xsl.xslt index.xml
rm -f index.xml

