#!/usr/bin/bash

n=4

if [ ! -z "$1" ]; then
   n=$1
fi

for ((i = 1; i <= $n; i++)); do
   echo "--- Test $i ---"
   ./main
done

echo "Done $n Run"
