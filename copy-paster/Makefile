cC=g++
CFLAGS=-Wall -g -std=c99

all : cp-poll cp-epoll

cp-poll : cp-poll.c
		${CC} ${CFLAGS} cp-poll.c -o cp-poll
              
cp-epoll : cp-epoll.c
		${CC} ${CFLAGS} cp-epoll.c -o cp-epoll


clean :
		rm cp-poll cp-epoll
