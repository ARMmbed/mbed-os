#!/bin/bash
# Execute script with root path where to find binaries. 
# For example ./run_unit_tests_with_valgrind.sh ./build/x86-linux-native-coverage/test/mbedclient/

input="binaries.txt"
valgrind_logs="valgrind_logs"
rm -rf $valgrind_logs
mkdir $valgrind_logs
find $1 -type f -executable -exec sh -c "file -i '{}' | grep -q 'x-executable; charset=binary'" \; -print > $input
while file= read -r binary
do
    valgrind --track-origins=yes --xml=yes --xml-file="${valgrind_logs}/valgrind_$(basename $binary).xml" "$binary"

done < "$input"
