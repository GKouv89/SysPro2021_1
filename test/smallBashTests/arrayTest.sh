#!/bin/bash

declare -a ids
declare -a remainingids
# At first, let's say we wish to generate only id's from 1 to 100
for((i = 0; i < 10000; ))
do
  echo i is $i.
  if [ "$i" -lt 9000 ]
  then
    idlength=$RANDOM
    idlength=$(( $idlength % 4 + 1 ))

    id=0
    for((j = 1; j <= "$idlength"; j++))
    do
      digit=$RANDOM
      digit=$(( $digit % 10 ))
      id=$(( $id * 10 ))
      id=$(( $id + $digit ))
    done  
  else
    index=$RANDOM
    index=$(( $index % 1000 ))
    id=${remainingids[$index]}
  fi
  
  if [[ ${ids[*]} =~ (^|[[:space:]])"$id"($|[[:space:]]) ]]
  # if [[ ${ids[@]} =~ "$id" ]]
  then
    echo $id REJECTED. >> acceptReject.txt
    continue
  else
    echo $id ACCEPTED. >> acceptReject.txt
    ids+=($id)
    echo $id >> ids.txt
    i=$(( i + 1 ))
    if [ "$i" -eq 9000 ]
    then
      echo Array of ids contains ${#ids[*]} elements
      for((j = 0; j < 10000; j++))
      do
        if [[ ${ids[*]} =~ (^|[[:space:]])"$j"($|[[:space:]]) ]]
        then
          continue
        else
          remainingids+=($j)
          echo $j has not been generated yet.
        fi      
      done
    fi
  fi
done
