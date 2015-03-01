#!/bin/sh

EXECUTABLE=./server
DATA_INPUT_FILE_OPEN=./input/tradingbook_opening.txt
DATA_INPUT_FILE_CLOSE=./input/tradingbook_closing.txt
YIELD_CURVE_FILE=./input/yieldcurve.txt
ERROR_LOGFILE=./output/error_logfile

$EXECUTABLE  $DATA_INPUT_FILE_OPEN $DATA_INPUT_FILE_CLOSE $YIELD_CURVE_FILE 2> $ERROR_LOGFILE >&1 

ret=$?
# standard unix practice is if an executable returns a non-zero code then it failed 
if [ $ret -ne 0 ]
then
	echo my program returned non-zero return code: $ret
	echo "prog failed"
	[ -s $ERROR_LOGFILE ] && cat $ERROR_LOGFILE
	exit
fi
