#!/bin/bash

if [ $# -ne 4 ]
then
  echo Invalid number of arguments.
  exit 1
fi

if [ ! -e $1 ]
then
  echo File $1 does not exist.
  exit 1
elif [ ! -f $1 ]
then
  echo File $1 is not a regular file.
  exit 1
fi

if [ ! -e $2 ]
then
  echo File $2 does not exist.
  exit 1
elif [ ! -f $2 ]
then
  echo File $2 is not a regular file.
  exit 1
fi

mapfile -t viruses < $1
mapfile -t countries < $2

countriesCount=${#countries[*]}
virusesCount=${#viruses[*]}
minidlength=1
maxidlength=5
minlength=3
maxlength=13  
minage=1
maxage=121

declare -A records
declare -a ids
recordsProduced=0
while [ "$recordsProduced" -lt "$3" ]
do
  # First, calculating ID
  idlength=0
  while [ "$idlength" -lt "$minidlength" ]
  do
    idlength=$RANDOM
    let "idlength %= $maxidlength"
  done  
  
  # echo Generating number with "$idlength" digit\(s\).
  id=0
  for((j = 1; j <= "$idlength"; j++))
  do
    digit=$RANDOM
    let "digit %= 10"
    let "id *= 10"
    let "id = id + digit"
  done
  # echo Number is: $id.
  
  if [ "$4" -eq 0 ]
  then
    if [[ ${ids[*]} =~ $id ]]
    then
      # echo ID already exists. Moving on to next rep.
      continue
    fi
  fi

  record="$id "
  for((times = 1; times <= 2; times++))
  do

    length=0
    while [ "$length" -lt "$minlength" ]
    do
      length=$RANDOM
      let "length %= $maxlength"
    done

    name=""
    for((i = 1; i <= length; i++))
    do
      asc=0
      minasc=65
      maxasc=90
      while [ "$asc" -lt "$minasc" ]
      do
        asc=$RANDOM
        let "asc %= $maxasc"
      done
      printf -v character "\x$(printf %x $asc)"
      name+=$character
    done
    record+=$name
    record+=" "
  done

  country=$RANDOM
  let "country %= $countriesCount"
  record+=${countries[$country]}
  record+=" "

  age=0

  while [ $age -lt $minage ]
  do
    age=$RANDOM
    let "age %= $maxage"
  done
  printf -v agestr %s $age
  record+=$agestr
  if [ "$recordsProduced" -eq 0 ]
  then
    echo $record > inputFile.txt
  else
    echo $record >> inputFile.txt
  fi
  ids+=($id)
  records[$id]+=$record
  let "recordsProduced += 1"
done




