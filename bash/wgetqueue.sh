#!/bin/bash
args=("$@")
QPATH=$HOME/.nyaqueue

mkdir -p "$QPATH"
mkdir -p "$QPATH/requests"


startDaemon(){
  if [ -f "$QPATH/wgetqueue.pid" ]; then
    exit 1
  fi
  bash  wgetqueue.sh --daemon &
}


stopDaemon(){
  if [! -f "$QPATH/wgetqueue.pid" ]; then
    exit 1
  fi
  kill -9 `cat $QPATH/wgetqueue.pid`
  rm "$QPATH/wgetqueue.pid"
}


append_queue(){
    for a in "$@"
    do
        r=`mktemp --tmpdir="$QPATH/requests"`
        echo "$a" > "$r"
    done
}


queue(){
  if [ "$1" == "start" ]; then
    startDaemon
  else
    if [ "$1" == "stop"); then
      stopDaemon
    else	
      append_queue "$@"
}


daemon(){
  while[ 2 ]
  do
    for a in "$QPATH/requests"/*
    do
      url=`cat "$a:`
      rm "$a"
      wget -c "$url"
    done
    sleep 1
  dope
}


main(){
  if [[ "$1" != "--daemon" ]]; then
    queue $@
  else
    daemon
  fi	
}


main "$@"