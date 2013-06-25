#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <map>
#include <string>
#define BUF_LEN 256
#define TABLE_SIZE 1000
#define ADDRESS "127.0.0.1"
using namespace std;
struct Node{
    int key;
    char* value;
    int collision;//collision = -1 already changed = 1 ready to change = 0
};
struct Node parseBuf(char* str){
    char*    pch = strtok(str," ");
    struct Node tmp;
    if (pch==NULL) return tmp;
    tmp.key = atoi(pch);
    pch = strtok (NULL, " ");
    if (pch==NULL) return tmp;
    tmp.value = pch;
    pch = strtok (NULL, " ");
    if (pch==NULL) return tmp ;
    tmp.collision = atoi(pch);
    return tmp;
}
int main(int argc, char** argv){//
    int n = argc;
    int sock;
    int sockout[n];
    struct hostent* host = gethostbyname(ADDRESS);
    struct sockaddr_in saddr;
    struct sockaddr_in saddrout[n];
    char* buf;
    int count; 
    struct Node table[TABLE_SIZE]; 
    struct pollfd* fds= (struct pollfd*) malloc(argc * sizeof(struct pollfd));
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
    saddr.sin_port = htons(atoi(argv[1]));
    saddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr * ) &saddr, sizeof(saddr))==-1){
        printf("bind() error??   %s\n", strerror(errno));
        return 1;
    } 
    fds[0].fd = sock;
    fds[0].events = POLLIN;
    for (int i = 0; i < n-1; i++){
        sockout[i] = socket (AF_INET, SOCK_DGRAM, 0);   
        if (i ==( n-2)) continue;
        saddrout[i].sin_family = AF_INET;
        saddrout[i].sin_port = htons(atoi(argv[i+2]));
        saddrout[i].sin_addr = *(struct in_addr*)host->h_addr_list[0];
    }
    fds[1].fd = 0;
    fds[1].events = POLLIN;
       
    while(1){
        int ready = poll(fds, argc-1, -1);
        if (ready <  0){
            printf("error %i", errno);
            break;
        }
        if (ready == 0){
            printf ("timeout");
            break;
        }
        if (fds[0].revents ==  POLLIN){
             if((count = recvfrom(fds[0].fd, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){
                printf("recvdrom() error: %s\n",strerror(errno));
             }
             struct Node tmp = parseBuf(buf);
             printf("key : %i\n value : %s\n",tmp.key, tmp.value);
             for (int j = 0; j <n-1; j++){   
                 sendto(sockout[j], tmp.value,strlen(tmp.value),0,(struct sockaddr *) &saddrout[j], sizeof(struct sockaddr_in));
             }
             if (table[tmp.key].collision == 1)
                table[tmp.key].collision = -1;
             if (table[tmp.key].collision == 0){
                table[tmp.key].key = tmp.key;
                table[tmp.key].value = tmp.value ;    
                table[tmp.key].collision = tmp.collision;
             }
             n--;
        }
        
        if (fds[1].revents==POLLIN){
            if(!(read(0,buf,100))){
                printf("recvdrom() error^ %s\n%s\n",strerror(errno),buf);
            }
            struct Node tmp = parseBuf(buf);
            for (int j = 0; j <n-1; j++){   
                sendto(sockout[j], tmp.value,strlen(tmp.value),0,(struct sockaddr *) &saddrout[j], sizeof(struct sockaddr_in));
            }
        }
    } 
    return 0;
}
