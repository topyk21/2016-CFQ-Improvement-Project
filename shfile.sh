#! /bin/bash
echo "bash shell start"

sudo blktrace -d /dev/sda -o - | blkparse -i - -o ./result &
#PARSE_PID=$(pgrep blkparse)
#echo $PARSE_PID

./a.out

TRACE_PID=$(pgrep blktrace)
echo $TRACE_PID
#PARSE_PID=$(pgrep blkparse)
#echo $PARSE_PID

sudo kill -15 $TRACE_PID
#sudo kill -15 $PARSE_PID

echo "blktrace finish"
ps -ef
