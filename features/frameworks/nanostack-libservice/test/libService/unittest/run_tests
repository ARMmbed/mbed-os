#!/bin/bash
echo
echo Build M2M API unit tests
echo

# Remember to add new test folder to Makefile
make clean
make all

echo
echo Create results
echo
mkdir results

find ./ -name '*.xml' | xargs cp -t ./results/

echo
echo Create coverage document
echo
mkdir coverages
cd coverages

#copy the .gcda & .gcno for all test projects (no need to modify
#cp ../../../source/*.gc* .
#find ../ -name '*.gcda' | xargs cp -t .
#find ../ -name '*.gcno' | xargs cp -t .
#find . -name "test*" -type f -delete
#find . -name "*test*" -type f -delete
#find . -name "*stub*" -type f -delete
#rm -rf main.*

lcov -q -d ../. -c -o app.info
lcov -q -r app.info "/test*" -o app.info
lcov -q -r app.info "/usr*" -o app.info
genhtml --no-branch-coverage app.info
cd ..
echo
echo
echo
echo Have a nice bug hunt!
echo
echo
echo
