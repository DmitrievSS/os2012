#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <poll.h>
#include <sys/stat.h>
#include <errno.h>
int main(int argc, char** argv){
  int n = (argc-1) / 2;
  struct pollfd* fds= (struct pollfd*) malloc(argc * sizeof(struct pollfd));
  for (int i = 0; i < n; i++){
    fds[2*i].fd = atoi(argv[2*i+1]);
    fds[2*i].events = POLLIN;
    fds[2*i+1].fd  = atoi(argv[2*i+2]);
    printf("%i\n",fds[2*i].events);
    fds[2*i+1].events = POLLOUT;        
    printf("%i\n",fds[2*i+1].events);
  }
  for (int i = 1; i <= (argc-1)/2; i++){
    printf("i - %i fds: %i %i\n",i,fds[2*i].revents,fds[2*i-1].revents);
  }
  while (n > 0){
    int ready = poll(fds, argc-1, -1);
    //printf("%i\n",ready);
    if (ready <  0){
      printf("error %i", errno);
      break;
    }
    if (ready == 0){
      printf ("timeout");
      break;
    }
    if (ready == 1){
      printf("HUYPIZDA");
    }
    printf(" fds: %i %i\n",fds[0].revents,fds[1].revents);
    for (int i = 0; i <= (argc-1)/2; i++){
    //printf("i - %i fds: %i %i\n",i,fds[2*i].revents,fds[2*i-1].revents);
        if ((fds[2*i].revents ==  POLLIN) && (fds[2*i+1].revents == POLLOUT)){
        struct stat* buffer = (struct stat*)malloc(sizeof(struct stat));  
        fstat(fds[2*i].fd,buffer);
        printf("%i\n",i);
        sendfile(fds[2*i+1].fd, fds[2*i].fd, 0, buffer->st_size);
        fds[2*i].events = 0;
        fds[2*i+1].events = 0;
        n--;
      }
    }
  }
  return 0;      
}
