echo blk > /sys/kernel/debug/tracing/current_tracer
echo 1 > /sys/kernel/debug/tracing/events/block/enable
echo 0 > /sys/kernel/debug/tracing/events/block/block_touch_buffer/enable
echo 0 > /sys/kernel/debug/tracing/events/block/block_dirty_buffer/enable

echo check
echo 1 > /sys/kernel/debug/tracing/tracing_on
sleep 2
./a.out
cat /sys/kernel/debug/tracing/trace > bt.txt
echo 0 > /sys/kernel/debug/tracing/tracing_on
