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
# dupesRequired=$( echo "scale=3; $3*20/100" | bc )
printf -v dupesRequired %.0f "$dupesRequired"

# In case we have too few records, we ensure that at least one will be a duplicate
# of another one
if [ "$dupesRequired" -eq 0 ]
then
  dupesRequired=1
fi

# dupesRequired=$(( $3 - $dupesRequired + 1))
# echo After reaching the $dupesRequired record, we will force creating duplicates.

while [ "$recordsProduced" -lt "$3" ]
do
  # First, calculating ID if duplicatesNotAllowed or if duplicatesAllowed 
  # and we haven't reached the point of duplicate production yet
  #if [ "$4" -eq 0 ] || [[ "$4" -eq 1 && "$recordsProduced" -lt "$dupesRequired" ]]
  # if [ "$4" -eq 0 ] || [ "$4" -eq 1 ]
  # then
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
    
    if [ "$4" -eq 0 ]
    then
      if [[ ${ids[*]} =~ $id ]]
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
        # echo DUPLICATE.
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
  # else
    # # In this case, we have reached the last few (compared to file size lines)
    # # and here we will pick one id from the id array and generate another record
    # # for it. While for larger files we do not necessarily need this step, it increases the
    # # chane that there will be duplicate entries for the same citizen AND virus
    # # because of the small count of viruses available.
    # echo So far, ${#ids[@]} generated
    # index=$RANDOM
    # index=$(( $index % ${#ids[@]} ))
  # fi

  if [ "$recordsProduced" -eq 0 ]
  then
    echo $record > inputFile.txt
  else
    echo $record >> inputFile.txt
  fi
  
  let "recordsProduced += 1"
done
