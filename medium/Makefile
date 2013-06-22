cC=g++
CFLAGS=-Wall -g -std=c99

all : syslogserv syslogclient shagal
shagal :  shagal.c
			${CC} ${CFLAGS} shagal.c -o shagal

syslogserv :  syslogserv.c
			${CC} ${CFLAGS} syslogserv.c -o syslogserv
syslogclient : syslogclient.c
			${CC} ${CFLAGS} syslogclient.c -o syslogclient
clean :
			rm syslogserv syslogclient shagal
