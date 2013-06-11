#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>

int EPOLL_RUN_TIMEOUT=-1;
int main (int argc, char** argv){
    struct epoll_event ev, events[argc-1];
    int epoll_fd=epoll_create(argc-1);
    for (int i = 0; i <  (argc-1)/2; i ++){
        ev.events = EPOLLIN;
        ev.data.fd=atoi(argv[2*i+1]);
        int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,atoi(argv[2*i+1]), &ev);
    }
    while (1) {
        int ndfs = epoll_wait(epoll_fd, events,argc-1,EPOLL_RUN_TIMEOUT);
        if (ndfs < 0){
            printf("error %i\n", errno);
            break;
        }
        for (int i = 0; i < ndfs/2; i++){
            if (events[2*i].events==EPOLLIN && events[2*i+1].events==EPOLLOUT){
                events[2*i].events = 0;
                events[2*i+1].events = 0;
            }
        } 
    }
    return 0;

}
