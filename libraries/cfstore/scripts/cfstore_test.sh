#!/bin/bash
# script to run tests
# - run script in empty dir e.g.
# cfstore_test.sh 2>&1 & 
#
# set -o xtrace

# clean up before start
rm -fR configuration-store-test 
rm -fR configuration-store-example1-test 
rm -fR configuration-store-example3-test 

echo "**************************************************"
echo "*** CONFIGURATION-STORE: CLONING GITHUB REPOS ***"
echo "**************************************************"
#git clone git@github.com:/armmbed/configuration-store.git configuration-store-test
#git clone git@github.com:/armmbed/configuration-store-example1.git configuration-store-example1-test 
#git clone git@github.com:/armmbed/configuration-store-example3.git configuration-store-example3-test

cd configuration-store-test
pwd

echo "***************************************************"
echo "*** CONFIGURATION-STORE: BUILDING/TESTING START ***"
echo "***************************************************"
rm -fR build
rm -fR yotta_modules
rm -fR yott_targets
echo "*** CONFIGURATION-STORE: TESTING FLASH BACKEND ASYNC MODE ***"
yt target frdm-k64f-gcc
yt build
mbedgt -V

echo "************************************************************"
echo "*** CONFIGURATION-STORE: TESTING FLASH BACKEND SYNC MODE ***"
echo "************************************************************"
rm -fR build
yotta --config='{"config":{"hardware":{"mtd":{"async":{"ops":0}}}}}' build
mbedgt -V
 
echo "************************************************************"
echo "*** CONFIGURATION-STORE: TESTING SRAM BACKEND ASYNC MODE ***"
echo "************************************************************"
rm -fR build
yotta --config='{"cfstore":{"backend":{"sram":1}}}' build
mbedgt -V

echo "************************************************************"
echo "*** CONFIGURATION-STORE: TESTING SRAM BACKEND SYNC MODE ***"
echo "************************************************************"
rm -fR build
yotta --config='{"config":{"hardware":{"mtd":{"async":{"ops":0}}}}, "cfstore":{"backend":{"sram":1}}}' build
mbedgt -V

cd ..
cd configuration-store-example1-test
pwd

echo "********************************************************************"
echo "*** CONFIGURATION-STORE-EXAMPLE1: TESTING SRAM BACKEND SYNC MODE ***"
echo "********************************************************************"
pwd

rm -fR build
rm -fR yotta_modules
rm -fR yott_targets
yt target frdm-k64f-gcc
yt build
# not possible to automate test

cd ..
cd configuration-store-example3-test
pwd

echo "********************************************************************"
echo "*** CONFIGURATION-STORE-EXAMPLE3: TESTING SRAM BACKEND SYNC MODE ***"
echo "********************************************************************"
pwd

rm -fR build
rm -fR yotta_modules
rm -fR yott_targets
yt target frdm-k64f-gcc
yt build
# not possible to automate test

cd ..
cd cfstore_example_mx2
pwd

echo "********************************************************************"
echo "*** CFSTORE_EXAMPLE_MX2: TESTING MORPHEUS EXAMPLE : GCC          ***"
echo "********************************************************************"
pwd

# todo: build on gcc
# clean

echo "********************************************************************"
echo "*** CFSTORE_EXAMPLE_MX2: TESTING MORPHEUS EXAMPLE : ARM          ***"
echo "********************************************************************"
pwd

# todo: build on arm
# clean


echo "********************************************************************"
echo "*** CFSTORE_EXAMPLE_MX2: TESTS MORPHEUS EXAMPLE : GCC            ***"
echo "********************************************************************"
pwd

# todo: copy example3.txt out of way
# do mbed compile --tests
