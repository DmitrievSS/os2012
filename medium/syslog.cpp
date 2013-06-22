#include<stdarg.h>
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
void syslog(int priority, const char *format, ...){
    va_list ap;
    va_start(ap, format);
    char* i;
    char tmp[1]={' '};
    for ( i = tmp ; strlen(i) > 0; i = va_arg(ap, char*)){
        int sock;
        struct sockaddr_in addr;
        struct hostent* host;
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        // printf("%s\n",argv[1]);
        if (sock == -1){
            printf("socket() error\n");
        }
        addr.sin_family = AF_INET;
        host = gethostbyname(ADDRESS);
        if (host == NULL) {
            fprintf(stderr, "Unknown server\n");
        }
        addr.sin_addr = *(struct in_addr*)host->h_addr_list[0];
        addr.sin_port = htons(PORT);
        int x = sendto(sock, i,strlen(i),0,(struct sockaddr *) &addr, sizeof(struct sockaddr_in));
        close (sock);
    }
}
void openlog(const char *ident, int option, int facility){}
void closelog(void){}
