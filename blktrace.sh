#! /bin/bash
echo "bash shell start"

whoami
#sudo blktrace -d /dev/sda -a queue -a issue -a complete -o - | blkparse -i - > result.txt & #-o ./result &
sudo blktrace -d /dev/sda -o - | blkparse -i - > result.txt & #-o ./result &
#PARSE_PID=$(pgrep blkparse)
#echo $PARSE_PID
sleep 2
./a.out

TRACE_PID=$(pgrep blktrace)
echo $TRACE_PID
#PARSE_PID=$(pgrep blkparse)
#echo $PARSE_PID

kill -15 $TRACE_PID
#sudo kill -15 $PARSE_PID

rm ./Thing1.txt
rm /usr/src/Thing1.txt
echo "blktrace finish"
#ps -ef
