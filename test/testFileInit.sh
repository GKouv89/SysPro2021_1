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

# echo Choosing 10 random countries.
# for ((i = 1; i <= 10; i++))
# do
  # number=$RANDOM
  # let "number %= $countriesCount"
  # echo ${countries[$number]}
# done

# echo There are $countriesCount countries in $2
# echo There are $virusesCount viruses in $1

# myIDArray=(12 1234)
# if [[ ${myIDArray[*]} =~ (^|[[:space:]])1($|[[:space:]]) ]]
# then
  # echo ID array contains 1.
# else
  # echo ID array doesn\'t contain 1.
# fi 

# if [[ ${myIDArray[*]} =~ (^|[[:space:]])12($|[[:space:]]) ]]
# then
  # echo ID array contains 12.
# else
  # echo ID array doesn\'t contain 12.
# fi 

# if [[ ${myIDArray[*]} =~ (^|[[:space:]])123($|[[:space:]]) ]]
# then
  # echo ID array contains 123.
# else
  # echo ID array doesn\'t contain 123.
# fi 

# if [[ ${myIDArray[*]} =~ (^|[[:space:]])1234($|[[:space:]]) ]]
# then
  # echo ID array contains 1234.
# else
  # echo ID array doesn\'t contain 1234.
# fi

# i=4
# while [ "$i" -gt 0 ]
# do
  # echo Generating number with "$i" digit\(s\).
  # id=0
  # for((j = 1; j <= i; j++))
  # do
    # digit=$RANDOM
    # let "digit %= 10"
    # let "id *= 10"
    # let "id = id + digit"
  # done
  # echo Number is: $id.
  # let "i -= 1"
# done 

# asc=67

# printf -v character "\x$(printf %x $asc)"
# echo $character

minlength=3
maxlength=12
length=0
modulo=`expr $maxlength - $minlength + 1`
# while [ "$length" -lt "$minlength" ]
# do
	# length=$RANDOM
	# let "length %= $maxlength"
# done
# echo Random generated length is "$length".

for((i = 1; i <= 5; i++))
do
  length=$RANDOM
  length=$(( $length % ( $maxlength - $minlength + 1 ) + $minlength ))
  echo Random generated length is $length.
done

# record=""
# minlength=3
# maxlength=12  
# for((times = 1; times <= 2; times++))
# do

  # length=0
  # while [ "$length" -lt "$minlength" ]
  # do
    # length=$RANDOM
    # let "length %= $maxlength"
  # done

  # name=""
  # for((i = 1; i <= length; i++))
  # do
    # asc=0
    # minasc=65
    # maxasc=90
    # while [ "$asc" -lt "$minasc" ]
    # do
      # asc=$RANDOM
      # let "asc %= $maxasc"
    # done
    # printf -v character "\x$(printf %x $asc)"
    # name+=$character
  # done
  # record+=$name
  # record+=" "
# done

# country=$RANDOM
# let "country %= $countriesCount"
# record+=${countries[$country]}
# record+=" "

# age=0
# minage=0
# maxage=121
# while [ $age -le $minage ]
# do
  # age=$RANDOM
  # let "age %= $maxage"
# done
# printf -v agestr %s $age
# record+=$agestr
# echo Record is $record



