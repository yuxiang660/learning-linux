#!/usr/bin/bash

n=4
if [ ! -z "$1" ]; then
   n=$1
fi

y=1
if [ ! -z "$2" ]; then
   y=$2
fi

x=1
if [ ! -z "$3" ]; then
   x=$3
fi

for ((i = 1; i <= $n; i++)); do
   echo "--- Test $i ---" > run.log
   ./main >> run.log
   y_match=`grep -e "y = $y" run.log | wc -l`
   x_match=`grep -e "x = $x" run.log | wc -l`
   if [[ $y_match == 1 && $x_match == 1 ]]; then
      echo "Find 'y=$y,x=$x' after $i run";
      cat run.log;
      exit;
   fi
done

echo "No 'y=$y,x=$x' after $n run"
