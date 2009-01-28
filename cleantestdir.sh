#!/bin/bash
if [ -d "tests" ]
then
	rm "tests/*.php" 2>/dev/null
	rm "tests/*.log" 2>/dev/null
	rm "tests/*.exp" 2>/dev/null
	rm "tests/*.diff" 2>/dev/null
	echo "the 'test' directory has been cleaned";
	exit;
fi
echo "the 'test' directory does not exist";
