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

record=""
minlength=3
maxlength=12  
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
minage=0
maxage=121
while [ $age -le $minage ]
do
  age=$RANDOM
  let "age %= $maxage"
done
printf -v agestr %s $age
record+=$agestr
echo Record is $record



