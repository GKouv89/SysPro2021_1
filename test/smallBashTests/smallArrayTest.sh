#!/bin/bash

ids=(1 2 4 6 7 10)
# echo "ids contain ${#ids[*]} elements"
for((j = 0; j <= 10; j++))
do
  # echo $j
  if [[ ${ids[@]} =~ (^|[[:space:]])"$j"($|[[:space:]]) ]]
  then
    echo "$j" found.
    continue
  else
    # remainingids+=($j)
    echo "$j" has not been generated yet.
  fi      
done
