#!/usr/bin/bash

n=4

if [ ! -z "$1" ]; then
   n=$1
fi

for ((i = 1; i <= $n; i++)); do
   echo "--- Test $i ---" > run.log
   ./main >> run.log
   y_1=`grep -e 'y = 1' run.log | wc -l`
   x_0=`grep -e 'x = 0' run.log | wc -l`
   if [[ $y_1 == 1 && $x_0 == 1 ]]; then
      echo "Find 'y=1,x=0' after $i run";
      cat run.log;
      exit;
   fi
done

echo "No 'y=1,x=0' after $n run"
