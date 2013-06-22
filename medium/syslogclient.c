#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PORT 8000
#define SOCK_NAME "mysocket"
#define ADDRESS "127.0.0.1"
int main(int argc, char** argv){
    int sock;
    struct sockaddr_in addr;
    struct hostent* host;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (argc < 2){
        printf("HATE");
        return 1;
    }
   // printf("%s\n",argv[1]);
    if (sock == -1){
        printf("socket() error\n");
        return 1;
    }
    addr.sin_family = AF_INET;
    host = gethostbyname(ADDRESS);
    if (host == NULL) {
        fprintf(stderr, "Unknown server\n");
        return 1;
    }
    addr.sin_addr = *(struct in_addr*)host->h_addr_list[0];
    addr.sin_port = htons(PORT);
    
    int x = sendto(sock, argv[1],strlen(argv[1]),0,(struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    //printf("%i\n",x);    
        //printf("rror sendto\n");
        //return 1;
   // }
    close(sock);
    return 0;
}
