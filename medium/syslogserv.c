#include<unistd.h>
#include<stdio.h>
#include<netdb.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#define PORT 8000 
#define BUF_LEN 256
int main (int argc, char** argv){
    if (fork()) {
        return 0;
    }
    if (setsid() < 0) {
        printf( "setsid() error: %s\n", strerror(errno));
        return 1;
    }
    int log ;
    if ( (log = open("log.txt", O_RDWR|O_CREAT))< 0){
        printf( "open() error: %s\n", strerror(errno));
        return 1;
    }
    int sock;
    struct sockaddr_in saddr;
    char* buf;
    int count;
    sock = socket (AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        printf( "socket() error: %s\n",strerror(errno));
        return 1;
    }
    buf = (char *) malloc(BUF_LEN);
    if (buf == NULL){
        printf("malloc() error: %s\n", strerror(errno));
        return 1;
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr * ) &saddr, sizeof(saddr))==-1){
        printf("bind() error%s\n",strerror(errno));
        return 1;
    }
    while (1){
        if((count = recvfrom(sock, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){
            printf("recvdrom() error^ %s\n",strerror(errno));
        }
        write(log,buf,count);
        write(log, "\n",1 );   
        if (!strcmp (buf, "exit")) break; 
    }   
    free(buf);
    close(sock);
    close(log);
    return 0;
}
