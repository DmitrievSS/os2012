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
#include <sys/stat.h>
#include <sys/sendfile.h>


int EPOLL_RUN_TIMEOUT=-1;
int main (int argc, char** argv){
    struct epoll_event ev[argc-1], events[argc-1];
    int epoll_fd=epoll_create(argc-1);
    for (int i = 0; i <  (argc-1)/2; i ++){
        ev[2*i].events = EPOLLIN;
        int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,atoi(argv[2*i+1]), &ev[2*i]);
        ev[2*i+1].events = EPOLLOUT;
        res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,atoi(argv[2*i+2]), &ev[2*i+1]);
    }
    int n = (argc-1)/2;
    while (n>0) {
        int ndfs = epoll_wait(epoll_fd, events,argc-1,EPOLL_RUN_TIMEOUT+2);
        if (ndfs!=0) printf("ndfs - %i\n", ndfs);
        if (ndfs < 0){
            printf("error %i\n", errno);
            break;
        }
        for (int i = 0; i < ndfs/2; i++){
            if (events[2*i].events==EPOLLIN && events[2*i+1].events==EPOLLOUT){
                struct stat* buffer = (struct stat*)malloc(sizeof(struct stat));
                fstat(events[2*i].data.fd, buffer);
                sendfile(events[2*i+1].data.fd,events[2*i].data.fd,0,buffer->st_size);   
                events[2*i].events = 0;
                events[2*i+1].events = 0;
                n--;
            }
        } 
    }
    return 0;
}
