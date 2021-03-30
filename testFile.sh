#!/bin/bash

generateRestOfRecord () {
  restOfRecord=""
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
    restOfRecord+="$name "
  done
  
  country=$RANDOM
  country=$(( $country % $countriesCount ))
  country=${countries[$country]}
  restOfRecord+="$country "

  age=$RANDOM
  age=$(( $age % 120 + 1 ))
  printf -v agestr %s $age
  restOfRecord+="$agestr "
}

generateDate () {
  fullDate=""
  day=$RANDOM
  day=$(( $day % 30 + 1 ))
  if [ "$day" -lt 10 ]
  then
    printf -v day "0%s" $day
  else
    printf -v day %s $day
  fi
  fullDate+="$day-"
  month=$RANDOM
  month=$(( $month % 12 + 1 ))
  if [ "$month" -lt 10 ]
  then
    printf -v month "0%s" $month
  else
    printf -v month %s $month
  fi
  fullDate+="$month-"
  year=$RANDOM
  year=$(( $year % 22 + 2000 ))
  printf -v year %s $year
  fullDate+="$year"
}

generateVaccination () {
  virus=$RANDOM
  virus=$(( $virus % $virusesCount ))
  vac="${viruses[$virus]} "
  
  die=$RANDOM
  die=$(( $die % 2 ))
  if [ "$die" -eq 0 ]
  then
    vac+="NO "
    die=$RANDOM
    die=$(( $die % 100 ))
    if [ "$die" -le 30 ]
    then
      generateDate
      vac+="$fullDate"
    fi
  else
    vac+="YES "
    generateDate
    vac+="$fullDate"
  fi
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

if [[ "$4" -eq 0 && "$3" -gt 10000 ]]
then
  set -- "${@:1:2}" "10000" "${@:4}"
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

# When a record is generated for a specific ID, it is stored in this associative array.
# In the version with duplicates, the moment the ID is generated and found in the indices of the array,
# the record is ready to be printed.
declare -A records
# Used for the version without duplicates. If a generated ID is in this array, no record is generated, we
# skip to id generation again.
declare -a ids
# Used for the version without duplicates. After 9000 records, the remaining 1000 that have not been generated
# are found and stored here, and the ID is chosen from these from that point forward.
declare -a remainingids
recordsProduced=0

while [ "$recordsProduced" -lt "$3" ]
do
  # if we are reaching the end of the count of the domain of possible generatable IDs
  # we seek to find which ones have NOT been generated yet and choose from them.
  # So, an array of the 1000 IDs not generated yet is created and each time from 
  # now on, the id is chosen from the array. Could have been chosen again, but
  # it is much quicker to pick another element from the array rather than just generating
  # a number in the domain of applicable numbers again.
  
  if [[ "$4" -eq 0 && "$recordsProduced" -eq 9000 && ${#remainingids[@]} -eq 0 ]]
  then
    for((j = 0; j < 10000; j++))
    do
      if [[ ${ids[*]} =~ (^|[[:space:]])"$j"($|[[:space:]]) ]]
      then
        continue
      else
        remainingids+=($j)
      fi      
    done
  fi
  
  # Now, actually calculating ID if duplicatesNotAllowed or if duplicatesAllowed 
  # and we haven't reached the point of duplicate production yet
  
  if [ "$4" -eq 1 ] || [[ "$4" -eq 0 && "$recordsProduced" -lt 9000 ]]
  then
    idlength=$RANDOM
    idlength=$(( $idlength % ( $maxidlength - $minidlength + 1 ) + $minidlength ))
    
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

  if [ "$4" -eq 0 ]
  then
    if [[ ${ids[*]} =~ (^|[[:space:]])"$id"($|[[:space:]]) ]]
    then
      continue
    fi
    record="$id "
    generateRestOfRecord
    record+="$restOfRecord"
    ids+=($id)
    records[$id]+=$record
    generateVaccination
    record+="$vac"
  else
    if [[ -v "records[$id]" ]]
    then
      record=${records[$id]}
    else
      record="$id "
      generateRestOfRecord
      record+="$restOfRecord"
      records[$id]+=$record
      ids+=($id)
    fi
    generateVaccination
    record+="$vac"
  fi
  
  if [ "$recordsProduced" -eq 0 ]
  then
    echo $record > inputFile.txt
  else
    echo $record >> inputFile.txt
  fi
  
  let "recordsProduced += 1"
done
