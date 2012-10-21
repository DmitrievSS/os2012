#!/bin/bash
args=("$@")
type="-type"
name="-name"	
NAME=""
TYPE=""
DIR=""

askMe(){
if [ "-$TYPE" "$1" ]; then
  if [[ "$1" == $NAME ]]; then
    echo "$1"
  fi
fi
}

find(){
echo "$1"
for filename in "$1"/*
do
  askMe ${filename##*/}
  if [ -d "$filename" ]; then
    echo "$NAME"
    echo "$filename"
    find "$filename"
  fi
done
}

parse(){

if [ $# == 5 ]; then
  DIR="${args[4]}"
  if [ args[0]==type ]; then
    if [ args[2]==name ]; then     
      NAME="${args[3]}"
      TYPE="${args[1]}"
    fi
  else
    if [ args[2]==type ]; then
      if [ args[0]==name ]; then
        NAME="${args[1]}"
        TYPE="${args[3]}"
      fi
    fi
  fi
fi     
}
main(){
  parse "$@"
  find "$DIR"	
}

main "$@"



