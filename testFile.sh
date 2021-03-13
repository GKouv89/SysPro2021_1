#!/bin/bash

generateDate () {
  day=$RANDOM
  day=$(( $day % 30 + 1 ))
  if [ "$day" -lt 10 ]
  then
    printf -v day "0%s" $day
  else
    printf -v day %s $day
  fi
  month=$RANDOM
  month=$(( $month % 12 + 1 ))
  if [ "$month" -lt 10 ]
  then
    printf -v month "0%s" $month
  else
    printf -v month %s $month
  fi
  year=$RANDOM
  year=$(( $year % 22 + 2000 ))
  printf -v year %s $year
  echo -n "${day}-${month}-${year}"
}

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
maxidlength=4
minlength=3
maxlength=12
characters=ABCDEFGHIJKLMNOPQRSTUVWXYZ  
minage=1
maxage=120

declare -A records
declare -a ids
recordsProduced=0
while [ "$recordsProduced" -lt "$3" ]
do
  # First, calculating ID
  idlength=$RANDOM
  idlength=$(( $idlength % ( $maxidlength - $minidlength + 1 ) + $minidlength ))
  
  # echo Generating number with "$idlength" digit\(s\).
  id=0
  for((j = 1; j <= "$idlength"; j++))
  do
    digit=$RANDOM
    digit=$(( $digit % 10 ))
    id=$(( $id * 10 ))
    id=$(( $id + $digit ))
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

    length=$RANDOM
    length=$(( $length % ( $maxlength - $minlength + 1) + $minlength ))
    
    name=""
    for((i = 1; i <= "$length"; i++))
    do
      printf -v character "${characters:RANDOM%26:1}"
      name+=$character
    done
    record+=$name
    record+=" "
  done
  
  country=$RANDOM
  country=$(( $country % $countriesCount ))
  record+=${countries[$country]}
  record+=" "

  age=0

  age=$RANDOM
  age=$(( $age % 120 + 1 ))
  printf -v agestr %s $age
  record+=$agestr
  ids+=($id)
  records[$id]+=$record
  
  record+=" "
  virus=$RANDOM
  virus=$(( $virus % $virusesCount ))
  record+=${viruses[$virus]}
  record+=" "
  
  die=$RANDOM
  die=$(( $die % 2 ))
  if [ "$die" -eq 0 ]
  then
    record+="NO "
    die=$RANDOM
    die=$(( $die % 100 ))
    if [ "$die" -le 30 ]
    then
      genDate=$(generateDate)
      record+=$genDate
    fi
  else
    record+="YES "
    genDate=$(generateDate)
    record+=$genDate
  fi
  

  if [ "$recordsProduced" -eq 0 ]
  then
    echo $record > inputFile.txt
  else
    echo $record >> inputFile.txt
  fi
  
  let "recordsProduced += 1"
done




