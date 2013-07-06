#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <poll.h>
#include <sys/stat.h>
#include <errno.h>
const int LONG = 1000;
int main(int argc, char** argv){
  int n = (argc-1) / 2;
  struct pollfd* fds= (struct pollfd*) malloc(argc * sizeof(struct pollfd));
  for (int i = 0; i < n; i++){
    fds[2*i].fd = atoi(argv[2*i+1]);
    fds[2*i].events = POLLIN;
    fds[2*i+1].fd  = atoi(argv[2*i+2]);
    fds[2*i+1].events = POLLOUT;        
  }
  while (n > 0){
    int ready = poll(fds, argc-1, -1);
    printf("ready %i\n",ready);
    if (ready <  0){
      printf("error %i", errno);
      break;
    }
    if (ready == 0){
      printf ("timeout");
      break;
    }
    
    for (int i = 0; i <= (argc-1)/2; i++){
        if ((fds[2*i].revents ==  POLLIN) && (fds[2*i+1].revents == POLLOUT)){
        char *buf =(char*) malloc(LONG);
        int tmp = 1;
        if ((tmp=read(fds[2*i].fd,buf,LONG)) > 0){
                write(fds[2*i+1].fd,buf,tmp);
                printf("tmp %i\n",tmp);
        } 
        else{
            fds[2*i].events = 0;
            fds[2*i+1].events = 0;
            n--;
        }
      }
    }
  }
  return 0;      
}
