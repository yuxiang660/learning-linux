#!/bin/bash

# Populate block devices
# > cat /sys/block/loop0/dev
#   7:0 (MAJOR:MINOR)
for i in /sys/block/*/dev /sys/block/*/*/dev
do
   if [ -f $i ]
   then
      MAJOR=$(sed 's/:.*//' < $i)
      MINOR=$(sed 's/.*://' < $i)
      DEVNAME=$(echo $i | sed -e 's@/dev@@' -e 's@.*/@@')
      # same as items in "/dev" folder
      echo /dev/$DEVNAME b $MAJOR $MINOR
   fi
done

# Populate char devices
# > cat /sys/class/tty/tty0/dev
#   4:0 (MAJOR:MINOR)
for i in /sys/bus/*/devices/*/dev /sys/class/*/*/dev
do
   if [ -f $i ]
   then
      MAJOR=$(sed 's/:.*//' < $i)
      MINOR=$(sed 's/.*://' < $i)
      DEVNAME=$(echo $i | sed -e 's@/dev@@' -e 's@.*/@@')
      # same as items in "/dev" folder
      echo /dev/$DEVNAME c $MAJOR $MINOR
   fi
done
