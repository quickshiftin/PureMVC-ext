#!/bin/bash

if [[ -n $q && "$1" -eq "-v" ]]
then
	isVerbose=1
else
	isVerbose=0
fi
cwd=`pwd`;

if [ -d "tests" ]
then
	if [ $isVerbose ]
	then
		rm $cwd/tests/*.php
		rm $cwd/tests/*.log
		rm $cwd/tests/*.exp
		rm $cwd/tests/*.diff
		rm $cwd/tests/*.out
	else
		rm $cwd/tests/*.php 2>/dev/null
		rm $cwd/tests/*.log 2>/dev/null
		rm $cwd/tests/*.exp 2>/dev/null
		rm $cwd/tests/*.diff 2>/dev/null
		rm $cwd/tests/*.out 2>/dev/null
	fi
	echo "the 'tests' directory has been cleaned";
	exit;
fi
echo "the 'tests' directory does not exist";
