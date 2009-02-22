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
		find tests/tests \( -name '*.php' -or -name '*.log' -or -name '*.exp' -or -name '*.diff' -or -name '*.out' \) -exec rm '{}' ';'
	else
		find tests/tests \( -name "$cwd/tests/*.php" -or -name "$cwd/tests/*.log" -or -name "$cwd/tests/*.exp" -or -name "$cwd/tests/*.diff" -or -name "$cwd/tests/*.out" \) -exec rm '{}' ';' 2>/dev/null
	fi
	echo "the 'tests' directory has been cleaned";
	exit;
fi
echo "the 'tests' directory does not exist";
