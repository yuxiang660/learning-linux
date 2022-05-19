#!/usr/bin/bash

n=1

if [ ! -z "$1" ]; then
   n=$1
fi

for ((i = 1; i <= $n; i++)); do
   #echo "--- Test $i ---"
   ./main > run.log
   match_num=`grep -e '= 1' run.log | wc -l`
   if [[ $match_num == 2 ]]; then
      echo "Find two one values after $i run";
      cat run.log;
      exit;
   fi
done

echo "Done but no two one values, need to run more times"
