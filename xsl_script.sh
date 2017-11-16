#!/bin/bash

# Copyright (c) 2015 ARM Limited. All rights reserved.

echo 
echo "Creating report"
echo 

echo '<?xml version="1.0" encoding="UTF-8" ?>
<?xml-stylesheet type="text/xsl" href="junit_xsl.xslt"?>
<list>' > results/index.xml

for f in $(find ./test -name *.xml);
do
	cp $f results
	name=$(basename $f)
	echo "<entry name='$name' />">> results/index.xml
done

echo '</list>' >> results/index.xml

echo
echo "Report created to results/index.xml (outputs html)"
echo