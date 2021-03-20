#!/bin/bash

if [[ "$4" -eq 0 && "$3" -gt 10000 ]]
then
  set -- "${@:1:2}" "10000" "${@:4}"
fi
echo $1 $2 $3 $4 
for((i = 0; i < "$3"; i++))
do
  echo $i
done
